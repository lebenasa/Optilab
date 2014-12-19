#include "stdafx.h"
#include "serialcapture.h"
#include "camera.h"
#include "stepper.h"
#include "cameramodel.h"
#include "sminterface.h"
using namespace std;

SerialCapture::SerialCapture(QObject *parent)
	: QObject(parent), engine{ this }, m_zoom(10), m_block(false)
{
	// Initialize
	m_camera = new DSCamera(this);
	m_stepper = new CNCStepper(this);
	m_interface = new SMInterface(0, this);
	m_model = new CameraModel(m_interface->rows(), m_interface->cols(), this);
	rootContext = engine.rootContext();

	m_size = m_camera->size() / m_zoom;

	// Connections
	connect(m_stepper, &Stepper::xyChanged, m_interface, &SMInterface::updatePos);
	connect(m_stepper, &Stepper::bufferFull, this, &SerialCapture::unblockStream);
	connect(m_camera, &DSCamera::frameReady, this, &SerialCapture::redirectImage);

	// Start UI
	rootContext->setContextProperty("serialcapture", this);
	rootContext->setContextProperty("camera", m_camera);
	rootContext->setContextProperty("stepper", m_stepper);
	rootContext->setContextProperty("model", m_model);
	engine.load(QUrl(QStringLiteral("qrc:///Resources/SerialCapture/SerialCapture.qml")));
}

SerialCapture::~SerialCapture()
{

}

void SerialCapture::blockStream() {
	if (!m_block) {
		m_block = true;
		emit blockChanged(true);
	}
}

void SerialCapture::unblockStream() {
	if (m_block) {
		m_block = false;
		emit blockChanged(false);
	}
}

void SerialCapture::redirectImage(const QImage& img) {
	if (!m_block && !m_stepper->isWorking())
		m_model->updateBuffer(img, m_interface->currentPos());
}

QSize SerialCapture::cellSize() const {
	return m_size;
}

void SerialCapture::setCellSize(const QSize& size) {
	if (m_size != size) {
		m_size = size;
		emit cellSizeChanged(m_size);
	}
}

void SerialCapture::zoomIn() {
	if (m_zoom > 1) {
		--m_zoom;
		setCellSize(m_camera->size() / m_zoom);
	}
}

void SerialCapture::zoomOut() {
	if (m_zoom < 10) {
		++m_zoom;
		setCellSize(m_camera->size() / m_zoom);
	}
}

void SerialCapture::moveToSelected() {
	blockStream();
	m_interface->moveTo(m_model->selectedCell());
}