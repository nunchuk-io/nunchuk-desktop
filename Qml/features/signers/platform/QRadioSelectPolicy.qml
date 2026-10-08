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
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.0
import "../../../Components/origins"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Texts"

QRadioSelect {
    id: radioRoot
    property string label: ""
    property string subLabel: ""
    property string iconUrl: ""
    isOverlay: true
    content: Component {
        id: radioText
        Row {
            spacing: 12
            Rectangle {
                width: 48
                height: 48
                radius: 48
                anchors.verticalCenter: parent.verticalCenter
                color: "#F5F5F5"
                QIcon {
                    iconSize: 24
                    source: iconUrl
                    anchors.centerIn: parent
                }
            }
            Column {
                spacing: 4
                QLato {
                    text: label
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                QLato {
                    width: 210
                    text: subLabel
                    font.pixelSize: 12
                    font.weight: Font.Normal                    
                    wrapMode: Text.WrapAnywhere
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
            }
        }
    }
}
