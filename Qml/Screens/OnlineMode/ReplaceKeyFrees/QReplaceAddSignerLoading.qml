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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
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
    enableHeader: false
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Item {
        Column {
            width: 400
            height: 56
            anchors.centerIn: parent
            spacing: 16
            QProgressbarTypeA {
                id: progresBar
                percentage: AppModel.addSignerPercentage
            }
            QLato{
                font.weight: Font.Bold
                font.pixelSize: 20
                text: {
                    var type = walletInfo.deviceType
                    if (type == "ledger") {
                        return STR.STR_QML_826
                    } else if (type == "trezor") {
                        return STR.STR_QML_831
                    } else if (type == "coldcard") {
                        return STR.STR_QML_912
                    } else if (type == "bitbox02") {
                        return STR.STR_QML_930
                    } else {
                        return "Unknown"
                    }
                }

                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}
