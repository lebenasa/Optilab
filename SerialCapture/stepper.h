#ifndef STEPPER_H
#define STEPPER_H

#include <QObject>

/*
Stepper - base class for stepper
- Has property for position, limit and buffer
- Basic movement in XYZ axis with soft limit
- Store and load position limit in registry
*/

using Command = std::function < void() > ;
using CommandPool = std::queue < Command >;

class Stepper : public QObject
{
	Q_OBJECT
	Q_PROPERTY(double x READ x NOTIFY xChanged)
	Q_PROPERTY(double y READ y NOTIFY yChanged)
	Q_PROPERTY(double z READ z NOTIFY zChanged)
	Q_PROPERTY(int bufferFree READ bufferFree NOTIFY bufferFreeChanged)
	Q_PROPERTY(bool limit0 READ limit0 NOTIFY limit0Changed)
	Q_PROPERTY(bool limit1 READ limit0 NOTIFY limit1Changed)
	Q_PROPERTY(bool limit2 READ limit0 NOTIFY limit2Changed)
	Q_PROPERTY(bool limit3 READ limit0 NOTIFY limit3Changed)
	Q_PROPERTY(bool limit4 READ limit0 NOTIFY limit4Changed)
	Q_PROPERTY(bool limit5 READ limit0 NOTIFY limit5Changed)
	Q_PROPERTY(bool limit6 READ limit0 NOTIFY limit6Changed)
	Q_PROPERTY(bool limit7 READ limit0 NOTIFY limit7Changed)
public:
	Stepper(QObject *parent = 0);
	~Stepper();

	virtual double x() = 0;
	virtual double y() = 0;
	virtual double z() = 0;

	virtual int bufferFree() = 0;
	virtual int bufferSize() = 0;

	virtual bool limitAt(int id) { return false; }

	bool limit0() { return limitAt(0); }
	bool limit1() { return limitAt(1); }
	bool limit2() { return limitAt(2); }
	bool limit3() { return limitAt(3); }
	bool limit4() { return limitAt(4); }
	bool limit5() { return limitAt(5); }
	bool limit6() { return limitAt(6); }
	bool limit7() { return limitAt(7); }

	void addCommand(Command);

	virtual bool isWorking() const {
		return m_working;
	}

public slots:
	virtual void jogUp() = 0;
	virtual void jogRight() = 0;
	virtual void jogDown() = 0;
	virtual void jogLeft() = 0;
	virtual void jogZUp() = 0;
	virtual void jogZDown() = 0;
	virtual void jogUR() = 0;
	virtual void jogDR() = 0;
	virtual void jogDL() = 0;
	virtual void jogUL() = 0;
	virtual void stop(int) = 0;

	virtual void moveX(double) = 0;
	virtual void moveY(double) = 0;
	virtual void moveZ(double) = 0;

	virtual void moveTo(const QPointF&) = 0;

	void setXLimit(double);
	void setYLimit(double);
	void setZLimit(double);
	void setSpeed(double);

	void nextCommand();

	void addMoveCommand(int code, double dist);
	void addWaitLimitCommand(int code, int movement);

signals:
	void xChanged(double x);
	void yChanged(double y);
	void zChanged(double z);
	void xyChanged(const QPointF& xy);

	void bufferFreeChanged(int buffer);
	void bufferFull();
	void isWorkingChanged(bool);
	
	void limit0Changed(bool limit);
	void limit1Changed(bool limit);
	void limit2Changed(bool limit);
	void limit3Changed(bool limit);
	void limit4Changed(bool limit);
	void limit5Changed(bool limit);
	void limit6Changed(bool limit);
	void limit7Changed(bool limit);

protected:
	double m_xLim, m_yLim, m_zLim, m_speed;
	CommandPool commandPool;
	void setWorking(bool w);

private:
	bool m_working;
};

/*
CNCStepper - Stepper implementation using CNC API
*/
class CNCStepper : public Stepper
{
	Q_OBJECT
	double m_x, m_y, m_z;
	long m_output, m_bufferFree, m_bufferSize, m_jog;
	std::bitset<8> m_limit;
	int movementCode;
	bool m_isActive = false;
	QTimer* statusUpdater;
public:
	CNCStepper(QObject* parent = 0);
	~CNCStepper();

	double x() { return m_x; }
	double y() { return m_y; }
	double z() { return m_z; }
	int output() { return m_output; }
	int bufferFree() { return m_bufferFree; }
	int bufferSize() { return m_bufferSize; }
	int jog() { return m_jog; }

	bool limitAt(int id) override { return m_limit.at(id); }
	bool isActive() { return m_isActive; }

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

	void moveTo(const QPointF& npos);

	void updateStatus();

protected:
	bool init();
	void deinit();
};

/*
MockStepper - Stepper implementation using dummy stepper
*/
class MockStepper : public Stepper
{
	Q_OBJECT
	double m_z, m_ztarget;
	int m_bufferFree, m_bufferSize;
	std::bitset<8> m_limit;
	int movementCode;
	QPointF m_target, m_pos;
	QTimer* eventDriver;
public:
	MockStepper(QObject* parent = 0);
	~MockStepper();

	double x() { return m_pos.x(); }
	double y() { return m_pos.y(); }
	double z() { return m_z; }
	int bufferFree() { return m_bufferFree; }
	int bufferSize() { return m_bufferSize; }

	bool limitAt(int id) override { return m_limit.at(id); }

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

	void moveTo(const QPointF& npos);

	void updateStatus();

private:
	void setTarget(const QPointF&);
	void setTarget(double x, double y);
};

#endif // STEPPER_H
