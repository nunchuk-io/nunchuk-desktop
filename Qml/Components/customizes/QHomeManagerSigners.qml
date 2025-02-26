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
import QtQuick 2.12
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import Qt.labs.platform 1.1
import DataPool 1.0
import "../../Components/customizes"
import "../../Components/customizes/Signers"
import "../../Components/origins"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    color: "transparent"
    property int count: masterList.count + remoteList.count
    clip: true
    Column {
        anchors.fill: parent
        spacing: 12
        Item {
            width: parent.width
            height: 24
            QText {
                id: keysTextTitle
                anchors {
                    left: parent.left
                    leftMargin: 16
                    verticalCenter: parent.verticalCenter
                }
                text: STR.STR_QML_015
                font.family: "Lato"
                font.pixelSize: 16
                color: "#FFFFFF"
                font.weight: Font.Bold
            }
            QIconButton{
                width: 24
                height: 24
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                icon:"qrc:/Images/Images/add-light.svg"
                onClicked: {
                    QMLHandle.sendEvent(EVT.EVT_HOME_ADD_NEW_SIGNER_REQUEST, { type: "add-key-free" })
                }
            }
        }
        Item {
            width: parent.width
            height: parent.height - 24
            Loader {
                anchors.fill: parent
                sourceComponent: flickerSignerList.signerReady ? null : loadingSignerBusy
            }
            Flickable {
                id: flickerSignerList
                anchors.fill: parent
                property bool signerReady: (masterList.count > 0) || (remoteList.count > 0) || userKeys.count > 0 || groupKeys.count > 0
                visible: signerReady
                clip: true
                flickableDirection: Flickable.VerticalFlick
                interactive: true
                contentHeight: contentDisplay.height + 12
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: contentDisplay
                    width: parent.width
                    spacing: 4
                    QListView {
                        id: userKeys
                        width: parent.width
                        height: contentHeight
                        model: UserWallet.hardwareReq
                        interactive: false
                        visible: ClientController.isNunchukLoggedIn
                        spacing: 4
                        delegate: QAddAssistedWalletSigner{
                            addTitle: modelData.title
                            addText: modelData.textBtn
                            height: modelData.mHeight
                            onCancel: {
                                UserWallet.cancelRequestKey(modelData.request_id);
                            }
                            onAdd: {
                                UserWallet.addHardwareFromBanner(modelData.request_id)
                                QMLHandle.sendEvent(EVT.EVT_ASK_HARDWARE_REQ)
                            }
                        }
                    }
                    QListView {
                        id: groupKeys
                        width: parent.width
                        height: contentHeight
                        model: GroupWallet.hardwareReq
                        interactive: false
                        visible: ClientController.isNunchukLoggedIn
                        spacing: 4
                        delegate: QAddAssistedWalletSigner{
                            addTitle: modelData.title
                            addText: modelData.textBtn
                            height: modelData.mHeight
                            onCancel: {
                                GroupWallet.cancelRequestKey(modelData.request_id, modelData.group_id);
                            }
                            onAdd: {
                                GroupWallet.addHardwareFromBanner(modelData.request_id)
                                QMLHandle.sendEvent(EVT.EVT_ASK_HARDWARE_REQ)
                            }
                        }
                    }
                    QListView {
                        id: masterList
                        width: parent.width
                        height: 48*masterList.count
                        model: AppModel.masterSignerList
                        interactive: false
                        delegate: QMasterSignerDelegate {
                            width: masterList.width
                            height: 48
                            signername : model.master_signer_name
                            card_id_or_xfp: model.master_signer_type === NUNCHUCKTYPE.NFC ? model.master_signer_device.cardId : model.master_signer_fingerPrint
                            devicetype: model.master_signer_deviceType
                            signerType: model.master_signer_type
                            signerTag: model.master_signer_tag
                            isPrimaryKey: model.master_signer_primary_key
                            onButtonClicked: {
                                QMLHandle.sendEvent(EVT.EVT_HOME_MASTER_SIGNER_INFO_REQUEST, model.master_signer_fingerPrint)
                            }
                        }
                    }
                    QListView {
                        id: remoteList
                        width: parent.width
                        height: 48*remoteList.count
                        model: AppModel.remoteSignerList
                        interactive: false
                        delegate: QMasterSignerDelegate {
                            width: remoteList.width
                            height: visible ? 48 : 0
                            nameWidth: 170
                            signername : model.singleSigner_name
                            devicetype: model.single_signer_devicetype
                            card_id_or_xfp: model.single_signer_type === NUNCHUCKTYPE.NFC ? model.single_signer_device_cardid : model.singleSigner_masterFingerPrint
                            signerType: model.single_signer_type
                            signerTag: model.single_signer_tag
                            accountIndex: model.single_signer_account_index
                            onButtonClicked: {
                                if (model.single_signer_type === NUNCHUCKTYPE.COLDCARD_NFC) {
                                    QMLHandle.sendEvent(EVT.EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST, model.singleSigner_masterFingerPrint)
                                } else {
                                    QMLHandle.sendEvent(EVT.EVT_HOME_REMOTE_SIGNER_INFO_REQUEST, model.singleSigner_masterFingerPrint)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    readonly property int _LIMIT_WAITING_BUSY: 5000
    Component {
        id: loadingSignerBusy
        Item {
            anchors.fill: parent
            QMasterSignerDelegate {
                width: 304
                height: 54
                enabled: false
                visible: !busyIdct.running
                signername : STR.STR_QML_016
                card_id_or_xfp: "DEADBEEF"
                Rectangle {
                    anchors.fill: parent
                    color: "#031F2B"
                    opacity: 0.5
                }
            }
            BusyIndicator {
                id: busyIdct
                anchors.centerIn: parent
                running: true
            }
            Timer {
                interval: _LIMIT_WAITING_BUSY
                running: true
                onTriggered: busyIdct.running = false
            }
        }
    }
}
