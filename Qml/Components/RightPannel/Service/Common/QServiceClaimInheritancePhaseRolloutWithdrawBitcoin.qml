/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
import QtQuick 2.12
import QtQuick.Controls 2.0
import NUNCHUCKTYPE 1.0
import Features.Claiming.ViewModels 1.0
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Chats"
import "./../../../customizes/Texts"
import "./../../../customizes/Buttons"
import "./../../../customizes/Lists"

Item {
    property string optionSelected: "withdraw-a-custom-amount"
    property var optionList: {
        var ls = []
        ls.push({ id: "withdraw-a-custom-amount",  label: STR.STR_QML_1736 })
        ls.push({ id: "withdraw-available-balance-now", label: STR.STR_QML_2244 })
        return ls
    }
    Column {
        anchors.fill: parent
        spacing: 24
        anchors{
            left: parent.left
            leftMargin: 24
            top: parent.top
            topMargin: 24
        }
        Rectangle {
            width: parent.width - 24
            height: parent.height * 0.42
            radius: 24
            color: "#D0E2FF"
            Column {
                anchors.centerIn: parent
                width: 798
                height: 128
                spacing: 12
                QLato {
                    width: parent.width
                    text: STR.STR_QML_2243
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignHCenter
                }
                QMontserrat {
                    width: parent.width
                    text: vm.availableBalanceDisplay + (AppSetting.unit === NUNCHUCKTYPE.BTC ? " BTC" : " sats")
                    font.pixelSize: 32
                    font.weight: Font.Medium
                    horizontalAlignment: Text.AlignHCenter
                }
                QLato {
                    width: parent.width
                    text: "from " + vm.balanceDisplay + (AppSetting.unit === NUNCHUCKTYPE.BTC ? " BTC" : " sats") + " total"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    color: "#032B3C"
                    horizontalAlignment: Text.AlignHCenter
                }
                QLine {
                    width: 120
                    height: 1
                    color: "#7E519B"
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                QLato {
                    width: parent.width
                    text: qsTr("%1 %2").arg(AppSetting.currency).arg(vm.availableBalanceCurrency)
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignHCenter
                }
                QLato {
                    width: parent.width
                    text: "from " + qsTr("%1 %2").arg(AppSetting.currency).arg(vm.balanceCurrency) + " total"
                    font.pixelSize: 12
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
        Item {
            width: parent.width - 24
            height: parent.height * 0.58 - 24
            Column {
                width: parent.width
                spacing: 16
                QMontserrat {
                    width: 627
                    text: STR.STR_QML_1735
                    font.pixelSize: 32
                    font.weight: Font.DemiBold
                    wrapMode: Text.WordWrap
                }
                QTextLink {
                    width: 345
                    height: 20
                    text: STR.STR_QML_2236
                    color: "#031F2B"
                    font.pixelSize: 12
                    onTextClicked: {
                        vm.viewReleaseScheduleClicked()
                    }
                }
                QLato {
                    width: 500
                    text: STR.STR_QML_1738
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Column {
                    width: 500
                    spacing: 12
                    QRadioButtonTypeF {
                        id: _customAmount
                        width: 356
                        height: 108
                        labelMaxWidth: 284
                        labelBottomLineHeight: 20
                        selected: optionSelected === "withdraw-a-custom-amount"
                        labelTop: "" //STR.STR_QML_1736
                        labelBottom: ""
                        onButtonClicked: { optionSelected = "withdraw-a-custom-amount" }
                        Item {
                            width: 271
                            height: 68
                            anchors {
                                bottom: _customAmount.bottom
                                bottomMargin: 18
                                left: _customAmount.left
                                leftMargin: 54
                            }
                            QTextInputBoxTypeB {
                                id: _input_balance
                                label: STR.STR_QML_1736
                                boxWidth: parent.width
                                boxHeight: 48
                                textInputted: {
                                    if (AppSetting.unit === NUNCHUCKTYPE.SATOSHI) {
                                        return vm.withdrawAmountSats
                                    } else if (AppSetting.unit === NUNCHUCKTYPE.BTC){
                                        return AppModel.qBTCFromAmount(vm.withdrawAmountSats)
                                    } else {
                                        return ""
                                    }
                                }
                                input.placeholderText: "0"
                                validator: (AppSetting.unit === NUNCHUCKTYPE.SATOSHI) ? intvalidator : doubleValidator
                                onTextInputtedChanged: {
                                    if(!_input_balance.isValid){
                                        _input_balance.isValid = true
                                        _input_balance.errorText = ""
                                    }
                                    _input_balance.showError = false;
                                    if (AppSetting.unit === NUNCHUCKTYPE.SATOSHI) {
                                        var tmp = parseInt(_input_balance.textInputted)
                                        vm.withdrawAmountSats = tmp > 0 ? tmp : 0
                                    } else if (AppSetting.unit === NUNCHUCKTYPE.BTC){
                                        var tmp = parseFloat(_input_balance.textInputted)
                                        vm.withdrawAmountSats = AppModel.qAmountFromBTC(tmp > 0 ? tmp : 0)
                                    }
                                }
                                input.onEditingFinished: {
                                    // Clamp to available balance when user leaves the field
                                    if (vm.withdrawAmountSats > vm.availableBalanceSats) {
                                        vm.withdrawAmountSats = vm.availableBalanceSats
                                    }
                                }
                            }
                            QLato {
                                anchors {
                                    bottom: _input_balance.bottom
                                    bottomMargin: 14
                                    right: _input_balance.right
                                    rightMargin: 12
                                }
                                width: 30
                                text: (AppSetting.unit === NUNCHUCKTYPE.BTC ? " BTC" : " sats")
                                font.pixelSize: 16
                                font.weight: Font.Normal
                                color: "#757575"
                                horizontalAlignment: Text.AlignLeft
                            }
                        }
                    }
                    QRadioButtonTypeF {
                        width: 356
                        height: 60
                        labelMaxWidth: 284
                        labelBottomLineHeight: 20
                        selected: optionSelected === "withdraw-available-balance-now"
                        labelTop: STR.STR_QML_2244
                        labelBottom: ""
                        onButtonClicked: { optionSelected = "withdraw-available-balance-now" }
                    }
                }
            }
            Row {
                spacing: 12
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    bottomMargin: 16
                }
                QTextButton {
                    width: 100
                    height: 48
                    label.text: STR.STR_QML_265
                    label.font.pixelSize: 16
                    type: eTypeE
                    visible: optionSelected === "withdraw-a-custom-amount"
                    enabled: vm.withdrawAmountSats > 0
                    onButtonClicked: {
                        if (vm.withdrawAmountSats > vm.availableBalanceSats) {
                            vm.withdrawAmountSats = vm.availableBalanceSats
                        }
                        vm.customAMountClicked()
                    }
                }
                QButtonLargeTail {
                    width: 220
                    height: 48
                    type: eTHIRT
                    label: STR.STR_QML_778
                    visible: optionSelected === "withdraw-available-balance-now"
                    optionVisible: imExContextMenu.visible
                    onButtonClicked: {
                        imExContextMenu.x = 20
                        imExContextMenu.y = 20 - imExContextMenu.height
                        imExContextMenu.open()
                    }
                    QContextMenu {
                        id: imExContextMenu
                        menuWidth: 320
                        labels: [
                            STR.STR_QML_779,
                            STR.STR_QML_780,
                        ]
                        icons: [
                            "qrc:/Images/Images/wallet-dark.svg",
                            "qrc:/Images/Images/spend-dark.svg",
                        ]
                        functions: [
                            function() { vm.withdrawToWalletClicked() },
                            function() { vm.withdrawToAddressClicked() },
                        ]
                        onItemClicked: {
                            var action = imExContextMenu.functions[index]
                            action()
                        }
                    }
                }
            }
        }
    }
    RegExpValidator { id: intvalidator;      regExp: /^[1-9][0-9]*$/ }
    RegExpValidator { id: doubleValidator;   regExp: /^(?:0|[1-9][0-9]*)(\.\d{1,8})?$/ }
    PhaseRolloutWithdrawBitcoinViewModel {
        id: vm
    }
}
