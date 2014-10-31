#include "camerawidget.h"

#define NOMINMAX
#include <Windows.h>
#include <atlstr.h>

#include "DSCAMAPI.h"

int CALLBACK SnapThreadCallback(BYTE* pBuffer) {
	auto pBmp24 = CameraISP(pBuffer);
	if (pBmp24)
		CameraDisplayRGB24(pBmp24);
	return TRUE;
}

CameraWidget::CameraWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	auto status = CameraInit(SnapThreadCallback, R1280_960, (HWND)ui.screen->winId(), 1, (HWND)this->winId());
	if (status == STATUS_OK)
		CameraPlay();
}

CameraWidget::~CameraWidget()
{
	CameraStop();
	CameraUnInit();
}

void CameraWidget::resizeEvent(QResizeEvent* event) {
	QWidget::resizeEvent(event);
	int srcWidth, srcHeight;
	CameraGetImageSize(&srcWidth, &srcHeight);
	auto screenSize = this->size();
	double aspectRatio = 1.0 * srcWidth / srcHeight;
	double screenRatio = 1.0 * screenSize.width() / screenSize.height();
	int frameWidth, frameHeight;
	int hOffset, vOffset;
	hOffset = vOffset = 0;
	if (aspectRatio > screenRatio) {
		frameWidth = screenSize.width();
		frameHeight = 1.0 * screenSize.width() / aspectRatio;
		vOffset = (screenSize.height() - frameHeight) / 2;
	}
	else {
		frameWidth = 1.0 * screenSize.height() * aspectRatio;
		frameHeight = screenSize.height();
		hOffset = (screenSize.width() - frameWidth) / 2;
	}
	ui.screenLayout->setContentsMargins(hOffset, vOffset, hOffset, vOffset);
}