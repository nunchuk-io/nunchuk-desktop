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
        label.text: STR.STR_QML_434
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_REVIEW_SHARED_WALLET)
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
            source: "qrc:/Images/Images/OnlineMode/compress-24px.png"
            scale: compressmouse.pressed ? 1: compressmouse.containsMouse ? 1.1 : 1
            MouseArea {
                id: compressmouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_REVIEW_SHARED_WALLET)
                }
            }
        }

        Column {
            width: 540
            spacing: 24
            anchors {
                left: parent.left
                leftMargin: 36
                top: parent.top
                topMargin: 100
            }
            Item {
                width: parent.width
                height: childrenRect.height
                QText {
                    anchors.left: parent.left
                    text: STR.STR_QML_025
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                }
                QText {
                    anchors.right: parent.right
                    text: RoomWalletData.walletName
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignRight
                    color: "#031F2B"
                }
            }
            Item {
                width: parent.width
                height: childrenRect.height
                QText {
                    anchors.left: parent.left
                    text: STR.STR_QML_022
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                }
                QText {
                    anchors.right: parent.right
                    text: RoomWalletData.multisigConfiguration
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignRight
                    color: "#031F2B"
                }
            }
            Rectangle {
                width: parent.width
                height: 2
                color: "#EAEAEA"
            }
            Item {
                width: parent.width
                height: childrenRect.height
                QText {
                    anchors.left: parent.left
                    text: STR.STR_QML_027
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                }
                QText {
                    anchors.right: parent.right
                    text: RoomWalletData.walletType
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignRight
                    color: "#031F2B"
                }
            }
            Item {
                width: parent.width
                height: childrenRect.height
                QText {
                    anchors.left: parent.left
                    text: STR.STR_QML_436
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                }
                QText {
                    anchors.right: parent.right
                    text: RoomWalletData.addressType
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignRight
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
                QMLHandle.sendEvent(EVT.EVT_REVIEW_SHARED_WALLET_BACK)
            }
        }
        QTextButton {
            id: buttonBottomR
            width: 100
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            anchors {
                right: parent.right
                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER)
            }
        }
    }
}
