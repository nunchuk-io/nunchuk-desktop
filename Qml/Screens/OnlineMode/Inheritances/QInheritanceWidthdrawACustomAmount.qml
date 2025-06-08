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
import QtQuick 2.4
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
import "../../../Components/customizes/QRCodes"
import "../../OnlineMode/Inheritances"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id:_customAmount
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1736
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property var inheritanceInfo: ServiceSetting.servicesTag.inheritanceInfo
    content: Item {
        Column {
            id: destColumn
            spacing: 16
            Item {
                width: 480
                height: 28
                Row {
                    anchors.fill: parent
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 6
                    QLato {
                        height: parent.height
                        text: STR.STR_QML_259
                        font.weight: Font.Bold
                        font.pixelSize: 16
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                    QLato {
                        height: parent.height
                        text: inheritanceInfo.balance + (AppSetting.unit === NUNCHUCKTYPE.BTC ? " BTC" : " sats")
                        font.weight: Font.Normal
                        font.pixelSize: 16
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                    QLato {
                        height: parent.height
                        text: qsTr("(%1%2)").arg(AppSetting.currency).arg(inheritanceInfo.balanceCurrency)
                        color: "#757575"
                        font.pixelSize: 12
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
            QWithdrawCustomAmountBox {
                id: destination
                anchors.horizontalCenter: parent.horizontalCenter
                width: 728
                height: 108
                onSendAllRequest: {
                    destination.toAmount = inheritanceInfo.balance
                }
            }
        }
        function getInputAmount(index){
            var map_flow = [
                {flow_action: "withdraw-to-a-nunchuk-wallet",      },
                {flow_action: "withdraw-to-address",               },                                
            ]
            var type = map_flow[index].flow_action
            var amount = convertToSatoshi(destination.onCurrency, destination.toAmount)
            var savedObj = {
                "type": type,
                "amount": amount,
            }
            return savedObj;
        }
        function convertToSatoshi(isCurrency, amount) {
            if(isCurrency) {
                return AppModel.qAmountFromCurrency(amount)
            }
            else if (AppSetting.unit === NUNCHUCKTYPE.BTC){
                return AppModel.qAmountFromBTC(amount)
            } else {
                return amount
            }
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    onNextClicked: {
        imExContextMenu.x = _customAmount.width - imExContextMenu.width
        imExContextMenu.y = _customAmount.height - 102 - imExContextMenu.height
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
            var inputObj = _customAmount.contentItem.getInputAmount(index)
            ServiceSetting.handleClaimInheritance(inputObj)
        }
    }
}


