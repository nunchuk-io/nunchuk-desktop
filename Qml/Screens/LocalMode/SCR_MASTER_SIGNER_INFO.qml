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
            signerDerivationPath: ""
            fingerPrint: AppModel.masterSignerInfo.fingerPrint
            keyName: AppModel.masterSignerInfo.masterSignername
            signerInfo: AppModel.masterSignerInfo
        }
    }
    Component {
        id: _keyInfo
        QKeyInfo {
            signerName: AppModel.masterSignerInfo.masterSignername
            signerSpec: ""
            signerXfp: AppModel.masterSignerInfo.fingerPrint
            signerDerivationPath: ""
            signerCardId: AppModel.masterSignerInfo.masterSignerDevice.cardId
            signerType: AppModel.masterSignerInfo.signerType
            isPrimaryKey: AppModel.masterSignerInfo.isPrimaryKey
            messageToSign: AppModel.masterSignerInfo.message
            messageToSignSHA256: AppModel.masterSignerInfo.messageSHA256
            isConnected: AppModel.masterSignerInfo.masterSignerDevice.deviceConnected
            signerInfo: AppModel.masterSignerInfo

            function requestRename(currentText) {
                QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_EDIT_NAME, currentText)
            }

            onRequestGetExpubs: {
                QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_GET_XPUBS, AppModel.masterSignerInfo.fingerPrint)
            }
            onRequestHealthCheck: {
                QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_HEALTH_CHECK, AppModel.masterSignerInfo.masterSignerId)
            }

            onRequestDeleteKey: {
                QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST, AppModel.masterSignerInfo.masterSignerId)
            }
            onCloseClicked: {
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
        }
    }
}
