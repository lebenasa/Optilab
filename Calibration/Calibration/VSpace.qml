import QtQuick 2.2
import QtQuick.Controls 1.1

Item {
    property int length: h2.y - h1.y
    property alias unit: cbUnit.currentIndex
    property alias realLength: tLength.text
//    anchors.fill: parent

    function getRealLength() {
        var mod = 0
        switch(unit) {
        case 0:
            mod = 1
            break
        case 1:
            mod = 1000
            break
        case 2:
            mod = 10000
            break
        default:
            mod = 1
            break
        }
        return mod * realLength
    }

    LineWidget {
        id: h1
        y: parent.height/2 - 60
        min: 50
        max: h2.y - 20
    }
    LineWidget {
        id: h2
        y: parent.height/2 + 60
        min: h1.y + 20
        max: parent.height - 20
    }
    Rectangle {
        id: hOrnament
        color: "#0500ff00"
        width: parent.width
        anchors.top: h1.bottom
        anchors.bottom: h2.top
        ComboBox {
            id: cbUnit
            anchors {
                right: parent.right; rightMargin: 10
                verticalCenter: parent.verticalCenter
            }
            width: 55
            model: [ "μm", "mm", "cm" ]

        }
        TextField {
            id: tLength
            anchors {
                right: cbUnit.left; rightMargin: 5
                verticalCenter: parent.verticalCenter
            }
            horizontalAlignment: Text.AlignRight
            width: 60
            font.pointSize: 10
            text: "100"
            validator: IntValidator { }
            onAccepted: focus = false
        }
    }
}
