import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.1

Rectangle {
    width: 240
    height: 150

    Rectangle {
        id: dummy
        width: 35
        height: 25
        color: "transparent"
        anchors {
            left: parent.left; leftMargin: 60
            top: parent.top; topMargin: 60
        }
    }

    StepperButton {
        id: up
        anchors {
            horizontalCenter: dummy.horizontalCenter
            bottom: dummy.top;
        }
        MouseArea {
            anchors.fill: parent
        }
    }
    StepperButton {
        id: right
        anchors {
            verticalCenter: dummy.verticalCenter
            left: dummy.right

        }
        rotation: 90
        MouseArea {
            anchors.fill: parent
        }
    }
    StepperButton {
        id: down
        anchors {
            horizontalCenter: dummy.horizontalCenter
            top: dummy.bottom
        }
        rotation: 180
        MouseArea {
            anchors.fill: parent
        }
    }
    StepperButton {
        id: left
        anchors {
            verticalCenter: dummy.verticalCenter
            right: dummy.left

        }
        rotation: 270
        MouseArea {
            anchors.fill: parent
        }
    }

    Rectangle {
        id: zUp
        width: 25
        height: 25
        property bool pushed: false
        anchors {
            right: parent.right; rightMargin: 20
            top: up.top
        }
        Image {
            anchors.fill: parent
            source: parent.pushed ? "img/plus-pushed.png" : "img/plus-normal.png"
            smooth: true
            fillMode: Image.PreserveAspectFit
        }
        MouseArea {
            id: control1
            anchors.fill: parent
            hoverEnabled: true
            onEntered: tooltipUpAnimation.start()
            onExited: {
                tooltipUpAnimation.stop()
                tooltipUp.opacity = 0
            }
        }
        Rectangle {
            id: tooltipUp
            width: 50
            height: 18
            border.width: 1
            color: "white"
            opacity: 0
            Text {
                text: "Page Up"
                font.pointSize: 8
                font.family: "Segoe UI"
                anchors.centerIn: parent
            }
            anchors {
                top: parent.bottom; topMargin: 5
                horizontalCenter: parent.horizontalCenter
            }
        }
        SequentialAnimation {
            id: tooltipUpAnimation
            PropertyAnimation { target: tooltipUp; property: "opacity"; to: 0; duration: 1000 }
            PropertyAnimation { target: tooltipUp; property: "opacity"; to: 1; duration: 250 }
        }
    }
    Rectangle {
        id: zDown
        width: 25
        height: 25
        property bool pushed: false
        anchors {
            right: parent.right; rightMargin: 20
            bottom: down.bottom
        }
        Image {
            anchors.fill: parent
            source: parent.pushed ? "img/min-pushed.png" : "img/min-normal.png"
            smooth: true
            fillMode: Image.PreserveAspectFit
        }
        MouseArea {
            id: control2
            anchors.fill: parent
            hoverEnabled: true
            onEntered: tooltipDownAnimation.start()
            onExited: {
                tooltipDownAnimation.stop()
                tooltipDown.opacity = 0
            }
        }
        Rectangle {
            id: tooltipDown
            width: 50
            height: 18
            border.width: 1
            color: "white"
            opacity: 0
            Text {
                text: "Page Down"
                font.pointSize: 8
                font.family: "Segoe UI"
                anchors.centerIn: parent
            }
            anchors {
                top: parent.bottom; topMargin: -3
                horizontalCenter: parent.horizontalCenter
            }
        }
        SequentialAnimation {
            id: tooltipDownAnimation
            PropertyAnimation { target: tooltipDown; property: "opacity"; to: 0; duration: 1000 }
            PropertyAnimation { target: tooltipDown; property: "opacity"; to: 1; duration: 250 }
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Up) {
            event.accepted = true
        }
        else if (event.key === Qt.Key_Right) {
            event.accepted = true
        }
        else if (event.key === Qt.Key_Down) {
            event.accepted = true
        }
        else if (event.key === Qt.Key_Left) {
            event.accepted = true
        }
        else if (event.key === Qt.Key_PageUp) {
            event.accepted = true
        }
        else if (event.key === Qt.Key_PageDown) {
            event.accepted = true
        }
    }
    Keys.onReleased: {
        if (event.key === Qt.Key_Up) {
            event.accepted = true
        }
        else if (event.key === Qt.Key_Right) {
            event.accepted = true
        }
        else if (event.key === Qt.Key_Down) {
            event.accepted = true
        }
        else if (event.key === Qt.Key_Left) {
            event.accepted = true
        }
        else if (event.key === Qt.Key_PageUp) {
            event.accepted = true
        }
        else if (event.key === Qt.Key_PageDown) {
            event.accepted = true
        }
    }
}
