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
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.3
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: ""
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    Column {
        anchors{
            fill: parent
            topMargin: 36
            leftMargin: 36
            rightMargin: 115
        }
        spacing: 24
        Rectangle {
            width: 96;height: 96;
            radius: 48
            color: "#F5F5F5"
            QIcon {
                iconSize: 60
                anchors.centerIn: parent
                source: "qrc:/Images/Images/save-backup-dark.svg"
            }
        }
        QLato {
            width: parent.width
            height: 40
            text: STR.STR_QML_1007
            font.pixelSize: 32
            font.weight: Font.DemiBold
            verticalAlignment: Text.AlignVCenter
        }
        QLato {
            width: parent.width
            height: 28
            text: STR.STR_QML_1008
            verticalAlignment: Text.AlignVCenter
            lineHeightMode: Text.FixedHeight
            lineHeight: 28
            wrapMode: Text.WordWrap
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    bottomRight: QTextButton {
        width: 71
        height: 48
        label.text: STR.STR_QML_777
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    }
}
