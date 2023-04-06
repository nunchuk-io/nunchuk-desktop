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
    id: _question
    QOnScreenContent {
        id: _content
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: ""
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, EVT.STATE_ID_SCR_LOCKDOWN_SUCCESS)
        }
        Item {
            width: 500
            height: 252
            anchors {
                top:parent.top
                topMargin: 144
                horizontalCenter: parent.horizontalCenter
            }
            Column {
                width: parent.width
                spacing: 24
                Rectangle {
                    width: 96;height: 96;
                    radius: 48
                    color: "#A7F0BA"
                    anchors.horizontalCenter: parent.horizontalCenter
                    QImage {
                        anchors.centerIn: parent
                        width: 60; height: 60;
                        source: "qrc:/Images/Images/check-dark.svg"
                    }
                }
                QText {
                    width: parent.width
                    height: 80
                    text: STR.STR_QML_720
                    color: "#031F2B"
                    font.family: "Lato"
                    font.pixelSize: 32
                    font.weight: Font.DemiBold
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 40
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                QText {
                    width: parent.width
                    height: 28
                    text: STR.STR_QML_721.arg(UserWallet.untilTime)
                    color: "#031F2B"
                    font.family: "Lato"
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        QTextButton {
            width: 120
            height: 48
            label.text: STR.STR_QML_341
            label.font.pixelSize: 16
            type: eTypeE
            anchors {
                right: parent.right
                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST)
            }
        }
    }
}
