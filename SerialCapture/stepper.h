#ifndef STEPPER_H
#define STEPPER_H

#include <QObject>

class Stepper : public QObject
{
	Q_OBJECT

public:
	Stepper(QObject *parent);
	~Stepper();

private:
	
};

#endif // STEPPER_H
