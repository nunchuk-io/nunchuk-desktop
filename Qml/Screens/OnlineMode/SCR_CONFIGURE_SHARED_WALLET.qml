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
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR
QScreen {
    id: roots
    popupWidth: 800
    popupHeight: 700

    QOnScreenContent {
        id: contenCenter
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_348
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CONFIGURE_SHARED_WALLET)
        }
        QIcon {
            iconSize: 24
            id: compressButton
            anchors {
                right: parent.right
                rightMargin: 84
                top: parent.top
                topMargin: 44
            }
            transformOrigin: Item.Center
            source: "qrc:/Images/Images/compress-24px.png"
            scale: compressmouse.pressed ? 1: compressmouse.containsMouse ? 1.1 : 1
            MouseArea {
                id: compressmouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CONFIGURE_SHARED_WALLET)
                }
            }
        }
        Column {
            spacing: 24
            anchors {
                left: parent.left
                leftMargin: 36
                top: parent.top
                topMargin: 100
            }
            Item {
                height: 48
                width: 540
                Column {
                    spacing: 4
                    anchors.verticalCenter: parent.verticalCenter
                    QText {
                        text: STR.STR_QML_349
                        color: "#031F2B"
                        font.weight: Font.DemiBold
                        font.pixelSize: 16
                        font.family: "Lato"
                    }
                    QText {
                        text: STR.STR_QML_350
                        color: "#031F2B"
                        font.weight: Font.Normal
                        font.pixelSize: 12
                        font.family: "Lato"
                    }
                }
                QVolumeTypeA {
                    width: 154
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    value: RoomWalletData.roomWalletReady ? RoomWalletData.roomWallet.walletN : 0
                    onValueChanged: {
                        if(value < RoomWalletData.roomWallet.walletM){
                            RoomWalletData.roomWallet.walletM = value;
                        }
                    }

                    onPlusClicked:{
                        if(RoomWalletData.roomWalletReady  && RoomWalletData.roomWallet.walletN < 15){
                            RoomWalletData.roomWallet.walletN++;
                        }
                    }
                    onMinusClicked: {
                        if(RoomWalletData.roomWalletReady && RoomWalletData.roomWallet.walletN > 2){
                            RoomWalletData.roomWallet.walletN--;
                        }
                    }
                }
            }
            Item {
                height: 48
                width: 540
                Column {
                    spacing: 4
                    anchors.verticalCenter: parent.verticalCenter
                    QText {
                        text: STR.STR_QML_351
                        color: "#031F2B"
                        font.weight: Font.DemiBold
                        font.pixelSize: 16
                        font.family: "Lato"
                    }
                    QText {
                        text: STR.STR_QML_352
                        color: "#031F2B"
                        font.weight: Font.Normal
                        font.pixelSize: 12
                        font.family: "Lato"
                    }
                }
                QVolumeTypeA {
                    width: 154
                    height: 50
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    value: RoomWalletData.roomWalletReady ? RoomWalletData.roomWallet.walletM : 0
                    onPlusClicked:{
                        if(RoomWalletData.roomWalletReady  && RoomWalletData.roomWallet.walletM < RoomWalletData.roomWallet.walletN){
                            RoomWalletData.roomWallet.walletM++;
                        }
                    }
                    onMinusClicked: {
                        if(RoomWalletData.roomWalletReady && RoomWalletData.roomWallet.walletM > 0){
                            RoomWalletData.roomWallet.walletM--;
                        }
                    }
                }
            }
        }
        Row {
            spacing: 8
            height: 24
            anchors.right: buttonBottomR.left
            anchors.rightMargin: 24
            anchors.verticalCenter: buttonBottomR.verticalCenter
            QText {
                font.family: "Lato"
                font.pixelSize: 12
                text: STR.STR_QML_556
                anchors.verticalCenter: parent.verticalCenter
                color: "#031F2B"
            }
            Rectangle {
                height: 24
                width: footerinfo.width + 20
                radius: 20
                color: "#EAEAEA"
                QText {
                    id: footerinfo
                    font.family: "Lato"
                    font.pixelSize: 12
                    text: {
                        var ret = ""
                        if(RoomWalletData.roomWalletReady){
                            if(RoomWalletData.roomWallet.walletM === 0){
                                ret = STR.STR_QML_557
                            }
                            else if(RoomWalletData.roomWallet.walletN === 1 && RoomWalletData.roomWallet.walletM === 1){
                                ret = STR.STR_QML_558
                            }
                            else if(RoomWalletData.roomWallet.walletN > 1 && RoomWalletData.roomWallet.walletM >= 1){
                                ret = STR.STR_QML_559.arg(RoomWalletData.roomWallet.walletM).arg(RoomWalletData.roomWallet.walletN)
                            }
                        }else{
                            ret = STR.STR_QML_557
                        }
                        return ret;
                    }
                    anchors.centerIn: parent
                    color: "#031F2B"
                }
            }
        }
        QButtonTextLink {
            height: 48
            label: STR.STR_QML_397
            fontPixelSize: 16
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_CONFIGURE_SHARED_WALLET_BACK)
            }
        }
        QTextButton {
            id: buttonBottomR
            width: 100
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            enabled: RoomWalletData.roomWallet.walletM > 0
            anchors {
                right: parent.right
                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_CONFIGURE_SHARED_WALLET_REQUEST)
            }
        }
    }
}
