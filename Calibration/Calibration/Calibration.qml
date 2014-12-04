import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.0
import QuickCam 1.0

ApplicationWindow {
    id: window
    title: qsTr("Calibration")
    width: 1280
    height: 720
    visible: true

//    flags: Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowSystemMenuHint |
//           Qt.WindowMinimizeButtonHint | Qt.WindowCloseButtonHint | Qt.MSWindowsFixedSizeDialogHint

    color: "#222"

//    Image {
//        anchors.fill: parent
//        source: "tester.jpg"
//    }

    onWidthChanged: calcAspectRatio()
    onHeightChanged: calcAspectRatio()
    Component.onCompleted: calcAspectRatio()

    function calcAspectRatio() {
        var offset = camera.calcAspectRatio(window.width, window.height)
        tm.height = offset.height
        rm.width = offset.width
        bm.height = offset.height
        lm.width = offset.width
    }

    Rectangle {
        id: tm
        width: parent.width
        height: 0
        color: "black"
        anchors {
            top: parent.top; left: parent.left; right: parent.right
        }
    }
    Rectangle {
        id: rm
        width: 0
        height: parent.height
        color: "black"
        anchors {
            right: parent.right; top: parent.top; bottom: parent.bottom
        }
    }
    Rectangle {
        id: bm
        width: parent.width
        height: 0
        color: "black"
        anchors {
            bottom: parent.bottom; left: parent.left; right: parent.right
        }
    }
    Rectangle {
        id: lm
        width: 0
        height: parent.height
        color: "black"
        anchors {
            left: parent.left; top: parent.top; bottom: parent.bottom
        }
    }

    Camera {
        id: camera
        anchors {
            top: tm.bottom; right: rm.left; bottom: bm.top; left: lm.right
        }
        focus: true
        property int currentKey
        Keys.onPressed: {
            if (event.isAutoRepeat) return
            switch (event.key) {
            case Qt.Key_Up:
                stepper.jogUp()
                currentKey = Qt.Key_Up
                break
            case Qt.Key_Right:
                stepper.jogRight()
                currentKey = Qt.Key_Right
                break
            case Qt.Key_Down:
                stepper.jogDown()
                currentKey = Qt.Key_Down
                break
            case Qt.Key_Left:
                stepper.jogLeft()
                currentKey = Qt.Key_Left
                break
            case Qt.Key_Plus:
                stepper.jogZUp()
                currentKey = Qt.Key_Plus
                break
            case Qt.Key_Minus:
                stepper.jogZDown()
                currentKey = Qt.Key_Minus
                break
            }
        }
        Keys.onReleased: {
            if (event.isAutoRepeat) return
            if (event.key === currentKey) {
                stepper.stop(0)
                event.accepted = true
            }
        }

        VSpace { id: vSpace; anchors.fill: camera    }
        HSpace { id: hSpace; anchors.fill: camera    }
    }

    Text {
        anchors {
            top: parent.top; left: parent.left
        }
        text: hSpace.length + "x" + vSpace.length + "\u000d\u000a" +
              "\u2190\u2191\u2192\u2193 : Move XY\u000d\u000a" +
              "+-   : Move Z"
        color: "magenta"
        font.family: "Arial"
    }

    function saveCalibration(pf) {
        saveDialog.profile = pf
        saveDialog.visible = true
    }

    function saveProfile(id) {
        manager.saveProfile(id, hSpace.length, hSpace.getRealLength(),
                            vSpace.length, vSpace.getRealLength(),
                            camera.sourceSize.width, camera.sourceSize.height)
    }

    MessageDialog {
        id: saveDialog
        property string profile: "4x"
        title: "Save Calibration Result"
        text: {
            var msg = "Overwrite calibration data in %1 profile?"
            return msg.arg(profile)
        }
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: {
            switch(profile) {
            case "4x":
                saveProfile(0)
                break
            case "10x":
                saveProfile(1)
                break
            case "40x":
                saveProfile(2)
                break
            case "100x":
                saveProfile(3)
                break
            case "custom":
                saveProfile(4)
                break
            }
        }
    }

    MessageDialog {
        id: revertDialog
        title: "Reset Calibration Profile"
        text: "Reset calibration profile to factory value?"
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: {
            manager.loadDefault()
        }
    }

    Item {
        width: 32
        height: 32*6 + 10*6
        anchors {
            top: parent.top; topMargin: 10
            right: parent.right; rightMargin: 20
        }
        Column {
            spacing: 10
            RoundButton {
                source: "4x.png"
                onClick: saveCalibration("4x")
            }
            RoundButton {
                source: "10x.png"
                onClick: saveCalibration("10x")
            }
            RoundButton {
                source: "40x.png"
                onClick: saveCalibration("40x")
            }
            RoundButton {
                source: "100x.png"
                onClick: saveCalibration("100x")
            }
            RoundButton {
                source: "custom.png"
                onClick: saveCalibration("custom")
            }
            RoundButton {
                source: "revert.png"
                onClick: revertDialog.visible = true
            }
        }
    }
}
