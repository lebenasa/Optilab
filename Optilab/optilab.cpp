#include "stdafx.h"
#include "optilab.h"
#include <math.h>
#include <QQmlContext>
#include <QDir>
#include <QtConcurrent>
#include <QFuture>
#include <QFile>

#include "singleimage.h"
#include "multiimage.h"
#include "videopreview.h"

#include "steppercontrol.h"
#include "stepperdebug.h"

#include "interpreter.h"

QSize screenSize;
cv::VideoWriter* recorder;
cv::Mat currentFrame;
enum RecordingStatus {
	Stopped, Paused, Recording
} recordingStatus;

Optilab::Optilab(QWidget *parent)
	: QMainWindow(parent), maxGamma(150), maxContrast(255),
	maxSaturation(255), maxAETarget(200), maxAEGain(48), maxRGBGain(200),
	isRibbonVisible(true), ongoingCapture(false)
{
	ui.setupUi(this);
	//setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint |
	//	Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
	setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
	screenSize = ui.screen->size();
	statusLabel = new QLabel;
	statusBar()->addWidget(statusLabel);
	timer = new QTimer(this);
	timer->setTimerType(Qt::PreciseTimer);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
	timerLabel = new QLabel;
	statusBar()->addPermanentWidget(timerLabel);
	serialCapStop = new QPushButton(this);
	serialCapStop->setObjectName("serialCapStop");
	statusBar()->addPermanentWidget(serialCapStop);
	serialCapStop->hide();
	connect(serialCapStop, &QPushButton::clicked, [this]() {
		QMutex mutex;
		QMutexLocker lock(&mutex);
		ongoingCapture = false;
	});
	scProgress = new QProgressBar(this);
	scProgress->setMaximumWidth(200);
	scProgress->setMaximumHeight(15);
	statusBar()->addPermanentWidget(scProgress);
	scProgress->setTextVisible(false);
	scProgress->hide();

	//Initialize Ribbon:
	ribbon = new QQuickView(QUrl("qrc:/Ribbon/QuickCamRibbon.qml"));
	ribbon->setResizeMode(QQuickView::SizeRootObjectToView);
	control = ribbon->rootObject();
	auto context = ribbon->rootContext();
	context->setContextProperty("camera", this);
	auto w = QWidget::createWindowContainer(ribbon, ui.ribbon);
	auto gridLayout = new QGridLayout(ui.ribbon);
	gridLayout->setSpacing(0);
	gridLayout->setContentsMargins(0, 0, 0, 0);
	gridLayout->setObjectName(QStringLiteral("ribbonLayout"));
	gridLayout->addWidget(w, 0, 0, 1, 1);
	connect(this, SIGNAL(cameraInitialized()), control, SLOT(initialize()));

	shutter = new QMediaPlayer(this, QMediaPlayer::LowLatency);
	shutter->setMedia(QUrl("qrc:/Ribbon/sound/camera-shutter.wav"));
	shutter->setVolume(50);

	//Start the camera:
	setStreamResolution(ResolutionLevel::High);

	//Set working directory:
	QString tempPath = QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0) + "/Optilab/";
	QDir workdir(tempPath);
	if (workdir.exists())
		QDir::setCurrent(tempPath);
	else {
		workdir.mkpath(workdir.path());
		QDir::setCurrent(workdir.path());
	}
	auto list = workdir.entryList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	for (auto file : list)
		workdir.remove(file);
	//qDebug() << QDir::currentPath();

	connect(this, &Optilab::serialCaptureFinished, this, &Optilab::saveSerialCapture);
	connect(this, &Optilab::requestRestoreResolution, this, &Optilab::restoreResolution);
	connect(this, &Optilab::serialCaptureFinished, [this](){ ongoingCapture = false; });
	connect(this, SIGNAL(serialCaptureFinished()), control, SLOT(enableCapture()));
	connect(this, SIGNAL(singleCaptureFinished()), control, SLOT(enableCapture()));
	connect(this, &Optilab::progressUp, this, &Optilab::updateProgressBar);

	//OnScreen Control
	stepperControl = new StepperControl(this);
	//stepperControl->show();
	//addDockWidget(Qt::RightDockWidgetArea, stepperControl);
	stepperDebug = new StepperDebug(this);
	stepperDebug->show();

	//Interpreter (for evaluation only)
	interpreter = new Interpreter(this);
	interpreter->show();
}

