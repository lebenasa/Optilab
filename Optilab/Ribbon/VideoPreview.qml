import QtQuick 2.2
import QtMultimedia 5.0

Rectangle {
    width: 640
    height: 480

    signal accept
    signal discard

    property url videoSource

    Video {
        id: preview
        anchors.fill: parent
        anchors.topMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.bottomMargin: 60
        source: videoSource
//        sourceSize: parent.size
        fillMode: VideoOutput.PreserveAspectFit
        autoPlay: true
        onStopped: play()
    }

    DialogButton {
        text: "SAVE"
        anchors {
            top: preview.bottom; topMargin: 15
            horizontalCenter: parent.horizontalCenter
            horizontalCenterOffset: -1.0 * parent.width / 4
        }
        onClicked: accept()
    }

    DialogButton {
        text: "DISCARD"
        anchors {
            top: preview.bottom; topMargin: 15
            horizontalCenter: parent.horizontalCenter
            horizontalCenterOffset: 1.0 * parent.width / 4
        }
        onClicked: discard()
    }
}
