#include "stdafx.h"
#include "interpreter.h"

Interpreter::Interpreter(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	engine = new QScriptEngine(this);
	ui.input->setClearButtonEnabled(true);
	//sync = new QTimer(this);
	//sync->setInterval(200);
	//connect(sync, &QTimer::timeout, this, &Interpreter::syncErrOut);
	//sync->start();
	connect(ui.input, &QLineEdit::returnPressed, this, &Interpreter::procInput);
}

Interpreter::~Interpreter()
{

}

void Interpreter::operator>>(QString& in) {
	QString res = engine->evaluate(in).toString();
	QString ref = (res.contains("undefined")) ? in : res;
	*this << ref;
}

void Interpreter::operator<<(QString& print) {
	ui.output->insertPlainText(print);
	ui.output->insertPlainText("\n");
}

void Interpreter::syncErrOut() {
	QTextStream stream1(stderr);
	QString line;
	do {
		line = stream1.readLine();
		*this << line;
	} while (!line.isNull());
	//QTextStream stream2(stdout);
	//do {
	//	(*this)(stream2.readLine());
	//} while (!stream2.readLine().isNull());
}

void Interpreter::procInput() {
	//(*this)(ui.input->text());
	//std::cerr << "Error!";
	*this >> ui.input->text();
	ui.input->clear();
}

void Interpreter::registerObject(QObject* obj, const QString& name) {
	if (obj) {
		auto ov = engine->newQObject(obj);
		engine->globalObject().setProperty(name, ov);
	}
}