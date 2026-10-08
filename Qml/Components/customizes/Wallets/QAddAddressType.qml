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
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "./../../origins"
import "./../../customizes"
import "./../../customizes/Chats"
import "./../../customizes/Texts"
import "./../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Column {
    id: _addressType
    spacing: 8
    property string walletConfigType: ""
    property bool isEnabled: true
    property var types: {
        var ls = []
        ls.push({badge_txt: STR.STR_QML_1547,   type: NUNCHUCKTYPE.NATIVE_SEGWIT,    displayName: STR.STR_QML_062 })
        ls.push({badge_txt: STR.STR_QML_1548,   type: NUNCHUCKTYPE.TAPROOT,          displayName: STR.STR_QML_553 })
        if (walletOptType !== NUNCHUCKTYPE.E_MINISCRIPT_WALLET) {
            ls.push({badge_txt: "",                 type: NUNCHUCKTYPE.NESTED_SEGWIT,    displayName: STR.STR_QML_063 })
            ls.push({badge_txt: "",                 type: NUNCHUCKTYPE.LEGACY,           displayName: STR.STR_QML_064 })
        }
        return ls
    }

    property int typeOption: NUNCHUCKTYPE.NATIVE_SEGWIT
    property int maxSize: 2
    signal selectTypeOption(var type)
    QLato {
        font.weight: Font.Bold
        font.pixelSize: 12
        text: STR.STR_QML_066
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    Repeater {
        model: maxSize
        QRadioButtonTypeG {
            id: btn
            property var itemData: types[index]
            border.width: 0
            width: _addressType.width
            height: 40
            radius: 8
            label: itemData.displayName
            fontPixelSize: 16
            fontWeight: Font.Normal
            color: btn.selected ? "#66D0E2FF" : "#FFFFFF"
            selected: typeOption === itemData.type
            textBadge: itemData.badge_txt
            fontBadgePixelSize: 10
            opacity: btn.enabled ? 1.0 : (btn.selected ? 1.0 : 0.4)
            enabled: {
                var tmp = isEnabled
                if (itemData.type === NUNCHUCKTYPE.NESTED_SEGWIT || itemData.type === NUNCHUCKTYPE.LEGACY) {
                    tmp = isEnabled && walletConfigType !== "miniscript"
                }
                return tmp
            }
            onButtonClicked: {
                selectTypeOption(itemData.type)
            }
        }
    }
    QButtonTextLink {
        visible: types.length > 2
        width: 67
        height: 16
        anchors.horizontalCenter: parent.horizontalCenter
        label: STR.STR_QML_1549
        icon: maxSize == 2 ? ["qrc:/Images/Images/expand-more-dark.svg", "qrc:/Images/Images/expand-more-dark.svg", "qrc:/Images/Images/expand-more-dark.svg"]
                           : ["qrc:/Images/Images/expand-less-dark.svg", "qrc:/Images/Images/expand-less-dark.svg", "qrc:/Images/Images/expand-less-dark.svg"]
        direction: eRIGHT
        fontPixelSize: 12
        onButtonClicked: {
            if (maxSize == 2) {
                maxSize = types.length
            } else {
                maxSize = 2
            }
        }
    }
}
