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
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "./../../origins"
import "./../../customizes"
import "./../../customizes/Chats"
import "./../../customizes/Texts"
import "./../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    Column {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 24
        QText {
            font.family: "Lato"
            font.pixelSize: 28
            font.weight: Font.Bold
            text: STR.STR_QML_742
        }
        Item {
            width: 627
            height: 750
            QComboBox {
                id: currencies
                currentIndex: ProfileSetting.currencyIndex
                width: 627
                height: 48
                model: ProfileSetting.currencies
                textRole: "displayName"
                onActivated: {
                    if (ProfileSetting.currencies.length > 0 && currentIndex >=0) {
                        var item = ProfileSetting.currencies[currentIndex]
                        ProfileSetting.setCurrency(item.currency)
                    }
                }
            }
        }
    }
}
