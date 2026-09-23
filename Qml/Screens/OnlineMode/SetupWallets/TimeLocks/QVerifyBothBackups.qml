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

// Setup 12c-*: "Verify your backups" checklist, shown when "Do both" is chosen in Key Distribution Choice.
QPopupOverlayScreen {
    id: _root
    property string xfp: ""
    property string signerTag: ""
    signal changeShareMethod()

    function open2(keyXfp, tag) {
        xfp = keyXfp
        signerTag = tag
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
    function canContinue() {
        var e = encryptedState()
        var s = seedState()
        return (e === "VERIFIED" || e === "SKIPPED") && (s === "VERIFIED" || s === "SKIPPED")
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
                        height: 72
                        radius: 12
                        border.width: 1
                        border.color: "#DEDEDE"
                        property string state_: _root.encryptedState()
                        // BUGFIX: dropped the VERIFIED green fill - badge already signals "Verified"
                        // (same convention as QAddRequestKey.qml); coloring the whole row too was redundant.
                        color: state_ === "SKIPPED" ? "#FDEBD2" : "#FFFFFF"
                        Item {
                            anchors { fill: parent; margins: 16 }
                            QIcon {
                                id: _encIcon
                                iconSize: 24
                                anchors { left: parent.left; verticalCenter: parent.verticalCenter }
                                source: "qrc:/Images/Images/change-password-dark.svg"
                            }
                            Column {
                                // BUGFIX: was a fixed-width Row item, so the action button/badge floated right
                                // after the text instead of pinning to the card's right edge; anchor-based
                                // layout now keeps the action fixed to the right like the mockup.
                                anchors {
                                    left: _encIcon.right
                                    leftMargin: 12
                                    right: parent.right
                                    rightMargin: 100
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
                                // Setup 12c-ivD: "Verification skipped" label next to Verify button (pixel
                                // placement not yet QA'd against mockup).
                                QLato {
                                    visible: _encryptedRow.state_ === "SKIPPED"
                                    text: STR.STR_QML_2280
                                    font.pixelSize: 11
                                    font.weight: Font.Bold
                                    color: "#9A6B23"
                                }
                            }
                            QBadge {
                                visible: _encryptedRow.state_ === "VERIFIED"
                                anchors { right: parent.right; verticalCenter: parent.verticalCenter }
                                width: 88
                                height: 24
                                iconSize: 24
                                icon: "qrc:/Images/Images/check-circle-dark.svg"
                                text: STR.STR_QML_2279
                                color: "#A7F0BA"
                            }
                            QTextButton {
                                visible: _encryptedRow.state_ !== "VERIFIED"
                                anchors { right: parent.right; verticalCenter: parent.verticalCenter }
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
                        }
                    }
                    Rectangle {
                        id: _seedRow
                        width: 539
                        height: 72
                        radius: 12
                        border.width: 1
                        border.color: "#DEDEDE"
                        property string state_: _root.seedState()
                        // BUGFIX: same as _encryptedRow - drop the redundant VERIFIED green fill.
                        color: state_ === "SKIPPED" ? "#FDEBD2" : "#FFFFFF"
                        Item {
                            anchors { fill: parent; margins: 16 }
                            QIcon {
                                id: _seedIcon
                                iconSize: 24
                                anchors { left: parent.left; verticalCenter: parent.verticalCenter }
                                source: "qrc:/Images/Images/Device_Icons/key-dark.svg"
                            }
                            Column {
                                // BUGFIX: same right-edge pin as _encryptedRow.
                                anchors {
                                    left: _seedIcon.right
                                    leftMargin: 12
                                    right: parent.right
                                    rightMargin: 100
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
                                QLato {
                                    visible: _seedRow.state_ === "SKIPPED"
                                    text: STR.STR_QML_2280
                                    font.pixelSize: 11
                                    font.weight: Font.Bold
                                    color: "#9A6B23"
                                }
                            }
                            QBadge {
                                visible: _seedRow.state_ === "VERIFIED"
                                anchors { right: parent.right; verticalCenter: parent.verticalCenter }
                                width: 88
                                height: 24
                                iconSize: 24
                                icon: "qrc:/Images/Images/check-circle-dark.svg"
                                text: STR.STR_QML_2279
                                color: "#A7F0BA"
                            }
                            QTextButton {
                                // NEEDS CONFIRMATION: seed phrase "Verify" always restarts startFlow(); there's
                                // no "verify only" shortcut like encrypted backup has.
                                visible: _seedRow.state_ !== "VERIFIED"
                                anchors { right: parent.right; verticalCenter: parent.verticalCenter }
                                width: label.paintedWidth + 32
                                height: 36
                                type: eTypeB
                                label.font.pixelSize: 14
                                label.text: STR.STR_QML_2281
                                onButtonClicked: _backupSeedFlow.startFlow()
                            }
                        }
                    }
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_2277
                        font.pixelSize: 12
                        color: "#5B6268"
                        wrapMode: Text.WordWrap
                    }
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_2278
                        font.pixelSize: 13
                        font.underline: true
                        color: "#0051CF"
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
