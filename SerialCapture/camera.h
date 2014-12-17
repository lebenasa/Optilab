#ifndef CAMERA_H
#define CAMERA_H

/* 
Base implementation of Camera
- Buffer current frame as QImage
- Capture to file and to buffer
- Set and get resolution
*/

#include <QObject>

class Camera : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QSize sourceSize READ size NOTIFY sourceSizeChanged)
public:
	Camera(QObject *parent = 0) : QObject(parent) { }
	~Camera() { }

public slots:
	virtual void setResolution(int res) = 0;

	virtual QSize& size() = 0;

	virtual void capture(int resolution, const QString &fileName) = 0;

signals :
	void frameReady(const QImage& frame);
	void sourceSizeChanged(const QSize& sz);

protected:
	virtual void initialize() = 0;
	virtual void deinitialize() = 0;
};

/*
DSCamera - implementation of Camera using MSHOT API
Camera control (brightness, saturation, etc) is handled elsewhere
*/

class DSCamera : public Camera
{
	Q_OBJECT
	int m_resolution;
	bool m_available = false;
	QSize m_size;
	QImage m_buffer;

public:
	DSCamera(QObject *parent = 0);
	~DSCamera();

	void imageProc(BYTE* pBuffer);

public slots:
	void setResolution(int res) override;

	QSize& size();

	void capture(int resolution, const QString &fileName);

	bool isAvailable() { return m_available; }

protected:
	void initialize();
	void deinitialize();
};

/*
QuickCam - QQuickItem to render camera stream
- Image stream is stretched, aspect ratio calculation is done elsewhere
- Can selectively render a part of image
- Can process image in rendering thread

To use simply connect Camera::frameReady signals to QuickCam::updateImage
*/

#include <qquickitem.h>

class QuickCam : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(QImage source READ currentFrame WRITE updateImage NOTIFY sourceChanged)
	Q_PROPERTY(bool blocked READ isBlocked WRITE block NOTIFY blockedChanged)
	Q_PROPERTY(RenderParams renderParams MEMBER renderParams)
	Q_ENUMS(RenderParams)
	QImage m_frame;
	bool m_blocked;
public:
	QuickCam(QQuickItem* parent = 0);
	~QuickCam();

	enum RenderParams {
		OriginalSize, ScaledToItem, Halved
	} renderParams;

	bool isBlocked();
	void block(bool bl);

	QImage currentFrame() const;

public slots:
	void updateImage(const QImage &frame);

signals:
	void blockedChanged(bool block);
	void sourceChanged(const QImage &nframe);

protected:
	QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData*);
};

#endif // CAMERA_H
