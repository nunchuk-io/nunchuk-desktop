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
    // true when the just-added key is an inheritance key awaiting key-distribution choice (Setup 09D/10D)
    property bool showDistributionChoice: false
    // BUGFIX: this screen is loaded directly by the state machine, so "draftWallet" isn't inherited; declare it here.
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
            // KEEPKEY: reuses the Trezor screen (assumed same protocol family, no dedicated screen).
            case NUNCHUCKTYPE.ADD_KEEPKEY: return _Trezor
            default: return null
            }
        }
    }

    Component {
        id: _Ledger
        QScreenAddLedger {}
    }
    Component {
        id: _Trezor
        QScreenAddTrezor {}
    }
    Component {
        id: _Coldcard
        QScreenAddColdcard {}
    }
    Component {
        id: _BitBox
        QScreenAddBitBox {}
    }
    Component {
        id: _Jade
        QScreenAddJade {}
    }
    Component {
        id: _distributionChoice
        QKeyDistributionChoice {
            Component.onCompleted: {
                // currentSigner only has "tags" (array) at this stage; "tag" (singular) is set later, so infer it here.
                var tags = SignerManagement.currentSigner.tags !== undefined ? SignerManagement.currentSigner.tags : []
                var tag = ""
                for (var i = 0; i < tags.length; i++) {
                    if (tags[i] !== "INHERITANCE") { tag = tags[i]; break }
                }
                refresh(tag)
            }
            onPrevClicked: showDistributionChoice = false
            onDistributionChosen: function(claimOptions) {
                // BUGFIX: used to navigate to backup flow even when the API call failed; now blocks on failure
                // (backend already shows its own error toast, so no extra toast is added here).
                if (!draftWallet.requestSetClaimOptions(claimOptions)) {
                    return
                }
                // BUGFIX: requestSetClaimOptions doesn't update dashInfo.keys[].claim_options locally; refresh so
                // onBackupClicked routes correctly next time.
                GroupWallet.refresh()
                showDistributionChoice = false
                var xfp = SignerManagement.currentSigner.xfp
                var hasSeed = claimOptions.indexOf("SEED_PHRASE") !== -1
                var hasEncrypted = claimOptions.indexOf("ENCRYPTED_BACKUP") !== -1
                // BUGFIX: always go through the Verify-your-backups checklist (Setup 12c), for 1 or 2
                // options, not just "Do both" - it shows only the row(s) matching claim_options.
                if (hasSeed || hasEncrypted) {
                    _verifyBothBackups.open2(xfp, signerTag, claimOptions)
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
    function doneOrTryAgainAddHardwareKey(isSuccess) {
        var isNormalFlow = SignerManagement.currentSigner.wallet_type !== "MINISCRIPT"
        var is_inheritance = GroupWallet.dashboardInfo.isInheritance()
        if (isNormalFlow) {
            if (isSuccess) {
                if (is_inheritance) {
                    showDistributionChoice = true
                } else {
                    closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                    AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG);
                }
            }
            else {
                GroupWallet.refresh()
                GroupWallet.dashboardInfo.requestShowLetAddYourKeys();
            }
        } else {
            var xfp = SignerManagement.currentSigner.xfp
            if (GroupWallet.dashboardInfo.enoughKeyAdded(xfp)) {
                if (is_inheritance) {
                    showDistributionChoice = true
                } else {
                    closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                    AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG);
                }
            }
            else {
                var onlyUseForClaimBanner = SignerManagement.currentSigner.onlyUseForClaimBanner !== undefined && SignerManagement.currentSigner.onlyUseForClaimBanner
                if (onlyUseForClaimBanner) {
                    closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                }
                else {
                    GroupWallet.refresh()
                    GroupWallet.dashboardInfo.requestShowLetAddYourKeys();
                }
            }
        }
    }
}
