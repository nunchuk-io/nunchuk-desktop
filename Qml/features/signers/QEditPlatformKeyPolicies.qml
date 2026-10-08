/*
 * This file is part of the Nunchuk software (https://nunchuk.io/)
 * Copyright (C) 2020-2024 Enigmo
 * Copyright (C) 2024 Nunchuk
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import Features.Signers.ViewModels 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Lists"
import "./platform"

QOnScreenContentTypeA {
    id: _content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2121
    onCloseClicked: vm.close()
    onPrevClicked: vm.back()
    extraHeader: Item { }
    readonly property var widthEdit: 343

    content: Item {

        Column {
            width: 435
            spacing: 8
            Row {
                width: parent.width
                height: 60
                spacing: 12
                QCircleIcon {
                    id: iconNameEdit
                    bgSize: 48
                    icon.iconSize: 24
                    icon.type: vm.signerType
                    icon.tag:  vm.signerTag
                    icon.iconUrl: vm.signerType === -1 ? "qrc:/Images/Images/global-key.svg" : ""
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                Column {
                    width: 200
                    spacing: 4
                    anchors.verticalCenter: iconNameEdit.verticalCenter
                    QLato {
                        width: parent.width
                        text: vm.policyName
                        font.pixelSize: 20
                        font.weight: Font.Demibold
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                    QLato {
                        width: parent.width
                        text: {
                            if(vm.enableSpendingLimit) {
                                var policy_string = (vm.currency ?? "") + " " + (vm.balance ?? "") + " / " + (vm.intervalStr ?? "")
                                if (policy_string.trim() === "/") {
                                    policy_string = STR.STR_QML_2148
                                }
                                return policy_string
                            } else {
                                return STR.STR_QML_2148
                            }
                        }
                        font.pixelSize: 16
                        font.weight: Font.Normal
                        color: "#031F2B"
                        wrapMode: Text.WrapAnywhere
                        lineHeight: 28
                        lineHeightMode: Text.FixedHeight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }

            Flickable {
                width: parent.width
                height: 440
                contentWidth: editContentCol.width
                contentHeight: editContentCol.height
                clip: true
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: editContentCol
                    width: 435
                    spacing: 8
                    QSetupEnableToggle {
                        id: _spendingLimitToggle
                        width: widthEdit
                        height: 79
                        label: STR.STR_QML_799
                        description: STR.STR_QML_2139
                        checked: vm.enableSpendingLimit
                        onToggled: {
                            vm.enableSpendingLimit = checked
                        }
                    }

                    QSetupSpendingLimit {
                        id: _spendingLimit
                        width: widthEdit
                        limit: vm.balance
                        currency: vm.currency
                        interval: vm.interval
                        visible: vm.enableSpendingLimit
                        onLimitInputChanged: {
                            vm.balance = value
                        }
                        onCurrencyInputChanged: {
                            vm.currency = value
                        }
                        onIntervalInputChanged: {
                            vm.interval = value
                        }
                    }

                    Rectangle {
                        width: widthEdit
                        height: 1
                        color: "#EAEAEA"
                    }

                    QSetupEnableToggle {
                        id: _coSigningDelayToggle
                        width: widthEdit
                        height: 79
                        label: STR.STR_QML_795
                        description: STR.STR_QML_796
                        checked: vm.enableCoSigningDelay
                        onToggled: {
                            vm.enableCoSigningDelay = checked
                        }
                    }

                    Row {
                        width: widthEdit
                        height: 68
                        spacing: 16
                        visible: vm.enableCoSigningDelay
                        QTextInputBoxTypeB {
                            id: input_hours
                            label: STR.STR_QML_797
                            titleFontSize: 12
                            boxWidth: 163
                            boxHeight: 48
                            textInputted: vm.hour.toString()
                            input.placeholderText: ""
                            isValid: textInputted !== ""
                            validator: RegExpValidator {
                                regExp: /^(0?\d|1\d|2[0-3])$/
                            }
                            onTypingFinished: {
                                vm.hour = currentText === "" ? 0 : parseInt(currentText)
                            }
                        }
                        QTextInputBoxTypeB {
                            id: input_minutes
                            label: STR.STR_QML_798
                            titleFontSize: 12
                            boxWidth: 163
                            boxHeight: 48
                            textInputted: vm.minute.toString()
                            input.placeholderText: ""
                            validator: RegExpValidator {
                                regExp: /^(0?\d|[1-5]\d)$/
                            }
                            onTypingFinished: {
                                vm.minute = currentText === "" ? 0 : parseInt(currentText)
                            }
                        }
                    }

                    QSetupEnableToggle {
                        id: _autoBroadcastToggle
                        width: 343
                        height: 87
                        label: STR.STR_QML_2125
                        description: STR.STR_QML_2136
                        checked: vm.autoBroadcast
                        onToggled: {
                            vm.autoBroadcast = checked
                        }
                    }
                }
            }
        }
    }

    bottomRight: QTextButton {
        width: 74
        height: 48
        label.text: "Apply"
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            vm.onApplyClicked()
        }
    }

    EditPlatformKeyPoliciesViewModel {
        id: vm
    }
}
