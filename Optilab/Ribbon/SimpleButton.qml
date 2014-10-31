import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Button {
    style: ButtonStyle {
        background: Rectangle {
            implicitWidth: 50
            implicitHeight: 10
            color: control.checked ? "cornflowerblue" : "transparent"
            border.width: 1
            border.color: control.checked ? "transparent" : "#333"
        }
        label: SegoeText {
            horizontalAlignment: Text.AlignHCenter
            text: control.text
            font.pointSize: 8
            color: control.checked ? "white" : "#333"
            font.weight: Font.DemiBold
        }
    }
}
