#include "stdafx.h"
#include "camerawidget.h"

CameraWidget::CameraWidget(QWidget *parent)
	: QWidget(parent)
{
	screenLayout = new QGridLayout(this);
	screenLayout->setSpacing(0);
	screenLayout->setContentsMargins(0, 0, 0, 0);
	screenLayout->setObjectName(QStringLiteral("screenLayout"));
	screen = new QWidget(this);
	screen->setObjectName(QStringLiteral("screen"));
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(screen->sizePolicy().hasHeightForWidth());
	screen->setSizePolicy(sizePolicy);

	screenLayout->addWidget(screen, 0, 0, 1, 1);
	setStyleSheet("background-color: rgb(30, 30, 30)");
}

CameraWidget::~CameraWidget()
{

}

void CameraWidget::paintEvent(QPaintEvent *)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}