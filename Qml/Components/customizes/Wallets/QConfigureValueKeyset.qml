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
import QRCodeItem 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id: _contentBip
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1693
    property var walletObject: AppModel.newWalletInfo
    content: Item {
        Column {
            id:_colum
            width: parent.width
            spacing: 16
            QLato {
                width: 539
                height: 84
                anchors.left: parent.left
                text: STR.STR_QML_1694
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }

            QListView {
                id: signers
                property bool enoughKeyset: (walletObject.walletSingleSignerAssigned.signerValueKeyCount >= walletObject.walletM)

                width: 350
                height: 84 * Math.max(signers.count, 5)
                clip: true
                model: walletObject.walletSingleSignerAssigned
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: QSignerConfigureDelegate {
                    signerData {
                        single_name: singleSigner_name
                        single_type: single_signer_type
                        single_tag: single_signer_tag
                        single_devicetype: single_signer_devicetype
                        single_masterFingerPrint: singleSigner_masterFingerPrint
                        single_account_index: single_signer_account_index
                        single_checked: single_signer_checked
                        single_is_local: single_signer_is_local
                        single_device_cardid: single_signer_device_cardid
                        single_value_key: single_signer_value_key
                    }
                    checkedProperty: single_signer_value_key
                    onBtnClicked: {
                        single_signer_value_key = !single_signer_value_key && !signers.enoughKeyset;
                    }
                    opacity: !single_signer_value_key ? 1.0 : 0.4
                }
            }
        }
    }
    nextEnable: walletObject.walletSingleSignerAssigned.signerValueKeyCount === walletObject.walletM
}
