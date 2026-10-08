import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: analogClock
    width: 350
    height: 500
    property int hours: 0
    property int minutes: 0
    property bool interactive: true
    property bool settingHours: true  // true = setting hours, false = setting minutes
    
    signal timeChanged(int newHours, int newMinutes)
    signal cancelClicked()
    signal okClicked(int hours, int minutes)
    
    Rectangle {
        anchors.fill: parent
        color: "#f0f0f0"
        radius: 10
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 20
            
            // Title
            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "Select Time"
                font.pixelSize: 18
                color: "#333333"
                font.bold: true
            }
            
            // Digital time display
            Rectangle {
                Layout.alignment: Qt.AlignHCenter
                width: 120
                height: 50
                color: "#1a1a1a"
                radius: 8
                
                RowLayout {
                    anchors.centerIn: parent
                    spacing: 5
                    
                    Rectangle {
                        width: 40
                        height: 35
                        color: analogClock.settingHours ? "#4CAF50" : "#2a2a2a"
                        radius: 4
                        border.color: analogClock.settingHours ? "#66BB6A" : "transparent"
                        border.width: analogClock.settingHours ? 2 : 0
                        
                        Text {
                            anchors.centerIn: parent
                            text: String(analogClock.hours).padStart(2, '0')
                            color: "white"
                            font.pixelSize: 20
                            font.bold: true
                        }
                        
                        MouseArea {
                            anchors.fill: parent
                            onClicked: analogClock.settingHours = true
                        }
                    }
                    
                    Text {
                        text: ":"
                        color: "white"
                        font.pixelSize: 20
                        font.bold: true
                    }
                    
                    Rectangle {
                        width: 40
                        height: 35
                        color: !analogClock.settingHours ? "#4CAF50" : "#2a2a2a"
                        radius: 4
                        border.color: !analogClock.settingHours ? "#66BB6A" : "transparent"
                        border.width: !analogClock.settingHours ? 2 : 0
                        
                        Text {
                            anchors.centerIn: parent
                            text: String(analogClock.minutes).padStart(2, '0')
                            color: "white"
                            font.pixelSize: 20
                            font.bold: true
                        }
                        
                        MouseArea {
                            anchors.fill: parent
                            onClicked: analogClock.settingHours = false
                        }
                    }
                }
            }
            

            
            // Clock face background
            Rectangle {
                id: clockFace
                Layout.alignment: Qt.AlignHCenter
                width: 280
                height: 280
                radius: width / 2
                color: "white"
                border.color: "#e0e0e0"
                border.width: 2
                
                // Hour markers (small dots) - removed to avoid overlap with numbers
                
                // Hour numbers (12-hour inner ring: 01-12)
                Repeater {
                    model: 12
                    Text {
                        visible: analogClock.settingHours
                        text: String(index === 0 ? 12 : index).padStart(2, '0')
                        font.pixelSize: 14
                        color: "#031F2B"
                        x: parent.width/2 + (parent.width/2 - 35) * Math.sin(index * Math.PI / 6) - width/2
                        y: parent.height/2 - (parent.height/2 - 35) * Math.cos(index * Math.PI / 6) - height/2
                    }
                }
                
                // 24-hour numbers (outer ring: 00 at top, then 13-23)
                Repeater {
                    model: 12
                    Text {
                        visible: analogClock.settingHours
                        text: String(index === 0 ? 0 : index + 12).padStart(2, '0')
                        font.pixelSize: 12
                        color: "#031F2B"
                        x: parent.width/2 + (parent.width/2 - 18) * Math.sin(index * Math.PI / 6) - width/2
                        y: parent.height/2 - (parent.height/2 - 18) * Math.cos(index * Math.PI / 6) - height/2
                    }
                }
                
                // Minute markers (for minute setting mode)
                Repeater {
                    model: 12
                    Text {
                        visible: !analogClock.settingHours
                        text: String(index * 5).padStart(2, '0')
                        font.pixelSize: 12
                        color: "#031F2B"
                        x: parent.width/2 + (parent.width/2 - 25) * Math.sin(index * Math.PI / 6) - width/2
                        y: parent.height/2 - (parent.height/2 - 25) * Math.cos(index * Math.PI / 6) - height/2
                    }
                }
                
                // Selected position indicator
                Rectangle {
                    id: selectedIndicator
                    width: 8
                    height: 8
                    radius: 4
                    color: "#4CAF50"
                    visible: true
                    z: 10
                    
                    property real indicatorAngle: analogClock.settingHours ? 
                                                  (analogClock.hours % 12) * 30 : 
                                                  analogClock.minutes * 6
                    property real indicatorRadius: analogClock.settingHours ? 
                                                   (analogClock.hours >= 13 || analogClock.hours === 0 ? 
                                                    clockFace.width/2 - 18 : clockFace.width/2 - 35) :
                                                   clockFace.width/2 - 25
                    
                    x: clockFace.width/2 + indicatorRadius * Math.sin(indicatorAngle * Math.PI / 180) - width/2
                    y: clockFace.height/2 - indicatorRadius * Math.cos(indicatorAngle * Math.PI / 180) - height/2
                    
                    Behavior on x { NumberAnimation { duration: 200 } }
                    Behavior on y { NumberAnimation { duration: 200 } }
                }
                
                // Center dot
                Rectangle {
                    id: centerDot
                    width: 12
                    height: 12
                    radius: 6
                    color: "#333333"
                    anchors.centerIn: parent
                    z: 100
                }
                
                // Hour hand
                Rectangle {
                    id: hourHand
                    width: 4
                    height: parent.height * 0.3
                    color: analogClock.settingHours ? "#4CAF50" : "#999999"
                    radius: 2
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.verticalCenter
                    anchors.bottomMargin: -2
                    transformOrigin: Item.Bottom
                    rotation: (analogClock.hours % 12) * 30
                    z: analogClock.settingHours ? 5 : 3
                    visible: analogClock.settingHours
                    
                    Behavior on rotation {
                        RotationAnimation {
                            duration: 300
                            direction: RotationAnimation.Shortest
                        }
                    }
                }
                
                // Minute hand
                Rectangle {
                    id: minuteHand
                    width: 3
                    height: parent.height * 0.4
                    color: !analogClock.settingHours ? "#4CAF50" : "#999999"
                    radius: 1.5
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.verticalCenter
                    anchors.bottomMargin: -1.5
                    transformOrigin: Item.Bottom
                    rotation: analogClock.minutes * 6
                    z: !analogClock.settingHours ? 5 : 2
                    visible: !analogClock.settingHours
                    
                    Behavior on rotation {
                        RotationAnimation {
                            duration: 300
                            direction: RotationAnimation.Shortest
                        }
                    }
                }
                
                // Interactive overlay for setting time by clicking on clock face
                MouseArea {
                    anchors.fill: parent
                    enabled: analogClock.interactive
                    
                    onClicked: {
                        var center = Qt.point(clockFace.width/2, clockFace.height/2)
                        var clickPos = Qt.point(mouseX, mouseY)
                        
                        if (analogClock.settingHours) {
                            // Setting hours - find closest hour number
                            var closestHour = -1
                            var minDistance = 999999
                            
                            // Check all 24 hour positions
                            for (var h = 0; h < 24; h++) {
                                var hourPos
                                var hourRadius
                                var hourAngle
                                
                                if (h >= 1 && h <= 12) {
                                    // Inner ring: hours 1-12
                                    hourRadius = clockFace.width/2 - 35
                                    hourAngle = (h === 12 ? 0 : h) * 30  // 12 is at 0 degrees
                                } else {
                                    // Outer ring: hours 0, 13-23
                                    hourRadius = clockFace.width/2 - 18
                                    if (h === 0) {
                                        hourAngle = 0  // 00 at top
                                    } else {
                                        hourAngle = (h - 12) * 30  // 13 at 1 o'clock position, etc.
                                    }
                                }
                                
                                // Calculate position of this hour number
                                var hourX = clockFace.width/2 + hourRadius * Math.sin(hourAngle * Math.PI / 180)
                                var hourY = clockFace.height/2 - hourRadius * Math.cos(hourAngle * Math.PI / 180)
                                
                                // Calculate distance from click to this hour position
                                var distance = Math.sqrt(Math.pow(clickPos.x - hourX, 2) + Math.pow(clickPos.y - hourY, 2))
                                
                                // Keep track of closest hour
                                if (distance < minDistance) {
                                    minDistance = distance
                                    closestHour = h
                                }
                            }
                            
                            // Only set if click is reasonably close to an hour number (within 25 pixels)
                            if (minDistance < 25 && closestHour !== -1) {
                                console.log("Clicked hour:", closestHour, "Distance:", minDistance)
                                analogClock.timeChanged(closestHour, analogClock.minutes)
                            }
                        } else {
                            // Setting minutes (0-59)
                            var angle = Math.atan2(clickPos.x - center.x, center.y - clickPos.y) * 180 / Math.PI
                            if (angle < 0) angle += 360
                            var newMinutes = Math.round(angle / 6) % 60
                            analogClock.timeChanged(analogClock.hours, newMinutes)
                        }
                    }
                }
                
                // Visual indicators for selection areas
                Repeater {
                    model: analogClock.settingHours ? 24 : 60
                    Rectangle {
                        visible: analogClock.settingHours ? 
                                (index < 12 || index >= 12) : true  // Show all for minutes, all for hours
                        width: analogClock.settingHours ? 
                               (index < 12 ? 25 : 20) :  // Inner ring bigger than outer for hours
                               15  // Minutes
                        height: width
                        radius: width / 2
                        color: "transparent"
                        border.color: analogClock.settingHours ? 
                                     (index < 12 ? "#E8F5E8" : "#F0F8F0") :  // Light green for hours
                                     "#E8F5E8"  // Light green for minutes
                        border.width: 1
                        
                        // Position calculation
                        property real ringRadius: analogClock.settingHours ? 
                                                 (index < 12 ? clockFace.width/2 - 35 : clockFace.width/2 - 18) :
                                                 clockFace.width/2 - 25
                        property real itemAngle: analogClock.settingHours ? 
                                                (index < 12 ? index * 30 : (index - 12) * 30) :
                                                index * 6
                        
                        x: clockFace.width/2 + ringRadius * Math.sin(itemAngle * Math.PI / 180) - width/2
                        y: clockFace.height/2 - ringRadius * Math.cos(itemAngle * Math.PI / 180) - height/2
                        z: -1  // Behind other elements
                    }
                }
            }
            
            // Cancel and OK buttons
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20
                spacing: 40
                
                Button {
                    text: "Cancel"
                    font.pixelSize: 16
                    Layout.preferredWidth: 80
                    Layout.preferredHeight: 40
                    background: Rectangle {
                        color: "transparent"
                        border.color: "#cccccc"
                        border.width: 1
                        radius: 6
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "#666666"
                        font: parent.font
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: analogClock.cancelClicked()
                }
                
                Button {
                    text: "OK"
                    font.pixelSize: 16
                    Layout.preferredWidth: 80
                    Layout.preferredHeight: 40
                    background: Rectangle {
                        color: "#2196F3"
                        radius: 6
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        font: parent.font
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: analogClock.okClicked(analogClock.hours, analogClock.minutes)
                }
            }
        }
    }
    
    // Helper function to set time
    function setTime(h, m) {
        hours = h
        minutes = m
    }
    
    // Helper function to get current time
    function getCurrentTime() {
        return {
            hours: hours,
            minutes: minutes
        }
    }
}