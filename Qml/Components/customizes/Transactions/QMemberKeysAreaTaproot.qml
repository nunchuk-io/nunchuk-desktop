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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _item
    property var transactionInfo
    signal keySignRequest(var signer)
    signal keyScanRequest
    signal keyExportRequest
    signal keyImportRequest
    property int pendingSignatue: 0
    property bool isDummy: false
    property string myRole: ""
    /*========================================*/
    width: 378
    height: 480

    Rectangle {
        width: 350
        height: 480
        radius: 12
        border.color: "#EAEAEA"
        color: "#FFFFFF"
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: 350
                height: 451
                radius: 12
            }
        }
        Loader {
            id: signersLoader
            anchors.fill: parent
            sourceComponent: transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES || transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST ? keysetsData : signersData
        }
    }

    Component {
        id: keysetsData
        QListView {
            id: signerlist
            anchors.fill: parent
            anchors.margins: 12
            spacing: 12
            model: transactionInfo.singleSignersAssigned
            ScrollBar.vertical: ScrollBar { active: true }
            delegate: QMemberKeysDelegateTaproot {
                implicitWidth: signerlist.width
                alreadySigned: model.single_signer_signed_status
                signername: model.singleSigner_name
                signerxfp: model.singleSigner_masterFingerPrint
                signerType: model.single_signer_type
                signerReadyToSign: model.single_signer_readyToSign
                isLocaluser: model.single_signer_is_local
                devicetype: model.single_signer_devicetype
                card_id: model.single_signer_device_cardid
                tag: model.single_signer_tag
                has_sign_btn: model.single_signer_has_sign_btn
                serverkeyMessage: transactionInfo.serverKeyMessage
                tx_status: transactionInfo.status
                is_cosigning: transactionInfo.isCosigning
                accountIndex: model.single_signer_account_index
                myRole: _item.myRole

                // For taproot
                keysetStatus: model.single_signer_keyset_status
                keysetIndex: model.single_signer_keyset_index
                keysetCount: transactionInfo.keysetsCount
                keysetM: transactionInfo.m
                keysetRemaining: model.single_signer_keyset_remaining

                onSignRequest: {
                    keySignRequest(model)
                }
                onScanRequest: {
                    keyScanRequest()
                }
                onExportRequest: {
                    keyExportRequest()
                }
                onImportRequest: {
                    keyImportRequest()
                }
            }
            Component.onCompleted: GlobalData.showOthersKeyset = true
        }
    }
    Component {
        id: signersData
        Column {
            anchors.fill: parent
            spacing: 12
            Item {
                height: 28
                width: parent.width - 24
                anchors.horizontalCenter: parent.horizontalCenter
                QLato {
                    width: parent.width
                    height: 16
                    anchors.bottom: parent.bottom
                    text: "Member keys"
                    font.pixelSize: 12
                    color: "#031F2B"
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
            QListView {
                id: signerlist
                width: parent.width - 24
                height: 326
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 16
                model: transactionInfo.singleSignersAssigned
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: QMemberKeysDelegate {
                    width: signerlist.width
                    alreadySigned: model.single_signer_signed_status
                    signername: model.singleSigner_name
                    signerxfp: model.singleSigner_masterFingerPrint
                    signerType: model.single_signer_type
                    signerReadyToSign: model.single_signer_readyToSign
                    isLocaluser: model.single_signer_is_local
                    devicetype: model.single_signer_devicetype
                    card_id: model.single_signer_device_cardid
                    tag: model.single_signer_tag
                    has_sign_btn: model.single_signer_has_sign_btn
                    serverkeyMessage: transactionInfo.serverKeyMessage
                    tx_status: transactionInfo.status
                    is_cosigning: transactionInfo.isCosigning
                    accountIndex: model.single_signer_account_index
                    myRole: _item.myRole
                    isValueKey: model.single_signer_value_key
                    onSignRequest: {
                        keySignRequest(model)
                    }
                    onScanRequest: {
                        keyScanRequest()
                    }
                    onExportRequest: {
                        keyExportRequest()
                    }
                    onImportRequest: {
                        keyImportRequest()
                    }
                }
                section
                {
                    property: "single_signer_is_local"
                    criteria: ViewSection.FullString
                    delegate: Rectangle {
                        id: sectionItem
                        property bool isLocalSigner: (section === "true")
                        width: signerlist.width
                        height: !isLocalSigner ? 16 : 0
                        Rectangle {
                            width: parent.width
                            height: 1
                            visible: !sectionItem.isLocalSigner
                            color: "#EAEAEA"
                        }
                    }
                }
            }
        }
    }
}
