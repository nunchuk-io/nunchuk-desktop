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
import QRCodeItem 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/QRCodes"
import "../../../localization/STR_QML.js" as STR

QScreen {

    QOnScreenContent {
        id: contenCenter
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_256
        extraHeader: Row {
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
                    QImage {
                        source: "qrc:/Images/Images/Escrow Wallet.png"
                        width: 16
                        height: 16
                    }
                    QText {
                        text: STR.STR_QML_212
                        color: "#F8D418"
                        width: 49
                        height: 10
                        font.weight: Font.Bold
                        font.family: "Lato"
                        font.pixelSize: 10
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            QText {
                text: "(" + AppModel.walletInfo.walletName + ")"
                color: "#031F2B"
                font.weight: Font.DemiBold
                font.family: "Montserrat"
                font.pixelSize: 14
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        onCloseClicked: {
            if(NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex){
                QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SEND)
            }
            else{
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SEND)
            }
        }
        QText {
            id: createtxlabel
            anchors {
                left: parent.left
                leftMargin: 43
                top: parent.top
                topMargin: 92
            }
            text: STR.STR_QML_257
            color: "#031F2B"
            font.weight: Font.Bold
            font.pixelSize: 16
        }
        QIcon {
            iconSize: 24
            id: checkboxMultiDes
            property bool checked: false
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 132
            }
            source: checkboxMultiDes.checked ? "qrc:/Images/Images/Checked_n.png" : "qrc:/Images/Images/UnChecked_n.png"
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    checkboxMultiDes.checked = !checkboxMultiDes.checked
                    if(checkboxMultiDes.checked){
                        destination.itemAt(0).toAmount = ""
                        requestAddMultiDestinations()
                    }
                    else{ requestDisableMultiDestinations() }
                }
            }
        }
        QText {
            anchors {
                left: checkboxMultiDes.right
                leftMargin: 8
                verticalCenter: checkboxMultiDes.verticalCenter
            }
            text: STR.STR_QML_258
            color: "#031F2B"
            font.pixelSize: 14
        }
        Row {
            id: balanceinfo
            spacing: 10
            height: 24
            width: 260
            anchors {
                right: parent.right
                rightMargin: 88
                verticalCenter: checkboxMultiDes.verticalCenter
            }
            QText {
                height: 21
                width: 110
                text: STR.STR_QML_259
                color: "#000000"
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 14
                anchors.verticalCenter: parent.verticalCenter
            }
            QText {
                height: 24
                width: 140
                text: AppModel.walletInfo.walletBalance + RoomWalletData.unitValue
                color: "#000000"
                font.family: "Lato"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignRight
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Flickable {
            width: 720
            height: 350
            flickableDirection: Flickable.VerticalFlick
            clip: true
            contentHeight: destColumn.height
            ScrollBar.vertical: ScrollBar { active: true }
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 171
            }
            Column {
                id: destColumn
                spacing: checkboxMultiDes.checked ? 16 : 36
                Repeater {
                    id: destination
                    property int numberDestination: 1
                    readonly property int destinationMAX: 10
                    property bool fullfill: false
                    model: 1
                    Row {
                        id: destinationRow
                        property alias toAddress: toAddr.textOutput
                        property alias toAmount: amount.textOutput
                        onToAddressChanged: {
                            destination.fullfill = destinationRow.toAddress !== "" && destinationRow.toAmount !== ""
                        }
                        onToAmountChanged: {
                            destination.fullfill = destinationRow.toAddress !== "" && destinationRow.toAmount !== ""
                        }
                        spacing: 8
                        Item {
                            width: 400
                            height: toAddr.height
                            QTextInputBox {
                                id: toAddr
                                width: parent.width - qricoimport.width
                                heightMin: 56
                                mode: eEDIT_MODE
                                placeholder.text: checkboxMultiDes.checked ? qsTr("%1 %2").arg(STR.STR_QML_260).arg(index + 1) : STR.STR_QML_260
                                border.color: "#DEDEDE"
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
                            width: !removeRowBtn.visible ? 309 : 269
                            heightMin: 56
                            mode: !checkboxSendAll.checked ? eEDIT_MODE : eREADONLY_MODE
                            placeholder.text: STR.STR_QML_261
                            rightPading: 40
                            validator: (AppSetting.unit === NUNCHUCKTYPE.SATOSHI) ? intvalidator : doubleValidator
                            border.color: "#DEDEDE"
//                            onTypingFinished: { if(AppSetting.unit === NUNCHUCKTYPE.SATOSHI){ amount.textOutput = Number(amount.textOutput).toLocaleString(Qt.locale("en-US")) } }

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
                            Row {
                                id: sendAll
                                width: 90
                                height: 24
                                spacing: 8
                                visible: !checkboxMultiDes.checked
                                enabled: visible
                                anchors {
                                    right: parent.right
                                    top: parent.bottom
                                    topMargin: 6
                                }
                                QIcon {
                                    iconSize: 24
                                    id: checkboxSendAll
                                    property bool checked: false
                                    anchors.verticalCenter: parent.verticalCenter
                                    source: checkboxSendAll.checked && visible ? "qrc:/Images/Images/Checked_n.png" : "qrc:/Images/Images/UnChecked_n.png"
                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            checkboxSendAll.checked = !checkboxSendAll.checked
                                            if(checkboxSendAll.checked){ destination.itemAt(0).toAmount = AppModel.walletInfo.walletBalance }
                                            else{ destination.itemAt(0).toAmount = "" }
                                        }
                                    }
                                }
                                QText {
                                    anchors.verticalCenter: checkboxSendAll.verticalCenter
                                    text: STR.STR_QML_262
                                    color: "#031F2B"
                                    font.pixelSize: 14
                                }
                            }
                        }
                        QImage {
                            id: removeRowBtn
                            width: 32
                            height: 32
                            source: "qrc:/Images/Images/Delete.png"
                            transformOrigin: Item.Center
                            scale: removeRow.pressed ? 0.95 : 1
                            visible: checkboxMultiDes.checked && destination.count > 1
                            enabled: visible
                            MouseArea {
                                id: removeRow
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                anchors.fill: parent
                                onClicked: {
                                    requestRemoveDestination(index)
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
                    visible: checkboxMultiDes.checked
                    enabled: checkboxMultiDes.checked
                    onButtonClicked: {
                        requestAddMultiDestinations()
                    }
                }
                QTextInputBox {
                    id: memo
                    width: 720
                    heightMin: 96
                    mode: eEDIT_MODE
                    placeholder.text: STR.STR_QML_264
                    texOutputValignment: memo.heightMin === 56 ? Text.AlignVCenter : Text.AlignTop
                    border.color: "#DEDEDE"
                }
            }
        }
        Row {
            spacing: 16
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 44
            }
            QTextButton {
                width: 120
                height: 48
                label.text: STR.STR_QML_245
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    if(AppModel.walletInfo.isSharedWallet){
                        QMLHandle.sendEvent(EVT.EVT_SEND_BACK_HOME_SHARED_WL)
                    }
                    else{
                        QMLHandle.sendEvent(EVT.EVT_SEND_BACK_REQUEST)
                    }
                }
            }
            QTextButton {
                width: 260
                height: 48
                enabled: destination.fullfill
                label.text: STR.STR_QML_265
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    var saved = [];
                    for(var i = 0; i < destination.model; i++){
                        if(destination.itemAt(i).toAddress !== "" && destination.itemAt(i).toAmount !== ""){
                            var savedObj = { "toAddress": destination.itemAt(i).toAddress,
                                             "toAmount" : destination.itemAt(i).toAmount};
                            saved[i] = savedObj
                        }
                    }

                    var destinationObj = { "destinationList"    : saved,
                                           "destinationMemo"    : memo.textOutput};
                    createTxBusyBox.open()
                    timerCreateTx.destinationData = destinationObj
                    timerCreateTx.restart()
                }
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
    RegExpValidator { id: intvalidator;      regExp: /^[1-9][0-9]*$/ }
    RegExpValidator { id: doubleValidator;   regExp: /^(?:0|[1-9][0-9]*)(\.\d{1,8})?$/ }
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
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    text: STR.STR_QML_236
                }
                QText {
                    width: 252
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
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
    Timer {
        id: timerCreateTx
        property var destinationData
        interval: 500
        onTriggered: {
            QMLHandle.sendEvent(EVT.EVT_SEND_CREATE_TRANSACTION_REQUEST, timerCreateTx.destinationData)
            createTxBusyBox.close()
        }
    }
    property var destinations: []
    function requestAddMultiDestinations(){
        destinations = []
        for(var i = 0; i < destination.count; i++){
            var savedObj = { "toAddress": destination.itemAt(i).toAddress,
                             "toAmount" : destination.itemAt(i).toAmount};
            destinations[i] = savedObj
        }
        if(destination.count < destination.destinationMAX) {destination.model++}
    }
    function requestRemoveDestination(index){
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
    function requestDisableMultiDestinations(){
        var savedObj = { "toAddress": destination.itemAt(0).toAddress,
                         "toAmount" : destination.itemAt(0).toAmount};
        destination.model = 1
        destinations[0] = savedObj
    }
}
