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
import QRCodeItem 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Screens/OnlineMode/Coins"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContent {
    id: contenCenter
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    readonly property var iconChecked: ["qrc:/Images/Images/Checked_n.png", "qrc:/Images/Images/Checked_d.png"]
    readonly property var iconUnChecked: ["qrc:/Images/Images/UnChecked_n.png", "qrc:/Images/Images/UnChecked_d.png"]
    label.text: QMLHandle.onsRequester() === EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT ? STR.STR_QML_210 : STR.STR_QML_211
    extraHeader:  Row {
        spacing: 8
        Rectangle {
            width: visible ? 84 : 0
            height: visible ? 24 : 0
            color: "#031F2B"
            radius: 4
            visible: AppModel.walletInfo.walletEscrow
            anchors.verticalCenter: parent.verticalCenter
            Row {
                anchors.centerIn: parent
                spacing: 3
                QIcon {
                    iconSize: 16
                    source: "qrc:/Images/Images/Escrow Wallet.png"
                }
                QLato {
                    text: STR.STR_QML_212
                    color: "#F8D418"
                    width: 49
                    height: 10
                    font.weight: Font.Bold
                    font.pixelSize: 10
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
        QMontserrat {
            text: "(" + AppModel.walletInfo.walletName + ")"
            color: "#031F2B"
            font.weight: Font.DemiBold
            font.pixelSize: 14
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    onCloseClicked: {
        if(NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex){
            QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CREATE_TRANSACTION)
        }
        else{
            QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CREATE_TRANSACTION)
        }
    }
    property var transactionInfo: AppModel.transactionInfo
    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 100
        spacing: 30
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
                    height: 480
                    radius: 12
                }
            }
            Flickable {
                anchors.fill: parent
                flickableDirection: Flickable.VerticalFlick
                interactive: contentHeight > height
                contentHeight: contentDisp.height
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: contentDisp
                    width: parent.width
                    spacing: 12
                    anchors.horizontalCenter: parent.horizontalCenter
                    QSendToAddressBlock {
                        id: sendToAddress
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    QChangeAddressBlock {
                        id: changeAddress
                        visible: transactionInfo.hasChange
                        editShow: transactionInfo.hasDraftCoinChange()
                        anchors.horizontalCenter: parent.horizontalCenter
                        onEditClicked: {
                            if (transactionInfo.hasDraftCoinChange()) {
                                changeTags.open()
                            }
                        }
                    }
                    QTransactionNoteBlock {
                        id: transationNote
                        visible: transactionInfo.hasChange
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    QSignaturesBlock {
                        id: signatures
                        visible: (transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    QShowInputCoinsBlock {
                        id: inputCoins
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
        Item {
            width: 350
            height: 480
            clip: true
            Column {
                anchors.fill: parent
                spacing: 16
                Column {
                    id: feeSetting
                    width: parent.width
                    spacing: 12
                    QSwitchTypeIconLabel {
                        id: manualfeesetting
                        width: parent.width
                        height: 36
                        label: STR.STR_QML_225
                        icon: "qrc:/Images/Images/signing-policy-dark.svg"
                    }
                    Column {
                        visible: manualfeesetting.getValue()
                        width: parent.width
                        spacing: 12
                        QCheckboxTooltip {
                            id: subtract
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: STR.STR_QML_226
                            tooltip: STR.STR_QML_227
                            checked: transactionInfo.subtractFromFeeAmount
                            rightOfParent: true
                            pointerPositionIndex: 6
                            pointerPositionRatio: 10
                            onButtonClicked: {
                                requestDraftTransaction()
                            }
                        }
                        QManualFeeRate {
                            id: manualfee
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        QCheckboxTooltip {
                            id: antisnipfee
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: STR.STR_QML_1729
                            tooltip: STR.STR_QML_1730
                            pointerPositionIndex: 6
                            pointerPositionRatio: 10
                            checked: AppSetting.enableAntiFeeSniping
                            onButtonClicked: {
                                // DON'T NEED MAKE DRAFT
                            }
                        }
                    }
                }
                QCoinSelectionTransaction {
                    id: coinSelection
                    height: 480 - feeSetting.childrenRect.height - 16
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
    }

    QButtonTextLink {
        height: 48
        label: STR.STR_QML_059
        fontPixelSize: 16
        anchors {
            left: parent.left
            leftMargin: 36
            bottom: parent.bottom
            bottomMargin: 36
        }
        visible: (QMLHandle.onsRequester() !== EVT.STATE_ID_SCR_TRANSACTION_INFO)
        onButtonClicked: {
            if(QMLHandle.onsRequester() === EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT){
                QMLHandle.sendEvent(EVT.EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE)
            }
            else {
                QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_BACK_REQUEST)
            }
        }
    }

    QTextButton {
        width: 265
        height: 48
        label.text: AppModel.walletInfo.tranReplace ? STR.STR_QML_784 : STR.STR_QML_235
        label.font.pixelSize: 16
        label.font.family: "Lato"
        type: eTypeE
        anchors {
            right: parent.right
            rightMargin: 36
            bottom: parent.bottom
            bottomMargin: 36
        }
        enabled:{
            if(manualfee.isChecked){ return (manualfee.textOutput !== "") }
            else{ return true }
        }
        onButtonClicked: {
            if(manualfee.validInput){
                if (onceClicked && transactionInfo.hasDraftCoinChange()) {
                    onceClicked = false
                    changeTags.open()
                } else {
                    requestCreateTransaction()
                }
            }
            else{
                confirmLowFee.open()
            }
        }
    }

    Popup {
        id: createTxBusyBox
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        Rectangle {
            id: boxmask
            width: 300
            height: 196
            radius: 24
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 300
                    height: 196
                    radius: 24
                }
            }
            Column {
                spacing: 12
                anchors.centerIn: parent
                QBusyIndicator {
                    width: 70
                    height: 70
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                QLato {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    text: STR.STR_QML_236
                }
                QLato {
                    width: 252
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 16
                    text: STR.STR_QML_122
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
        DropShadow {
            anchors.fill: boxmask
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: boxmask
        }
    }
    property bool onceClicked: true
    QConfirmYesNoPopup {
        id: confirmLowFee
        contentText: STR.STR_QML_809
        onConfirmNo: close()
        onConfirmYes: {
            close()
            if (onceClicked && transactionInfo.hasDraftCoinChange()) {
                onceClicked = false
                changeTags.open()
            } else {
                requestCreateTransaction()
            }
        }
    }
    Timer {
        id: timerCreateTx
        property var destinationData
        interval: 500
        onTriggered: {
            QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_SIGN_REQUEST, timerCreateTx.destinationData)
            createTxBusyBox.close()
        }
    }
    function requestDraftTransaction(){
        var msg = {
            "subtractFromFeeAmout"   : subtract.checked,
            "feeRate"                : manualfee.textOutput,
            "manualFee"              : manualfee.isChecked,
            "manualOutput"           : false,
            "antiFeeSnipping"        : false, // always false
        };
        QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX, msg)
    }
    function requestCreateTransaction(){
        var msg = {
            "subtractFromFeeAmout"   : subtract.checked,
            "feeRate"                : manualfee.textOutput,
            "manualFee"              : manualfee.isChecked,
            "manualOutput"           : false,
            "antiFeeSnipping"        : antisnipfee.checked,
        };
        createTxBusyBox.open()
        timerCreateTx.destinationData = msg
        timerCreateTx.restart()
    }

    QChangeTagsInTransaction {
        id: changeTags
    }
}
