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
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle{
    width: idRow.childrenRect.width + 8*2
    height: 16
    radius: 20
    color: "#EAEAEA"
    property alias icon: idIcon
    property alias label: idLabel
    Row {
        id: idRow
        anchors.centerIn: parent
        height: parent.height
        spacing: 4
        QIcon {
            id: idIcon
            iconSize: 12
            source: "qrc:/Images/Images/collab-wallet-dark.svg"
            anchors.verticalCenter: parent.verticalCenter
            visible: idIcon.source != ""
        }
        QLato {
            id: idLabel
            font.pixelSize: 10
            text: STR.STR_QML_438
            font.weight: Font.Bold
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
