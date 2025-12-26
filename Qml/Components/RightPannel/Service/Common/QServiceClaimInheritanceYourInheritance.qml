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
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import Features.Claiming.ViewModels 1.0
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Chats"
import "./../../../customizes/Texts"
import "./../../../customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Item {
    property var inheritanceInfo: ServiceSetting.servicesTag.inheritanceInfo

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
                    text: STR.STR_QML_775
                    font.pixelSize: 20
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignHCenter
                }
                QMontserrat {
                    width: parent.width
                    text: inheritanceInfo.balance + (AppSetting.unit === NUNCHUCKTYPE.BTC ? " BTC" : " sats")
                    font.pixelSize: 32
                    font.weight: Font.Medium
                    horizontalAlignment: Text.AlignHCenter
                }
                QLato {
                    width: parent.width
                    text: qsTr("%1 %2").arg(AppSetting.currency).arg(inheritanceInfo.balanceCurrency)
                    font.pixelSize: 20
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
                QText {
                    width: 700
                    text: STR.STR_QML_776
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.pixelSize: 32
                    font.weight: Font.Medium
                    wrapMode: Text.WordWrap
                }
                QText {
                    width: 343
                    text: STR.STR_QML_781
                    color: "#031F2B"
                    font.family: "Lato"
                    font.pixelSize: 20
                    font.weight: Font.Normal
                    visible: _message.text !== ""
                }
                Rectangle {
                    width: 627
                    height: _message.paintedHeight + 12*2
                    color: "#F5F5F5"
                    radius: 12
                    visible: _message.text !== ""
                    QText {
                        id: _message
                        width: 627 - 12*2
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            top: parent.top
                            topMargin: 12
                        }
                        text: inheritanceInfo.note
                        color: "#000000"
                        font.family: "Lato"
                        font.pixelSize: 16
                        font.weight: Font.Normal
                        wrapMode: Text.WordWrap
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
                    width: 71
                    height: 48
                    label.text: STR.STR_QML_777
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        ServiceSetting.servicesTag.clearInheritance()
                    }
                }
                QTextButton {
                    width: 158
                    height: 48
                    label.text: STR.STR_QML_1735
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        var _input = {
                            "isWithdrawBitcoin": true
                        }
                        vm.withdrawBitcoinClicked(_input)
                    }
                }
            }
        }
    }
    InheritanceUnlockedViewModel {
        id: vm
        Component.onCompleted: {
            vm.attachContext(vmContext)
        }
    }
}
