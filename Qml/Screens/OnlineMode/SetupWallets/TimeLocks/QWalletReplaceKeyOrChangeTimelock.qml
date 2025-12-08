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
import QtQuick 2.4
import QtQuick.Controls 2.3
import DataPool 1.0
import NUNCHUCKTYPE 1.0
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
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2048

    property var user: ClientController.user
    property var dashInfo: GroupWallet.dashboardInfo
    property bool isKeyHolderLimited: dashInfo.myRole === "KEYHOLDER_LIMITED"

    readonly property var hb_description_map: [
        {height: 40,  description: STR.STR_QML_1889 },
        {height: 40,  description: STR.STR_QML_1890 },
    ]
    readonly property var hb_premier_description_map: [
        {height: 40,  description: STR.STR_QML_1892 },
        {height: 40,  description: STR.STR_QML_1893 },
    ]
    readonly property var free_description_map: [
        {height: 40,  description: STR.STR_QML_1892 },
        {height: 40,  description: STR.STR_QML_1893 },
    ]
    function guideImageSource() {
        var m = dashInfo.mInfo
        var n = dashInfo.nInfo
        var allowInheritance = dashInfo.allowInheritance
        if (m === 2 && n === 4) {
            return "qrc:/Images/Images/inheritance-illustration-2-of-4-and-1-of-3.png"
        } else if (m === 3 && n === 5) {
            return allowInheritance ? "qrc:/Images/Images/inheritance-illustration-3-of-5-and-2-of-4.png"
                                    : "qrc:/Images/Images/inheritance-illustration-3-of-5-and-2-of-4-off.png"
        } else {
            // Default fallback
            return "qrc:/Images/Images/inheritance-illustration-3-of-5-and-2-of-4-off.png"
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
    function closeScreen() {
        dashInfo.markRead()
        closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    }
    onCloseClicked: closeScreen()
    content: Item {
        Row {
            anchors.fill: parent
            spacing: 36
            Column {
                spacing: 16
                Rectangle {
                    width: 346
                    height: 456
                    radius: 24
                    color: "#D0E2FF"
                    QPictureSmooth {
                        width: 346
                        height: 229
                        anchors.verticalCenter: parent.verticalCenter
                        source: guideImageSource()
                    }
                }
                QLato {
                    width: 346
                    text: STR.STR_QML_2047
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Item {
                width: 346
                height: parent.height
                Flickable {
                    anchors.fill: parent
                    contentHeight: _contentColumn.height
                    clip: true
                    ScrollBar.vertical: ScrollBar { active: true }
                    Column {
                        id: _contentColumn
                        width: 346
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
                                model: dashInfo.replaceKeys
                                QReplaceRequestKey {
                                    width: 346
                                    onTapsignerClicked: {
                                        dashInfo.startReplaceKeyAtIndex(index)
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
                                        dashInfo.startReplaceKeyAtIndex(index)
                                        var has = SignerManagement.currentSigner.has !== undefined && SignerManagement.currentSigner.has
                                        if (!has) {
                                            _hardwareAddKey.key_index = modelData.key_index
                                            _hardwareAddKey.isInheritance = false
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
                                        if(modelData.wallet_type === "MULTI_SIG") {
                                            _importColdcardBackup.xfp = modelData.xfp
                                            _importColdcardBackup.open()
                                            var _input = {
                                                type: "open-import-encrypted-backup",
                                                fingerPrint: modelData.xfp,
                                            }
                                            dashInfo.requestBackupColdcard(_input)
                                        } else {
                                            GroupWallet.qAddHardware = modelData.hwType
                                            _backupSeedPhraseFlow.startFlow()
                                        }
                                        dashInfo.startReplaceKeyAtIndex(index)
                                    }
                                }
                            }
                        }
                        Item {
                            width: parent.width
                            height: 48
                            visible: false // TBD
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
                            // height: 72
                            sourceComponent: _timeLockReplaced
                            Component {
                                id: _timeLockReplaced
                                Column {
                                    spacing: 4
                                    QDashRectangle {
                                        width: 346
                                        height: 72
                                        radius: 8
                                        isDashed: false
                                        borderWitdh: isDashed ? 2 : 1
                                        borderColor: isDashed ? "#031F2B" : "#DEDEDE"
                                        color: dashInfo.timelockReplacementDisp.trim() !== dashInfo.timeLock.trim() ? "#A7F0BA" : "#FFFFFF"
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
                                                    text: dashInfo.timelockReplacementDisp
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
                                                _popupSetupAnOnChainTimelock.open()
                                            }
                                        }
                                    }
                                    Row {
                                        spacing: 4
                                        visible: dashInfo.timelockReplacementDisp.trim() !== dashInfo.timeLock.trim()
                                        QSvgImage {
                                            width: 16
                                            height: 16
                                            source: "qrc:/Images/Images/replace.svg"
                                        }
                                        QLato {
                                            text: STR.STR_QML_2054.arg(dashInfo.timeLock)
                                            font.pixelSize: 12
                                            color: "#031F2B"
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
    onPrevClicked: {
        closeScreen()
        // var _continue = {
        //     type: "cancel-key-replacement",
        // }
        // QMLHandle.sendEvent(EVT.EVT_DASHBOARD_ALERT_INFO_ENTER, _continue)
    }

    bottomRight: Item{}

    QPopupSetupOnChainTimelockWallet {
        id: _popupSetupAnOnChainTimelock
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
}
