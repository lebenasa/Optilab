#include "stdafx.h"
#include "multiimage.h"
#include "optilab.h"

MultiImage::MultiImage(QWidget *parent)
	: QDialog(parent)
{
	initDialog();
}

MultiImage::MultiImage(const QStringList& sources, QWidget* parent)
	: QDialog(parent)
{
	initDialog();
	setImagePath(sources);
}

MultiImage::~MultiImage()
{
	if (dialog) delete dialog;
}

QSize MultiImage::sizeHint() const {
	return QSize(640, 480);
}

void MultiImage::initDialog() {
	dialog = new QQuickView(QUrl("qrc:/Ribbon/MultiImage.qml"));
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

void MultiImage::setImagePath(const QStringList& path) {
	m_imgPath = path;
	QStringList imgSources;
	for (auto img : m_imgPath) {
		imgSources.append(QUrl::fromLocalFile(img).toString());
	}
	root->setProperty("imageSource", imgSources);
}

void MultiImage::saveImage() {
	QString lastPath = Optilab::loadSettings("SC_LAST_PATH").toString();
	if (lastPath == "")
		lastPath = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).at(0);
	auto target = QFileDialog::getExistingDirectory(this, "Select Output Folder", lastPath);
	if (target != "") {
		int a, b, c, d;
		a = b = c = 0;
		d = 1;
		QDir targetDir(target);
		QString fileName = targetDir.dirName();
		for (auto img : m_imgPath) {
			if (QFile::exists(target + "/" + fileName + QString("_%1%2%3%4.jpg").arg(a).arg(b).arg(c).arg(d)))
				QFile::remove(target + "/" + fileName + QString("_%1%2%3%4.jpg").arg(a).arg(b).arg(c).arg(d));
			QFile::copy(img, target + "/" + fileName + QString("_%1%2%3%4.jpg").arg(a).arg(b).arg(c).arg(d));
			++d;
			if (d == 10) {
				d = 0;
				++c;
				if (c == 10) {
					c = 0;
					++b;
					if (b == 10) {
						b = 0;
						++a;
					}
				}
			}
		}
		Optilab::saveSettings("SC_LAST_PATH", target);
		accept();
	}
}