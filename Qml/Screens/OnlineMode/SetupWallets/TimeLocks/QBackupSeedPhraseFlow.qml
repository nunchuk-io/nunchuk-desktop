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
import QtGraphicalEffects 1.12
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
import "../../../OnlineMode/AddHardwareKeys"
import "../../../OnlineMode/SetupWallets"
import "../../../../../localization/STR_QML.js" as STR

QPopupOverlayScreen {
    id: _infoPopup
    signal nextClicked()
    property string selected_verify_option: "SELF_VERIFIED"
    QScreenStateFlow {
        id: stateFlow
    }
    function startFlow() {
        _infoPopup.open()
        stateFlow.setScreenFlow("backup-your-inheritance-key-seed-phrase")
    }

    readonly property var map_flow: [
        {screen: "backup-your-inheritance-key-seed-phrase",    screen_component: backup_your_inheritance_key_seed_phrase},
        {screen: "important-notice-about-passphrase",          screen_component: important_notice_about_passphrase},
        {screen: "important-notice-about-passphrase-guide",    screen_component: important_notice_about_passphrase_guide},
        {screen: "re-add-the-stored-key",                      screen_component: _re_add_the_stored_key},
        {screen: "result-restore-key",                         screen_component: _resultRestoreKey},
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
                        draftWallet.requestVerifySingleSigner(selected_verify_option)
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
    function verifyResult(result) {
        if (result == 1) {
            GroupWallet.refresh()
            _infoPopup.close()
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
                            draftWallet.requestVerifySingleSignerViaConnectDevice(_refresh.contentItem.mDevicelist.currentIndex, selected_verify_option)
                        }
                    }
                }
            }
            Connections {
                target: draftWallet
                onVerifySingleSignerResult: verifyResult(result)
            }
        }
    }
    Component {
        id: _resultRestoreKey
        QScreenResultFinalize {
            successText: STR.STR_QML_1965
            successDescription: STR.STR_QML_1966
            isSuccess: true
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onNextClicked: {
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
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
                onVerifySingleSignerResult: verifyResult(result)
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
                onVerifySingleSignerResult: verifyResult(result)
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
                onVerifySingleSignerResult: verifyResult(result)
            }
        }
    }
}
