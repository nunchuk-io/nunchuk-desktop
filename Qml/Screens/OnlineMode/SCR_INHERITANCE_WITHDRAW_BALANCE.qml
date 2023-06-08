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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property string walletId: ""
    property int currentSelect: -1
    function isValid()
    {
        var balance = ServiceSetting.inheritance.balanceSats // sats
        var minFee = parseInt(AppModel.minFee)*1000; // sats
        return balance <= minFee
    }

    QOnScreenContentTypeB {
        id:_withdraw
        visible: ServiceSetting.claimInheritanceFlow === ServiceType.WITHDRAW_TO_NUNCHUK_WALLET
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_779
        extraHeader: Item {}
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, EVT.STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE)
        }
        content: Item {
            Column {
                spacing: 24
                QLato {
                    text: STR.STR_QML_782
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Repeater {
                    model: AppModel.walletList
                    QRadioButtonTypeD {
                        width: 443
                        height: 92
                        labelTop: model.wallet_name
                        labelCenter: qsTr("%1/%2 %3").arg(model.wallet_M).arg(model.wallet_N).arg(STR.STR_QML_069)
                        labelBottom: model.wallet_Balance + RoomWalletData.unitValue
                        walletIcon: model.wallet_isSharedWallet ? "qrc:/Images/Images/OnlineMode/Joint wallet_031F2B.png" :
                                                                 model.wallet_isAssistedWallet ? "qrc:/Images/Images/OnlineMode/Joint wallet_031F2B.png" :
                                                                                              model.wallet_Escrow ?  "qrc:/Images/Images/OnlineMode/Escrow Wallet.png" : ""
                        walletType: model.wallet_isSharedWallet ? STR.STR_QML_438 :
                                                                 model.wallet_isAssistedWallet ? STR.STR_QML_679 : ""
                        selected: currentSelect === index
                        onButtonClicked: {
                            currentSelect = index
                            walletId = model.wallet_id
                        }
                    }
                }
            }
        }
        onPrevClicked: {
            QMLHandle.sendEvent(EVT.EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, EVT.STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE)
        }
        onNextClicked: {
            if (isValid()) {
                AppModel.showToast(-1,
                               STR.STR_QML_816,
                               EWARNING.ERROR_MSG,
                               "");
            } else {
                QMLHandle.sendEvent(EVT.EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST,walletId)
            }
        }
    }
    property string destinationAddress: ""
    QOnScreenContentTypeB {
        id:_newTran
        visible: ServiceSetting.claimInheritanceFlow === ServiceType.WITHDRAW_TO_ADDRESS
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_786
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, EVT.STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE)
        }
        content: Item {
            Item {
                width: parent.width
                height: 20
                QLato {
                    text: STR.STR_QML_787
                    anchors.left: parent.left
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    text: STR.STR_QML_214
                    width: 221
                    anchors.right: parent.right
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
            Column {
                id: destColumn
                anchors{
                    top: parent.top
                    topMargin: 24
                }
                spacing: 36
                Repeater {
                    id: destination
                    property int numberDestination: 1
                    readonly property int destinationMAX: 10
                    property bool fullfill: false
                    model: 1
                    Row {
                        id: destinationRow
                        property alias toAddress: toAddr.textOutput
                        property string toAmount: amount.placeholder.text
                        onToAddressChanged: {
                            destination.fullfill = destinationRow.toAddress !== "" && destinationRow.toAmount !== ""
                        }
                        onToAmountChanged: {
                            destination.fullfill = destinationRow.toAddress !== "" && destinationRow.toAmount !== ""
                        }
                        spacing: 28
                        Item {
                            width: 479
                            height: toAddr.height
                            QTextInputBox {
                                id: toAddr
                                width: parent.width - qricoimport.width
                                heightMin: 56
                                mode: eEDIT_MODE
                                placeholder.text: STR.STR_QML_260
                                border.color: "#DEDEDE"
                                onTextOutputChanged: {
                                    destinationAddress = toAddr.textOutput
                                }
                            }
                            Rectangle {
                                width: 56
                                height: toAddr.height
                                anchors.left: toAddr.right
                                anchors.verticalCenter: toAddr.verticalCenter
                                color: "#C9DEF1"
                                radius: 8
                                QImage {
                                    id: qricoimport
                                    source: "qrc:/Images/Images/QrIco.png"
                                    anchors.centerIn: parent
                                    transformOrigin: Item.Center
                                    scale: qrMouse.pressed ? 0.95 : 1
                                    width: 56
                                    height: 56
                                    MouseArea {
                                        id: qrMouse
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                        anchors.fill: parent
                                        onClicked: {
                                            qrscaner.addressRequestIndex = index
                                            qrscaner.open()
                                        }
                                    }
                                }
                            }
                        }
                        QTextInputBox {
                            id: amount
                            width: 221
                            heightMin: 56
                            mode: eREADONLY_MODE
                            placeholder.text: ServiceSetting.inheritance.balance
                            color: "#DEDEDE"
                            border.color: "#DEDEDE"
                            rightPading: 40
                            validator: (AppSetting.unit === NUNCHUCKTYPE.SATOSHI) ? intvalidator : doubleValidator
                            errorText.text: STR.STR_QML_110
                            QText {
                                color: "#839096"
                                width: 23
                                height: 16
                                anchors {
                                    top: parent.top
                                    topMargin: (amount.textActiveFocus) || (amount.textOutput !== "") ? 28: 20
                                    right: parent.right
                                    rightMargin: 16
                                }
                                text: RoomWalletData.unitValue
                                font.pixelSize: 16
                                font.family: "Lato"
                            }
                        }
                        QImage {
                            id: removeRowBtn
                            width: 32
                            height: 32
                            source: "qrc:/Images/Images/Delete.png"
                            transformOrigin: Item.Center
                            scale: removeRow.pressed ? 0.95 : 1
                            visible: destination.count > 1
                            enabled: visible
                            MouseArea {
                                id: removeRow
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                anchors.fill: parent
                                onClicked: {
                                    requestRemoveDestination(destination,index)
                                }
                            }
                        }
                        Component.onCompleted: {
                            if(index < destinations.length){
                                destinationRow.toAddress = destinations[index]["toAddress"]
                                destinationRow.toAmount = destinations[index]["toAmount"]
                            }
                            destination.fullfill = destination.fullfill & destinationRow.toAddress !== "" & destinationRow.toAmount !== ""
                        }
                    }
                }
                QButtonMedium {
                    width: 185
                    height: 40
                    type: eOUTLINE_NORMAL
                    label: STR.STR_QML_263
                    visible: false
                    onButtonClicked: {
                        requestAddMultiDestinations(destination)
                    }
                }
            }
            QQrImportScanner {
                id: qrscaner
                property int addressRequestIndex: -1
                onTagFound: {
                    if(qrscaner.addressRequestIndex !== -1 && qrscaner.addressRequestIndex < destination.model){
                        destination.itemAt(qrscaner.addressRequestIndex).toAddress = tag
                        qrscaner.addressRequestIndex = -1
                    }
                    qrscaner.close()
                }
            }
        }
        bottomRight: QTextButton {
            width: 164
            height: 48
            label.text: STR.STR_QML_257
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                _newTran.nextClicked()
            }
        }
        onPrevClicked: {
            QMLHandle.sendEvent(EVT.EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, EVT.STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE)
        }
        onNextClicked: {
            if (isValid()) {
                AppModel.showToast(-1,
                               STR.STR_QML_816,
                               EWARNING.ERROR_MSG,
                               "");
            } else {
                QMLHandle.sendEvent(EVT.EVT_INHERITANCE_NEW_TRANSACTION_REQ,destinationAddress)
            }
        }


    }

    property var destinations: []
    RegExpValidator { id: intvalidator;      regExp: /^[1-9][0-9]*$/ }
    RegExpValidator { id: doubleValidator;   regExp: /^(?:0|[1-9][0-9]*)(\.\d{1,8})?$/ }
    function requestAddMultiDestinations(destination){
        destinations = []
        for(var i = 0; i < destination.count; i++){
            var savedObj = { "toAddress": destination.itemAt(i).toAddress,
                             "toAmount" : destination.itemAt(i).toAmount};
            destinations[i] = savedObj
        }
        if(destination.count < destination.destinationMAX) {destination.model++}
    }
    function requestRemoveDestination(destination,index){
        var idx = 0
        destinations = []
        for(var i = 0; i < destination.count; i++){
            if(i === index){ continue }
            var savedObj = { "toAddress": destination.itemAt(i).toAddress,
                             "toAmount" : destination.itemAt(i).toAmount};
            destinations[idx] = savedObj
            idx++
        }
        if(destination.count > 1){destination.model--}
    }
}
