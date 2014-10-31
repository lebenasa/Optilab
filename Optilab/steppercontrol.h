#ifndef STEPPERCONTROL_H
#define STEPPERCONTROL_H

#include <QDialog>
#include <QQuickItem>
#include <QQuickView>

class Stepper;

class StepperControl : public QDialog
{
	Q_OBJECT

public:
	StepperControl(QWidget *parent);
	~StepperControl();

	void setStepper(Stepper*);

private:
	QQuickView* control;
	QQuickItem* root;
	Stepper* stepper;

	QPoint lastPos;

protected:
	void closeEvent(QCloseEvent*);
	void showEvent(QShowEvent*);
};

#endif // STEPPERCONTROL_H
