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
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import NUNCHUCKTYPE 1.0
import EWARNING 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../OnlineMode/AddHardwareKeys"
import "../OnlineMode/SetupWallets"
import "../OnlineMode/SetupWallets/TimeLocks"
import "../../../localization/STR_QML.js" as STR

QScreen {
    // BUGFIX: this "reuse existing key" screen lacked showDistributionChoice/is_inheritance handling that
    // SCR_ADD_HARDWARE.qml already has, so reusing a key as inheritance key was a dead end. Mirrored here.
    property bool showDistributionChoice: false
    property var draftWallet: GroupWallet.qIsByzantine ? GroupWallet : UserWallet

    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: {
            if (showDistributionChoice) return _distributionChoice
            var hardwareType = SignerManagement.currentSigner.hwType
            switch(hardwareType) {
            case NUNCHUCKTYPE.ADD_LEDGER: return _Ledger
            case NUNCHUCKTYPE.ADD_TREZOR: return _Trezor
            case NUNCHUCKTYPE.ADD_COLDCARD: return _Coldcard
            case NUNCHUCKTYPE.ADD_BITBOX: return _BitBox
            case NUNCHUCKTYPE.ADD_JADE: return _Jade
            // BUGFIX: KEEPKEY case was missing here (only added to the other 2 sibling screens). Reuses Trezor flow.
            case NUNCHUCKTYPE.ADD_KEEPKEY: return _Trezor
            default: return null
            }
        }
    }
    Component {
        id: _Ledger
        QScreenAddLedgerExist {}
    }
    Component {
        id: _Trezor
        QScreenAddTrezorExist {}
    }
    Component {
        id: _Coldcard
        QScreenAddColdcardExist {}
    }
    Component {
        id: _BitBox
        QScreenAddBitBoxExist {}
    }
    Component {
        id: _Jade
        QScreenAddJadeExist {}
    }
    Component {
        id: _distributionChoice
        QKeyDistributionChoice {
            Component.onCompleted: {
                // Same as SCR_ADD_HARDWARE.qml: currentSigner only has "tags" (array) at this stage.
                var tags = SignerManagement.currentSigner.tags !== undefined ? SignerManagement.currentSigner.tags : []
                var tag = ""
                for (var i = 0; i < tags.length; i++) {
                    if (tags[i] !== "INHERITANCE") { tag = tags[i]; break }
                }
                refresh(tag)
            }
            onPrevClicked: showDistributionChoice = false
            onDistributionChosen: function(claimOptions) {
                if (!draftWallet.requestSetClaimOptions(claimOptions)) {
                    return
                }
                GroupWallet.refresh()
                showDistributionChoice = false
                var xfp = SignerManagement.currentSigner.xfp
                var hasSeed = claimOptions.indexOf("SEED_PHRASE") !== -1
                var hasEncrypted = claimOptions.indexOf("ENCRYPTED_BACKUP") !== -1
                if (hasSeed && hasEncrypted) {
                    _verifyBothBackups.open2(xfp, signerTag)
                } else if (hasSeed) {
                    _backupSeedPhraseFlow.startFlow()
                } else if (hasEncrypted) {
                    _encryptedBackupFlow.startFlow(signerTag, xfp)
                } else {
                    closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                    AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG);
                }
            }
        }
    }
    QBackupSeedPhraseFlow {
        id: _backupSeedPhraseFlow
    }
    QEncryptedBackupFlow {
        id: _encryptedBackupFlow
    }
    QVerifyBothBackups {
        id: _verifyBothBackups
        onChangeShareMethod: showDistributionChoice = true
    }
    function isFlowClamOrAddKeyClaim() {
        var onlyUseForClaimBanner = SignerManagement.currentSigner.onlyUseForClaimBanner !== undefined && SignerManagement.currentSigner.onlyUseForClaimBanner // Add Key From Claim Banner
        var onlyUseForClaim = SignerManagement.currentSigner.onlyUseForClaim !== undefined && SignerManagement.currentSigner.onlyUseForClaim // Claim Flow
        return onlyUseForClaimBanner || onlyUseForClaim
    }
    function doneAddHardwareKey() {
        // BUGFIX: this function ignored is_inheritance, so reusing a key as inheritance key here closed
        // like a normal key, never prompting Key Distribution Choice. Synced with SCR_ADD_HARDWARE.qml,
        // including the MINISCRIPT branch.
        var isNormalFlow = SignerManagement.currentSigner.wallet_type !== "MINISCRIPT"
        var is_inheritance = GroupWallet.dashboardInfo.isInheritance()
        if (isNormalFlow) {
            if (is_inheritance) {
                showDistributionChoice = true
            } else {
                AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG);
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
        } else {
            var xfp = SignerManagement.currentSigner.xfp
            if (GroupWallet.dashboardInfo.enoughKeyAdded(xfp)) {
                if (is_inheritance) {
                    showDistributionChoice = true
                } else {
                    AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG);
                    closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                }
            } else {
                var onlyUseForClaimBanner = SignerManagement.currentSigner.onlyUseForClaimBanner !== undefined && SignerManagement.currentSigner.onlyUseForClaimBanner
                if (onlyUseForClaimBanner) {
                    closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                } else {
                    GroupWallet.refresh()
                    GroupWallet.dashboardInfo.requestShowLetAddYourKeys();
                }
            }
        }
    }
}
