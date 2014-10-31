#include "stdafx.h"
#include "singleimage.h"
#include "opencv2\opencv.hpp"
#include "optilab.h"

SingleImage::SingleImage(QWidget *parent)
	: QDialog(parent)
{
	initDialog();
}

SingleImage::SingleImage(const QString& path, QWidget* parent)
	: QDialog(parent)
{
	initDialog();
	setImagePath(path);
}

SingleImage::~SingleImage()
{
	if (dialog) delete dialog;
}

QSize SingleImage::sizeHint() const {
	return QSize(640, 480);
}

void SingleImage::initDialog() {
	dialog = new QQuickView(QUrl("qrc:/Ribbon/SingleImage.qml"));
	dialog->setResizeMode(QQuickView::SizeRootObjectToView);
	root = dialog->rootObject();
	auto w = QWidget::createWindowContainer(dialog, this);
	auto gridLayout = new QGridLayout(this);
	gridLayout->setSpacing(0);
	gridLayout->setContentsMargins(0, 0, 0, 0);
	gridLayout->addWidget(w, 0, 0, 1, 1);
	connect(root, SIGNAL(accept()), this, SLOT(saveImage()));
	connect(root, SIGNAL(discard()), this, SLOT(reject()));
	setWindowTitle("Preview");
}

void SingleImage::setImagePath(const QString& path) {
	m_imgPath = path;
	qDebug() << m_imgPath;
	QUrl imgPath = QUrl::fromLocalFile(m_imgPath);
	root->setProperty("imageSource", imgPath);
}

void SingleImage::saveImage() {
	QString lastPath = Optilab::loadSettings("SINGLE_LAST_PATH").toString();
	if (lastPath == "")
		lastPath = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).at(0);
	auto target = QFileDialog::getSaveFileName(this, "Save Image", lastPath,
		"JPEG file - Fast and Nice Compression (*.jpg);;Portable Network Graphics - Slow with Decent Compression (*.png);;Bitmap - Fastest with No Compression (*.bmp)");
	if (target != "") {
		if (QFile::exists(target))
			QFile::remove(target);
		auto mat = cv::imread(m_imgPath.toStdString());
		cv::Mat img;
		cv::resize(mat, img, cv::Size(), 1.75, 1.75, cv::INTER_LANCZOS4);
		cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
		QImage out = QImage(img.data, img.cols, img.rows, QImage::Format_RGB888);
		out.save(target);
		Optilab::saveSettings("SINGLE_LAST_PATH", target);
		accept();
	}
}