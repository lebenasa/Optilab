#ifndef STEPPER_H
#define STEPPER_H

#include <QObject>

class Stepper : public QObject
{
	Q_OBJECT
	QString msg;
	double _x, _y, _z;
	long _output, _bufferFree, _bufferSize, _jog, _limit;
	bool _stop;
	bool m_isActive;
public:
	Stepper(QObject *parent=0);
	~Stepper();

	QString message() { return msg; }
	void update();

	double X() { return _x; }
	double Y() { return _y; }
	double Z() { return _z; }
	long output() { return _output; }
	long bufferFree() { return _bufferFree; }
	long bufferSize() { return _bufferSize; }
	long jog() { return _jog; }
	long limit() { return _limit; }

	bool isActive() { return m_isActive; }

private:
	bool initStepper();

signals:
	void stepperUpdated();

public slots:
	void jogUp();
	void jogRight();
	void jogDown();
	void jogLeft();
	void jogZUp();
	void jogZDown();
	void stop();
};

#endif // STEPPER_H
