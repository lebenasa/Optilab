import QtQuick 2.2

Rectangle {
    id: rectangle1
    width: 640
    height: 480

    signal accept
    signal discard

    property var imageSource
    property int index: 0

    Image {
        id: preview
        anchors.fill: parent
        anchors.topMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.bottomMargin: 60
        source: imageSource[index]
        sourceSize: parent.size
        fillMode: Image.PreserveAspectFit
    }

    Image {
        id: prev
        source: "img/next.png"
        rotation: 180
        anchors {
            left: parent.left; leftMargin: 5
            verticalCenter: preview.verticalCenter
        }
    }

    Image {
        id: next
        source: "img/next.png"
        anchors {
            right: parent.right; rightMargin: 5
            verticalCenter: preview.verticalCenter
        }
    }

    SequentialAnimation {
        id: prevClicked
        NumberAnimation { target: prev; property: "scale"; duration: 50; to: 1.2 }
        NumberAnimation { target: prev; property: "scale"; duration: 50; to: 1.0 }
    }

    SequentialAnimation {
        id: nextClicked
        NumberAnimation { target: next; property: "scale"; duration: 50; to: 1.2 }
        NumberAnimation { target: next; property: "scale"; duration: 50; to: 1.0 }
    }

    MouseArea {
        id: prevMouse
        anchors.fill: prev
        onClicked: {
            prevClicked.start()
            if (index > 0) --index
        }
    }

    MouseArea {
        id: nextMouse
        anchors.fill: next
        onClicked: {
            nextClicked.start()
            if (index < imageSource.length-1) ++index
        }
    }

    Text {
        id: indicator
        text: index+1 + "/" + imageSource.length
        anchors {
            top: preview.bottom; topMargin: 5
            horizontalCenter: preview.horizontalCenter
        }
    }

    DialogButton {
        text: "SAVE"
        anchors {
            top: indicator.bottom; topMargin: 5
            horizontalCenter: parent.horizontalCenter
            horizontalCenterOffset: -1.0 * parent.width / 4
        }
        onClicked: accept()
    }

    DialogButton {
        text: "DISCARD"
        anchors {
            top: indicator.bottom; topMargin: 5
            horizontalCenter: parent.horizontalCenter
            horizontalCenterOffset: 1.0 * parent.width / 4
        }
        onClicked: discard()
    }
}
