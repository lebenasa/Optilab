import QtQuick 2.2

Rectangle {
    id: rectangle1
    width: 100
    height: 40
    color: "transparent"
    property var model
    property int currentIndex: 0
    property string title

    Image {
        id: prev
        width: 12
        height: 12
        anchors.top: titleText.bottom
        anchors.topMargin: 2
        anchors.left: parent.left
        anchors.leftMargin: 0
        fillMode: Image.PreserveAspectFit
        source: "img/next.png"
        mirror: true

        SequentialAnimation {
            id: clickAni2
            NumberAnimation { target: prev; property: "scale"; to: 1.2; duration: 100 }
            NumberAnimation { target: prev; property: "scale"; to: 1.0; duration: 100 }
        }


        MouseArea {
            id: mouseArea1
            anchors.fill: parent
            onClicked: {
                (currentIndex > 0) ? --currentIndex : { }
                clickAni2.start()
            }
        }
    }

    Image {
        id: image1
        x: 98
        width: 12
        height: 12
        anchors.top: titleText.bottom
        anchors.topMargin: 2
        anchors.right: parent.right
        anchors.rightMargin: 0
        fillMode: Image.PreserveAspectFit
        source: "img/next.png"

        SequentialAnimation {
            id: clickAni1
            NumberAnimation { target: image1; property: "scale"; to: 1.2; duration: 100 }
            NumberAnimation { target: image1; property: "scale"; to: 1.0; duration: 100 }
        }

        MouseArea {
            id: mouseArea2
            anchors.fill: parent
            onClicked: {
                (currentIndex < model.length-1) ? ++currentIndex : { }
                clickAni1.start()
            }
        }
    }

    SegoeText {
        id: label
        text: model[currentIndex]
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.verticalCenter: prev.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }

    SegoeText {
        id: titleText
        text: title
        anchors.top: parent.top
        anchors.topMargin: 1
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
