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
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../origins"
import "../customizes/Texts"
import "../customizes/Buttons"
import "../customizes/Signers"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    id: signermanagerdlg
    property alias  nameWidth: text.width
    property alias  signername: id_signername.text
    property string  card_id_or_xfp: ""
    property string devicetype: ""
    property int    signerType : 0
    property bool   isPrimaryKey: false
    property string signerTag: ""
    property int    accountIndex: 0
    color: mastersignerMouse.containsMouse ? Qt.rgba(255, 255, 255, 0.1) : "transparent"
    Rectangle {
        width: parent.width
        height: 1
        color: "#C9DEF1"
        opacity: 0.1
        anchors.bottom: parent.bottom
    }
    QSignerLightIcon {
        id: indicator
        iconSize: 24
        device_type: devicetype
        type: signerType
        tag: signerTag
        anchors.left: parent.left
        anchors.leftMargin: 13
        anchors.verticalCenter: parent.verticalCenter
    }
    Column {
        id: text
        anchors.left: indicator.right
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        width: 160
        height: 37
        QText {
            id: id_signername
            width: parent.width
            height: paintedHeight
            font.family: "Montserrat"
            color: "#F1FAFE"
            elide: Text.ElideRight
            font.pixelSize: 14
            wrapMode: Text.WordWrap
            font.weight: Font.DemiBold
            horizontalAlignment: Text.AlignLeft
        }
        QLato {
            id: id_xfp
            width: parent.width
            height: 16
            color: "#F1FAFE"
            text: {
                if (signerTag == "NFC" || signerType === NUNCHUCKTYPE.NFC) {
                    var textR = card_id_or_xfp.substring(card_id_or_xfp.length - 5, card_id_or_xfp.length).toUpperCase()
                    return "Card ID: ••" + textR
                } else {
                    return "XFP: " + card_id_or_xfp.toUpperCase()
                }
            }
            elide: Text.ElideRight
            font.pixelSize: 12
            horizontalAlignment: Text.AlignLeft
            font.capitalization: Font.AllUppercase
        }
    }

    Column {
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        spacing: 4
        Row {
            spacing: 4
            anchors.right: parent.right
            anchors.rightMargin: 10
            height: 16
            Rectangle {
                width: primaryText.width + 16
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                color: "#FDD95C"
                visible: isPrimaryKey
                radius: 20
                QText {
                    id: primaryText
                    text: STR.STR_QML_641
                    font.family: "Lato"
                    font.weight: Font.Bold
                    font.pixelSize: 10
                    anchors.centerIn: parent
                    color: "#031F2B"
                }
            }
            Rectangle {
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height
                width: accText.width + 16
                color: "#EAEAEA"
                radius: 20
                visible: accountIndex > 0
                QText {
                    id: accText
                    text: qsTr("Acct %1").arg(accountIndex)
                    font.family: "Lato"
                    font.weight: Font.Bold
                    font.pixelSize: 10
                    anchors.centerIn: parent
                    color: "#031F2B"
                }
            }
        }
        Item {
            width: 85
            height: 16
            anchors.right: parent.right
            anchors.rightMargin: 10
            Rectangle {
                anchors.right: parent.right
                height: parent.height
                width: typeText.width + 16
                color: "#EAEAEA"
                visible: signerType !== NUNCHUCKTYPE.SERVER
                radius: 20
                QText {
                    id: typeText
                    text: GlobalData.signers(signerType)
                    font.family: "Lato"
                    font.weight: Font.Bold
                    font.pixelSize: 10
                    anchors.centerIn: parent
                    color: "#031F2B"
                }
            }
        }
    }

    signal buttonClicked()
    MouseArea {
        id: mastersignerMouse
        hoverEnabled: true
        anchors.fill: parent
        onClicked: signermanagerdlg.buttonClicked()
    }
}
