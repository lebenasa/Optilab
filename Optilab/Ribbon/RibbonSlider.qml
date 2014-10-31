import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Slider {
    property color grooveColor: "cornflowerblue"
    style: SliderStyle {
        groove: Rectangle {
            implicitHeight: 4
            implicitWidth: 100
            color: "#ccc"
            Rectangle {
                height: 4
                width: styleData.handlePosition
                anchors { left: parent.left; top: parent.top; bottom: parent.bottom }
                color: control.enabled ? grooveColor : "gray"
            }
        }
        handle: Rectangle {
            implicitHeight: 4
            implicitWidth: 5
            color: control.enabled ? grooveColor : "gray"
        }
    }
}
