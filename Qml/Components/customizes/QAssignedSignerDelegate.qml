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
import "../origins"
import "../../../localization/STR_QML.js" as STR

Row {
    width: 216
    height: 56
    spacing: 8
    property string signerName: "value"
    property string signerXFP: "value"
    property string lastHealthCheck: "value"
    property int    signerType: 0
    Rectangle {
        width: 4
        height: parent.height - 2
        color: "#C9DEF1"
        radius: 2
    }
    Column {
        spacing: 4
        Item {
            width: 204
            height: 21
            QText {
                id: signernametext
                width: 100
                height: 21
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 14
                color: "#031F2B"
                text: signerName
                elide: Text.ElideRight
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle {
                width: signerTypeText.width + 10
                height: signerTypeText.height + 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                color: "#C9DEF1"
//                visible: (signerType === NUNCHUCKTYPE.AIRGAP)
                radius: 20
                QText {
                    id: signerTypeText
                    text: GlobalData.signers(signerType)
                    font.family: "Lato"
                    font.weight: Font.Bold
                    font.pixelSize: 10
                    anchors.centerIn: parent
                    color: "#031F2B"
                }
            }
        }
        QText {
            width: 204
            height: 16
            font.family: "Lato"
            font.pixelSize: 12
            color: "#031F2B"
            text: "XFP: " + signerXFP
            font.capitalization: Font.AllUppercase
        }
        QText {
            width: 204
            height: 10
            font.family: "Lato"
            font.pixelSize: 10
            color: "#839096"
            text: qsTr("%1: %2").arg(STR.STR_QML_455).arg(lastHealthCheck)
        }
    }
}
