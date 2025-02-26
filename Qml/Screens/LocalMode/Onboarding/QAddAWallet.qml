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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1249
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property bool isPaidSubscription: ClientController.user.isSubscribedUser
    content: Item {
        Column {
            id: _list
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: - 16 + 20
            width: 550
            spacing: 16
            QCreateHotWallet {
                width: _list.width
                height: 136
                anchors.left: parent.left
                isPaid: true
                color: "#F5F5F5"
                title: STR.STR_QML_1553
                optional: "(1 remaining)"
                description: STR.STR_QML_1554
                note: STR.STR_QML_1555
                desImg: "qrc:/Images/Images/type-assisted-wallet.svg"
                onItemclicked: {
                }
                QSubscriptionRequired {
                    width: 150
                    height: 20
                    anchors {
                        top: parent.top
                        right: parent.right
                    }
                }
                enabled: false
                visible: isPaidSubscription
            }
            QCreateHotWallet {
                width: _list.width
                height: 108
                anchors.left: parent.left
                color: "#FFFFFF"
                title: STR.STR_QML_1262
                description: STR.STR_QML_1405
                desImg: "qrc:/Images/Images/type-unassisted-wallet.svg"
                onItemclicked: {
                    var _input = {
                        type: "create-new-wallet"
                    }
                    QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
                }
            }
            QCreateHotWallet {
                width: _list.width
                height: 108
                anchors.left: parent.left
                color: "#FFFFFF"
                title: STR.STR_QML_1543
                description: STR.STR_QML_1544
                desImg: "qrc:/Images/Images/type-free-group-wallet.svg"
                onItemclicked: {
                    var _input = {
                        type: "create-new-group-wallet"
                    }
                    QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
                }
            }
            QCreateHotWallet {
                width: _list.width
                height: 108
                anchors.left: parent.left
                color: "#FFFFFF"
                title: STR.STR_QML_1251
                description: STR.STR_QML_1252
                desImg: "qrc:/Images/Images/type-hot-wallet.svg"
                onItemclicked: {
                    OnBoarding.state = "hotWallet"
                }
            }
            QLine {
                width: _list.width
                visible: !isPaidSubscription
            }
            QCreateHotWallet {
                width: _list.width
                height: 80
                anchors.left: parent.left
                color: "#F5F5F5"
                title: ""
                description: STR.STR_QML_1545
                desImg: "qrc:/Images/Images/type-assisted-wallet.svg"
                visible: !isPaidSubscription
                onItemclicked: {

                }
            }
        }
    }

    bottomLeft: Item {}
    bottomRight: Row {
        spacing: 12
        Rectangle {
            width: 215
            height: 48
            border.width: 1
            border.color: "#031F2B"
            radius: 44
            Row {
                height: 48
                anchors.left: parent.left
                anchors.leftMargin: 16
                spacing: 16
                QLato {
                    id: idText
                    anchors.verticalCenter: parent.verticalCenter
                    text: STR.STR_QML_1546
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    MouseArea {
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        anchors.fill: parent
                        onClicked: {
                            enterLink.clearText()
                            enterLink.open()
                        }
                    }
                }
                QLine {
                    width: 1
                    height: 21
                    color: "#031F2B"
                    anchors.verticalCenter: parent.verticalCenter
                }
                QIcon {
                    id: idIcon
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/QR-dark.svg"
                    iconSize: 24
                    MouseArea {
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        anchors.fill: parent
                        onClicked: {
                            qrscanerLink.open()
                        }
                    }
                }
            }
        }
        QTextButton {
            width: 198
            height: 48
            label.text: STR.STR_QML_1253
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                optionMenu.x = 20
                optionMenu.y = 20 - optionMenu.height
                optionMenu.open()
            }
            QContextMenu {
                id: optionMenu
                menuWidth: 350
                labels: [
                    STR.STR_QML_038,
                    STR.STR_QML_1664,
                    STR.STR_QML_037,
                    STR.STR_QML_040,
                    STR.STR_QML_1254
                ]
                icons: [
                    "qrc:/Images/Images/QRCodeScan.png",
                    "qrc:/Images/Images/import.png",
                    "qrc:/Images/Images/import.png",
                    "qrc:/Images/Images/import.png",
                    "qrc:/Images/Images/import.png",
                ]
                functions: [
                    function(){
                        qrscaner.open()
                    },
                    function(){
                        fileDialog.recoverType = "recover-sandbox-wallet"
                        fileDialog.open()
                    },
                    function(){
                        fileDialog.recoverType = "recover-via-bsms-config-file"
                        fileDialog.open()
                    },
                    function(){
                        fileDialog.recoverType = "recover-via-coldcard"
                        fileDialog.open()
                    },
                    function(){
                        OnBoarding.state = "recoverHotWallet"
                    },
                ]
                onItemClicked: {
                    functions[index]()
                }
            }
        }
    }
    QQrImportScanner {
        id: qrscaner
        onTagFound: {
            if(OnBoarding.importQrHotWallet(qrscaner.tags)){
                qrscaner.close()
                closeTo(NUNCHUCKTYPE.WALLET_TAB)
            }
        }
    }
    FileDialog {
        id: fileDialog
        property string recoverType: ""
        fileMode: FileDialog.OpenFile
        onAccepted: {
            var _input = {
                type: "recover-existing-wallet",
                filePath: fileDialog.file,
                recoverType: fileDialog.recoverType
            }
            QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
        }
    }
    QPopupInfo {
        id:_signed_in_user_info
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1633
    }
    readonly property int eTypeF: 5
    readonly property int eTypeE: 4
    QPopupInfoTwoButtons {
        id: _guest_user
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1634
        labels: [STR.STR_QML_341,STR.STR_QML_1635]
        types: [eTypeF, eTypeE]
        isVertical: false
        funcs: [
            function() {},
            function() {
                var _input = {
                    type: "sign-up",
                }
                QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
            }
        ]
    }
    QPopupEnterTextInput {
        id: enterLink
        title: STR.STR_QML_1637
        btnLabels: [STR.STR_QML_035, STR.STR_QML_097]
        onEnterText: {
            var _input = {
                type: "enter-link-url",
                sandboxUrl: str
            }
            QMLHandle.notifySendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
        }
    }
    QQrImportScanner {
        id: qrscanerLink
        onTagFound: {
            console.log(tag)
            if (qrscanerLink.tags.length > 0) {
                var _input = {
                    type: "enter-link-qr-url",
                    sandboxUrl: tag
                }
                QMLHandle.notifySendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
                qrscanerLink.close()
            }
        }
    }
    Connections {
        target: SharedWallet
        onGroupWalletLimitChanged: {
            if (isGuest) {
                _guest_user.open()
            } else {
                _signed_in_user_info.open()
            }
        }
    }
}
