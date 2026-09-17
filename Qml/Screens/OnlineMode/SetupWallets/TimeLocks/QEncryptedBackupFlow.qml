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

// Branch (b) - Encrypted backup: Setup 15D/15aD/15bD/16D/17D/18D, generic for all hardware incl. COLDCARD.
// CORRECTION: an earlier comment claimed COLDCARD needed special routing; verified QBackupCOLDCARD.qml
// calls the identical requestBackupColdcard() API, so this generic flow is correct, not a bug.
// Known gap: _deviceGuide only has detailed steps for KEYSTONE; others share a generic placeholder text.
QPopupOverlayScreen {
    id: _root
    signal finished()
    property string xfp: ""
    property string signerTag: ""
    // BUGFIX: "draftWallet" isn't a global context property; declare it locally to avoid ReferenceError.
    property var draftWallet: GroupWallet.qIsByzantine ? GroupWallet : UserWallet

    QScreenStateFlow {
        id: stateFlow
    }

    function deviceName(tag) {
        switch (tag) {
        case "KEYSTONE": return "Keystone"
        case "PASSPORT": return "Foundation Passport"
        case "JADE": return "Blockstream Jade"
        case "LEDGER": return "Ledger"
        case "TREZOR": return "Trezor"
        case "BITBOX": return "BitBox"
        case "KEEPKEY": return "KeepKey"
        case "KRUX": return "Krux"
        default: return tag
        }
    }

    function startFlow(tag, keyXfp) {
        signerTag = tag
        xfp = keyXfp
        // "open-import-encrypted-backup" resets AppModel.addSignerPercentage to 0.
        GroupWallet.dashboardInfo.requestBackupColdcard({type: "open-import-encrypted-backup", fingerPrint: keyXfp})
        stateFlow.setScreenFlow("back-up-your-inheritance-key")
        _root.open()
    }

    // Setup 12c*: when the backup is already uploaded (PENDING/SKIPPED), "Verify" jumps straight to
    // "verify-your-backup" without restarting the upload; startFlow() above is for the not-yet-uploaded case.
    function startVerifyOnly(tag, keyXfp) {
        signerTag = tag
        xfp = keyXfp
        stateFlow.setScreenFlow("verify-your-backup")
        _root.open()
    }

    readonly property var map_flow: [
        {screen: "back-up-your-inheritance-key", screen_component: _backupIntro},
        {screen: "device-guide",                 screen_component: _deviceGuide},
        {screen: "import-progress",              screen_component: _importProgress},
        {screen: "verify-your-backup",           screen_component: _verifyBackup},
    ]

    content: {
        var itemScreen = map_flow.find(function(e) { return e.screen === stateFlow.screenFlow })
        if (itemScreen) {
            return itemScreen.screen_component
        } else {
            _root.close()
            return null
        }
    }

    // Setup 15D
    Component {
        id: _backupIntro
        QOnScreenContentTypeA {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_2290
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onNextClicked: stateFlow.setScreenFlow("device-guide")
            content: Item {
                Column {
                    width: 539
                    spacing: 24
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_2291
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 20
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                    }
                    Repeater {
                        id: _guide
                        width: parent.width
                        readonly property var content_map: [
                            {content: STR.STR_QML_2292, icon: "qrc:/Images/Images/1.Active.svg"},
                            {content: STR.STR_QML_2293, icon: "qrc:/Images/Images/2.Active.svg"},
                            {content: STR.STR_QML_2294, icon: "qrc:/Images/Images/3.Active.svg"},
                        ]
                        model: content_map.length
                        Row {
                            property var _item: _guide.content_map[index]
                            width: 539
                            spacing: 12
                            QIcon { iconSize: 24; source: _item.icon }
                            QLato {
                                width: 500
                                text: _item.content
                                font.pixelSize: 16
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 20
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignLeft
                            }
                        }
                    }
                    QWarningBgMulti {
                        width: 539
                        height: 48
                        icon: "qrc:/Images/Images/info-60px.svg"
                        txt.text: STR.STR_QML_2295
                    }
                }
            }
        }
    }

    // Setup 15aD (Keystone) / 15bD (placeholder for other types)
    Component {
        id: _deviceGuide
        QOnScreenContentTypeA {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_2296.arg(_root.deviceName(_root.signerTag))
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: stateFlow.setScreenFlow("back-up-your-inheritance-key")
            onNextClicked: stateFlow.setScreenFlow("import-progress")
            content: Item {
                Column {
                    width: 539
                    spacing: 16
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_2297.arg(_root.deviceName(_root.signerTag))
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 20
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                    }
                    Repeater {
                        id: _guide2
                        width: parent.width
                        // Keystone has concrete steps; other vendors share a generic placeholder for now.
                        readonly property var content_map: _root.signerTag === "KEYSTONE"
                            ? [
                                {content: STR.STR_QML_2304, icon: "qrc:/Images/Images/1.Active.svg"},
                                {content: STR.STR_QML_2305, icon: "qrc:/Images/Images/2.Active.svg"},
                                {content: STR.STR_QML_2306, icon: "qrc:/Images/Images/3.Active.svg"},
                            ]
                            : [
                                {content: STR.STR_QML_2292, icon: "qrc:/Images/Images/1.Active.svg"},
                                {content: STR.STR_QML_2294, icon: "qrc:/Images/Images/2.Active.svg"},
                            ]
                        model: content_map.length
                        Row {
                            property var _item: _guide2.content_map[index]
                            width: 539
                            spacing: 12
                            QIcon { iconSize: 24; source: _item.icon }
                            QLato {
                                width: 500
                                text: _item.content
                                font.pixelSize: 16
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 20
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignLeft
                            }
                        }
                    }
                    QWarningBgMulti {
                        width: 539
                        height: 48
                        visible: _root.signerTag === "KEYSTONE"
                        icon: "qrc:/Images/Images/info-60px.svg"
                        txt.text: STR.STR_QML_2298.arg(_root.deviceName(_root.signerTag))
                    }
                }
            }
        }
    }

    // Setup 16D (import) + 17D (progress) + success/failed, driven by AppModel.addSignerPercentage
    Component {
        id: _importProgress
        Loader {
            anchors.fill: parent
            sourceComponent: {
                if (AppModel.addSignerPercentage === 0) return _importFile
                else if (AppModel.addSignerPercentage > 0 && AppModel.addSignerPercentage < 100) return _loading
                else if (AppModel.addSignerPercentage === 100) return _success
                else return _failed
            }
        }
    }

    Component {
        id: _importFile
        QOnScreenContentTypeA {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_1621
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: stateFlow.setScreenFlow("device-guide")
            content: Item {
                Row {
                    spacing: 36
                    Rectangle {
                        width: 346
                        height: 300
                        radius: 24
                        color: "#D0E2FF"
                        QPicture {
                            width: 200
                            height: 200
                            anchors.centerIn: parent
                            source: "qrc:/Images/Images/upload-cloud.svg"
                        }
                    }
                    QLato {
                        width: 346
                        text: STR.STR_QML_2299
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 20
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }
            bottomRight: QTextButton {
                width: 107
                height: 48
                label.text: STR.STR_QML_1165
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: fileDialog.open()
            }
            FileDialog {
                id: fileDialog
                fileMode: FileDialog.OpenFile
                onAccepted: {
                    var _input = {type: "import-encrypted-backup", fingerPrint: _root.xfp, currentFile: fileDialog.currentFile}
                    GroupWallet.dashboardInfo.requestBackupColdcard(_input)
                }
            }
        }
    }

    Component {
        id: _loading
        QImportEncryptedBackupLoading {}
    }

    Component {
        id: _success
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
                Column {
                    width: parent.width
                    spacing: 12
                    QMontserrat {
                        width: parent.width
                        height: 40
                        text: STR.STR_QML_2301
                        font.pixelSize: 32
                        font.weight: Font.Medium
                        verticalAlignment: Text.AlignVCenter
                    }
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_2302
                        verticalAlignment: Text.AlignVCenter
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 20
                        wrapMode: Text.WordWrap
                    }
                }
            }
            bottomRight: Row {
                spacing: 12
                QTextButton {
                    width: 120
                    height: 48
                    label.text: STR.STR_QML_265 // Continue
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: stateFlow.setScreenFlow("verify-your-backup")
                }
            }
        }
    }

    Component {
        id: _failed
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
                QMontserrat {
                    width: parent.width
                    height: 40
                    text: STR.STR_QML_2303
                    font.pixelSize: 24
                    font.weight: Font.Medium
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                }
            }
            bottomRight: Row {
                spacing: 12
                QTextButton {
                    width: 120
                    height: 48
                    label.text: STR.STR_QML_1632 // Try again
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        // BUGFIX: previously didn't reset addSignerPercentage here, so retrying from
                        // device-guide looped back to the failed screen instead of the file picker.
                        GroupWallet.dashboardInfo.requestBackupColdcard({type: "open-import-encrypted-backup", fingerPrint: _root.xfp})
                        stateFlow.setScreenFlow("device-guide")
                    }
                }
            }
        }
    }

    // Setup 18D
    Component {
        id: _verifyBackup
        QVerifyYourBackup {
            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onBackupVerifyChosen: function(verifyOption) {
                // BUGFIX: used to show a SUCCESS toast regardless of the API result; now checks the
                // (synchronous) return value and lets backend's own error toast handle failures.
                if (draftWallet.requestVerifyEncryptedBackup(verifyOption)) {
                    // BUGFIX: missing refresh after a successful verify - dashInfo.keys[] isn't updated
                    // locally by the verify call, so captions/checklist would show stale state. Matches
                    // QBackupSeedPhraseFlow.qml (onVerifySingleSignerResult).
                    GroupWallet.refresh()
                    _root.close()
                    AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG)
                    _root.finished()
                }
            }
        }
    }
}
