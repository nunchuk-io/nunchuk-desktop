import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.12
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
import Features.Inheritance.OffChain.ViewModels 1.0
import "../../../Components/customizes/Texts"

QOnScreenContentTypeA {
    id: root
    
    WithdrawalRuleViewModel {
        id: vm
    }
    
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: qsTr("Stage %1").arg(vm.stage.index)
    onCloseClicked: vm.close()
    
    property var timeUnits: [STR.STR_QML_2199, STR.STR_QML_2200, STR.STR_QML_2201, STR.STR_QML_2202]  // year, month, week, day
    
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
                            color: "#031F2B"
                            radius: 12
                        }
                        Rectangle {
                            width: 40
                            height: parent.height
                            color: "#EAEAEA"
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
                        confirmDeletePopup.open()
                    }
                }
                // Header: Withdrawal rule:
                QLato {
                    anchors {
                        left: parent.left
                        bottom: parent.bottom
                    }
                    text: STR.STR_QML_2158 + ":"
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
                        text: STR.STR_QML_2209
                        font.pixelSize: 16
                        wrapMode: Text.WordWrap
                        lineHeight: 20
                        lineHeightMode: Text.FixedHeight
                    }
                    
                    QLato {
                        id: noteText
                        text: STR.STR_QML_2210
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
                            id: amountPerReleaseInput
                            label: STR.STR_QML_2211
                            boxWidth: parent.width
                            boxHeight: 48
                            textInputted: vm.amountPerRelease
                            validator: RegExpValidator {
                                regExp: /^$|^[0-9]+$/
                            }
                            isValid: vm.amountPerRelease > 0 && vm.amountPerRelease <= 100
                            onTextInputtedChanged: {
                                if (textInputted !== "") {
                                    vm.amountPerRelease = parseInt(textInputted)
                                }
                            }
                        }
                        QLato {
                            anchors {
                                right: parent.right
                                rightMargin: 12
                                bottom: parent.bottom
                                bottomMargin: 12
                            }
                            text: "%"
                            font.pixelSize: 16
                        }
                    }
                    
                    Item {
                        width: (parent.width - 12) / 2
                        height: 68
                        QTextInputBoxTypeB {
                            id: repeatEveryInput
                            label: STR.STR_QML_2212
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
                                rightMargin: 6
                                bottom: parent.bottom
                                bottomMargin: -8
                            }
                            width: 100
                            height: 48
                            background: Item {}
                            current_id: {
                                for (var i = 0; i < _interal.model.length; i++) {
                                    if (_interal.model[i].id === vm.repeatInterval) {
                                        return _interal.model[i]
                                    }
                                }
                                return _interal.model[0]
                            }
                            defaultValue: {
                                return { id: qsTr("%1").arg(_interal.model[0].interval),  displayName: qsTr("%1").arg(_interal.model[0].displayName) }
                            }
                            displayText: getDisplay().displayName
                            model:  [
                                { id: qsTr("YEAR"),  displayName: qsTr("year") },
                                { id: qsTr("MONTH"), displayName: qsTr("month")},
                                { id: qsTr("WEEK"),  displayName: qsTr("week") },
                                { id: qsTr("DAY"),   displayName: qsTr("day")  },
                            ]
                            textRole: "displayName"
                            onSignalActivated: {
                                vm.repeatInterval = _interal.current_id.id
                                // _row.intervalInputChanged(_interal.getDisplay().id)
                            }
                        }
                    }
                }
                
                Item {
                    width: (parent.width - 12) / 2
                    height: 68
                    QTextInputBoxTypeB {
                        id: totalAllocationInput
                        label: STR.STR_QML_2213
                        boxWidth: parent.width
                        boxHeight: 48
                        textInputted: vm.totalAllocation
                        validator: RegExpValidator {
                            regExp: /^$|^[0-9]+$/
                        }
                        isValid: vm.totalAllocation <= 100
                        onTextInputtedChanged: {
                            if (textInputted !== "") {
                                vm.totalAllocation = parseInt(textInputted)
                            }
                        }
                    }
                    QLato {
                        anchors {
                            right: parent.right
                            rightMargin: 12
                            bottom: parent.bottom
                            bottomMargin: 12
                        }
                        text: "%"
                        font.pixelSize: 16
                    }
                }
            }
        }
    }
    onPrevClicked: vm.back()

    QConfirmYesNoPopup {
        id: confirmDeletePopup
        title: STR.STR_QML_2245 + " " + vm.stage.index
        contentText: STR.STR_QML_2246 + "\n\n" + STR.STR_QML_2247
        leftBtnLabel: STR.STR_QML_245
        rightBtnLabel: STR.STR_QML_427
        onConfirmNo: close()
        onConfirmYes: {
            close()
            vm.deleteClicked()
        }
    }

    bottomRight: Row {
        spacing: 12
        
        QTextButton {
            width: label.implicitWidth + 32
            height: 48
            label.text: STR.STR_QML_097  // "Continue"
            label.font.pixelSize: 14
            type: eTypeE
            enabled: vm.amountPerRelease > 0 && vm.amountPerRelease <= 100 && vm.totalAllocation <= 100
            onButtonClicked: {
                vm.continueClicked()
            }
        }
    }
}
