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

import "./Qml/Popups"
import "./Qml/Components/customizes"
import "./Qml/Components/customizes/Popups"
import "./Qml/core"
import "./localization/STR_QML.js" as STR

Item {
    width: QAPP_DEVICE_WIDTH
    height: QAPP_DEVICE_HEIGHT
    antialiasing: true
    smooth: true
    property int popupWidth: 800
    property int popupHeight: 700
    ScreenHost {
        id: screenHost
        objectName: "screenHost"
    }

    PopupHost {
        id: popupHost
        objectName: "popupHost"
    }

    SubScreenHost {
        id: subScreenHost
        objectName: "subScreenHost"
    }

    QPopupInfo{
        id:_checkforupdate
        z:100
    }
    function funcUpdateAvailable(title,message,doItLaterCTALbl) {
        _checkforupdate.title = title
        _checkforupdate.contentText = message
        _checkforupdate.btnLabel = doItLaterCTALbl
        _checkforupdate.open()
    }
    function funcUpdateRequired(title,message,doItLaterCTALbl) {
        _checkforupdate.title = title
        _checkforupdate.contentText = message
        _checkforupdate.btnLabel = doItLaterCTALbl
        _checkforupdate.open()
    }

    ToastHost {
        id: toastHost
        objectName: "toastHost"
        isScreenBase: popupHost.dataList.count == 0 ? true : false
    }

    QPopupInfoTwoButtons {
        id: _syncing_wallet_from_server
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1289
        labels: [STR.STR_QML_432, STR.STR_QML_433]
        isVertical: false
        funcs: [
            function() {
                AppModel.confirmSyncingWalletFromServer(false, true)
            },
            function() {
                AppModel.confirmSyncingWalletFromServer(true, false)
            }
        ]
    }
    Connections {
        target: AppModel
        function onSyncingWalletFromServer(fingerPrint) {
            _syncing_wallet_from_server.contentText = STR.STR_QML_1289.arg(fingerPrint.toUpperCase())
            _syncing_wallet_from_server.open()
        }
    }
}
