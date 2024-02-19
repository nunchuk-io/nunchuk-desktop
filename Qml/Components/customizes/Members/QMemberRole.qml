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
import "../../../Components/customizes/Texts"
import "../../../../localization/STR_QML.js" as STR

QBadge {
    property string role: ""
    property var roles: {
        "MASTER": STR.STR_QML_949,
        "ADMIN": STR.STR_QML_950,
        "KEYHOLDER": STR.STR_QML_951,
        "KEYHOLDER_LIMITED": STR.STR_QML_952,
        "OBSERVER": STR.STR_QML_953,
        "CUSTOMIZE": STR.STR_QML_849,
    }
    text: roles[role]
    color: "#EAEAEA"
}
