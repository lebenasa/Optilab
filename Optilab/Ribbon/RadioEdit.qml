import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

RadioButton {
    id: radio
    width: 100
    property alias editText: input.text
    TextInput {
        id: input
        width: 85
        anchors {
            left: parent.left; leftMargin: 15
            verticalCenter:parent.verticalCenter
        }
        text: "A"
        font.family: "Segoe UI"
        font.pointSize: 9
        color: "#444"
        horizontalAlignment: Text.AlignLeft
        maximumLength: 15
        onAccepted: radio.focus = false
    }
}
