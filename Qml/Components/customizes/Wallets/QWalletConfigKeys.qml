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

import QtQuick 2.12
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import Qt.labs.platform 1.1
import DataPool 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Signers"
import "../../customizes/Texts"
import "../../../../localization/STR_QML.js" as STR

Item {
    width: 352
    height: 480
    Column {
        anchors.fill: parent
        spacing: 8
        Row {
            height: 32
            spacing: 8
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/mulitsig-dark.svg"
                anchors.verticalCenter: parent.verticalCenter
            }
            QLato {
                width: 44
                height: 20
                anchors.verticalCenter: parent.verticalCenter
                text: STR.STR_QML_015
                font.weight: Font.Bold
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
        }
        Rectangle {
            width: 350
            height: 452
            border.width: 1
            border.color: "#EAEAEA"
            radius: 12
            QListView {
                id: signerlist
                anchors.fill: parent
                anchors.margins: 12
                spacing: 16
                clip: true
                model: walletInfo.walletSingleSignerAssigned
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: QWalletKeyDelegate {
                    width: signerlist.width
                    signerData {
                        single_name: singleSigner_name
                        single_type: single_signer_type
                        single_tag: single_signer_tag
                        single_devicetype: single_signer_devicetype
                        single_masterFingerPrint: singleSigner_masterFingerPrint
                        single_account_index: single_signer_account_index
                        single_checked: single_signer_checked
                        single_is_local: single_signer_is_local
                        single_value_key: single_signer_value_key
                        single_derivationPath: singleSigner_derivationPath
                        single_device_cardid: single_signer_device_cardid
                    }
                    onViewPoliciesRequest: {
                        var objectRequest = {
                            "requestType": "viewPolicies",
                            "requestData": AppModel.walletInfo.walletId
                        }
                        QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_SIGNER_INFO_REQUEST, objectRequest)
                    }
                }
            }
        }
    }
}