Optilab::~Optilab()
{
	CameraStop();
	CameraUnInit();
	if (ribbon)	delete ribbon;
	if (recorder) {
		if (recorder->isOpened())
			recorder->release();
	}
}

int CALLBACK SnapThreadCallback(BYTE* pBuffer) {
	BYTE *pBmp24 = CameraISP(pBuffer);
	int W, H;
	CameraGetImageSize(&W, &H);
	cv::Mat frame = cv::Mat(H, W, CV_8UC3, pBmp24);
	if (pBmp24)
		CameraDisplayRGB24(frame.data);

	//if (recordingStatus == Recording && recorder) {
	QMutex mutex;
	mutex.lock();
	cv::flip(frame, currentFrame, 0);
	mutex.unlock();
	//}
	return TRUE;
}

void Optilab::resizeEvent(QResizeEvent* event) {
	QWidget::resizeEvent(event);
	calcAspectRatio();
}

void Optilab::calcAspectRatio() {
	screenSize = (isRibbonVisible) ? ui.screen->size() : QSize(ui.screen->width(), ui.screen->height() + ui.ribbon->minimumHeight());
	int srcWidth, srcHeight;
	CameraGetImageSize(&srcWidth, &srcHeight);
	//auto screenSize = ui.screen->size();
	double aspectRatio = 1.0 * srcWidth / srcHeight;
	double screenRatio = 1.0 * screenSize.width() / screenSize.height();
	int frameWidth, frameHeight;
	int hOffset, vOffset;
	hOffset = vOffset = 0;
	if (aspectRatio > screenRatio) {
		frameWidth = screenSize.width();
		frameHeight = 1.0 * screenSize.width() / aspectRatio;
		vOffset = (screenSize.height() - frameHeight) / 2;
	}
	else {
		frameWidth = 1.0 * screenSize.height() * aspectRatio;
		frameHeight = screenSize.height();
		hOffset = (screenSize.width() - frameWidth) / 2;
	}
	ui.screen->layout()->setContentsMargins(hOffset, vOffset, hOffset, vOffset);
}

int Optilab::getStreamResolution() {
	return streamResolution;
}

void Optilab::setStreamResolution(int res) {
	auto status = CameraStop();
	status = CameraUnInit();
#ifdef DICAMAPI_H
	status = CameraInit(1, R1280_1024, (HWND)ui.screen->winId(), 0, this);
#else
	status = CameraInit(SnapThreadCallback, (DS_RESOLUTION)(res), (HWND)ui.screen->screenHandle(), 1, (HWND)this->winId());
#endif
	if (status == STATUS_OK) {
		streamResolution = res;
		status = CameraPlay();
		CameraSetB2RGBMode(DS_B2RGB_MODE::B2RGB_MODE_LINE);
		CameraSetColorEnhancement(TRUE);
		//CameraSetLightFrquency(DS_LIGHT_FREQUENCY::LIGHT_FREQUENCY_60HZ);
		CameraSetFrameSpeed(DS_FRAME_SPEED::FRAME_SPEED_NORMAL);
		emit cameraInitialized();
		if (res == 0)
			statusLabel->setText("High Quality Streaming");
		else
			statusLabel->setText("High Speed Streaming");
	}
}

void Optilab::restartCamera() {

}

void Optilab::singleCapture(int resolution) {
	LPCTSTR filename = L"Z";
	shutter->play();
	auto status = CameraCaptureFile(filename, FILE_JPG, 100, (DS_RESOLUTION)resolution);
	//QProcess* explore = new QProcess(this);
	QString appPath = QDir::currentPath();
	//qDebug() << appPath;
	QString img = appPath + "/swrdaonl.jpg";
	//auto args = appPath.replace('/', "\\");
	//explore->start(QString("explorer.exe \"%1\"").arg(args));
	if (QFile::exists(img))
		QFile::remove(img);
	QFile::copy(appPath + "/Z.jpg", img);
	SingleImage dialog(img, this);
	dialog.exec();
	emit singleCaptureFinished();
}

void Optilab::timedCapture(int interval, int duration, int resolution) {
	if (interval > duration) {
		QMessageBox::warning(this, "Invalid Time Settings", "Interval is larger than duration.");
		return;
	}
	countedCapture(interval, duration / interval, resolution);
}

