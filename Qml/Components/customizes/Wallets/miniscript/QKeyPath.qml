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

Item {
    id: miniscriptContainer
    width: parent.width
    height: getKeyHeight()
    property var miniscript
    function getKeyHeight() {
        if(miniscript.keyObj != null && miniscript.keyObj != undefined) {
            if (miniscript.keyObj.singleSigner_derivationPath !== undefined && miniscript.keyObj.singleSigner_derivationPath !== null) {
                return 76
            } else {
                return 58
            }            
        } else {
            return 48
        }
    }
    QKeyComponentAddRemove {
        anchors.fill: parent
        onBip32PathClick: (key, xfp, path) => {
            editBip32Path.clearError()
            editBip32Path.isShowListDevice = false
            editBip32Path.signerData = signerData
            editBip32Path.idx = index
            editBip32Path.xfp = xfp
            editBip32Path.key = key
            editBip32Path.open()
        }
        onRemoveClicked: (key) => {
            _confirmRemoveKey.key = key
            _confirmRemoveKey.firstLine = "KeyPath"
            _confirmRemoveKey.open()
        }
        onAddClicked: (key) => {
            if (typeof(sandbox) !== 'undefined' && sandbox != null) {
                var obj = {
                    keyName: key,
                    group: "KeyPath",
                }
                sandbox.requestAddOrRepaceKey(obj)
            } else {
                // Fallback for when sandbox is not available
                newWalletInfo.requestAddSigner("KeyPath", key)
            }
        }
    }
}