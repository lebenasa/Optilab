#include "stdafx.h"
#include "stepper.h"
#include "api_wrapper.cpp"

#include <chrono>
#include <thread>
#include <bitset>

CEventHandler* pCncApi = NULL;
Stepper* stepper = NULL;

enum Movement {
	Idle, Up, Right, Down, Left, ZUp, ZDown
};

Stepper::Stepper(QObject *parent)
	: QObject(parent)
{
	stepper = this;
	initStepper();
	xLimit = yLimit = zLimit = false;
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

	std::bitset<8> lim = _limit;
	if (lim.at(2) == true) {
		stop();
		coord->X = 0.0;
		pCncApi->mObject->SendSetPos(coord);
	}

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

const double DIST = 500;
const double SPEED = 100;
void Stepper::jogUp() {
	if (!pCncApi->mObject || pCncApi->mObject->GetBufferFree() < 13) return;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Y, DIST, SPEED, UnitsEnum_Millimeters);
	movementCode = Up;
}

void Stepper::jogRight() {
	if (!pCncApi->mObject || pCncApi->mObject->GetBufferFree() < 13) return;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_X, DIST, SPEED, UnitsEnum_Millimeters);
	movementCode = Right;
}

void Stepper::jogDown() {
	if (!pCncApi->mObject || pCncApi->mObject->GetBufferFree() < 13) return;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Y, -DIST, SPEED, UnitsEnum_Millimeters);
	movementCode = Down;
}

void Stepper::jogLeft() {
	if (!pCncApi->mObject || pCncApi->mObject->GetBufferFree() < 13) return;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_X, -DIST, SPEED, UnitsEnum_Millimeters);
	movementCode = Left;
}

void Stepper::jogZUp() {
	if (!pCncApi->mObject || pCncApi->mObject->GetBufferFree() < 13) return;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Z, DIST, SPEED, UnitsEnum_Millimeters);
	movementCode = ZUp;
}

void Stepper::jogZDown() {
	if (!pCncApi->mObject || pCncApi->mObject->GetBufferFree() < 13) return;
	pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Z, -DIST, SPEED, UnitsEnum_Millimeters);
	movementCode = ZDown;
}

void Stepper::stop() {
	if (pCncApi->mObject) {
		_stop = true;
		pCncApi->mObject->SendStop();
		movementCode = Idle;
	}
}