#include "quickcam.h"
#include "qwidget.h"
#include "qstandardpaths.h"
#include "qfile.h"
#include "qprocess.h"
#include "qsgsimpletexturenode.h"
#include "qquickwindow.h"
#include "opencv2\opencv.hpp"

#include "DSCAMAPI.h"

#define QCAM50

void* pQuickCam;

#ifdef QCAM13
int CALLBACK SnapThreadCallback(BYTE* pBuffer, DI_DATA_TYPE type, void* context) {
	if (type == DATA_TYPE_RGB24) {
		auto caller = static_cast<QuickCam*>(context);
		caller->imgProc(pBuffer);
		return 1;
	}
	return 0;
}
#elif defined QCAM50
int CALLBACK SnapThreadCallback(BYTE* pBuffer) {
	BYTE* buf = CameraISP(pBuffer);
	if (pQuickCam && buf) {
		auto quickCam = static_cast<QuickCam*>(pQuickCam);
		quickCam->imgProc(buf);
		CameraDisplayRGB24(buf);
		return 1;
	}
	return 0;
}
#endif

QuickCam::QuickCam(QQuickItem* parent)
	: QQuickItem(parent), m_available(true), m_captureMode(0), m_captureRes(0),
	m_videoCompressor(0), m_zoom(1.0), m_awbEnabled(false), m_aeEnabled(false),
	m_streamRes(2), m_burstInterval(500), m_burstDuration(5000), m_burstCount(5)
{
#ifdef QCAM50
	pQuickCam = this;
#endif
	setStreamResolution(m_streamRes);
	setFlag(QQuickItem::ItemHasContents, true);
}

QuickCam::~QuickCam()
{
	CameraStop();
	CameraUnInit();
}

//void QuickCam::paint(QPainter *painter) {
//	QMutexLocker lock(&frameMutex);
//	if (frames.isEmpty()) return;
//	painter->setRenderHint(QPainter::Antialiasing, true);
//	//QImage img = frames.at(0).scaledToWidth(this->boundingRect().size().width());
//	if (!frames.at(0).isNull())
//		painter->drawImage(this->boundingRect(), frames.at(0));
//	frames.pop_front();
//}

QSGNode* QuickCam::updatePaintNode(QSGNode* node, UpdatePaintNodeData * u) {
	Q_UNUSED(u)
	QSGSimpleTextureNode* n = static_cast<QSGSimpleTextureNode*>(node);
	if (!n) {
		n = new QSGSimpleTextureNode();
	}
	n->setRect(boundingRect());
	if (!frames.isEmpty()) {
		auto texture = n->texture();
		if (texture) texture->deleteLater();
		n->setTexture(this->window()->createTextureFromImage(frames.dequeue()));
	}
	else {
		auto black = QImage(frameSize, QImage::Format_RGB888);
		black.fill(Qt::black);
		n->setTexture(this->window()->createTextureFromImage(black));
	}
	return n;
}

void QuickCam::imgProc(BYTE *pBuffer) {
	int W, H;
	CameraGetImageSize(&W, &H);
	cv::Mat frame = cv::Mat(H, W, CV_8UC3, pBuffer);
	cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
	cv::flip(frame, frame, 0);
	//cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
	//cv::flip(mat, mat, 0);
	QImage buff(frame.data, W, H, QImage::Format_RGB888);
	frames.enqueue(buff);
	//qDebug() << W << "x" << H;
	emit frameReady();
	//buffMutex.lock();
	//buffer.push(buff.copy());
	//while (buffer.size() > 3) buffer.removeFirst();
	//buffMutex.unlock();
}

void QuickCam::setAvailable(bool available) {
	m_available = available;
	emit availableChanged();
	if (!m_available) emit cameraNotFound();
}

void QuickCam::setCaptureMode(int mode) {
	m_captureMode = mode;
	emit captureModeChanged(mode);
}

void QuickCam::setCaptureRes(int res) {
	m_captureRes = res;
	emit captureResolutionChanged(res);
}

