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
import "../../../localization/STR_QML.js" as STR

Item {
    id: _item
    property bool existWallet: false
    property string myRole: AppModel.walletInfo.myRole
    anchors.fill: parent
    anchors.margins: 24
    Column {
        spacing: 24
        Rectangle {
            visible: AppModel.walletInfo.needBackup
            width: _item.width
            height: 80
            radius: 8
            color: "#FDEBD2"
            Row {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8
                QImage {
                    height: 36
                    width: 36
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
                    _info2.open()
                }
            }
        }
        QAreaWalletDetail{
            id: _walletDes
            width: _item.width
            height: _item.height * 0.49
            isAssisted: AppModel.walletInfo.isAssistedWallet
            isHotWallet: AppModel.walletInfo.needBackup
            Row{
                anchors.fill: parent
                Item {
                    height: parent.height
                    width: parent.width * _walletDes.ratio

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
                        visible: myRole !== "OBSERVER"
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
                        Rectangle{
                            width: 70
                            height: parent.height
                            radius: 20
                            visible: AppModel.walletInfo.isAssistedWallet
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
                                    text: STR.STR_QML_679
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
                            text: qsTr("%1 %2").arg(AppModel.walletInfo.walletBalance).arg(RoomWalletData.unitValue)
                        }
                        QText {
                            id: wlBalanceCurrency
                            width: parent.width
                            color: "#F1FAFE"
                            font.pixelSize: 16
                            font.family: "Lato"
                            font.weight: Font.Medium
                            text: qsTr("%1%2 %3")
                            .arg(AppSetting.currencySymbol)
                            .arg(AppModel.walletInfo.walletBalanceCurrency)
                            .arg(AppSetting.currency)
                        }
                    }

                    QButtonTextLink {
                        height: 24
                        label: STR.STR_QML_574
                        icon: [
                            "qrc:/Images/Images/OnlineMode/arrow_outline_right.png",
                            "qrc:/Images/Images/OnlineMode/arrow_outline_right.png",
                            "qrc:/Images/Images/OnlineMode/arrow_outline_right.png"
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
                            bottom: parent.bottom
                            bottomMargin: 24
                        }
                        QTextButton {
                            label.text: STR.STR_QML_002
                            width: 204
                            type: eTypeF
                            enabled: existWallet && (myRole !== "OBSERVER")
                            onButtonClicked: {
                                if(AppModel.walletInfo.isSharedWallet){
                                    QMLHandle.sendEvent(EVT.EVT_GOTO_HOME_CHAT_TAB)
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
                            enabled: existWallet
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
                    width: parent.width * (1.0 - _walletDes.ratio)
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
                            height: 72
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
                                iconSize: 16
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
                                    "visibility_031F2B.png",
                                    "visibility_031F2B.png",
                                    "visibility_F1FAFE.png",
                                    "visibility_F1FAFE.png"
                                ]
                                fontPixelSize: 16
                                iconSize: 16
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
            height: _item.height * 0.45
            Item {
                anchors.fill: parent
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
                    height: 6*64
                    model: AppModel.walletInfo.transactionHistory
                    interactive: false
                    cacheBuffer: 0
                    anchors {
                        right: parent.right
                        top: trans_lbl.bottom
                        topMargin: 12
                    }
                    clip: true
                    delegate: QTransactionDelegate {
                        width: transaction_lst.width
                        height: 64
                        transactionisReceiveTx:transaction_isReceiveTx
                        transactiontxid: transaction_txid
                        transactiondestinationList: transaction_destinationDisp_role
                        transactionstatus: transaction_status
                        transactionMemo: transaction_memo
                        transactionAmount: transaction_isReceiveTx ? transaction_subtotal : transaction_total
                        transactiontotalCurrency: transaction_isReceiveTx ? transaction_subtotalCurrency : transaction_totalCurrency
                        confirmation: Math.max(0, (AppModel.chainTip - transaction_height)+1)
                        transactionDate: transaction_blocktime
                        addressWidth: width*0.20
                        statusWidth: width*0.20
                        timeWidth: width*0.15
                        memoWidth: width*0.20
                        amountWidth: width*0.25
                        transactionIsRbf: transaction_isRbf
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

}

