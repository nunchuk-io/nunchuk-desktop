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
        label.text: STR.STR_QML_357
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CREATE_SHARED_WALLET)
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
                    QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CREATE_SHARED_WALLET)
                }
            }
        }

        Column {
            spacing: 24
            anchors.left: parent.left
            anchors.leftMargin: 36
            anchors.top: parent.top
            anchors.topMargin: 124
            QTextInputBoxTypeB {
                id: walletname
                label: STR.STR_QML_025
                boxWidth: 540
                boxHeight: 48
                isValid: true
                enableLengthLimit: true
                maxLength: 20
                textInputted: RoomWalletData.roomWalletReady ? RoomWalletData.roomWallet.walletName : ""
                onTextInputtedChanged: { if(RoomWalletData.roomWalletReady) RoomWalletData.roomWallet.walletName = textInputted }
            }
            QTextInputBoxTypeB {
                id: walletdescription
                label: STR.STR_QML_026
                boxWidth: 540
                boxHeight: 48
                isValid: true
                enableLengthLimit: true
                maxLength: 40
                visible: false
                textInputted: RoomWalletData.roomWalletReady ? RoomWalletData.roomWallet.walletDescription : ""
                onTextInputtedChanged: { if(RoomWalletData.roomWalletReady) RoomWalletData.roomWallet.walletDescription = textInputted }
            }
            Rectangle {
                width: 540
                height: 1
                color: "#EAEAEA"
            }

            Item {
                id: addressTypeItem
                implicitWidth: 540
                implicitHeight: childrenRect.height
                Column {
                    id: addressheader
                    readonly property int eDEFAUT: 3
                    spacing: 16
                    Item {
                        implicitWidth: 540
                        implicitHeight: childrenRect.height
                        Column {
                            id: infoaddresstype
                            spacing: 4
                            anchors.left: parent.left
                            QText {
                                text: STR.STR_QML_358
                                color: "#031F2B"
                                font.weight: Font.Bold
                                font.pixelSize: 16
                                font.family: "Lato"
                            }
                            QText {
                                text: STR.STR_QML_359
                                color: "#031F2B"
                                font.weight: Font.Normal
                                font.pixelSize: 12
                                font.family: "Lato"
                            }
                        }
                        QSwitchTypeB {
                            id: addressTypeSwitcher
                            width: 84
                            height: 48
                            anchors.verticalCenter: infoaddresstype.verticalCenter
                            anchors.right: parent.right
                        }
                    }
                    Item {
                        id: expandAddress
                        implicitWidth: 540
                        implicitHeight: childrenRect.height
                        visible: addressTypeSwitcher.switchOn
                        focus: true

                        Row {
                            id: walletTypesRow
                            spacing: 24
                            QRadioButtonTypeC {
                                isDefault: true
                                selected: RoomWalletData.typeSeleted === NUNCHUCKTYPE.NATIVE_SEGWIT
                                labelTop: STR.STR_QML_062
                                labelBottom: STR.STR_QML_360
                                anchors.verticalCenter: parent.verticalCenter
                                onButtonClicked: {if(RoomWalletData.roomWalletReady) RoomWalletData.roomWallet.walletAddressType  = NUNCHUCKTYPE.NATIVE_SEGWIT}
                            }
                            QRadioButtonTypeC {
                                selected: RoomWalletData.typeSeleted === NUNCHUCKTYPE.NESTED_SEGWIT
                                labelTop: STR.STR_QML_063
                                labelBottom: STR.STR_QML_361
                                anchors.verticalCenter: parent.verticalCenter
                                onButtonClicked: {if(RoomWalletData.roomWalletReady) RoomWalletData.roomWallet.walletAddressType  = NUNCHUCKTYPE.NESTED_SEGWIT}
                            }
                            QRadioButtonTypeC {
                                selected: RoomWalletData.typeSeleted === NUNCHUCKTYPE.LEGACY
                                labelTop: STR.STR_QML_064
                                labelBottom: STR.STR_QML_362
                                anchors.verticalCenter: parent.verticalCenter
                                onButtonClicked: {if(RoomWalletData.roomWalletReady) RoomWalletData.roomWallet.walletAddressType  = NUNCHUCKTYPE.LEGACY}
                            }
                        }
                    }
                }
            }
            Item {
                id: walletTypeItem
                readonly property int eDEFAUT: 3
                implicitWidth: 540
                implicitHeight: childrenRect.height
                Column {
                    id: walletTypeheader
                    spacing: 16
                    Item {
                        implicitWidth: 540
                        implicitHeight: childrenRect.height
                        Column {
                            id: infowalletType
                            spacing: 4
                            anchors.left: parent.left
                            QText {
                                text: STR.STR_QML_363
                                color: "#031F2B"
                                font.weight: Font.Bold
                                font.pixelSize: 16
                                font.family: "Lato"
                            }
                            QText {
                                text: STR.STR_QML_364
                                color: "#031F2B"
                                font.weight: Font.Normal
                                font.pixelSize: 12
                                font.family: "Lato"
                            }
                        }
                        QSwitchTypeB {
                            id: walletTypeSwitcher
                            width: 84
                            height: 48
                            anchors.verticalCenter: infowalletType.verticalCenter
                            anchors.right: parent.right
                        }
                    }
                    Item {
                        id: expandWallet
                        implicitWidth: 540
                        implicitHeight: childrenRect.height
                        visible: walletTypeSwitcher.switchOn
                        focus: true
                        property bool isEscrowWallet: RoomWalletData.roomWalletReady ? RoomWalletData.roomWallet.walletEscrow : false
                        Row {
                            spacing: 24
                            QRadioButtonTypeC {
                                isDefault: true
                                selected: !expandWallet.isEscrowWallet
                                labelTop: STR.STR_QML_028
                                labelBottom: ""
                                anchors.verticalCenter: parent.verticalCenter
                                onButtonClicked: {if(RoomWalletData.roomWalletReady ) RoomWalletData.roomWallet.walletEscrow  = false}
                            }
                            QRadioButtonTypeC {
                                selected: expandWallet.isEscrowWallet
                                labelTop: STR.STR_QML_029
                                labelBottom: STR.STR_QML_365
                                anchors.verticalCenter: parent.verticalCenter
                                onButtonClicked: {if(RoomWalletData.roomWalletReady ) RoomWalletData.roomWallet.walletEscrow  = true}
                            }
                        }
                    }
                }
            }
        }
        QButtonTextLink {
            height: 48
            displayIcon: false
            label: STR.STR_QML_035
            fontPixelSize: 16
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_CREATE_SHARED_WALLET_BACK)
            }
        }

        QTextButton {
            id: buttonRecover
            width: 240
            height: 48
            label.text: STR.STR_QML_603
            label.font.pixelSize: 16
            type: eTypeB
            anchors {
                right: buttonBottomR.left
                rightMargin: 14
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_RECOVER_SHARED_WALLET_REQUEST)
            }
        }

        QTextButton {
            id: buttonBottomR
            width: 100
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            enabled: walletname.textInputted !== ""
            anchors {
                right: parent.right
                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_CREATE_SHARED_WALLET_REQUEST)
            }
        }
    }
}
