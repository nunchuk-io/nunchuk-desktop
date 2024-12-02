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
import "../origins"
import "../customizes"
import "../customizes/Texts"
import "../customizes/Buttons"
import "../customizes/QRCodes"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    id: sendDelegateRoot
    width: parent.width
    height: 192
    color: "#F5F5F5"
    radius: 12

    property var inputObject: ({
                                   "toType": "Input", // "Input, Address, Wallet"
                                   "toAddress": "",
                                   "toAddressDisplay": "",
                                   "toAmount": ""
                               })

    property int   itemCount: 0
    property alias recipientLabel: addressInput.label
    property alias toAmount: amountInput.textInputted
    property alias toAddress: addressInput.textInputted
    property alias enableInput: addressInput.enabled
    property bool  enableInputAmount: true
    property bool  onCurrency: false

    signal qrCodeRequest()
    signal removeItemRequest()
    signal favoriteRequest()
    signal sendAllRemainingRequest()

    function setFavorite(fav) {
        if ((fav.toType === "Address") || (fav.toType === "Wallet")){
            setFavoriteSelected(fav)
        }
        else{
            setFavoriteInput(fav)
        }
    }

    function setFavoriteSelected(cached) {
        inputObject = cached
        toAddress = inputObject.toAddressDisplay
        toAmount  = inputObject.toAmount
        addressInput.input.leftPadding = 44
        closeLoader.sourceComponent = clearfav
        if ((inputObject.toType === "Address")){
            iconLoader.sourceComponent  = iconFavAddr
        }
        else {
            iconLoader.sourceComponent  = iconFavWallet
        }
        enableInput = false
    }

    function setFavoriteInput(cached) {
        inputObject = cached
        inputObject.toType = "Input"
        toAddress = inputObject.toAddressDisplay
        toAmount  = inputObject.toAmount
        addressInput.input.leftPadding = 12
        closeLoader.sourceComponent = dropdownfav
        iconLoader.sourceComponent  = null
        enableInput = true
    }

    Column {
        anchors.fill: parent
        anchors.margins: 18
        spacing: 12

        Row {
            spacing: 24
            Item {
                width: childrenRect.width
                height: childrenRect.height
                QTextInputBoxTypeB {
                    id: addressInput
                    label: STR.STR_QML_787
                    boxWidth: 620
                    boxHeight: 48
                    isValid: true
                    maxLength: 280
                    enabled: (inputObject.toType === "Input")
                    disabledColor: "#FFFFFF"
                    input.rightPadding: 36
                }
                Loader {
                    id: closeLoader
                    anchors.bottom: addressInput.bottom
                    anchors.bottomMargin: 12
                    anchors.right: addressInput.right
                    anchors.rightMargin: 12
                    sourceComponent: dropdownfav
                }
                Loader {
                    id: iconLoader
                    anchors.bottom: addressInput.bottom
                    anchors.bottomMargin: 12
                    anchors.left: addressInput.left
                    anchors.leftMargin: 12
                    sourceComponent: null
                }
                QButtonTextLink {
                    height: 24
                    label: STR.STR_QML_1134
                    displayIcon: false
                    btnText.font.underline: true
                    anchors.top: addressInput.top
                    anchors.right: addressInput.right
                    visible: itemCount > 1
                    onButtonClicked: {
                        removeItemRequest()
                    }
                }
            }
            QImage {
                id: qricoimport
                source: "qrc:/Images/Images/QrSendButton.png"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 6
                transformOrigin: Item.Center
                scale: qrMouse.pressed ? 0.95 : 1
                width: 36
                height: 36
                MouseArea {
                    id: qrMouse
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    anchors.fill: parent
                    onClicked: {
                        qrCodeRequest()
                    }
                }
            }
        }
        Item {
            width: parent.width
            height: childrenRect.height
            enabled: enableInputAmount
            QTextInputBoxTypeB {
                id: amountInput
                label: STR.STR_QML_214
                boxWidth: 620
                boxHeight: 48
                isValid: true
                validator: (AppSetting.unit === NUNCHUCKTYPE.SATOSHI) ? intvalidator : doubleValidator
            }
            Row {
                anchors.bottom: amountInput.bottom
                anchors.bottomMargin: 12
                anchors.right: amountInput.right
                anchors.rightMargin: 12
                spacing: 4
                QText {
                    height: 24
                    anchors.verticalCenter: parent.verticalCenter
                    text: {
                        if(sendDelegateRoot.onCurrency) {
                            return qsTr("%1").arg(AppSetting.currency)
                        }
                        else{
                            return qsTr("%1").arg((AppSetting.unit === NUNCHUCKTYPE.SATOSHI) ? "sat" : "BTC")
                        }
                    }
                    color: "#031F2B"
                    font.family: "Lato"
                    font.weight: Font.DemiBold
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                }
                QImage {
                    height: 24
                    width: 24
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/fav_swapt-24px.png"
                    transformOrigin: Item.Center
                    scale: moneyMouse.pressed ? 0.95 : 1
                    MouseArea {
                        id: moneyMouse
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        anchors.fill: parent
                        onClicked: {
                            sendDelegateRoot.onCurrency = !sendDelegateRoot.onCurrency
                        }
                    }
                }
            }
            QButtonTextLink {
                height: 24
                label: "Send all remaining"
                displayIcon: false
                btnText.font.underline: true
                anchors.top: amountInput.top
                anchors.right: amountInput.right
                onButtonClicked: {
                    sendAllRemainingRequest()
                }
            }
        }
    }
    RegExpValidator { id: intvalidator;      regExp: /^[1-9][0-9]*$/ }
    RegExpValidator { id: doubleValidator;   regExp: /^(?:0|[1-9][0-9]*)(\.\d{1,8})?$/ }
    Component {
        id: dropdownfav
        QImage {
            source: "qrc:/Images/Images/arrow_drop_down_24px.png"
            transformOrigin: Item.Center
            scale: favMouse.pressed ? 0.95 : 1
            width: 24
            height: 24
            MouseArea {
                id: favMouse
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                anchors.fill: parent
                onClicked: {
                    favoriteRequest()
                }
            }
        }
    }
    Component {
        id: clearfav
        QImage {
            source: "qrc:/Images/Images/fav_close-circle-dark.png"
            transformOrigin: Item.Center
            scale: favMouse.pressed ? 0.95 : 1
            width: 24
            height: 24
            MouseArea {
                id: favMouse
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                anchors.fill: parent
                onClicked: {
                    var inputObject = ({
                                           "toType": "Input",
                                           "toAddress": "",
                                           "toAddressDisplay": "",
                                           "toAmount": toAmount
                                       })
                    setFavoriteInput(inputObject)
                }
            }
        }
    }
    Component {
        id: iconFavAddr
        QImage {
            source: "qrc:/Images/Images/fav_bookmark_24px.png"
            width: 24
            height: 24
        }
    }
    Component {
        id: iconFavWallet
        QImage {
            source: "qrc:/Images/Images/fav_wallet-dark.png"
            width: 24
            height: 24
        }
    }
}
