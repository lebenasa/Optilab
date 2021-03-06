#ifndef STEPPER_H
#define STEPPER_H

#include <QObject>
#include <qtimer.h>

class Stepper : public QObject
{
	Q_OBJECT
	QString msg;
	double _x, _y, _z;
	long _output, _bufferFree, _bufferSize, _jog, _limit;
	bool m_isActive;
	int movementCode;

	double xLim, yLim, zLim, _speed;

	bool _isInitiating;

	QTimer* statusUpdater;
public:
	Stepper(QObject *parent=0);
	~Stepper();

	QString message() { return msg; }
	void updateStatus();

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
	void jogUR();
	void jogDR();
	void jogDL();
	void jogUL();
	void stop(int);

	void moveX(double);
	void moveY(double);
	void moveZ(double);

	void setXLimit(double);
	void setYLimit(double);
	void setZLimit(double);
	void setSpeed(double);

	void initPosition();
};

#endif // STEPPER_H
