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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
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

QBoxShadow {
    id: _item
    anchors.left: parent.left
    width: 550
    height: 68
    color: "#F5F5F5"
    property string description: ""
    property string desImg: ""
    radius: 12
    Row {
        anchors {
            left: parent.left
            leftMargin: 16
            verticalCenter: parent.verticalCenter
        }
        height: 44
        spacing: 16
        QIcon {
            iconSize: 44
            source: desImg
            anchors.verticalCenter: parent.verticalCenter
        }
        QLato {
            width: _item.width - 56 - 76
            height: 40
            text: description
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            lineHeightMode: Text.FixedHeight
            lineHeight: 20
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    QIcon {
        iconSize: 24
        source: "qrc:/Images/Images/right-arrow-dark.svg"
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: 16
        }
    }
}
