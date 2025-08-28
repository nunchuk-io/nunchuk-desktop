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
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Rectangle {
    width: 352
    height: 396
    radius: 16
    color: "#F5F5F5"
    readonly property var types: [
        {type: NUNCHUCKTYPE.NATIVE_SEGWIT,   displayName: STR.STR_QML_062 },
        {type: NUNCHUCKTYPE.TAPROOT,         displayName: STR.STR_QML_553 },
        {type: NUNCHUCKTYPE.NESTED_SEGWIT,   displayName: STR.STR_QML_063 },
        {type: NUNCHUCKTYPE.LEGACY,          displayName: STR.STR_QML_064 },
    ]
    property var infos: [
        {label: STR.STR_QML_025,  labelValue: newWalletInfo.walletName },
        {label: STR.STR_QML_1639, labelValue: STR.STR_QML_1801 },
        {label: STR.STR_QML_066,  labelValue: types.find(function(e) {return e.type === newWalletInfo.walletAddressType }).displayName },
    ]
    Column {
        width: 304
        spacing: 12
        anchors {
            left: parent.left
            leftMargin: 24
            top: parent.top
            topMargin: 16
        }
        Repeater {
            model: infos
            Item {
                width: parent.width
                height: 28
                QLato {
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }
                    text: modelData.label
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }

                QLato {
                    anchors {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                    text: modelData.labelValue
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
