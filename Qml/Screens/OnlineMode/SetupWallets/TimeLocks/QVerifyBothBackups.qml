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
import "../../../../../localization/STR_QML.js" as STR

// Setup 12c-*: "Verify your backups" checklist - single entry point for any configured inheritance key
// (1 or 2 claim_options), not just "Do both"; the row for a method not in claim_options is grayed out
// and disabled (see active_/hasEncryptedOption()/hasSeedOption() below), not hidden.
QPopupOverlayScreen {
    id: _root
    property string xfp: ""
    property string signerTag: ""
    // BUGFIX: right after requestSetClaimOptions()+GroupWallet.refresh(), dashInfo.keys[].claim_options
    // can still be stale (refresh is async) by the time this screen opens - callers already have the
    // claim_options they just chose/read, so accept it directly instead of re-deriving from currentKey().
    property var selectedClaimOptions: []
    signal changeShareMethod()

    function open2(keyXfp, tag, claimOpts) {
        xfp = keyXfp
        signerTag = tag
        selectedClaimOptions = claimOpts !== undefined ? claimOpts : []
        _root.open()
    }

    // Looks up the current key by xfp in dashInfo.keys, to read the latest verifications[].
    function currentKey() {
        var keys = GroupWallet.dashboardInfo.keys
        for (var i = 0; i < keys.length; i++) {
            if (keys[i].xfp === _root.xfp) return keys[i]
        }
        return null
    }
    function verificationFor(method) {
        var key = currentKey()
        if (!key || key.verifications === undefined) return null
        var list = key.verifications
        for (var i = 0; i < list.length; i++) {
            if (list[i].verification_method === method) return list[i]
        }
        return null
    }
    // Setup 12cD..12c-viiD: 3 reachable states for encrypted (NOT_UPLOADED/SKIPPED/VERIFIED).
    // BUGFIX (confirmed via runtime log: backend 400 "Missing encrypted backup" on verify): a
    // verifications[] entry with verification_type "NONE" can exist before any file is actually
    // uploaded, so "NONE" must count as NOT_UPLOADED here too, same as QAddRequestKey.qml's fileStatusText().
    function encryptedState() {
        var v = verificationFor("ENCRYPTED_BACKUP")
        if (!v || v.verification_type === "NONE") return "NOT_UPLOADED"
        if (v.verification_type === "SKIPPED_VERIFICATION") return "SKIPPED"
        return "VERIFIED"
    }
    function seedState() {
        var v = verificationFor("SEED_PHRASE")
        if (!v || v.verification_type === "NONE") return "PENDING"
        if (v.verification_type === "SKIPPED_VERIFICATION") return "SKIPPED"
        return "VERIFIED"
    }
    // BUGFIX: this screen is now the single entry point for ANY configured key (1 or 2 claim_options),
    // not just "Do both" - each row must only be active when its method is actually selected, and
    // Continue must not wait on a method the key never opted into.
    function claimOptions() {
        if (selectedClaimOptions.length > 0) return selectedClaimOptions
        var key = currentKey()
        return (key && key.claim_options !== undefined) ? key.claim_options : []
    }
    function hasEncryptedOption() {
        return claimOptions().indexOf("ENCRYPTED_BACKUP") !== -1
    }
    function hasSeedOption() {
        return claimOptions().indexOf("SEED_PHRASE") !== -1
    }
    function canContinue() {
        var e = encryptedState()
        var s = seedState()
        var encryptedOk = !hasEncryptedOption() || (e === "VERIFIED" || e === "SKIPPED")
        var seedOk = !hasSeedOption() || (s === "VERIFIED" || s === "SKIPPED")
        return encryptedOk && seedOk
    }

    // BUGFIX: was a 2-screen state machine (checklist/remove-confirm); the remove-confirm step
    // moved to the Key Distribution Choice call sites (NUN-10192), so this only ever shows _checklist now.
    content: _checklist

    Component {
        id: _checklist
        QOnScreenContentTypeA {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_2270
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            nextEnable: _root.canContinue()
            onNextClicked: {
                _root.close()
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG)
            }
            content: Item {
                Column {
                    width: 539
                    spacing: 16
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_2271
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 20
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                    }
                    Rectangle {
                        id: _encryptedRow
                        width: 539
                        // BUGFIX: row grows to guarantee >=12px top/bottom gap around whichever side (text
                        // column or action column) is taller, instead of a fixed height:72 that overflowed.
                        height: Math.max(72, Math.max(_encCol.height, _encActionCol.height) + 24)
                        radius: 12
                        border.width: 1
                        border.color: "#DEDEDE"
                        property string state_: _root.encryptedState()
                        // BUGFIX: this method may not be selected at all for this key - gray it out and
                        // disable its action when so, instead of always treating it as active.
                        property bool active_: _root.hasEncryptedOption()
                        enabled: active_
                        opacity: active_ ? 1.0 : 0.4
                        // BUGFIX: design (Setup 12c-viiD) shows a green fill for VERIFIED, not just the
                        // badge - a previous change dropped this as "redundant" but that doesn't match design.
                        color: state_ === "SKIPPED" ? "#FDEBD2" : (state_ === "VERIFIED" ? "#A7F0BA" : "#FFFFFF")
                        Item {
                            anchors { fill: parent; leftMargin: 16; rightMargin: 16; topMargin: 12; bottomMargin: 12 }
                            // BUGFIX: design wraps the row icon in a round neutral badge, not a bare icon.
                            Rectangle {
                                id: _encIconBadge
                                width: 48
                                height: 48
                                radius: 24
                                color: "#F5F5F5"
                                anchors { left: parent.left; verticalCenter: parent.verticalCenter }
                                QIcon {
                                    id: _encIcon
                                    iconSize: 24
                                    anchors.centerIn: parent
                                    source: "qrc:/Images/Images/backup_black_24dp.svg"
                                }
                            }
                            Column {
                                id: _encCol
                                anchors {
                                    left: _encIconBadge.right
                                    leftMargin: 12
                                    right: _encActionCol.left
                                    rightMargin: 12
                                    verticalCenter: parent.verticalCenter
                                }
                                spacing: 2
                                QLato { text: STR.STR_QML_2272; font.weight: Font.ExtraBold; font.pixelSize: 15 }
                                QLato {
                                    width: parent.width
                                    text: STR.STR_QML_2273
                                    font.pixelSize: 12
                                    wrapMode: Text.WordWrap
                                }
                            }
                            // BUGFIX: design places "Verification skipped" under the action button/badge on
                            // the right (not under the description on the left, as it was before) - this
                            // column groups the badge/button with that label so they stay pinned together.
                            Item {
                                id: _encActionCol
                                anchors { right: parent.right; verticalCenter: parent.verticalCenter }
                                width: 120
                                height: (_encryptedRow.state_ === "VERIFIED" ? 24 :
                                        (36 + (_encryptedRow.state_ === "SKIPPED" ? 4 + _encSkippedLabel.height : 0)))
                                QBadge {
                                    id: _encBadge
                                    visible: _encryptedRow.state_ === "VERIFIED"
                                    anchors { right: parent.right; top: parent.top }
                                    width: 88
                                    height: 24
                                    iconSize: 24
                                    icon: "qrc:/Images/Images/check-circle-dark.svg"
                                    text: STR.STR_QML_2279
                                    color: "#A7F0BA"
                                }
                                QTextButton {
                                    id: _encButton
                                    visible: _encryptedRow.state_ !== "VERIFIED"
                                    anchors { right: parent.right; top: parent.top }
                                    width: label.paintedWidth + 32
                                    height: 36
                                    type: eTypeB
                                    label.font.pixelSize: 14
                                    label.text: _encryptedRow.state_ === "NOT_UPLOADED" ? STR.STR_QML_2282 : STR.STR_QML_2281
                                    onButtonClicked: {
                                        // NOT_UPLOADED: full flow from start. SKIPPED: was uploaded once
                                        // (skip only reachable after a real upload attempt) - jump to verify.
                                        if (_encryptedRow.state_ === "NOT_UPLOADED") {
                                            _encryptedFlow.startFlow(_root.signerTag, _root.xfp)
                                        } else {
                                            _encryptedFlow.startVerifyOnly(_root.signerTag, _root.xfp)
                                        }
                                    }
                                }
                                QLato {
                                    id: _encSkippedLabel
                                    visible: _encryptedRow.state_ === "SKIPPED"
                                    anchors { right: parent.right; top: _encButton.bottom; topMargin: 4 }
                                    text: STR.STR_QML_2280
                                    font.pixelSize: 11
                                    font.weight: Font.Bold
                                    color: "#9A6B23"
                                }
                            }
                        }
                    }
                    Rectangle {
                        id: _seedRow
                        width: 539
                        // BUGFIX: same overflow fix as _encryptedRow - guarantee >=12px top/bottom gap
                        // around whichever side (text column or action column) is taller.
                        height: Math.max(72, Math.max(_seedCol.height, _seedActionCol.height) + 24)
                        radius: 12
                        border.width: 1
                        border.color: "#DEDEDE"
                        property string state_: _root.seedState()
                        // BUGFIX: same as _encryptedRow - gray out and disable when not selected.
                        property bool active_: _root.hasSeedOption()
                        enabled: active_
                        opacity: active_ ? 1.0 : 0.4
                        // BUGFIX: same as _encryptedRow - design shows a green fill for VERIFIED.
                        color: state_ === "SKIPPED" ? "#FDEBD2" : (state_ === "VERIFIED" ? "#A7F0BA" : "#FFFFFF")
                        Item {
                            anchors { fill: parent; leftMargin: 16; rightMargin: 16; topMargin: 12; bottomMargin: 12 }
                            // BUGFIX: same round icon badge as _encryptedRow.
                            Rectangle {
                                id: _seedIconBadge
                                width: 48
                                height: 48
                                radius: 24
                                color: "#F5F5F5"
                                anchors { left: parent.left; verticalCenter: parent.verticalCenter }
                                QIcon {
                                    id: _seedIcon
                                    iconSize: 24
                                    anchors.centerIn: parent
                                    source: "qrc:/Images/Images/Device_Icons/key-dark.svg"
                                }
                            }
                            Column {
                                id: _seedCol
                                anchors {
                                    left: _seedIconBadge.right
                                    leftMargin: 12
                                    right: _seedActionCol.left
                                    rightMargin: 12
                                    verticalCenter: parent.verticalCenter
                                }
                                spacing: 2
                                QLato { text: STR.STR_QML_2275; font.weight: Font.ExtraBold; font.pixelSize: 15 }
                                QLato {
                                    width: parent.width
                                    text: STR.STR_QML_2276
                                    font.pixelSize: 12
                                    wrapMode: Text.WordWrap
                                }
                            }
                            // BUGFIX: same right-side grouping as _encryptedRow - "Verification skipped"
                            // belongs under the action button, not under the left-side description.
                            Item {
                                id: _seedActionCol
                                anchors { right: parent.right; verticalCenter: parent.verticalCenter }
                                width: 120
                                height: (_seedRow.state_ === "VERIFIED" ? 24 :
                                        (36 + (_seedRow.state_ === "SKIPPED" ? 4 + _seedSkippedLabel.height : 0)))
                                QBadge {
                                    id: _seedBadge
                                    visible: _seedRow.state_ === "VERIFIED"
                                    anchors { right: parent.right; top: parent.top }
                                    width: 88
                                    height: 24
                                    iconSize: 24
                                    icon: "qrc:/Images/Images/check-circle-dark.svg"
                                    text: STR.STR_QML_2279
                                    color: "#A7F0BA"
                                }
                                QTextButton {
                                    id: _seedButton
                                    // NEEDS CONFIRMATION: seed phrase "Verify" always restarts startFlow(); there's
                                    // no "verify only" shortcut like encrypted backup has.
                                    visible: _seedRow.state_ !== "VERIFIED"
                                    anchors { right: parent.right; top: parent.top }
                                    width: label.paintedWidth + 32
                                    height: 36
                                    type: eTypeB
                                    label.font.pixelSize: 14
                                    label.text: STR.STR_QML_2281
                                    // BUGFIX: pass the actual key (with a real derivation_path) instead of
                                    // letting startFlow() read the unset global SignerManagement.currentSigner -
                                    // root cause of the seed-phrase re-verify HWI "Run command exit error!".
                                    // BUGFIX: pass isOffChain=true explicitly, independent of currentKey().
                                    onButtonClicked: _backupSeedFlow.startFlow(_root.currentKey(), true)
                                }
                                QLato {
                                    id: _seedSkippedLabel
                                    visible: _seedRow.state_ === "SKIPPED"
                                    anchors { right: parent.right; top: _seedButton.bottom; topMargin: 4 }
                                    text: STR.STR_QML_2280
                                    font.pixelSize: 11
                                    font.weight: Font.Bold
                                    color: "#9A6B23"
                                }
                            }
                        }
                    }
                    // BUGFIX: design shows this as a grey rounded info box with an (i) icon, not plain
                    // text - reusing the same info-box component/icon already used elsewhere in this flow.
                    QWarningBgMulti {
                        width: 539
                        icon: "qrc:/Images/Images/info-60px.svg"
                        txt.text: STR.STR_QML_2277
                    }
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_2278
                        font.pixelSize: 13
                        // BUGFIX: design shows this as plain bold dark text, not a blue underlined link.
                        font.weight: Font.Bold
                        color: "#031F2B"
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                // BUGFIX: was routing to the remove-confirm screen instead of the "How will you
                                // pass the inheritance key" choice screen whenever a backup was already uploaded -
                                // this link must always start that flow (changeShareMethod), regardless of upload state.
                                _root.close()
                                _root.changeShareMethod()
                            }
                        }
                    }
                }
            }
        }
    }

    QBackupSeedPhraseFlow {
        id: _backupSeedFlow
    }
    QEncryptedBackupFlow {
        id: _encryptedFlow
    }
}
