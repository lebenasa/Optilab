#ifndef QUICKCAM_H
#define QUICKCAM_H

#include <QQuickItem>
#include <QQueue>
#include <QStack>
#include <QMutex>


#define NOMINMAX
#include <Windows.h>
#include <atlstr.h>

#define WIN32_LEAN_AND_MEAN
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>

class QSGTexture;
class QuickCam : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(bool available READ isAvailable NOTIFY availableChanged)
	Q_PROPERTY(int captureMode READ captureMode WRITE setCaptureMode NOTIFY captureModeChanged)
	Q_PROPERTY(int captureResolution READ captureRes WRITE setCaptureRes NOTIFY captureResolutionChanged)
	Q_PROPERTY(int videoCompressor READ videoCompressor WRITE setVideoCompressor NOTIFY videoCompressorChanged)
	Q_PROPERTY(bool awbEnabled READ isAWBEnable WRITE enableAWB NOTIFY awbEnableChanged)
	Q_PROPERTY(bool aeEnabled READ isAEEnable WRITE enableAE NOTIFY aeEnableChanged)
	Q_PROPERTY(int rGain READ rGain WRITE setRGain NOTIFY rGainChanged)
	Q_PROPERTY(int gGain READ gGain WRITE setGGain NOTIFY gGainChanged)
	Q_PROPERTY(int bGain READ bGain WRITE setBGain NOTIFY bGainChanged)
	Q_PROPERTY(int contrast READ contrast WRITE setContrast NOTIFY contrastChanged)
	Q_PROPERTY(int saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)
	Q_PROPERTY(int gamma READ gamma WRITE setGamma NOTIFY gammaChanged)
	Q_PROPERTY(int aeTarget READ aeTarget WRITE setAETarget NOTIFY aeTargetChanged)
	Q_PROPERTY(int aeGain READ aeGain WRITE setAEGain NOTIFY aeGainChanged)
	Q_PROPERTY(int exposureTime READ exposureTime WRITE setExposureTime NOTIFY exposureTimeChanged)
	Q_PROPERTY(double zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
	Q_PROPERTY(int burstInterval READ burstInterval WRITE setBurstInterval NOTIFY burstIntervalChanged)
	Q_PROPERTY(int burstDuration READ burstDuration WRITE setBurstDuration NOTIFY burstDurationChanged)
	Q_PROPERTY(int burstCount READ burstCount WRITE setBurstCount NOTIFY burstCountChanged)
	Q_PROPERTY(QString streamResolutionString READ streamResolutionString NOTIFY streamResChanged)
	Q_PROPERTY(QSize sourceSize READ sourceSize NOTIFY streamResChanged)

	QQueue<QImage> frames;
	QStack<QImage> buffer;
	//QQueue<QSGTexture*> textureBuff;
	QSize frameSize;

	QMutex frameMutex;
	QMutex buffMutex;
	
	bool m_available;
	int m_captureMode;
	int m_captureRes;
	int m_videoCompressor;

	bool m_awbEnabled;
	bool m_aeEnabled;

	double m_zoom;

	int m_streamRes;

	int m_burstInterval;
	int m_burstDuration;
	int m_burstCount;

	QStringList m_tempImagesPath;

	int m_rGain, m_gGain, m_bGain;
	int m_contrast, m_saturation, m_gamma;
	int m_target, m_gain, m_exptime;
public:
	QuickCam(QQuickItem* parent = 0);
	~QuickCam();

	//void paint(QPainter* painter);
	
	void imgProc(BYTE* pBuffer);

	QSize sourceSize() { return frameSize; }

	bool isAvailable() const { return m_available; }
	void setAvailable(bool);
	int captureMode() const { return m_captureMode; }
	void setCaptureMode(int mode);
	int captureRes() const { return m_captureRes; }
	void setCaptureRes(int res);
	int videoCompressor() const { return m_videoCompressor; }
	void setVideoCompressor(int compressor);

	bool isAWBEnable() const;
	void enableAWB(bool enable);
	bool isAEEnable() const;
	void enableAE(bool enable);

	int rGain() const;
	int gGain() const;
	int bGain() const;

	void setRGain(int red);
	void setGGain(int green);
	void setBGain(int blue);

	int contrast() const;
	void setContrast(int ctrs);
	int saturation() const;
	void setSaturation(int sat);
	int gamma() const;
	void setGamma(int gamma);

	int aeTarget() const;
	void setAETarget(int target);
	int exposureTime() const;
	void setExposureTime(int time);
	int aeGain() const;
	void setAEGain(int gain);

	int zoom() const { return m_zoom; }
	void setZoom(double zl);

	int burstInterval() const { return m_burstInterval; }
	void setBurstInterval(int interval);
	int burstDuration() const { return m_burstDuration; }
	void setBurstDuration(int duration);
	int burstCount() const { return m_burstCount; }
	void setBurstCount(int count);

	void singleCapture();
	void timedCapture();
	void countedCapture();

	QString streamResolutionString() const;

signals:
	void frameReady();

	//Capture Mechanism
	//	if captureResolution == streamResolution:
	//		captureFunc()
	//	else:
	//		oldStreamResolution = streamResolution
	//		setStreamResolution(captureResolution)
	//		waitInDifferentThread(3 seconds, oldStreamResolution)
	//
	//	{ThreadWaiterFunc(duration, oldStreamResolution)}
	//	sleep_for(3 seconds)
	//	captureFunc()
	//	setStreamResolution(oldStreamResolution)
	//
	//	{captureFunc()}
	//	collectData()
	//	saveBufToTemp()
	//	emit captureDone()
	//
	//	{onCaptureDone}
	//	int result = previewCapturedFile()
	//	if (result == Accepted):
	//		copyTempToSavePath()

	void captureDone();
	void streamResChanged();

	void availableChanged();
	void cameraNotFound();
	void captureModeChanged(int);
	void captureResolutionChanged(int);
	void videoCompressorChanged(int);
	void awbEnableChanged(bool);
	void aeEnableChanged(bool);
	void rGainChanged(int);
	void gGainChanged(int);
	void bGainChanged(int);
	void contrastChanged(int);
	void saturationChanged(int);
	void gammaChanged(int);

	void aeGainChanged(int);
	void exposureTimeChanged(int);
	void aeTargetChanged(int);

	void zoomChanged();

	void burstIntervalChanged(int);
	void burstDurationChanged(int);
	void burstCountChanged(int);

public slots:
	void restartCamera();
	void capture();
	void startRecording();
	void stopRecording();
	void oneShotAWB();

	int getMaxExposureTime();

	void setStreamResolution(int);

	QStringList tempImagePath() const { return m_tempImagesPath; }
	void saveResult(const QUrl &path);

	void updateEnhancement();
	void updateColorGain();
	void updateAEParams();

	double getAspectRatio();

	QSize calcAspectRatio(int screenWidth, int screenHeight);

protected:
	QSGNode* updatePaintNode(QSGNode*, UpdatePaintNodeData *);
};

#endif // QUICKCAM_H
