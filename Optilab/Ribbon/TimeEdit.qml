import QtQuick 2.2
import QtQuick.Window 2.1

Window {
    id: timeEdit
    property alias ht : hour.tens
    property alias ho : hour.ones
    property alias mt : minutes.tens
    property alias mo : minutes.ones
    property alias st : seconds.tens
    property alias so : seconds.ones
    width: 300
    height: 160
    visible: false
    x: Screen.width /2 - width /2
    y: Screen.height /2 - height /2
    flags: Qt.FramelessWindowHint
    title: "Edit Time"
    modality: Qt.ApplicationModal

    Rectangle {
        id: frame
        color: "transparent"
        focus: true
        anchors.fill: parent
        border.width: 2
        Keys.onPressed: {
            if (event.key == Qt.Key_Enter) close()
        }
        MouseArea {
            anchors.fill: parent
            onClicked: frame.focus = true
        }
    }

    InputText {
        id: hour
        anchors {
            right: separator1.left
            verticalCenter: minutes.verticalCenter
        }
        onNextFocus: seconds.focus = true
        onEnterPressed: close()
    }


    Text {
        id: separator1
        anchors {
            right: minutes.left
            verticalCenter: minutes.verticalCenter
        }
        verticalAlignment: Text.AlignVCenter
        font.family: "Segoe UI"
        font.weight: Font.DemiBold
        font.pointSize: 32
        color: "#555"
        text: ":"
    }
    InputText {
        id: minutes
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter; verticalCenterOffset: -15
        }
        timeType: "min"
        onNextFocus: hour.focus = true
        onEnterPressed: close()
    }
    Text {
        id: separator2
        anchors {
            left: minutes.right
            verticalCenter: minutes.verticalCenter
        }
        verticalAlignment: Text.AlignVCenter
        font.family: "Segoe UI"
        font.weight: Font.DemiBold
        font.pointSize: 32
        color: "#555"
        text: ":"
    }
    InputText {
        id: seconds
        anchors {
            left: separator2.right
            verticalCenter: minutes.verticalCenter
        }
        timeType: "sec"
        onNextFocus: minutes.focus = true
        onEnterPressed: close()
    }

    DialogButton {
        id: okButton
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom; bottomMargin: 20
        }
        text: "OK"
        onClicked: close()
    }

    onVisibilityChanged: {
        if (visible) seconds.focus = true
    }
}
