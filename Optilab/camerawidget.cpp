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
	setFocusPolicy(Qt::ClickFocus);
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

void CameraWidget::keyPressEvent(QKeyEvent* event) {
	if (event->isAutoRepeat()) {

	}
	else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_8)
		emit jogUp();
	else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_6)
		emit jogRight();
	else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_2)
		emit jogDown();
	else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_4)
		emit jogLeft();
	else if (event->key() == Qt::Key_Plus)
		emit jogZUp();
	else if (event->key() == Qt::Key_Minus)
		emit jogZDown();
	else if (event->key() == Qt::Key_PageUp || event->key() == Qt::Key_9)
		emit jogUR();
	else if (event->key() == Qt::Key_PageDown || event->key() == Qt::Key_3)
		emit jogDR();
	else if (event->key() == Qt::Key_End || event->key() == Qt::Key_1)
		emit jogDL();
	else if (event->key() == Qt::Key_Home || event->key() == Qt::Key_7)
		emit jogUL();
	QWidget::keyPressEvent(event);
}

void CameraWidget::keyReleaseEvent(QKeyEvent* event) {
	if (event->isAutoRepeat()) {

	}
	else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_8)
		emit stop(1);
	else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_6)
		emit stop(2);
	else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_2)
		emit stop(3);
	else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_4)
		emit stop(4);
	else if (event->key() == Qt::Key_Plus)
		emit stop(5);
	else if (event->key() == Qt::Key_Minus)
		emit stop(6);
	else if (event->key() == Qt::Key_PageUp || event->key() == Qt::Key_9)
		emit stop(7);
	else if (event->key() == Qt::Key_PageDown || event->key() == Qt::Key_3)
		emit stop(8);
	else if (event->key() == Qt::Key_End || event->key() == Qt::Key_1)
		emit stop(9);
	else if (event->key() == Qt::Key_Home || event->key() == Qt::Key_7)
		emit stop(10);
	QWidget::keyPressEvent(event);
}

void CameraWidget::focusOutEvent(QFocusEvent* event) {
	emit stop(0);
	QWidget::focusOutEvent(event);
}