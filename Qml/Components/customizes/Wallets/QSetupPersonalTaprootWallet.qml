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
// Qt imports
import QtQuick 2.4
import QtQuick.Controls 2.3
import Qt.labs.platform 1.1
import QtGraphicalEffects 1.12

// Application-specific imports
import "../../../../localization/STR_QML.js" as STR
import "../../customizes"
import "../../customizes/Buttons"
import "../../customizes/Popups"
import "../../customizes/Signers"
import "../../customizes/Texts"
import "../../origins"

// Third-party or additional module imports
import DataPool 1.0
import EWARNING 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0

QOnScreenContentTypeA {
    id: signerConfigRoot
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1696
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Item {
        Row {
            height: 503
            spacing: 22
            Flickable {
                id: flickerLeft
                width: 350 + 6
                height: 503
                clip: true
                flickableDirection: Flickable.VerticalFlick
                interactive: true
                contentHeight: contentSigners.childrenRect.height
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: contentSigners
                    width: 350
                    spacing: 12
                    Column {
                        width: 350
                        QLato {
                            height: 20
                            text: STR.STR_QML_337
                            font.weight: Font.Bold
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                        QListView {
                            id: supportedSigners
                            width: 350
                            height: contentHeight
                            clip: true
                            interactive: false
                            model: newWalletInfo.assignAvailableSigners
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
                                }
                                visible: single_signer_taproot_supported && !single_signer_checked
                                height: visible ? 84 : 0
                                onBtnClicked: {
                                    setkeyCheckPassphrase(model)
                                }
                                enabled: true
                                opacity: enabled ? 1.0 : 0.4
                            }
                        }
                    }
                    QLine {
                        width: 350
                    }
                    Column {
                        width: 350
                        QLato {
                            height: 20
                            text: STR.STR_QML_1697
                            font.weight: Font.Bold
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                        QListView {
                            id: notSupportedSigners
                            width: 350
                            height: contentHeight
                            interactive: false
                            clip: true
                            model: newWalletInfo.assignAvailableSigners
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
                                }
                                isShowCheckBox: false
                                visible: !single_signer_taproot_supported
                                height: visible ? 84 : 0
                                onBtnClicked: {

                                }
                                enabled: false
                                opacity: enabled ? 1.0 : 0.4
                            }
                        }
                    }
                }
            }
            Flickable {
                id: flickerRight
                width: 350 + 6
                height: 503
                clip: true
                flickableDirection: Flickable.VerticalFlick
                interactive: true
                contentHeight: contentSignersAssign.childrenRect.height
                ScrollBar.vertical: ScrollBar { active: true }
                function updateVisibleCount() {
                    return newWalletInfo.assignAvailableSigners.signerSelectedCount
                }
                Column {
                    id: contentSignersAssign
                    width: 350
                    spacing: 16
                    Column {
                        visible: newWalletInfo.assignAvailableSigners.signerSelectedCount > 0
                        width: 350
                        QLato {
                            height: 20
                            text: STR.STR_QML_1711
                            font.weight: Font.Bold
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                        QListView {
                            id: signersAssign
                            width: 350
                            height: signersAssign.contentHeight
                            clip: true
                            model: newWalletInfo.assignAvailableSigners
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
                                    single_derivationPath: singleSigner_derivationPath
                                }
                                visible: single_signer_checked
                                height: visible ? 84 : 0
                                hasEditPath: true
                                onBtnClicked: {
                                    model.single_signer_checked = !model.single_signer_checked
                                    newWalletInfo.walletM = 0
                                    newWalletInfo.walletN = 0
                                }
                                enabled: true
                                opacity: enabled ? 1.0 : 0.4
                                onBip32PathClick: {
                                    var modelData = newWalletInfo.assignAvailableSigners.get(index);
                                    editBip32Path.requestEditBIP32Path(modelData, modelData)
                                }
                            }
                        }
                    }
                    QLine {
                        width: 350
                        visible: newWalletInfo.assignAvailableSigners.signerSelectedCount > 0
                    }
                    QEditNumberKeys {}
                }
            }
        }
    }
    onPrevClicked: QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK)
    bottomRight: Row {
        height: 48
        spacing: 23
        Row {
            spacing: 8
            height: 28
            QLato {
                width: paintedWidth
                text: STR.STR_QML_353
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.verticalCenter: parent.verticalCenter
            }
            QBadge {
                text: {
                    var ret = ""
                    if(newWalletInfo.walletM === 0){
                        ret = STR.STR_QML_557
                    }
                    else if(newWalletInfo.walletN === 1 && newWalletInfo.walletM === 1){
                        ret = STR.STR_QML_558
                    }
                    else if(newWalletInfo.walletN > 1 && newWalletInfo.walletM >= 1){
                        ret = qsTr("%1/%2 %3").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN).arg(STR.STR_QML_069)
                    }
                    return ret;
                }
                color: "#EAEAEA"
                font.weight: Font.Normal
                anchors.verticalCenter: parent.verticalCenter
            }
            anchors.verticalCenter: parent.verticalCenter
        }

        QTextButton {
            width: 133
            height: 48
            anchors.verticalCenter: parent.verticalCenter
            label.text: STR.STR_QML_1698
            label.font.pixelSize: 16
            type: eTypeE
            enabled: newWalletInfo.walletM > 0
            onButtonClicked: {
                nextClicked()
            }
        }
    }
    property var targetEditBIP32: AppModel.newWalletInfo
    QSetupEditBIP32PathFlow {
        id: editBip32Path
    }
}
