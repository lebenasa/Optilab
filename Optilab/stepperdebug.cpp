#include "stdafx.h"
#include "stepperdebug.h"

#include <bitset>
#include <string>

StepperDebug::StepperDebug(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

StepperDebug::~StepperDebug()
{
	stepper = NULL;
}

void StepperDebug::updateStatus() {
	if (!stepper) return;
	ui.X->setText(QString::number(stepper->X()));
	ui.Y->setText(QString::number(stepper->Y()));
	ui.Z->setText(QString::number(stepper->Z()));
	std::bitset<8> jog, out, limit;
	jog = stepper->jog();
	ui.Jog->setText(QString::fromStdString(jog.to_string()));
	out = stepper->output();
	ui.Output->setText(QString::fromStdString(out.to_string()));
	limit = stepper->limit();
	ui.Limit->setText(QString::fromStdString(limit.to_string()));
	QString buffer = QString("%1/%2").arg(stepper->bufferFree()).arg(stepper->bufferSize());
	ui.Buffer->setText(buffer);
}

void StepperDebug::showEvent(QShowEvent* event) {
	move(1101, 184 + this->height());
	event->accept();
}