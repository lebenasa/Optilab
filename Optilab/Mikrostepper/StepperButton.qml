import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    width: 34
    height: 50
    property bool pushed: false
    Image {
        anchors.fill: parent
        source: pushed ? "img/Up-pushed.png" : "img/Up-normal.png"
        smooth: true
        fillMode: Image.PreserveAspectFit
    }
}
