#ifndef SINGLEIMAGE_H
#define SINGLEIMAGE_H

#include <QDialog>
#include <QQuickItem>
#include <QQuickView>

class SingleImage : public QDialog
{
	Q_OBJECT

public:
	SingleImage(QWidget *parent = 0);
	SingleImage(const QString& path, QWidget* parent = 0);
	~SingleImage();

	QSize sizeHint() const;

	void setImagePath(const QString& path);

public slots:
	void saveImage();

private:
	QQuickView* dialog;
	QQuickItem* root;
	void initDialog();

	QString m_imgPath;
};

#endif // SINGLEIMAGE_H
