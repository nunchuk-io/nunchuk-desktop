import QtQuick 2.12
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Popups"
import Features.Inheritance.OffChain.ViewModels 1.0

QOnScreenContentTypeB {
    id: root
    
    FallbackSettingsViewModel {
        id: vm
    }
    
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2156
    extraHeader: Item {}
    onCloseClicked: vm.close()
    
    content: Item {
        width: 728
        height: 356
        
        Column {
            width: parent.width
            spacing: 24
            
            // Description text
            QLato {
                width: parent.width
                text: STR.STR_QML_2190
                font.pixelSize: 16
                wrapMode: Text.WordWrap
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
            }
            
            // Fallback options
            Column {
                width: parent.width
                height: 356
                spacing: 16

                Row {
                    width: parent.width
                    height: vm.selectedFallback === "DATE_BASED" ? 200: 120
                    spacing: 16

                    QRadioButtonTypeF {
                        width: 356
                        height: vm.selectedFallback === "DATE_BASED" ? 200: 120
                        labelMaxWidth: 284
                        labelBottomLineHeight: 20
                        selected: vm.selectedFallback === "NONE"
                        labelTop: STR.STR_QML_2191
                        labelBottom: STR.STR_QML_2192
                        onButtonClicked: { vm.selectedFallback = "NONE" }
                    }

                    QRadioButtonTypeF {
                        id: _DateBased
                        width: 356
                        height: vm.selectedFallback === "DATE_BASED" ? 200: 120
                        labelMaxWidth: 284
                        labelBottomLineHeight: 20
                        selected: vm.selectedFallback === "DATE_BASED"
                        labelTop: STR.STR_QML_2193
                        labelBottom: vm.selectedFallback === "DATE_BASED" ? STR.STR_QML_2226 : STR.STR_QML_2194
                        onButtonClicked: { vm.selectedFallback = "DATE_BASED" }
                        Item {
                            width: 271
                            height: 68
                            visible: vm.selectedFallback === "DATE_BASED"
                            anchors {
                                bottom: _DateBased.bottom
                                bottomMargin: 18
                                left: _DateBased.left
                                leftMargin: 54
                            }
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
                    }
                }

                QRadioButtonTypeF {
                    id: _Inactivity
                    width: 356
                    height: vm.selectedFallback === "INACTIVITY" ? 220 : 120
                    labelMaxWidth: 284
                    labelBottomLineHeight: 20
                    selected: vm.selectedFallback === "INACTIVITY"
                    labelTop: STR.STR_QML_2195
                    labelBottom: vm.selectedFallback === "INACTIVITY" ? STR.STR_QML_2196 : STR.STR_QML_2194
                    onButtonClicked: { vm.selectedFallback = "INACTIVITY" }
                    isRecommend: true

                    Item {
                        width: 271
                        height: 68
                        visible: vm.selectedFallback === "INACTIVITY"
                        anchors {
                            bottom: _Inactivity.bottom
                            bottomMargin: 18
                            left: _Inactivity.left
                            leftMargin: 54
                        }
                        Column {
                            width: parent.width
                            spacing: 4
                            Row {
                                height: 68
                                spacing: 8
                                Item {
                                    width: 144
                                    height: 68
                                    QTextInputBoxTypeB {
                                        id: repeatEveryInput
                                        label: STR.STR_QML_2197
                                        boxWidth: parent.width
                                        boxHeight: 48
                                        textInputted: vm.repeatEvery
                                        validator: RegExpValidator {
                                            regExp: /^$|^[0-9]+$/
                                        }
                                        onTextInputtedChanged: {
                                            if (textInputted !== "") {
                                                vm.repeatEvery = parseInt(textInputted)
                                            }
                                        }
                                    }
                                    QComboBox {
                                        id: _interal
                                        anchors {
                                            right: parent.right
                                            rightMargin: 0
                                            bottom: parent.bottom
                                            bottomMargin: -8
                                        }
                                        width: 100
                                        height: 48
                                        background: Item { }
                                        current_id: {
                                            for (var i = 0; i < _interal.model.length; i++) {
                                                if (_interal.model[i].id === vm.inactivityUnit) {
                                                    return _interal.model[i]
                                                }
                                            }
                                            return _interal.model[0]
                                        }
                                        defaultValue: {
                                            return { id: qsTr("%1").arg(_interal.model[0].id), displayName: qsTr("%1").arg(_interal.model[0].displayName) }
                                        }
                                        displayText: getDisplay().displayName
                                        model: [
                                            { id: qsTr("YEAR"), displayName: qsTr("year") },
                                            { id: qsTr("MONTH"), displayName: qsTr("month") },
                                            { id: qsTr("WEEK"), displayName: qsTr("week") },
                                            { id: qsTr("DAY"), displayName: qsTr("day") },
                                        ]
                                        textRole: "displayName"
                                        onSignalActivated: {
                                            vm.inactivityUnit = _interal.current_id.id
                                        }
                                    }
                                }
                                
                                QLato {
                                    font.pixelSize: 16
                                    font.weight: Font.Normal
                                    text: STR.STR_QML_2198
                                    anchors {
                                        bottom: parent.bottom
                                        bottomMargin: 8
                                    }
                                }
                            }
                        }
                    }
                    
                }
            }
        }
    }
    bottomLeft: Row {
        spacing: 12
        QButtonTextLink {
            width: 97
            height: 48
            label: STR.STR_QML_059
            visible: vm.fallbackOption == FallbackOption.FallbackSecond
            onButtonClicked: {
                vm.back()
            }
        }
        QTextButton {
            width: 100
            height: 48
            visible: vm.fallbackOption == FallbackOption.FallbackFirst
            label.text: STR.STR_QML_245
            label.font.pixelSize: 14
            type: eTypeF
            onButtonClicked: vm.close()
        }
    }
    bottomRight: Row {
        spacing: 12        
        QTextButton {
            width: label.paintedWidth + 32
            height: 48
            label.text: STR.STR_QML_835
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                vm.onContinueClicked()
            }
        }
    }
}
