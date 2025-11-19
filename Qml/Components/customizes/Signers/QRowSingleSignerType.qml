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

Row {

    property bool isPrimaryKey: false
    property int  signerType: 0
    property int  accountIndex: 0
    property bool accountVisible: true
    spacing: 4
    Rectangle {
        width: signerPrimaryText.width + 16
        height: 16
        color: "#FDD95C"
        visible: isPrimaryKey
        radius: 20
        anchors.verticalCenter: parent.verticalCenter
        QText {
            id: signerPrimaryText
            text: STR.STR_QML_641
            font.family: "Lato"
            font.weight: Font.Bold
            font.pixelSize: 10
            anchors.centerIn: parent
            color: "#031F2B"
        }
    }
    Rectangle {
        width: signerTypeText.implicitWidth + 16
        height: signerTypeText.implicitHeight + 8
        color: "#EAEAEA"
        radius: 20
        visible: signerType !== NUNCHUCKTYPE.SERVER
        anchors.verticalCenter: parent.verticalCenter
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
    Rectangle {
        width: accttext.implicitWidth + 16
        height: accttext.implicitHeight + 8
        color: "#EAEAEA"
        visible: (signerType !== NUNCHUCKTYPE.SERVER) && accountVisible
        anchors.verticalCenter: parent.verticalCenter
        radius: 8
        QText {
            id: accttext
            font.family: "Lato"
            color: "#031F2B"
            font.pixelSize: 10
            anchors.centerIn: parent
            font.weight: Font.Bold
            text: qsTr("Acct %1").arg(accountIndex)
        }
    }
}
