#include "stdafx.h"
#include "ribbonwindow.h"

RibbonWindow::RibbonWindow(HWND hwnd)
	: QWinWidget(hwnd)
{
	windowHandle = hwnd;
	setObjectName("mainPanel");
	optilab = new Optilab(this);

	// Horizontal layout
	QHBoxLayout *horizontalLayout = new QHBoxLayout;
	horizontalLayout->setSpacing(0);
	horizontalLayout->setMargin(0);

	// Window title
	QLabel *windowTitle = new QLabel(this);
	windowTitle->setObjectName("windowTitle");
	windowTitle->setText("OPTILAB VIEWER");
	windowTitle->setStyleSheet("font-size: 16px; color: #555555; font-family: \"Segoe UI\"; font-weight: 400;");
	windowTitle->setAttribute(Qt::WA_TransparentForMouseEvents);
	horizontalLayout->addStretch();
	horizontalLayout->addWidget(windowTitle);
	horizontalLayout->addStretch();

	// System buttons
	//Fullscreen
	//QPushButton *pushButtonFullscreen = new QPushButton("", this);
	//pushButtonFullscreen->setObjectName("pushButtonFullscreen");
	//horizontalLayout->addWidget(pushButtonFullscreen);
	//connect(pushButtonFullscreen, &QPushButton::clicked, this, &RibbonWindow::fullscreenTriggered);
	//pushButtonFullscreen->setToolTip("Fullscreen");

	//Hide/Show Ribbon
	QPushButton *pushButtonRibbon = new QPushButton("", this);
	pushButtonRibbon->setObjectName("pushButtonRibbon");
	horizontalLayout->addWidget(pushButtonRibbon);
	connect(pushButtonRibbon, &QPushButton::clicked, optilab, &Optilab::toggleRibbon);
	pushButtonRibbon->setToolTip("Hide Ribbon");

	// Minimize
	QPushButton *pushButtonMinimize = new QPushButton("", this);
	pushButtonMinimize->setObjectName("pushButtonMinimize");
	horizontalLayout->addWidget(pushButtonMinimize);
	QObject::connect(pushButtonMinimize, SIGNAL(clicked()), this, SLOT(minimizeTriggered()));
	pushButtonMinimize->setToolTip("Minimize");

	// Maximize
	QPushButton *pushButtonMaximize = new QPushButton("", this);
	pushButtonMaximize->setObjectName("pushButtonMaximize");
	horizontalLayout->addWidget(pushButtonMaximize);
	QObject::connect(pushButtonMaximize, SIGNAL(clicked()), this, SLOT(maximizeTriggered()));
	pushButtonMaximize->setToolTip("Maximize");

	// Close
	QPushButton *pushButtonClose = new QPushButton("", this);
	pushButtonClose->setObjectName("pushButtonClose");
	horizontalLayout->addWidget(pushButtonClose);
	QObject::connect(pushButtonClose, SIGNAL(clicked()), this, SLOT(closeTriggered()));
	pushButtonClose->setToolTip("Close");

	// Main panel layout
	QGridLayout *mainGridLayout = new QGridLayout();
	mainGridLayout->setSpacing(0);
	mainGridLayout->setMargin(0);
	setLayout(mainGridLayout);

	// Central widget
	QWidget *centralWidget = new QWidget(this);
	centralWidget->setObjectName("centralWidget");
	centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// Vertical layout
	QVBoxLayout *verticalLayout = new QVBoxLayout();
	verticalLayout->setSpacing(0);
	verticalLayout->setMargin(0);
	verticalLayout->setAlignment(Qt::AlignHCenter);
	verticalLayout->addLayout(horizontalLayout);

	//Optilab
	optilab->setAttribute(Qt::WA_NoMousePropagation);
	verticalLayout->addWidget(optilab);

	// Show
	centralWidget->setLayout(verticalLayout);
	mainGridLayout->addWidget(centralWidget);

	show();
}

RibbonWindow::~RibbonWindow()
{

}

// Button events
void RibbonWindow::minimizeTriggered() {
	ShowWindow(parentWindow(), SW_MINIMIZE);
}

void RibbonWindow::maximizeTriggered() {
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(parentWindow(), &wp);
	if (wp.showCmd == SW_MAXIMIZE) {
		ShowWindow(parentWindow(), SW_RESTORE);
	}
	else {
		ShowWindow(parentWindow(), SW_MAXIMIZE);
	}
	optilab->calcAspectRatio();
}

void RibbonWindow::closeTriggered() {
	optilab->close();
	PostQuitMessage(0);
}

WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };
void RibbonWindow::fullscreenTriggered() {
	DWORD dwStyle = GetWindowLong(parentWindow(), GWL_STYLE);
	if (dwStyle & WS_OVERLAPPEDWINDOW) {
		MONITORINFO mi = { sizeof(mi) };
		if (GetWindowPlacement(parentWindow(), &g_wpPrev) &&
			GetMonitorInfo(MonitorFromWindow(parentWindow(),
			MONITOR_DEFAULTTOPRIMARY), &mi)) {
			SetWindowLong(parentWindow(), GWL_STYLE,
				dwStyle & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(parentWindow(), HWND_TOP,
				mi.rcMonitor.left, mi.rcMonitor.top,
				mi.rcMonitor.right - mi.rcMonitor.left,
				mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

			RECT winrect;
			GetClientRect(parentWindow(), &winrect);
			setGeometry(8, 8, winrect.right - 16, winrect.bottom - 16);
	}
}
	else {
		SetWindowLong(parentWindow(), GWL_STYLE,
			dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(parentWindow(), &g_wpPrev);
		SetWindowPos(parentWindow(), NULL, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

		RECT winrect;
		GetClientRect(parentWindow(), &winrect);
		setGeometry(4, 4, winrect.right - 8, winrect.bottom - 8);
	}
}

#if QT_VERSION >= 0x050000
bool RibbonWindow::nativeEvent(const QByteArray &, void *msg, long *) {
#else
bool RibbonWindow::winEvent(MSG *message, long *) {
#endif
#if QT_VERSION >= 0x050000
	MSG *message = (MSG *)msg;
#endif
	switch (message->message) {
	case WM_SYSKEYDOWN: {
		if (message->wParam == VK_SPACE) {
			RECT winrect;
			GetWindowRect(windowHandle, &winrect);
			TrackPopupMenu(GetSystemMenu(windowHandle, false), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, windowHandle, NULL);
			break;
		}
	}
	case WM_KEYDOWN: {
		if (message->wParam == VK_F5 ||
			message->wParam == VK_F6 ||
			message->wParam == VK_F7
			) {
			SendMessage(windowHandle, WM_KEYDOWN, message->wParam, message->lParam);
			break;
		}
	}
	}

	return false;
}

void RibbonWindow::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		ReleaseCapture();
		SendMessage(windowHandle, WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}

	if (event->type() == QEvent::MouseButtonDblClick) {
		if (event->button() == Qt::LeftButton) {
			WINDOWPLACEMENT wp;
			wp.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(parentWindow(), &wp);
			if (wp.showCmd == SW_MAXIMIZE) {
				ShowWindow(parentWindow(), SW_RESTORE);
			}
			else {
				ShowWindow(parentWindow(), SW_MAXIMIZE);
			}
		}
	}
}

