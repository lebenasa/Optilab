#include "camerawidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CameraWidget w;
	w.show();
	return a.exec();
}
