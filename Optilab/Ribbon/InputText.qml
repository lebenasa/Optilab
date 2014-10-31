import QtQuick 2.2

Text {
    id: root
    property int tens : 0
    property int ones : 0
    property int inputPos: 0
    property string timeType : "hr"
    signal nextFocus
    signal enterPressed

    verticalAlignment: Text.AlignVCenter
    font.family: "Segoe UI"
    font.weight: Font.DemiBold
    font.pointSize: 32
    color: (activeFocus) ? "cornflowerblue" : "#555"
    text: "%1%2".arg(tens).arg(ones)
    Text {
        anchors {
            right: parent.right; rightMargin: 3
            bottom: parent.top; bottomMargin: -12
        }
        text: timeType
        color: "maroon"
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.focus = true
            inputPos = 0
        }
    }

    function inputNumber(num) {
        if (inputPos == 0) {
            tens = 0
            ones = num
            inputPos = 1
        }
        else {
            tens = (ones < 6) ? ones : 6
            ones = (tens < 6) ? num : 0
            inputPos = 0
            nextFocus()
        }
    }

    function queryKeyEvent(event) {
        if (event.key == Qt.Key_0) {
            inputNumber(0)
            event.accepted = true
        }
        else if (event.key == Qt.Key_1) {
            inputNumber(1)
            event.accepted = true
        }
        else if (event.key == Qt.Key_2) {
            inputNumber(2)
            event.accepted = true
        }
        else if (event.key == Qt.Key_3) {
            inputNumber(3)
            event.accepted = true
        }
        else if (event.key == Qt.Key_4) {
            inputNumber(4)
            event.accepted = true
        }
        else if (event.key == Qt.Key_5) {
            inputNumber(5)
            event.accepted = true
        }
        else if (event.key == Qt.Key_6) {
            inputNumber(6)
            event.accepted = true
        }
        else if (event.key == Qt.Key_7) {
            inputNumber(7)
            event.accepted = true
        }
        else if (event.key == Qt.Key_8) {
            inputNumber(8)
            event.accepted = true
        }
        else if (event.key == Qt.Key_9) {
            inputNumber(9)
            event.accepted = true
        }
        else if (event.key == Qt.Key_Enter || event.key == Qt.Key_Return) {
            enterPressed()
            event.accepted = true
        }
    }

    Keys.onPressed: {
        queryKeyEvent(event)
    }

    SequentialAnimation {
        id: animation
        running: activeFocus
        NumberAnimation { target: root; property: "scale"; duration: 150; to: 1.3 }
        NumberAnimation { target: root; property: "scale"; duration: 150; to: 1.0 }
    }

//    ParallelAnimation {
//        id: animation
//        running: activeFocus
//        SpringAnimation { target: root; property: "scale"; duration: 1000; spring: 2; damping: 0.2; from: 1.2; to: 1.0}
//        SequentialAnimation {
//            PropertyAnimation { target: root; property: "color"; duration: 200; to: "white" }
//            PropertyAnimation { target: root; property: "color"; duration: 200; to: "cornflowerblue" }
//        }
//        loops: Animation.Infinite
//        onRunningChanged: {
//            if (!running) root.color = "cornflowerblue"
//        }
//    }
}
