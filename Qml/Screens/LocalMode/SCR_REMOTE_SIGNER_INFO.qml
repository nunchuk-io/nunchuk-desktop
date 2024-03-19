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
import DRACO_CODE 1.0
import "../../Components/customizes"
import "../../Components/customizes/Signers"

QScreen {
    readonly property int eKEY_INFO: 1
    readonly property int eSIGN_MESSAGE: 2
    property var action_type: eKEY_INFO

    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: action_type === eKEY_INFO ? _keyInfo : _signMessage
    }
    Component {
        id: _signMessage
        QSignMessage {
            signerDerivationPath: AppModel.singleSignerInfo.signerDerivationPath
            fingerPrint: AppModel.singleSignerInfo.signerMasterFingerPrint
            keyName: AppModel.singleSignerInfo.signerName
            signerInfo: AppModel.singleSignerInfo
        }
    }
    Component {
        id: _keyInfo
        QKeyInfo {
            signerName: AppModel.singleSignerInfo.signerName
            signerSpec: AppModel.walletInfo.walletEscrow ? AppModel.singleSignerInfo.signerPublickey : AppModel.singleSignerInfo.signerXpub
            signerXfp: AppModel.singleSignerInfo.signerMasterFingerPrint
            signerDerivationPath: AppModel.singleSignerInfo.signerDerivationPath
            signerCardId: ""
            signerType: AppModel.singleSignerInfo.signerType
            isPrimaryKey: AppModel.singleSignerInfo.isPrimaryKey
            messageToSign: ""
            messageToSignSHA256: ""
            isTopXPubs: false
            signerInfo: AppModel.singleSignerInfo
            isConnected: AppModel.masterSignerInfo.masterSignerDevice.deviceConnected

            function requestRename(currentText) {
                QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_INFO_EDIT_NAME, currentText)
            }

            onRequestHealthCheck: {
                QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK)
            }

            onRequestDeleteKey: {
                var remoteSignerObj = { "master_fingerprint"    : AppModel.singleSignerInfo.signerMasterFingerPrint,
                    "derivation_path"       : AppModel.singleSignerInfo.signerDerivationPath };
                QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST, remoteSignerObj)
            }
        }
    }
}
