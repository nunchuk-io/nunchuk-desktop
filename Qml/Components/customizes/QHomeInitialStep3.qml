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
import "../../Components/origins"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Transactions"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

Item {
    id: _item
    property bool existWallet: false
    property string myRole: AppModel.walletInfo.myRole
    property bool walletNeedBackup: AppModel.walletInfo.needBackup
    property bool walletIsReplaced: AppModel.walletInfo.isReplaced
    property bool walletIsAssisted: AppModel.walletInfo.isAssistedWallet
    property bool walletIsShared: AppModel.walletInfo.isSharedWallet
    property bool walletIsLocked: AppModel.walletInfo.isLocked
    property bool walletIsSandboxWallet: AppModel.walletInfo.isGlobalGroupWallet


    anchors.fill: parent
    anchors.margins: 24
    Column {
        spacing: 24
        Rectangle {
            visible: walletNeedBackup
            width: _item.width
            height: 80
            radius: 8
            color: "#FDEBD2"
            Row {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8
                QIcon {
                    iconSize: 36
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/warning-dark.svg"
                }
                QLato {
                    width: 642
                    height: parent.height
                    text: STR.STR_QML_1265
                }
            }
            QTextLink {
                width: 101
                height: 48
                anchors {
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                text: STR.STR_QML_1267
                font.underline: false
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                onTextClicked: {
                    _info2.contentText = STR.STR_QML_1268
                    _info2.labels = [STR.STR_QML_097,STR.STR_QML_1276]
                    _info2.funcs = [
                                function() {
                                    OnBoarding.state = "seedPhrase"
                                    QMLHandle.sendEvent(EVT.EVT_ONBOARDING_REQUEST)
                                },
                                function() {
                                    _info2.close()
                                }
                            ]
                    _info2.open();
                }
            }
        }
        QAreaWalletDetail{
            id: _walletDes
            width: _item.width
            height: (_item.height * 0.49)
            isAssisted: walletIsAssisted
            isHotWallet: walletNeedBackup
            isLocked: walletIsLocked
            isReplaced: walletIsReplaced
            isSandboxWallet: walletIsSandboxWallet
            Row{
                anchors.fill: parent
                Item {
                    height: parent.height
                    width: parent.width * (myRole === "FACILITATOR_ADMIN" ? 1 : _walletDes.ratio)
                    QText {
                        id: displayWalletname
                        width: 380
                        height: 36
                        text: AppModel.walletInfo.walletName
                        font.weight: Font.Bold
                        font.family: "Lato"
                        font.pixelSize: 28
                        color: "#F1FAFE"
                        elide: Text.ElideRight
                        anchors {
                            left: parent.left
                            leftMargin: 24
                            top: parent.top
                            topMargin: 24
                        }
                    }
                    Row {
                        spacing: 8
                        anchors{
                            right: parent.right
                            rightMargin: 36
                            top: parent.top
                            topMargin: 30*QAPP_DEVICE_HEIGHT_RATIO
                        }
                        QIconButton{
                            id: _group
                            width: 24
                            height: 24
                            visible: (myRole !== "OBSERVER") && (myRole !== "")
                            icon: (AppModel.walletInfo.isHoneyBadger || AppModel.walletInfo.isIronHand) ? "qrc:/Images/Images/health-check-light.svg" : "qrc:/Images/Images/groups-dark.svg"
                            onClicked: {
                                GroupWallet.dashboardInfo.isShowDashBoard = true
                            }
                        }
                        QIconButton{
                            id:_more
                            width: 24
                            height: 24
                            visible: (myRole !== "OBSERVER") && (myRole !== "FACILITATOR_ADMIN")
                            icon: "qrc:/Images/Images/more_horiz_24px.png"
                            onClicked: {
                                optionMenu.popup(_more,mouse.x - 250 + 24,24)
                            }
                        }
                    }

                    QText {
                        id: displayDescription
                        text: AppModel.walletInfo.walletDescription
                        width: displayWalletname.width
                        elide: Text.ElideRight
                        height: displayDescription.text !== "" ? 18 : 0
                        font.pixelSize: 12
                        font.family: "Lato"
                        color: "#F1FAFE"
                        anchors {
                            left: displayWalletname.left
                            top: displayWalletname.bottom
                        }
                    }
                    Row{
                        anchors {
                            left: parent.left
                            leftMargin: 24
                            top: displayDescription.bottom
                            topMargin: 16*QAPP_DEVICE_HEIGHT_RATIO
                        }
                        width: parent.width
                        height: 16
                        spacing: 8
                        Rectangle {
                            width: _txt.paintedWidth + 10*2
                            height: parent.height
                            radius: 20
                            color: "#EAEAEA"
                            QText {
                                id:_txt
                                anchors.centerIn: parent
                                text: (myRole === "FACILITATOR_ADMIN") ? "••••••" : (AppModel.walletInfo.walletN === 1) ? STR.STR_QML_070 : qsTr("%1/%2 %3").arg(AppModel.walletInfo.walletM).arg(AppModel.walletInfo.walletN).arg(STR.STR_QML_069);
                                color: "#031F2B"
                                font.weight: Font.Bold
                                font.pixelSize: 10
                            }
                        }
                        Rectangle{
                            width: 70
                            height: parent.height
                            radius: 20
                            visible: walletIsShared
                            color: "#EAEAEA"
                            Row {
                                anchors.centerIn: parent
                                spacing: 4
                                QIcon {
                                    iconSize: 12
                                    source: "qrc:/Images/Images/collab-wallet-dark.svg"
                                }
                                QText{
                                    font.family: "Lato"
                                    font.pixelSize: 10
                                    color: "#031F2B"
                                    text: STR.STR_QML_438
                                    font.weight: Font.Bold
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                        Rectangle{
                            width: 70
                            height: parent.height
                            radius: 20
                            visible: walletIsSandboxWallet
                            color: "#EAEAEA"
                            Row {
                                anchors.centerIn: parent
                                spacing: 4
                                QIcon {
                                    iconSize: 12
                                    source: "qrc:/Images/Images/sandboxGroup.svg"
                                }
                                QLato{
                                    font.pixelSize: 10
                                    text: STR.STR_QML_1675
                                    font.weight: Font.Bold
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                        Rectangle{
                            width: 70
                            height: parent.height
                            radius: 20
                            visible: _walletDes.isAssisted && !_walletDes.isReplaced
                            color: "#EAEAEA"
                            Row {
                                anchors.centerIn: parent
                                spacing: 4
                                QIcon {
                                    iconSize: 12
                                    source: "qrc:/Images/Images/collab-wallet-dark.svg"
                                }
                                QText{
                                    font.family: "Lato"
                                    font.pixelSize: 10
                                    color: "#031F2B"
                                    text: (myRole === "FACILITATOR_ADMIN") ? "••••••" : STR.STR_QML_679
                                    font.weight: Font.Bold
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                        Rectangle {
                            width: 70
                            height: parent.height
                            visible: _walletDes.isReplaced
                            radius: 20
                            color: "#EAEAEA"
                            Row {
                                anchors.centerIn: parent
                                spacing: 4
                                QText{
                                    font.family: "Lato"
                                    font.pixelSize: 10
                                    color: "#031F2B"
                                    text: STR.STR_QML_1345
                                    font.weight: Font.Bold
                                }
                            }
                        }
                    }
                    Column {
                        id: balanceInfoCol
                        width: parent.width*0.9
                        anchors {
                            top: parent.top
                            topMargin: 143*QAPP_DEVICE_HEIGHT_RATIO
                            left: parent.left
                            leftMargin: 24
                        }
                        spacing: 4
                        QText {
                            id: wlBalance
                            width: parent.width
                            color: "#F1FAFE"
                            font.pixelSize: 28
                            font.family: "Lato"
                            font.weight: Font.Bold
                            text: (myRole === "FACILITATOR_ADMIN") ? "••••••" : qsTr("%1 %2").arg(AppModel.walletInfo.walletBalance).arg(RoomWalletData.unitValue)
                        }
                        QText {
                            id: wlBalanceCurrency
                            width: parent.width
                            color: "#F1FAFE"
                            font.pixelSize: 16
                            font.family: "Lato"
                            font.weight: Font.Medium
                            text: (myRole === "FACILITATOR_ADMIN") ? "••••••" : qsTr("%1%2 %3").arg(AppSetting.currencySymbol).arg(AppModel.walletInfo.walletBalanceCurrency).arg(AppSetting.currency)
                        }
                    }
                    QButtonTextLink {
                        height: 24
                        label: STR.STR_QML_574
                        icon: [
                            "qrc:/Images/Images/right-arrow-light.svg",
                            "qrc:/Images/Images/right-arrow-light.svg",
                            "qrc:/Images/Images/right-arrow-light.svg"
                        ]
                        textColor: [
                            "#FFFFFF",
                            "#FFFFFA",
                            "#FFFFFF"
                        ]
                        direction: eRIGHT
                        _mirror:false
                        enabled: existWallet
                        anchors {
                            left: parent.left
                            leftMargin: 24
                            top: parent.top
                            topMargin: 239*QAPP_DEVICE_HEIGHT_RATIO
                        }
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_INFO_REQUEST)
                        }
                    }
                    Row {
                        id: buttongHandles
                        spacing: 12
                        anchors {
                            left: parent.left
                            leftMargin: 24
                            bottom: parent.bottom
                            bottomMargin: 24*QAPP_DEVICE_HEIGHT_RATIO
                        }
                        QIconTextButton {
                            width: 132
                            height: 48
                            label: STR.STR_QML_002
                            icons: ["spend-dark.svg", "spend-light.svg", "spend-dark.svg","spend-light.svg"]
                            fontPixelSize: 16
                            iconSize: 24
                            type: eTypeF
                            enabled: (existWallet && (myRole !== "OBSERVER")) && !walletIsLocked
                            onButtonClicked: {
                                if(walletIsShared){
                                    QMLHandle.sendEvent(EVT.EVT_GOTO_HOME_CHAT_TAB)
                                    QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WL_SEND_REQUEST,AppModel.walletInfo.walletId)
                                }
                                else{
                                    QMLHandle.sendEvent(EVT.EVT_HOME_SEND_REQUEST)
                                }
                            }
                        }

                        QIconTextButton {
                            width: 132
                            height: 48
                            label: STR.STR_QML_003
                            icons: ["received-dark.svg", "received-light.svg", "received-dark.svg", "received-light.svg"]
                            fontPixelSize: 16
                            iconSize: 24
                            type: eTypeF
                            onButtonClicked: { QMLHandle.sendEvent(EVT.EVT_HOME_RECEIVE_REQUEST) }
                        }

                        QIconTextButton {
                            width: 132
                            height: 48
                            label: STR.STR_QML_1407
                            icons: ["bitcoin-dark.svg", "bitcoin-light.svg", "bitcoin-disabled.svg", "bitcoin-light.svg"]
                            fontPixelSize: 16
                            iconSize: 24
                            type: eTypeF
                            enabled: AppModel.walletInfo.utxoList.count > 0
                            onButtonClicked: {
                                AppModel.walletInfo.isViewCoinShow = true
                            }
                        }
                    }

                    /* for menu */
                    QContextMenu {
                        id: optionMenu
                        menuWidth: 275
                        labels: [
                            STR.STR_QML_532,
                            STR.STR_QML_347,
                            STR.STR_QML_1490,
                            STR.STR_QML_970  //Manage group chat history
                        ]
                        icons: [
                            "qrc:/Images/Images/import_031F2B.png",
                            "qrc:/Images/Images/Backup.png",
                            "qrc:/Images/Images/export_invoices.png",
                            "qrc:/Images/Images/clock-dark.svg",
                        ]
                        enables: [
                            true,
                            true,
                            (myRole !== "FACILITATOR_ADMIN" && myRole !== "KEYHOLDER_LIMITED"),
                            walletIsSandboxWallet,
                        ]
                        visibles: [
                            true,
                            true,
                            (myRole !== "FACILITATOR_ADMIN" && myRole !== "KEYHOLDER_LIMITED"),
                            walletIsSandboxWallet,
                        ]
                        functions: [
                            function() { // Import PSBT
                                openfileDialog.open()
                            },
                            function() { // "Save wallet config"
                                exportwalletDialog.visible = false
                                exportwalletDialog.exportFormat = "bsms"
                                exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                        + RoomWalletData.getValidFilename(AppModel.walletInfo.walletName)
                                        + ".bsms"
                                exportwalletDialog.open()
                            },
                            function() { // "Export invoices"
                                var currentDate = new Date();
                                var monthNames = ["JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                                                  "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"];
                                var day = currentDate.getDate();
                                var month = monthNames[currentDate.getMonth()];
                                var year = currentDate.getFullYear();
                                var today = day + '' + month + '' + year;;

                                exportwalletDialog.visible = false
                                exportwalletDialog.exportFormat = "pdf"
                                exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                        + RoomWalletData.getValidFilename(AppModel.walletInfo.walletName)
                                        + "_transaction_history_"
                                        + today
                                        + ".pdf"
                                exportwalletDialog.open()
                            },
                            function() {
                                console.warn("Manage group chat history")
                                var obj = {
                                    type: "show-manage-chat-history"
                                }
                                QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                                chatHistorySandbox.open()
                            },
                        ]
                        onItemClicked: {
                            functions[index]()
                        }
                    }
                }
                Item {
                    id: area_quickrecevied
                    width: parent.width * (1.0 - _walletDes.ratio)
                    height: parent.height
                    visible: (myRole !== "FACILITATOR_ADMIN")
                    Column {
                        width: parent.width
                        anchors.centerIn: parent
                        spacing: 16*QAPP_DEVICE_HEIGHT_RATIO
                        QText {
                            id: quickRecv
                            font.weight: Font.Bold
                            font.pixelSize: 16
                            color: "#031F2B";
                            text: STR.STR_QML_004
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        Rectangle {
                            width: 162
                            height: 162
                            radius: 12
                            color: "#FFFFFF"
                            anchors.horizontalCenter: parent.horizontalCenter
                            QRCodeItem {
                                id: qrCode
                                anchors.fill: parent
                                anchors.margins: 6
                                borderWitdh: 6
                                textInput: AppModel.walletInfo.walletAddress
                            }
                            layer.enabled: true
                            layer.effect: DropShadow {
                                horizontalOffset: 3
                                verticalOffset: 3
                                radius: 8.0
                                samples: 17
                                color: "#80000000"
                            }
                        }
                        Item{
                            width: 329
                            height: 72*QAPP_DEVICE_HEIGHT_RATIO
                            anchors.horizontalCenter: parent.horizontalCenter
                            Rectangle {
                                id: qrHex
                                anchors.fill: parent
                                color: "#FFFFFF"
                                radius: 4
                                layer.enabled: true
                                layer.effect: DropShadow {
                                    horizontalOffset: 3
                                    verticalOffset: 3
                                    radius: 8.0
                                    samples: 17
                                    color: "#80000000"
                                }
                            }
                            QText {
                                z: 1
                                anchors.fill: parent
                                anchors.margins: 5
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WrapAnywhere
                                font.weight: Font.Normal
                                font.pixelSize: 16
                                color: "#031F2B";
                                text: qrCode.textInput
                            }
                        }
                        Row {
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: 311
                            height: 48
                            spacing: 10
                            QIconTextButton {
                                width: 97
                                height: 48
                                label: STR.STR_QML_005
                                icons: ["copy-dark.svg", "copy-light.svg", "copy-dark.svg","copy-light.svg"]
                                fontPixelSize: 16
                                iconSize: 24
                                type: eTypeB
                                enabled: existWallet
                                onButtonClicked: {
                                    QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_COPY_ADDRESS, qrCode.textInput)
                                }
                            }
                            QIconTextButton {
                                width: 175
                                height: 48
                                label: STR.STR_QML_006
                                icons: [
                                    "visibility-dark.svg",
                                    "visibility-light.svg",
                                    "visibility-disabled.svg",
                                    "visibility-light.svg"
                                ]
                                fontPixelSize: 16
                                iconSize: 24
                                type: eTypeB
                                onButtonClicked: {QMLHandle.sendEvent(EVT.EVT_HOME_DISPLAY_ADDRESS, qrCode.textInput) }
                                enabled: AppModel.walletInfo.containsHWSigner
                            }
                            QTooltip {
                                width: 20
                                height: 20
                                anchors.verticalCenter: parent.verticalCenter
                                toolTip: STR.STR_QML_007
                                rightOfParent: true
                            }
                        }
                    }
                }
            }
        }
        Item {
            width: _item.width
            height: _item.height * 0.49
            Row {
                id: trans_lbl
                height: 24
                spacing: 8
                visible: AppModel.walletInfo !== null && AppModel.walletInfo.transactionHistory !== null
                QText {
                    height: 24
                    font.weight: Font.DemiBold
                    font.pixelSize: 20
                    color: "#031F2B";
                    font.family: "Lato"
                    text: STR.STR_QML_010
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                QText {
                    height: 24
                    font.weight: Font.Normal
                    font.pixelSize: 12
                    color: "#757575";
                    font.family: "Lato"
                    text: STR.STR_QML_010_number.arg(AppModel.walletInfo && AppModel.walletInfo.transactionHistory ? AppModel.walletInfo.transactionHistory.count : 0)
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            QButtonTextLink {
                height: 24
                label: STR.STR_QML_011
                direction: eRIGHT
                enabled: transaction_lst.count > 0
                anchors {
                    right: parent.right
                }
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_HOME_TRANSACTION_HISTORY_REQUEST)
                }
            }
            QText {
                id: trans_not_have
                anchors {
                    top: parent.top
                    topMargin: 100
                    horizontalCenter: parent.horizontalCenter
                }
                height: 24
                font.weight: Font.Normal
                font.pixelSize: 14
                color: "#323E4A";
                font.family: "Montserrat"
                text: STR.STR_QML_692
                visible: transaction_lst.count == 0
            }
            QListView {
                id: transaction_lst
                width: parent.width
                height: parent.height*(walletNeedBackup ? 0.6 : 0.9)
                model: AppModel.walletInfo.transactionHistory
                ScrollBar.vertical: ScrollBar { active: transaction_lst.contentHeight > transaction_lst.height }
                anchors {
                    right: parent.right
                    top: trans_lbl.bottom
                    topMargin: 12
                }
                clip: true
                delegate: QTransactionDelegate {
                    width: transaction_lst.width
                    height: 64
                    parentList: transaction_lst
                    transactionisReceiveTx: transaction_isReceiveTx
                    transactiontxid: transaction_txid
                    transactiondestinationList: transaction_destinationDisp_role
                    transactionstatus: transaction_status
                    transactionMemo: transaction_memo
                    transactionAmount: (transaction_isReceiveTx ? transaction_subtotal : transaction_total)
                    transactiontotalCurrency: (transaction_isReceiveTx ? transaction_subtotalCurrency : transaction_totalCurrency)
                    confirmation: Math.max(0, (AppModel.chainTip - transaction_height)+1)
                    transactionDate: transaction_blocktime
                    addressWidth: width*0.20
                    statusWidth: width*0.20
                    timeWidth: width*0.15
                    memoWidth: width*0.20
                    amountWidth: width*0.25
                    transactionIsRbf: transaction_isRbf
                    isFacilitatorAdmin: (myRole === "FACILITATOR_ADMIN")
                    onButtonClicked: {
                        QMLHandle.signalNotifySendEvent(EVT.EVT_HOME_TRANSACTION_INFO_REQUEST, transactiontxid)
                    }
                }
            }
            BusyIndicator {
                id: notxWaiting
                anchors.centerIn: transaction_lst
                running: transaction_lst.count === 0 && ClientController.isMatrixLoggedIn === false
            }
            Connections {
                target: AppModel
                onFinishedGetTransactionHistory: {notxWaiting.visible = false}
            }
        }
    }

    Loader {
        anchors.right: parent.right
        anchors.rightMargin: 30
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        sourceComponent: AppModel.walletInfo.isGlobalGroupWallet ? messageBubble : null
    }
    Component {
        id: messageBubble
        QMessageBubble { }
    }

    FileDialog {
        id: openfileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_HOME_IMPORT_PSBT, openfileDialog.file)
        }
    }

    FileDialog {
        id: exportwalletDialog
        property string exportFormat: "bsms"
        fileMode: FileDialog.SaveFile
        onAccepted: {
            if(exportwalletDialog.exportFormat === "pdf") {
                invoicePopup.open()
                invoicePopup.startPrintInvoices(exportwalletDialog.currentFile)
            }
            else {
                var exportObj = {
                    "export_type"    : "bsms",
                    "export_path"    : exportwalletDialog.currentFile
                };
                QMLHandle.sendEvent(EVT.EVT_HOME_EXPORT_BSMS, exportObj)
            }
        }
    }
    enabled: !isLocked
    property bool isLocked:  AppModel.walletInfo.dashboardInfo ? AppModel.walletInfo.dashboardInfo.isLocked : false
    Loader {
        anchors.fill: parent
        sourceComponent: isLocked ? locked : null
    }
    Component {
        id: locked
        QHomeLockdownInProgress {
            anchors.fill: parent
        }
    }

    Popup {
        id: invoicePopup
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}

        property int currentIndex: 0
        property int total: 100

        function startPrintInvoices(filepath) {
            contentInvoice.item.startPrintInvoices(filepath)
        }
        function progressChanged(index, total) {
            invoicePopup.currentIndex = index
            invoicePopup.total = total
        }
        Loader {
            id: contentInvoice
            sourceComponent: null
            visible: false
        }

        Rectangle {
            width: 300
            height: 222
            radius: 24
            color: "#FFFFFF"
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: -386/2
            Column {
                spacing: 8
                anchors.centerIn: parent
                QText {
                    width: 252
                    height: 28
                    text: "Processing"
                    font.weight: Font.Bold
                    font.family: "Lato"
                    font.pixelSize: 20
                    color: "#031F2B"
                    anchors.horizontalCenter: parent.horizontalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                Item {
                    width: 252
                    height: 16
                }
                QProgressbarTypeA {
                    width: 252
                    anchors.horizontalCenter: parent.horizontalCenter
                    percentage: invoicePopup.currentIndex*100/invoicePopup.total
                }
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    QText {
                        width: 252/2
                        height: 28
                        text: invoicePopup.currentIndex + "/" + invoicePopup.total
                        font.family: "Lato"
                        font.pixelSize: 16
                        color: "#031F2B"
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QText {
                        width: 252/2
                        height: 28
                        text: Math.round(invoicePopup.currentIndex*100/invoicePopup.total) + "%"
                        font.family: "Lato"
                        font.pixelSize: 16
                        color: "#031F2B"
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                    }
                }
                Item {
                    width: 252
                    height: 16
                }
                QTextButton {
                    width: 252
                    height: 48
                    label.text: "Cancel"
                    label.font.pixelSize: 16
                    type: eTypeB
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        invoicePopup.close()
                    }
                }
            }
        }
        onOpened : {
            invoicePopup.currentIndex = 0
            invoicePopup.total = 100
            contentInvoice.sourceComponent = invoiceComponent
        }
        onClosed: {
            invoicePopup.currentIndex = 0
            invoicePopup.total = 100
            contentInvoice.sourceComponent = null
        }
    }
    Component {
        id: invoiceComponent
        Repeater {
            id: listInvoices
            width: 728
            height: 496
            clip: true
            model: AppModel.walletInfo.transactionHistory
            delegate: QTransactionInvoice {
                txid            : transaction_txid
                total           : transaction_total
                subtotal        : transaction_subtotal
                blocktime       : transaction_blocktime
                fee             : transaction_fee
                memo            : transaction_memo
                hasChange       : transaction_hasChange
                changeAmount    : transaction_change_role.amount
                changeAddress   : transaction_change_role.address
                isRecieveTx     : transaction_isReceiveTx
                destinations    : transaction_destinationList
                interactive     : false
            }
            function startPrintInvoices(filepath) {
                var objects = [];
                for(var i = 0; i < listInvoices.count; i++) {
                    objects.push(listInvoices.itemAt(i))
                }
                // PDFPrinter.printInvoicesToPdf(filepath, objects)
                var exportObj = {
                    "export_type"    : "pdf",
                    "export_path"    : filepath,
                    "export_data"    : objects
                };
                QMLHandle.sendEvent(EVT.EVT_HOME_EXPORT_BSMS, exportObj)
            }
        }
    }
    Connections {
        target: PDFPrinter
        onFinished: {
            invoicePopup.close()
        }
        onProgressChanged: {
            invoicePopup.progressChanged(index, total)
        }
    }
}
