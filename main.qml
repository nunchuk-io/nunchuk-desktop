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
import "./localization/STR_QML.js" as STR

Item {
    width: QAPP_DEVICE_WIDTH
    height: QAPP_DEVICE_HEIGHT
    antialiasing: true
    smooth: true
    Loader {
        id: screen_layer
        anchors.fill: parent
        transformOrigin: Item.TopLeft
    }
    function screen_Transition() {
        screen_layer.sourceComponent = null
        screen_layer.sourceComponent = QAppScreen
    }

    ListModel { id: onsDataList }
    Repeater {
        id: ons_Creator
        transformOrigin: Item.TopLeft
        Loader {
            id: osd_Loader
            source: osdSource
        }
    }
    function load_Popup(Onsdata, popCount) {
        onsDataList.clear()
        for(var onsCnt = 0; onsCnt < popCount; onsCnt++ ) { var data = {'osdSource': Onsdata[onsCnt]}; onsDataList.append(data); }
        ons_Creator.model = onsDataList
    }

    property int popupWidth: 800
    property int popupHeight: 700

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

    Popup {
        id: popup
        width: parent.width
        height: parent.height
        modal: true
        background: Item{}
        dim: false
        anchors.centerIn: parent
        visible: toastLoader.model.count > 0
        closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
        QToastMessage {
            id: toastLoader
            isScreenBase: onsDataList.count == 0 ? true : false
            anchors.fill: parent
            model: ListModel { id: toastes}
            // onToastVisibleChanged : {
            //     if(toastLoader.model.count == 0){
            //         popup.close()
            //     }
            // }
        }
    }

    function loadToastMessage(toastObj) {
        console.log(toastObj.code, toastObj.type, toastObj.what)
        for(var i = 0; i < toastLoader.model.count; i++){
            console.log(toastLoader.model.get(i).code , toastObj.code, toastObj.type)
            if(toastLoader.model.get(i).code === toastObj.code && (toastObj.type > EWARNING.ERROR_MSG)) {
                return;
            }
        }
        var toastItem = {
            "contentDisplay": toastObj.contentDisplay,
            "explaination"  : toastObj.explaination,
            "what"          : toastObj.what,
            "code"          : toastObj.code,
            "type"          : toastObj.type,
            "popupType"     : toastObj.popupType
        }
        toastLoader.model.append(toastItem)
        popup.open()
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
