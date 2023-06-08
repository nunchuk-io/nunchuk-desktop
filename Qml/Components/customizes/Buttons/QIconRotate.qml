import QtQuick 2.0

Item {
    id: _rotate
    property bool optionVisible: false
    state: !enabled ? "disable" : optionVisible ? "onOption" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))
    width: 16
    height: 16
    function pressed()
    {
        _rotate.state = "clicked"
    }
    function containsMouseChanged()
    {
        _rotate.state = !enabled ? "disable" : optionVisible ? "onOption" : (btnMouse.pressed ? "clicked" : (btnMouse.containsMouse ? "hover" : "default"))
    }
    function released()
    {
        _rotate.state = "onOption"
    }

    onOptionVisibleChanged: {
        if(optionVisible) { _rotate.state = "onOption"}
        else{ _rotate.state = "default"}
    }
    Behavior on rotation {
        NumberAnimation {duration: 200}
    }
    Row {
        anchors.centerIn: parent
        Repeater {
            model: 3
            Rectangle { width: 4; height: 4; radius: 4; color: text.color}
        }
    }
    states: [
        State {
            name: "default"
            PropertyChanges {
                target: _rotate
                rotation: 0
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: _rotate
                rotation: 0
            }
        },
        State {
            name: "disable"
            PropertyChanges {
                target: _rotate
                rotation: 0
            }
        },
        State {
            name: "clicked"
            PropertyChanges {
                target: _rotate
                rotation: 0
            }
        },
        State {
            name: "onOption"
            PropertyChanges {
                target: _rotate
                rotation: 90
            }
        }
    ]

}
