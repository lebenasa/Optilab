import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Button {
    width: 40
    height: 40
    style: ButtonStyle {
        background: Rectangle {
            color: "maroon"
            radius: 100
            Text {
                color: "white"
                anchors.centerIn: parent
                text: "10x"
                font.pointSize: 10
            }
        }
    }
}
