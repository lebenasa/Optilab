#include "stdafx.h"

#include "qguiapplication.h"
#include "serialcapture.h"

int main(int argc, char *argv[])
{
	QGuiApplication a(argc, argv);
	SerialCapture app;
	app.show();
	return a.exec();
}
