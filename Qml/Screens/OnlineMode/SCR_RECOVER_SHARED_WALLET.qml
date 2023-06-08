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
import Qt.labs.platform 1.1
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
        label.text: STR.STR_QML_603
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_RECOVER_SHARED_WALLET)
        }
        QImage {
            id: compressButton
            width: 24
            height: 24
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
                    QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_RECOVER_SHARED_WALLET)
                }
            }
        }

        Column {
            width: 460
            spacing: 24
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 144
            }
            Rectangle{
                width: 96
                height: 96
                color: "#F5F5F5"
                radius: 48
                anchors.horizontalCenter: parent.horizontalCenter
                QImage{
                    width: 60
                    height: 60
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/collab-wallet-dark.svg"
                }
            }
            QText{
                width: paintedWidth
                height: paintedHeight
                anchors.horizontalCenter: parent.horizontalCenter
                text:STR.STR_QML_604
                font.pixelSize: 16
                font.family: "Lato"
                font.weight: Font.Normal
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            Row{
                width: parent.width
                height: 24
                spacing: 12
                QImage{
                    width: 24
                    height: 24
                    source: "qrc:/Images/Images/1.Active.svg"
                }
                QText{
                    width: paintedWidth
                    height: paintedHeight
                    anchors.verticalCenter: parent.verticalCenter
                    text:STR.STR_QML_605
                    font.pixelSize: 16
                    font.family: "Lato"
                    font.weight: Font.Normal
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            Row{
                width: parent.width
                height: 24
                spacing: 12
                QImage{
                    width: 24
                    height: 24
                    source: "qrc:/Images/Images/2.Active.svg"
                }
                QText{
                    width: paintedWidth
                    height: paintedHeight
                    anchors.verticalCenter: parent.verticalCenter
                    text:STR.STR_QML_606
                    font.pixelSize: 16
                    font.family: "Lato"
                    font.weight: Font.Normal
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
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
                QMLHandle.sendEvent(EVT.EVT_RECOVER_SHARED_WALLET_BACK)
            }
        }

        QTextButton {
            id: buttonRecoverQR
            width: 205
            height: 48
            label.text: STR.STR_QML_608
            label.font.pixelSize: 16
            type: eTypeB
            anchors {
                right: buttonRecoverBSMS.left
                rightMargin: 14
                bottom: parent.bottom
                bottomMargin: 36
            }

            onButtonClicked: {
                qrscaner.open()
            }
        }

        QTextButton {
            id: buttonRecoverBSMS
            width: 272
            height: 48
            label.text: STR.STR_QML_607
            label.font.pixelSize: 16
            type: eTypeE
            anchors {
                right: parent.right
                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {
                openfileDialog.open()
            }
        }
    }

    Popup {
        id: recover_rename
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        QOnScreenContent {
            id: _contenCenter
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_603
            onCloseClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_RECOVER_SHARED_WALLET)
            }
            QImage {
                id: _compressButton
                width: 24
                height: 24
                anchors {
                    right: parent.right
                    rightMargin: 84
                    top: parent.top
                    topMargin: 44
                }
                transformOrigin: Item.Center
                source: "qrc:/Images/Images/OnlineMode/compress-24px.png"
                scale: _compressmouse.pressed ? 1: _compressmouse.containsMouse ? 1.1 : 1
                MouseArea {
                    id: _compressmouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_RECOVER_SHARED_WALLET)
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
            }
            QButtonTextLink {
                height: 48
                label: STR.STR_QML_397
                fontPixelSize: 16
                displayIcon:true
                anchors {
                    left: parent.left
                    leftMargin: 40
                    bottom: parent.bottom
                    bottomMargin: 40
                }
                onButtonClicked: {
                    recover_rename.close()
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
                    var bsms_import = { "passFile": false,
                                        "filePath" : "" };
                    QMLHandle.sendEvent(EVT.EVT_IMPORT_BSMS_SHARED_WALLET,bsms_import)
                }
            }
        }
    }

    QQrImportScanner {
        id: qrscaner
        onTagFound: {
            if(AppModel.parseKeystoneCollabWallet(qrscaner.tags)){
                qrscaner.close()
                QMLHandle.sendEvent(EVT.EVT_IMPORT_QR_SHARED_WALLET, qrscaner.tags)
            }
        }
    }
    FileDialog {
        id: openfileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            var bsms_import = { "passFile": result,
                                "filePath" : openfileDialog.file };
            QMLHandle.sendEvent(EVT.EVT_IMPORT_BSMS_SHARED_WALLET,bsms_import)
            recover_rename.open()
        }
    }
}
