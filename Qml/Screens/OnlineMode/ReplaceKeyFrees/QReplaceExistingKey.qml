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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_901
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    content: QAddAnExistingKey {
        existList: walletInfo.signerExistList
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 215
            height: 48
            label.text: STR.STR_QML_900
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                var _input = {
                    type: "request-add-a-key"
                }
                QMLHandle.sendEvent(EVT.EVT_REPLACE_KEYS_ENTER, _input)
            }
        }
        QTextButton {
            width: 150
            height: 48
            label.text: STR.STR_QML_899
            label.font.pixelSize: 16
            type: eTypeE
            enabled: _content.contentItem.fingerPrint !== ""
            onButtonClicked: {
                inputtingIndex.current_index = walletInfo.reuseKeyGetCurrentIndex(_content.contentItem.fingerPrint)
                inputtingIndex.device_name = _content.contentItem.key_name
                inputtingIndex.device_type = _content.contentItem.device_type
                inputtingIndex.signer_type = _content.contentItem.type
                inputtingIndex.device_tag = _content.contentItem.tag
                inputtingIndex.device_xfp = _content.contentItem.fingerPrint
                inputtingIndex.device_bip32_path = walletInfo.bip32path(inputtingIndex.device_xfp, inputtingIndex.current_index)
                setReplaceFlow("customize-key-account")
            }
        }
    }
}
