#include "stdafx.h"
#include "quickcam.h"
#include "stepper.h"
#include "profilemanager.h"

#include "qapplication.h"
#include "qqmlapplicationengine.h"
#include "qqmlcontext.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qmlRegisterType<QuickCam>("QuickCam", 1, 0, "Camera");

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:///Calibration/Calibration.qml")));
	auto ctx = engine.rootContext();

	ProfileManager manager;
	ctx->setContextProperty("manager", &manager);

#ifndef DEBUG
	Stepper* stepper = new Stepper;
	ctx->setContextProperty("stepper", stepper);
#endif

	return a.exec();
}
