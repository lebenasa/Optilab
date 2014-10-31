#ifndef STEPPERDEBUG_H
#define STEPPERDEBUG_H

#include <QDialog>
#include "ui_StepperDebug.h"

#include "stepper.h"

class StepperDebug : public QDialog
{
	Q_OBJECT
	Ui::StepperDebug ui;
	Stepper* stepper;
public:
	StepperDebug(QWidget *parent = 0);
	~StepperDebug();

	void setStepper(Stepper* step) {
		stepper = step;
	}

public slots:
	void updateStatus();
private:
	
protected:
	void showEvent(QShowEvent*);
};

#endif // STEPPERDEBUG_H
