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
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../customizes/Popups"
import "../../customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id: signerConfigRoot
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1696
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    content: Item {
        Row {
            height: 503
            spacing: 22
            Flickable {
                id: flickerLeft
                width: 350 + 6
                height: 503
                clip: true
                flickableDirection: Flickable.VerticalFlick
                interactive: true
                contentHeight: contentSigners.childrenRect.height
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: contentSigners
                    width: 350
                    spacing: 12
                    Column {
                        width: 350
                        QLato {
                            height: 20
                            text: STR.STR_QML_337
                            font.weight: Font.Bold
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                        QListView {
                            id: supportedSigners
                            width: 350
                            height: contentHeight
                            clip: true
                            interactive: false
                            model: newWalletInfo.assignAvailableSigners
                            delegate: QSignerConfigureDelegate {
                                signerData {
                                    single_name: singleSigner_name
                                    single_type: single_signer_type
                                    single_tag: single_signer_tag
                                    single_devicetype: single_signer_devicetype
                                    single_masterFingerPrint: singleSigner_masterFingerPrint
                                    single_account_index: single_signer_account_index
                                    single_checked: single_signer_checked
                                    single_is_local: single_signer_is_local
                                    single_device_cardid: single_signer_device_cardid
                                }
                                visible: single_signer_taproot_supported && !single_signer_checked
                                height: visible ? 84 : 0
                                onBtnClicked: {
                                    model.single_signer_checked = !model.single_signer_checked
                                }
                                enabled: true
                                opacity: enabled ? 1.0 : 0.4
                            }
                        }
                    }
                    QLine {
                        width: 350
                    }
                    Column {
                        width: 350
                        QLato {
                            height: 20
                            text: STR.STR_QML_1697
                            font.weight: Font.Bold
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                        QListView {
                            id: notSupportedSigners
                            width: 350
                            height: contentHeight
                            interactive: false
                            clip: true
                            model: newWalletInfo.assignAvailableSigners
                            delegate: QSignerConfigureDelegate {
                                signerData {
                                    single_name: singleSigner_name
                                    single_type: single_signer_type
                                    single_tag: single_signer_tag
                                    single_devicetype: single_signer_devicetype
                                    single_masterFingerPrint: singleSigner_masterFingerPrint
                                    single_account_index: single_signer_account_index
                                    single_checked: single_signer_checked
                                    single_is_local: single_signer_is_local
                                    single_device_cardid: single_signer_device_cardid
                                }
                                isShowCheckBox: false
                                visible: !single_signer_taproot_supported
                                height: visible ? 84 : 0
                                onBtnClicked: {

                                }
                                enabled: false
                                opacity: enabled ? 1.0 : 0.4
                            }
                        }
                    }
                }
            }


            Flickable {
                id: flickerRight
                width: 350 + 6
                height: 503
                clip: true
                flickableDirection: Flickable.VerticalFlick
                interactive: true
                contentHeight: contentSignersAssign.childrenRect.height
                ScrollBar.vertical: ScrollBar { active: true }
                function updateVisibleCount() {
                    return newWalletInfo.assignAvailableSigners.signerSelectedCount
                }
                Column {
                    id: contentSignersAssign
                    width: 350
                    spacing: 16
                    Column {
                        width: 350
                        QLato {
                            height: 20
                            text: STR.STR_QML_337
                            font.weight: Font.Bold
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                        QListView {
                            id: signersAssign
                            width: 350
                            height: signersAssign.contentHeight
                            clip: true
                            model: newWalletInfo.assignAvailableSigners
                            ScrollBar.vertical: ScrollBar { active: true }
                            delegate: QSignerConfigureDelegate {
                                signerData {
                                    single_name: singleSigner_name
                                    single_type: single_signer_type
                                    single_tag: single_signer_tag
                                    single_devicetype: single_signer_devicetype
                                    single_masterFingerPrint: singleSigner_masterFingerPrint
                                    single_account_index: single_signer_account_index
                                    single_checked: single_signer_checked
                                    single_is_local: single_signer_is_local
                                    single_device_cardid: single_signer_device_cardid
                                }
                                visible: single_signer_checked
                                height: visible ? 84 : 0
                                onBtnClicked: {
                                    model.single_signer_checked = !model.single_signer_checked
                                    newWalletInfo.walletM = 0
                                    newWalletInfo.walletN = 0
                                }
                                enabled: true
                                opacity: enabled ? 1.0 : 0.4
                            }
                        }
                    }
                    QLine {
                        width: 350
                    }
                    Column {
                        width: 350
                        spacing: 17
                        Column {
                            width: 350
                            spacing: 4
                            QLato {
                                height: 20
                                text: STR.STR_QML_1652
                                font.weight: Font.Bold
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                            }
                            QLato {
                                height: 20
                                text: STR.STR_QML_1653
                                font.weight: Font.Bold
                                font.pixelSize: 12
                                color: "#757575"
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                            }
                        }
                        Item {
                            width: 350
                            height: 48
                            Row {
                                height: 48
                                spacing: 16
                                Rectangle {
                                    width: 36
                                    height: 36
                                    radius: 36
                                    anchors.verticalCenter: parent.verticalCenter
                                    color: "transparent"
                                    border.width: 2
                                    border.color: minus.pressed ?"#595959" : "#031F2B"
                                    Rectangle {
                                        width: 12
                                        height: 2
                                        color: minus.pressed ?"#595959" : "#031F2B"
                                        anchors.centerIn: parent
                                    }
                                    MouseArea {
                                        id: minus
                                        anchors.fill: parent
                                        onClicked: {
                                            if(newWalletInfo.walletM > 0){
                                                newWalletInfo.walletM--;
                                            }
                                        }
                                    }
                                }
                                Rectangle {
                                    id: root
                                    width: 48
                                    height: 48
                                    radius: 8
                                    color: "#FFFFFF"
                                    border.color: "#DEDEDE"
                                    border.width: 1
                                    QLato {
                                        anchors.centerIn: parent
                                        text: newWalletInfo.walletM
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                Rectangle {
                                    width: 36
                                    height: 36
                                    radius: 36
                                    anchors.verticalCenter: parent.verticalCenter
                                    color: "transparent"
                                    border.width: 2
                                    border.color: plusbtn.pressed ?"#595959" : "#031F2B"
                                    Rectangle {
                                        width: 12
                                        height: 2
                                        color: plusbtn.pressed ?"#595959" : "#031F2B"
                                        anchors.centerIn: parent
                                    }
                                    Rectangle {
                                        width: 2
                                        height: 12
                                        color: plusbtn.pressed ?"#595959" : "#031F2B"
                                        anchors.centerIn: parent
                                    }
                                    MouseArea {
                                        id: plusbtn
                                        anchors.fill: parent
                                        onClicked: {
                                            var count = flickerRight.updateVisibleCount()
                                            newWalletInfo.walletN = count
                                            if(newWalletInfo.walletM < count){
                                                newWalletInfo.walletM++;
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
    onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    bottomRight: Row {
        spacing: 28
        Row {
            spacing: 8
            height: 28
            QLato {
                width: paintedWidth
                text: STR.STR_QML_353
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            QBadge {
                text: (newWalletInfo.walletN === 1) ? STR.STR_QML_070 :
                                                      qsTr("%1/%2 %3").arg(newWalletInfo.walletM).arg(newWalletInfo.walletN).arg(STR.STR_QML_069);

                color: "#EAEAEA"
                font.weight: Font.Normal
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        QTextButton {
            width: 133
            height: 48
            label.text: STR.STR_QML_1698
            label.font.pixelSize: 16
            type: eTypeE
            enabled: newWalletInfo.walletM > 0
            onButtonClicked: {
                _keysetPopup.open()
                _keysetPopup.switchValueKeyset()
                QMLHandle.sendEvent(EVT.EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER)
            }
        }
    }

    property var newWalletInfo  : AppModel.newWalletInfo
    property string flow_screen: newWalletInfo.screenFlow
    readonly property var map_flow: [
        {screen: "value-keyset",              screen_component: valueKeyset},
        {screen: "review-wallet",             screen_component: review_wallet},
        {screen: "bsms-file-success",         screen_component: bsms_file_success},
        {screen: "register-wallet-hardware",  screen_component: register_wallet_hardware},
        {screen: "configure-value-keyset",    screen_component: configure_value_keyset},
    ]

    property var selectedComponent: {
        var found = map_flow.find(e =>  { return (e.screen === flow_screen)});
        return found ? found.screen_component : null;
    }

    QPopupOverlayScreen {
        id: _keysetPopup
        content: selectedComponent

        function switchValueKeyset() {
            newWalletInfo.screenFlow = "value-keyset"
        }
        function switchConfigValueKeyset() {
            newWalletInfo.screenFlow = "configure-value-keyset"
        }
        function switchReviewWallet() {
            newWalletInfo.screenFlow = "review-wallet"
        }
        function switchBsmsFileSuccess() {
            newWalletInfo.screenFlow = "bsms-file-success"
        }
        function switchRegisterWalletHardware() {
            newWalletInfo.screenFlow = "register-wallet-hardware"
        }
    }
    property string saveKeyset: ""
    Component {
        id : valueKeyset
        QEnableValueKeyset {
            onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
            onPrevClicked: {
                _keysetPopup.close()
                newWalletInfo.backScreen()
            }
            onNextClicked: {
                saveKeyset = enableValueKeyset
                if (enableValueKeyset === "enable-value-keyset") {
                    newWalletInfo.enableValuekeyset = true
                    _keysetPopup.switchConfigValueKeyset()
                }
                else {
                    newWalletInfo.enableValuekeyset = false
                    _keysetPopup.switchReviewWallet()
                }
            }
        }
    }
    Component {
        id: configure_value_keyset
        QConfigureValueKeyset {
            onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
            onPrevClicked: newWalletInfo.backScreen()
            onNextClicked: {
                _keysetPopup.switchReviewWallet()
            }
        }
    }
    Component {
        id: review_wallet
        QDraftWalletReviewWallet {
            onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
            onPrevClicked: newWalletInfo.backScreen()
            onNextClicked: {
                AppModel.startCreateWallet(false, "")
                _keysetPopup.switchBsmsFileSuccess()
            }
        }
    }
    Component {
        id: bsms_file_success
        QSaveYourWalletBSMSFile {
            onNextClicked: {
                _keysetPopup.switchRegisterWalletHardware()
            }
        }
    }
    Component {
        id: register_wallet_hardware
        QRegisterWalletOnHardware {

        }
    }
}
