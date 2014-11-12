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

signals:
	void jogUp();
	void jogDown();
	void jogRight();
	void jogLeft();
	void jogZUp();
	void jogZDown();
	void stop(int);

protected:
	void paintEvent(QPaintEvent*);
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);
};

#endif // CAMERAWIDGET_H
