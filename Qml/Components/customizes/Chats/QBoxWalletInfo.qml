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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Chats"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: sharedwalletInfo
    width: 180
    height: 90
    Rectangle {
        id: bgSharedWallet
        width: parent.width - 4
        height: parent.height - 4
        anchors.centerIn: parent
        color: "#FFFFFF"
        visible: false
        radius: 8
    }
    DropShadow {
        anchors.fill: bgSharedWallet
        verticalOffset: 2
        cached: true
        radius: 8
        samples: 16
        color: Qt.rgba(0, 0, 0, 0.15)
        source: bgSharedWallet
        Row {
            anchors.fill: parent
            anchors.margins: 8
            spacing: 8
            QIcon {
                id: icoSharedWalletInfo
                iconSize: 18
                source: "qrc:/Images/Images/collab-wallet-dark.svg"
            }
            Column {
                spacing: 8
                Rectangle {
                    height: 24
                    width: currentState.width + 20
                    radius: 20
                    color: RoomWalletData.roomWalletCreated ? "#A7F0BA" : RoomWalletData.fullSigners ? "#FDEBD2" : "#FFD7D9"
                    QText {
                        id: currentState
                        font.family: "Lato"
                        font.pixelSize: 10
                        text: RoomWalletData.roomWalletCreated ? STR.STR_QML_376 : RoomWalletData.fullSigners ? STR.STR_QML_377 : STR.STR_QML_378
                        anchors.centerIn: parent
                        color: "#031F2B"
                        font.weight: Font.Bold
                    }
                }
                QText {
                    id: configinfo
                    font.family: "Lato"
                    font.pixelSize: 12
                    text: RoomWalletData.multisigConfiguration + " " + RoomWalletData.walletType
                    color: "#031F2B"
                }
                QText {
                    id: sharedwlname
                    font.family: "Lato"
                    font.pixelSize: 12
                    text: RoomWalletData.walletName
                    color: "#031F2B"
                    font.weight: Font.Bold
                }
            }
        }
    }
    MouseArea {
        id: sharedWalletInfoMouse
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        enabled: sharedwalletInfo.visible
        onClicked: {
            if(RoomWalletData.roomWalletReady){
                QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WALLET_CONFIGURE, RoomWalletData.currentRoom.roomid)
            }
        }
    }
}

