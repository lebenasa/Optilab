import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1

Rectangle {
    id: root
    width: 1280
    height: 125

    property int captureResolution: 0
    property var initialValue
    property int activeTeam : 0
    property bool firstTimeStart: true

    function initialize() {
        if (firstTimeStart) {
            var lastProfile = Number(camera.loadSettings("CAM_PROFILE_LAST"))
            if (lastProfile === 0) teamA.checked = true
            else if (lastProfile === 1) teamB.checked = true
            else if (lastProfile === 2) teamC.checked = true
            else if (lastProfile === 3) teamD.checked = true
            var names = String(camera.loadSettings("CAM_PROFILE_NAME1"))
            console.log(names)
            teamA.editText = (names === "undefined") ? "Profile A" : names
            names = String(camera.loadSettings("CAM_PROFILE_NAME2"))
            teamB.editText = (names === "undefined") ? "Profile B" : names
            names = String(camera.loadSettings("CAM_PROFILE_NAME3"))
            teamC.editText = (names === "undefined") ? "Profile C" : names
            names = String(camera.loadSettings("CAM_PROFILE_NAME4"))
            teamD.editText = (names === "undefined") ? "Profile D" : names
            firstTimeStart = false
        }
        updateWB()
        updateColor()
        aeButton.checked = camera.isAEEnabled()
        updateAE()
//        console.log(rGain.value, gGain.value, bGain.value)
//        console.log(brightness.value, contrast.value, saturation.value)
//        console.log(aeButton.checked, target.value, gain.value, time.value)
    }

    function updateWB() {
        rGain.value = camera.getRGain()
        gGain.value = camera.getGGain()
        bGain.value = camera.getBGain()
    }

    function updateColor() {
        brightness.value = camera.getGamma()
        contrast.value = camera.getContrast()
        saturation.value = camera.getSaturation()
    }

    function updateAE() {
        target.value = camera.getAETarget()
        gain.value = camera.getAEGain()
        time.value = camera.getExposureTime()
    }

    function disableCapture() {
        capture.enabled = false
        serialCapture.enabled = false
        elapsedCapture.enabled = false
        startCapture.enabled = false
    }

    function enableCapture() {
        capture.enabled = true
        serialCapture.enabled = true
        elapsedCapture.enabled = true
        startCapture.enabled = true
    }

    function saveLastParams() {
        camera.saveSettings("CAM_PROFILE_LAST", activeTeam)
        camera.saveSettings("CAM_PROFILE_NAME1", teamA.editText)
        camera.saveSettings("CAM_PROFILE_NAME2", teamB.editText)
        camera.saveSettings("CAM_PROFILE_NAME3", teamC.editText)
        camera.saveSettings("CAM_PROFILE_NAME4", teamD.editText)
    }

    TimeEdit {
        id: ie1
        so: 1
        onVisibleChanged: {
            if (!visible) root.focus = true
        }
    }
    TimeEdit {
        id: ie2
        so: 1
        onVisibleChanged: {
            if (!visible) root.focus = true
        }
    }
    TimeEdit {
        id: de1
        so: 5
        onVisibleChanged: {
            if (!visible) root.focus = true
        }
    }

    Rectangle {
        id: ribbon
        height: 20
        color: "white"
        anchors { right: parent.right; left: parent.left; top: parent.top }
        ExclusiveGroup { id: menuGroup }
        Row {
            anchors.fill: parent
            spacing: 2
            MenuButton {
                id: captureMenu
                text: "CAPTURE"
                exclusiveGroup: menuGroup
                checked: true
            }
            MenuButton {
                id: recordMenu
                text: "RECORD"
                exclusiveGroup: menuGroup
            }
            MenuButton {
                id: cameraMenu
                text: "CAMERA"
                exclusiveGroup: menuGroup
            }
            MenuButton {
                id: stepperMenu
                text: "STEPPER"
                exclusiveGroup: menuGroup
            }
        }
    }

    Rectangle {
        id: page1
        height: 100
        color: "white"
        anchors { right: parent.right; left: parent.left; top: ribbon.bottom; topMargin: 3 }
        visible: captureMenu.checked
        Row {
            anchors.fill: parent
            anchors.margins: 3
            spacing: 2
            RibbonButton {
                id: capture
                iconSource: "img/capture.png"
                text: "Capture Image"
                onClicked: {
                    disableCapture()
                    camera.singleCapture(captureResolution)
                }
            }
            Rectangle {
                width: 1
                height: 95
                color: "#ccc"
            }
            Rectangle {
                id: scRoot1
                width: serialCapture.width + interval1.width + 20
                height: 95
                property int captureInterval : 1000
                RibbonButton {
                    id: serialCapture
                    iconSource: "img/serialcapture1.png"
                    text: "Start Capture"
                    anchors {
                        left: parent.left; top: parent.top
                    }
                    onClicked: {
                        disableCapture()
                        camera.countedCapture(parent.captureInterval, frameCount.val, captureResolution)
                    }
                }
                Rectangle {
                    id: interval1
                    width: 100
                    height: 40
                    color: "transparent"
                    anchors {
                        left: serialCapture.right; leftMargin: 10
                        top: parent.top; topMargin: 2
                    }
                    Column {
                        SegoeText {
                            width: 100
//                            height: 20
                            text: "Interval"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        DialogButton {
                            width: 100
                            height: 22
                            drawBorder: false
                            text: "%1%2:%3%4:%5%6".arg(ie1.ht).arg(ie1.ho).arg(
                                      ie1.mt).arg(ie1.mo).arg(ie1.st).arg(ie1.so)
                            onClicked: ie1.visible = true
                            onTextChanged: {
                                scRoot1.captureInterval = ie1.so + ie1.st * 10 +
                                        ie1.mo * 60 + ie1.mt * 10 * 60 +
                                        ie1.ho * 60 * 60 + ie1.ht * 10 * 60 * 60
                            }
                        }
                    }
                }
                SpinInt {
                    id: frameCount
                    anchors {
                        left: interval1.left
                        top: interval1.bottom;
                    }
                    title: "Frame Count"
                    minVal: 2
                }
                SegoeText {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                    }
                    text: "Serial Capture"
                }
            }
            Rectangle {
                width: 1
                height: 95
                color: "#ccc"
            }
            Rectangle {
                id: scRoot2
                width: elapsedCapture.width + interval2.width + 20
                height: 95
                property int captureInterval: 500
                property int captureDuration: 5000
                RibbonButton {
                    id: elapsedCapture
                    iconSource: "img/serialcapture2.png"
                    text: "Start Capture"
                    anchors {
                        left: parent.left; top: parent.top
                    }
                    onClicked: {
                        disableCapture()
                        camera.timedCapture(parent.captureInterval, parent.captureDuration, captureResolution)
                    }
                }
                Rectangle {
                    id: interval2
                    width: 100
                    height: 40
                    color: "transparent"
                    anchors {
                        left: elapsedCapture.right; leftMargin: 10
                        top: parent.top
                    }
                    Column {
                        SegoeText {
                            width: 100
//                            height: 20
                            text: "Interval"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        DialogButton {
                            width: 100
                            height: 22
                            drawBorder: false
                            text: "%1%2:%3%4:%5%6".arg(ie2.ht).arg(ie2.ho).arg(
                                      ie2.mt).arg(ie2.mo).arg(ie2.st).arg(ie2.so)
                            onClicked: ie2.visible = true
                            onTextChanged: {
                                scRoot2.captureInterval = ie2.so + ie2.st * 10 +
                                        ie2.mo * 60 + ie2.mt * 10 * 60 +
                                        ie2.ho * 60 * 60 + ie2.ht * 10 * 60 * 60
                            }
                        }
                    }
                }
                Rectangle {
                    id: duration
                    width: 100
                    height: 40
                    color: "transparent"
                    anchors {
                        left: interval2.left
                        top: interval2.bottom;
                    }
                    Column {
                        SegoeText {
                            width: 100
//                            height: 20
                            text: "Duration"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        DialogButton {
                            width: 100
                            height: 22
                            drawBorder: false
                            text: "%1%2:%3%4:%5%6".arg(de1.ht).arg(de1.ho).arg(
                                      de1.mt).arg(de1.mo).arg(de1.st).arg(de1.so)
                            onClicked: de1.visible = true
                            onTextChanged: {
                                scRoot2.captureDuration = de1.so + de1.st * 10 +
                                        de1.mo * 60 + de1.mt * 10 * 60 +
                                        de1.ho * 60 * 60 + de1.ht * 10 * 60 * 60
                            }
                        }
                    }
                }
                SegoeText {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                    }
                    text: "Elapsed Capture"
                }
            }
            Rectangle {
                width: 1
                height: 95
                color: "#ccc"
            }
        }
    }

    Rectangle {
        id: page2
        height: 100
        color: "white"
        anchors { right: parent.right; left: parent.left; top: ribbon.bottom; topMargin: 3 }
        visible: recordMenu.checked
        Row {
            anchors.fill: parent
            anchors.margins: 3
            spacing: 2
            Rectangle {
                width: startCapture.width + pauseCapture.width + stopCapture.width + 5
                height: 95
                Row {
                    anchors {
                        top: parent.top; left: parent.left
                    }
                    spacing: 2
                    RibbonButton {
                        id: startCapture
                        iconSource: enabled ? "img/record1.png" : "img/record2.png"
                        text: "Start"
                        onClicked: {
                            camera.startRecording(1)
                            disableCapture()
                            pauseCapture.enabled = true
                            stopCapture.enabled = true
                        }
                    }
                    RibbonButton {
                        id: pauseCapture
                        iconSource: enabled ? "img/pause1.png" : "img/pause2.png"
                        text: "Pause"
                        enabled: false
                        onClicked: {
                            camera.pauseRecording()
                            enabled = false
                            startCapture.text = "Resume"
                            enableCapture()
                        }
                    }
                    RibbonButton {
                        id: stopCapture
                        iconSource: enabled ? "img/stop1.png" : "img/stop2.png"
                        text: "Stop"
                        enabled: false
                        onClicked: {
                            camera.stopRecording()
                            enabled = false
                            pauseCapture.enabled = false
                            enableCapture()
                            startCapture.text = "Start"
                        }
                    }
                }
                SegoeText {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                    }
                    text: "Recording"
                }
            }
            Rectangle {
                width: 1
                height: 95
                color: "#ccc"
            }
        }
    }

    Rectangle {
        id: page3
        height: 100
        color: "white"
        anchors { right: parent.right; left: parent.left; top: ribbon.bottom; topMargin: 3 }
        visible: cameraMenu.checked
        Row {
            anchors.fill: parent
            anchors.margins: 3
            spacing: 2
            Rectangle {
                width: wbButton.width + rGain.width + 20
                height: 95
                RibbonButton {
                    id: wbButton
                    onClicked: {
                        camera.enableAWB(true)
                        updateWB()
                    }
                    iconSource: "img/wb.png"
                    text: "One Shot"
                    anchors {
                        left: parent.left; top: parent.top
                    }
                }

                RibbonSlider {
                    id: rGain
                    grooveColor: "crimson"
                    anchors {
                        left: wbButton.right; leftMargin: 10
                        verticalCenter: wbButton.verticalCenter
                        verticalCenterOffset: -20
                    }
                    enabled: !wbButton.checked
                    onValueChanged: camera.setRGain(value)
                }
                RibbonSlider {
                    id: gGain
                    grooveColor: "lime"
                    anchors {
                        horizontalCenter: rGain.horizontalCenter
                        top: rGain.bottom; topMargin: 15
                    }
                    enabled: !wbButton.checked
                    onValueChanged: camera.setGGain(value)
                }
                RibbonSlider {
                    id: bGain
                    anchors {
                        horizontalCenter: rGain.horizontalCenter
                        top: gGain.bottom; topMargin: 15
                    }
                    enabled: !wbButton.checked
                    onValueChanged: camera.setBGain(value)
                }
                SegoeText {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                    }
                    text: "White Balance"
                }
            }
            Rectangle {
                width: 1
                height: 95
                color: "#ccc"
            }
            Rectangle {
                width: brightnessIcon.width + brightness.width + 20
                height: 95

                Image {
                    id: brightnessIcon
                    source: "img/brightness.png"
                    width: 16; height: 16
                    anchors {
                        left: parent.left; leftMargin: 3
                        top: parent.top; topMargin: 10
                    }
                    fillMode: Image.PreserveAspectFit
                }
                Image {
                    id: contrastIcon
                    source: "img/contrast.png"
                    width: 16; height: 16
                    anchors {
                        horizontalCenter: brightnessIcon.horizontalCenter
                        top: brightnessIcon.bottom; topMargin: 5
                    }
                    fillMode: Image.PreserveAspectFit
                }
                Image {
                    id: saturationIcon
                    source: "img/saturation.png"
                    width: 16; height: 16
                    anchors {
                        horizontalCenter: brightnessIcon.horizontalCenter
                        top: contrastIcon.bottom; topMargin: 5
                    }
                    fillMode: Image.PreserveAspectFit
                }

                RibbonSlider {
                    id: brightness
                    grooveColor: "yellow"
                    anchors {
                        left: brightnessIcon.right; leftMargin: 10
                        verticalCenter: brightnessIcon.verticalCenter
                    }
                    onValueChanged: camera.setGamma(value)
                }
                RibbonSlider {
                    id: contrast
                    grooveColor: "magenta"
                    anchors {
                        left: contrastIcon.right; leftMargin: 10
                        verticalCenter: contrastIcon.verticalCenter
                    }
                    onValueChanged: camera.setContrast(value)
                }
                RibbonSlider {
                    id: saturation
                    grooveColor: "teal"
                    anchors {
                        left: saturationIcon.right; leftMargin: 10
                        verticalCenter: saturationIcon.verticalCenter
                    }
                    onValueChanged: camera.setSaturation(value)
                }
                SegoeText {
                    id: colorControlText
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                    }
                    text: "Color Control"
                }
//                Image {
//                    anchors {
//                        right: parent.right; rightMargin: 5
//                        verticalCenter: colorControlText.verticalCenter
//                    }
//                    smooth: true
//                    width: 12
//                    height: 12
//                    source: "img/reset.png"
//                    scale: resetMouseArea.containsMouse ? 1.1 : 1.0
//                    MouseArea {
//                        id: resetMouseArea
//                        anchors.fill: parent
//                        hoverEnabled: true
//                        onClicked: {
//                            brightness.value = initialValue[0]
//                            contrast.value = initialValue[1]
//                            saturation.value = initialValue[2]
//                        }
//                    }
//                }
            }
            Rectangle {
                width: 1
                height: 95
                color: "#ccc"
            }
            Rectangle {
                width: aeButton.width + targetText.width + target.width + 23
                height: 95
                RibbonButton {
                    id: aeButton
                    checkable: true
                    onCheckedChanged: {
                        camera.enableAE(checked)
                        updateAE()
                    }
                    iconSource: "img/ae.png"
                    text: checked ? "Automatic" : "Manual"
                    anchors {
                        left: parent.left; top: parent.top
                    }
                }

                SegoeText {
                    id: targetText
                    text: "Target"
                    anchors {
                        left: aeButton.right; leftMargin: 5
                        verticalCenter: aeButton.verticalCenter
                        verticalCenterOffset: -22
                    }
                }

                SegoeText {
                    id: gainText
                    text: "Gain"
                    anchors {
                        horizontalCenter: targetText.horizontalCenter
                        top: targetText.bottom; topMargin: 5
                    }
                }

                SegoeText {
                    id: timeText
                    text: "Time"
                    anchors {
                        horizontalCenter: targetText.horizontalCenter
                        top: gainText.bottom; topMargin: 5
                    }
                }

                RibbonSlider {
                    id: target
                    grooveColor: "blue"
                    anchors {
                        left: targetText.right; leftMargin: 10
                        verticalCenter: aeButton.verticalCenter
                        verticalCenterOffset: -20
                    }
                    enabled: aeButton.checked
                    onValueChanged: camera.setAETarget(value)
                }
                RibbonSlider {
                    id: gain
                    grooveColor: "red"
                    anchors {
                        horizontalCenter: target.horizontalCenter
                        top: target.bottom; topMargin: 15
                    }
                    enabled: !aeButton.checked
                    onValueChanged: camera.setAEGain(value)
                }
                RibbonSlider {
                    id: time
                    grooveColor: "green"
                    anchors {
                        horizontalCenter: target.horizontalCenter
                        top: gain.bottom; topMargin: 15
                    }
                    enabled: !aeButton.checked
                    onValueChanged: camera.setExposureTime(value)
                }
                SegoeText {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                    }
                    text: "Exposure"
                }
            }
            Rectangle {
                width: 1
                height: 95
                color: "#ccc"
            }
            Rectangle {
                width: 200
                height: 95
                ExclusiveGroup { id: parameterTeam }
                Column {
                    id: col1
                    width: teamA.width
                    anchors {
                        top: parent.top; topMargin: 5
                        left: parent.left; leftMargin: 5
                    }
                    spacing: 2
                    RadioEdit {
                        id: teamA
                        exclusiveGroup: parameterTeam
                        checked: true
                        editText: "Profile A"
                        onCheckedChanged: {
                            if (checked) {
                                activeTeam = 0
                                camera.loadProfile(activeTeam)
                                initialize()
                            }
                        }
                    }
                    RadioEdit {
                        id: teamB
                        exclusiveGroup: parameterTeam
                        editText: "Profile B"
                        onCheckedChanged: {
                            if (checked) {
                                activeTeam = 1
                                camera.loadProfile(activeTeam)
                                initialize()
                            }
                        }
                    }
                }
                Column {
                    anchors {
                        top: parent.top; topMargin: 5
                        left: col1.right; leftMargin: 0
                    }
                    spacing: 2
                    RadioEdit {
                        id: teamC
                        exclusiveGroup: parameterTeam
                        editText: "Profile C"
                        onCheckedChanged: {
                            if (checked) {
                                activeTeam = 2
                                camera.loadProfile(activeTeam)
                                initialize()
                            }
                        }
                    }
                    RadioEdit {
                        id: teamD
                        exclusiveGroup: parameterTeam
                        editText: "Profile D"
                        onCheckedChanged: {
                            if (checked) {
                                activeTeam = 3
                                camera.loadProfile(activeTeam)
                                initialize()
                            }
                        }
                    }
                }
                Row {
                    anchors {
                        top: col1.bottom; topMargin: 15
                        left: col1.left
                    }
                    spacing: 20
                    DialogButton {
                        width: 80
                        height: teamA.height
                        fontSize: 8
                        text: "Save"
                        onClicked: {
                            camera.saveProfile(activeTeam)
                        }
                    }
//                    DialogButton {
//                        width: 80
//                        height: teamA.height
//                        fontSize: 8
//                        text: "Load"
//                        onClicked: {
//                            camera.loadProfile(activeTeam)
//                            initialize()
//                        }
//                    }
                    DialogButton {
                        width: 80
                        height: teamA.height
                        fontSize: 8
                        text: "Default"
                        onClicked: {
                            camera.loadProfile(-1)
                            initialize()
                        }
                    }
                }

                SegoeText {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                    }
                    text: "Profile"
                }
            }
            Rectangle {
                width: 1
                height: 95
                color: "#ccc"
            }
            Rectangle {
                width: 150
                height: 95
                Switch {
                    id: streamMode
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: -10
                    checked: true
                    onCheckedChanged: checked ? camera.setStreamResolution(0) : camera.setStreamResolution(2)
                }
                SegoeText {
                    text: "Speed"
                    anchors {
                        verticalCenter: streamMode.verticalCenter
                        right: streamMode.left; rightMargin: 10
                    }
                }
                SegoeText {
                    text: "Quality"
                    anchors {
                        verticalCenter: streamMode.verticalCenter
                        left: streamMode.right; leftMargin: 10
                    }
                }
                SegoeText {
                    text: "Stream Mode"
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                    }
                }
            }
            Rectangle {
                width: 1
                height: 95
                color: "#ccc"
            }
        }
    }

    Rectangle {
        id: page4
        height: 100
        color: "white"
        anchors { right: parent.right; left: parent.left; top: ribbon.bottom; topMargin: 3 }
        visible: stepperMenu.checked
        Row {
            anchors.fill: parent
            anchors.margins: 3
            spacing: 2
            RibbonButton {
                text: "On-Screen Control"
                iconSource: "img/onscreen-control.png"
                onClicked: camera.showOnScreenControl()
            }
            RibbonButton {
                text: "On-Screen Scale"
                iconSource: "img/scale.png"
            }
            RibbonButton {
                text: "Calibration"
                iconSource: "img/calibration.png"
            }
            Rectangle {
                width: 1
                height: 95
                color: "#ccc"
            }
            Rectangle {
                width: 185
                height: 95
                color: "transparent"
                Row {
                    anchors.fill: parent
                    spacing: 2
                    RibbonButton {
                        text: "Row-Column"
                        iconSource: "img/row-col.png"
                    }
                    RibbonButton {
                        text: "Width-Height"
                        iconSource: "img/width-height.svg"
                    }
                    RibbonButton {
                        text: "Three Points"
                        iconSource: "img/three-points.svg"
                    }
                }
                SegoeText {
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                    }
                    text: "Serial Capture"
                }
            }
            Rectangle {
                width: 1
                height: 95
                color: "#ccc"
            }
        }
    }
}
