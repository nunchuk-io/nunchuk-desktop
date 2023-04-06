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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: _period
    property int questionSelected: 0
    QOnScreenContentTypeA {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_725
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, EVT.STATE_ID_SCR_ENTER_BACKUP_PASSWORD)
        }
        Column {
            anchors {
                top: parent.top
                topMargin: 84
                left: parent.left
                leftMargin: 36
            }
            width: 539
            spacing: 24
            QText {
                width: 539
                height: 28
                text: {
                    var name = UserWallet.signer.masterSignername
                    var card_id = UserWallet.signer.masterSignerDevice.cardId
                    var textR = card_id.substring(card_id.length - 5,card_id.length).toUpperCase()
                    return STR.STR_QML_726.arg(name).arg(textR)
                }
                color: "#031F2B"
                font.family: "Lato"
                font.pixelSize: 16
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }

            QTextInputBoxTypeB {
                id: backupPassword
                label: STR.STR_QML_727
                boxWidth: 537
                boxHeight: 48
                isValid: true
                isPassword:true
                onTextInputtedChanged: {
                    if(!backupPassword.isValid){
                        backupPassword.isValid = true
                        backupPassword.errorText = ""
                    }
                    backupPassword.showError = false;
                }
            }

        }
        Connections {
            target: UserWallet
            onBackupPasswordErrorAlert: {
                backupPassword.errorText = errormsg
                backupPassword.isValid = false
                backupPassword.showError = true
            }
        }
        Rectangle {
            width: 718
            height: 80
            radius: 8
            color: "#EAEAEA"
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: 120
            }

            Row {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 11
                QImage {
                    width: 30
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/OnlineMode/error_outline_24px_n.png"
                }
                QText {
                    width: 650
                    height: 28
                    anchors.verticalCenter: parent.verticalCenter
                    text: STR.STR_QML_740
                    color: "#031F2B"
                    font.family: "Lato"
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
        onPrevClicked: {
            QMLHandle.sendEvent(EVT.EVT_ENTER_BACKUP_PASSWORD_BACK)
        }

        onNextClicked: {
            QMLHandle.sendEvent(EVT.EVT_INPUT_BACKUP_PASSWORD_REQUEST, backupPassword.textInputted)
        }
    }
}
