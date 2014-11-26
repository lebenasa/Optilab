import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.0

Rectangle {
    id: save
    property alias source: img.source
    signal click
    width: 32; height: 32
    radius: 128
    border.width: 1
    border.color: "green"
    color: ma.pressed ? "#0500ff00" : ma.containsMouse ? "#3000ff00" : "#0500ff00"
    Image {
        id: img
        anchors.centerIn: parent
        smooth: true
        sourceSize.width: 16; sourceSize.height: 16
    }
    MouseArea {
        id: ma
        anchors.fill: parent
        hoverEnabled: true
        onClicked: click()
    }
}
