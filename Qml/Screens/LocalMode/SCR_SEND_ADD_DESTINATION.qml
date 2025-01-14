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
import "../../Popups"
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/QRCodes"
import "../../Components/customizes/Wallets"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: sendRoot
    property var    walletInfo: AppModel.walletInfo
    property bool   sendCoinSelected: false
    property var    totalAmount: sendRoot.sendCoinSelected ? walletInfo.utxoList.amountDisplay : walletInfo.walletBalance
    property var    totalAmountSats: sendRoot.sendCoinSelected ? walletInfo.utxoList.amountSats : walletInfo.walletBalanceSats

    Component.onCompleted: requestFilterOutWallet()
    QOnScreenContent {
        id: contenCenter
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_786
        extraHeader: Row {
            spacing: 8
            Rectangle {
                width: visible ? 84 : 0
                height: visible ? 24 : 0
                color: "#031F2B"
                radius: 4
                visible: walletInfo.walletEscrow
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

            QLato {
                text: "(" + walletInfo.walletName + ")"
                font.weight: Font.Bold
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
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

        Column {
            anchors.fill: parent
            anchors {
                topMargin: 100
                leftMargin: 36
                rightMargin: 36
            }
            spacing: 8
            Row {
                id: walletbalanceInfo
                anchors.left: parent.left
                Item {
                    width: 480
                    height: 28
                    Row {
                        anchors.fill: parent
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 6
                        QLato {
                            height: parent.height
                            text: sendRoot.sendCoinSelected ? STR.STR_QML_1488 : STR.STR_QML_259
                            font.weight: Font.Bold
                            font.pixelSize: 16
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                        QLato {
                            height: parent.height
                            text: qsTr("%1%2").arg(sendRoot.totalAmount).arg(RoomWalletData.unitValue)
                            font.weight: Font.Normal
                            font.pixelSize: 16
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                        QLato {
                            height: parent.height
                            text: qsTr("(%1%2)").arg(AppSetting.currencySymbol).arg(sendRoot.sendCoinSelected ? walletInfo.utxoList.amountCurrency : walletInfo.walletBalanceCurrency)
                            color: "#757575"
                            font.pixelSize: 12
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
                Item {
                    width: 250
                    height: 28
                    visible: destination.count == 1
                    QLato {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        text: sendRoot.sendCoinSelected ? STR.STR_QML_1487 : STR.STR_QML_262
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        scale: sendAllMouse.pressed ? 0.95 : 1
                        transformOrigin: Item.Center
                        MouseArea {
                            id: sendAllMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                destination.itemAt(0).toAmount = sendRoot.totalAmount
                            }
                        }
                    }
                }
            }

            Item {
                width: parent.width
                height: 458
                Flickable {
                    anchors.fill: parent
                    flickableDirection: Flickable.VerticalFlick
                    clip: true
                    contentHeight: destColumn.height
                    ScrollBar.vertical: ScrollBar { active: true }
                    interactive: contentHeight > height
                    Column {
                        id: destColumn
                        width: parent.width
                        spacing: 16
                        Repeater {
                            id: destination
                            property int numberDestination: 1
                            readonly property int destinationMAX: 10
                            property bool fullfill: false
                            model: 1
                            QSendDelegate {
                                id: destinationRow
                                width: parent.width
                                height: 192
                                recipientLabel: destination.count > 1 ? STR.STR_QML_1339.arg(index+1): STR.STR_QML_787
                                itemCount: destination.count
                                onToAddressChanged: {
                                    destination.fullfill = destinationRow.toAddress !== "" && destinationRow.toAmount !== ""
                                }
                                onToAmountChanged: {
                                    destination.fullfill = destinationRow.toAddress !== "" && destinationRow.toAmount !== ""
                                }
                                onQrCodeRequest: {
                                    qrscaner.addressRequestIndex = index
                                    qrscaner.open()
                                }
                                onRemoveItemRequest: {
                                    requestRemoveDestination(index)
                                }
                                onFavoriteRequest: {
                                    requestFilterOutAddress()
                                    requestFilterOutWallet()
                                    favoritesPopup.addressRequestIndex = index
                                    favoritesPopup.open()
                                }
                                onSendAllRemainingRequest: {
                                    var remaining = sendRoot.totalAmountSats
                                    if(remaining > 0){
                                        var places = remaining
                                        for(var i = 0; i < destination.model; i++){
                                            if(i !== index){
                                                var itemAt = destination.itemAt(i)
                                                var satSend = destinationRow.convertToSatoshi(itemAt.onCurrency, itemAt.toAmount)
                                                remaining -= satSend
                                            }
                                        }
                                        remaining = destinationRow.convertFromSatoshi(destination.itemAt(index).onCurrency, remaining)
                                    }
                                    destination.itemAt(index).toAmount = remaining
                                }

                                Component.onCompleted: {
                                    if(index < destinations.length){
                                        destinationRow.setFavorite(destinations[index])
                                    }
                                    destination.fullfill = destination.fullfill & destinationRow.toAddress !== "" & destinationRow.toAmount !== ""
                                }
                                function convertToSatoshi(isCurrency, amount) {
                                    if(isCurrency) {
                                        return AppModel.qAmountFromCurrency(amount)
                                    }
                                    else if (AppSetting.unit === NUNCHUCKTYPE.BTC){
                                        return AppModel.qAmountFromBTC(amount)
                                    } else {
                                        return amount
                                    }
                                }
                                function convertFromSatoshi(isCurrency, amount) {
                                    if(isCurrency) {
                                        return AppModel.qCurrencyFromAmount(amount)
                                    }
                                    else if (AppSetting.unit === NUNCHUCKTYPE.BTC){
                                        return AppModel.qBTCFromAmount(amount)
                                    } else {
                                        return amount
                                    }
                                }
                            }
                        }

                        Item {
                            width: childrenRect.width
                            height: 48
                            Row {
                                spacing: 6
                                anchors.verticalCenter: parent.verticalCenter
                                Item {
                                    id: plus
                                    width: 24
                                    height: 24
                                    anchors.verticalCenter: parent.verticalCenter
                                    Rectangle {
                                        width: 14
                                        height: 2
                                        color: "#031F2B"
                                        anchors.centerIn: parent
                                    }
                                    Rectangle {
                                        width: 2
                                        height: 14
                                        color: "#031F2B"
                                        anchors.centerIn: parent
                                    }
                                }
                                QText {
                                    height: parent.height
                                    text: STR.STR_QML_263
                                    color: "#031F2B"
                                    font.family: "Lato"
                                    font.weight: Font.DemiBold
                                    font.pixelSize: 16
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                            scale: addDestinationMouse.pressed ? 0.95 : 1
                            transformOrigin: Item.Center
                            MouseArea {
                                id: addDestinationMouse
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    requestAddMultiDestinations()
                                }
                            }
                        }

                        Item {
                            width: parent.width
                            height: childrenRect.height
                            QTextInputBoxTypeB {
                                id: memo
                                label: STR.STR_QML_264
                                boxWidth: parent.width - 1
                                boxHeight: 100
                                isValid: true
                                maxLength: 280
                                input.verticalAlignment: TextInput.AlignTop
                            }
                            QText {
                                anchors.top: parent.top
                                anchors.right: parent.right
                                text: qsTr("%1 / %2").arg(memo.length).arg(memo.maxLength)
                                color: "#595959"
                                font.family: "Lato"
                                font.pixelSize: 12
                            }
                        }
                    }
                }
            }

            Item {
                width: parent.width
                height: 92
                Rectangle {
                    width: parent.width + 36*2
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: 1
                    color: "#DEDEDE"
                }

                QButtonTextLink {
                    width: 80
                    height: 48
                    label: STR.STR_QML_245
                    displayIcon: false
                    anchors.verticalCenter: parent.verticalCenter
                    onButtonClicked: {
                        if(walletInfo.isSharedWallet){
                            QMLHandle.sendEvent(EVT.EVT_SEND_BACK_HOME_SHARED_WL)
                        }
                        else{
                            QMLHandle.sendEvent(EVT.EVT_SEND_BACK_REQUEST)
                        }
                    }
                }
                QTextButton {
                    width: 100
                    height: 48
                    enabled: destination.fullfill
                    label.text: STR.STR_QML_265
                    label.font.pixelSize: 16
                    type: eTypeE
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    onButtonClicked: {
                        var saved = [];
                        for(var i = 0; i < destination.model; i++){
                            var toType = destination.itemAt(i).inputObject.toType
                            var toAddress = ""
                            var toAmount = ""
                            var onCurrency = false
                            if(toType === "Input" || toType === ""){
                                toAddress = destination.itemAt(i).toAddress
                                toAmount = destination.itemAt(i).toAmount
                                onCurrency = destination.itemAt(i).onCurrency
                            }
                            else {
                                toAddress = destination.itemAt(i).inputObject.toAddress
                                toAmount = destination.itemAt(i).toAmount
                                onCurrency = destination.itemAt(i).onCurrency
                            }
                            if(toAmount !== "" && toAddress !== ""){
                                var savedObj = {
                                    "toAddress": toAddress,
                                    "toAmount" : toAmount,
                                    "onCurrency" : onCurrency
                                };
                                saved[i] = savedObj
                            }
                        }
                        var destinationObj = {
                            "destinationList"    : saved,
                            "destinationMemo"    : memo.textInputted
                        };
                        createTxBusyBox.open()
                        timerCreateTx.destinationData = destinationObj
                        timerCreateTx.restart()
                    }
                }
            }
        }
    }

    QQrImportScanner {
        id: qrscaner
        property int addressRequestIndex: -1
        onTagFound: {
            if(qrscaner.addressRequestIndex === -2){
                favValueEdit.textInputted = tag
            }
            else {
                if(qrscaner.addressRequestIndex >= 0 || qrscaner.addressRequestIndex < destination.model){
                    var inputObject = ({
                                           "toType": "Input",
                                           "toAddress": tag,
                                           "toAddressDisplay": tag,
                                           "toAmount": ""
                                       })
                    destination.itemAt(qrscaner.addressRequestIndex).setFavoriteInput(inputObject)

                }
            }
            qrscaner.addressRequestIndex = -1
            qrscaner.close()
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
    Popup {
        id: favoritesPopup
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        property int addressRequestIndex: -1
        Rectangle {
            id: mask
            width: 600
            height: 500
            color: "#FFFFFF"
            radius: 24
            layer.enabled: true
            anchors.centerIn: parent
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: mask.width
                    height: mask.height
                    radius: 24
                }
            }
            QMontserrat {
                font.weight: Font.Medium
                font.pixelSize: 32
                lineHeightMode: Text.FixedHeight
                lineHeight: 40
                wrapMode: Text.WordWrap
                text: STR.STR_QML_1337
                anchors {
                    left: parent.left
                    leftMargin: 24
                    top: parent.top
                    topMargin: 24
                }
            }
            QCloseButton {
                anchors {
                    right: parent.right
                    rightMargin: 24
                    top: parent.top
                    topMargin: 24
                }
                onClicked: {
                    favoritesPopup.close()
                }
            }
        }
        DropShadow {
            anchors.fill: mask
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
            source: mask
        }
        Column {
            anchors.fill: mask
            anchors {
                topMargin: 100
                leftMargin: 36
                rightMargin: 36
            }
            spacing: 8
            Flickable {
                width: parent.width
                height: parent.height - 100
                flickableDirection: Flickable.VerticalFlick
                clip: true
                contentHeight: favColumn.height
                ScrollBar.vertical: ScrollBar { active: true }
                interactive: contentHeight > height
                Column {
                    id: favColumn
                    width: parent.width
                    spacing: 16
                    Item {
                        width: parent.width
                        height: 24
                        Row {
                            height: parent.height
                            spacing: 8
                            QIcon {
                                iconSize: 24
                                source: "qrc:/Images/Images/fav_bookmark_24px.svg"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QText {
                                text: STR.STR_QML_1328
                                color: "#031F2B"
                                font.family: "Lato"
                                font.weight: Font.DemiBold
                                font.pixelSize: 16
                                verticalAlignment: Text.AlignVCenter
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        QButtonTextLink {
                            height: 24
                            label: savedAddressList.count > 0 ? STR.STR_QML_849 : STR.STR_QML_941
                            displayIcon: false
                            btnText.font.underline: true
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            onButtonClicked: {
                                if(savedAddressList.count > 0){
                                    savedAddress.open()
                                }
                                else{
                                    addNewAddress.open()
                                }
                            }
                        }
                    }
                    QListView {
                        id: savedAddressList
                        width: parent.width
                        height: contentHeight
                        spacing: 16
                        interactive: false
                        visible: savedAddressList.count > 0
                        delegate: Rectangle {
                            id: savedAddressItem
                            width: savedAddressList.width
                            height: 56
                            radius: 8
                            color: "#F5F5F5"
                            property string dataObject: model.modelData
                            property string dataLabel: savedAddressItem.dataObject.split("[split]")[0]
                            property string dataValue: savedAddressItem.dataObject.split("[split]")[1]
                            QText {
                                text: savedAddressItem.dataLabel
                                color: "#031F2B"
                                font.family: "Lato"
                                font.pixelSize: 16
                                verticalAlignment: Text.AlignVCenter
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.fill: parent
                                anchors.margins: 12
                                wrapMode: Text.WrapAnywhere
                            }
                            MouseArea {
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                anchors.fill: parent
                                onClicked: {
                                    if(favoritesPopup.addressRequestIndex !== -1 && favoritesPopup.addressRequestIndex < destination.model){
                                        var inputObject = ({
                                                              "toType": "Address",
                                                              "toAddress": savedAddressItem.dataValue,
                                                              "toAddressDisplay": savedAddressItem.dataLabel,
                                                              "toAmount": destination.itemAt(favoritesPopup.addressRequestIndex).toAmount
                                                          })
                                        destination.itemAt(favoritesPopup.addressRequestIndex).setFavoriteSelected(inputObject)
                                    }
                                    favoritesPopup.addressRequestIndex = -1
                                    favoritesPopup.close()
                                }
                            }
                        }
                    }
                    Item {
                        width: parent.width
                        height: 24
                        visible: walletList.count > 1
                        Row {
                            height: parent.height
                            spacing: 8
                            QIcon {
                                iconSize: 24
                                source: "qrc:/Images/Images/fav_wallet-dark.svg"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QText {
                                text: STR.STR_QML_1329
                                color: "#031F2B"
                                font.family: "Lato"
                                font.weight: Font.DemiBold
                                font.pixelSize: 16
                                verticalAlignment: Text.AlignVCenter
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                    QListView {
                        id: walletList
                        width: parent.width
                        height: contentHeight
                        spacing: 16
                        interactive: false
                        model: filterOutWallet
                        visible: walletList.count > 0
                        delegate: QWalletManagerDelegate {
                            id: walletListdelegate
                            width: walletList.width
                            isCurrentIndex: false
                            isEscrow: modelData.wallet_Escrow
                            isShared: modelData.wallet_isSharedWallet
                            isAssisted: modelData.wallet_isAssistedWallet
                            walletCurrency: modelData.wallet_Balance_Currency
                            walletName :modelData.wallet_name
                            walletBalance: modelData.wallet_Balance
                            walletM: modelData.wallet_M
                            walletN: modelData.wallet_N
                            isDashboard: false
                            isLocked: modelData.wallet_dashboard ? modelData.wallet_dashboard.isLocked : false
                            walletRole: modelData.wallet_role
                            hasOwner: modelData.wallet_hasOwner
                            primaryOwner: modelData.wallet_primaryOwner
                            isHotWallet: modelData.wallet_isHotWallet
                            visible: modelData.wallet_id !== walletInfo.walletId
                            enabled: visible
                            layer.enabled: true
                            layer.effect: OpacityMask {
                                maskSource: Rectangle {
                                    width: walletListdelegate.width
                                    height: walletListdelegate.height
                                    radius: 8
                                }
                            }
                            onButtonClicked: {
                                if(favoritesPopup.addressRequestIndex !== -1 && favoritesPopup.addressRequestIndex < walletList.count){
                                    var inputObject= ({
                                                          "toType": "Wallet",
                                                          "toAddress": modelData.wallet_Address,
                                                          "toAddressDisplay": modelData.wallet_name,
                                                          "toAmount": destination.itemAt(favoritesPopup.addressRequestIndex).toAmount
                                                      })
                                    destination.itemAt(favoritesPopup.addressRequestIndex).setFavoriteSelected(inputObject)
                                }
                                favoritesPopup.addressRequestIndex = -1
                                favoritesPopup.close()
                            }
                        }
                    }
                }
            }
            Item {
                width: parent.width
                height: 92
                QLine {
                    width: parent.width + 36*2
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: 100
                        horizontalCenter: parent.horizontalCenter
                    }
                }
                QButtonTextLink {
                    width: 80
                    height: 48
                    label: STR.STR_QML_245
                    displayIcon: false
                    anchors.verticalCenter: parent.verticalCenter
                    onButtonClicked: {
                        favoritesPopup.close()
                        favoritesPopup.addressRequestIndex = -1
                    }
                }
            }
        }
    }
    property var filterOutAddress: AppSetting.favoriteAddresses
    property var filterOutWallet: []//AppModel.walletList
    Popup {
        id: savedAddress
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}

        QOnScreenContent {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_1332
            extraHeader:Item {}
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 764
                    height: 700
                    radius: 24
                }
            }
            bottomLeft:  QButtonTextLink {
                height: 48
                label: STR.STR_QML_059
                onButtonClicked: {
                    savedAddress.close()
                }
            }
            bottomRight: QTextButton {
                width: 166
                height: 48
                label.text: STR.STR_QML_1333
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    addNewAddress.dataLabel = ""
                    addNewAddress.dataValue = ""
                    addNewAddress.open()
                }
            }
            onCloseClicked: {
                savedAddress.close()
            }

            Rectangle {
                width: 539
                height: 420
                color: "#FFFFFF"
                border.color: "#EAEAEA"
                anchors.top: parent.top
                anchors.topMargin: 112
                anchors.left: parent.left
                anchors.leftMargin: 36
                radius: 12
                QListView {
                    id: savedAddresses
                    anchors.fill: parent
                    model: AppSetting.favoriteAddresses
                    visible: savedAddresses.count > 0
                    delegate: Item {
                        id: savedAddressesItem
                        width: savedAddresses.width
                        height: 48
                        property string dataObject: model.modelData
                        property string dataLabel: savedAddressesItem.dataObject.split("[split]")[0]
                        property string dataValue: savedAddressesItem.dataObject.split("[split]")[1]
                        Row {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 8
                            QIcon {
                                iconSize: 24
                                source: "qrc:/Images/Images/fav_bookmark_24px.svg"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QText {
                                text: savedAddressesItem.dataLabel
                                color: "#031F2B"
                                font.family: "Lato"
                                font.pixelSize: 16
                                verticalAlignment: Text.AlignVCenter
                                anchors.verticalCenter: parent.verticalCenter
                                wrapMode: Text.WrapAnywhere
                                width: parent.width - 64
                            }
                            QIcon {
                                iconSize: 24
                                source: "qrc:/Images/Images/right-arrow-dark.svg"
                                anchors.verticalCenter: parent.verticalCenter
                                scale: mouseedit.pressed ? 0.95 : 1
                                transformOrigin: Item.Center
                                MouseArea {
                                    id: mouseedit
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    anchors.fill: parent
                                    onClicked: {
                                        addNewAddress.dataLabel = savedAddressesItem.dataLabel
                                        addNewAddress.dataValue = savedAddressesItem.dataValue
                                        addNewAddress.open()
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 102
                width: 728
                height: 60
                radius: 8
                color: "#FDEBD2"
                Row {
                    anchors.centerIn: parent
                    spacing: 8
                    QIcon {
                        iconSize: 36
                        source: "qrc:/Images/Images/warning-dark.svg"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QText {
                        width: 660
                        anchors.verticalCenter: parent.verticalCenter
                        font.weight: Font.DemiBold
                        font.family: "Lato"
                        font.pixelSize: 12
                        wrapMode: Text.WordWrap
                        color: "#031F2B"
                        text: STR.STR_QML_1338
                    }
                }
            }
        }
    }
    Popup {
        id: addNewAddress
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        property string dataLabel: ""
        property string dataValue: ""
        onOpened: {
            favLabelEdit.textInputted = addNewAddress.dataLabel
            favValueEdit.textInputted = addNewAddress.dataValue
        }
        onClosed: {
            addNewAddress.dataLabel = ""
            addNewAddress.dataValue = ""
        }

        QOnScreenContent {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_1333
            extraHeader:Item {}
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 764
                    height: 700
                    radius: 24
                }
            }
            bottomLeft:  QButtonTextLink {
                height: 48
                label: STR.STR_QML_059
                onButtonClicked: {
                    addNewAddress.close()
                    requestFilterOutAddress()
                }
            }
            bottomRight: Row {
                spacing: 12
                QTextButton {
                    width: 250
                    height: 48
                    label.text: STR.STR_QML_1335
                    label.font.pixelSize: 16
                    type: eTypeB
                    visible: addNewAddress.dataValue !== ""
                    enabled: visible
                    onButtonClicked: {
                        confirmRemove.open()
                    }
                }
                QTextButton {
                    width: 66
                    height: 48
                    label.text: STR.STR_QML_835
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled: favLabelEdit.textInputted !== "" && favValueEdit.textInputted !== "" && favValueEdit.isValid
                    onButtonClicked: {
                        if(addNewAddress.dataLabel !== "" && addNewAddress.dataValue !== ""){
                            AppSetting.updateFavoriteAddress(favLabelEdit.textInputted, favValueEdit.textInputted)
                        }
                        else{
                            var valid = AppSetting.validateAddress(favValueEdit.textInputted)
                            if(!valid){
                                favValueEdit.isValid = false
                                favValueEdit.errorText = "Invalid address"
                            }
                            else{
                                favValueEdit.isValid = true
                                favValueEdit.errorText = ""
                                AppSetting.addFavoriteAddress(favLabelEdit.textInputted, favValueEdit.textInputted)
                                requestFilterOutAddress()
                                addNewAddress.close()
                            }
                        }
                    }
                }
            }
            onCloseClicked: {
                addNewAddress.close()
                requestFilterOutAddress()
            }
            Column {
                width: 540
                anchors.top: parent.top
                anchors.topMargin: 92
                anchors.left: parent.left
                anchors.leftMargin: 36
                spacing: 24
                QText {
                    text: STR.STR_QML_1334
                    color: "#031F2B"
                    font.family: "Lato"
                    font.pixelSize: 16
                }
                Item {
                    width: parent.width
                    height: childrenRect.height
                    QTextInputBoxTypeB {
                        id: favLabelEdit
                        label: "Label"
                        boxWidth: parent.width - 72
                        boxHeight: 48
                        isValid: true
                        maxLength: 40
                        input.placeholderText: "Enter a label"
                    }
                    Row {
                        anchors.top: favLabelEdit.bottom
                        anchors.topMargin: 4
                        anchors.left: parent.left
                        spacing: 4
                        visible: favLabelEdit.length > 0
                        QIcon {
                            iconSize: 16
                            source: "qrc:/Images/Images/info-60px.svg"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QText {
                            text: qsTr("%1 / %2").arg(favLabelEdit.length).arg(favLabelEdit.maxLength)
                            color: "#595959"
                            font.family: "Lato"
                            font.pixelSize: 12
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
                Item {
                    width: parent.width
                    height: childrenRect.height
                    QTextInputBoxTypeB {
                        id: favValueEdit
                        label: "Address"
                        boxWidth: parent.width - 72
                        boxHeight: 48
                        isValid: (textInputted === "") ? true : AppSetting.validateAddress(textInputted)
                        showError: true
                        errorText: "Invalid address"
                        enabled: addNewAddress.dataValue === ""
                        input.text: addNewAddress.dataValue
                        input.placeholderText: "Enter an address"
                        input.rightPadding: 48
                    }
                    QIcon {
                        iconSize: 36
                        id: qricoimport
                        source: "qrc:/Images/Images/QrSendButton.svg"
                        anchors.bottom: favValueEdit.bottom
                        anchors.bottomMargin: favValueEdit.isValid ? 6 : 38
                        anchors.right: favValueEdit.right
                        anchors.rightMargin: 6
                        transformOrigin: Item.Center
                        scale: qrMouse.pressed ? 0.95 : 1
                        visible: addNewAddress.dataValue === ""
                        MouseArea {
                            id: qrMouse
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                qrscaner.addressRequestIndex = -2
                                qrscaner.open()
                            }
                        }
                    }
                }
            }
            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 102
                width: 728
                height: 60
                radius: 8
                color: "#FDEBD2"
                Row {
                    anchors.centerIn: parent
                    spacing: 8
                    QIcon {
                        iconSize: 36
                        source: "qrc:/Images/Images/warning-dark.svg"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QText {
                        width: 660
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WordWrap
                        color: "#031F2B"
                        text: STR.STR_QML_1338
                        font.weight: Font.DemiBold
                        font.family: "Lato"
                        font.pixelSize: 12
                    }
                }
            }
        }
    }
    QConfirmYesNoPopup{
        id: confirmRemove
        property var data
        contentText:STR.STR_QML_1336.arg(addNewAddress.dataLabel)
        contentWrapMode: Text.WrapAnywhere
        onConfirmNo: close()
        onConfirmYes: {
            AppSetting.removeFavoriteAddress(addNewAddress.dataLabel, addNewAddress.dataValue)
            requestFilterOutAddress()
            addNewAddress.close()
            savedAddress.close()
            close()
        }
    }
    property var destinations: []
    function requestAddMultiDestinations(){
        destinations = []
        for(var i = 0; i < destination.count; i++){
            if(destination.itemAt(i).inputObject.toType === "Input" || destination.itemAt(i).inputObject.toType === ""){
                destination.itemAt(i).inputObject.toType = "Input"
                destination.itemAt(i).inputObject.toAddress = destination.itemAt(i).toAddress
                destination.itemAt(i).inputObject.toAddressDisplay = destination.itemAt(i).toAddress
            }
            destination.itemAt(i).inputObject.toAmount = destination.itemAt(i).toAmount
            destinations[i] = destination.itemAt(i).inputObject
        }
        if(destination.count < destination.destinationMAX) {
            destination.model++
        }
    }
    function requestRemoveDestination(index){
        var idx = 0
        destinations = []
        for(var i = 0; i < destination.count; i++){
            if(i === index){ continue }
            if(destination.itemAt(i).inputObject.toType === "Input" || destination.itemAt(i).inputObject.toType === ""){
                destination.itemAt(i).inputObject.toType = "Input"
                destination.itemAt(i).inputObject.toAddress = destination.itemAt(i).toAddress
                destination.itemAt(i).inputObject.toAddressDisplay = destination.itemAt(i).toAddress
            }
            destination.itemAt(i).inputObject.toAmount = destination.itemAt(i).toAmount
            destinations[idx] = destination.itemAt(i).inputObject
            idx++
        }
        if(destination.count > 1){
            destination.model--
        }
    }
    function requestRollOverProcess(address) {
        var inputObject = ({
                               "toType": "Input",
                               "toAddress": address,
                               "toAddressDisplay": address,
                               "toAmount": walletInfo.walletBalance
                          })
        destination.itemAt(0).setFavoriteInput(inputObject)
        sendRoot.sendCoinSelected = false
    }
    function requestSelectedCoin(address) {
        var inputObject = ({
                               "toType": "Input",
                               "toAddress": address,
                               "toAddressDisplay": address,
                               "toAmount": walletInfo.utxoList.amountDisplay
                          })
        destination.itemAt(0).setFavoriteInput(inputObject)
        sendRoot.sendCoinSelected = true
    }
    function containAddress(address) {
        for(var i = 0; i < destination.count; i++){
            var current = destination.itemAt(i).inputObject.toAddress
            if (current === address) {
                return true
            }
        }
        return false
    }

    function requestFilterOutAddress() {
        AppSetting.loadFavoriteAddresses()
        filterOutAddress = []
        for(var i = 0; i < AppSetting.favoriteAddresses.length; i++) {
            var _address = AppSetting.favoriteAddresses[i]
            var dataLabel = _address.split("[split]")[0]
            var dataValue = _address.split("[split]")[1]
            if (!containAddress(dataValue)) {
                filterOutAddress.push(_address)
            }
        }
        savedAddressList.model = filterOutAddress
    }

    function requestFilterOutWallet() {
        filterOutWallet = []
        for(var i = 0; i < AppModel.walletList.count; i++) {
            var wallet = AppModel.walletList.get(i)
            if (wallet.wallet_id !== walletInfo.walletId) {
                var wallet_Address = wallet.wallet_Address
                if (!containAddress(wallet_Address)) {
                    filterOutWallet.push(wallet)
                }
            }
        }
        walletList.model = filterOutWallet
    }

    Connections {
        target: walletInfo
        onRollOverProcess: {
            requestRollOverProcess(address)
        }
        onRequestCreateTransaction: {
            requestSelectedCoin(address)
        }
    }
}
