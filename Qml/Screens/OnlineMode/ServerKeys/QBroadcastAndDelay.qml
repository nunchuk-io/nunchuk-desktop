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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Members"
import "../../../../localization/STR_QML.js" as STR

Column {
    width: 539
    height: 161
    spacing: 16
    property var serverKeyInfo
    property var broadcastDelay: serverKeyInfo.broadcastDelay
    QLato {
        width: parent.width
        height: 20
        text: STR.STR_QML_792
        font.weight: Font.DemiBold
        verticalAlignment: Text.AlignVCenter
    }
    Rectangle {
        color: "#F5F5F5"
        radius: 12
        width: 539
        height: 125
        Column {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 16
            Item {
                width: 507
                height: 40
                QLato {
                    width: 247
                    height: 40
                    text: STR.STR_QML_794
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                    }
                }
                QLato {
                    width: 247
                    height: 20
                    text: broadcastDelay.auto_broadcast_transaction ? "On" : "Off"
                    color: broadcastDelay.auto_broadcast_transaction_isChanged? "#CF4018": "#031F2B"
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    anchors {
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                    }
                }
            }
            QLine{
                width: 507
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Item {
                width: 507
                height: 20
                QLato {
                    width: 247
                    height: 20
                    text: STR.STR_QML_795
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                    }
                }
                QLato {
                    width: 247
                    height: 20
                    text: broadcastDelay.signing_delay_seconds == 0 ? "Off" : serverKeyInfo.secondsToString(broadcastDelay.signing_delay_seconds)
                    color: broadcastDelay.signing_delay_seconds_isChanged? "#CF4018": "#031F2B"
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    anchors {
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                    }
                }
            }
        }
    }
}


