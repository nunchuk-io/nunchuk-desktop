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
import Qt.labs.platform 1.1
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    width: _item.width + 2*12
    height: 64
    border.width: 1
    border.color: "#DEDEDE"
    radius: 12
    property string amount: "100000.0"
    property string currency: "sat"
    property string blocktime: "12/16/2022 at 9:32 PM"
    property bool isThis: false
    signal coinAncestryClicked()
    Column {
        width: _item.width
        anchors {
            left: parent.left
            leftMargin: 12
            verticalCenter: parent.verticalCenter
        }
        spacing: 4
        Item {
            id: _item
            width: _row.childrenRect.width
            height: 20
            Row {
                id: _row
                anchors.verticalCenter: parent.verticalCenter
                spacing: 4
                QLato {
                    width: paintedWidth
                    text: qsTr("%1 %2").arg(amount).arg(currency)
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                QBadge {
                    height: 16
                    font.pixelSize: 10
                    font.weight: Font.Bold
                    text: isThis ? STR.STR_QML_1476 : STR.STR_QML_1477
                    border.width: 1
                    border.color: "#EAEAEA"
                    color: "#FFFFFF"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
        Item {
            width: _item.width
            height: _txt.paintedHeight
            QLato {
                id: _txt
                width: _item.width - 12
                text: qsTr("%1").arg(blocktime)
                color: "#595959"
                font.pixelSize: 12
                font.weight: Font.Normal
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                lineHeightMode: Text.FixedHeight
                lineHeight: 16
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            coinAncestryClicked()
        }
    }
}
