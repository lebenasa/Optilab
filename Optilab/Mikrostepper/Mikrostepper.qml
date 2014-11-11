import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.1

Rectangle {
    width: 240
    height: 150
    property bool keylock: false

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
            onPressed: {
                if (!keylock) {
                    keylock = true
                    up.pushed = true
                    stepper.jogUp()
                }
            }
            onReleased: {
                if(up.pushed == true) {
                    keylock = false
                    up.pushed = false
                    stepper.stop()
                }
            }
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
            onPressed: {
                if (!keylock) {
                    keylock = true
                    right.pushed = true
                    stepper.jogRight()
                }
            }
            onReleased: {
                if(right.pushed == true) {
                    keylock = false
                    right.pushed = false
                    stepper.stop()
                }
            }
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
            onPressed: {
                if (!keylock) {
                    keylock = true
                    down.pushed = true
                    stepper.jogDown()
                }
            }
            onReleased: {
                if(down.pushed == true) {
                    keylock = false
                    down.pushed = false
                    stepper.stop()
                }
            }
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
            onPressed: {
                if (!keylock) {
                    keylock = true
                    left.pushed = true
                    stepper.jogLeft()
                }
            }
            onReleased: {
                if(left.pushed == true) {
                    keylock = false
                    left.pushed = false
                    stepper.stop()
                }
            }
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
            onPressed: {
                if (!keylock) {
                    keylock = true
                    zUp.pushed = true
                    stepper.jogZUp()
                }
            }
            onReleased: {
                if(zUp.pushed == true) {
                    keylock = false
                    zUp.pushed = false
                    stepper.stop()
                }
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
            onPressed: {
                if (!keylock) {
                    keylock = true
                    zDown.pushed = true
                    stepper.jogZDown()
                }
            }
            onReleased: {
                if(zDown.pushed == true) {
                    keylock = false
                    zDown.pushed = false
                    stepper.stop()
                }
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

//    focus: true
//    property bool rightPressed: false
//    Timer {
//        interval: 200; running: true; repeat: true
//        onTriggered: rightPressed = false
//    }
//    onRightPressedChanged: {
//        if (rightPressed) stepper.jogRight()
//        else {
//            rightTimer.running = true
//        }
//    }
//    Timer {
//        id: rightTimer
//        interval: 500
//        onTriggered: {
//            if (!rightPressed) stepper.stop()
//        }
//    }
//    property bool leftPressed: false
//    Timer {
//        interval: 200; running: true; repeat: true
//        onTriggered: leftPressed = false
//    }
//    onLeftPressedChanged: {
//        if (leftPressed) stepper.jogLeft()
//        else {
//            leftTimer.running = true
//        }
//    }
//    Timer {
//        id: leftTimer
//        interval: 500
//        onTriggered: {
//            if (!leftPressed) stepper.stop()
//        }
//    }
//    property bool upPressed: false
//    Timer {
//        interval: 200; running: true; repeat: true
//        onTriggered: upPressed = false
//    }
//    onUpPressedChanged: {
//        if (upPressed) stepper.jogUp()
//        else {
//            upTimer.running = true
//        }
//    }
//    Timer {
//        id: upTimer
//        interval: 500
//        onTriggered: {
//            if (!upPressed) stepper.stop()
//        }
//    }
//    property bool downPressed: false
//    Timer {
//        interval: 200; running: true; repeat: true
//        onTriggered: downPressed = false
//    }
//    onDownPressedChanged: {
//        if (downPressed) stepper.jogDown()
//        else {
//            downTimer.running = true
//        }
//    }
//    Timer {
//        id: downTimer
//        interval: 500
//        onTriggered: {
//            if (!downPressed) stepper.stop()
//        }
//    }
//    property bool zUpPressed: false
//    Timer {
//        interval: 200; running: true; repeat: true
//        onTriggered: zUpPressed = false
//    }
//    onZUpPressedChanged: {
//        if (zUpPressed) stepper.jogZUp()
//        else {
//            zUpTimer.running = true
//        }
//    }
//    Timer {
//        id: zUpTimer
//        interval: 500
//        onTriggered: {
//            if (!zUpPressed) stepper.stop()
//        }
//    }
//    property bool zDownPressed: false
//    Timer {
//        interval: 200; running: true; repeat: true
//        onTriggered: zDownPressed = false
//    }
//    onZDownPressedChanged: {
//        if (zDownPressed) stepper.jogZDown()
//        else {
//            zDownTimer.running = true
//        }
//    }
//    Timer {
//        id: zDownTimer
//        interval: 500
//        onTriggered: {
//            if (!zDownPressed) stepper.stop()
//        }
//    }

//    Keys.onPressed: {
//        if (upPressed | rightPressed | downPressed | leftPressed) return
//        if (event.key === Qt.Key_Up) {
//            event.accepted = true
//            upPressed = true
//        }
//        else if (event.key === Qt.Key_Right) {
//            event.accepted = true
//            rightPressed = true
//        }
//        else if (event.key === Qt.Key_Down) {
//            event.accepted = true
//            downPressed = true
//        }
//        else if (event.key === Qt.Key_Left) {
//            event.accepted = true
//            leftPressed = true
//        }
//        else if (event.key === Qt.Key_PageUp) {
//            event.accepted = true
//            zUpPressed = true
//        }
//        else if (event.key === Qt.Key_PageDown) {
//            event.accepted = true
//            zDownPressed = true
//        }
//    }
//    Keys.onReleased: {
//        if (event.key === Qt.Key_Up) {
//            event.accepted = true
//        }
//        else if (event.key === Qt.Key_Right) {
//            event.accepted = true
//        }
//        else if (event.key === Qt.Key_Down) {
//            event.accepted = true
//        }
//        else if (event.key === Qt.Key_Left) {
//            event.accepted = true
//        }
//        else if (event.key === Qt.Key_PageUp) {
//            event.accepted = true
//        }
//        else if (event.key === Qt.Key_PageDown) {
//            event.accepted = true
//        }
//    }
}
