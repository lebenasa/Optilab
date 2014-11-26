import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.0

ApplicationWindow {
    id: window
    title: qsTr("Calibration")
    width: 1280
    height: 720
    x: (Screen.width - width) / 2
    y: 35

    color: "#222"

    Image {
        anchors.fill: parent
        source: "tester.jpg"
    }

    VSpace { id: vSpace    }
    HSpace { id: hSpace    }
    Text {
        anchors {
            top: parent.top; left: parent.left
        }
        text: hSpace.length + "x" + vSpace.length
        color: "magenta"
    }

    function saveCalibration(pf) {
        saveDialog.profile = pf
        saveDialog.visible = true
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
        }
    }

    MessageDialog {
        id: revertDialog
        title: "Reset Calibration Profile"
        text: "Reset calibration profile to factory value?"
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        onAccepted: {
        }
    }

    Item {
        width: 32*6 + 10*6
        anchors {
            top: parent.top; topMargin: 10
            right: parent.right; rightMargin: 20
        }
        Row {
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
