import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Button {
    checkable: true
    style: ButtonStyle {
        background: Rectangle {
            implicitWidth: 80
            color: "white"
            border.width: control.checked ? 1 : 0
            border.color: "#ccc"
        }
        label: SegoeText {
            text: control.text
            color: control.checked ? "cornflowerblue" : "#555"
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 10
        }
    }
}