void QuickCam::setVideoCompressor(int compressor) {
	m_videoCompressor = compressor;
	emit videoCompressorChanged(compressor);
}

bool QuickCam::isAWBEnable() const {
	return m_awbEnabled;
}

void QuickCam::enableAWB(bool enable) {
	if (m_awbEnabled == enable) return;
	auto status = CameraSetAWBState(enable);
	if (status == STATUS_OK) {
		m_awbEnabled = enable;
		emit awbEnableChanged(enable);
		updateColorGain();
	}
}

bool QuickCam::isAEEnable() const {
	return m_aeEnabled;
}

void QuickCam::enableAE(bool enable) {
	if (m_aeEnabled == enable) return;
	auto status = CameraSetAeState(enable);
	if (status == STATUS_OK) {
		m_aeEnabled = enable;
		emit aeEnableChanged(enable);
		updateAEParams();
	}
}

int QuickCam::rGain() const {
	return m_rGain;
}

int QuickCam::gGain() const {
	return m_gGain;
}

int QuickCam::bGain() const {
	return m_bGain;
}

void QuickCam::setRGain(int red) {
	if (red != m_rGain) {
		CameraSetGain(red, m_gGain, m_bGain);
		emit rGainChanged(red);
	}
}

void QuickCam::setGGain(int green) {
	if (green != m_gGain) {
		CameraSetGain(m_rGain, green, m_bGain);
		emit gGainChanged(green);
	}
}

void QuickCam::setBGain(int blue) {
	if (blue != m_bGain) {
		CameraSetGain(m_rGain, m_gGain, blue);
		emit bGainChanged(blue);
	}
}

int QuickCam::contrast() const {
	return m_contrast;
}

void QuickCam::setContrast(int ctrs) {
	if (m_contrast == ctrs) return;
	auto status = CameraSetContrast(ctrs);
	if (status == STATUS_OK) {
		m_contrast = ctrs;
		emit contrastChanged(ctrs);
	}
}

int QuickCam::saturation() const {
	return m_saturation;
}

void QuickCam::setSaturation(int sat) {
	if (m_saturation == sat) return;
	auto status = CameraSetSaturation(sat);
	if (status == STATUS_OK) {
		m_saturation = sat;
		emit saturationChanged(sat);
	}
}

int QuickCam::gamma() const {
	return m_gamma;
}

void QuickCam::setGamma(int gamma) {
	if (m_gamma != gamma) {
		auto status = CameraSetGamma(gamma);
		if (status == STATUS_OK) {
			m_gamma = gamma;
			emit gammaChanged(gamma);
		}
	}
}

int QuickCam::aeTarget() const {
	uchar val;
	CameraGetAeTarget(&val);
	return val;
}

void QuickCam::setAETarget(int target) {
	auto status = CameraSetAeTarget(target);
	if (status == STATUS_OK) emit aeTargetChanged(target);
}

int QuickCam::aeGain() const {
	ushort val;
	CameraGetAnalogGain(&val);
	return val;
}

void QuickCam::setAEGain(int gain) {
	auto status = CameraSetAnalogGain(gain);
	if (status == STATUS_OK) emit aeGainChanged(gain);
}

int QuickCam::exposureTime() const {
	int val;
	CameraGetExposureTime(&val);
	return val;
}

void QuickCam::setExposureTime(int time) {
	auto status = CameraSetExposureTime(time);
	if (status == STATUS_OK) emit exposureTimeChanged(time);
}

int QuickCam::getMaxExposureTime() {
	ushort val;
	CameraGetMaxExposureTime(&val);
	return val;
}

void QuickCam::setZoom(double zl) {
	m_zoom = zl;
	emit zoomChanged();
}

