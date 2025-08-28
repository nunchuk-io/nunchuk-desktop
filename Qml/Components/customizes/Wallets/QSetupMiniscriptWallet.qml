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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Wallets/miniscript"
import "../../../Components/customizes/Popups"
import "../../../Components/customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1696
    content: Flickable {
        flickableDirection: Flickable.VerticalFlick
        ScrollBar.vertical: ScrollBar { active: true }
        contentHeight: columnMiniscript.childrenRect.height
        contentWidth: 504
        clip: true    
        Column {
            id: columnMiniscript
            width: 450
            spacing: 4
            Item {
                width: 450
                height: 24
                Row {
                    anchors.fill: parent
                    spacing: 8
                    QIcon {
                        iconSize: 24
                        source: "qrc:/Images/Images/policies_dark.svg"
                    }
                    QLato {
                        width: paintedWidth
                        text: STR.STR_QML_1844
                        font.weight: Font.Bold
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            Item {
                width: 450
                height: 24
                QBadge {
                    width: 63
                    height: 16
                    text: STR.STR_QML_1846
                    fontColor: "#FFFFFF"
                    color: "#031F2B"
                    font.pixelSize: 10
                    font.weight: Font.Black
                    anchors.bottom: parent.bottom
                    visible: newWalletInfo.keypaths.length > 0
                }
                QBadge {
                    width: 108
                    height: 16
                    text: STR.STR_QML_1858
                    fontColor: "#FFFFFF"
                    color: "#031F2B"
                    opacity: 0.4
                    font.pixelSize: 10
                    font.weight: Font.Black
                    anchors.bottom: parent.bottom
                    visible: newWalletInfo.keypaths.length == 0
                }
                visible: newWalletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT
            }
            Column {
                visible: newWalletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT && newWalletInfo.keypaths.length > 0
                width: 450
                Repeater {
                    model: newWalletInfo.keypaths
                    delegate: QKeyPath {
                        width: 450
                        miniscript: modelData
                    }
                }
            }
            QLine {
                width: 450
                height: 1
                color: "#DEDEDE"
                visible: newWalletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT && newWalletInfo.keypaths.length > 0
            }
            Item {
                width: 450
                height: 28
                QBadge {
                    width: 74
                    height: 16
                    text: STR.STR_QML_1857
                    fontColor: "#FFFFFF"
                    color: "#031F2B"
                    font.pixelSize: 10
                    font.weight: Font.Black
                    anchors.bottom: parent.bottom
                }
                visible: newWalletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT
            }
            Column {
                width: 450
                Repeater {
                    model: newWalletInfo.treeMiniscript
                    delegate: QMiniscriptPolicesAddDelegate {
                        width: 450
                        miniscript: modelData
                    }
                }
            }
        }
    }
    nextEnable: newWalletInfo.enoughSigners

    QConfirmYesNoPopup{
        id:_confirmRemoveKey
        property string key: ""
        property string firstLine: ""
        title: STR.STR_QML_661
        contentText: STR.STR_QML_243
        onConfirmNo: close()
        onConfirmYes: {
            close()
            newWalletInfo.requestRemoveSigner(firstLine, key)
        }
    }
    QPopupEditBIP32Path {
        id: editBip32Path
        property string key: ""
        onEnterText: {
            gettingPublicKey.open()
            newWalletInfo.editBIP32Path(key, xfp, pathBip32)
        }
    }
    QPopupGettingPublicKeyLoading {
        id: gettingPublicKey
    }
    QPopupInfoTwoButtons {
        id: _warningMultiSigner
        property var key: ""
        property var xfp: ""
        property var pathBip32: ""
        property var signerData: {}
        isVertical: false
        title: STR.STR_QML_661
        contentText: STR.STR_QML_1868
        labels: [STR.STR_QML_035, STR.STR_QML_1867]
        funcs: [
            function() {},
            function() {
                editBip32Path.clearError()
                editBip32Path.isShowListDevice = false
                editBip32Path.signerData = signerData
                editBip32Path.idx = 0
                editBip32Path.xfp = xfp
                editBip32Path.key = key
                editBip32Path.open()
            }
        ]
    }

    Connections {
        target: newWalletInfo
        function onEditBIP32PathSuccess(typeError) {
            if (typeError == 1) {
                editBip32Path.close()
                gettingPublicKey.close()
            } else if (typeError == -1 || typeError == -3){
                gettingPublicKey.close()
                editBip32Path.showError(typeError)
            } else if (typeError == -2) {
                gettingPublicKey.close()
                editBip32Path.isShowListDevice = true
                editBip32Path.showError(typeError)
            } else {}
        }
        function onDuplicateKeyError(key, xfp, path, keyObj){
            if (editBip32Path.opened) return
            _warningMultiSigner.open()
            _warningMultiSigner.key = key
            _warningMultiSigner.xfp = xfp
            _warningMultiSigner.pathBip32 = path
            var data = {
                    single_name: keyObj.singleSigner_name,
                    single_type: keyObj.single_signer_type,
                    single_tag: keyObj.single_signer_tag,
                    single_devicetype: keyObj.single_signer_devicetype,
                    single_masterFingerPrint: keyObj.singleSigner_masterFingerPrint,
                    single_account_index: keyObj.single_signer_account_index,
                    single_checked: keyObj.single_signer_checked,
                    single_is_local: keyObj.single_signer_is_local,
                    single_value_key: keyObj.single_signer_value_key,
                    single_derivationPath: keyObj.singleSigner_derivationPath,
                    single_device_cardid: keyObj.single_signer_device_cardid,
                    single_isOccupied: keyObj.single_signer_isOccupied,
                    single_isReplaced: keyObj.single_signer_isReplaced,
                    single_keyReplaced: keyObj.single_signer_keyReplaced,
                }
            _warningMultiSigner.signerData = data; // Ensure it's an object
        }
    }
}
