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
import "../../../Components/customizes/Texts"

Rectangle {
    id: root
    width: parent.width
    height: 76
    color: "#FFFFFF"
    radius: 12

    // Property: timezone display (e.g., "Asia/Jakarta (GMT+7:00)")
    property string timezone: "Asia/Jakarta (GMT+7:00)"

    // Signals
    signal editClicked()

    Column {
        anchors {
            fill: parent
            margins: 12
        }
        spacing: 12

        // Title and Edit button
        Item {
            width: parent.width
            height: 20

            QLato {
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
                text: STR.STR_QML_2168
                font.pixelSize: 16
                font.weight: Font.Bold
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }

            QTextLink {
                width: 29
                height: 20
                anchors.right: parent.right
                text: STR.STR_QML_849
                color: "#031F2B"
                anchors.verticalCenter: parent.verticalCenter
                onTextClicked: {
                    root.editClicked()
                }
            }
        }

        // Timezone display
        QLato {
            text: timezone
            font.pixelSize: 16
            color: "#031F2B"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
    }
}
