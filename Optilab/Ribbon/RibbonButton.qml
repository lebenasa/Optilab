import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Button {
    id: root
    style: ButtonStyle {
        background: Rectangle {
            implicitWidth: 60
            implicitHeight: 80
            color: control.pressed ? "cornflowerblue" : "transparent"
            border.width: control.checked ? 1 : 0
            border.color: "cornflowerblue"
        }
        label: Item {
            anchors.fill: parent
            Image {
                id: icon
                anchors {
                    left: parent.left; right: parent.right
                    top: parent.top
                }
                sourceSize.width: 60
                sourceSize.height: 50
                source: control.iconSource
                smooth: true
                fillMode: Image.PreserveAspectFit
            }
            Text {
                width: 60
                text: control.text
                anchors {
                    horizontalCenter: icon.horizontalCenter
                    bottom: parent.bottom
                }
                font.family: "Segoe UI"
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 9
                font.weight: control.hovered ? Font.DemiBold : Font.Light
                wrapMode: Text.WordWrap
                color: "#444"
            }
        }
    }
}
