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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR

QScreen {
    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_657
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_REPLACE_PRIMARY_KEY)
        }
        Column {
            id:_colum
            spacing: 24
            anchors.centerIn: parent
            QImage {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 96
                height: 96
                source: "qrc:/Images/Images/account-circle-dark.svg"
            }

            QText {
                width: 500
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "Lato"
                font.pixelSize: 16
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                color: "#031F2B"
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: STR.STR_QML_658
            }
            Item {
                width: 536
                height: _1.paintedHeight
                anchors.horizontalCenter: parent.horizontalCenter
                QImage {
                    anchors.left: parent.left
                    width: 24
                    height: 24
                    source: "qrc:/Images/Images/1.Active.svg"
                }
                QText {
                    id:_1
                    anchors.right: parent.right
                    width: 500
                    font.family: "Lato"
                    font.pixelSize: 16
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    color: "#031F2B"
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    text: STR.STR_QML_659
                }
            }
            Item {
                width: 536
                height: _2.paintedHeight
                anchors.horizontalCenter: parent.horizontalCenter
                QImage {
                    anchors.left: parent.left
                    width: 24
                    height: 24
                    source: "qrc:/Images/Images/2.Active.svg"
                }
                QText {
                    id:_2
                    anchors.right: parent.right
                    width: 500
                    font.family: "Lato"
                    font.pixelSize: 16
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    color: "#031F2B"
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    text: STR.STR_QML_660
                }
            }
        }

        QButtonTextLink {
            height: 24
            label: STR.STR_QML_245
            displayIcon:false
            anchors {
                left: parent.left
                leftMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_REPLACE_PRIMARY_KEY)
            }
        }

        QTextButton {
            width: 200
            height: 48
            label.text: STR.STR_QML_265
            label.font.pixelSize: 16
            type: eTypeE
            anchors {
                right: parent.right
                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {
                if(AppModel.primaryKey.needPassphraseSent){
                    var passPhraseData = {
                        "state_id":EVT.STATE_ID_SCR_REPLACE_PRIMARY_KEY,
                        "mastersigner_id":AppModel.primaryKey.masterSignerId,
                    }
                    QMLHandle.sendEvent(EVT.EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
                }
                else{
                    QMLHandle.sendEvent(EVT.EVT_REPLACE_PRIMARY_KEY_REQUEST,true)
                }
            }
        }
    }
}
