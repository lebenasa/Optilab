#include "stdafx.h"
#include "stepper.h"

#include "api_wrapper.h"

//Stepper
Stepper::Stepper(QObject *parent)
	: QObject(parent), m_working(false)
{
	QSettings s("Miconos", "Optilab");
	m_xLim = s.value("X_LIMIT", 100).toDouble();
	m_yLim = s.value("Y_LIMIT", 100).toDouble();
	m_zLim = s.value("Z_LIMIT", 100).toDouble();
	m_speed = s.value("SPEED", 100).toDouble();
}

Stepper::~Stepper()
{

}

void Stepper::setWorking(bool w) {
	if (m_working != w) {
		m_working = w;
		emit isWorkingChanged(m_working);
	}
}

void Stepper::setXLimit(double lim) {
	m_xLim = lim;
	QSettings s("Miconos", "Optilab");
	s.setValue("X_LIMIT", lim);
}

void Stepper::setYLimit(double lim) {
	m_yLim = lim;
	QSettings s("Miconos", "Optilab");
	s.setValue("Y_LIMIT", lim);
}

void Stepper::setZLimit(double lim) {
	m_zLim = lim;
	QSettings s("Miconos", "Optilab");
	s.setValue("Z_LIMIT", lim);
}

void Stepper::setSpeed(double spd) {
	m_speed = spd;
	QSettings s("Miconos", "Optilab");
	s.setValue("SPEED", spd);
}

void Stepper::addCommand(Command cmd) {
	commandPool.push(cmd);
}

//Call this function to start all command sequence
void Stepper::nextCommand() {
	disconnect(this, &Stepper::bufferFull, this, &Stepper::nextCommand);
	auto cmd = commandPool.front();
	cmd();
	commandPool.pop();
	connect(this, &Stepper::bufferFull, this, &Stepper::nextCommand);
}

//Convenient functions
void Stepper::addMoveCommand(int code, double dist) {
	switch (code) {
	case 0:
		addCommand([=]() { moveX(dist); });
		break;
	case 1:
		addCommand([=]() { moveY(dist); });
		break;
	case 2:
		addCommand([=]() { moveZ(dist); });
		break;
	}
}

//Currently only support code 2 (X Axis) and code 0 (Y Axis)
void Stepper::addWaitLimitCommand(int code, int movement) {
	if (code == 2) {
		auto cmd1 = [this]() {
			moveX(-1000);
			connect(this, &Stepper::limit2Changed, this, &Stepper::nextCommand);
		};
		auto cmd2 = [this]() { stop(0); };
		addCommand(cmd1);
		addCommand(cmd2);
	}
	else if (code == 0) {
		auto cmd1 = [this]() {
			moveY(-1000);
			connect(this, &Stepper::limit0Changed, this, &Stepper::nextCommand);
		};
		auto cmd2 = [this]() { stop(0); };
		addCommand(cmd1);
		addCommand(cmd2);
	}
}

//CNCStepper
CEventHandler* pCncApi = NULL;
CNCStepper* stepper = NULL;

CNCStepper::CNCStepper(QObject* parent)
	: Stepper(parent)
{
	stepper = this;
	movementCode = 0;
	statusUpdater = new QTimer(this);
	connect(statusUpdater, &QTimer::timeout, this, &CNCStepper::updateStatus);
	statusUpdater->start(100);
	m_isActive = init();
}

CNCStepper::~CNCStepper()
{
	deinit();
}

void __cdecl ControllerEventCallBack() {
	stepper->updateStatus();
}

bool CNCStepper::init() {
	pCncApi = new CEventHandler;
	pCncApi->pEvtCallBack = ControllerEventCallBack;
	HRESULT hr = pCncApi->mObject.CreateInstance(__uuidof(CNC));
	if (pCncApi->mObject == 0) {
		delete pCncApi;
		pCncApi = NULL;
		return false;
	}
	pCncApi->ConnectSink();

	long result;
	ICoord* pCoord = 0;

	hr = CoInitialize(NULL);

	result = pCncApi->mObject->Initialize();
	if (result == 0) {
		return false;
	}
	
	ControllerEventCallBack();
	return true;
}

void CNCStepper::deinit() {
	pCncApi->DisconnectSink();
	pCncApi->mObject->Dispose();
	pCncApi->mObject->Release();
	pCncApi->mObject = 0;
}

void CNCStepper::updateStatus() {
	ICoord *coord = 0;
	coord = pCncApi->mObject->GetPosition();
	double _x = coord->GetX();
	double _y = coord->GetY();
	double _z = coord->GetZ();
	if (m_x != _x) {
		m_x = _x;
		emit xChanged(m_x);
	}
	if (m_y != _y) {
		m_y = _y;
		emit yChanged(m_y);
	}
	emit xyChanged(QPointF(m_x, m_y));
	if (m_z != _z) {
		m_z = _z;
		emit zChanged(m_z);
	}
	coord->Release();
	coord = NULL;

	long _bufferFree = pCncApi->mObject->GetBufferFree();
	if (m_bufferFree != _bufferFree) {
		m_bufferFree = _bufferFree;
		emit bufferFreeChanged(m_bufferFree);
		if (m_bufferFree == m_bufferSize) {
			emit bufferFull();
			setWorking(false);
		}
		else
			setWorking(true);
	}
	
	m_bufferSize = pCncApi->mObject->GetBufferSize();
	m_output = pCncApi->mObject->GetOutput();
	m_jog = pCncApi->mObject->GetJog();

	long lim = pCncApi->mObject->GetLimit();
	std::bitset<8> limit = lim;

	for (int n = 0; n < 8; ++n) {
		if (m_limit.at(n) != limit.at(n)) {
			m_limit[n] = limit[n];
			if (n == 0)
				emit limit0Changed(m_limit[0]);
			else if (n == 1)
				emit limit0Changed(m_limit[1]);
			else if (n == 2)
				emit limit0Changed(m_limit[2]);
			else if (n == 3)
				emit limit0Changed(m_limit[3]);
			else if (n == 4)
				emit limit0Changed(m_limit[4]);
			else if (n == 5)
				emit limit0Changed(m_limit[5]);
			else if (n == 6)
				emit limit0Changed(m_limit[6]);
			else if (n == 7)
				emit limit0Changed(m_limit[7]);
		}
	}
}

