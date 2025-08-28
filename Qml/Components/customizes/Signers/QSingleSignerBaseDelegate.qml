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
    property bool hasEditPath: false
    property alias signerData: dataSingle
    signal btnClicked()
    signal bip32PathClick(var xfp, var path)
    QSingleSignerData {
        id: dataSingle
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked:{
            btnClicked()
        }
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
            icon.type: dataSingle.single_type
            icon.tag: dataSingle.single_tag
            anchors.verticalCenter: parent.verticalCenter
            color: "#F5F5F5"
            visible: dataSingle.single_is_local
        }
        QBadge {
            width: 48
            height: 48
            iconSize: 24
            radius: 48
            icon: "qrc:/Images/Images/Device_Icons/key-dark.svg"
            anchors.verticalCenter: parent.verticalCenter
            color: "#F5F5F5"
            visible: !dataSingle.single_is_local
        }
        Column {
            width: 302 - 48 - 12
            height: childrenRect.height
            anchors.verticalCenter: parent.verticalCenter
            spacing: 4
            QLato {
                width: 146
                height: 28
                text: dataSingle.single_name
                font.weight: Font.Normal
            }
            Row {
                height: 16
                spacing: 4
                QSignerBadgeName {
                    typeStr: ""
                    type: dataSingle.single_type
                    tag: dataSingle.single_tag
                    color: "#DEDEDE"
                    height: 16
                    font.weight: Font.Bold
                    font.pixelSize: 10
                }
                QBadge {
                    text: qsTr("Acct %1").arg(dataSingle.single_account_index)
                    height: 16
                    color: "#EAEAEA"
                    visible: (dataSingle.single_account_index > 0) && (dataSingle.single_type !== NUNCHUCKTYPE.SERVER)
                    radius: 8
                    font.pixelSize: 10
                }
            }
            Row {
                QLato {
                    width: 146
                    height: 16
                    text: {
                        if (dataSingle.single_type === NUNCHUCKTYPE.NFC) {
                            var card_id = dataSingle.single_device_cardid
                            var textR = card_id.substring(card_id.length - 5, card_id.length).toUpperCase()
                            return "Card ID: ••" + textR
                        } else {
                            return "XFP: " + dataSingle.single_masterFingerPrint.toUpperCase()
                        }
                    }
                    color: "#595959"
                    font.weight: Font.Normal
                    font.capitalization: Font.AllUppercase
                    font.family: "Lato"
                    font.pixelSize: 12
                }
            }
            QLato {
                visible: hasEditPath
                width: paintedWidth + 4 + 12
                text: qsTr("BIP32 path: %1").arg(dataSingle.single_derivationPath)
                color: "#757575"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 12
                font.underline: _icon.visible
                QIcon {
                    id: _icon
                    iconSize: 12
                    source: "qrc:/Images/Images/editBIP32.svg"
                    anchors {
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                    }
                    visible: dataSingle.single_type === NUNCHUCKTYPE.SOFTWARE ||
                             dataSingle.single_type === NUNCHUCKTYPE.HARDWARE
                }
                MouseArea {
                    visible: _icon.visible
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        bip32PathClick(dataSingle.single_masterFingerPrint, dataSingle.single_derivationPath)
                    }
                }
            }
        }
        
    }
}
