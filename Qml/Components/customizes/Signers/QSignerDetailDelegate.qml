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
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Row {
    id: del
    property string typeStr: ""
    property string tag: ""
    property string signerName: ""
    property string card_id_or_xfp: ""
    property string key_color: ""
    property int    accountIndex: 0
    property int typeInt: -1
    spacing: 12
    QCircleIcon {
        bgSize: 48
        icon.iconSize: 24
        icon.typeStr: typeStr
        icon.type: typeInt
        icon.tag: tag
        anchors.verticalCenter: parent.verticalCenter
        color: key_color
    }
    Column {
        width: 424
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        spacing: 4
        QText {
            width: 146
            height: 20
            text: signerName
            color: "#031F2B"
            font.weight: Font.Normal
            font.family: "Lato"
            font.pixelSize: 16
        }
        Row {
            spacing: 4
            QSignerBadgeName {
                typeStr: del.typeStr
                type: del.typeInt
                tag: del.tag
                color: "#DEDEDE"
                height: 16
                font.weight: Font.Black
                font.pixelSize: 10
            }
            Rectangle {
                width: accttext.width + 10
                height: 16
                color: "#EAEAEA"
                visible: (accountIndex > 0) && (del.typeStr !== "SERVER")
                radius: 8
                QText {
                    id: accttext
                    font.family: "Lato"
                    color: "#031F2B"
                    font.pixelSize: 10
                    anchors.centerIn: parent
                    font.weight: Font.Black
                    text: qsTr("Acct %1").arg(accountIndex)
                }
            }
        }
        QText {
            width: 146
            height: 20
            text: {
                if (typeStr === "NFC" || typeInt === NUNCHUCKTYPE.NFC) {
                    var textR = card_id_or_xfp.substring(card_id_or_xfp.length - 5, card_id_or_xfp.length).toUpperCase()
                    return "Card ID: ••" + textR
                } else {
                    return "XFP: " + card_id_or_xfp.toUpperCase()
                }
            }
            color: "#595959"
            font.weight: Font.Normal
            font.capitalization: Font.AllUppercase
            font.family: "Lato"
            font.pixelSize: 12
        }
    }
}
