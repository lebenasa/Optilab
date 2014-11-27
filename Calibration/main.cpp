#include "Calibration\quickcam.h"

#include "qapplication.h"
#include "qqmlapplicationengine.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qmlRegisterType<QuickCam>("QuickCam", 1, 0, "Camera");

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:///Calibration/Calibration.qml")));

	return a.exec();
}
