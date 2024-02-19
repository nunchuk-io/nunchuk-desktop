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
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"

Row {
    id: _id
    property var user
    property var role
    height: 48
    spacing: 12
    QAvatar {
        id: _avatar
        width: 48
        height: 48
        avatarLocal: user ? user.avatar : ""
        avatarUrl: user ? user.avatar : ""
        username: user ? user.name : ""
        displayStatus: false
        anchors.verticalCenter: parent.verticalCenter
    }
    QTextBadgeVerical {
        id: _textBadge
        height: 40
        anchors.verticalCenter: parent.verticalCenter
        role: _id.role
        label: user ? user.name : ""
    }
}
