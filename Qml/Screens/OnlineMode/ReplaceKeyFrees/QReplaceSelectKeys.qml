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
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1366
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Item {
        Column {
            anchors.fill: parent
            spacing: 24
            QLato {
                width: 539
                height: paintedHeight
                text: STR.STR_QML_1367
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
            }
            QListView {
                id: signerlist
                width: 539
                height: 440
                spacing: 16
                model: walletInfo.walletSingleSignerAssigned
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: QReplaceKeyDelegate {
                    width: signerlist.width
                    signerData {
                        single_name: model.singleSigner_name
                        single_type: model.single_signer_type
                        single_tag: model.single_signer_tag
                        single_devicetype: model.single_signer_devicetype
                        single_masterFingerPrint: model.singleSigner_masterFingerPrint
                        single_account_index: model.single_signer_account_index
                        single_checked: model.single_signer_checked
                        single_is_local: model.single_signer_is_local
                        single_value_key: model.single_signer_value_key
                        single_derivationPath: model.singleSigner_derivationPath
                        single_device_cardid: model.single_signer_device_cardid
                        single_isOccupied: model.single_signer_isOccupied
                        single_isReplaced: model.single_signer_isReplaced
                        single_keyReplaced: model.single_signer_keyReplaced
                    }
                    onReplaceClicked: {
                        setReplaceFlow("replace-existing-key")
                        selectReplaceKey(model.singleSigner_masterFingerPrint, index)
                    }
                    onRemoveClicked: {
                        _confirmRemoveKey.idx = idx
                        _confirmRemoveKey.open()
                    }
                }
            }
        }

    }

    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    bottomRight: Row{
        spacing: 12
        QButtonLargeTail {
            id: otheraction
            width: 102
            height: 48
            label: STR.STR_QML_1094
            type: eSECONDARY
            optionVisible: othersContextMenu.visible
            layoutDirection: Qt.RightToLeft
            onButtonClicked: {
                othersContextMenu.x = 20
                othersContextMenu.y = 20 - othersContextMenu.height
                othersContextMenu.open()
            }
            QContextMenu {
                id: othersContextMenu
                menuWidth: 300
                labels: {
                    var ls = [];
                    ls.push(STR.STR_QML_1340)
                    return ls
                }
                icons:{
                    var ls = [];
                    ls.push("")
                    return ls
                }
                enables: [true]
                visibles: [true]
                functions: {
                    var ls = [];
                    ls.push(function(){
                        _confirm.open()
                    })
                    return ls
                }
                colors:{
                    var ls = [];
                    ls.push("#031F2B")
                    return ls
                }
                onItemClicked: {
                    functions[index]()
                }
            }
        }
        QTextButton {
            width: 258
            height: 48
            label.text: STR.STR_QML_1369
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                var _input = {
                    type: "replace-Key-Enter",
                }
                QMLHandle.sendEvent(EVT.EVT_REPLACE_KEYS_ENTER, _input)
            }
        }
    }
    QConfirmYesNoPopup {
        id: _confirm
        property string link:"https://nunchuk.io/"
        title: STR.STR_QML_024
        contentText: STR.STR_QML_1379
        onConfirmNo: close()
        onConfirmYes: {
            var _input = {
                type: "replace-cancel",
            }
            QMLHandle.sendEvent(EVT.EVT_REPLACE_KEYS_ENTER, _input)
            closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        }
    }
    QConfirmYesNoPopup{
        id:_confirmRemoveKey
        property var idx
        title: STR.STR_QML_661
        contentText: STR.STR_QML_243
        onConfirmNo: close()
        onConfirmYes: {
            close()
            walletInfo.removeKeyReplaced(idx)
        }
    }
}
