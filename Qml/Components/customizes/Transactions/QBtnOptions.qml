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
    property alias funcs    : imExContextMenu.functions
    property alias labels   : imExContextMenu.labels
    property alias icons    : imExContextMenu.icons
    property alias colors   : imExContextMenu.colors
    property alias enables  : imExContextMenu.enables
    property alias visibles : imExContextMenu.visibles

    property bool  enableRequestSignature : false
    property bool  enableScheduleBroadcast: false
    property bool  enableCancelTransaction: false
    property bool  isAssisedWallet        : false
    property bool  isSharedWallet         : false

    width: 160
    height: 48
    type: eSECONDARY
    label: "More Options"
    optionVisible: imExContextMenu.visible
    layoutDirection: Qt.RightToLeft
    onButtonClicked: {
        imExContextMenu.x = 20
        imExContextMenu.y = 20 - imExContextMenu.height
        imExContextMenu.open()
    }
    QContextMenu {
        id: imExContextMenu
        menuWidth: 250
        labels: [
            "Request signature",
            "Schedule broadcast",
            STR.STR_QML_691,
            (isAssisedWallet || isSharedWallet) ? "Cancel transaction" : "Remove transaction"
        ]
        icons: [
            "qrc:/Images/Images/OnlineMode/signature-dark.png",
            "qrc:/Images/Images/OnlineMode/scheduling-dark.png",
            "qrc:/Images/Images/Copy_031F2B.png",
            "qrc:/Images/Images/OnlineMode/cancel_red_24dp.png"
        ]
        colors:   [ "#031F2B", "#031F2B", "#031F2B", "#CF4018" ]
        enables:  [ enableRequestSignature, enableScheduleBroadcast, true, enableCancelTransaction ]
        visibles: [ enableRequestSignature, enableScheduleBroadcast, true, enableCancelTransaction ]
        onItemClicked: {
            functions[index]()
        }
    }
}
