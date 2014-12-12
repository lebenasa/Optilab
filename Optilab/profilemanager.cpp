#include "stdafx.h"
#include "profilemanager.h"

ProfileManager::ProfileManager(QObject *parent)
	: QObject(parent), s("Miconos", "Optilab", this)
{

}

ProfileManager::~ProfileManager()
{

}

//void ProfileManager::saveProfile(int &index, QSize &length, QSize &real, QSize pixel) {
//	auto width = calibrate(length.width(), real.width(), pixel.width());
//	auto height = calibrate(length.height(), real.height(), pixel.height());
//	QString target = "PROFILE_" + QString().number(index);
//	if (width > 0.0 && height > 0.0) {
//		s.setValue(target, QSizeF(width, height));
//	}
//}

void ProfileManager::saveProfile(const int &index, const int& lw, const int& rw, const int& lh, const int &rh, const int &pw, const int &ph) {
	auto width = calibrate(lw, rw, pw);
	auto height = calibrate(lh, rh, ph);
	QString target = "PROFILE_" + QString().number(index);
	if (width > 0.0 && height > 0.0) {
		s.setValue(target, QSizeF(width, height));
	}
}

void ProfileManager::loadDefault() {
	s.setValue("PROFILE_0", QSizeF());
	s.setValue("PROFILE_1", QSizeF());
	s.setValue("PROFILE_2", QSizeF());
	s.setValue("PROFILE_3", QSizeF());
	s.setValue("PROFILE_4", QSizeF());
}

QSizeF ProfileManager::loadProfile(const int &index) {
	QString target = "PROFILE_" + QString().number(index);
	return s.value(target).toSizeF();
}

double ProfileManager::calibrate(const int &length, const int &real, const int &pixel) {
	return 1.0 * pixel / length * real;
}
