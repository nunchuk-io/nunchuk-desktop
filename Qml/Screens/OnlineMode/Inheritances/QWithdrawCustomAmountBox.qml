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
import QRCodeItem 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    id: sendDelegateRoot
    width: parent.width
    height: 192
    color: "#F5F5F5"
    radius: 12

    property var inputObject: ({
                                   "toType": "Input", // "Input, Address, Wallet"
                                   "toAmount": ""
                               })

    property alias toAmount: amountInput.textInputted
    property bool  onCurrency: false

    signal sendAllRequest()

    Column {
        anchors.fill: parent
        anchors.margins: 18
        spacing: 12
        Item {
            width: parent.width
            height: childrenRect.height
            QTextInputBoxTypeB {
                id: amountInput
                label: STR.STR_QML_214
                boxWidth: 693
                boxHeight: 48
                isValid: true
                validator: {
                    if(sendDelegateRoot.onCurrency) {
                        return doubleValidator
                    }
                    else{
                        return (AppSetting.unit === NUNCHUCKTYPE.SATOSHI) ? intvalidator : doubleValidator
                    }
                }
            }
            Row {
                anchors.bottom: amountInput.bottom
                anchors.bottomMargin: 12
                anchors.right: amountInput.right
                anchors.rightMargin: 12
                spacing: 4
                QLato {
                    height: 24
                    anchors.verticalCenter: parent.verticalCenter
                    text: {
                        if(sendDelegateRoot.onCurrency) {
                            return qsTr("%1").arg(AppSetting.currency)
                        }
                        else{
                            return qsTr("%1").arg((AppSetting.unit === NUNCHUCKTYPE.SATOSHI) ? "sat" : "BTC")
                        }
                    }
                    font.weight: Font.Bold
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                }
                QIcon {
                    iconSize: 24
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/fav_swapt-24px.svg"
                    transformOrigin: Item.Center
                    scale: moneyMouse.pressed ? 0.95 : 1
                    MouseArea {
                        id: moneyMouse
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        anchors.fill: parent
                        onClicked: {
                            sendDelegateRoot.onCurrency = !sendDelegateRoot.onCurrency
                        }
                    }
                }
            }
            QButtonTextLink {
                height: 24
                label: STR.STR_QML_262
                displayIcon: false
                btnText.font.underline: true
                anchors.top: amountInput.top
                anchors.right: amountInput.right
                onButtonClicked: {
                    sendAllRequest()
                }
            }
        }
    }
    RegExpValidator { id: intvalidator;      regExp: /^[1-9][0-9]*$/ }
    RegExpValidator { id: doubleValidator;   regExp: /^(?:0|[1-9][0-9]*)(\.\d{1,8})?$/ }
}
