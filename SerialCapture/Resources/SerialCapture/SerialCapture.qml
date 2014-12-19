import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1
import QuickCam 1.0

ApplicationWindow {
    id: window
    title: qsTr("Serial Capture Prototype")
    width: 1280
    height: 720
    visible: true

    SplitView {
        anchors.fill: parent
        Rectangle {
            color: "#222"
            Layout.fillWidth: true
            ScrollView {
                id: gridArea
                anchors.fill: parent
                clip: true
                Rectangle {
                    color: "#222"
                    width: serialcapture.cellSize.width * cameraGrid.columns
                    height: serialcapture.cellSize.height * cameraGrid.rows
                    Grid {
                        id: cameraGrid
                        spacing: 0
                        columns: cammodel.cols
                        rows: cammodel.rows
                        anchors.fill: parent
                        Repeater {
                            id: cameraArray
                            model: cammodel
                            Rectangle {
                                id: delegateBorder
                                width: serialcapture.cellSize.width
                                height: serialcapture.cellSize.height
                                color: "transparent"
                                border.width: 1
                                border.color: selected ? "yellow" : delegateMouse.containsMouse ? "magenta" : "green"
                                CameraItem {
                                    id: cameraDelegate
                                    anchors.fill: parent
                                    anchors.margins: 1
                                    blocked: false
                                    source: buffer
                                    renderParams: CameraItem.ScaledToItem
                                }
//                                MouseArea {
//                                    id: delegateMouse
//                                    anchors.fill: parent
//                                    hoverEnabled: true
//                                    onPressed: {
//                                        if (mouse.button == Qt.LeftButton) {
//                                            cammodel.clearSelection()
//                                            selected = true
//                                        }
//                                    }
//                                }
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
            Column {
                spacing: 5
                anchors.fill: parent
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "%1x%2".arg(stepper.x).arg(stepper.y)
                    color: "magenta"
                }
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "%1x%2".arg(istep.cellPos.x).arg(istep.cellPos.y)
                    color: "magenta"
                }
                Button {
                    id: moveButton
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Move"
                    onClicked: serialcapture.moveToSelected()
                }
            }
        }
    }
}
