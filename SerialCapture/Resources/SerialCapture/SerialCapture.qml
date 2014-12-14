import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1
//import QuickCam 1.0
//import Stepper 1.0

ApplicationWindow {
    id: window
    title: qsTr("Serial Capture Prototype")
    width: 640
    height: 480
    visible: true

//    Camera {
//        id: src
//    }
//    Stepper {
//        id: stepper
//    }

    property size src: "128x102"
    function clearSelection() {
        for (var i = 0; i < cameraArray.model; i++) {
            cameraArray.itemAt(i).selected = false
        }
    }

    function indexToPair(index) {
        var col = index % cameraGrid.rows
        var row = (index - col) / cameraGrid.columns
//        console.log(col, row)
        return Qt.point(~~col, ~~row)
    }

    function pairToIndex(pt) {
        return pt.x + pt.y * cameraGrid.rows
    }

    function pointToPair(point) {
        var x = ~~(point.x / src.width)
        var y = ~~(point.y / src.height)
        return Qt.point(x, y)
    }

    function pointToIndex(point) {
        return pairToIndex(pointToPair(point))
    }

    function boxSelect(p1, p2) {

    }

    Component.onCompleted: {
//        console.log(indexToPair(pairToIndex(Qt.point(3, 5))))
//        cameraArray.itemAt(pointToIndex(Qt.point(400, 100))).selected = true
    }

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
//                    width: src.sourceSize.width; height: src.sourceSize.height
                    width: src.width * cameraGrid.columns
                    height: src.height * cameraGrid.rows
                    Grid {
                        id: cameraGrid
                        spacing: 0
                        columns: 10
                        rows: 10
                        anchors.fill: parent
                        Repeater {
                            id: cameraArray
                            model: 100
//                            CameraItem {
//                                id: cameraDelegate
//                                width: src.sourceSize.width / 10
//                                height: src.sourceSize.height / 10
//                                blocked: true
//                                renderParams: CameraItem.ScaledToItem
//                                Connections {
//                                    target: src
//                                    onFrameReady: cameraDelegate.updateImage(frame)
//                                }
//                            }
                            Rectangle {
                                id: cameraDelegate
                                width: src.width; height: src.height
                                color: "black"
                                property bool highlight: false
                                property bool selected: false
                                border.width: 1
                                border.color: (selected) ? "yellow" : (highlight) ? "magenta" : "transparent"
                            }
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
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
