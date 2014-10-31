#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>

class CameraWidget : public QWidget
{
	Q_OBJECT

public:
	CameraWidget(QWidget *parent);
	~CameraWidget();

	HWND screenHandle() { return (HWND)screen->winId(); }

private:
	QGridLayout *screenLayout;
	QWidget *screen;

protected:
	void paintEvent(QPaintEvent*);
};

#endif // CAMERAWIDGET_H