void QuickCam::capture() {
	//if (m_captureRes == m_streamRes) {
	//	if (m_captureMode == 0) singleCapture();
	//	else if (m_captureMode == 1)
	//		auto future = QtConcurrent::run([this]() { timedCapture(); });
	//	else
	//		auto future = QtConcurrent::run([this]() { countedCapture(); });
	//	return;
	//}
	//
	//if (m_captureMode == 0) {
	//	auto func = [this](std::chrono::duration<int> duration, int captureRes, int oldStreamRes) {
	//		setStreamResolution(captureRes);
	//		std::this_thread::sleep_for(duration);
	//		singleCapture();
	//		//std::this_thread::sleep_for(std::chrono::seconds(1));
	//		setStreamResolution(oldStreamRes);
	//	};
	//	auto future = QtConcurrent::run(func, std::chrono::seconds(2), m_captureRes, m_streamRes);
	//}
	//else if (m_captureMode == 1) {
	//	auto func = [this](int captureRes, int oldStreamRes) {
	//		setStreamResolution(captureRes);
	//		std::this_thread::sleep_for(std::chrono::seconds(2));
	//		timedCapture();
	//		setStreamResolution(oldStreamRes);
	//	};
	//	auto future = QtConcurrent::run(func, m_captureRes, m_streamRes);
	//}
	//else {
	//	auto func = [this](int captureRes, int oldStreamRes) {
	//		setStreamResolution(captureRes);
	//		std::this_thread::sleep_for(std::chrono::seconds(2));
	//		countedCapture();
	//		setStreamResolution(oldStreamRes);
	//	};
	//	auto future = QtConcurrent::run(func, m_captureRes, m_streamRes);
	//}
}

void QuickCam::singleCapture() {
	//QString tempPath = QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0);
	//tempPath += "/singleCaptureTemp.png";
	//qDebug() << tempPath;
	//QMutexLocker locker(&buffMutex);
	//auto p = buffer.pop();
	//p.save(tempPath);
	//m_tempImagesPath.clear();
	//m_tempImagesPath.append(tempPath);
	//emit captureDone();
	//QProcess* explore = new QProcess(this);
	////QString tmp = QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0);
	//auto args = tempPath.replace(QChar('/'), QChar('\\'));
	//explore->start(QString("explorer.exe \"%1\"").arg(args));
	//disconnect(this, &QuickCam::frameReady, this, &QQuickItem::update);
	//CameraCaptureFile(L"temp", 8, 90, (DI_RESOLUTION)m_captureRes);
	//connect(this, &QuickCam::frameReady, this, &QQuickItem::update, Qt::QueuedConnection);
}

void QuickCam::timedCapture() {
	////WARNING: Call this from non-GUI thread only!
	//QString tempPath = QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0);
	//m_tempImagesPath.clear();
	//int fcount = m_burstDuration / m_burstInterval;
	//for (int i = 0; i < fcount; ++i) {
	//	QString path = tempPath + QString("/%1_timedTemp.png").arg(i, 4, 10, QChar('0'));
	//	m_tempImagesPath.append(path);
	//}
	//for (auto path : m_tempImagesPath) {
	//	buffMutex.lock();
	//	auto p = buffer.pop();
	//	p.save(tempPath);
	//	buffMutex.unlock();
	//	std::this_thread::sleep_for(std::chrono::milliseconds(m_burstInterval));
	//}
	//QProcess* explore = new QProcess(this);
	//auto args = tempPath.replace(QChar('/'), QChar('\\'));
	//explore->start(QString("explorer.exe \"%1\"").arg(args));
}

void QuickCam::countedCapture() {
	////WARNING: Call this from non-GUI thread only!
	//QString tempPath = QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0);
	//m_tempImagesPath.clear();
	//int fcount = m_burstCount;
	//for (int i = 0; i < fcount; ++i) {
	//	QString path = tempPath + QString("/%1_countedTemp.png").arg(i, 4, 10, QChar('0'));
	//	m_tempImagesPath.append(path);
	//}
	//for (auto path : m_tempImagesPath) {
	//	buffMutex.lock();
	//	auto p = buffer.pop();
	//	p.save(tempPath);
	//	buffMutex.unlock();
	//	std::this_thread::sleep_for(std::chrono::milliseconds(m_burstInterval));
	//}
	//emit captureDone();
	//QProcess* explore = new QProcess(this);
	//auto args = tempPath.replace(QChar('/'), QChar('\\'));
	//explore->start(QString("explorer.exe \"%1\"").arg(args));
}