QFuture<void> status;
void Optilab::countedCapture(int interval, int framecount, int resolution) {
	if (ongoingCapture) return;
	ongoingCapture = true;
	//if (streamResolution != resolution) {
	//	auto status = CameraStop();
	//	status = CameraUnInit();
	//	status = CameraInit(SnapThreadCallback, (DS_RESOLUTION)resolution, (HWND)ui.screen->winId(), 1, NULL);
	//	if (status == STATUS_OK) CameraPlay();
	//	else return;
	//}

	scProgress->setMaximum(framecount);
	scProgress->setMinimum(0);
	scProgress->show();
	serialCapStop->show();

	auto func = [this](int itv, int fc, int rs) { serialCaptureInternal(itv, fc, rs); };
	status = QtConcurrent::run(func, interval, framecount, resolution);
}

void Optilab::serialCaptureInternal(int interval, int framecount, int resolution) {
	serialResult.clear();
	for (int i = 0; i < framecount; ++i) {
		QMutex lock;
		lock.lock();
		if (!ongoingCapture) break;
		lock.unlock();
		QString tempPath = "S";
		LPCTSTR filename = tempPath.utf16();
		shutter->play();
		emit progressUp();
		//auto status = CameraCaptureFile(filename, FILE_JPG, 100, (DS_RESOLUTION)resolution);
		cv::Mat img;
		cv::cvtColor(currentFrame, img, cv::COLOR_BGR2RGB);
		QImage im(img.data, img.cols, img.rows, QImage::Format_RGB888);
		QString path = QDir::currentPath();
		//QString result = path + "/S.jpg";
		//if (QFile::exists(result)) {
		//	QString rname = path + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + ".jpg";
		//	bool res = QFile::copy(result, rname);
		//	serialResult.append(rname);
		//}
		QString rname = path + "/" + QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss-zzz") + ".jpg";
		bool res = im.save(rname);
		serialResult.append(rname);
		std::this_thread::sleep_for(std::chrono::seconds(interval));
	}
	//auto explore = new QProcess(this);
	//auto workdir = QDir::currentPath();
	//workdir = workdir.replace('/', '\\');
	//explore->start(QString("explorer.exe \"%1\"").arg(workdir));
	//if (streamResolution != resolution)
	//	emit requestRestoreResolution();
	emit serialCaptureFinished();
}

void Optilab::updateProgressBar() {
	scProgress->setValue(scProgress->value() + 1);
}

void Optilab::saveSerialCapture() {
	serialCapStop->hide();
	scProgress->setRange(0, 0);
	MultiImage dialog(serialResult, this);
	dialog.exec();
	scProgress->hide();
	scProgress->setValue(0);
}

void Optilab::restoreResolution() {
	setStreamResolution(streamResolution);
}

void Optilab::startRecording(int compressor) {
	if (recordingStatus == Paused) {
		recordingStatus = Recording;
		timer->start(10);
		return;
	}
	int sW, sH;
	CameraGetImageSize(&sW, &sH);
	videoFile = QDateTime::currentDateTime().toString("hh-mm-ss-zzz") + ".avi";
	auto writer = new cv::VideoWriter;
	bool result = writer->open(videoFile.toStdString(), cv::VideoWriter::fourcc('D', 'I', 'V', 'X'), 25.0, cv::Size(sW, sH));
	if (result) {
		recorder = writer;
		recordingStatus = Recording;
		timeStarted = QTime(0, 0, 0, 0);
		timer->start(40);
	}
}

void Optilab::pauseRecording() {
	if (recorder) {
		recordingStatus = Paused;
		timer->stop();
	}
}

void Optilab::stopRecording() {
	if (recorder) {
		recordingStatus = Stopped;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		recorder->release();
		timer->stop();
		timerLabel->setText("");
		if (QFile::exists(videoFile)) {
			QFileInfo vid(videoFile);
			VideoPreview preview(vid.absoluteFilePath(), this);
			preview.exec();
		}
	}
}

void Optilab::updateTimer() {
	QMutex mutex;
	mutex.lock();
	if (recorder && recordingStatus == Recording)
		recorder->write(currentFrame);
	mutex.unlock();
	timeStarted = timeStarted.addMSecs(40);
	timerLabel->setText(timeStarted.toString("hh:mm:ss"));
}

