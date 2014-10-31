#include "stdafx.h"
#include "videopreview.h"
#include "optilab.h"

VideoPreview::VideoPreview(QWidget *parent)
	: QDialog(parent)
{
	initDialog();
}

VideoPreview::VideoPreview(const QString& path, QWidget* parent)
	: QDialog(parent)
{
	initDialog();
	setVideoPath(path);
}

VideoPreview::~VideoPreview()
{
	if (dialog) delete dialog;
}

QSize VideoPreview::sizeHint() const {
	return QSize(640, 480);
}

void VideoPreview::initDialog() {
	dialog = new QQuickView(QUrl("qrc:/Ribbon/VideoPreview.qml"));
	dialog->setResizeMode(QQuickView::SizeRootObjectToView);
	root = dialog->rootObject();
	auto w = QWidget::createWindowContainer(dialog, this);
	auto gridLayout = new QGridLayout(this);
	gridLayout->setSpacing(0);
	gridLayout->setContentsMargins(0, 0, 0, 0);
	gridLayout->addWidget(w, 0, 0, 1, 1);
	connect(root, SIGNAL(accept()), this, SLOT(saveVideo()));
	connect(root, SIGNAL(discard()), this, SLOT(reject()));
	setWindowTitle("Video Preview");
}

void VideoPreview::setVideoPath(const QString& path) {
	m_vidPath = path;
	qDebug() << m_vidPath;
	QUrl vidPath = QUrl::fromLocalFile(m_vidPath);
	root->setProperty("videoSource", vidPath);
	//QMetaObject::invokeMethod(root, "play");
}

void VideoPreview::saveVideo() {
	root->setProperty("videoSource", "");
	QString lastPath = Optilab::loadSettings("VIDEO_LAST_PATH").toString();
	if (lastPath == "")
		lastPath = QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).at(0);
	auto target = QFileDialog::getSaveFileName(this, "Save Video", lastPath,
		"AVI Video files (*.avi)");
	if (target != "") {
		if (QFile::exists(target))
			QFile::remove(target);
		QFile::copy(m_vidPath, target);
		Optilab::saveSettings("VIDEO_LAST_PATH", target);
		accept();
	}
	else {
		QUrl vidPath = QUrl::fromLocalFile(m_vidPath);
		root->setProperty("videoSource", vidPath);
	}
}
