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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../Components/customizes/Wallets/miniscript"
import "../../../../../localization/STR_QML.js" as STR


Rectangle {
    id: dlg
    radius: 8
    border.width: isFocused ? 2 : 1
    border.color: isFocused ? "#031F2B" : "#DEDEDE"
    property bool isFocused: false
    width: 500
    implicitHeight: miniscriptContainer.childrenRect.height + 16 + 8
    signal selectedClicked()
    Column {
        id: miniscriptContainer
        anchors.fill: parent
        anchors.margins: 16
        width: dlg.width
        spacing: 8
        Item {
            width: dlg.width
            height: 16
            QBadge {
                width: 74
                height: 16
                text: STR.STR_QML_1846
                fontColor: "#FFFFFF"
                color: "#031F2B"
                font.pixelSize: 10
                font.weight: Font.Black
                anchors.bottom: parent.bottom
            }
            visible: walletInfo.keyPathActivated
        }
        Repeater {
            model: walletInfo.keypaths
            delegate: QKeyComponentTransaction {
                height: 58
                width: dlg.width
                property var miniscript: modelData
                property var itemData: modelData.keyObj
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
            }
        }
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            selectedClicked()
        }
    }
}