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
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    border.color: "#DEDEDE"
    border.width: 1
    color: "transparent"
    radius: 12
    signal notifyEdited()
    readonly property var repeats: [
        // {id: "FIVE_MINUTES",      frequency: STR.STR_QML_1326 },
        {id: "MONTHLY",           frequency: STR.STR_QML_1322 },
        {id: "THREE_MONTHLY",     frequency: STR.STR_QML_1323 },
        {id: "SIX_MONTHLY",       frequency: STR.STR_QML_1324 },
        {id: "YEARLY",            frequency: STR.STR_QML_1325 },
    ]
    Item {
        anchors {
            fill: parent
            margins: 16
        }
        Column {
            anchors.left: parent.left
            height: parent.height
            width: 484
            spacing: 12
            Row {
                height: 18
                width: parent.width
                spacing: 12
                QBadge {
                    height: 16
                    text: GlobalData.getHealthStatusLabel(modelData.lastState)
                    color: GlobalData.getHealthStatusColor(modelData.lastState)
                    font.pixelSize: 10
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            QSignerDetailDelegate {
                id: _signerDetail
                height: 60
                width: parent.width
                typeStr: modelData.keyinfo.type
                tag: modelData.keyinfo.tag
                signerName: modelData.keyinfo.name
                accountIndex: parseInt(modelData.keyinfo.account_index)
                card_id_or_xfp: modelData.keyinfo.tapsigner ? modelData.keyinfo.tapsigner.card_id : modelData.keyinfo.xfp
                key_color: GlobalData.getHealthStatusColor(modelData.lastState)
            }
        }

        Rectangle {
            id: _rect
            color: "#F5F5F5"
            radius: 12
            height: 48
            width: 198
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            visible: modelData.reminder ? true : false
            property var freq: repeats.find(function(e) { if (e.id === modelData.reminder.frequency) return true; else return false })
            Row {
                width: parent.width - 12*2
                height: 24
                spacing: 8
                anchors {
                    left: parent.left
                    leftMargin: 12
                    verticalCenter: parent.verticalCenter
                }
                QImage {
                    width: 24
                    height: 24
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/scheduling-dark.svg"
                }
                QLato {
                    text: _rect.freq.frequency
                    font.pixelSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            QTextLink {
                id: _edit
                width: 29
                height: 20
                text: STR.STR_QML_849
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                onTextClicked: {
                    notifyEdited()
                }
            }
        }
    }

}
