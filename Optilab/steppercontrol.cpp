#include "stdafx.h"
#include "steppercontrol.h"
#include <qqmlcontext.h>
#include "optilab.h"
#include "stepper.h"

StepperControl::StepperControl(QWidget *parent)
	: QDialog(parent, Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint),
	lastPos(1101, 154)
{
	auto lp = Optilab::loadSettings("CONTROL_LAST_POS").toPoint();
	if (lp.x() != 0 && lp.y() != 0) lastPos = lp;
	setMinimumSize(240, 150);
	control = new QQuickView(QUrl("qrc:/Mikrostepper/Mikrostepper.qml"));
	control->setResizeMode(QQuickView::SizeRootObjectToView);
	root = control->rootObject();
	auto w = QWidget::createWindowContainer(control, this);
	auto gridLayout = new QGridLayout(this);
	gridLayout->setSpacing(0);
	gridLayout->setContentsMargins(0, 0, 0, 0);
	gridLayout->addWidget(w, 0, 0, 1, 1);
	setWindowTitle("On-Screen Control");
}

StepperControl::~StepperControl()
{	
	stepper = NULL;
	Optilab::saveSettings("CONTROL_LAST_POS", lastPos);
}

void StepperControl::closeEvent(QCloseEvent* event) {
	lastPos = this->pos();
	qDebug() << lastPos;
	event->accept();
}

void StepperControl::showEvent(QShowEvent* event) {
	move(lastPos);
	event->accept();
}

void StepperControl::setStepper(Stepper* step) {
	if (step) {
		stepper = step;
		auto ctx = control->rootContext();
		ctx->setContextProperty("stepper", stepper);
	}
}