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
// Qt imports
import QtQuick 2.4
import QtQuick.Controls 2.3
import Qt.labs.platform 1.1
import QtGraphicalEffects 1.12

// Application-specific imports
import "../../../../localization/STR_QML.js" as STR
import "../../customizes"
import "../../customizes/Buttons"
import "../../customizes/Popups"
import "../../customizes/Signers"
import "../../customizes/Texts"
import "../../origins"

// Third-party or additional module imports
import DataPool 1.0
import EWARNING 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0

Item {
    id: editBIP32PathFlow
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent   
    property var singleData:{}
    property var customData:{}
    function requestEditBIP32Path(singleData, customData) {
        bip32Path.clearError()
        bip32Path.isShowListDevice = false        
        editBIP32PathFlow.singleData = singleData
        editBIP32PathFlow.customData = customData
        bip32Path.open()
    }
    QPopupEditBIP32Path {
        id: bip32Path
        property string key: ""
        onEnterText: {
            gettingPublicKey.open()
            if (typeof(targetEditBIP32) !== 'undefined' && targetEditBIP32 != null) {                
                targetEditBIP32.editBIP32Path(singleData, customData, pathBip32)
            } else {
                gettingPublicKey.close()
                console.log("targetEditBIP32 is null")
            }
        }
    }
    QPopupGettingPublicKeyLoading {
        id: gettingPublicKey
    }

    QPopupInfo{
        id: _warningMultiSigner
        title: STR.STR_QML_661
        contentText: STR.STR_QML_1868
        btnLabel: STR.STR_QML_097
        action: function() {
                requestEditBIP32Path(singleData, customData)
            }
    }
    Connections {
       target: targetEditBIP32
       function onEditBIP32PathSuccess(typeError) {
            if (typeError == 1) {
                bip32Path.close()
                gettingPublicKey.close()
            } else if (typeError == -1 || typeError == -3){
                gettingPublicKey.close()
                bip32Path.showError(typeError)
            } else if (typeError == -2) {
                gettingPublicKey.close()
                bip32Path.isShowListDevice = true
                bip32Path.showError(typeError)
            } else {}
        }
       function onDuplicateKeyError(singleData, customData){
            if (bip32Path.opened) return
           _warningMultiSigner.open()
           editBIP32PathFlow.singleData = singleData
           editBIP32PathFlow.customData = customData
       }
    }
}
