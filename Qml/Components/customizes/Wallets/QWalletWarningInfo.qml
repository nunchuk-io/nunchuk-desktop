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

Rectangle {
    width: 846
    height: 60
    radius: 8
    color: "#FDEBD2"

    property bool   forceClose  : false
    property alias  content     : textContent.text
    signal closeWarning()
    signal hyperlinkClicked()

    Row {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8
        QIcon {
            iconSize: 36
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/Images/Images/warning_amber-60px.png"
        }
        QLato {
            id: textContent
            width: parent.width - 76
            height: parent.height
            font.pixelSize: 16
            textFormat: Text.RichText
            color: "#031F2B"
            anchors.verticalCenter: parent.verticalCenter
            onLinkActivated: {
                hyperlinkClicked()
            }
        }
        QIcon {
            iconSize: 24
            anchors.verticalCenter: parent.verticalCenter
            source: "qrc:/Images/Images/close_24px.png"
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    forceClose = true
                    closeWarning()
                }
            }
        }
    }
}
