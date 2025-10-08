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
import "../../../Components/customizes/Wallets/miniscript"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: {
        if (sandbox.isCreate || sandbox.isReplace) {
            return STR.STR_QML_1556
        } else if (sandbox.isRecovery) {
            return STR.STR_QML_1664
        } else {
            return ""
        }
    }
    extraHeader: Item {}
    property var sandbox:       AppModel.newWalletInfo.sandbox
    onCloseClicked: {
        if (sandbox.isCreate || sandbox.isReplace) {
            closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        } else if (sandbox.isRecovery) {
            _confirm.open()
        }
    }

    content: Item {
        anchors {
            top: parent.top
            topMargin: 8
        }
        Row {
            height: parent.height
            spacing: 24
            QGroupWalletInfo {
                onCopySandboxUrl: {
                    ClientController.copyMessage(sandbox.url)
                    AppModel.showToast(0, STR.STR_QML_1655, EWARNING.SUCCESS_MSG);
                }
                onShowQrSandboxUrl: {
                    qrcodeExportSandboxUrl.open()
                }
                onClickSetting: {
                    var _input = {
                        type: "setting-group-sandbox"
                    }
                    QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
                }
            }
            Flickable {
                id: keylistflick
                clip: true
                width: 352
                height: 492
                contentHeight: keylistLoader.implicitHeight
                interactive: contentHeight > height
                flickableDirection: Flickable.VerticalFlick
                contentY : {
                    if (newWalletInfo.walletType === NUNCHUCKTYPE.MINISCRIPT ||
                        sandbox.walletType === NUNCHUCKTYPE.MINISCRIPT) {
                        return 0
                    } else {
                        return contentHeight > height ? contentHeight - height : 0
                    }
                }
                ScrollBar.vertical: ScrollBar { active: keylistflick.interactive }

                Loader {
                    id: keylistLoader
                    sourceComponent: {
                        if (newWalletInfo.walletType === NUNCHUCKTYPE.MINISCRIPT ||
                            sandbox.walletType === NUNCHUCKTYPE.MINISCRIPT) {
                            return keylistMiniComponent
                        } else {
                            return keylistComponent
                        }
                    }
                }

                Component {
                    id: keylistComponent
                    QGroupWalletKeys {

                    }
                }
                Component {
                    id: keylistMiniComponent
                    QGroupWalletMiniscriptKeys {

                    }
                }
            }
        }
    }

    bottomLeft: Item {}
    onPrevClicked: {
        if (sandbox.isCreate) {
            QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK)
        } else if (sandbox.isRecovery) {

        } else if (sandbox.isReplace) {

        }
    }

    bottomRight: Row {
        spacing: 12
        Item {
            width: _txt.paintedWidth + 2*16
            height: 48
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    _confirm.open()
                }
            }
            QLato {
                id: _txt
                width: _txt.paintedWidth + 2*16
                height: 20
                anchors.centerIn: parent
                text: {
                    if (sandbox.isCreate) {
                        return STR.STR_QML_1644
                    } else if (sandbox.isRecovery) {
                        return STR.STR_QML_245
                    } else if (sandbox.isReplace) {
                        return STR.STR_QML_1703
                    } else {
                        return ""
                    }
                }
                color: "#CF4018"
                font.weight: Font.Bold
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
        QTextButton {
            width: label.paintedWidth + 16*2
            height: 48
            label.text: {
                if (sandbox.isCreate) {
                    return STR.STR_QML_1249
                }
                else if (sandbox.isRecovery) {
                    return STR.STR_QML_1670
                }
                else if (sandbox.isReplace) {
                    return STR.STR_QML_1704
                }
                else {
                    return ""
                }
            }
            label.font.pixelSize: 16
            type: eTypeE
            enabled: sandbox.enoughSigner
            onButtonClicked: {
                var _input = {}
                if (sandbox.isReplace) {
                    _input = {
                        type: "create-replace-wallet-sandbox"
                    }
                    QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
                } else if (sandbox.isRecovery) {
                    _input = {
                        type: "review-group-sandbox"
                    }
                    QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
                } else {
                    if (sandbox.checkWaitingOthers()) {
                        _waiting_for_other.open()
                    } else {
                        if (sandbox.walletType === NUNCHUCKTYPE.MINISCRIPT) {
                            _input = {
                                type: "review-group-sandbox"
                            }
                            QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
                        } else {
                            if (sandbox.addressType === NUNCHUCKTYPE.TAPROOT) {
                                if (sandbox.groupN === 1) {
                                    _input = {
                                        type: "review-group-sandbox"
                                    }
                                    QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
                                } else {
                                    _input = {
                                        type: "switch-to-intro-taproot"
                                    }
                                    QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
                                }
                            } else {
                                _input = {
                                    type: "review-group-sandbox"
                                }
                                QMLHandle.sendEvent(EVT.EVT_SETUP_GROUP_WALLET_ENTER, _input)
                            }
                        }
                    }
                }
            }
        }
    }
    property var targetEditBIP32: AppModel.newWalletInfo.sandbox
    QSetupEditBIP32PathFlow {
        id: editBip32Path
    }

    QConfirmYesNoPopup {
        id: _confirm
        title: STR.STR_QML_334
        contentText: {
            if (sandbox.isCreate) {
                return STR.STR_QML_1645
            } else if (sandbox.isRecovery) {
                return STR.STR_QML_1666
            } else if (sandbox.isReplace) {
                return STR.STR_QML_1379
            } else {
                return ""
            }
        }

        onConfirmNo: close()
        onConfirmYes: {
            close()
            if (sandbox.isCreate) {
                sandbox.deleteGroup()
            } else if (sandbox.isRecovery) {
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            } else if (sandbox.isReplace) {
                sandbox.deleteGroup()
            }
        }
    }
    QConfirmYesNoPopup{
        id:_confirmRemoveKey
        property var idx
        property string key: ""
        property string firstLine: ""
        title: STR.STR_QML_661
        contentText: STR.STR_QML_243
        onConfirmNo: close()
        onConfirmYes: {
            close()
            if (sandbox.walletType === NUNCHUCKTYPE.MINISCRIPT) {
                sandbox.removeKeyName(firstLine, key)
            } else {
                sandbox.removeKey(idx)
            }            
        }
    }

    QQrExportGroupWallet {
        id: qrcodeExportSandboxUrl
        label: STR.STR_QML_1636
        address: sandbox.url
    }
}
