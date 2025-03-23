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

Item {
    id: del
    width: 350
    height: 84
    property alias signerData: data
    property bool  isShowCheckBox: true
    property bool  checkedProperty: data.single_checked
    signal btnClicked()
    QSingleSignerData {
        id: data
    }
    Row {
        anchors {
            left: parent.left
            leftMargin: 12
        }

        height: parent.height
        spacing: 12
        QCircleIcon {
            bgSize: 48
            icon.iconSize: 24
            icon.typeStr: ""
            icon.type: data.single_type
            icon.tag: data.single_tag
            anchors.verticalCenter: parent.verticalCenter
            color: "#F5F5F5"
            visible: data.single_is_local
        }
        QBadge {
            width: 48
            height: 48
            iconSize: 24
            radius: 48
            icon: "qrc:/Images/Images/Device_Icons/key-dark.svg"
            anchors.verticalCenter: parent.verticalCenter
            color: "#F5F5F5"
            visible: !data.single_is_local
        }
        Column {
            width: 302 - 48 - 12
            height: childrenRect.height
            anchors.verticalCenter: parent.verticalCenter
            spacing: 4
            QLato {
                width: 146
                height: 28
                text: data.single_name
                font.weight: Font.Normal
            }
            Row {
                height: 16
                spacing: 4
                QSignerBadgeName {
                    typeStr: ""
                    type: data.single_type
                    tag: data.single_tag
                    color: "#DEDEDE"
                    height: 16
                    font.weight: Font.Black
                    font.pixelSize: 10
                }
                QBadge {
                    text: qsTr("Acct %1").arg(data.single_account_index)
                    height: 16
                    color: "#EAEAEA"
                    visible: (data.single_account_index > 0) && (data.single_type !== NUNCHUCKTYPE.SERVER)
                    radius: 8
                }
            }
            QLato {
                width: 146
                height: 16
                text: {
                    if (data.single_type === NUNCHUCKTYPE.NFC) {
                        var card_id = data.single_device_cardid
                        var textR = card_id.substring(card_id.length - 5, card_id.length).toUpperCase()
                        return "Card ID: ••" + textR
                    } else {
                        return "XFP: " + data.single_masterFingerPrint.toUpperCase()
                    }
                }
                color: "#595959"
                font.weight: Font.Normal
                font.capitalization: Font.AllUppercase
                font.family: "Lato"
                font.pixelSize: 12
            }
        }
        QIcon {
            iconSize: 24
            source: checkedProperty ? "qrc:/Images/Images/SignerChecked.svg" : "qrc:/Images/Images/SignerUnChecked.svg"
            anchors.verticalCenter: parent.verticalCenter
            visible: isShowCheckBox
        }
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked:{
            btnClicked()
        }
    }
}
