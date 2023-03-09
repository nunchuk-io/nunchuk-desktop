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
import QtQuick 2.12
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../../localization/STR_QML.js" as STR
QScreen {
    QOnScreenContent {
        id:_content
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_239
        enableHeader: false
        QImage {
            width: 120
            height: 120
            anchors{
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 150
            }
            source: "qrc:/Images/Images/logo-light.svg"
        }
        Item {
            width: 448
            height: 262
            anchors.centerIn: parent
            QText {
                id: title
                width: 312
                height: 72
                text: STR.STR_QML_237
                wrapMode: Text.WordWrap
                font.family: "Montserrat"
                font.pixelSize: 24
                font.weight: Font.DemiBold
                color: "#031F2B"
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 40
            }
            QTextInputBox {
                id: passInputted
                width: 248
                heightMin: 56
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: title.bottom
                anchors.topMargin: 24
                border.color: "#C9DEF1"
                placeholder.text: STR.STR_QML_162
                echoMode: TextInput.Password
                Keys.onEnterPressed: {
                    if(passInputted.textOutput !== ""){
                        QMLHandle.sendEvent(EVT.EVT_LOGIN_PERFORM_PASSWORD_REQUEST, passInputted.textOutput)
                    }
                }
            }
            QTextButton {
                id: changepassbtn
                width: 248
                height: 48
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: passInputted.bottom
                anchors.topMargin: 25
                label.text: STR.STR_QML_239
                label.font.pixelSize: 14
                type: eTypeA
                enabled: (passInputted.textOutput !== "") && (passInputted.length >= 8)
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_LOGIN_PERFORM_PASSWORD_REQUEST, passInputted.textOutput)
                }
            }
        }
    }
}
