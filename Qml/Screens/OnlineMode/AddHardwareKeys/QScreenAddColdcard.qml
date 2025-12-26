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
import "../../../../localization/STR_QML.js" as STR
import "../../../Components/customizes"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/origins"
import "../../OnlineMode/SetupWallets"
import "../../OnlineMode/SetupWallets/TimeLocks"
import DataPool 1.0
import EWARNING 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import Qt.labs.platform 1.1
import QtGraphicalEffects 1.12
import QtQuick 2.4
import QtQuick.Controls 2.3

QScreenAdd {
    anchors.fill: parent
    // Screen-to-component mapping; supports static components and resolver functions
    readonly property var map_flow: [
        {screen: "_ASK_PASSPHRASE",       screen_component: _passPhraseSelect},
        {screen: "_IMPORTANT_NOTICE",     screen_component: _importantNotice},
        {screen: "_BACKUP_PASSPHRASE",    screen_component: _passPhraseBackup},
        {screen: "_BACKUP_COLDCARD",      screen_component: _backupCOLDCARD},
        {screen: "eSCREEN_INFORMATION",      screen_component: _Information},
        {screen: "eSCREEN_REFRESH_DEVICE",   screen_component: _RefreshDevice},
        {screen: "eSCREEN_VIA_FILE",         screen_component: function() {
                                                var walletType = SignerManagement.currentSigner.wallet_type
                                                if (walletType === "MINISCRIPT") {
                                                    var has = SignerManagement.currentSigner.has
                                                    var hasSecond = SignerManagement.currentSigner.hasSecond
                                                    if (!has && !hasSecond) return _ViaFile12
                                                    if (has && !hasSecond) return _ViaFile22
                                                    return null
                                                }
                                                return _ViaFile
                                            }},
        {screen: "eSCREEN_VIA_QR",           screen_component: function() {
                                                var has = SignerManagement.currentSigner.has
                                                var hasSecond = SignerManagement.currentSigner.hasSecond
                                                if (!has && !hasSecond) return _ViaQR12
                                                if (has && !hasSecond) return _ViaQR22
                                                return null
                                            }},
        {screen: "eSCREEN_LOADING",          screen_component: _loadingScreen},
        {screen: "eSCREEN_SUCCESS",        screen_component: _result},
        {screen: "eSCREEN_ERROR",          screen_component: _result},
        {screen: "eSCREEN_CLAIM_INHERITANCE_PLAN_RESULT_ERROR", screen_component: _resultClaimInheritancePlan},
    ]

    Loader {
        id: _background
        anchors.fill: parent
        // Choose the screen component via helper functions for clarity
        sourceComponent: {
            var itemScreen = map_flow.find(function(e) { return e.screen === stateScreen.screenFlow })
            if (!itemScreen) return _Information
            var sc = itemScreen.screen_component
            return (typeof sc === 'function') ? sc() : sc
        }
    }

    // No AppModel.addSignerWizard coupling; state transitions are driven locally via stateScreen
    Component {
        id: _passPhraseSelect
        QSelectPassPhraseQuestion {
            onRequestBack: {
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
            onRequestNext: {
                if (option === "not-have-a-passphrase") {
                    stateScreen.setScreenFlow("eSCREEN_INFORMATION")
                } else {
                    stateScreen.setScreenFlow("_IMPORTANT_NOTICE")
                }
            }
        }
    }
    Component {
        id: _importantNotice
        QImportantNoticeAboutPassphrase {
            onRequestBack: stateScreen.setScreenFlow("_ASK_PASSPHRASE")
            onRequestNext: {
                stateScreen.setScreenFlow("_BACKUP_PASSPHRASE")
            }
            onRequestWithout: {
                var alert = GroupWallet.dashboardInfo.alert
                var can_replace = alert.payload.can_replace
                if (can_replace) {
                    GroupWallet.dashboardInfo.requestShowReplacementKey();
                } else {
                    GroupWallet.dashboardInfo.requestShowLetAddYourKeys();
                }
            }
        }
    }
    Component {
        id: _passPhraseBackup
        QPassphraseBackupReminder {
            onRequestBack: stateScreen.setScreenFlow("_IMPORTANT_NOTICE")
            onRequestNext: {
                stateScreen.setScreenFlow("eSCREEN_INFORMATION")
            }
        }
    }
    Component {
        id: _backupCOLDCARD
        QBackupCOLDCARD {
            inputFingerPrint: AppModel.masterSignerInfo.fingerPrint
            onPrevClicked: stateScreen.setScreenFlow("eSCREEN_SUCCESS")
        }
    }
    
    function doneOrTryAgainAddColdcardKey(isSuccess) {
        var isNormalFlow = SignerManagement.currentSigner.wallet_type !== "MINISCRIPT"
        if (isNormalFlow) {
            if (isSuccess) {
                var is_inheritance = GroupWallet.dashboardInfo.isInheritance()
                if (is_inheritance) {
                    stateScreen.setScreenFlow("_BACKUP_COLDCARD")
                } else {
                    AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG);
                    closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                }
            } else {
                GroupWallet.refresh()
                GroupWallet.dashboardInfo.requestShowLetAddYourKeys();
            }
        } else {
            var xfp = SignerManagement.currentSigner.xfp
            if (GroupWallet.dashboardInfo.enoughKeyAdded(xfp)) {
                AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG);
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            } else {
                GroupWallet.refresh()
                GroupWallet.dashboardInfo.requestShowLetAddYourKeys();
            }
        }
    }
    
    Component {
        id: _Information
        QOnScreenContentTypeA {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_904
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            content: Item {
                Row {
                    spacing: 36
                    Rectangle {
                        width: 346
                        height: 512
                        radius: 24
                        color: "#D0E2FF"
                        QPicture {
                            width: 346
                            height: 300
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/coldcard-illustration.svg"
                        }
                    }
                    Item {
                        width: 346
                        height: 512
                        Column {
                            width: parent.width
                            spacing: 24
                            QLato {
                                width: parent.width
                                text: STR.STR_QML_817
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 28
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                            Repeater {
                                id: _guide
                                width: parent.width
                                readonly property var content_map: [
                                    {height: 48, headline:STR.STR_QML_907, content: STR.STR_QML_908 , icon: "qrc:/Images/Images/1.Active.svg" },
                                    {height: 84, headline:STR.STR_QML_909, content: STR.STR_QML_910 , icon: "qrc:/Images/Images/2.Active.svg" },
                                ]
                                model: content_map.length
                                Rectangle {
                                    property var _item: _guide.content_map[index]
                                    width: 346
                                    height: _item.height
                                    Row {
                                        spacing: 12
                                        QIcon {
                                            iconSize: 24
                                            id: _ico
                                            source: _item.icon
                                        }
                                        Column {
                                            width: 310
                                            height: _item.height
                                            spacing: 4
                                            QLato {
                                                width: 310
                                                text: _item.headline
                                                font.weight: Font.DemiBold
                                                horizontalAlignment: Text.AlignLeft
                                                verticalAlignment: Text.AlignVCenter
                                            }
                                            QLato {
                                                id: _term
                                                width: 310
                                                text: _item.content
                                                textFormat: Text.RichText
                                                lineHeightMode: Text.FixedHeight
                                                lineHeight: 28
                                                wrapMode: Text.WordWrap
                                                horizontalAlignment: Text.AlignLeft
                                                verticalAlignment: Text.AlignVCenter
                                                onLinkActivated: Qt.openUrlExternally("https://coldcard.com/docs/quick")
                                                MouseArea {
                                                    anchors.fill: parent
                                                    cursorShape: _term.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                                                    acceptedButtons: Qt.NoButton
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

            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onNextClicked: {
                 stateScreen.setScreenFlow("eSCREEN_REFRESH_DEVICE")
            }
        }
    }
    Component {
        id: _RefreshDevice
        QOnScreenContentTypeA {
            id: _refresh
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_904
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            content: QAddKeyRefreshDevices {
                title: STR.STR_QML_911
                state_id: EVT.STATE_ID_SCR_ADD_HARDWARE
            }
            onPrevClicked: {
                 stateScreen.setScreenFlow("eSCREEN_INFORMATION")
            }
            bottomRight: Row {
                spacing: 12
                QIconTextButton {
                    width: 244
                    height: 48
                    label: STR.STR_QML_1922
                    icons: ["QR-dark.svg", "QR-dark.svg", "QR-dark.svg","QR-dark.svg"]
                    fontPixelSize: 16
                    iconSize: 16
                    type: eTypeB
                    visible: SignerManagement.currentSigner.wallet_type === "MINISCRIPT"
                    onButtonClicked: stateScreen.setScreenFlow("eSCREEN_VIA_QR")
                }
                QIconTextButton {
                    width: 244
                    height: 48
                    label: STR.STR_QML_1050
                    icons: ["importFile.svg", "importFile.svg", "importFile.svg","importFile.svg"]
                    fontPixelSize: 16
                    iconSize: 16
                    type: eTypeB
                    onButtonClicked: stateScreen.setScreenFlow("eSCREEN_VIA_FILE")
                }
                QTextButton {
                    width: 120
                    height: 48
                    label.text: STR.STR_QML_265
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled: _refresh.contentItem.isEnable()
                    onButtonClicked: {
                        _refresh.contentItem.addDevice()
                    }
                }
            }
        }
    }
    Component {
        id: _ViaQR12
        QAddCOLDCARDGuideViaQR_1_2
        {
            id: via_file
            onPrevClicked: {
                 stateScreen.setScreenFlow("eSCREEN_REFRESH_DEVICE")
            }
        }
    }
    Component {
        id: _ViaQR22
        QAddCOLDCARDGuideViaQR_2_2
        {
            id: via_file
            onPrevClicked: {
                 stateScreen.setScreenFlow("eSCREEN_REFRESH_DEVICE")
            }
        }
    }
    Component {
        id: _ViaFile12
        QAddCOLDCARDGuideViaFile_1_2
        {
            id: via_file
            onPrevClicked: {
                 stateScreen.setScreenFlow("eSCREEN_REFRESH_DEVICE")
            }
        }
    }
    Component {
        id: _ViaFile22
        QAddCOLDCARDGuideViaFile_2_2
        {
            id: via_file
            onPrevClicked: {
                 stateScreen.setScreenFlow("eSCREEN_REFRESH_DEVICE")
            }
        }
    }
    Component {
        id: _ViaFile
        QScreenAddColdcardViaAFile
        {
            id: via_file
            onPrevClicked: {
                 stateScreen.setScreenFlow("eSCREEN_REFRESH_DEVICE")
            }
        }
    }
    Component {
        id: _loadingScreen
        QScreenAddKeyLoadingState {
            progressTitle: STR.STR_QML_912
        }
    }

    // Screen: Result - Success
    Component {
        id: _result
        QScreenAddKeyResult {
            isSuccess: stateScreen.screenFlow === "eSCREEN_SUCCESS"
            resultTitle: isSuccess ? STR.STR_QML_913 : STR.STR_QML_966
            resultSubtitle: isSuccess ? STR.STR_QML_828 : ""
            onDoneClicked: doneOrTryAgainAddHardwareKey(isSuccess)
        }
    }
    Component {
        id: _resultClaimInheritancePlan
        QScreenAddKeyResult {
            isSuccess: false
            resultTitle: STR.STR_QML_2045
            resultSubtitle: STR.STR_QML_2046
            bottomRight: Row {
                spacing: 12
                QTextButton {
                    width: 120
                    height: 48
                    label.text: STR.STR_QML_097
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                    }
                }
            }
        }
    }
    function addColdcardViaImportFile(filePath) {
        var isNormalFlow = SignerManagement.currentSigner.wallet_type !== "MINISCRIPT"
        if (isNormalFlow) {
            draftWallet.requestImportFileAddOrReplacementWithIndexAsync(filePath, -1)
        } else {
            var onlyUseForClaim = SignerManagement.currentSigner.onlyUseForClaim !== undefined && SignerManagement.currentSigner.onlyUseForClaim
            if (onlyUseForClaim) {
                ServiceSetting.servicesTag.requestDownloadWalletViaImportFile(filePath, 0)
            } else {
                var has = SignerManagement.currentSigner.has
                var hasSecond = SignerManagement.currentSigner.hasSecond
                if (!has && !hasSecond) return draftWallet.requestImportFileAddOrReplacementWithIndexAsync(filePath, 0)
                if (has && !hasSecond) return draftWallet.requestImportFileAddOrReplacementWithIndexAsync(filePath, 1)
            }
        }
    }
}
