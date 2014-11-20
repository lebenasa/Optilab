#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QDialog>
#include "ui_interpreter.h"

#include <qscriptengine.h>

class Interpreter : public QDialog
{
	Q_OBJECT

public:
	Interpreter(QWidget *parent = 0);
	~Interpreter();
	
	void operator>>(QString&);		//Process command via interpreter
	void operator<<(QString&);		//Print something to output

	void registerObject(QObject* obj, const QString& name);

private:
	Ui::Interpreter ui;
	QTimer* sync;
	QScriptEngine* engine;

public slots:
	void syncErrOut();
	void procInput();
};

#endif // INTERPRETER_H
