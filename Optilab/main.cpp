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

#ifndef DEBUG
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
	splash->showMessage("Done!", Qt::AlignBottom | Qt::AlignCenter);
	a->processEvents();
#endif

	HBRUSH windowBackground = CreateSolidBrush(RGB(255, 255, 255));
	NativeWindow window(a, windowBackground, 0, 0, 1024, 720);
	window.setMinimumSize(800, 600);
#ifndef DEBUG
	if (stepper) {
		Optilab* optilab = window.window()->widget();
		optilab->setStepper(stepper);
	}
	splash->finish(window.window());
#endif
	a->exec();
}
