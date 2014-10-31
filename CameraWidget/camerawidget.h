#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QtWidgets/QWidget>
#include "ui_camerawidget.h"

class CameraWidget : public QWidget
{
	Q_OBJECT

public:
	CameraWidget(QWidget *parent = 0);
	~CameraWidget();

private:
	Ui::CameraWidgetClass ui;

protected:
	void resizeEvent(QResizeEvent*);
};

#endif // CAMERAWIDGET_H
