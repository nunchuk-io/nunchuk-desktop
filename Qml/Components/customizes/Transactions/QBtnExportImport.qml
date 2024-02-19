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
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QButtonLargeTail {
    id: advancedBtn
    property alias funcs: imExContextMenu.functions
    width: 240
    height: 48
    type: eSECONDARY
    label: STR.STR_QML_299
    optionVisible: imExContextMenu.visible
    onButtonClicked: {
        imExContextMenu.x = 20
        imExContextMenu.y = 20 - imExContextMenu.height
        imExContextMenu.open()
    }
    QContextMenu {
        id: imExContextMenu
        menuWidth: 200
        labels: [
            STR.STR_QML_300,
            STR.STR_QML_114,
            STR.STR_QML_301,
            STR.STR_QML_302
        ]
        icons: [
            "qrc:/Images/Images/ExportFile.svg",
            "qrc:/Images/Images/OnlineMode/QRCodeScan.png",
            "qrc:/Images/Images/importFile.svg",
            "qrc:/Images/Images/OnlineMode/QRCodeScan.png"
        ]

        onItemClicked: {
            functions[index]()
        }
    }
}
