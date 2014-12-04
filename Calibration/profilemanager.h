#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <QObject>
#include <QSettings>

#include <QSize>
#include <QSizeF>

enum Profile {
	P4x, P10x, P40x, P100x, PCustom
};

class ProfileManager : public QObject
{
	Q_OBJECT

public:
	ProfileManager(QObject *parent = 0);
	~ProfileManager();

public slots:
	//saveProfile
	//Params:
	//	- index : follows Profile enumerator
	//	- lw, lh: length in pixel
	//	- pw, ph: width and height in pixel
	//	- rw, rh: length in micrometer
	//void saveProfile(int &index, QSize &length, QSize &real, QSize size = QSize(1024, 768));
	void saveProfile(const int &index, const int &lw, const int &rw, const int &lh, const int &rh, const int &sw, const int &sh);
	void loadDefault();
	QSizeF loadProfile(const int &index);

private:
	QSettings s;
	double calibrate(const int &length, const int &real, const int &size);
};

#endif // PROFILEMANAGER_H
