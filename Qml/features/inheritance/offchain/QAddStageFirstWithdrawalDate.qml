import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import Features.Inheritance.OffChain.ViewModels 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Popups"
import "../../../core/components"

QOnScreenContentTypeA {
    id: root
    
    FirstWithdrawalDateViewModel {
        id: vm
    }
    
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: qsTr("Stage %1").arg(vm.stage.index)
    onCloseClicked: vm.close()
    content: Item {
        width: 569
        height: 380
        
        Column {
            anchors.fill: parent
            spacing: 16
            Item {
                width: 728
                height: 40
                Item {
                    width: 88
                    height: 4
                    Row {
                        width: 88
                        height: 4
                        spacing: 8
                        Rectangle {
                            width: 40
                            height: parent.height
                            color: "#EAEAEA"
                            radius: 12
                        }
                        Rectangle {
                            width: 40
                            height: parent.height
                            color: "#031F2B"
                            radius: 12
                        }
                    }
                }
                
                QTextLink {
                    anchors.right: parent.right
                    width: 36
                    height: 19
                    text: STR.STR_QML_246
                    onTextClicked: {
                        vm.deleteClicked()
                    }
                }
                QLato {
                    anchors {
                        left: parent.left
                        bottom: parent.bottom
                    }
                    text: STR.STR_QML_2157 + ":"
                    font.pixelSize: 14
                    font.weight: Font.Bold
                    color: "#031F2B"
                }
            }
           
            // Note
            Rectangle {
                width: 728
                height: 112
                color: "#F5F5F5"
                radius: 12
                
                Column {
                    anchors.fill: parent
                    anchors {
                        topMargin: 16
                        leftMargin: 12
                        bottomMargin: 16
                        rightMargin: 12
                    }
                    spacing: 20
                    QLato {
                        width: parent.width
                        height: paintedHeight
                        text: STR.STR_QML_2205
                        font.pixelSize: 16
                        wrapMode: Text.WordWrap
                        lineHeight: 20
                        lineHeightMode: Text.FixedHeight
                    }
                    
                    QLato {
                        id: noteText
                        text: STR.STR_QML_2206
                        font.pixelSize: 16
                    }
                }
            }
            
            // Form fields
            Column {
                width: 728
                spacing: 12
                
                // Amount per Release and Repeat every (on same row)
                Row {
                    width: 728
                    spacing: 24                   
                    Item {
                        width: (parent.width - 12) / 2
                        height: 68
                        QTextInputBoxTypeB {
                            id: _input_date
                            label: STR.STR_QML_1463
                            labelComponent.textFormat: Text.RichText
                            labelComponent.font.pixelSize: 12
                            boxWidth: parent.width
                            boxHeight: 48
                            textInputted: _calendarInput.dateString
                            input.placeholderText: vm.valueDate
                            onTextInputtedChanged: {
                                if(!_input_date.isValid){
                                    _input_date.isValid = true
                                    _input_date.errorText = ""
                                }
                                _input_date.showError = false;
                                vm.valueDate = _input_date.textInputted
                            }
                        }
                        QIcon {
                            iconSize: 24
                            source: "qrc:/Images/Images/calendar-light.svg"
                            anchors {
                                bottom: _input_date.bottom
                                bottomMargin: 14
                                right: _input_date.right
                                rightMargin: 12
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            propagateComposedEvents: true
                            onClicked: {
                                _calendarInput.open()
                            }
                        }
                        QPopupCalendar {
                            id: _calendarInput
                            minimumDate: new Date()
                            dateString: vm.valueDate
                        }
                    }
                
                    Item {
                        width: (parent.width - 12) / 2
                        height: _input_time.height
                        QTextInputBoxTypeB {
                            id: _input_time
                            label: STR.STR_QML_1989
                            labelComponent.textFormat: Text.RichText
                            labelComponent.font.pixelSize: 12
                            boxWidth: parent.width
                            boxHeight: 48
                            textInputted: _analogClockInput.timeString
                            input.placeholderText: vm.valueTime
                            onTextInputtedChanged: {
                                if(!_input_time.isValid){
                                    _input_time.isValid = true
                                    _input_time.errorText = ""
                                }
                                _input_time.showError = false;
                                vm.valueTime = _input_time.textInputted
                            }
                        }
                        QIcon {
                            iconSize: 24
                            source: "qrc:/Images/Images/clock-dark.svg"
                            anchors {
                                bottom: _input_time.bottom
                                bottomMargin: 14
                                right: _input_time.right
                                rightMargin: 12
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            propagateComposedEvents: true
                            onClicked: {
                                _analogClockInput.open()
                            }
                        }
                    }
                }
            }
        }
    }
    QPopupAnalogClock {
        id: _analogClockInput
        timeString: vm.valueTime
    }
    onPrevClicked: vm.back()
    
    bottomRight: Row {
        spacing: 12
        
        QTextButton {
            width: label.implicitWidth + 32
            height: 48
            label.text: STR.STR_QML_097  // "Continue"
            label.font.pixelSize: 14
            type: eTypeE
            onButtonClicked: {
                vm.onContinueClicked()
            }
        }
    }
}
