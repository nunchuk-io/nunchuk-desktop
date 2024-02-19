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
import "../../Components/customizes/Wallets"
import "../../Components/origins"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR

Item {
    signal askDeny(var deny)
    property int count: walletList.count
    property bool isPending: pendingList.count > 0 ? true : false
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
            Loader {
                visible: (walletList.count + pendingList.count) === 0
                anchors.fill: parent
                sourceComponent: (walletList.count + pendingList.count) >= 0 ? null : loadingWalletBusy
            }
            Flickable {
                id: flickerWalletList
                anchors.fill: parent
                clip: true
                flickableDirection: Flickable.VerticalFlick
                interactive: true
                contentHeight: contentDisplay.childrenRect.height
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: contentDisplay
                    width: parent.width
                    spacing: 4
                    QListView {
                        id: pendingList
                        height: contentHeight
                        width: parent.width - 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        visible: pendingList.count > 0
                        snapMode: ListView.NoSnap
                        model: GroupWallet.dashboards
                        interactive: false
                        currentIndex: GroupWallet.currentIndex
                        spacing: 4
                        delegate: QPendingWallet {
                            id: pendingdelegate
                            width: pendingList.width
                            name_person: modelData.userName
                            email_person: modelData.userEmail
                            user_accepted: modelData.accepted
                            isCurrentIndex: (!walletList.visible) ? (pendingList.visible) && (index === pendingList.currentIndex) :
                                                                    (pendingList.visible) && (index === pendingList.currentIndex) && (GlobalData.listFocusing === 0)
                            onDeny: {
                                GlobalData.listFocusing = 0
                                GroupWallet.currentIndex = index
                                var obj = {
                                    type: "deny",
                                    group_id: modelData.groupId
                                }
                                askDeny(obj)
                            }
                            onAccept: {
                                GlobalData.listFocusing = 0
                                GroupWallet.currentIndex = index
                                var obj = {
                                    type: "accept",
                                    group_id: modelData.groupId
                                }
                                QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                            }
                            onDashboard: {
                                GlobalData.listFocusing = 0
                                GroupWallet.currentIndex = index
                                var obj = {
                                    type: "dashboard",
                                    group_id: modelData.groupId
                                }
                                QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                            }
                            onButtonClicked: {
                                GlobalData.listFocusing = 0
                                GroupWallet.currentIndex = index
//                                if (user_accepted) {
                                    var obj = {
                                        type: "dashboard",
                                        group_id: modelData.groupId
                                    }
                                    QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
//                                }
                            }

                            layer.enabled: true
                            layer.effect: OpacityMask {
                                maskSource: Rectangle {
                                    width: pendingdelegate.width
                                    height: pendingdelegate.height
                                    radius: 8
                                }
                            }
                        }
                    }
                    QListView {
                        id: walletList
                        height: contentHeight
                        width: parent.width - 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        visible: walletList.count > 0
                        model: AppModel.walletList
                        interactive: false
                        currentIndex: AppModel.walletListCurrentIndex
                        spacing: 4
                        delegate: QWalletManagerDelegate {
                            id: walletListdelegate
                            width: walletList.width
                            isCurrentIndex: (!pendingList.visible) ? (walletList.visible) && (index === walletList.currentIndex) :
                                                                     (walletList.visible) && (index === walletList.currentIndex) && (GlobalData.listFocusing === 1)
                            isEscrow: model.wallet_Escrow
                            isShared: model.wallet_isSharedWallet
                            isAssisted: model.wallet_isAssistedWallet
                            walletCurrency: model.wallet_Balance_Currency
                            walletName :model.wallet_name
                            walletBalance: model.wallet_Balance
                            walletM: model.wallet_M
                            walletN: model.wallet_N
                            isDashboard: model.wallet_group_id !== ""
                            isLocked: model.wallet_dashboard ? model.wallet_dashboard.isLocked : false
                            walletRole: model.wallet_role
                            hasOwner: model.wallet_hasOwner
                            primaryOwner: model.wallet_primaryOwner
                            onDashboard: {
                                GlobalData.listFocusing = 1
                                var obj = {
                                    type: "wallet_dashboard",
                                    data: index,
                                    group_id: model.wallet_group_id
                                }
                                QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                            }
                            onButtonClicked: {
                                GlobalData.listFocusing = 1
                                var obj = {
                                    type: "selected",
                                    data: index,
                                    group_id: model.wallet_group_id
                                }
                                QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                            }

                            layer.enabled: true
                            layer.effect: OpacityMask {
                                maskSource: Rectangle {
                                    width: walletListdelegate.width
                                    height: walletListdelegate.height
                                    radius: 8
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    readonly property int _LIMIT_WAITING_BUSY: 5000
    Component {
        id: loadingWalletBusy
        Item {
            anchors.fill: parent
            QWalletManagerDelegate {
                visible: !busyIdct.running
                width: parent.width
                height: 64
                enabled: false
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
            BusyIndicator {
                id: busyIdct
                anchors.centerIn: parent
                running: true
            }
            Timer {
                interval: _LIMIT_WAITING_BUSY
                running: true
                onTriggered: busyIdct.running = false
            }
        }
    }
}
