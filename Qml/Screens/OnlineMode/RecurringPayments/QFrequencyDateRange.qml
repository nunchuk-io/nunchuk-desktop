import QtQuick 2.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR
Item {
    id: _item
    property bool noEndate: true
    property string start_date: _calendarStart.dateString
    property string end_date: _calendarEnd.dateString
    signal needChangeDate()
    height: _col.childrenRect.height
    Column {
        id: _col
        spacing: 16
        Item {
            width: parent.width
            height: 24
            QLato {
                font.weight: Font.Bold
                font.pixelSize: 16
                text: STR.STR_QML_1103
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Item {
            width: _item.width
            height: 72
            QTextInputBoxTypeB {
                id: _start_date
                label: STR.STR_QML_1104
                boxWidth: _item.width
                boxHeight: 48
                textInputted: _calendarStart.dateString
                input.placeholderText: "MM/DD/YYYY"
                onTextInputtedChanged: {
                    if(!_start_date.isValid){
                        _start_date.isValid = true
                        _start_date.errorText = ""
                    }
                    _start_date.showError = false;
                }
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                propagateComposedEvents: true
                onClicked: {
                    _calendarStart.open()
                }
            }
        }
        Item {
            width: _item.width
            height: 72
            QTextInputBoxTypeB {
                id: _end_date
                label: STR.STR_QML_1105
                boxWidth: _item.width
                boxHeight: 48
                textInputted: _calendarEnd.dateString
                input.placeholderText: "MM/DD/YYYY"
                enabled: !_no_end_date.checked
                onTextInputtedChanged: {
                    if(!_end_date.isValid){
                        _end_date.isValid = true
                        _end_date.errorText = ""
                    }
                    _end_date.showError = false;
                }
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                propagateComposedEvents: true
                onClicked: {
                    if (_end_date.enabled)
                        _calendarEnd.open()
                }
            }
        }

        QCheckBoxButton {
            id: _no_end_date
            width: _item.width
            height: 24
            label: STR.STR_QML_1106
            checked: noEndate
            onButtonClicked: {
                noEndate = checked
                if (noEndate) {
                    _calendarEnd.dateString = ""
                }
            }
        }
    }
    onNeedChangeDate: {
        _calendarStart.open()
    }

    QPopupCalendar {
        id: _calendarStart
        minimumDate: new Date()
    }
    QPopupCalendar {
        id: _calendarEnd
        minimumDate: new Date()
    }
}
