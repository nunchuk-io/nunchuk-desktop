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
    property bool hasWallet: false
    label.text: hasWallet ? STR.STR_QML_1249: STR.STR_QML_1742
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property bool isPaidSubscription: ClientController.user.isSubscribedUser
    content: Item {
        Column {
            id: _list
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: - 16 + 20
            width: 550
            spacing: 16
            QLato {
                width: _list.width
                height: 20
                visible: !hasWallet
                text: STR.STR_QML_1746
                font.pixelSize: 16
                font.weight: Font.Normal
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Flickable {
                height: 500
                width: _list.width + 4
                anchors.horizontalCenter: parent.horizontalCenter
                flickableDirection: Flickable.VerticalFlick
                clip: true
                interactive: true
                contentHeight: contentWallet.childrenRect.height + 10
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: contentWallet
                    anchors{
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                        topMargin: 2
                    }                    
                    spacing: 16
                    
                    QButtonAssistedWallet {
                        title: STR.STR_QML_1553
                        optional: {
                            var remainCount = ServiceSetting.servicesTag.walletConfig.remaining_wallet_count
                            return qsTr("(%1 remaining)").arg(remainCount)
                        }
                        description: STR.STR_QML_1554
                        note: STR.STR_QML_1555
                        desImg: "qrc:/Images/Images/type-assisted-wallet.svg"
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
                    QButtonCustomWallet {
                        onItemclicked: {
                            var _input = {
                                type: "create-new-wallet"
                            }
                            QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
                        }
                    }
                    
                    QButtonGroupWallet {
                        onItemclicked: {
                            var _input = {
                                type: "create-new-group-wallet"
                            }
                            QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
                        }
                    }
                    QButtonHotWallet {
                        onItemclicked: {
                            OnBoarding.screenFlow = "hotWallet"
                        }
                    }
                    QButtonMiniscript {
                        onItemclicked: {
                            var _input = {
                                type: "create-miniscript-wallet"
                            }
                            QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
                        }
                    }
                    QLine {
                        width: _list.width
                        visible: !isPaidSubscription
                    }
                    QButtonRecommended {
                        description: STR.STR_QML_1545
                        desImg: "qrc:/Images/Images/type-assisted-wallet.svg"
                        visible: !isPaidSubscription
                    }
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
            border.width: 2
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
                            qrscaner.importUrlLink()
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
            QMultiContextMenu {
                id: optionMenu
                menuWidth: 350
                property var recoverGroupWalletMenu: [
                    {
                        visible: true,
                        label: STR.STR_QML_1089,
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() { // QR code
                            if (SharedWallet.checkSandboxWalletLimit()) {
                                qrscaner.importSandboxWallet()
                            }
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1087,
                        icon: "",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() { // BSMS/descriptors
                            if (SharedWallet.checkSandboxWalletLimit()) {
                                fileDialog.recoverType = "recover-sandbox-wallet"
                                fileDialog.open()
                            }
                        }
                    },
                ]
                mapMenu: [
                    {
                        visible: true,
                        label: STR.STR_QML_038,
                        icon: "qrc:/Images/Images/QR-dark.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function(){
                            qrscaner.importHotWallet()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_037,
                        icon: "qrc:/Images/Images/recover.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function(){
                            fileDialog.recoverType = "recover-via-bsms-config-file"
                            fileDialog.open()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1664,
                        icon: "qrc:/Images/Images/recover.svg",
                        iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                        color: "#031F2B",
                        enable: true,
                        subMenu: recoverGroupWalletMenu,
                        action: function(){ }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1254,
                        icon: "qrc:/Images/Images/recover.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function(){
                            OnBoarding.screenFlow = "recoverHotWallet"
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_040,
                        icon: "qrc:/Images/Images/recover.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function(){
                            fileDialog.recoverType = "recover-via-coldcard"
                            fileDialog.open()
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_041,
                        icon: "qrc:/Images/Images/importFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function(){
                            fileDialog.recoverType = "import-db"
                            fileDialog.open()
                        }
                    },
                ]
            }
        }
    }
    QQrImportScanner {
        id: qrscaner
        property string scanType: ""
        onTagFound: {
            console.warn("scanType: ",qrscaner.scanType, tag)
            if (qrscaner.scanType == "import-hot-wallet") {
                if(qrscaner.complete){
                    if(OnBoarding.importQrHotWallet(qrscaner.tags)){
                        qrscaner.close()
                        OnBoarding.screenFlow = "updateWalletName"
                    }
                }
            }
            else if (qrscaner.scanType == "import-url-link") {
                if (qrscaner.tags.length > 0) {
                    var _input = {
                        type: "enter-link-qr-url",
                        sandboxUrl: tag
                    }
                    QMLHandle.notifySendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
                    qrscaner.close()
                }
            }
            else if (qrscaner.scanType == "import-sandbox-wallet") {
                if(qrscaner.complete){
                    if(SharedWallet.importQrSandboxWallet(qrscaner.tags)){
                        qrscaner.close()
                        closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                    }
                }
            }
            else{}
        }
        function importHotWallet() {
            qrscaner.scanType = "import-hot-wallet"
            qrscaner.open()
        }
        function importUrlLink() {
            qrscaner.scanType = "import-url-link"
            qrscaner.open()
        }
        function importSandboxWallet() {
            qrscaner.scanType = "import-sandbox-wallet"
            qrscaner.open()
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
