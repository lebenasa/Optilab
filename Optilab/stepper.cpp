#include "stdafx.h"
#include "stepper.h"
#include "api_wrapper.cpp"

CEventHandler* pCncApi = NULL;
Stepper* stepper;

Stepper::Stepper(QObject *parent)
	: QObject(parent)
{
	stepper = this;
	initStepper();
}

Stepper::~Stepper()
{
	pCncApi->DisconnectSink();
	pCncApi->mObject->Dispose();
	pCncApi->mObject->Release();
	pCncApi->mObject = 0;
}

void ControllerEventCallBack() {
	if (stepper)
		stepper->update();
}

bool Stepper::initStepper() {
	pCncApi = new CEventHandler;
	pCncApi->pEvtCallBack = ControllerEventCallBack;
	pCncApi->mObject.CreateInstance(__uuidof(CNC));
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

	CoInitialize(NULL);

	result = pCncApi->mObject->Initialize();
	if (result == 0) return false;

	msg = "Stepper found! Connection established.";

	ControllerEventCallBack();
	m_isActive = true;
	return true;
}

void Stepper::update() {
	ICoord* coord = 0;
	coord = pCncApi->mObject->GetPosition();
	_x = coord->GetX();
	_y = coord->GetY();
	_z = coord->GetZ();
	coord->Release();
	coord = NULL;

	_bufferFree = pCncApi->mObject->GetBufferFree();
	_bufferSize = pCncApi->mObject->GetBufferSize();
	_output = pCncApi->mObject->GetOutput();
	_jog = pCncApi->mObject->GetJog();
	_limit = pCncApi->mObject->GetLimit();

	emit stepperUpdated();
}

const double DIST = 500;
const double SPEED = 100;
void Stepper::jogUp() {
	if (pCncApi->mObject && pCncApi->mObject->GetBufferFree() > 12)
		pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Y, DIST, SPEED, UnitsEnum_Millimeters);
}

void Stepper::jogRight() {
	if (pCncApi->mObject && pCncApi->mObject->GetBufferFree() > 12)
		pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_X, DIST, SPEED, UnitsEnum_Millimeters);
}

void Stepper::jogDown() {
	if (pCncApi->mObject && pCncApi->mObject->GetBufferFree() > 12)
		pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Y, -DIST, SPEED, UnitsEnum_Millimeters);
}

void Stepper::jogLeft() {
	if (pCncApi->mObject && pCncApi->mObject->GetBufferFree() > 12)
		pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_X, -DIST, SPEED, UnitsEnum_Millimeters);
}

void Stepper::jogZUp() {
	if (pCncApi->mObject && pCncApi->mObject->GetBufferFree() > 12)
		pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Z, DIST, SPEED, UnitsEnum_Millimeters);
}

void Stepper::jogZDown() {
	if (pCncApi->mObject && pCncApi->mObject->GetBufferFree() > 12)
		pCncApi->mObject->SendMoveDeltaAxis(AxisEnum_Z, -DIST, SPEED, UnitsEnum_Millimeters);
}

void Stepper::stop() {
	if (pCncApi->mObject) {
		_stop = true;
		pCncApi->mObject->SendStop();
	}
}