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
    width: 350
    height: 480
    Column {
        width: parent.width
        spacing: 8
        Row {
            spacing: 8
            height: 20
            QText {
                color: "#031F2B"
                font.family: "Lato"
                font.pixelSize: 16
                font.weight: Font.Bold
                text: STR.STR_QML_289
                anchors.verticalCenter: parent.verticalCenter
            }
            QIcon {
                iconSize: 16
                visible: (transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                source: "qrc:/Images/Images/pending_actions-24px.png"
                anchors.verticalCenter: parent.verticalCenter
            }
            QText {
                visible: (transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                color: "#595959"
                font.family: "Lato"
                font.pixelSize: 12
                text: {
                    var pend = isDummy ? pendingSignatue : Math.max(0, transactionInfo.m - transactionInfo.numberSigned)
                    return STR.str_QML_220(pend)
                }
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Rectangle {
            width: 350
            height: 451
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
            QListView {
                id: signerlist
                anchors.fill: parent
                anchors.margins: 12
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
                    property: "single_signer_is_local" //"single_signer_type"
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