int Optilab::getMaxExposureTime() {
	USHORT expTime = 0;
	CameraGetMaxExposureTime(&expTime);
	return expTime;
}

bool Optilab::isAWBEnabled() {
	BOOL state = TRUE;
	CameraGetAWBState(&state);
	if (state == TRUE) return true;
	else return false;
}

void Optilab::enableAWB(bool enable) {
	BOOL state = (enable) ? TRUE : FALSE;
	CameraSetAWBState(state);
}

bool Optilab::isAEEnabled() {
	BOOL state = TRUE;
	CameraGetAeState(&state);
	if (state == TRUE) return true;
	else return false;
}

void Optilab::enableAE(bool enable) {
	BOOL state = (enable) ? TRUE : FALSE;
	CameraSetAeState(state);
}

double Optilab::getRGain() {
	INT v, d, dd;
	CameraGetGain(&v, &d, &dd);
	return 1.0 * (v - 20) / (maxRGBGain - 20);
}

void Optilab::setRGain(double red) {
	int r = red * (maxRGBGain - 20) + 20;
	int g = getGGain() * (maxRGBGain - 20) + 20;
	int b = getBGain() * (maxRGBGain - 20) + 20;
	CameraSetGain(r, g, b);
}

double Optilab::getGGain() {
	INT v, d, dd;
	CameraGetGain(&d, &v, &dd);
	return 1.0 * (v - 20) / (maxRGBGain - 20);
}

void Optilab::setGGain(double green) {
	int r = getRGain() * (maxRGBGain - 20) + 20;
	int g = green * (maxRGBGain - 20) + 20;
	int b = getBGain() * (maxRGBGain - 20) + 20;
	CameraSetGain(r, g, b);
}

double Optilab::getBGain() {
	INT v, d, dd;
	CameraGetGain(&d, &dd, &v);
	return 1.0 * (v - 20) / (maxRGBGain - 20);
}

void Optilab::setBGain(double blue) {
	int r = getRGain() * (maxRGBGain - 20) + 20;
	int g = getGGain() * (maxRGBGain - 20) + 20;
	int b = blue * (maxRGBGain - 20) + 20;
	CameraSetGain(r, g, b);
}

double Optilab::getContrast() {
	uchar val;
	CameraGetContrast(&val);
	return 1.0 * val / maxContrast;
}

void Optilab::setContrast(double ctrs) {
	uchar v = ctrs * maxContrast;
	CameraSetContrast(v);
}

double Optilab::getSaturation() {
	uchar val;
	CameraGetSaturation(&val);
	return 1.0 * val / maxSaturation;
}

void Optilab::setSaturation(double sat) {
	uchar v = sat * maxSaturation;
	CameraSetSaturation(v);
}

double Optilab::getGamma() {
	uchar v;
	CameraGetGamma(&v);
	return 1.0 * (1.0-((v - 20) / (maxGamma - 20)));
}

void Optilab::setGamma(double gamma) {
	uchar v = -1.0 * ((gamma - 1.0) * (maxGamma - 20) - 20);
	CameraSetGamma(v);
}

double Optilab::getAETarget() {
	uchar v;
	CameraGetAeTarget(&v);
	return 1.0 * (v - 40) / (maxAETarget - 40);
}

void Optilab::setAETarget(double target) {
	uchar v = target * (maxAETarget - 40) + 40;
	CameraSetAeTarget(v);
}

double Optilab::getExposureTime() {
	int v;
	CameraGetExposureTime(&v);
	return 1.0 * (v - 1) / (getMaxExposureTime() - 1);
}

void Optilab::setExposureTime(double time) {
	int v = time * (getMaxExposureTime() - 1) + 1;
	CameraSetExposureTime(v);
}

double Optilab::getAEGain() {
	ushort v;
	CameraGetAnalogGain(&v);
	return 1.0 * (v - 1) / (maxAEGain - 1);
}

void Optilab::setAEGain(double gain) {
	ushort v = gain * (maxAEGain - 1) + 1;
	CameraSetAnalogGain(v);
}

