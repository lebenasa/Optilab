import QtQuick 2.2
import QtQuick.Controls 1.1

Item {
    property int length: h2.x - h1.x
    property alias unit: cbUnit.currentIndex
    property alias realLength: tLength.text
    anchors.fill: parent

    LineWidget {
        id: h1
        orientation: 1
        x: 580
        min: 20
        max: h2.x - 20
    }
    LineWidget {
        id: h2
        orientation: 1
        x: 700
        min: h1.x + 20
        max: parent.width - 20
    }
    Rectangle {
        id: hOrnament
        color: "#0500ff00"
        height: parent.height
        anchors.left: h1.right
        anchors.right: h2.left
        Item {
            width: cbUnit.width + tLength.width + 10
            height: cbUnit.height
            anchors {
                bottom: parent.bottom; bottomMargin: 10
                horizontalCenter: parent.horizontalCenter
            }
            Row {
                spacing: 5
                TextField {
                    id: tLength
                    horizontalAlignment: Text.AlignRight
                    width: 60
                    font.pointSize: 10
                    text: "100"
                    validator: IntValidator { }
                    onAccepted: focus = false
                }
                ComboBox {
                    id: cbUnit
                    width: 55
                    model: [ "μm", "mm", "cm" ]
                }
            }
        }
    }
}
