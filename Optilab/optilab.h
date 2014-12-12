#ifndef OPTILAB_H
#define OPTILAB_H

#include <QtWidgets/QMainWindow>
#include <QQuickItem>
#include <QQuickView>
#include <QMediaPlayer>
#include "ui_optilab.h"

class StepperControl;
class Stepper;
class StepperDebug;
class Interpreter;

enum ResolutionLevel {
	Highest, High, Mid, Low
};

enum VideoCompressor {
	MJPG, MPEG, H264
};

class Optilab : public QMainWindow
{
	Q_OBJECT

public:
	Optilab(QWidget *parent = 0);
	~Optilab();

	void imgProc(cv::Mat &frame);
	int getMaxExposureTime();

	void serialCaptureInternal(int interval, int fcount, int resolution);

	void setStepper(Stepper*);

signals:
	void cameraInitialized();
	void serialCaptureFinished();
	void singleCaptureFinished();
	void requestRestoreResolution();
	void progressUp();

public slots:
	void restartCamera();
	void singleCapture(int resolution);
	void timedCapture(int interval, int duration, int resolution);
	void countedCapture(int interval, int framecount, int resolution);
	void updateProgressBar();
	void saveSerialCapture();
	void startRecording(int compressor);
	void pauseRecording();
	void stopRecording();

	bool isAWBEnabled();
	void enableAWB(bool);
	bool isAEEnabled();
	void enableAE(bool);

	int getStreamResolution();

	void setStreamResolution(int);
	double getRGain();
	void setRGain(double val);
	double getGGain();
	void setGGain(double val);
	double getBGain();
	void setBGain(double val);

	double getGamma();
	void setGamma(double val);
	double getContrast();
	void setContrast(double val);
	double getSaturation();
	void setSaturation(double val);

	double getAEGain();
	void setAEGain(double val);
	double getExposureTime();
	void setExposureTime(double);
	double getAETarget();
	void setAETarget(double);

	void loadProfile(int);
	void saveProfile(int);

	void restoreResolution();

	void updateTimer();

	void calcAspectRatio();
	void toggleRibbon();

	static QVariant loadSettings(const QString& name);
	static void saveSettings(const QString& name, const QVariant& value);

	void showOnScreenControl();

	void viewGrid(int id, int size);	//Size in micrometer
	void hideGrid();

private:
	Ui::OptilabClass ui;
	QQuickView* ribbon;
	QQuickItem* control;
	bool isRibbonVisible;

	int streamResolution;
	double maxRGBGain;
	double maxContrast;
	double maxSaturation;
	double maxGamma;
	double maxAEGain;
	double maxExposureTime;
	double maxAETarget;

	QStringList serialResult;

	QLabel* statusLabel;

	QString videoFile;
	QLabel* timerLabel;
	QTime timeStarted;
	QTimer* timer;
	QProgressBar* scProgress;
	QPushButton* serialCapStop;

	QMediaPlayer* shutter;

	bool ongoingCapture;

	StepperControl* stepperControl;
	Stepper* stepper;
	StepperDebug* stepperDebug;

	Interpreter* interpreter;

	bool _grid;

protected:
	void resizeEvent(QResizeEvent* event);
	void closeEvent(QCloseEvent* event);
};

#endif // OPTILAB_H
