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
public:
	Camera(QObject *parent = 0) { }
	~Camera() {
		deinitialize();
	}

public slots:
	virtual void setResolution(int res) = 0;

	virtual QSize& size() = 0;

	virtual void capture(int resolution, const QString &fileName) = 0;
	virtual QImage& load(int resolution) {
		capture(resolution, "PREP.jpg");
		return QImage{ "PREP.jpg" };
	}

signals :
	void frameReady(const QImage&);

protected:
	virtual void initialize() = 0;
	virtual void deinitialize() = 0;
};

/*
DSCamera - implementation of Camera using MSHOT API
Camera control (brightness, saturation, etc) is handled elsewhere
*/

class DSCamera : Camera
{
	Q_OBJECT
	int m_resolution;
	bool m_available = false;
	QImage m_buffer;

public:
	DSCamera(int initResolution, QObject *parent = 0);

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

#endif // CAMERA_H
