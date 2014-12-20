#ifndef SERIALCAPTURE_H
#define SERIALCAPTURE_H

#include <QObject>
#include "qqmlapplicationengine.h"
#include "qqmlcontext.h"

class DSCamera;
class Stepper;
class CameraModel;
class SMInterface;

class SerialCapture : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QSize cellSize READ cellSize NOTIFY cellSizeChanged)
	Q_PROPERTY(bool block MEMBER m_block NOTIFY blockChanged)
	
public:
	SerialCapture(QObject *parent = 0);
	~SerialCapture();

	QSize cellSize() const;
	void setCellSize(const QSize &size);

public slots:
	void zoomIn();
	void zoomOut();

	void blockStream();
	void unblockStream();
	void redirectImage(const QImage& img);

	void moveToSelected();

	void show();

signals:
	void cellSizeChanged(const QSize &size);
	void blockChanged(bool);

private:
	DSCamera* m_camera;
	Stepper* m_stepper;
	CameraModel* m_model;
	SMInterface* m_interface;
	QQmlApplicationEngine engine;
	QQmlContext* rootContext;

	QSize m_size;	// cell size, i.e for implementing zoom
	int m_zoom;
	bool m_block;
};

#endif // SERIALCAPTURE_H
