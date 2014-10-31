#include "stdafx.h"
#include "nativewindow.h"
#include <QtWidgets/QApplication>
#include <qquickview.h>
#include <QSplashScreen>
#include "stepper.h"
#include "optilab.h"

int main(int argc, char *argv[])
{
	QApplication* a = new QApplication(argc, argv);
	QFile stylesheet(":/Icons/BorderlessWindow.css");
	if (stylesheet.open(QFile::ReadOnly)) {
		QString styleSheet = stylesheet.readAll();
		a->setStyleSheet(styleSheet);
	}

	QPixmap pixmap;
	bool res = pixmap.load(":/Ribbon/img/splash-screen.png");
	QSplashScreen* splash = new QSplashScreen(pixmap);
	splash->show();
	a->processEvents();
	splash->showMessage("Initializing stepper. Please wait...", Qt::AlignBottom | Qt::AlignHCenter);
	a->processEvents();
	Stepper* stepper = new Stepper();
	splash->showMessage(stepper->message(), Qt::AlignBottom | Qt::AlignHCenter);
	a->processEvents();
	//splash->showMessage("Initializing position. Please wait. . .", Qt::AlignBottom | Qt::AlignCenter);
	//a->processEvents();
	//stepper->initPosition();
	//splash->showMessage("Done!", Qt::AlignBottom | Qt::AlignCenter);
	//a->processEvents();

	HBRUSH windowBackground = CreateSolidBrush(RGB(255, 255, 255));
	NativeWindow window(a, windowBackground, 0, 0, 1024, 720);
	window.setMinimumSize(800, 600);
	if (stepper) {
		Optilab* optilab = window.window()->widget();
		optilab->setStepper(stepper);
	}
	splash->finish(window.window());
	a->exec();
}
