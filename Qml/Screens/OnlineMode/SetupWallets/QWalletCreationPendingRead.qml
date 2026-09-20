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
import QtQuick
// Removed unused imports: QtQuick.Controls, QtGraphicalEffects, HMIEVENTS, EWARNING, and Chats
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../Components/customizes/Popups"
import "../../OnlineMode/AddHardwareKeys"
import "TimeLocks"
import "../../../../localization/STR_QML.js" as STR


QOnScreenContentTypeB {
    id: _onScreen
    property var dashInfo: GroupWallet.dashboardInfo    
    property bool isKeyHolderLimited: dashInfo.myRole === "KEYHOLDER_LIMITED"
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_938
    onCloseClicked: closeScreen()
    content: Item {
        Row {
            anchors.fill: parent
            spacing: 36
            Item {
                width: 346
                height: parent.height
                Column {
                    anchors.fill: parent
                    spacing: 36
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_939.arg(dashInfo.mInfo).arg(dashInfo.nInfo).arg(dashInfo.mInfo === 2 ? STR.STR_QML_939_two : STR.STR_QML_939_three)
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QLato {
                        width: parent.width
                        text: {
                            if (dashInfo.allowInheritance)
                            {
                                var user = ClientController.user
                                if (dashInfo.mInfo === 2 && dashInfo.nInfo === 4) {
                                    if (dashInfo.isPremierGroup) {
                                        return STR.STR_QML_940_without
                                    } else {
                                        return STR.STR_QML_940_one
                                    }
                                }
                                else {
                                    if (dashInfo.isPremierGroup) {
                                        return STR.STR_QML_940_without
                                    } else {
                                        return STR.STR_QML_940_two
                                    }
                                }
                            }
                            else {
                                return STR.STR_QML_940_without
                            }
                        }
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
            Item {
                width: 346
                height: parent.height
                Item {
                    width: parent.width
                    height: 36
                    QLato {
                        width: 96
                        height: 36
                        text: STR.STR_QML_289
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QRefreshButtonA {
                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                        }
                        width: 70
                        height: 36
                        color: "transparent"
                        border.color: "transparent"
                        iconSize: 18
                        iconSpacing: 4
                        label: STR.STR_QML_652
                        fontPixelSize: 12
                        onButtonClicked: {
                            GroupWallet.refresh()
                            stopRefresh()
                        }
                    }
                }
                Rectangle {
                    anchors {
                        top: parent.top
                        topMargin: 36
                    }
                    color: "#FFFFFF"
                    border.color: "#FFEAEA"
                    radius: 12
                    width: 346
                    height: 456
                    Flickable {
                        anchors {
                            fill: parent
                            margins: 12
                        }
                        Column {
                            width: 322
                            spacing: 16
                            Repeater {
                                id: signers
                                model: GroupWallet.dashboardInfo.keys
                                QAddRequestKey {
                                    onInheritanceKeyClicked: {
                                        // BUGFIX: was missing the guide (02D/03D) and the "has" resume check;
                                        // now matches QWalletCreationPendingOnchainRead.qml.
                                        dashInfo.startAddKeyAtIndex(index)
                                        var has = SignerManagement.currentSigner.has !== undefined && SignerManagement.currentSigner.has
                                        if (!has) {
                                            _hardwareAddKey.key_index = modelData.key_index
                                            _inheritanceConfigureGuide.openGuide()
                                        } else {
                                            GroupWallet.addHardwareFromConfig(modelData.hwType, dashInfo.groupId, modelData.key_index)
                                            dashInfo.requestStartKeyCreate(modelData.tag, true)
                                        }
                                    }
                                    onHardwareClicked: {
                                        _hardwareAddKey.key_index = modelData.key_index
                                        // BUGFIX: was hardcoded false; any hardware slot can now be an
                                        // inheritance key per backend is_inheritance, so read it from modelData.
                                        _hardwareAddKey.isInheritance = modelData.is_inheritance !== undefined && modelData.is_inheritance
                                        _hardwareAddKey.open()
                                        dashInfo.startAddKeyAtIndex(index)
                                    }
                                    onSerkeyClicked: {
                                        _info.contentText = STR.STR_QML_962
                                        _info.open()
                                    }
                                    onBackupClicked: {
                                        // BUGFIX (NUN-10192): was unconditional legacy COLDCARD import; now
                                        // routes by claim_options like QWalletCreationPendingOnchainRead.qml.
                                        dashInfo.startAddKeyAtIndex(index)
                                        var claimOptions = modelData.claim_options !== undefined ? modelData.claim_options : []
                                        var hasSeed = claimOptions.indexOf("SEED_PHRASE") !== -1
                                        var hasEncrypted = claimOptions.indexOf("ENCRYPTED_BACKUP") !== -1
                                        if (hasSeed && hasEncrypted) {
                                            _verifyBothBackups.open2(modelData.xfp, modelData.tag)
                                        } else if (hasEncrypted) {
                                            var verifs = modelData.verifications !== undefined ? modelData.verifications : []
                                            var encryptedVerif = null
                                            for (var vi = 0; vi < verifs.length; vi++) {
                                                if (verifs[vi].verification_method === "ENCRYPTED_BACKUP") { encryptedVerif = verifs[vi]; break }
                                            }
                                            if (encryptedVerif === null) {
                                                _encryptedBackupFlow.startFlow(modelData.tag, modelData.xfp)
                                            } else {
                                                _encryptedBackupFlow.startVerifyOnly(modelData.tag, modelData.xfp)
                                            }
                                        } else if (hasSeed) {
                                            GroupWallet.qAddHardware = modelData.hwType
                                            _backupSeedPhraseFlow.startFlow()
                                        } else if (modelData.wallet_type === "MULTI_SIG") {
                                            // Setup 20dD: empty claim_options -> "Set up" reopens Key Distribution
                                            // Choice. ROLLOUT WARNING: needs backend NUN-10192 deployed in sync.
                                            _changeDistribution.hwType = modelData.hwType
                                            _changeDistribution.openFor(modelData.xfp, modelData.tag)
                                        } else {
                                            GroupWallet.qAddHardware = modelData.hwType
                                            _backupSeedPhraseFlow.startFlow()
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    onPrevClicked: closeScreen()
    bottomRight: Item{}

    QInheritanceConfigureGuide {
        id: _inheritanceConfigureGuide
        onNextClicked: {
            _hardwareAddKey.isInheritance = true
            _hardwareAddKey.open()
        }
    }

    QPopupHardwareAddKey {
        id: _hardwareAddKey
        isKeyHolderLimited: _onScreen.isKeyHolderLimited
        isMiniscript: dashInfo.walletType === "MINISCRIPT"
        onNextClicked: {
            dashInfo.requestStartKeyCreate(hardware)
        }
    }

    QPopupInfo{
        id:_info
        contentText: STR.STR_QML_961
    }

    function importEncryptedBackup(xfp, file) {
        var _input = {
            type: "import-encrypted-backup",
            fingerPrint: xfp,
            currentFile: file
        }
        dashInfo.requestBackupColdcard(_input)
    }

    QPopupImportColdcardBackup {
        id: _importColdcardBackup
    }

    QBackupSeedPhraseFlow {
        id: _backupSeedPhraseFlow
    }
    QEncryptedBackupFlow {
        id: _encryptedBackupFlow
    }
    QVerifyBothBackups {
        id: _verifyBothBackups
        onChangeShareMethod: {
            // Same as QWalletCreationPendingOnchainRead.qml: reopen Key Distribution Choice, no dead-end.
            var idx = findKeyIndexByXfp(_verifyBothBackups.xfp)
            if (idx < 0) return
            _changeDistribution.hwType = dashInfo.keys[idx].hwType
            _changeDistribution.openFor(_verifyBothBackups.xfp, _verifyBothBackups.signerTag)
        }
    }

    // Looks up a key's index in dashInfo.keys by xfp, for contexts that only have xfp/tag, not the Repeater index.
    function findKeyIndexByXfp(xfp) {
        var ks = dashInfo.keys
        for (var i = 0; i < ks.length; i++) {
            if (ks[i].xfp === xfp) return i
        }
        return -1
    }

    // "Change how you share this key" popup - wraps QKeyDistributionChoice in a QPopupOverlayScreen,
    // same pattern as QWalletCreationPendingOnchainRead.qml.
    QPopupOverlayScreen {
        id: _changeDistribution
        property string xfp: ""
        property string signerTag: ""
        property int hwType: -1
        content: _changeDistributionComp
        Component {
            id: _changeDistributionComp
            QKeyDistributionChoice {
                onDistributionChosen: function(claimOptions) {
                    var idx = findKeyIndexByXfp(_changeDistribution.xfp)
                    if (idx < 0) { _changeDistribution.close(); return }
                    dashInfo.startAddKeyAtIndex(idx)
                    // BUGFIX (like SCR_ADD_HARDWARE.qml): check the result before closing/navigating on.
                    if (!(GroupWallet.qIsByzantine ? GroupWallet : UserWallet).requestSetClaimOptions(claimOptions)) {
                        return
                    }
                    // BUGFIX: missing refresh, same as SCR_ADD_HARDWARE.qml.
                    GroupWallet.refresh()
                    _changeDistribution.close()
                    var hasSeed = claimOptions.indexOf("SEED_PHRASE") !== -1
                    var hasEncrypted = claimOptions.indexOf("ENCRYPTED_BACKUP") !== -1
                    if (hasSeed && hasEncrypted) {
                        _verifyBothBackups.open2(_changeDistribution.xfp, _changeDistribution.signerTag)
                    } else if (hasEncrypted) {
                        _encryptedBackupFlow.startFlow(_changeDistribution.signerTag, _changeDistribution.xfp)
                    } else if (hasSeed) {
                        GroupWallet.qAddHardware = _changeDistribution.hwType
                        _backupSeedPhraseFlow.startFlow()
                    }
                }
            }
        }
        function openFor(keyXfp, tag) {
            xfp = keyXfp
            signerTag = tag
            open()
            // ids inside Component {} aren't reachable from outside; use itemInfo (Loader.item) instead.
            if (itemInfo) itemInfo.refresh(tag)
        }
    }
}
