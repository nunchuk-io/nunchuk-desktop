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
import QtQuick 2.15
import QtQuick.Controls 2.15
import "./../Popups"

Popup {
    id: popup
    width: parent.width
    height: parent.height
    modal: true
    dim: false
    anchors.centerIn: parent
    visible: toastLoader.model.count > 0
    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    property alias isScreenBase: toastLoader.isScreenBase

    QToastMessage {
        id: toastLoader
        anchors.fill: parent
        model: ListModel {
            id: toastes
        }
    }

    background: Item {
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
}
