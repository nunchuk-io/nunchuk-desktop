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
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Chats"
import "./../../../customizes/Texts"
import "./../../../customizes/Buttons"
import "./../../../customizes/Lists"
import "../../../../../localization/STR_QML.js" as STR

Item {
    property var inheritanceInfo: ServiceSetting.servicesTag.inheritanceInfo
    property string optionSelected: "withdraw-a-custom-amount"
    property var optionList: {
        var ls = []
        ls.push({ id: "withdraw-a-custom-amount",  label: STR.STR_QML_1736 })
        ls.push({ id: "withdraw-full-balance-now", label: STR.STR_QML_1737 })
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
                QMontserrat {
                    width: 627
                    text: STR.STR_QML_1735
                    font.pixelSize: 32
                    font.weight: Font.DemiBold
                    wrapMode: Text.WordWrap
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
                QRadioList {
                    width: 500
                    height: optionList.length * 60
                    model: optionList
                    delegateHeight: 60
                    spacing: 12
                    delegateType: _RadioTypeB
                    function radioSelect() {
                        return optionSelected
                    }
                    function setRadioSelect(id, data) {
                        optionSelected = id
                        ServiceSetting.servicesTag.setClaimInheritanceCustomAmount(0)
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
                QIconTextButton {
                    width: 98
                    height: 48
                    label: STR.STR_QML_059
                    icons: ["left-arrow-dark.svg", "left-arrow-dark.svg", "left-arrow-dark.svg","left-arrow-dark.svg"]
                    fontPixelSize: 16
                    iconSize: 24
                    type: eTypeJ
                    onButtonClicked: {
                        ServiceSetting.servicesTag.updateInheritanceCheckStatus("isWithdrawBitcoin", false)
                    }
                }
                QTextButton {
                    width: 100
                    height: 48
                    label.text: STR.STR_QML_265
                    label.font.pixelSize: 16
                    type: eTypeE
                    visible: optionSelected === "withdraw-a-custom-amount"
                    onButtonClicked: {
                        var _input = {
                            "type": optionSelected,
                            "isShowScreen": true,
                        }
                        ServiceSetting.handleClaimInheritance(_input)
                    }
                }
                QButtonLargeTail {
                    width: 220
                    height: 48
                    type: eTHIRT
                    label: STR.STR_QML_778
                    visible: optionSelected === "withdraw-full-balance-now"
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
                        onItemClicked: {
                            var map_flow = [
                                {flow_action: "withdraw-to-a-nunchuk-wallet",      },
                                {flow_action: "withdraw-to-address",               },                                
                            ]
                            var type = map_flow[index].flow_action
                            var _input = {
                                "type": type,
                                "isShowScreen": true,
                            }
                            ServiceSetting.handleClaimInheritance(_input)
                        }
                    }
                }
            }
        }
    }
}
