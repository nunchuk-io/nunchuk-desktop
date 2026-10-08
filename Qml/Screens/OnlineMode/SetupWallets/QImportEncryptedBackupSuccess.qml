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

QOnScreenContent {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: ""
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Column {
        anchors.fill: parent
        anchors.margins: 36
        spacing: 24
        Rectangle {
            width: 96;height: 96;
            radius: 48
            color: "#A7F0BA"
            QIcon {
                iconSize: 60
                anchors.centerIn: parent
                source: "qrc:/Images/Images/check-dark.svg"
            }
        }
        Column {
            width: parent.width
            spacing: 12
            QMontserrat {
                width: parent.width
                height: 40
                text: STR.STR_QML_913
                font.pixelSize: 32
                font.weight: Font.Medium
                verticalAlignment: Text.AlignVCenter
            }
            QLato {
                width: parent.width
                height: 28
                text: STR.STR_QML_828
                verticalAlignment: Text.AlignVCenter
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
            }
        }
    }
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 120
            height: 48
            label.text: STR.STR_QML_777
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        }
    }
}
