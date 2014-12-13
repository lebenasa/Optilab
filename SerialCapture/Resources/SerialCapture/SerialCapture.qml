import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import QuickCam 1.0
import Stepper 1.0

ApplicationWindow {
    title: qsTr("Serial Capture")
    width: 640
    height: 480
    visible: true

    Camera {
        id: src
        onFrameReady: renderer.updateImage(frame)
    }
    Stepper {
        id: stepper
    }

    CameraItem {
        id: renderer
        anchors.fill: parent

//        focus: true
//        property int currentKey
//        Keys.onPressed: {
//            if (event.isAutoRepeat) return
//            switch (event.key) {
//            case Qt.Key_Up:
//                stepper.jogUp()
//                currentKey = Qt.Key_Up
//                break
//            case Qt.Key_Right:
//                stepper.jogRight()
//                currentKey = Qt.Key_Right
//                break
//            case Qt.Key_Down:
//                stepper.jogDown()
//                currentKey = Qt.Key_Down
//                break
//            case Qt.Key_Left:
//                stepper.jogLeft()
//                currentKey = Qt.Key_Left
//                break
//            case Qt.Key_Plus:
//                stepper.jogZUp()
//                currentKey = Qt.Key_Plus
//                break
//            case Qt.Key_Minus:
//                stepper.jogZDown()
//                currentKey = Qt.Key_Minus
//                break
//            }
//        }
//        Keys.onReleased: {
//            if (event.isAutoRepeat) return
//            if (event.key === currentKey) {
//                stepper.stop(0)
//                event.accepted = true
//            }
//        }
    }
}
