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
	else if (event->key() == Qt::Key_Up) {
		emit jogUp();
	}
	else if (event->key() == Qt::Key_Right) {
		emit jogRight();
	}
	else if (event->key() == Qt::Key_Down) {
		emit jogDown();
	}
	else if (event->key() == Qt::Key_Left) {
		emit jogLeft();
	}
	else if (event->key() == Qt::Key_PageUp) {
		emit jogZUp();
	}
	else if (event->key() == Qt::Key_PageDown) {
		emit jogZDown();
	}
	else if (event->key() == Qt::Key_Plus) {
		emit jogZUp();
	}
	else if (event->key() == Qt::Key_Minus) {
		emit jogZDown();
	}
	QWidget::keyPressEvent(event);
}

void CameraWidget::keyReleaseEvent(QKeyEvent* event) {
	if (event->isAutoRepeat()) {

	}
	else if (event->key() == Qt::Key_Up) {
		emit stop(1);
	}
	else if (event->key() == Qt::Key_Right) {
		emit stop(2);
	}
	else if (event->key() == Qt::Key_Down) {
		emit stop(3);
	}
	else if (event->key() == Qt::Key_Left) {
		emit stop(4);
	}
	else if (event->key() == Qt::Key_PageUp) {
		emit stop(5);
	}
	else if (event->key() == Qt::Key_PageDown) {
		emit stop(6);
	}
	else if (event->key() == Qt::Key_Plus) {
		emit stop(5);
	}
	else if (event->key() == Qt::Key_Minus) {
		emit stop(6);
	}
	QWidget::keyPressEvent(event);
}