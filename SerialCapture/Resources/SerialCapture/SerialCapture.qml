import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import QuickCam 1.0
import Stepper 1.0

ApplicationWindow {
    title: qsTr("Serial Capture Prototype")
    width: 640
    height: 480
    visible: true

    Camera {
        id: src
    }
//    Stepper {
//        id: stepper
//    }

    Rectangle {
        anchors.fill: parent
        color: "#333"
        Flickable {
            id: gridArea
            anchors {
                right: tempCtrlPanel.left; top: parent.top
                left: parent.left; bottom: parent.bottom
            }
            contentWidth: cameraGrid.width
            contentHeight: cameraGrid.height
            clip: true
            Grid {
                id: cameraGrid
                width: src.sourceSize.width; height: src.sourceSize.height
                spacing: 0
                columns: 10
                rows: 10
                anchors.fill: parent
                Repeater {
                    id: cameraArray
                    model: 100
                    CameraItem {
                        id: cameraDelegate
                        width: src.sourceSize.width / 10
                        height: src.sourceSize.height / 10
                        blocked: false
                        renderParams: CameraItem.ScaledToItem
                        Connections {
                            target: src
                            onFrameReady: cameraDelegate.updateImage(frame)
                        }
                    }
                }
            }
        }
        Rectangle {
            id: tempCtrlPanel
            width: 150
            color: "#333"
            anchors {
                right: parent.right; top: parent.top;
                bottom: parent.bottom
            }
        }
    }
}
