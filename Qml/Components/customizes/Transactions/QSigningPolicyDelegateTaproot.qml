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
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

Item {
    width: 350
    height: _column.childrenRect.height
    property alias signerData: dataSingle

    QSingleSignerData {
        id: dataSingle
    }

    Row {
        anchors {
            left: parent.left
            leftMargin: 12
        }
        height: parent.height
        spacing: 12
        Rectangle {
            id: _keyIcon
            width: 48
            height: 48
            radius: width
            color: "#F5F5F5"
            anchors.verticalCenter: parent.verticalCenter
            QSignerDarkIcon {
                iconSize: 24
                anchors.centerIn: parent
                device_type: dataSingle.single_devicetype
                type: dataSingle.single_type
                tag: dataSingle.single_tag
            }
        }
        Column {
            id: _column
            height: childrenRect.height
            width: 350 - 12 - 48 - 12
            anchors.verticalCenter: parent.verticalCenter
            spacing: 4
            QLato {
                width: parent.width
                text: dataSingle.single_name
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Row {
                height: 16
                spacing: 4
                QBadge {
                    text: STR.STR_QML_1695
                    height: 16
                    color: "#D0E2FF"
                    visible: dataSingle.single_value_key
                    radius: 8
                    fontSize: 10
                }
                QSignerBadgeName {
                    typeStr: ""
                    type: dataSingle.single_type
                    tag: dataSingle.single_tag
                    color: "#DEDEDE"
                    height: 16
                    font.pixelSize: 10
                }
                QBadge {
                    text: qsTr("Acct %1").arg(dataSingle.single_account_index)
                    height: 16
                    color: "#EAEAEA"
                    visible: dataSingle.single_account_index > 0
                    radius: 8
                    fontSize: 10
                }
                visible: dataSingle.single_type !== NUNCHUCKTYPE.SERVER
            }
            QLato {
                width: 146
                height: 16
                visible: dataSingle.single_type !== NUNCHUCKTYPE.SERVER
                text: {
                    if (dataSingle.single_type === NUNCHUCKTYPE.NFC) {
                        var card_id = dataSingle.single_device_cardid
                        var textR = card_id.substring(card_id.length - 5, card_id.length).toUpperCase()
                        return "Card ID: ••" + textR
                    }
                    else {
                        return "XFP: " + dataSingle.single_masterFingerPrint.toUpperCase()
                    }
                }
                color: "#031F2B"
                font.weight: Font.Normal
                font.capitalization: Font.AllUppercase
                font.family: "Lato"
                font.pixelSize: 12
            }
        }
    }
}