enum Movement {
	Idle, Up, Right, Down, Left, ZUp, ZDown, UpRight, DownRight, DownLeft, UpLeft
};

void CNCStepper::jogUp() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST = 0 - m_y;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Y, DIST, m_speed, UnitsEnum_Millimeters);
	movementCode = Up;
}

void CNCStepper::jogRight() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST = m_xLim - m_x;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_X, DIST, m_speed, UnitsEnum_Millimeters);
	movementCode = Right;
}

void CNCStepper::jogDown() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST = m_yLim - m_y;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Y, DIST, m_speed, UnitsEnum_Millimeters);
	movementCode = Down;
}

void CNCStepper::jogLeft() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST = 0 - m_x;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_X, DIST, m_speed, UnitsEnum_Millimeters);
	movementCode = Left;
}

void CNCStepper::jogZUp() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST = m_zLim - m_z;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Z, DIST, m_speed, UnitsEnum_Millimeters);
	movementCode = ZUp;
}

void CNCStepper::jogZDown() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST = 0 - m_z;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Z, DIST, m_speed, UnitsEnum_Millimeters);
	movementCode = ZDown;
}

void CNCStepper::jogUR() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST_X = m_xLim - m_x;
	double DIST_Y = m_y;
	auto DIST = (DIST_X < DIST_Y) ? DIST_X : DIST_Y;
	ICoord* coord = 0;
	coord = pCncApi->mObject->GetPosition();
	coord->X = DIST;
	coord->Y = -DIST;
	coord->Z = 0.0;
	pCncApi->mObject->SendMoveDelta(coord, m_speed);
	movementCode = UpRight;
	coord->Release();
	coord = NULL;
}

void CNCStepper::jogDR() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST_X = m_xLim - m_x;
	double DIST_Y = m_yLim - m_y;
	auto DIST = (DIST_X < DIST_Y) ? DIST_X : DIST_Y;
	ICoord* coord = 0;
	coord = pCncApi->mObject->GetPosition();
	coord->X = DIST;
	coord->Y = DIST;
	coord->Z = 0.0;
	pCncApi->mObject->SendMoveDelta(coord, m_speed);
	movementCode = DownRight;
	coord->Release();
	coord = NULL;
}

void CNCStepper::jogDL() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST_X = m_x;
	double DIST_Y = m_yLim - m_y;
	auto DIST = (DIST_X < DIST_Y) ? DIST_X : DIST_Y;
	ICoord* coord = 0;
	coord = pCncApi->mObject->GetPosition();
	coord->PutX(-DIST);
	coord->PutY(DIST);
	coord->PutZ(0.0);
	pCncApi->mObject->SendMoveDelta(coord, m_speed);
	movementCode = DownLeft;
	coord->Release();
	coord = NULL;
}

void CNCStepper::jogUL() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST_X = m_x;
	double DIST_Y = m_y;
	auto DIST = (DIST_X < DIST_Y) ? DIST_X : DIST_Y;
	ICoord* coord = 0;
	coord = pCncApi->mObject->GetPosition();
	coord->X = -DIST;
	coord->Y = -DIST;
	coord->Z = 0.0;
	pCncApi->mObject->SendMoveDelta(coord, m_speed);
	movementCode = UpLeft;
	coord->Release();
	coord = NULL;
}

void CNCStepper::stop(int code) {
	if (pCncApi->mObject) {
		if (code == movementCode || code == 0) {
			pCncApi->mObject->SendStop();
			movementCode = Idle;
		}
	}
}

void CNCStepper::moveX(double dist) {
	if (!pCncApi->mObject || movementCode != Idle) return;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_X, dist, m_speed, UnitsEnum_Millimeters);
}

void CNCStepper::moveY(double dist) {
	if (!pCncApi->mObject || movementCode != Idle) return;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Y, dist, m_speed, UnitsEnum_Millimeters);
}

void CNCStepper::moveZ(double dist) {
	if (!pCncApi->mObject || movementCode != Idle) return;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Z, dist, m_speed, UnitsEnum_Millimeters);
}

void CNCStepper::moveTo(const QPointF& npos) {
	if (!pCncApi->mObject) return;
	ICoord* coord = 0;
	coord = pCncApi->mObject->GetPosition();
	coord->X = npos.x();
	coord->Y = npos.y();
	pCncApi->mObject->SendMovePos(coord, m_speed);
	coord->Release();
	coord = nullptr;
}