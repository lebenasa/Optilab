#ifndef MULTIIMAGE_H
#define MULTIIMAGE_H

#include <QDialog>
#include <QQuickItem>
#include <QQuickView>

class MultiImage : public QDialog
{
	Q_OBJECT

public:
	MultiImage(QWidget *parent);
	MultiImage(const QStringList& sources, QWidget* parent);
	~MultiImage();

	QSize sizeHint() const;

	void setImagePath(const QStringList& path);

public slots:
	void saveImage();

private:
	QQuickView* dialog;
	QQuickItem* root;
	void initDialog();

	QStringList m_imgPath;	
};

#endif // MULTIIMAGE_H
