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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR
QScreen {
    QOnScreenContent {
        id: contenCenter
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_304
        extraHeader: Row {
            spacing: 8
            Rectangle {
                width: visible ? 84 : 0
                height: visible ? 24 : 0
                color: "#031F2B"
                radius: 4
                visible: (NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex) ? RoomWalletData.roomWallet.walletEscrow : AppModel.walletInfo.walletEscrow
                anchors.verticalCenter: parent.verticalCenter
                Row {
                    anchors.centerIn: parent
                    spacing: 3
                    QImage {
                        source: "qrc:/Images/Images/Escrow Wallet.png"
                        width: 16
                        height: 16
                    }
                    QText {
                        text: STR.STR_QML_212
                        color: "#F8D418"
                        width: 49
                        height: 10
                        font.weight: Font.Bold
                        font.family: "Lato"
                        font.pixelSize: 10
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            QText {
                text: (NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex) ? "(" + RoomWalletData.walletName + ")": "(" + AppModel.walletInfo.walletName + ")"
                color: "#031F2B"
                font.weight: Font.DemiBold
                font.family: "Montserrat"
                font.pixelSize: 14
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        onCloseClicked: {
            if(NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex){
                QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT)
            }
            else{
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT)
            }
        }
        QText {
            anchors {
                left: parent.left
                leftMargin: 43
                top: parent.top
                topMargin: 92
            }
            text: STR.STR_QML_257
            color: "#031F2B"
            font.weight: Font.Bold
            font.pixelSize: 16
        }
        Row {
            spacing: 10
            height: 24
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: flickcontent.top
                bottomMargin: 8
            }
            QText {
                height: 21
                width: 110
                text: STR.STR_QML_305
                color: "#000000"
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 14
                anchors.verticalCenter: parent.verticalCenter
            }
            QText {
                height: 24
                width: 140
                text: AppModel.utxoList.amountDisplay + RoomWalletData.unitValue
                color: "#000000"
                font.family: "Lato"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignRight
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Flickable {
            id: flickcontent
            width: 720
            height: 350
            flickableDirection: Flickable.VerticalFlick
            clip: true
            contentHeight: destColumn.height
            ScrollBar.vertical: ScrollBar { active: true }
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 171
            }
            Column {
                id: destColumn
                spacing: 16
                Row {
                    id: destinationRow
                    spacing: 32
                    QTextInputBox {
                        id: toAddr
                        width: 419
                        heightMin: 56
                        mode: eEDIT_MODE
                        placeholder.text: STR.STR_QML_260
                        border.color: "#DEDEDE"
                    }
                    QTextInputBox {
                        id: amount
                        width: 269
                        heightMin: 56
                        mode: eREADONLY_MODE
                        placeholder.text: STR.STR_QML_261
                        textOutput: AppModel.utxoList.amountDisplay
                        color: Qt.rgba(0, 0, 0, 0.1)
                        border.color: "#DEDEDE"
                        rightPading: 40
                        QText {
                            color: "#839096"
                            width: 23
                            height: 16
                            anchors {
                                top: parent.top
                                topMargin: (amount.textActiveFocus) || (amount.textOutput !== "") ? 28: 20
                                right: parent.right
                                rightMargin: 16
                            }
                            text: RoomWalletData.unitValue
                            font.pixelSize: 16
                            font.family: "Lato"
                        }
                    }
                }

                QTextInputBox {
                    id: memo
                    width: 720
                    heightMin: 96
                    mode: eEDIT_MODE
                    placeholder.text: STR.STR_QML_264
                    border.color: "#DEDEDE"
                }
            }
        }
        Row {
            spacing: 16
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            QTextButton {
                width: 120
                height: 48
                label.text: STR.STR_QML_245
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: { QMLHandle.sendEvent(EVT.EVT_CONSOLIDATE_BACK_REQUEST) }
            }
            QTextButton {
                width: 260
                height: 48
                enabled: (toAddr.textOutput !== "") && (amount.textOutput !== "")
                label.text: STR.STR_QML_265
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    var destinationObj = { "toAddress": toAddr.textOutput,
                                           "toAmount":  amount.textOutput,
                                           "destinationMemo"    : memo.textOutput};
                    createTxBusyBox.open()
                    timerCreateTx.destinationData = destinationObj
                    timerCreateTx.restart()
                }
            }
        }
    }
    Popup {
        id: createTxBusyBox
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        Rectangle {
            id: boxmask
            width: 300
            height: 196
            radius: 24
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 300
                    height: 196
                    radius: 24
                }
            }
            Column {
                spacing: 12
                anchors.centerIn: parent
                QBusyIndicator {
                    width: 70
                    height: 70
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    text: STR.STR_QML_236
                }
                QText {
                    width: 252
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    text: STR.STR_QML_122
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
        DropShadow {
            anchors.fill: boxmask
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: boxmask
        }
    }
    Timer {
        id: timerCreateTx
        property var destinationData
        interval: 500
        onTriggered: {
            QMLHandle.sendEvent(EVT.EVT_CONSOLIDATE_MAKE_TRANSACTION, timerCreateTx.destinationData)
            createTxBusyBox.close()
        }
    }
}
