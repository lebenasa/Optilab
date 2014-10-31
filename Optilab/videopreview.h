#ifndef VIDEOPREVIEW_H
#define VIDEOPREVIEW_H

#include <QDialog>
#include <QQuickItem>
#include <QQuickView>

class VideoPreview : public QDialog
{
	Q_OBJECT

public:
	VideoPreview(QWidget *parent = 0);
	VideoPreview(const QString& path, QWidget* parent = 0);
	~VideoPreview();

	QSize sizeHint() const;

	void setVideoPath(const QString& path);

	public slots:
	void saveVideo();

private:
	QQuickView* dialog;
	QQuickItem* root;
	void initDialog();

	QString m_vidPath;
};

#endif // VIDEOPREVIEW_H
