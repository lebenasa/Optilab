import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1
import QuickCam 1.0
import Stepper 1.0

ApplicationWindow {
    id: window
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

    SplitView {
        anchors.fill: parent
        Rectangle {
            color: "#222"
            Layout.fillWidth: true
            ScrollView {
                id: gridArea
    //            contentWidth: cameraGrid.width
    //            contentHeight: cameraGrid.height
                anchors.fill: parent
                clip: true
                Rectangle {
                    color: "#222"
                    width: src.sourceSize.width; height: src.sourceSize.height
                    Grid {
                        id: cameraGrid
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
                                blocked: true
                                renderParams: CameraItem.ScaledToItem
                                Connections {
                                    target: src
                                    onFrameReady: cameraDelegate.updateImage(frame)
                                }
                            }
                        }
                    }
                }
            }
        }
        Rectangle {
            id: tempCtrlPanel
            Layout.minimumWidth: 150
            color: "#222"
            anchors {
                right: parent.right; top: parent.top;
                bottom: parent.bottom
            }
        }
    }
}
