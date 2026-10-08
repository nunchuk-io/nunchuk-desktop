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
import "../../Components/customizes/Popups"
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
    
    content: Item {
        Column {
            width: parent.width
            spacing: 24
            Item {
                width: 472
                height: 64
                Column {
                    width: parent.width
                    Item {
                        width: parent.width
                        height: 36
                        QLato {
                            text: STR.STR_QML_2122
                            color: vm.isTypeChanged ? "#CF4018" : "#031F2B"
                            font.pixelSize: 16
                            font.weight: Font.Bold
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            anchors.verticalCenter: parent.verticalCenter                               
                        }
                        QTextLink {
                            id: _edit
                            width: _edit.paintedWidth
                            height: 20
                            text: STR.STR_QML_849
                            anchors {
                                verticalCenter: parent.verticalCenter
                                right: parent.right
                            }
                            onTextClicked: {
                                vm.onEditClicked()
                            }
                            // visible: !vm.isWallet
                        }
                    }
                    QLato {
                        text: STR.STR_QML_2123
                        color: vm.isTypeChanged ? "#CF4018" : "#031F2B"
                        font.pixelSize: 16
                        font.weight: Font.Normal
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }

            QPlatformKeyPolicyDelegate {
                width: 400
                height: 140
                label: vm.globalPolicy.singleSigner_name ?? ""
                subLabel: {
                    var str = (vm.globalPolicy.currency ?? "") + " " + (vm.globalPolicy.balance ?? "") + " / " + (vm.globalPolicy.interval ? formatInterval(vm.globalPolicy.interval) : "")
                    if (str.trim() === "/") {
                        str = STR.STR_QML_2148
                    }
                    if (vm.globalPolicy.enableSpendingLimit ?? false) {
                        str
                    } else {
                        STR.STR_QML_2148
                    }                    
                }
                subLabelChanged: (vm.globalPolicy.currencyChanged ?? false) || (vm.globalPolicy.balanceChanged ?? false) || (vm.globalPolicy.intervalChanged ?? false)
                iconUrl: "qrc:/Images/Images/global-key.svg"
                enableCoSigningDelay: vm.globalPolicy.enableCoSigningDelay ?? false
                coSigningDelayChanged: vm.globalPolicy.enableCoSigningDelayChanged ?? false
                autoBroadcast: vm.globalPolicy.autoBroadcast ?? false
                autoBroadcastChanged: vm.globalPolicy.autoBroadcastChanged ?? false
                onEditClicked: vm.onEditPolicyClicked()
            }
        }
    }

    bottomRight: Row {
        spacing: 12
        QButtonTextLink {
            width: 200
            height: 48
            label: STR.STR_QML_2129
            textColor: ["#CF4018", "#CF4018", "#CF4018"]
            displayIcon:false
            visible: vm.isEntryPointGroup
            onButtonClicked: {
                vm.onRemovePlatformKeyClicked()
            }
        }
        QTextButton {
            width: label.paintedWidth + 32
            height: 48
            label.text: STR.STR_QML_193
            type: eTypeR
            visible: vm.isEntryPointAlert
            onClicked: vm.onDiscardChangesClicked()
        }
        QTextButton {
            width: label.paintedWidth + 32
            height: 48
            label.text: STR.STR_QML_2130
            type: eTypeE
            visible: vm.isEntryPointGroup
            onClicked: vm.onContinueToSaveChangesClicked()
        }
        QTextButton {
            width: label.paintedWidth + 32
            height: 48
            label.text: STR.STR_QML_2130
            type: eTypeE
            visible: vm.isEntryPointWallet
            onClicked: vm.onApplyClicked()
        }
        QTextButton {
            width: label.paintedWidth + 32
            height: 48
            label.text: STR.STR_QML_2138.arg(vm.pending_signatures).arg((vm.pending_signatures > 1) ? "s" : "")
            type: eTypeE
            visible: vm.isEntryPointAlert
            onClicked: vm.onContinueSignaturePendingClicked()
        }
    }

    QPopupInfoTwoButtons {
        id: _SecurityDelayRequired
        btnWith: 118
        isVertical: false
        title: STR.STR_QML_661
        contentText: STR.STR_QML_2141.arg(vm.afterHours)
        labels: [STR.STR_QML_035, STR.STR_QML_509]
        funcs: [
            function() { _SecurityDelayRequired.close() },
            function() { 
                vm.onSignClicked()
            }
        ]
    }

    QPopupInfoTwoButtons {
        id: _DummyTransactionRequired
        btnWith: 118
        isVertical: false
        title: STR.STR_QML_661
        contentText: STR.STR_QML_2140
        labels: [STR.STR_QML_035, STR.STR_QML_509]
        funcs: [
            function() { _DummyTransactionRequired.close() },
            function() { 
                vm.onSignClicked()
            }
        ]
    }
    
    Connections {
        target: vm
        onNotifySecurityDelayRequired: _SecurityDelayRequired.open()
        onNotifyDummyTransactionRequired: _DummyTransactionRequired.open()
    }
    
    GlobalPlatformKeyPoliciesViewModel {
        id: vm
    }
}
