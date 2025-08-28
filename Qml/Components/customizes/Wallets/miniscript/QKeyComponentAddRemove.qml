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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

QKeyComponent {
    signal removeClicked(string key)
    signal addClicked(string key)
    property var itemData: miniscript.keyObj
    signerData {
        single_name: itemData.singleSigner_name
        single_type: itemData.single_signer_type
        single_tag: itemData.single_signer_tag
        single_devicetype: itemData.single_signer_devicetype
        single_masterFingerPrint: itemData.singleSigner_masterFingerPrint
        single_account_index: itemData.single_signer_account_index
        single_checked: itemData.single_signer_checked
        single_is_local: itemData.single_signer_is_local
        single_value_key: itemData.single_signer_value_key
        single_derivationPath: itemData.singleSigner_derivationPath
        single_device_cardid: itemData.single_signer_device_cardid
        single_isOccupied: itemData.single_signer_isOccupied
        single_isReplaced: itemData.single_signer_isReplaced
        single_keyReplaced: itemData.single_signer_keyReplaced
    }
    QTextButton {
        anchors {
            top: parent.top
            topMargin: 8
            right: parent.right
            rightMargin: 12
        }
        width: label.paintedWidth + 2*16
        height: 36
        type: eTypeB
        visible: label.text !== ""
        label.text: {
            var hasKey = miniscript.keyObj != null && miniscript.keyObj != undefined
            if (typeof(sandbox) !== 'undefined' && sandbox != null) {
                if (sandbox.isReplace) {
                    if (hasKey) {
                        var keyObj = miniscript.keyObj
                        return keyObj.single_signer_isReplaced ? STR.STR_QML_1134 : STR.STR_QML_1368
                    } else {
                        return STR.STR_QML_941
                    }
                } else if (sandbox.isRecovery) {                    
                    if (hasKey) {
                        var keyObj = miniscript.keyObj
                        return keyObj.single_signer_is_local ? "" : STR.STR_QML_941
                    } else {
                        return STR.STR_QML_941
                    }
                } else {
                    // Use for Flow Create Group Wallet
                    return hasKey ? STR.STR_QML_1134 : STR.STR_QML_941
                }
            } else {
                // Fallback for when sandbox is not available
                return hasKey ? STR.STR_QML_1134 : STR.STR_QML_941
            }
        }
        label.font.pixelSize: 12
        onButtonClicked: {
            if(miniscript.keyObj != null && miniscript.keyObj != undefined) {
                removeClicked(miniscript.key)
            } else {
                addClicked(miniscript.key)
            }
        }
    }
}