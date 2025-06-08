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
import "../../../Components/customizes/QRCodes"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_901
    extraHeader: Item {}
    property var newWalletInfo: AppModel.newWalletInfo
    property var sandbox        : AppModel.newWalletInfo.sandbox
    function closeToSetup() {
        sandbox.screenFlow = "setup-group-wallet"
        sandbox.clearOccupied()
    }

    onCloseClicked: closeToSetup()
    content: QAddAnExistingKey {
        notice: ""
        existList: AppModel.newWalletInfo.signerExistList
    }
    onPrevClicked: closeToSetup()
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 215
            height: 48
            label.text: STR.STR_QML_900
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                sandbox.requestAddNewKey()
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
                if(!_content.contentItem.modelKey.signer_allowAssignToWallet) {
                    _infoHotWallet.open()
                    return
                }
                var masterSignerObj = {
                    type: "group-sandbox-add-existing-key",
                    xfp: _content.contentItem.fingerPrint,
                    keyName: _content.contentItem.key_name
                };
                QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, masterSignerObj)
            }
        }
    }
    QPopupInfo{
        id:_infoHotWallet
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1771
        usingMin: true
    }
}
