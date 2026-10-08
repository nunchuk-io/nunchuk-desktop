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
import QRCodeItem 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR
QScreen {
    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_306
        extraHeader: QText {
            height: 21
            text: (NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex) ? "(" + RoomWalletData.walletName + ")": "(" + AppModel.walletInfo.walletName + ")"
            color: "#031F2B"
            font.weight: Font.DemiBold
            font.family: "Montserrat"
            font.pixelSize: 14
            anchors.verticalCenter: parent.verticalCenter
        }
        onCloseClicked: {
            if(NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex){
                QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_UTXO_OUTPUT)
            }
            else{
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_UTXO_OUTPUT)
            }
        }
        QButtonTextLink {
            height: 24
            label: STR.STR_QML_059
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_UTXO_INFO_BACK_REQUEST)
            }
        }
        Flickable {
            id: flickcontent
            width: 720
            height: Math.min(500, contentHeight)
            contentHeight: contentDisplay.height
            flickableDirection: Flickable.VerticalFlick
            clip: true
            ScrollBar.vertical: ScrollBar { active: true }
            interactive: height < contentHeight
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 108
            }
            Column {
                id: contentDisplay
                width: 720
                spacing: 16

                Row {
                    width: parent.width
                    height: 54
                    spacing: 30
                    QTextInputBox {
                        width: 360
                        height: parent.height
                        color: "transparent"
                        placeholder.text: STR.STR_QML_214
                        mode: eREADONLY_MODE
                        textOutput: AppModel.walletInfo.utxoInfo.amount + RoomWalletData.unitValue
                    }
                    QTextInputBox {
                        width: 150
                        height: parent.height
                        color: "transparent"
                        placeholder.text: STR.STR_QML_287
                        mode: eREADONLY_MODE
                        textOutput: Math.max(0, (AppModel.blockHeight - AppModel.walletInfo.utxoInfo.height)+1)
                    }
                    QTextInputBox {
                        width: 150
                        height: parent.height
                        color: "transparent"
                        placeholder.text: STR.STR_QML_307
                        mode: eREADONLY_MODE
                        textOutput: AppModel.walletInfo.utxoInfo.vout
                    }
                }

                QTextInputBox {
                    width: parent.width
                    heightMin: 54
                    color: "transparent"
                    placeholder.text: STR.STR_QML_308
                    mode: eREADONLY_MODE
                    textOutput: AppModel.walletInfo.utxoInfo.txid
                }
                QTextInputBox {
                    width: parent.width
                    heightMin: 54
                    color: "transparent"
                    placeholder.text: STR.STR_QML_309
                    mode: eREADONLY_MODE
                    textOutput: AppModel.walletInfo.utxoInfo.address
                }
                QTextInputBox {
                    width: parent.width
                    heightMin: 54
                    color: "transparent"
                    placeholder.text: STR.STR_QML_310
                    mode: eREADONLY_MODE
                    textOutput: AppModel.walletInfo.utxoInfo.scriptPublickey
                }
            }
        }
        QButtonTextLink {
            height: 24
            label: STR.STR_QML_311
            direction: eRIGHT
            anchors {
                right: parent.right
                rightMargin: 52
                top: flickcontent.bottom
                topMargin: 48
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_UTXO_INFO_CHECKOUT_TX_RELATED, AppModel.walletInfo.utxoInfo.txid)
            }
        }
    }
}
