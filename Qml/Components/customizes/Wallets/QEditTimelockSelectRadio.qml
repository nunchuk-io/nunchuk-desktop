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
import "./../../origins"
import "./../../customizes/Texts"
import "./../../customizes/Buttons"

QRadioSelect {
    id: radioRoot
    width: 313
    spacing: 8
    property string headline: ""
    property string description: ""
    content: Component {
        Column {
            id: contentInfo
            spacing: 4
            QLato {
                width: contentInfo.width
                text: headline
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QLato {
                width: contentInfo.width
                text: description
                font.pixelSize: 12
                font.weight: Font.Normal
                color: "#757575"
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                lineHeight: 16
                lineHeightMode: Text.FixedHeight
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
    }   
}
