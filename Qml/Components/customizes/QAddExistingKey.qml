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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Signers"
import "../../../localization/STR_QML.js" as STR

Item {
    property string notice: ""
    property var existList: draftWallet.signerExistList
    property string key_name: ""
    property string fingerPrint: ""

    Column {
        spacing: 24
        QLato {
            text: notice
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            visible: notice != ""
        }
        Rectangle {
            width: 539
            height: 464
            border.width: 1
            border.color: "#EAEAEA"
            radius: 8
            QListView {
                model: existList
                clip: true
                ScrollBar.vertical: ScrollBar { active: true }
                anchors.fill: parent
                delegate: QSingleSignerExistDelegate {
                    signerData {
                        single_name: modelData.singleSigner_name
                        single_type: modelData.single_signer_type
                        single_tag: modelData.single_signer_tag
                        single_devicetype: modelData.single_signer_devicetype
                        single_masterFingerPrint: modelData.singleSigner_masterFingerPrint
                        single_account_index: modelData.single_signer_account_index
                        single_checked: modelData.single_signer_checked
                        single_is_local: modelData.single_signer_is_local
                        single_device_cardid: modelData.single_signer_device_cardid
                        single_need_Topup_Xpub: modelData.single_signer_need_Topup_Xpub
                        single_enabled: modelData.single_signer_enabled
                    }
                    width: 539
                    opacity: modelData.single_signer_enabled ? 1.0 : 0.4
                    enabled: modelData.single_signer_enabled
                    isSelected: fingerPrint === modelData.singleSigner_masterFingerPrint && key_name === modelData.singleSigner_name
                    onRadioClicked: {
                        fingerPrint = modelData.singleSigner_masterFingerPrint
                        key_name = modelData.singleSigner_name
                    }
                }
            }
        }
    }
}
