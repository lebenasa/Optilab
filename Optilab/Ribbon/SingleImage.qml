import QtQuick 2.2

Rectangle {
    width: 640
    height: 480

    signal accept
    signal discard

    property url imageSource

    Image {
        id: preview
        anchors.fill: parent
        anchors.topMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.bottomMargin: 60
        source: imageSource
        sourceSize: parent.size
        fillMode: Image.PreserveAspectFit
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
