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
import "../../../../../localization/STR_QML.js" as STR

// Setup 12c-*: "Verify your backups" checklist, shown when "Do both" is chosen in Key Distribution Choice.
QPopupOverlayScreen {
    id: _root
    property string xfp: ""
    property string signerTag: ""
    signal changeShareMethod()
    // BUGFIX: same as QEncryptedBackupFlow.qml - "draftWallet" must be declared locally, not global.
    property var draftWallet: GroupWallet.qIsByzantine ? GroupWallet : UserWallet

    QScreenStateFlow {
        id: stateFlow
    }

    function open2(keyXfp, tag) {
        xfp = keyXfp
        signerTag = tag
        stateFlow.setScreenFlow("checklist")
        _root.open()
    }

    // Looks up the current key by xfp in dashInfo.keys to read the latest verifications[], reactive on draftWalletChanged.
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
    // Setup 12cD..12c-viiD: 4 row states - NOT_UPLOADED/PENDING/SKIPPED/VERIFIED, same logic as
    // QAddRequestKey.qml's fileStatusText()/seedStatusText().
    function encryptedState() {
        var v = verificationFor("ENCRYPTED_BACKUP")
        // NOTE (assumed, needs backend confirmation): no verifications[] entry = never uploaded.
        if (!v) return "NOT_UPLOADED"
        if (v.verification_type === "NONE") return "PENDING"
        if (v.verification_type === "SKIPPED_VERIFICATION") return "SKIPPED"
        return "VERIFIED"
    }
    function seedState() {
        var v = verificationFor("SEED_PHRASE")
        if (!v || v.verification_type === "NONE") return "PENDING"
        if (v.verification_type === "SKIPPED_VERIFICATION") return "SKIPPED"
        return "VERIFIED"
    }
    function isUploaded() {
        return encryptedState() !== "NOT_UPLOADED"
    }
    function canContinue() {
        var e = encryptedState()
        var s = seedState()
        return (e === "VERIFIED" || e === "SKIPPED") && (s === "VERIFIED" || s === "SKIPPED")
    }

    readonly property var map_flow: [
        {screen: "checklist",       screen_component: _checklist},
        {screen: "remove-confirm",  screen_component: _removeConfirm},
    ]
    content: {
        var itemScreen = map_flow.find(function(e) { return e.screen === stateFlow.screenFlow })
        return itemScreen ? itemScreen.screen_component : _checklist
    }

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
                        color: state_ === "VERIFIED" ? "#66A7F0BA" : (state_ === "SKIPPED" ? "#FDEBD2" : "#FFFFFF")
                        Row {
                            anchors { fill: parent; margins: 16 }
                            spacing: 12
                            QIcon { iconSize: 24; anchors.verticalCenter: parent.verticalCenter; source: "qrc:/Images/Images/upload-cloud.svg" }
                            Column {
                                width: 300
                                anchors.verticalCenter: parent.verticalCenter
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
                                anchors.verticalCenter: parent.verticalCenter
                                width: 88
                                height: 24
                                iconSize: 24
                                icon: "qrc:/Images/Images/check-circle-dark.svg"
                                text: STR.STR_QML_2279
                                color: "#A7F0BA"
                            }
                            QTextButton {
                                visible: _encryptedRow.state_ !== "VERIFIED"
                                anchors.verticalCenter: parent.verticalCenter
                                width: label.paintedWidth + 32
                                height: 36
                                type: eTypeB
                                label.font.pixelSize: 14
                                label.text: _encryptedRow.state_ === "NOT_UPLOADED" ? STR.STR_QML_2282 : STR.STR_QML_2281
                                onButtonClicked: {
                                    // NOT_UPLOADED: full flow from start. PENDING/SKIPPED: already uploaded,
                                    // jump straight to verify-your-backup.
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
                        color: state_ === "VERIFIED" ? "#66A7F0BA" : (state_ === "SKIPPED" ? "#FDEBD2" : "#FFFFFF")
                        Row {
                            anchors { fill: parent; margins: 16 }
                            spacing: 12
                            QIcon { iconSize: 24; anchors.verticalCenter: parent.verticalCenter; source: "qrc:/Images/Images/Device_Icons/key-dark.svg" }
                            Column {
                                width: 300
                                anchors.verticalCenter: parent.verticalCenter
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
                                anchors.verticalCenter: parent.verticalCenter
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
                                anchors.verticalCenter: parent.verticalCenter
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
                                if (_root.isUploaded()) {
                                    stateFlow.setScreenFlow("remove-confirm")
                                } else {
                                    _root.close()
                                    _root.changeShareMethod()
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Confirm removing the encrypted backup when switching from "Do both" to "Share seed phrase directly".
    Component {
        id: _removeConfirm
        QOnScreenContentTypeA {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_2283
            onCloseClicked: stateFlow.setScreenFlow("checklist")
            onPrevClicked: stateFlow.setScreenFlow("checklist")
            content: Item {
                Column {
                    width: 539
                    spacing: 16
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_2284
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 20
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                    }
                    QWarningBgMulti {
                        width: 539
                        height: 48
                        icon: "qrc:/Images/Images/info-60px.svg"
                        txt.text: STR.STR_QML_2285
                    }
                }
            }
            bottomRight: QTextButton {
                width: label.paintedWidth + 32
                height: 48
                type: eTypeD
                label.text: STR.STR_QML_2286
                label.font.pixelSize: 16
                onButtonClicked: {
                    // BUGFIX: used to close regardless of result; now checks it (backend toasts on failure).
                    if (!draftWallet.requestSetClaimOptions(["SEED_PHRASE"])) {
                        return
                    }
                    // BUGFIX: missing refresh, same as SCR_ADD_HARDWARE.qml/QEncryptedBackupFlow.qml.
                    GroupWallet.refresh()
                    _root.close()
                    _root.changeShareMethod()
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
