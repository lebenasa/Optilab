import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
import QtQuick.Controls.Styles 1.1

Button {
    property bool drawBorder: true
    property int fontSize: 10
    style: ButtonStyle {
        background: Rectangle {
            implicitWidth: 100
            implicitHeight: 25
            border.width: (drawBorder) ? 1 : 0
            border.color: "cornflowerblue"
            color: control.pressed ? "transparent" : control.hovered ? "cornflowerblue" : "transparent"
        }
        label: Text {
            text: control.text
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.centerIn: parent
            font.family: "Segoe UI"
            font.pointSize: fontSize
            font.letterSpacing: 1
            color: control.pressed ? "#333" : control.hovered ? "#ccc" : "#333"
        }
    }
}
