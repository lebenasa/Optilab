#include "stdafx.h"
#include "stepper.h"
#include "api_wrapper.cpp"

#include "QSettings"
#include <chrono>
#include <thread>
#include <bitset>

CEventHandler* pCncApi = NULL;
Stepper* stepper = NULL;

enum Movement {
	Idle, Up, Right, Down, Left, ZUp, ZDown, UpRight, DownRight, DownLeft, UpLeft
};

Stepper::Stepper(QObject *parent)
	: QObject(parent)
{
	stepper = this;
	QSettings s("Miconos", "Optilab");
	xLim = s.value("X_LIMIT", 100).toDouble();
	yLim = s.value("Y_LIMIT", 100).toDouble();
	zLim = s.value("Z_LIMIT", 100).toDouble();
	_speed = s.value("SPEED", 100).toDouble();
	initStepper();
	movementCode = 0;
	statusUpdater = new QTimer(this);
	connect(statusUpdater, &QTimer::timeout, this, &Stepper::updateStatus);
	statusUpdater->start(100);
}

Stepper::~Stepper()
{
	pCncApi->DisconnectSink();
	pCncApi->mObject->Dispose();
	pCncApi->mObject->Release();
	pCncApi->mObject = 0;
}

void __cdecl ControllerEventCallBack() {
	stepper->updateStatus();
}

bool Stepper::initStepper() {
	pCncApi = new CEventHandler;
	pCncApi->pEvtCallBack = ControllerEventCallBack;
	HRESULT hr = pCncApi->mObject.CreateInstance(__uuidof(CNC));
	if (pCncApi->mObject == 0) {
		delete pCncApi;
		pCncApi = NULL;
		msg = "Failed to establish connection to Stepper.";
		m_isActive = false;
		return false;
	}
	pCncApi->ConnectSink();

	long result;
	ICoord* pCoord = 0;

	hr = CoInitialize(NULL);

	result = pCncApi->mObject->Initialize();
	if (result == 0) return false;

	msg = "Stepper found! Connection established.";

	ControllerEventCallBack();
	m_isActive = true;
	return true;
}

void Stepper::updateStatus() {
	ICoord* coord = 0;
	coord = pCncApi->mObject->GetPosition();
	_x = coord->GetX();
	_y = coord->GetY();
	_z = coord->GetZ();

	_bufferFree = pCncApi->mObject->GetBufferFree();
	_bufferSize = pCncApi->mObject->GetBufferSize();
	_output = pCncApi->mObject->GetOutput();
	_jog = pCncApi->mObject->GetJog();
	_limit = pCncApi->mObject->GetLimit();

	//std::bitset<8> lim = _limit;
	//if (lim.at(2) == true) {
	//	pCncApi->mObject->SendStop();
	//	coord->X = 0.0;
	//	pCncApi->mObject->SendSetPos(coord);
	//}

	coord->Release();
	coord = NULL;
	emit stepperUpdated();
}

void Stepper::initPosition() {
	ICoord* coord = 0;
	coord = pCncApi->mObject->GetPosition();
	coord->X = 0;
	coord->Y = 0;
	coord->Z = 0;
	pCncApi->mObject->SendMovePos(coord, 100);
	coord->Release();
	coord = NULL;
	//updateStatus();
	//while (_x > 0.1) {
	//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//	updateStatus();
	//}
}

//const double DIST = 500;
//const double SPEED = 100;
void Stepper::jogUp() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST = 0 - _y;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Y, DIST, _speed, UnitsEnum_Millimeters);
	movementCode = Up;
}

void Stepper::jogRight() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST = xLim - _x;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_X, DIST, _speed, UnitsEnum_Millimeters);
	movementCode = Right;
}

void Stepper::jogDown() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST = yLim - _y;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Y, DIST, _speed, UnitsEnum_Millimeters);
	movementCode = Down;
}

void Stepper::jogLeft() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST = 0 - _x;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_X, DIST, _speed, UnitsEnum_Millimeters);
	movementCode = Left;
}

void Stepper::jogZUp() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST = zLim - _z;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Z, DIST, _speed, UnitsEnum_Millimeters);
	movementCode = ZUp;
}

void Stepper::jogZDown() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST = 0 - _z;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Z, DIST, _speed, UnitsEnum_Millimeters);
	movementCode = ZDown;
}

void Stepper::jogUR() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST_X = xLim - _x;
	double DIST_Y = _y;
	auto DIST = (DIST_X < DIST_Y) ? DIST_X : DIST_Y;
	ICoord* coord = 0;
	coord = pCncApi->mObject->GetPosition();
	coord->X = DIST;
	coord->Y = -DIST;
	coord->Z = 0.0;
	pCncApi->mObject->SendMoveDelta(coord, _speed);
	movementCode = UpRight;
	coord->Release();
	coord = NULL;
}

void Stepper::jogDR() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST_X = xLim - _x;
	double DIST_Y = yLim - _y;
	auto DIST = (DIST_X < DIST_Y) ? DIST_X : DIST_Y;
	ICoord* coord = 0;
	coord = pCncApi->mObject->GetPosition();
	coord->X = DIST;
	coord->Y = DIST;
	coord->Z = 0.0;
	pCncApi->mObject->SendMoveDelta(coord, _speed);
	movementCode = DownRight;
	coord->Release();
	coord = NULL;
}

void Stepper::jogDL() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST_X = _x;
	double DIST_Y = yLim - _y;
	auto DIST = (DIST_X < DIST_Y) ? DIST_X : DIST_Y;
	ICoord* coord = 0;
	coord = pCncApi->mObject->GetPosition();
	coord->PutX(-DIST);
	coord->PutY(DIST);
	coord->PutZ(0.0);
	pCncApi->mObject->SendMoveDelta(coord, _speed);
	movementCode = DownLeft;
	coord->Release();
	coord = NULL;
}

void Stepper::jogUL() {
	if (!pCncApi->mObject || movementCode != Idle) return;
	double DIST_X = _x;
	double DIST_Y = _y;
	auto DIST = (DIST_X < DIST_Y) ? DIST_X : DIST_Y;
	ICoord* coord = 0;
	coord = pCncApi->mObject->GetPosition();
	coord->X = -DIST;
	coord->Y = -DIST;
	coord->Z = 0.0;
	pCncApi->mObject->SendMoveDelta(coord, _speed);
	movementCode = UpLeft;
	coord->Release();
	coord = NULL;
}

void Stepper::stop(int code) {
	if (pCncApi->mObject) {
		if (code == movementCode || code == 0) {
			pCncApi->mObject->SendStop();
			movementCode = Idle;
		}
	}
}