void QuickCam::saveResult(const QUrl &path) {
	QFile img(QStandardPaths::displayName(QStandardPaths::TempLocation) + "\\tempImg");
	if (img.exists()) {
		img.copy(path.toLocalFile());
		img.remove();
	}
}

void QuickCam::startRecording() {

}

void QuickCam::stopRecording() {

}

void QuickCam::oneShotAWB() {
	enableAWB(true);
	enableAWB(false);
}

void QuickCam::setStreamResolution(int res) {
	auto status = CameraStop();
	status = CameraUnInit();
#ifdef QCAM13
	status = CameraInit(1, (DI_RESOLUTION)res, 0, SnapThreadCallback, this);
#elif defined QCAM50
	status = CameraInit(SnapThreadCallback, (DS_RESOLUTION)res, 0, 1, 0);
#endif
	if (status == STATUS_OK) {
		m_streamRes = res;
		disconnect(this, &QuickCam::frameReady, this, &QQuickItem::update);
		CameraPlay();
		int W, H;
		CameraGetImageSize(&W, &H);
		frameSize = QSize(W, H);
		connect(this, &QuickCam::frameReady, this, &QQuickItem::update, Qt::QueuedConnection);
		setAvailable(true);
		//updateEnhancement();
		//updateColorGain();
		//updateAEParams();
	}
	else {
		setAvailable(false);
	}
	emit streamResChanged();
}

QString QuickCam::streamResolutionString() const {
	if (m_streamRes == 0) {
#ifdef QCAM13
		return "1280x1024"; 
#elif defined QCAM50
		return "2592x1944";
#endif
	}
	else if (m_streamRes == 1) {
#ifdef QCAM13
		return "640x480"; 
#elif defined QCAM50
		return "1280x960";
#endif
	}
	else {
#ifdef QCAM13
		return "320x240";
#elif defined QCAM50
		return "640x480";
#endif
	}
}

void QuickCam::setBurstInterval(int interval) {
	if (m_burstInterval != interval) {
		m_burstInterval = interval;
		emit burstIntervalChanged(interval);
	}
}

void QuickCam::setBurstDuration(int duration) {
	if (m_burstDuration != duration) {
		m_burstDuration = duration;
		emit burstDurationChanged(duration);
	}
}

void QuickCam::setBurstCount(int count) {
	if (m_burstCount != count) {
		m_burstCount = count;
		emit burstCountChanged(count);
	}
}

void QuickCam::restartCamera() {
	setStreamResolution(m_streamRes);
}

void QuickCam::updateEnhancement() {
	uchar val;
	CameraGetContrast(&val);
	m_contrast = val;
	emit contrastChanged(val);
	val = 0;
	CameraGetSaturation(&val);
	m_saturation = val;
	emit saturationChanged(val);
	val = 0;
	CameraGetGamma(&val);
	m_gamma = val;
	emit gammaChanged(val);
}

void QuickCam::updateColorGain() {
	CameraGetGain(&m_rGain, &m_gGain, &m_bGain);
	emit rGainChanged(m_rGain);
	emit gGainChanged(m_gGain);
	emit bGainChanged(m_bGain);
}

void QuickCam::updateAEParams() {
	uchar val;
	CameraGetAeTarget(&val);
	m_target = val;
	emit aeTargetChanged(val);
	ushort va;
	CameraGetAnalogGain(&va);
	m_gain = va;
	emit aeGainChanged(va);
	CameraGetExposureTime(&m_exptime);
	emit exposureTimeChanged(m_exptime);
}

double QuickCam::getAspectRatio() {
	return 1.0 * frameSize.width() / frameSize.height();
}