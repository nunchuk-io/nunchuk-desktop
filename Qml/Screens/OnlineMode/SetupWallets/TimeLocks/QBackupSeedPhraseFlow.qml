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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../Components/customizes/Popups"
import "../../../../Components/customizes/QRCodes"
import "../../../../Components/customizes/Signers"
import "../../../OnlineMode/AddHardwareKeys"
import "../../../OnlineMode/SetupWallets"
import "../../../../../localization/STR_QML.js" as STR

QPopupOverlayScreen {
    id: _infoPopup
    signal nextClicked()
    property string selected_verify_option: "SELF_VERIFIED"
    // BUGFIX: backend error message for the failed-verify screen (Setup 13cD) - not the exact
    // "expected/actual XFP" the mockup shows, since no such structured field is returned by the API.
    property string verify_error_msg: ""
    // BUGFIX (real root cause of "Run command exit error!" on re-verify, confirmed): this used to
    // ALWAYS snapshot from the mutable global SignerManagement.currentSigner - fine for the 3 call
    // sites that call dashInfo.startAddKeyAtIndex(index) (which sets that global) right before
    // startFlow(), but QVerifyBothBackups.qml never sets that global at all (it threads its own
    // xfp/signerTag instead, same as QEncryptedBackupFlow.qml), so derivation_path (and possibly xfp)
    // read empty there, and hwi_.GetXpubAtPath(device, "") always exits with an error during the
    // re-add-device verify step. Now startFlow(key) accepts the actual key object explicitly
    // (QVerifyBothBackups.currentKey(), which carries a real derivation_path) and prefers it when
    // given; falls back to the global for the other 3 call sites that still call startFlow() bare.
    property string xfp: ""
    property string signerTag: ""
    property string signerName: ""
    property string signerType: ""
    property string derivationPath: ""
    // BUGFIX: this flow is shared - called bare (no key) from the on-chain MINISCRIPT replace-key
    // screens, and with an explicit key from QVerifyBothBackups.qml for off-chain claim_options.
    // verification_method ("SEED_PHRASE") only has meaning for an off-chain claim_options key; the
    // on-chain path must follow the old on-chain logic and not send it at all (empty), otherwise the
    // backend wrongly validates a MINISCRIPT key as an off-chain inheritance key ([400]).
    property string verificationMethod: ""
    // BUGFIX: tracks which caller started this flow, so verifyResult() below can follow old on-chain
    // logic exactly (silent close / toast-only on failure) instead of the new off-chain result screens.
    property bool isOffChain: false
    QScreenStateFlow {
        id: stateFlow
    }
    // BUGFIX: isOffChain now passed explicitly by caller instead of inferred from key, to avoid a race.
    function startFlow(key, offChain) {
        isOffChain = offChain === true
        if (isOffChain && !key) {
            // key not ready yet (refresh race) - retry and bail instead of falling to on-chain branch.
            GroupWallet.refresh()
            return
        }
        var k = isOffChain ? key : SignerManagement.currentSigner
        verificationMethod = isOffChain ? "SEED_PHRASE" : ""
        xfp = k.xfp !== undefined ? k.xfp : ""
        signerTag = k.tag !== undefined ? k.tag : ""
        signerName = k.name !== undefined ? k.name : ""
        signerType = k.type !== undefined ? k.type : ""
        derivationPath = k.derivation_path !== undefined ? k.derivation_path : ""
        // BUGFIX: the "re-add-the-stored-key" screen's QScreenAdd computes _HARDWARE_TAG/_HARDWARE_TYPE
        // (used by QAddKeyRefreshDevices to filter the scanned device list) from
        // draftWallet.qAddHardware - but this flow never set it, so it stayed at whatever a PRIOR,
        // unrelated "add hardware key" action last left it as. The device list then filtered by the
        // wrong vendor type (or an empty one), hiding every device a successful scan actually found -
        // "No devices available" even with the right hardware connected, for any vendor. Set it here
        // from the key's own hwType (same field QWalletCreationPendingRead.qml etc. use), falling back
        // to SignerManagement.currentSigner.hwType for the 3 call sites that call startFlow() bare.
        var hwType = k.hwType !== undefined ? k.hwType : SignerManagement.currentSigner.hwType
        var draft = GroupWallet.qIsByzantine ? GroupWallet : UserWallet
        draft.qAddHardware = hwType
        _infoPopup.open()
        stateFlow.setScreenFlow("backup-your-inheritance-key-seed-phrase")
    }

    readonly property var map_flow: [
        {screen: "backup-your-inheritance-key-seed-phrase",    screen_component: backup_your_inheritance_key_seed_phrase},
        {screen: "important-notice-about-passphrase",          screen_component: important_notice_about_passphrase},
        {screen: "important-notice-about-passphrase-guide",    screen_component: important_notice_about_passphrase_guide},
        {screen: "re-add-the-stored-key",                      screen_component: _re_add_the_stored_key},
        {screen: "result-restore-key",                         screen_component: _resultRestoreKey},
        {screen: "result-restore-key-failed",                  screen_component: _resultRestoreKeyFailed},
        {screen: "coldcard-via-file-screen",                   screen_component: coldcard_via_file_screen},
        {screen: "coldcard-via-qr-screen",                     screen_component: coldcard_via_qr_screen},
        {screen: "blockstream-jade-via-qr-screen",               screen_component: blockstream_jade_via_qr_screen}
    ]
    content: {
        var itemScreen = map_flow.find(function(e) {if (e.screen == stateFlow.screenFlow) return true; else return false})
        if (itemScreen) {
            return itemScreen.screen_component
        } else {
            _infoPopup.close()
            return null
        }
    }

    Component {
        id: backup_your_inheritance_key_seed_phrase
        QBackUpYourInheritanceKeySeedPhrase {
            onCloseClicked: _infoPopup.close()
            onPrevClicked: stateFlow.backScreen()
            onNextClicked: stateFlow.setScreenFlow("important-notice-about-passphrase")
        }
    }

    Component {
        id: important_notice_about_passphrase
        QScreenAdd {
            anchors.fill: parent
            QVerifyYourInheritanceKeySeedPhraseQuestion {
                onCloseClicked: _infoPopup.close()
                onPrevClicked: stateFlow.backScreen()
                onNextClicked: {
                    selected_verify_option = verify_option
                    if (verify_option === "SKIPPED_VERIFICATION") {
                        // BUGFIX: same as the Verify button below - must thread verificationMethod
                        // explicitly, otherwise this falls back to the C++ default "SEED_PHRASE" even
                        // for on-chain MINISCRIPT keys.
                        draftWallet.requestVerifySingleSigner(selected_verify_option, verificationMethod)
                    } else {
                        stateFlow.setScreenFlow("important-notice-about-passphrase-guide")
                    }
                }
            }
            Connections {
                target: draftWallet
                onVerifySingleSignerResult: function(result) {
                    if (result == 1) {
                        GroupWallet.refresh()
                        _infoPopup.close()
                    }
                }
            }
        }
    }

    Component {
        id: important_notice_about_passphrase_guide
        QVerifyYourInheritanceKeySeedPhraseGuide {
            onCloseClicked: _infoPopup.close()
            onPrevClicked: stateFlow.backScreen()
            onNextClicked: stateFlow.setScreenFlow("re-add-the-stored-key")
        }
    }
    // Off-chain (Setup 13bD/13cD) routes to the new result screens either way. On-chain must follow
    // the old on-chain logic: success still refreshes+closes silently; failure gets a short toast
    // only (old baseline showed nothing at all on failure - confirmed too silent, kept a toast here).
    function verifyResult(result, errorMsg) {
        if (result == 1) {
            GroupWallet.refresh()
            if (isOffChain) {
                stateFlow.setScreenFlow("result-restore-key")
            } else {
                _infoPopup.close()
            }
        } else if (isOffChain) {
            verify_error_msg = errorMsg !== undefined ? errorMsg : ""
            stateFlow.setScreenFlow("result-restore-key-failed")
        } else {
            AppModel.showToast(-1, errorMsg !== undefined && errorMsg !== "" ? errorMsg : STR.STR_QML_2329, EWARNING.ERROR_MSG)
        }
    }
    Component {
        id: _re_add_the_stored_key
        QScreenAdd {
            anchors.fill: parent
            QOnScreenContentTypeA {
                id: _refresh
                width: popupWidth
                height: popupHeight
                anchors.centerIn: parent
                label.text: STR.STR_QML_1967
                onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                content: QAddKeyRefreshDevices {
                    title: {
                        var hardwareType = SignerManagement.currentSigner.hwType
                        switch(hardwareType) {
                        case NUNCHUCKTYPE.ADD_LEDGER: return STR.STR_QML_824
                        case NUNCHUCKTYPE.ADD_TREZOR: return ""
                        case NUNCHUCKTYPE.ADD_COLDCARD: return STR.STR_QML_911
                        case NUNCHUCKTYPE.ADD_BITBOX: return ""
                        case NUNCHUCKTYPE.ADD_JADE: return STR.STR_QML_1538
                        default: return ""
                        }
                    }
                    state_id: EVT.STATE_ID_SCR_ADD_HARDWARE
                }
                onPrevClicked: stateFlow.backScreen()
                bottomRight: Row {
                    spacing: 12
                    QIconTextButton {
                        width: rowObj.implicitWidth + 32
                        height: 48
                        label: {
                            var hardwareType = SignerManagement.currentSigner.hwType
                            switch(hardwareType) {
                            case NUNCHUCKTYPE.ADD_COLDCARD: return STR.STR_QML_1922
                            case NUNCHUCKTYPE.ADD_JADE: return STR.STR_QML_2040
                            default: return ""
                            }
                        }
                        icons: ["QR-dark.svg", "QR-dark.svg", "QR-dark.svg","QR-dark.svg"]
                        fontPixelSize: 16
                        iconSize: 16
                        type: eTypeB
                        visible: SignerManagement.currentSigner.hwType == NUNCHUCKTYPE.ADD_COLDCARD || SignerManagement.currentSigner.hwType == NUNCHUCKTYPE.ADD_JADE
                        onButtonClicked: {
                            if (SignerManagement.currentSigner.hwType == NUNCHUCKTYPE.ADD_COLDCARD) {
                                stateFlow.setScreenFlow("coldcard-via-qr-screen")
                            } else if (SignerManagement.currentSigner.hwType == NUNCHUCKTYPE.ADD_JADE) {
                                stateFlow.setScreenFlow("blockstream-jade-via-qr-screen")
                            }
                        }
                    }
                    QIconTextButton {
                        width: rowObj.implicitWidth + 32
                        height: 48
                        label: STR.STR_QML_1050
                        icons: ["importFile.svg", "importFile.svg", "importFile.svg","importFile.svg"]
                        fontPixelSize: 16
                        iconSize: 16
                        type: eTypeB
                        visible: SignerManagement.currentSigner.hwType == NUNCHUCKTYPE.ADD_COLDCARD
                        onButtonClicked: {
                            stateFlow.setScreenFlow("coldcard-via-file-screen")
                        }
                    }
                    QTextButton {
                        width: label.paintedWidth + 32
                        height: 48
                        label.text: STR.STR_QML_265
                        label.font.pixelSize: 16
                        type: eTypeE
                        enabled: _refresh.contentItem.isEnable()
                        onButtonClicked: {
                            draftWallet.requestVerifySingleSignerViaConnectDevice(_refresh.contentItem.mDevicelist.currentIndex, selected_verify_option, _infoPopup.verificationMethod, _infoPopup.xfp, _infoPopup.derivationPath)
                        }
                    }
                }
            }
            Connections {
                target: draftWallet
                onVerifySingleSignerResult: verifyResult(result, errorMsg)
            }
        }
    }
    // Setup 13bD: "Seed phrase verified" - re-added device's public key matched the inheritance key.
    Component {
        id: _resultRestoreKey
        QOnScreenContent {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: ""
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            content: Column {
                anchors.fill: parent
                anchors.margins: 36
                spacing: 24
                Rectangle {
                    width: 96; height: 96
                    radius: 48
                    color: "#A7F0BA"
                    QIcon {
                        iconSize: 60
                        anchors.centerIn: parent
                        source: "qrc:/Images/Images/check-dark.svg"
                    }
                }
                QLato {
                    width: parent.width
                    height: 40
                    text: STR.STR_QML_2326
                    font.pixelSize: 32
                    font.weight: Font.DemiBold
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle {
                    width: 539
                    height: 76
                    radius: 12
                    border.width: 1
                    border.color: "#DEDEDE"
                    Row {
                        anchors { fill: parent; margins: 16 }
                        spacing: 12
                        QIcon {
                            iconSize: 24
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/Device_Icons/key-dark.svg"
                        }
                        Column {
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 4
                            QLato {
                                text: _infoPopup.signerName !== "" ? _infoPopup.signerName : _infoPopup.signerTag
                                font.weight: Font.ExtraBold
                                font.pixelSize: 15
                            }
                            // NOTE (assumed field name "type", needs visual QA): matches the convention used
                            // by QSignerDetailDelegate.qml (typeStr: modelData.keyinfo.type) elsewhere.
                            QSignerBadgeName {
                                typeStr: _infoPopup.signerType
                                tag: _infoPopup.signerTag
                                color: "#DEDEDE"
                                height: 16
                                font.weight: Font.Bold
                                font.pixelSize: 10
                            }
                            QLato {
                                text: STR.STR_QML_2327
                                font.pixelSize: 12
                                color: "#5B6268"
                            }
                        }
                    }
                }
            }
            bottomRight: QTextButton {
                width: 120
                height: 48
                label.text: STR.STR_QML_265
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
        }
    }

    // Setup 13cD: "This key doesn't match" - re-added device derives a different public key.
    // GAP (needs backend confirmation): the API has no structured expected/actual XFP field, only a
    // free-text error message, so the card subtitle below falls back to that message, not "XFP: X - expected Y".
    Component {
        id: _resultRestoreKeyFailed
        QOnScreenContent {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: ""
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            content: Column {
                anchors.fill: parent
                anchors.margins: 36
                spacing: 24
                Rectangle {
                    width: 96; height: 96
                    radius: 48
                    color: "#FFD7D9"
                    QIcon {
                        iconSize: 60
                        anchors.centerIn: parent
                        source: "qrc:/Images/Images/error_outline_24px.png"
                    }
                }
                QLato {
                    width: parent.width
                    height: 40
                    text: STR.STR_QML_2328
                    font.pixelSize: 32
                    font.weight: Font.DemiBold
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    width: 539
                    text: STR.STR_QML_2329
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 24
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                }
                Rectangle {
                    width: 539
                    height: 76
                    radius: 12
                    border.width: 1
                    border.color: "#DEDEDE"
                    Row {
                        anchors { fill: parent; margins: 16 }
                        spacing: 12
                        QIcon {
                            iconSize: 24
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/Device_Icons/key-dark.svg"
                        }
                        Column {
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 4
                            QLato {
                                text: _infoPopup.signerName !== "" ? _infoPopup.signerName : _infoPopup.signerTag
                                font.weight: Font.ExtraBold
                                font.pixelSize: 15
                            }
                            QSignerBadgeName {
                                typeStr: _infoPopup.signerType
                                tag: _infoPopup.signerTag
                                color: "#DEDEDE"
                                height: 16
                                font.weight: Font.Bold
                                font.pixelSize: 10
                            }
                            // GAP: no expected/actual XFP field from the API - show the backend's own
                            // error message when present, else just this key's XFP.
                            QLato {
                                text: _infoPopup.verify_error_msg !== "" ? _infoPopup.verify_error_msg : "XFP: " + _infoPopup.xfp.toUpperCase()
                                font.pixelSize: 12
                                color: "#5B6268"
                                wrapMode: Text.WordWrap
                                width: 380
                            }
                        }
                    }
                }
            }
            bottomRight: QTextButton {
                width: label.paintedWidth + 32
                height: 48
                label.text: STR.STR_QML_1632
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: stateFlow.setScreenFlow("re-add-the-stored-key")
            }
        }
    }

    Component {
        id: coldcard_via_file_screen
        QScreenAdd {
            anchors.fill: parent
            QReAddCOLDCARDGuideViaFile {
                onCloseClicked: _infoPopup.close()
                onPrevClicked: stateFlow.backScreen()                
            }
            Connections {
                target: draftWallet
                onVerifySingleSignerResult: verifyResult(result, errorMsg)
            }
        }
    }
    Component {
        id: coldcard_via_qr_screen
        QScreenAdd {
            anchors.fill: parent
            QReAddCOLDCARDGuideViaQR {
                onCloseClicked: _infoPopup.close()
                onPrevClicked: stateFlow.backScreen()                
            }
            Connections {
                target: draftWallet
                onVerifySingleSignerResult: verifyResult(result, errorMsg)
            }
        }
    }
    Component {
        id: blockstream_jade_via_qr_screen
        QScreenAdd {
            anchors.fill: parent
            QReAddBlockstreamJadeViaQR {
                onCloseClicked: _infoPopup.close()
                onPrevClicked: stateFlow.backScreen()                
            }
            Connections {
                target: draftWallet
                onVerifySingleSignerResult: verifyResult(result, errorMsg)
            }
        }
    }
}
