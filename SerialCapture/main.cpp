#include "stdafx.h"

#include "qguiapplication.h"
#include "qqmlapplicationengine.h"
#include "qqmlcontext.h"

#include "camera.h"

int main(int argc, char *argv[])
{
	QGuiApplication a(argc, argv);
	qmlRegisterType<DSCamera>("QuickCam", 1, 0, "Camera");
	qmlRegisterType<QuickCam>("QuickCam", 1, 0, "CameraItem");

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:///Resources/SerialCapture/SerialCapture.qml")));
	auto ctx = engine.rootContext();

	return a.exec();
}
