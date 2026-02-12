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
import Features.Wallets.ViewModels 1.0
import "../../Components/customizes/Wallets"
import "../../Components/origins"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    id: walletsRoot
    color: "transparent"
    signal askDeny(var deny)
    property int  count: walletList.count + pendingList.count + pendingGroupList.count
    property bool isPending: pendingList.count > 0 ? true : false
    width: parent.width
    height: (parent.height - 215)/2
    clip: true
    readonly property int _FOCUS_PENDING_GROUP_WALLET: 0
    readonly property int _FOCUS_PENDING_WALLET: 1
    readonly property int _FOCUS_WALLET: 2
    Column {
        width: parent.width
        spacing: 12
        Item {
            width: parent.width
            height: 24
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
                    OnBoarding.screenFlow = "addAWallet"
                    QMLHandle.sendEvent(EVT.EVT_ONBOARDING_REQUEST)
                }
            }
        }
        Item {
            width: parent.width
            height: walletsRoot.height - (40)
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
                contentHeight: contentDisplay.height
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: contentDisplay
                    width: parent.width
                    spacing: 4
                    QListView {
                        id: pendingGroupList
                        height: contentHeight
                        width: parent.width - 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        visible: pendingGroupList.count > 0
                        snapMode: ListView.NoSnap
                        model: SharedWallet.sandboxList
                        interactive: false
                        currentIndex: SharedWallet.currentIndex
                        spacing: 4
                        delegate: QPendingGroupWallet {
                            width: pendingGroupList.width
                            name_group: model.group_name
                            isCurrentIndex: (!walletList.visible) ? (pendingGroupList.visible) && (index === pendingGroupList.currentIndex) :
                                                                    (pendingGroupList.visible) && (index === pendingGroupList.currentIndex)
                                                                    && (GlobalData.listFocusing === _FOCUS_PENDING_GROUP_WALLET)

                            onButtonClicked: {
                                GlobalData.listFocusing = _FOCUS_PENDING_GROUP_WALLET
                                SharedWallet.currentIndex = index
                                var obj = {
                                    type: "setup-group-wallet",
                                    group_id: model.group_id
                                }
                                QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                            }
                        }
                    }
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
                                                                    (pendingList.visible) && (index === pendingList.currentIndex)
                                                                    && (GlobalData.listFocusing === _FOCUS_PENDING_WALLET)
                            onDeny: {
                                GlobalData.listFocusing = _FOCUS_PENDING_WALLET
                                GroupWallet.currentIndex = index
                                var obj = {
                                    type: "deny",
                                    group_id: modelData.groupId
                                }
                                askDeny(obj)
                            }
                            onAccept: {
                                GlobalData.listFocusing = _FOCUS_PENDING_WALLET
                                GroupWallet.currentIndex = index
                                var obj = {
                                    type: "accept",
                                    group_id: modelData.groupId
                                }
                                QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                            }
                            onDashboard: {
                                GlobalData.listFocusing = _FOCUS_PENDING_WALLET
                                GroupWallet.currentIndex = index
                                var obj = {
                                    type: "dashboard",
                                    group_id: modelData.groupId
                                }
                                QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                            }
                            onButtonClicked: {
                                GlobalData.listFocusing = _FOCUS_PENDING_WALLET
                                GroupWallet.currentIndex = index
                                var obj = {
                                    type: "dashboard",
                                    group_id: modelData.groupId
                                }
                                QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
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
                    QManagerActiveWallets {
                        id: walletList
                        width: parent.width - 32
                        anchors.horizontalCenter: parent.horizontalCenter
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
                walletM: 0
                walletN: 0
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
    WalletListViewModel {
        id: vm
        Component.onCompleted: {
            vm.attachContext(vmContext)
        }
    }
}
