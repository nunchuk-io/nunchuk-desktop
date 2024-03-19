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
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

Item {
    width: 350
    height: _col.childrenRect.height
    property int listHeight: 292
    visible: inWallets.count
    Column {
        id: _col
        anchors.fill: parent
        spacing: 8
        QLato {
            text: STR.STR_QML_128
            font.weight: Font.Bold
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
        Rectangle {
            width: 350
            height: listHeight
            border.color: "#DEDEDE"
            border.width: 1
            color: "transparent"
            radius: 12
            QListView {
                id: inWallets
                anchors.fill: parent
                model: AppModel.walletsUsingSigner
                clip: true
                ScrollBar.vertical: ScrollBar { active: true }
                spacing: 16
                delegate: Item {
                    width: ListView.width
                    height: 40
                    Row {
                        anchors {
                            verticalCenter: parent.verticalCenter
                            left: parent.left
                            leftMargin: 18
                        }
                        spacing: 4
                        QIcon {
                            iconSize: 24
                            source: "qrc:/Images/Images/wallet-dark.svg"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QLato {
                            text: modelData.name
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QBadge {
                            text: qsTr("%1/%2").arg(modelData.walletM).arg(modelData.walletN)
                            color: "#EAEAEA"
                            font.weight: Font.Normal
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
            }
        }
    }
}


