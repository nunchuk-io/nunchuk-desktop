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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR


QOnScreenContentTypeB {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_106
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    signal keyTypeSelected(var hardware)
    property string hardwareTag: ""
    content: Item {
        Column {
            anchors.top: parent.top
            anchors.topMargin: -16
            QLato {
                width: parent.width
                height: 56
                text: STR.STR_QML_1285
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }

            Column {
                spacing: 0
                Repeater {
                    model: {
                        var ls = []
                        ls.push({add_type: NUNCHUCKTYPE.ADD_JADE,     txt: "Blockstream Jade"           , type: "jade"    , tag: "JADE"    })
                        ls.push({add_type: NUNCHUCKTYPE.ADD_COLDCARD, txt: "COLDCARD"                   , type: "coldcard", tag: "COLDCARD"})
                        ls.push({add_type: NUNCHUCKTYPE.ADD_PASSPORT, txt: "Foundation Passport"        , type: "passport", tag: "PASSPORT"})
                        ls.push({add_type: NUNCHUCKTYPE.ADD_SEEDSIGNER, txt: "Generic air-gapped device", type: "", tag: ""})
                        ls.push({add_type: NUNCHUCKTYPE.ADD_KEYSTONE, txt: "Keystone"                   , type: "keystone", tag: "KEYSTONE"})
                        return ls
                    }
                    QRadioButtonTypeA {
                        id: btn
                        width: 728
                        height: 48
                        label: modelData.txt
                        layoutDirection: Qt.LeftToRight
                        fontFamily: "Lato"
                        fontPixelSize: 16
                        fontWeight: Font.Normal
                        selected: hardwareTag === modelData.tag
                        onButtonClicked: {
                            hardwareTag = modelData.tag
                        }
                    }
                }
            }
        }
    }
    nextEnable: true
    onPrevClicked:{ closeClicked() }
    onNextClicked: {
        keyTypeSelected(hardwareTag)
    }
}
