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
import "../../origins"

Row {
    spacing: 4
    property var accountIndexs: []
    property string walletType: ""
    Repeater {
        model: accountIndexs
        Rectangle {
            width: accttext.width + 10
            height: 16
            color: "#EAEAEA"
            visible: {
                if (walletType === "MULTI_SIG") {
                    return (modelData > 0)
                } else {
                    return true
                }
            }
            radius: 8
            opacity: modelData === "X" || modelData === "Y" ? 0.5 : 1.0
            QLato {
                id: accttext
                font.pixelSize: 10
                anchors.centerIn: parent
                font.weight: Font.Bold
                text: qsTr("Acct %1").arg(modelData)
            }
        }
    }
}
