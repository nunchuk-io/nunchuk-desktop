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
import QtQuick 2.0
import "../../../customizes"
import "../../../customizes/Texts"
import "../../../customizes/Members"
import "../../../../../localization/STR_QML.js" as STR

Item {
    property var member
    property int roleHeight: 24
    height: Math.max(_col.implicitHeight, 48)
    QAvatar {
        width: 48
        height: 48
        avatarLocal: member.user ? member.user.avatar : ""
        avatarUrl: member.user ? member.user.avatar : ""
        username: member.user ? member.user.name : ""
        displayStatus: false
        activeStatus: member.status === "PENDING" ? false : true
        anchors.verticalCenter: parent.verticalCenter
        color: member.status === "PENDING" ? "#EAEAEA" : "#FDD95C"
    }
    Column {
        id: _col
        anchors {
            left: parent.left
            leftMargin: 48 + 12
            verticalCenter: parent.verticalCenter
        }
        spacing: 4
        QLato {
            height: 20
            text: member.user ? member.user.name : member.email_or_username
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        QMemberRole{
            height: roleHeight
            role: member.role
            font.weight: Font.Normal
            font.pixelSize: 12
        }
        QLato {
            height: 16
            font.pixelSize: 12
            color: "#595959"
            text: member.user ? member.user.email : member.email_or_username
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
    QLato {
        anchors {
            right: parent.right
            rightMargin: 0
            verticalCenter: parent.verticalCenter
        }
        height: 16
        font.pixelSize: 12
        text: member.status === "PENDING" ? STR.STR_QML_469 : ""
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
