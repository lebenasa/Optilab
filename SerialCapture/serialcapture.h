#ifndef SERIALCAPTURE_H
#define SERIALCAPTURE_H

#include <QObject>

class SerialCapture : public QObject
{
	Q_OBJECT

public:
	SerialCapture(QObject *parent);
	~SerialCapture();

private:
	
};

#endif // SERIALCAPTURE_H
