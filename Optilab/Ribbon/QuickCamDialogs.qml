import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import QtQuick.Controls.Styles 1.1

ApplicationWindow {
    title: qsTr("Preview")
    width: 640
    height: 480

    Window {
        id: singleImage
        width: 640
        height: 480
        visible: false
        flags: Qt.Dialog
        property alias imageSource: singleImageDG.imageSource
        SingleImage {
            id: singleImageDG
            anchors.fill: parent
        }
    }

    Window {
        id: multiImage
        width: 640
        height: 480
        visible: false
        flags: Qt.Dialog
        property alias imageSource: multiImageDG.imageSource
        MultiImage {
            id: multiImageDG
            anchors.fill: parent
        }
        title: "Preview"
    }

    Window {
        id: videoPreview
        width: 640
        height: 480
        visible: false
        flags: Qt.Dialog
        property alias videoSource: videoPreviewDG.videoSource
        VideoPreview {
            id: videoPreviewDG
            anchors.fill: parent
        }
        title: "Preview"
    }

    Column {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10
        DialogButton {
            text: "Single Image"
            onClicked: {
                singleImage.visible = true
                singleImage.imageSource = "sample/1.jpg"
            }
        }
        DialogButton {
            text: "Multi Image"
            onClicked: {
                multiImage.visible = true
                multiImage.imageSource = ["sample/1.jpg", "sample/2.jpg", "sample/3.jpg", "sample/4.jpg", "sample/5.jpg",
                                          "sample/6.jpg", "sample/7.jpg", "sample/8.jpg", "sample/9.jpg", "sample/a.jpg", "sample/b.jpg",
                                          "sample/c.jpg", "sample/d.jpg", "sample/e.jpg", "sample/f.jpg", "sample/g.jpg", "sample/h.jpg"]
            }
        }
        DialogButton {
            text: "Video Preview"
            onClicked: {
                videoPreview.visible = true
                videoPreview.videoSource = "sample/record.avi"
            }
        }
    }
}
