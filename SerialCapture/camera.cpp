#include "stdafx.h"
#include "camera.h"

#include <qsgsimpletexturenode.h>
#include <qquickwindow.h>

#include "DSCAMAPI.h"
#include "opencv2\opencv.hpp"

//Useful macro for error checking
#define STATUS_MSG(msg) #msg

bool CHECKSTAT(DS_CAMERA_STATUS stat) {
	if (stat != STATUS_OK) {
		std::cerr << STATUS_MSG(stat);
		return false;
	}
	return true;
}

//DSCamera implementation
DSCamera* dsc;

DSCamera::DSCamera(QObject *parent)
	: Camera(parent), m_resolution(2)
{
	dsc = this;
	initialize();
}

DSCamera::~DSCamera() {
	deinitialize();
}

int CALLBACK SnapThreadCallback(BYTE* pBuffer) {
	BYTE *pBmp24 = CameraISP(pBuffer);
	if (pBmp24 && dsc)
		dsc->imageProc(pBmp24);
	return TRUE;
}

void DSCamera::imageProc(BYTE* pBuffer) {
	auto sz = size();
	cv::Mat frame = cv::Mat(sz.height(), sz.width(), CV_8UC3, pBuffer);
	cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
	cv::flip(frame, frame, 0);
	m_buffer = QImage(frame.data, sz.width(), sz.height(), QImage::Format_RGB888);
	emit frameReady(m_buffer);
}

void DSCamera::setResolution(int res) {
	deinitialize();
	bool status = CHECKSTAT(CameraInit(SnapThreadCallback, (DS_RESOLUTION)res, 0, 1, 0));
	if (status) {
		m_resolution = res;
		CameraSetB2RGBMode(DS_B2RGB_MODE::B2RGB_MODE_LINE);
		CameraSetColorEnhancement(TRUE);
		CameraSetLightFrquency(DS_LIGHT_FREQUENCY::LIGHT_FREQUENCY_60HZ);
		CameraSetFrameSpeed(DS_FRAME_SPEED::FRAME_SPEED_NORMAL);
		m_available = CHECKSTAT(CameraPlay());
		emit sourceSizeChanged(size());
	}
	else {
		m_available = false;
	}
}

QSize& DSCamera::size() {
	int W, H;
	CameraGetImageSize(&W, &H);
	m_size = QSize(W, H);
	return m_size;
}

void DSCamera::capture(int res, const QString &fileName) {
	LPCTSTR fn = L"Z";
	if (!CHECKSTAT(CameraCaptureFile(fn, FILE_JPG, 100, (DS_RESOLUTION)res)))
		return;
	if (QFile::exists(fileName))
		QFile::remove(fileName);
	QFile::copy("Z.jpg", fileName);
}

void DSCamera::initialize() {
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

	//Start camera:
	setResolution(m_resolution);
}

void DSCamera::deinitialize() {
	CameraStop();
	CameraUnInit();
}

//QuickCam implementation
QuickCam::QuickCam(QQuickItem* parent)
	: QQuickItem(parent), m_frame(QSize(10, 10), QImage::Format_RGB888), m_blocked(false)
{
	m_frame.fill(Qt::black);
	renderParams = OriginalSize;
	setFlag(QQuickItem::ItemHasContents, true);
}

QuickCam::~QuickCam() {

}

bool QuickCam::isBlocked() {
	return m_blocked;
}

void QuickCam::block(bool bl) {
	if (m_blocked != bl) {
		m_blocked = bl;
		emit blockedChanged(bl);
	}
}

QImage QuickCam::currentFrame() const {
	return m_frame;
}

void QuickCam::updateImage(const QImage &frame) {
	if (!m_blocked) {
		auto src = frame;
		if (renderParams == ScaledToItem)
			src = src.scaled(QSize(width(), height()));
		else if (renderParams == Halved)
			src = src.scaled(QSize(src.width() / 2, src.height() / 2));
		m_frame = src;
	}
	update();
	emit sourceChanged(m_frame);
}

QSGNode* QuickCam::updatePaintNode(QSGNode* node, UpdatePaintNodeData* u) {
	Q_UNUSED(u)
	QSGSimpleTextureNode* n = static_cast<QSGSimpleTextureNode*>(node);
	if (!n) {
		n = new QSGSimpleTextureNode();
	}
	n->setRect(boundingRect());

	auto texture = n->texture();
	if (texture) texture->deleteLater();
	n->setTexture(this->window()->createTextureFromImage(m_frame));
	return n;
}