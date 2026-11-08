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
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import EWARNING 1.0
import Features.Draftwallets.OnChain.ViewModels 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../Components/customizes/Signers"
import "../../../../Components/customizes/Popups"
import "../../../../Components/customizes/Chats"
import "../../../OnlineMode/AddHardwareKeys"
import "../../../OnlineMode/SetupWallets"
import "../../../OnlineMode/SetupWallets/TimeLocks"
import "../../../../../localization/STR_QML.js" as STR


QOnScreenContentTypeB {
    id: _onScreen
    property var dashInfo: GroupWallet.dashboardInfo
    property bool isKeyHolderLimited: dashInfo.myRole === "KEYHOLDER_LIMITED"
    readonly property bool isConfigurable: dashInfo.isConfigurablePolicy === true
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1940
    property var user: ClientController.user
    onCloseClicked: closeScreen()
    readonly property var hb_description_map: [
        {height: 40,  description: STR.STR_QML_1889 },
        {height: 40,  description: STR.STR_QML_1890 + STR.STR_QML_2061 },
    ]
    readonly property var hb_premier_description_map: [
        {height: 40,  description: STR.STR_QML_1892 },
        {height: 40,  description: STR.STR_QML_1893 + STR.STR_QML_2061},
    ]
    readonly property var free_description_map: [
        {height: 40,  description: STR.STR_QML_1892 },
        {height: 40,  description: STR.STR_QML_1893 + STR.STR_QML_2061},
    ]
    function guideImageSource() {
        var m = dashInfo.mInfo
        var n = dashInfo.nInfo
        var allowInheritance = dashInfo.allowInheritance
        if (m === 2 && n === 4) {
            return isConfigurable ? "qrc:/Images/Images/onchain_2_of_4_left_panel.svg"
                                  : "qrc:/Images/Images/inheritance-illustration-2-of-4-and-1-of-3.svg"
        } else if (m === 3 && n === 5) {
            return allowInheritance ? "qrc:/Images/Images/inheritance-illustration-3-of-5-and-2-of-4.svg"
                                    : "qrc:/Images/Images/inheritance-illustration-3-of-5-and-2-of-4-no-inheritance.svg"
        } else {
            // Default fallback
            return "qrc:/Images/Images/inheritance-illustration-3-of-5-and-2-of-4-no-inheritance.svg"
        }
    }
    function guideTextMap() {
        var m = dashInfo.mInfo
        var n = dashInfo.nInfo
        var allowInheritance = dashInfo.allowInheritance
        if (m === 2 && n === 4) {
            return hb_description_map
        } else if (m === 3 && n === 5) {
            return hb_premier_description_map
        } else {
            // Default fallback
            return free_description_map
        }
    }
    content: Item {
        Row {
            anchors.fill: parent
            spacing: 36
            Rectangle {
                width: 346
                height: 512
                radius: 24
                color: "#D0E2FF"
                QSvgImage {
                    width: 346
                    height: width * (implicitHeight / implicitWidth)
                    anchors.verticalCenter: parent.verticalCenter
                    source: guideImageSource()
                }
            }
            Item {
                width: 346
                height: parent.height
                Flickable {
                    id: _flick
                    anchors.fill: parent
                    contentHeight: _contentColumn.height
                    clip: true
                    ScrollBar.vertical: QScrollBar { }
                    Column {
                        id: _contentColumn
                        width: parent.width - 8  // leave room for QScrollBar (8px) — was 346
                        spacing: 4
                        QOnChainConfigurablePolicy {
                            id: _configurablePolicy
                            width: parent.width
                            visible: _onScreen.isConfigurable
                            dashInfo: _onScreen.dashInfo
                            isKeyHolderLimited: _onScreen.isKeyHolderLimited
                            primaryDateText: vm.valueDate.length > 0 ? qsTr("%1 %2").arg(vm.valueDate).arg(vm.valueTime) : ""
                            primarySubText: vm.isShowBlockHeight ? qsTr("%1 %2").arg(utils.formatAmount(qsTr("%1").arg(vm.blockHeight))).arg(STR.STR_QML_188) : ""
                            onKeyInheritanceClicked: function(key) { handleInheritanceKey(key.flat_index, key) }
                            onKeyHardwareClicked: function(key) { handleHardwareKey(key.flat_index, key) }
                            onKeyServerClicked: function(key) { handleServerKey() }
                            onKeyBackupClicked: function(key) {
                                if (key.section === "PRIMARY" && key.primary_customized !== true) {
                                    _verifyBackups.open()
                                } else {
                                    handleBackupKey(key.flat_index, key)
                                }
                            }
                            onKeyChangeClicked: function(key) {
                                dashInfo.startChangeKeyAtIndex(key.flat_index)
                                _hardwareAddKey.key_index = key.key_index
                                _hardwareAddKey.isInheritance = key.is_inheritance === true
                                var reusable = key.single_account === true ? dashInfo.reusableKeys(key.flat_index) : []
                                if (reusable.length > 0) {
                                    _chooseKey.slot = key
                                    _chooseKey.open()
                                    if (_chooseKey.itemInfo) {
                                        _chooseKey.itemInfo.selectedXfp = ""
                                        _chooseKey.itemInfo.keys = reusable
                                    }
                                } else {
                                    _deviceSafety.open()
                                }
                            }
                            onRefreshClicked: GroupWallet.refresh()
                            onPrimaryTimelockClicked: vm.onConfigureFallbackClicked(-1)
                            onCustomizePrimaryClicked: {
                                if (dashInfo.customizeIntroSeen()) {
                                    openCustomizePrimary()
                                } else {
                                    dashInfo.markCustomizeIntroSeen()
                                    _customizeIntro.open()
                                }
                            }
                            onAddFallbackClicked: _fallbackIntro.open()
                            onEditFallbackClicked: function(fallback) {
                                openPolicy(fallback.index, false, fallback.m, fallback.n)
                            }
                            onRemoveFallbackClicked: function(fallback) {
                                _confirmRemoveFallback.fallbackIndex = fallback.index
                                _confirmRemoveFallback.title = STR.STR_QML_2358.arg(fallback.index + 1)
                                var hasLater = fallback.index < dashInfo.fallbackPolicies.length - 1
                                _confirmRemoveFallback.contentText = fallback.n === 1 ? (hasLater ? STR.STR_QML_2569 : STR.STR_QML_2568)
                                                                                      : (hasLater ? STR.STR_QML_2431 : STR.STR_QML_2430)
                                _confirmRemoveFallback.open()
                            }
                            onFallbackTimelockClicked: function(fallback) { vm.onConfigureFallbackClicked(fallback.index) }
                        }
                        Column {
                            width: parent.width
                            spacing: 4
                            visible: !_onScreen.isConfigurable
                            QLato {
                                width: parent.width
                                text: guideTextMap()[0].description
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 20
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                            Item { width: parent.width; height: 12 }
                            Column {
                                width: parent.width
                                spacing: 16
                                Repeater {
                                    id: signers
                                    model: dashInfo.keys
                                    QAddRequestKey {
                                        width: parent.width  // = _contentColumn.width - 8 = 338
                                        onInheritanceKeyClicked: handleInheritanceKey(index, modelData)
                                        onHardwareClicked: handleHardwareKey(index, modelData)
                                        onSerkeyClicked: handleServerKey()
                                        onBackupClicked: handleBackupKey(index, modelData)
                                    }
                                }
                            }
                            Item {
                                width: parent.width
                                height: 48
                                QRefreshButtonA {
                                    anchors {
                                        horizontalCenter: parent.horizontalCenter
                                        top: parent.top
                                        topMargin: 4
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
                            QLato {
                                width: parent.width
                                text: guideTextMap()[1].description
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 20
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                            Item { width: parent.width; height: 12 }
                            Loader {
                                width: parent.width
                                height: 72
                                sourceComponent: if (vm.isShowBlockHeight) return _blockHeightLock
                                                else if (vm.valueDate.length > 0) return _timeLock
                                                else return _timeLockEmpty
                                Component {
                                    id: _blockHeightLock
                                    Rectangle {
                                        width: parent.width  // = Loader.width = _contentColumn.width - 8 = 338
                                        height: 72
                                        radius: 8
                                        color: "#A7F0BA"
                                        Row {
                                            anchors {
                                                fill: parent
                                                margins: 12
                                            }
                                            spacing: 12
                                            QBadge {
                                                width: 48
                                                height: 48
                                                radius: 48
                                                iconSize: 24
                                                icon: "qrc:/Images/Images/Timer.svg"
                                                anchors.verticalCenter: parent.verticalCenter
                                                color: "#F5F5F5"
                                            }
                                            Column {
                                                width: 150
                                                height: 40
                                                anchors.verticalCenter: parent.verticalCenter
                                                spacing: 4
                                                QLato {
                                                    width: 150
                                                    height: 20
                                                    text: qsTr("Est. %1 %2").arg(vm.valueDate).arg(vm.valueTime)
                                                    horizontalAlignment: Text.AlignLeft
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                                QLato {
                                                    width: 150
                                                    height: 16
                                                    font.pixelSize: 12
                                                    text: qsTr("%1 %2").arg(utils.formatAmount(qsTr("%1").arg(vm.blockHeight))).arg(STR.STR_QML_188)
                                                    horizontalAlignment: Text.AlignLeft
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                            }
                                        }
                                        QTextButton {
                                            anchors {
                                                verticalCenter: parent.verticalCenter
                                                right: parent.right
                                                rightMargin: 12
                                            }
                                            width: label.paintedWidth + 2*20
                                            height: 36
                                            type: eTypeB
                                            label.text: STR.STR_QML_1413
                                            label.font.pixelSize: 16
                                            onButtonClicked: {
                                                vm.onConfigureClicked()
                                            }
                                        }
                                    }
                                }

                                Component {
                                    id: _timeLock
                                    Rectangle {
                                        width: parent.width  // = Loader.width = _contentColumn.width - 8 = 338
                                        height: 72
                                        radius: 8
                                        color: "#A7F0BA"
                                        Row {
                                            anchors {
                                                fill: parent
                                                margins: 12
                                            }
                                            spacing: 12
                                            QBadge {
                                                width: 48
                                                height: 48
                                                radius: 48
                                                iconSize: 24
                                                icon: "qrc:/Images/Images/Timer.svg"
                                                anchors.verticalCenter: parent.verticalCenter
                                                color: "#F5F5F5"
                                            }
                                            Column {
                                                width: 150
                                                height: 48
                                                anchors.verticalCenter: parent.verticalCenter
                                                spacing: 4
                                                QLato {
                                                    width: 150
                                                    height: 20
                                                    text: STR.STR_QML_1988
                                                    horizontalAlignment: Text.AlignLeft
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                                QLato {
                                                    width: 150
                                                    height: 20
                                                    text: qsTr("%1 %2").arg(vm.valueDate).arg(vm.valueTime)
                                                    horizontalAlignment: Text.AlignLeft
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                            }
                                        }
                                        QTextButton {
                                            anchors {
                                                verticalCenter: parent.verticalCenter
                                                right: parent.right
                                                rightMargin: 12
                                            }
                                            width: label.paintedWidth + 2*20
                                            height: 36
                                            type: eTypeB
                                            label.text: STR.STR_QML_1413
                                            label.font.pixelSize: 16
                                            onButtonClicked: {
                                                vm.onConfigureClicked()
                                            }
                                        }
                                    }
                                }
                                Component {
                                    id: _timeLockEmpty
                                    QDashRectangle {
                                        width: 346
                                        height: 72
                                        radius: 8
                                        borderWitdh: 2
                                        borderColor: "#031F2B"
                                        Row {
                                            anchors {
                                                fill: parent
                                                margins: 12
                                            }
                                            spacing: 12
                                            QBadge {
                                                width: 48
                                                height: 48
                                                radius: 48
                                                iconSize: 24
                                                icon: "qrc:/Images/Images/Timer.svg"
                                                anchors.verticalCenter: parent.verticalCenter
                                                color: "#F5F5F5"
                                            }
                                            QLato {
                                                width: 150
                                                height: 28
                                                text: STR.STR_QML_1896
                                                horizontalAlignment: Text.AlignLeft
                                                verticalAlignment: Text.AlignVCenter
                                                anchors.verticalCenter: parent.verticalCenter
                                            }
                                        }
                                        QTextButton {
                                            anchors {
                                                verticalCenter: parent.verticalCenter
                                                right: parent.right
                                                rightMargin: 12
                                            }
                                            width: label.paintedWidth + 2*20
                                            height: 36
                                            type: eTypeB
                                            label.text: STR.STR_QML_958
                                            label.font.pixelSize: 16
                                            onButtonClicked: {
                                                vm.onConfigureClicked()
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
    }
    function handleInheritanceKey(index, modelData) {
        dashInfo.startAddKeyAtIndex(index)
        var has = SignerManagement.currentSigner.has !== undefined && SignerManagement.currentSigner.has
        if (!has) {
            _hardwareAddKey.key_index = modelData.key_index
            var canReuse = _onScreen.isConfigurable && (modelData.section === "FALLBACK" || (modelData.section === "PRIMARY" && modelData.primary_customized === true))
            var reusable = canReuse ? dashInfo.reusableKeys(index) : []
            if (reusable.length > 0) {
                _chooseKey.slot = modelData
                _chooseKey.open()
                if (_chooseKey.itemInfo) {
                    _chooseKey.itemInfo.selectedXfp = ""
                    _chooseKey.itemInfo.keys = reusable
                }
            } else {
                openNewKeyGuide(modelData)
            }
        } else {
            GroupWallet.addHardwareFromConfig(modelData.hwType, dashInfo.groupId, modelData.key_index)
            dashInfo.requestStartKeyCreate(modelData.tag, true)
        }
    }
    function requestRestartWizard() {
    }

    function openNewKeyGuide(modelData) {
        _inheritanceConfigureGuide.fallbackNumber = modelData.section === "FALLBACK" ? modelData.fallback_index + 1 : 0
        _inheritanceConfigureGuide.openGuide()
    }
    readonly property var draftWalletObj: GroupWallet.qIsByzantine ? GroupWallet : UserWallet
    function handleHardwareKey(index, modelData) {
        dashInfo.startAddKeyAtIndex(index)
        var has = SignerManagement.currentSigner.has !== undefined && SignerManagement.currentSigner.has
        if (!has) {
            _hardwareAddKey.key_index = modelData.key_index
            // BUGFIX: was hardcoded false; any hardware slot can now be an
            // inheritance key per backend is_inheritance, so read it from modelData.
            _hardwareAddKey.isInheritance = modelData.is_inheritance !== undefined && modelData.is_inheritance
            if (_onScreen.isConfigurable && modelData.section !== "SPENDING") {
                _deviceSafety.open()
            } else {
                _hardwareAddKey.open()
            }
        } else {
            GroupWallet.addHardwareFromConfig(modelData.hwType, dashInfo.groupId, modelData.key_index)
            dashInfo.requestStartKeyCreate(modelData.tag, true)
        }
    }
    function handleServerKey() {
        _info.contentText = STR.STR_QML_962
        _info.open()
    }
    function handleBackupKey(index, modelData) {
        dashInfo.startAddKeyAtIndex(index)
        // BUGFIX: check wallet_type FIRST - claim_options is an off-chain
        // (MULTI_SIG, NUN-10192) concept only; checking it before wallet_type
        // risked routing an on-chain MINISCRIPT key into the off-chain
        // verify-backups/distribution-choice screens if claim_options were
        // ever non-empty for it. MINISCRIPT must always keep the pre-existing
        // on-chain flow below, untouched by the off-chain feature.
        if (modelData.wallet_type === "MULTI_SIG") {
            // Route by claim_options (NUN-10192) chosen in Key Distribution
            // Choice. BUGFIX: single-option keys (seed-only/encrypted-only)
            // used to skip straight into their own flow; now every configured
            // key (1 or 2 options) always goes through the Verify-your-backups
            // checklist (Setup 12c), which shows only the row(s) matching
            // claim_options and picks startFlow()/startVerifyOnly() itself
            // based on upload state.
            var claimOptions = modelData.claim_options !== undefined ? modelData.claim_options : []
            var hasSeed = claimOptions.indexOf("SEED_PHRASE") !== -1
            var hasEncrypted = claimOptions.indexOf("ENCRYPTED_BACKUP") !== -1
            if (hasSeed || hasEncrypted) {
                _verifyBothBackups.open2(modelData.xfp, modelData.tag, claimOptions)
            } else {
                // Setup 20dD (NUN-10192): empty claim_options reopens Key
                // Distribution Choice instead of the old Coldcard-import flow.
                // ROLLOUT WARNING: NUN-10192 is still "To Do" on backend - until
                // deployed, claim_options is empty for all existing inheritance
                // MULTI_SIG keys, so this FE must ship in sync with the backend.
                _changeDistribution.hwType = modelData.hwType
                _changeDistribution.openFor(modelData.xfp, modelData.tag)
            }
        } else {
            GroupWallet.qAddHardware = modelData.hwType
            _backupSeedPhraseFlow.startFlow()
        }
    }
    onPrevClicked: closeScreen()
    bottomLeft: QTextButton {
        width: 80
        height: 48
        label.text: _onScreen.isConfigurable ? STR.STR_QML_059 : STR.STR_QML_245
        label.font.pixelSize: 16
        type: eTypeF
        onButtonClicked: _onScreen.prevClicked()
    }
    bottomRight: QTextButton {
        visible: _onScreen.isConfigurable
        width: 99
        height: 48
        label.text: STR.STR_QML_265
        label.font.pixelSize: 16
        type: eTypeE
        enabled: dashInfo.draftReadyToContinue === true
        onButtonClicked: {
            GroupWallet.refresh()
            closeScreen()
        }
    }
    QmlUtils {
        id: utils
    }
    QInheritanceConfigureGuide {
        id: _inheritanceConfigureGuide
        isConfigurable: _onScreen.isConfigurable
        isCustomized: _onScreen.isConfigurable && dashInfo.primaryPolicy.is_customized === true
        onNextClicked: {
            _hardwareAddKey.isInheritance = true
            _hardwareAddKey.open()
        }
    }

    QPopupOverlayScreen {
        id: _deviceSafety
        content: _deviceSafetyComp
        Component {
            id: _deviceSafetyComp
            QBeforeYouUseThisDevice {
                onCloseClicked: _deviceSafety.close()
                onPrevClicked: _deviceSafety.close()
                onNextClicked: {
                    _deviceSafety.close()
                    _hardwareAddKey.open()
                }
            }
        }
    }

    QPopupHardwareAddKey {
        id: _hardwareAddKey
        isKeyHolderLimited: _onScreen.isKeyHolderLimited
        isMiniscript: dashInfo.walletType === "MINISCRIPT"
        onNextClicked: {
            _checkFirmware.hadwareTag = hardware
            _checkFirmware.open()
        }
    }

    QPopupCheckYourFirmware {
        id: _checkFirmware
        onNextClicked: {
            dashInfo.requestStartKeyCreate(hadwareTag)
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
            // BUGFIX: this handler was missing, making "Change how you share this key" a dead-end from
            // the dashboard. _verifyBothBackups only keeps xfp/tag, so look up the key by xfp first.
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

    // "Change how you share this key" popup, opened from the dashboard - wraps QKeyDistributionChoice in
    // a QPopupOverlayScreen like QEncryptedBackupFlow/QVerifyBothBackups already do in this file.
    QPopupOverlayScreen {
        id: _changeDistribution
        property string xfp: ""
        property string signerTag: ""
        property int hwType: -1
        // content points to a named Component (QPopupOverlayScreen convention), not an inline object literal.
        content: _changeDistributionComp
        Component {
            id: _changeDistributionComp
            QKeyDistributionChoice {
                onDistributionChosen: function(claimOptions) {
                    var idx = findKeyIndexByXfp(_changeDistribution.xfp)
                    if (idx < 0) { _changeDistribution.close(); return }
                    // NUN-10192: dropping ENCRYPTED_BACKUP from an already-uploaded key needs confirm-in-app
                    // first (moved here from QVerifyBothBackups.qml's link click).
                    if (claimOptions.indexOf("ENCRYPTED_BACKUP") === -1 && hasUploadedEncryptedBackup(_changeDistribution.xfp)) {
                        _confirmRemoveBackup.openWith(claimOptions)
                        return
                    }
                    applyDistributionChoice(idx, claimOptions)
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

    // Shared "apply" step for _changeDistribution, called directly or after _confirmRemoveBackup confirms.
    function applyDistributionChoice(idx, claimOptions) {
        dashInfo.startAddKeyAtIndex(idx)
        // BUGFIX (like SCR_ADD_HARDWARE.qml): check the result before closing/navigating on.
        if (!(GroupWallet.qIsByzantine ? GroupWallet : UserWallet).requestSetClaimOptions(claimOptions)) {
            return
        }
        // BUGFIX: missing refresh, same as SCR_ADD_HARDWARE.qml.
        GroupWallet.refresh()
        _changeDistribution.close()
        // BUGFIX: always go through the Verify-your-backups checklist (Setup 12c), same as onBackupClicked.
        var hasSeed = claimOptions.indexOf("SEED_PHRASE") !== -1
        var hasEncrypted = claimOptions.indexOf("ENCRYPTED_BACKUP") !== -1
        if (hasSeed || hasEncrypted) {
            _verifyBothBackups.open2(_changeDistribution.xfp, _changeDistribution.signerTag, claimOptions)
        }
    }

    // True if this key already has an ENCRYPTED_BACKUP verifications[] entry (i.e. a file was uploaded).
    function hasUploadedEncryptedBackup(xfp) {
        var idx = findKeyIndexByXfp(xfp)
        if (idx < 0) return false
        var verifs = dashInfo.keys[idx].verifications !== undefined ? dashInfo.keys[idx].verifications : []
        for (var i = 0; i < verifs.length; i++) {
            if (verifs[i].verification_method === "ENCRYPTED_BACKUP") return true
        }
        return false
    }

    QPopupConfirmRemoveBackup {
        id: _confirmRemoveBackup
        onConfirmed: function(claimOptions) {
            var idx = findKeyIndexByXfp(_changeDistribution.xfp)
            if (idx < 0) return
            applyDistributionChoice(idx, claimOptions)
        }
    }

    Timer {
        id: _scrollToLastKey
        interval: 100
        onTriggered: {
            var key = SignerManagement.currentSigner
            if (!_onScreen.isConfigurable || !key || key.section === undefined || key.section === "SPENDING") return
            var y = _configurablePolicy.sectionY(key.section, key.fallback_index)
            _flick.contentY = Math.max(0, Math.min(y, _flick.contentHeight - _flick.height))
        }
    }
    Component.onCompleted: {
        _scrollToLastKey.start()
        var pendingFallback = dashInfo.takePendingFallbackTimelock()
        if (pendingFallback >= 0) {
            Qt.callLater(function() { vm.onConfigureFallbackClicked(pendingFallback) })
        }
    }

    QIconButton {
        id: _moreButton
        visible: _onScreen.isConfigurable && !dashInfo.isLocked
                 && dashInfo.myRole !== "FACILITATOR_ADMIN" && dashInfo.myRole !== "KEYHOLDER_LIMITED"
        width: 48
        height: 48
        bgColor: "transparent"
        icon: "qrc:/Images/Images/more-horizontal-dark.svg"
        anchors {
            right: closebutton.left
            rightMargin: 8
            verticalCenter: closebutton.verticalCenter
        }
        onClicked: _setupMenu.popup(_moreButton, _moreButton.width - _setupMenu.width, _moreButton.height)
    }

    QOnChainSetupMenu {
        id: _setupMenu
        readonly property bool canCancelCustomization: dashInfo.primaryPolicy.is_customized === true
        items: [
            { title: STR.STR_QML_2375, subtitle: STR.STR_QML_2376, visible: canCancelCustomization },
            { title: STR.STR_QML_2451 },
            { title: STR.STR_QML_2452 }
        ]
        onItemClicked: function(index) {
            if (index === 0) {
                _confirmCancelCustomization.open()
            } else if (index === 1) {
                requestRestartWizard()
            } else if (index === 2) {
                closeScreen()
            }
        }
    }

    QConfirmYesNoPopup {
        id: _confirmCancelCustomization
        contentText: STR.STR_QML_2377
        onConfirmNo: close()
        onConfirmYes: {
            close()
            dashInfo.resetPrimary()
        }
    }

    function openCustomizePrimary(fromIntro) {
        var p = dashInfo.primaryPolicy
        openPolicy(-1, fromIntro === true, p.m !== undefined ? p.m : 2, p.n !== undefined ? p.n : 4)
    }

    function openPolicy(target, fromIntro, m, n) {
        _primaryPolicy.target = target
        _primaryPolicy.fromIntro = fromIntro
        _primaryPolicy.open()
        var item = _primaryPolicy.itemInfo
        if (!item) return
        if (target === -1) {
            item.titleText = STR.STR_QML_2391
            item.descriptionText = STR.STR_QML_2392
        } else {
            var number = target >= 0 ? target + 1 : dashInfo.fallbackPolicies.length + 1
            item.titleText = STR.STR_QML_2432.arg(number)
            item.descriptionText = STR.STR_QML_2392
        }
        item.setup(m, n)
    }

    QPopupOverlayScreen {
        id: _primaryPolicy
        property int target: -1
        property bool fromIntro: false
        content: _primaryPolicyComp
        Component {
            id: _primaryPolicyComp
            QPrimaryInheritancePolicy {
                onCloseClicked: _primaryPolicy.close()
                onPrevClicked: {
                    _primaryPolicy.close()
                    if (!_primaryPolicy.fromIntro) return
                    if (_primaryPolicy.target === -1) _customizeIntro.open()
                    else _fallbackIntro.open()
                }
                onSaved: function(m, n) {
                    _primaryPolicy.close()
                    if (_primaryPolicy.target === -1) dashInfo.customizePrimary(m, n)
                    else if (_primaryPolicy.target === -2) dashInfo.addFallback(m, n)
                    else dashInfo.editFallback(_primaryPolicy.target, m, n)
                }
            }
        }
    }

    QPopupOverlayScreen {
        id: _fallbackIntro
        content: _fallbackIntroComp
        Component {
            id: _fallbackIntroComp
            QCustomizeInheritanceIntro {
                titleText: STR.STR_QML_2354
                introText: STR.STR_QML_2417
                stepsHeading: STR.STR_QML_2418
                steps: [
                    { title: STR.STR_QML_2419, description: STR.STR_QML_2420 },
                    { title: STR.STR_QML_2421, description: STR.STR_QML_2422 },
                    { title: STR.STR_QML_2423, description: STR.STR_QML_2424 }
                ]
                footerText: ""
                infoTexts: [STR.STR_QML_2425, STR.STR_QML_2426]
                secondaryLabel: STR.STR_QML_2427
                primaryLabel: STR.STR_QML_2428
                onCloseClicked: _fallbackIntro.close()
                onPrevClicked: _fallbackIntro.close()
                onKeepCurrentClicked: {
                    _fallbackIntro.close()
                    openPolicy(-2, true, 1, 1)
                }
                onNextClicked: {
                    _fallbackIntro.close()
                    dashInfo.addFallback(1, 1)
                }
            }
        }
    }

    QPopupOverlayScreen {
        id: _customizeIntro
        content: _customizeIntroComp
        Component {
            id: _customizeIntroComp
            QCustomizeInheritanceIntro {
                onCloseClicked: _customizeIntro.close()
                onPrevClicked: _customizeIntro.close()
                onKeepCurrentClicked: _customizeIntro.close()
                onNextClicked: {
                    _customizeIntro.close()
                    openCustomizePrimary(true)
                }
            }
        }
    }

    QPopupOverlayScreen {
        id: _verifyBackups
        content: _verifyBackupsComp
        Component {
            id: _verifyBackupsComp
            QVerifyBackups {
                dashInfo: _onScreen.dashInfo
                onCloseClicked: _verifyBackups.close()
                onPrevClicked: _verifyBackups.close()
                onNextClicked: _verifyBackups.close()
                onVerifyKeyClicked: function(key) { handleBackupKey(key.flat_index, key) }
            }
        }
    }

    QPopupOverlayScreen {
        id: _chooseKey
        property var slot: ({})
        property var pendingKey: null
        content: _chooseKeyComp
        Component {
            id: _chooseKeyComp
            QChooseAKey {
                onCloseClicked: _chooseKey.close()
                onPrevClicked: _chooseKey.close()
                onAddNewKeyClicked: {
                    _chooseKey.close()
                    openNewKeyGuide(_chooseKey.slot)
                }
                onKeySelected: function(key) {
                    _chooseKey.pendingKey = key
                    draftWalletObj.addExistingKeyAsync(key.xfp)
                }
            }
        }
    }

    Connections {
        target: _onScreen.dashInfo
        function onDraftPolicyRequestFinished(ok, message) {
            if (!ok && message !== "") {
                AppModel.showToast(-1, message, EWARNING.EXCEPTION_MSG)
            }
        }
    }

    Connections {
        target: draftWalletObj
        function onExistingKeyResult(result, accountIndex, errorMsg) {
            var key = _chooseKey.pendingKey
            if (result === 1) {
                _chooseKey.close()
                GroupWallet.refresh()
                AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG)
                var slot = _chooseKey.slot
                if (slot && slot.section === "FALLBACK" && dashInfo.fallbackTimelockMissing(slot.fallback_index)) {
                    vm.onConfigureFallbackClicked(slot.fallback_index)
                }
            } else if (result === 2 && key) {
                _chooseKey.close()
                if (key.hwType >= 0) {
                    GroupWallet.addHardwareFromConfig(key.hwType, dashInfo.groupId, _chooseKey.slot.key_index, true)
                    _checkFirmware.hadwareTag = key.tag
                    _checkFirmware.open()
                } else {
                    _hardwareAddKey.isInheritance = true
                    _hardwareAddKey.open()
                }
            } else if (errorMsg !== "") {
                AppModel.showToast(-1, errorMsg, EWARNING.EXCEPTION_MSG)
            }
        }
    }

    QPopupInfoTwoButtons {
        id: _confirmRemoveFallback
        property int fallbackIndex: -1
        contentText: STR.STR_QML_2430
        isVertical: true
        labels: [STR.STR_QML_2429, STR.STR_QML_035]
        funcs: [
            function() { dashInfo.removeFallback(_confirmRemoveFallback.fallbackIndex) },
            function() {}
        ]
    }

    LetConfigureYourWalletViewModel {
        id: vm
    }
}
