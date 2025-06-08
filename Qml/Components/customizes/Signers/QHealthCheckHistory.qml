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
import QtQuick.Controls 2.5
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import HMIEVENTS 1.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

Item {
    width: 350
    property int parentHeight: 452
    height: hasReminder ? (parentHeight - 48) : parentHeight
    property bool hasReminder: false
    property var descriptions: [
        {type: "HEALTH_CHECK",      description: STR.STR_QML_1206 },
        {type: "TRANSACTION",       description: STR.STR_QML_1207 },
        {type: "DUMMY_TRANSACTION", description: STR.STR_QML_1206 },
    ]
    Column {
        anchors.fill: parent
        spacing: 8
        QLato {
            text: STR.STR_QML_1201
            font.weight: Font.Bold
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
        Rectangle {
            width: 350
            height: hasReminder ? (parentHeight - 28 - 48) : (parentHeight - 28)
            border.color: "#DEDEDE"
            border.width: 1
            color: "transparent"
            radius: 12

            Item {
                width: 350
                height: 52
                QLato {
                    anchors.centerIn: parent
                    text: STR.STR_QML_1202
                    font.weight: Font.Bold
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    visible: _history.count === 0
                }
            }
            QListView {
                id: _history
                anchors {
                    fill: parent
                    margins: 12
                }
                model: signerInfo.healthCheckHistory
                clip: true
                ScrollBar.vertical: ScrollBar { active: true }
                spacing: 16
                delegate: Item {
                    width: 326
                    height: 40
                    Column {
                        anchors.fill: parent
                        spacing: 4
                        QLato {
                            text: modelData.created_time_millis
                            width: parent.width
                            height: 20
                            font.weight: Font.Bold
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                        QLato {
                            text: descriptions.find(function(e) {if (e.type === modelData.type) return true; else return false}).description
                            width: parent.width
                            height: 20
                            font.weight: Font.Normal
                            color: "#595959"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            visible: modelData.type === "DUMMY_TRANSACTION" || modelData.type === "HEALTH_CHECK"
                        }
                        QTextLink {
                            width: 56
                            height: 20
                            font.weight: Font.Normal
                            font.pixelSize: 12
                            font.underline: true
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            text: descriptions.find(function(e) {if (e.type === modelData.type) return true; else return false}).description
                            color: "#595959"
                            visible: modelData.type === "TRANSACTION"
                            onTextClicked: {
                                if (modelData.type === "TRANSACTION") {
                                    GroupWallet.dashboardInfo.health.transactionSignedClicked(modelData.payload)
                                }
                            }
                        }
                    }
                }
            }

        }
    }
}


