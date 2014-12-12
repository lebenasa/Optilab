import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import QuickCam 1.0

ApplicationWindow {
    title: qsTr("Serial Capture")
    width: 640
    height: 480
    visible: true

    Camera {
        id: src
        onFrameReady: renderer.updateImage(frame)
    }
    CameraItem {
        id: renderer
        anchors.fill: parent
    }
}