void Optilab::loadProfile(int index) {
	auto team = (DS_PARAMETER_TEAM)index;
	if (index > 4 || index < 0) {
		CameraLoadDefault();
	}
	else {
		auto status = CameraReadParameter(team);
	}
	//Check some troublesome parameter:
	BYTE val;
	BOOL state;
	CameraGetFrameSpeed(&val);
	if (FRAME_SPEED_NORMAL != (DS_FRAME_SPEED)val)
		CameraSetFrameSpeed(FRAME_SPEED_NORMAL);
	CameraGetB2RGBMode(&val);
	if (B2RGB_MODE_LINE != (DS_B2RGB_MODE)val)
		CameraSetB2RGBMode(B2RGB_MODE_LINE);
	CameraGetMonochrome(&state);
	if (state == TRUE)
		CameraSetMonochrome(FALSE);
	auto dh = MIRROR_DIRECTION_HORIZONTAL;
	auto dv = MIRROR_DIRECTION_VERTICAL;
	CameraGetMirror(dh, &state);
	if (state == TRUE)
		CameraSetMirror(dh, FALSE);
	CameraGetMirror(dv, &state);
	if (state == TRUE)
		CameraSetMirror(dv, FALSE);
	CameraSaveParameter(team);
}

void Optilab::saveProfile(int index) {
	if (index > 4 || index < 0) return;
	auto team = (DS_PARAMETER_TEAM)index;
	auto status = CameraSaveParameter(team);
}

void Optilab::toggleRibbon() {
	isRibbonVisible = !isRibbonVisible;
	if (isRibbonVisible) {
		ui.ribbon->show();
		QPushButton* pushButtonRibbon = parent()->findChild<QPushButton*>("pushButtonRibbon");
		pushButtonRibbon->setStyleSheet("#pushButtonRibbon {image: url(:/Icons/HideRibbon.png);} #pushButtonRibbon:hover { image: url(:/Icons/HideRibbon-hover.png); }");
		pushButtonRibbon->setToolTip("Hide Ribbon");
	}
	else {
		ui.ribbon->hide();
		QPushButton* pushButtonRibbon = parent()->findChild<QPushButton*>("pushButtonRibbon");
		pushButtonRibbon->setStyleSheet("#pushButtonRibbon {image: url(:/Icons/ShowRibbon.png);} #pushButtonRibbon:hover { image: url(:/Icons/ShowRibbon-hover.png); }");
		pushButtonRibbon->setToolTip("Show Ribbon");
	}
	calcAspectRatio();
}

QVariant Optilab::loadSettings(const QString& name) {
	QSettings settings("Miconos", "Optilab");
	return settings.value(name);
}

void Optilab::saveSettings(const QString& name, const QVariant& value) {
	QSettings settings("Miconos", "Optilab");
	settings.setValue(name, value);
}

void Optilab::closeEvent(QCloseEvent* event) {
	QMutex lock;
	lock.lock();
	ongoingCapture = false;
	lock.unlock();
	status.waitForFinished();
	bool res = QMetaObject::invokeMethod(control, "saveLastParams");
	event->accept();
}

void Optilab::showOnScreenControl() {
	stepperControl->show();
}

void Optilab::setStepper(Stepper* step) {
	if (step) {
		stepper = step;
		stepperControl->setStepper(stepper);
		stepperDebug->setStepper(stepper);
		connect(stepper, &Stepper::stepperUpdated, stepperDebug, &StepperDebug::updateStatus);
		//stepper->initPosition();
		connect(ui.screen, &CameraWidget::jogUp, stepper, &Stepper::jogUp);
		connect(ui.screen, &CameraWidget::jogRight, stepper, &Stepper::jogRight);
		connect(ui.screen, &CameraWidget::jogDown, stepper, &Stepper::jogDown);
		connect(ui.screen, &CameraWidget::jogLeft, stepper, &Stepper::jogLeft);
		connect(ui.screen, &CameraWidget::jogZUp, stepper, &Stepper::jogZUp);
		connect(ui.screen, &CameraWidget::jogZDown, stepper, &Stepper::jogZDown);
		connect(ui.screen, &CameraWidget::jogUR, stepper, &Stepper::jogUR);
		connect(ui.screen, &CameraWidget::jogDR, stepper, &Stepper::jogDR);
		connect(ui.screen, &CameraWidget::jogDL, stepper, &Stepper::jogDL);
		connect(ui.screen, &CameraWidget::jogUL, stepper, &Stepper::jogUL);
		connect(ui.screen, &CameraWidget::stop, stepper, &Stepper::stop);
		interpreter->registerObject(stepper, "stepper");
	}
}