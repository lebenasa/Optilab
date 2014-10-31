#ifndef RibbonWindow_H
#define RibbonWindow_H

#include "QWinWidget.h"
#include "optilab.h"

class RibbonWindow : public QWinWidget
{
	Q_OBJECT
public:
	RibbonWindow(HWND hwnd);
	~RibbonWindow();

#if QT_VERSION >= 0x050000
	bool nativeEvent(const QByteArray &, void *msg, long *result);
#else
	bool winEvent(MSG *message, long *result);
#endif
	void mousePressEvent(QMouseEvent *event);

	Optilab* widget() { return optilab; }

public slots:
	void minimizeTriggered();
	void maximizeTriggered();
	void closeTriggered();
	void fullscreenTriggered();

private:
	HWND windowHandle;
	Optilab* optilab;
};

#endif // RibbonWindow_H
