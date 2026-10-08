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

QScreenAdd {
    anchors.fill: parent
    signal nextClicked()
    QOnScreenContent {
        id:_content
        anchors.fill: parent
        anchors.centerIn: parent
        enableHeader:false
        offset: 24
        onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        QLato {
            anchors{
                top: parent.top
                topMargin: 24
                horizontalCenter: parent.horizontalCenter
            }
            width: popupWidth - _content.offset*2
            height: 20
            text: STR.STR_QML_1535
            font.weight: Font.Bold
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        QLato {
            anchors{
                top: parent.top
                topMargin: 20 + 12 + 24
                horizontalCenter: parent.horizontalCenter
            }
            width: 252
            height: 84
            text: STR.STR_QML_1536
            lineHeightMode: Text.FixedHeight
            lineHeight: 20
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        bottomLeft: Row {
            spacing: 8
            QTextButton {
                width: 120
                height: 48
                label.text: STR.STR_QML_432
                label.font.pixelSize: 16
                type: eTypeF
                onButtonClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
        }
        bottomRight: Row {
            spacing: 12
            QTextButton {
                width: 120
                height: 48
                label.text: STR.STR_QML_433
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    nextClicked()
                }
            }
        }
    }
}
