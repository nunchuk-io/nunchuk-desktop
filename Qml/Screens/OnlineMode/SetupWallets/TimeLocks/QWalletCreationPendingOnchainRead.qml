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
import Features.Draftwallets.OnChain.ViewModels 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../Components/customizes/Signers"
import "../../../../Components/customizes/Popups"
import "../../../OnlineMode/AddHardwareKeys"
import "../../../OnlineMode/SetupWallets"
import "../../../OnlineMode/SetupWallets/TimeLocks"
import "../../../../../localization/STR_QML.js" as STR


QOnScreenContentTypeB {
    id: _onScreen
    property var dashInfo: GroupWallet.dashboardInfo
    property bool isKeyHolderLimited: dashInfo.myRole === "KEYHOLDER_LIMITED"
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
            return "qrc:/Images/Images/inheritance-illustration-2-of-4-and-1-of-3.svg"
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
                    anchors.fill: parent
                    contentHeight: _contentColumn.height
                    clip: true
                    ScrollBar.vertical: QScrollBar { }
                    Column {
                        id: _contentColumn
                        width: parent.width - 8  // leave room for QScrollBar (8px) — was 346
                        spacing: 4
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
                                    onInheritanceKeyClicked: {
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
                                        dashInfo.startAddKeyAtIndex(index)
                                        var has = SignerManagement.currentSigner.has !== undefined && SignerManagement.currentSigner.has
                                        if (!has) {
                                            _hardwareAddKey.key_index = modelData.key_index
                                            // BUGFIX: was hardcoded false; any hardware slot can now be an
                                            // inheritance key per backend is_inheritance, so read it from modelData.
                                            _hardwareAddKey.isInheritance = modelData.is_inheritance !== undefined && modelData.is_inheritance
                                            _hardwareAddKey.open()
                                        } else {
                                            GroupWallet.addHardwareFromConfig(modelData.hwType, dashInfo.groupId, modelData.key_index)
                                            dashInfo.requestStartKeyCreate(modelData.tag, true)
                                        }
                                    }
                                    onSerkeyClicked: {
                                        _info.contentText = STR.STR_QML_962
                                        _info.open()
                                    }
                                    onBackupClicked: {
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
    onPrevClicked: closeScreen()
    bottomRight: Item{}
    QmlUtils {
        id: utils
    }
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

    LetConfigureYourWalletViewModel {
        id: vm
    }
}
