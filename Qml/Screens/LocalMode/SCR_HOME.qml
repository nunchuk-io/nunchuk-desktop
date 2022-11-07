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
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: homeroot


    Component {
        id: step1
        QHomeInitialStep1 {
            anchors.fill: parent
        }
    }
    Component {
        id: step2
        QHomeInitialStep2 {
            anchors.fill: parent
        }
    }

    Component {
        id: step3
        Item {
            anchors.fill: parent
            QAreaWalletDetail{
                width: parent.width - 48
                height: parent.height * 0.42
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 32
                }
                Row{
                    anchors.fill: parent
                    Item {
                        height: parent.height
                        width: parent.width * 0.6

                        QText {
                            id: displayWalletname
                            width: 460
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

                        QIconButton{
                            id:_more
                            width: 24
                            height: 24
                            anchors{
                                right: parent.right
                                rightMargin: 36
                                top: parent.top
                                topMargin: 30
                            }
                            icon: "qrc:/Images/Images/OnlineMode/more_horiz_24px.png"
                            onClicked: {
                                optionMenu.popup(_more,mouse.x - 250 + 24,24)
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
                                topMargin: 16
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
                                    text: AppModel.walletInfo.walletN === 1 ? STR.STR_QML_070 : qsTr("%1/%2 %3").arg(AppModel.walletInfo.walletM).arg(AppModel.walletInfo.walletN).arg(STR.STR_QML_069);
                                    color: "#031F2B"
                                    font.weight: Font.Bold
                                    font.pixelSize: 10
                                }
                            }
                            Rectangle{
                                width: 70
                                height: parent.height
                                radius: 20
                                visible: AppModel.walletInfo.isSharedWallet
                                color: "#EAEAEA"
                                Row {
                                    anchors.centerIn: parent
                                    spacing: 4
                                    QImage {
                                        width: 12
                                        height: 12
                                        source: "qrc:/Images/Images/OnlineMode/Joint wallet_031F2B.png"
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
                        }

                        Column {
                            id: balanceInfoCol
                            width: parent.width*0.9
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.top: parent.top
                            anchors.topMargin: 143
                            spacing: 4
                            QText {
                                id: wlBalance
                                width: parent.width
                                color: "#F1FAFE"
                                font.pixelSize: 28
                                font.family: "Lato"
                                font.weight: Font.Bold
                                text: qsTr("%1 %2").arg(AppModel.walletInfo.walletBalance).arg((AppSetting.unit === 1) ? "sat" : "BTC")
                            }
                            QText {
                                id: wlBalanceCurrency
                                width: parent.width
                                color: "#F1FAFE"
                                font.pixelSize: 16
                                font.family: "Lato"
                                font.weight: Font.Medium
                                text: qsTr("$%1 USD").arg(AppModel.walletInfo.walletBalanceUSD)
                            }
                        }

                        QButtonTextLink {
                            height: 24
                            label: STR.STR_QML_574
                            icon: ["qrc:/Images/Images/OnlineMode/arrow_outline_right.png", "qrc:/Images/Images/OnlineMode/arrow_outline_right.png", "qrc:/Images/Images/OnlineMode/arrow_outline_right.png"]
                            textColor: ["#FFFFFF", "#FFFFFA", "#FFFFFF"]
                            direction: eRIGHT
                            _mirror:false
                            enabled: walletmanagerlst.count > 0
                            anchors {
                                left: parent.left
                                leftMargin: 24
                                top: parent.top
                                topMargin: 239
                            }
                            onButtonClicked: {
                                QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_INFO_REQUEST)
                            }
                        }


                        Row {
                            id: buttongHandles
                            spacing: parent.width - 204 *2 - 24*2
                            anchors {
                                horizontalCenter: parent.horizontalCenter
                                top: parent.top
                                topMargin: 294
                            }
                            QTextButton {
                                label.text: STR.STR_QML_002
                                width: 204
                                type: eTypeF
                                enabled: walletmanagerlst.count > 0
                                onButtonClicked: {
                                    if(AppModel.walletInfo.isSharedWallet){
                                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                                        QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WL_SEND_REQUEST,AppModel.walletInfo.walletId)
                                    }
                                    else{
                                        QMLHandle.sendEvent(EVT.EVT_HOME_SEND_REQUEST)
                                    }
                                }
                            }
                            QTextButton {
                                label.text: STR.STR_QML_003
                                width: 204
                                type: eTypeF
                                enabled: walletmanagerlst.count > 0
                                onButtonClicked: { QMLHandle.sendEvent(EVT.EVT_HOME_RECEIVE_REQUEST) }
                            }
                        }

                        /* for menu */
                        QContextMenu {
                            id: optionMenu
                            labels: [
                                STR.STR_QML_532,
                                STR.STR_QML_347,
                            ]
                            icons: [
                                "qrc:/Images/Images/import_031F2B.png",
                                "qrc:/Images/Images/OnlineMode/Backup.png",
                            ]
                            onItemClicked: {
                                switch(index){
                                case 0: // Import PSBT
                                    openfileDialog.open()
                                    break;
                                case 1: // "Save wallet config"
                                    exportwalletDialog.visible = false
                                    exportwalletDialog.exportFormat = NUNCHUCKTYPE.DESCRIPTOR
                                    exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                            + RoomWalletData.getValidFilename(RoomWalletData.walletName)
                                            + ".bsms"
                                    exportwalletDialog.open()
                                    break;
                                default:
                                    break;
                                }
                            }
                        }
                    }
                    Item {
                        id: area_quickrecevied
                        width: parent.width * 0.4
                        height: parent.height
                        Column {
                            width: parent.width
                            anchors.centerIn: parent
                            spacing: 16
                            QText {
                                id: quickRecv
                                font.weight: Font.Bold
                                font.pixelSize: 16
                                color: "#031F2B";
                                text: STR.STR_QML_004
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Item{
                                width: parent.height*0.3 + 50
                                height: width
                                anchors.horizontalCenter: parent.horizontalCenter
                                QRCodeItem {
                                    id: qrCode
                                    anchors.fill: parent
                                    borderWitdh: 6
                                    textInput: AppModel.walletInfo.walletAddress
                                    visible: false
                                }
                                DropShadow {
                                    anchors.fill: parent
                                    horizontalOffset: 3
                                    verticalOffset: 3
                                    radius: 8.0
                                    samples: 17
                                    color: "#80000000"
                                    source: qrCode
                                }
                            }

                            Item{
                                width: parent.width * 0.8
                                height: 72
                                anchors.horizontalCenter: parent.horizontalCenter
                                Rectangle {
                                    id: qrHex
                                    anchors.fill: parent
                                    color: "#FFFFFF"
                                    radius: 4
                                }
                                DropShadow {
                                    anchors.fill: parent
                                    horizontalOffset: 3
                                    verticalOffset: 3
                                    radius: 8.0
                                    samples: 17
                                    color: "#80000000"
                                    source: qrHex
                                    QText {
                                        anchors.fill: parent
                                        anchors.leftMargin: 5
                                        anchors.topMargin: 5
                                        anchors.rightMargin: 5
                                        anchors.bottomMargin: 5
                                        horizontalAlignment: Text.AlignHCenter
                                        wrapMode: Text.WrapAnywhere
                                        font.weight: Font.Medium
                                        font.pixelSize: 16
                                        color: "#031F2B";
                                        text: qrCode.textInput
                                        anchors.centerIn: parent
                                    }
                                }
                            }

                            Row {
                                anchors.horizontalCenter: parent.horizontalCenter
                                width: parent.width - spacing*2
                                height: 48
                                spacing: (parent.width - 97 - 175 - 20)/4
                                QButtonIcon {
                                    width: 97
                                    height: 48
                                    label: STR.STR_QML_005
                                    icons: ["Copy_031F2B.png", "Copy_031F2B.png", "Copy_9CAEB8.png","Copy_9CAEB8.png"]
                                    fontPixelSize: 16
                                    iconSize: 16
                                    type: eSECOND
                                    enabled: walletmanagerlst.count > 0
                                    onButtonClicked: {
                                        QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_COPY_ADDRESS, qrCode.textInput)
                                    }
                                }
                                QButtonIcon {
                                    width: 175
                                    height: 48
                                    label: STR.STR_QML_006
                                    icons: ["visibility_031F2B.png","visibility_031F2B.png","visibility_F1FAFE.png","visibility_F1FAFE.png"]
                                    fontPixelSize: 16
                                    iconSize: 16
                                    type: eSECOND
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
                width: parent.width - 64
                height: parent.height * 0.45
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                    bottomMargin: 50
                }
                Item {
                    width: parent.width
                    height: parent.height
                    anchors.right: parent.right
                    QText {
                        id: trans_lbl
                        height: 24
                        font.weight: Font.DemiBold
                        font.pixelSize: 14
                        color: "#323E4A";
                        font.family: "Montserrat"
                        text: (AppModel.walletInfo !== null) && AppModel.walletInfo.transactionHistory !== null ? STR.STR_QML_010.arg(AppModel.walletInfo.transactionHistory.count) : ""
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
                    QListView {
                        id: transaction_lst
                        width: parent.width
                        height: 327
                        model: AppModel.walletInfo.transactionHistoryShort
                        anchors {
                            left: parent.left
                            top: trans_lbl.bottom
                            topMargin: 24
                        }
                        clip: true
                        delegate: QTransactionDelegate {
                            width: transaction_lst.width
                            height: 40
                            transactionisReceiveTx:transaction_isReceiveTx
                            transactiontxid:transaction_txid
                            transactiondestinationList:transaction_destinationList
                            transactionstatus: transaction_status
                            transactionMemo: transaction_memo
                            transactiontotalBTC: (transaction_isReceiveTx ? "" : "- ") + transaction_totalBTC
                            transactiontotalUSD: (transaction_isReceiveTx ? "" : "- ") + transaction_totalUSD
                            confirmation: Math.max(0, (AppModel.chainTip - transaction_height)+1)
                            transactionDate: transaction_blocktime
                            addressWidth: transaction_lst.width*0.20
                            statusWidth: transaction_lst.width*0.20
                            timeWidth: transaction_lst.width*0.15
                            memoWidth: transaction_lst.width*0.20
                            amountWidth: transaction_lst.width*0.25
                            onButtonClicked: {
                                QMLHandle.sendEvent(EVT.EVT_HOME_TRANSACTION_INFO_REQUEST, transaction_txid)
                            }
                        }
                        ScrollBar.vertical: ScrollBar { active: true }
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
        }
    }

    Row {
        Item {
            id: pannel_left
            width: 377
            height: homeroot.height
            transformOrigin: Item.Center
            Rectangle {
                id: bgColor
                anchors.fill: parent
                color: "#031F2B"
                visible: false
            }
            DropShadow {
                id: dropshodow1
                anchors.fill: bgColor
                radius: 24
                samples: 16
                color: Qt.rgba(0, 0, 0, 0.14)
                source: bgColor
            }
            QMenuBar {
                width: 72
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                currentIndex: 1
                labels: [
                    STR.STR_QML_460,
                    STR.STR_QML_461,
                    ClientController.isNunchukLoggedIn ? STR.STR_QML_462 : STR.STR_QML_533
                ]
                enables: [
                    ClientController.isNunchukLoggedIn,
                    true,
                    true
                ]
                onLocalModeRequest: {
                    if(0 === AppModel.nunchukMode){
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE)
                    }
                    else{
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINE_HWL)
                    }
                }
                onOnlineModeRequest: {
                    QMLHandle.sendEvent(EVT.EVT_GOTO_HOME_CHAT_TAB)
                }
                onOpenSettingRequest: {
                    QMLHandle.sendEvent(EVT.EVT_APP_SETTING_REQUEST)
                }
                onSignoutRequest: {
                    ClientController.requestSignout()
                }
                onUpdateProfileRequest: {
                    QMLHandle.sendEvent(EVT.EVT_ROOT_UPDATE_PROFILE)
                }
                onSigninRequest: {
                    QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                }
            }
            Item {
                width: 304
                height: parent.height
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                Column {
                    anchors.fill: parent
                    anchors.topMargin: 24
                    anchors.bottomMargin: 24
                    spacing: 24
                    Item {
                        width: parent.width
                        height: (parent.height - 215)/2
                        Column {
                            anchors.fill: parent
                            spacing: 12
                            Item {
                                width: parent.width
                                height: 36
                                QText {
                                    id: walletsTextTitle
                                    anchors {
                                        left: parent.left
                                        leftMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                    text: STR.STR_QML_000
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                    color: "#FFFFFF"
                                    font.weight: Font.Bold
                                }
                                QConnection {
                                    width: label.paintedWidth + 12 + 20 + 6
                                    height: 24
                                    radius: 20
                                    label.text: {
                                        var mode = "";
                                        switch(AppSetting.connectionState){
                                        case NUNCHUCKTYPE.OFFLINE: mode = STR.STR_QML_612; break;
                                        case NUNCHUCKTYPE.SYNCING: mode = STR.STR_QML_613; break;
                                        case NUNCHUCKTYPE.ONLINE: mode = STR.STR_QML_611; break;
                                        default: mode = ""
                                        }
                                        var server = ""
                                        switch(AppSetting.primaryServer){
                                        case NUNCHUCKTYPE.MAIN: server = ""; break;
                                        case NUNCHUCKTYPE.TESTNET: server = "(" + STR.STR_QML_013 + ")"; break;
                                        case NUNCHUCKTYPE.SIGNET: server = "(" + STR.STR_QML_584 + ")"; break;
                                        default: server = ""
                                        }
                                        return mode + server
                                    }
                                    anchors.left: walletsTextTitle.right
                                    anchors.leftMargin: 24
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                QIconButton{
                                    width: 24
                                    height: 24
                                    anchors.right: parent.right
                                    anchors.rightMargin: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    icon:"qrc:/Images/Images/add-light.svg"
                                    onClicked: {
                                        QMLHandle.sendEvent(EVT.EVT_HOME_ADD_WALLET_REQUEST)
                                    }
                                }
                            }
                            Item {
                                width: parent.width
                                height: parent.height - 48
                                QWalletManagerDelegate {
                                    width: parent.width
                                    height: 64
                                    enabled: false
                                    visible: walletmanagerlst.count == 0
                                    isEscrow: false
                                    walletName : STR.STR_QML_014
                                    walletBalance: "0"
                                    walletM: "0"
                                    walletN: "0"
                                    Rectangle {
                                        anchors.fill: parent
                                        color: "#031F2B"
                                        opacity: 0.5
                                    }
                                }
                                QListView {
                                    id: walletmanagerlst
                                    anchors.fill: parent
                                    model: AppModel.walletList
                                    clip: true
                                    currentIndex: AppModel.walletListCurrentIndex
                                    delegate: QWalletManagerDelegate {
                                        width: 304
                                        isCurrentIndex: index === walletmanagerlst.currentIndex
                                        isEscrow: model.wallet_Escrow
                                        isShared: model.wallet_isSharedWallet
                                        walletCurrency: model.wallet_Balance_USD
                                        walletName :model. wallet_name
                                        walletBalance: model.wallet_Balance
                                        walletM: model.wallet_M
                                        walletN: model.wallet_N
                                        onButtonClicked: {
                                            if(index !== walletmanagerlst.currentIndex || AppModel.walletInfo.walletName === ""){
                                                QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, index)
                                            }
                                        }
                                    }
                                    ScrollBar.vertical: ScrollBar { id: scrollWallet; active: true ;function wheel(up){if(up){decrease()}else{increase()}}}
                                    MouseArea { anchors.fill: parent;z: 10;propagateComposedEvents: true;onWheel: { scrollWallet.wheel(wheel.angleDelta.y > 0);}}
                                }
                                QImage {
                                    width: walletmanagerlst.width
                                    height: 36
                                    anchors.bottom: walletmanagerlst.bottom
                                    anchors.bottomMargin: -1
                                    anchors.horizontalCenter: walletmanagerlst.horizontalCenter
                                    source: "qrc:/Images/Images/OnlineMode/Cover.png"
                                    visible: walletmanagerlst.count > 0
                                }
                            }
                        }
                    }
                    Item {
                        width: parent.width
                        height: (parent.height - 215)/2
                        Column {
                            anchors.fill: parent
                            spacing: 12
                            Item {
                                width: parent.width
                                height: 36
                                QText {
                                    id: keysTextTitle
                                    anchors {
                                        left: parent.left
                                        leftMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                    text: STR.STR_QML_015
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                    color: "#FFFFFF"
                                    font.weight: Font.Bold
                                }
                                QIconButton{
                                    width: 24
                                    height: 24
                                    anchors.right: parent.right
                                    anchors.rightMargin: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    icon:"qrc:/Images/Images/add-light.svg"
                                    onClicked: {
                                        QMLHandle.sendEvent(EVT.EVT_HOME_ADD_NEW_SIGNER_REQUEST)
                                    }
                                }
                            }
                            Item {
                                width: parent.width
                                height: parent.height - 48
                                QMasterSignerDelegate {
                                    width: 304
                                    height: 54
                                    enabled: false
                                    visible: !flickerSignerList.signerReady
                                    signername : STR.STR_QML_016
                                    signerXFP: "XFP: DEADBEEF"
                                    Rectangle {
                                        anchors.fill: parent
                                        color: "#031F2B"
                                        opacity: 0.5
                                    }
                                }
                                Flickable {
                                    id: flickerSignerList
                                    anchors.fill: parent
                                    property bool signerReady: (mastersignerlist.count > 0) || (remoteSignerlist.count > 0)
                                    visible: signerReady
                                    clip: true
                                    flickableDirection: Flickable.VerticalFlick
                                    interactive: true
                                    contentHeight: contentDisplay.height
                                    ScrollBar.vertical: ScrollBar { id: scrollSigner; active: true ; function wheel(up){if(up){decrease()}else{increase()}}}
                                    MouseArea { anchors.fill: parent;z: 10;propagateComposedEvents: true;onWheel: { scrollSigner.wheel(wheel.angleDelta.y > 0);}}
                                    Column {
                                        id: contentDisplay
                                        QListView {
                                            id: mastersignerlist
                                            width: 304
                                            height: 48*mastersignerlist.count
                                            model: AppModel.masterSignerList
                                            interactive: false
                                            delegate: QMasterSignerDelegate {
                                                width: 304
                                                height: 48
                                                signername : model.master_signer_name
                                                signerXFP: "XFP: " + model.master_signer_fingerPrint
                                                devicetype: model.master_signer_deviceType
                                                signerType: model.master_signer_type
                                                isPrimaryKey: model.master_signer_primary_key
                                                onButtonClicked: {
                                                    QMLHandle.sendEvent(EVT.EVT_HOME_MASTER_SIGNER_INFO_REQUEST, index)
                                                }
                                            }
                                        }
                                        QListView {
                                            id: remoteSignerlist
                                            width: 304
                                            height: 48*remoteSignerlist.count
                                            model: AppModel.remoteSignerList
                                            interactive: false
                                            delegate: QMasterSignerDelegate {
                                                width: 304
                                                height: 48
                                                nameWidth: 170
                                                signername : model.singleSigner_name
                                                signerXFP: "XFP: " + model.singleSigner_masterFingerPrint
                                                signerType: NUNCHUCKTYPE.AIRGAP
                                                onButtonClicked: {
                                                    QMLHandle.sendEvent(EVT.EVT_HOME_REMOTE_SIGNER_INFO_REQUEST, index)
                                                }
                                            }
                                        }
                                    }
                                }
                                QImage {
                                    width: flickerSignerList.width
                                    height: 36
                                    anchors.bottom: flickerSignerList.bottom
                                    anchors.bottomMargin: -1
                                    anchors.horizontalCenter: flickerSignerList.horizontalCenter
                                    source: "qrc:/Images/Images/OnlineMode/Cover.png"
                                }
                            }
                        }
                    }
                    Item {
                        width: parent.width
                        height: 167
                        Column {
                            anchors.fill: parent
                            spacing: 12
                            Item {
                                width: parent.width
                                height: 36
                                QText {
                                    id: txFeeTitle
                                    anchors {
                                        left: parent.left
                                        leftMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                    text: STR.STR_QML_017
                                    color: "#FFFFFF"
                                    font.weight: Font.Bold
                                    font.pixelSize: 16
                                }
                                Rectangle {
                                    width: 140
                                    height: 16
                                    radius: 20
                                    anchors {
                                        left: txFeeTitle.right
                                        leftMargin: 8
                                        verticalCenter: parent.verticalCenter
                                    }
                                    Row {
                                        spacing: 4
                                        anchors.centerIn: parent
                                        QImage {
                                            width: 12
                                            height: 12
                                            source: "qrc:/Images/Images/OnlineMode/history-24px.png"
                                            anchors.verticalCenter: parent.verticalCenter
                                        }
                                        QText{
                                            font.family: "Lato"
                                            font.weight: Font.Bold
                                            font.pixelSize: 10
                                            color: "#031F2B"
                                            text: "Updated " + AppModel.lasttimeCheckEstimatedFee
                                            anchors.verticalCenter: parent.verticalCenter
                                        }
                                    }
                                }
                            }
                            Row {
                                id: rowFeeInfo
                                readonly property var prioritiesLabel: [
                                    STR.STR_QML_020,
                                    STR.STR_QML_019,
                                    STR.STR_QML_018
                                ]
                                readonly property var prioritiesFee: [
                                    AppModel.hourFee,
                                    AppModel.halfHourFee,
                                    AppModel.fastestFee
                                ]
                                readonly property var prioritiesFeeUSD: [
                                    AppModel.hourFeeUSD,
                                    AppModel.halfHourFeeUSD,
                                    AppModel.fastestFeeUSD
                                ]
                                width: parent.width - 24
                                height: 83
                                spacing: 8
                                anchors.horizontalCenter: parent.horizontalCenter
                                Repeater {
                                    model: 3
                                    Rectangle {
                                        width: 89
                                        height: 83
                                        radius: 8
                                        color: "transparent"
                                        border.color: "#EAEAEA"
                                        Column {
                                            anchors.fill: parent
                                            anchors.topMargin: 9
                                            anchors.bottomMargin: 9
                                            spacing: 0
                                            QText {
                                                width: 50
                                                font.family: "Lato"
                                                font.pixelSize: 12
                                                lineHeightMode: Text.FixedHeight
                                                lineHeight: 16
                                                color: "#FFFFFF"
                                                wrapMode: Text.WordWrap
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                text: rowFeeInfo.prioritiesLabel[index]
                                            }
                                            QText {
                                                width: 76
                                                font.family: "Lato"
                                                font.pixelSize: 12
                                                font.weight: Font.Bold
                                                lineHeightMode: Text.FixedHeight
                                                lineHeight: 16
                                                color: "#FFFFFF"
                                                wrapMode: Text.WordWrap
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                text: qsTr("%1 sat/vB").arg(rowFeeInfo.prioritiesFee[index])
                                            }
                                            QText {
                                                width: 76
                                                font.family: "Lato"
                                                font.pixelSize: 12
                                                font.weight: Font.Bold
                                                lineHeightMode: Text.FixedHeight
                                                lineHeight: 16
                                                color: "#A7F0BA"
                                                wrapMode: Text.WordWrap
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                text: qsTr("$%1").arg(rowFeeInfo.prioritiesFeeUSD[index])
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Rectangle {
            width: homeroot.width - pannel_left.width
            height: homeroot.height
            color: "#F1FAFE"
            Loader {
                width: parent.width
                height: parent.height
                anchors.centerIn: parent
                sourceComponent: {
                    if(walletmanagerlst.count > 0){ return step3;}
                    else{
                        if(mastersignerlist.count > 0){ return step2;}
                        else{ return step1 ;}
                    }
                }
            }
        }
    }
    Popup {
        id: displayAddressBusybox
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape
        background: Item{}
        property string addrToVerify: ""
        Rectangle {
            id: displayAddressMask
            width: 500
            height: 250
            radius: 8
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 500
                    height: 250
                    radius: 8
                }
            }
            QCloseButton {
                anchors {
                    right: parent.right
                    rightMargin: 16
                    top: parent.top
                    topMargin: 16
                }
                onClicked: {displayAddressBusybox.close()}
            }
            Column {
                spacing: 16
                anchors.centerIn: parent
                QBusyIndicator {
                    width: 70
                    height: 70
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 14
                    font.weight: Font.Bold
                    text: STR.STR_QML_008
                }
                Rectangle {
                    width: 450
                    height: 60
                    color: Qt.rgba(0, 0, 0, 0.1)
                    anchors.horizontalCenter: parent.horizontalCenter
                    QText {
                        anchors.fill: parent
                        anchors.leftMargin: 5
                        anchors.rightMargin: 5
                        anchors.topMargin: 5
                        anchors.bottomMargin: 5
                        wrapMode: Text.WrapAnywhere
                        horizontalAlignment: Text.AlignHCenter
                        text: AppModel.walletInfo.walletAddress
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "#323E4A"
                    }
                }
            }
        }
        DropShadow {
            anchors.fill: displayAddressMask
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: displayAddressMask
        }
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
        property int exportFormat: -1
        fileMode: FileDialog.SaveFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_HOME_EXPORT_BSMS, exportwalletDialog.currentFile)
        }
    }
    Connections {
        target: AppModel
        onStartDisplayAddress: {
            if(isOnTop) displayAddressBusybox.open()
            else displayAddressBusybox.close()
        }
        onFinishedDisplayAddress: {
            displayAddressBusybox.close()
        }
    }
    QPopupGuestMode{
        id: guestMode
        onGotItClicked: {
            close()
        }
        Component.onCompleted: {
            if(!ClientController.isNunchukLoggedIn && !ClientController.guestMode){
                ClientController.guestMode = true
                guestMode.open()
            }
        }
    }
}
