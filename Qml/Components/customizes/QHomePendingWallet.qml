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
import QtQuick.Controls.Styles 1.3
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import QtQuick.Layouts 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Wallets"
import "../../Components/customizes/Members"
import "../../Components/RightPannel/Wallet/Dashboard"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    id: pendingDashboard
    anchors.fill: parent
    color: "#F5F5F5"
    enabled: !isLocked
    property var dashboardInfo: GroupWallet.dashboardInfo
    property bool   accepted:       dashboardInfo.accepted
    property string myRole:         dashboardInfo.myRole
    property string groupStatus:    dashboardInfo.groupStatus
    property bool   isLocked:       dashboardInfo.isLocked
    property bool   groupId:        dashboardInfo.groupId
    property bool   isPremierUser:  ClientController.user.isByzantineUserPremier
    property bool   isIronHandUser:  ClientController.user.isIronHandUser
    property bool   isHoneyBadgerUser:  ClientController.user.isHoneyBadgerUser
    property bool   isPremierGroup: dashboardInfo.isPremierGroup
    property bool hasPro: AppModel.walletInfo.isPro
    property bool hasViewInheritancePlan: (AppModel.walletInfo.inheritancePlanInfo.isActived && hasPro && (myRole === "KEYHOLDER" ||  myRole === "MASTER" || myRole === "ADMIN")) || myRole === ""
    property bool hasPlatformKeyCoSign: (hasPro && (myRole === "KEYHOLDER" ||  myRole === "MASTER" || myRole === "ADMIN")) || isIronHandUser || isHoneyBadgerUser
    property bool hasWalletLockdown: (myRole === "MASTER" || myRole === "ADMIN") || isIronHandUser || isHoneyBadgerUser
    Loader {
        anchors.fill: parent
        sourceComponent: accepted ? acceptedGroup : notAcceptedGroup
    }
    Loader {
        anchors.fill: parent
        sourceComponent: isLocked && (groupStatus !== "PENDING_WALLET")? locked : null
    }
    Component {
        id: locked
        QHomeLockdownInProgress {
            anchors.fill: parent
        }
    }
    Component {
        id: notAcceptedGroup
        QHomeInitialStep1 {
            anchors.fill: parent
        }
    }
    Component {
        id: acceptedGroup
        Column {
            anchors.fill: parent
            anchors.margins: 24
            spacing: 24
            property int unitSelected: AppSetting.unit
            property bool enableFixedPrecision: AppSetting.enableFixedPrecision
            Item {
                width: parent.width
                height: 48
                QText {
                    font.family: "Lato"
                    font.pixelSize: 28
                    font.weight: Font.Bold
                    text: dashboardInfo.hasWallet ? AppModel.walletInfo.walletName : STR.STR_QML_944
                    anchors.verticalCenter: parent.verticalCenter
                }
                Row {
                    anchors.right: parent.right
                    layoutDirection: Qt.RightToLeft
                    spacing: 12
                    visible: (myRole !== "KEYHOLDER_LIMITED")
                    QIconButton {
                        width: 48
                        height: 48
                        bgColor: "#F5F5F5"
                        icon: "qrc:/Images/Images/more-horizontal-dark.svg"
                        onClicked: {
                            if("PENDING_WALLET" === pendingDashboard.groupStatus){
                                pendingOptionMenu.popup()
                            }
                            else{
                                activeOptionMenu.popup()
                            }
                        }
                    }
                    QIconButton {
                        width: 48
                        height: 48
                        bgColor: "#F5F5F5"
                        icon: "qrc:/Images/Images/OnlineMode/wallet-dark.png"
                        onClicked: {
                            // FIXME GOTO WALLET INFO
                            dashboardInfo.isShowDashBoard = false
                        }
                    }
                    QIconButton {
                        width: 48
                        height: 48
                        bgColor: "#F5F5F5"
                        icon: "qrc:/Images/Images/message-dark.svg"
                        onClicked: {
                            if(dashboardInfo){
                                dashboardInfo.requestByzantineChat();
                            }
                        }
                    }
                }
                QContextMenu {
                    id: activeOptionMenu
                    menuWidth: 300
                    labels: [
                        STR.STR_QML_875, //Claim an inheritance
                        STR.STR_QML_738, //Platform key co-signing policies
                        STR.STR_QML_697, //Emergency lockdown
                        STR.STR_QML_969, //View recurring payments
                        STR.STR_QML_970  //Manage group chat history
                    ]
                    visibles: [
                        !isPremierUser && !isPremierGroup && hasViewInheritancePlan,
                        hasPlatformKeyCoSign,
                        hasWalletLockdown,
                        (myRole === "MASTER") || (myRole === "ADMIN") || (myRole === "KEYHOLDER"),
                        (myRole === "MASTER") || (myRole === "ADMIN")
                    ]
                    enables: [true, true, true, true, true]
                    functions: [
                        function(){ //View Inheritance plan
                            QMLHandle.sendEvent(EVT.EVT_GOTO_SERVICE_SETTING_TAB, 4)
                        },
                        function(){ //Platform key co-signing policies
                            QMLHandle.sendEvent(EVT.EVT_GOTO_SERVICE_SETTING_TAB, 7)
                        },
                        function(){ //Emergency lockdown
                            QMLHandle.sendEvent(EVT.EVT_GOTO_SERVICE_SETTING_TAB, 1)
                        },
                        function(){//View recurring payments
                            QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_REQUEST)
                        },
                        function(){//Manage group chat history
                            console.warn("4")
                        }
                    ]
                    colors: [ "#031F2B", "#031F2B", "#031F2B", "#031F2B", "#031F2B" ]
                    onItemClicked: {
                        functions[index]()
                    }
                }
                QContextMenu {
                    id: pendingOptionMenu
                    menuWidth: 300
                    colors: [ "#031F2B", "#CF4018" ]
                    labels: [
                        STR.STR_QML_970, //Manage group chat history
                        STR.STR_QML_1049 //Cancel pending wallet
                    ]
                    visibles: [
                        true,
                        (myRole === "MASTER")
                    ]
                    enables: [ true, true ]
                    functions: [
                        function(){
                            // Chat history
                        },
                        function(){
                            // Cancel pending
                            var _cancel_pending = {
                                type: "reset",
                                group_id: groupId
                            }
                            _confirm.data = _cancel_pending
                            _confirm.contentText = STR.STR_QML_1156
                            _confirm.open()
                        }
                    ]
                    onItemClicked: {
                        functions[index]()
                    }
                }
            }
            Item {
                width: parent.width
                height: Math.min(224, 24 + 12 + 12 + 88*_alert.count)
                visible: _alert.count > 0
                Column {
                    anchors.fill: parent
                    spacing: 12
                    Item {
                        width: parent.width
                        height: 24
                        Row {
                            anchors.fill: parent
                            spacing: 8
                            QIcon {
                                iconSize: 24
                                source: "qrc:/Images/Images/error-dark.svg"
                            }
                            QLato {
                                width: 50
                                height: 24
                                anchors.verticalCenter: parent.verticalCenter
                                text:STR.STR_QML_934
                                font.weight: Font.Bold
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                            }
                        }
                    }
                    Flickable {
                        width: parent.width
                        height: ((_alert.count > 2 ? 1 : 0)*12) + 88 * (Math.min(_alert.count, 2))
                        contentWidth: parent.width
                        contentHeight: alertContain.childrenRect.height
                        flickableDirection: Flickable.VerticalFlick
                        ScrollBar.vertical: ScrollBar { active: true }
                        clip: true
                        Column {
                            id: alertContain
                            anchors.fill: parent
                            spacing: 12
                            Repeater {
                                id: _alert
                                model: dashboardInfo.alerts
                                QDashboardAlert {
                                    width: parent.width
                                    onClickView: {
                                        GroupWallet.markRead(modelData.id)
                                        if (modelData.type === AlertType.REQUEST_INHERITANCE_PLANNING_APPROVED) {
                                            _info1.open()
                                            _info1.contentText = STR.STR_QML_1048
                                        }
                                    }

                                    onClickDismiss: {
                                        GroupWallet.dismiss(modelData.id)
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Item {
                width: parent.width
                height: 104
                visible: (_healthy.count > 0)
                Column {
                    anchors.fill: parent
                    spacing: 12
                    Item {
                        width: parent.width
                        height: 24
                        Row {
                            anchors.fill: parent
                            spacing: 8
                            QIcon {
                                iconSize: 24
                                source: "qrc:/Images/Images/health-check-dark.svg"
                            }
                            QLato {
                                width: 123
                                height: 24
                                anchors.verticalCenter: parent.verticalCenter
                                text:STR.STR_QML_948
                                font.weight: Font.Bold
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                            }
                        }
                    }
                    Rectangle {
                        width: parent.width
                        height: 68
                        color: "#FFFFFF"
                        radius: 8
                        Row {
                            anchors {
                                verticalCenter: parent.verticalCenter
                                left: parent.left
                                leftMargin: 16
                            }
                            spacing: 4
                            Repeater {
                                id: _healthy
                                model: dashboardInfo.health.healthStatuses
                                QCircleIcon {
                                    bgSize: 36
                                    icon.iconSize: 18
                                    icon.typeStr: modelData.keyinfo.type
                                    icon.tag: modelData.keyinfo.tag
                                    anchors.verticalCenter: parent.verticalCenter
                                    color: GlobalData.getHealthStatusColor(modelData.lastState)
                                    visible: modelData.keyinfo.type !== "SERVER"
                                }
                            }
                        }
                        MouseArea {
                            id: qrMouse
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                QMLHandle.sendEvent(EVT.EVT_KEY_HEALTH_CHECK_STATUS_REQUEST)
                            }
                        }
                    }
                }
            }
            Rectangle {
                width: parent.width
                height: 1
                color: "#EAEAEA"
                visible:  (myRole !== "KEYHOLDER_LIMITED")
            }
            Column {
                width: parent.width
                height: childrenRect.height
                spacing: 12
                visible:  (myRole !== "KEYHOLDER_LIMITED")
                Item {
                    width: parent.width
                    height: 24
                    visible: dashboardInfo.members.count > 0
                    Row {
                        anchors.fill: parent
                        spacing: 8
                        QIcon {
                            iconSize: 24
                            source: "qrc:/Images/Images/account-member-dark.svg"
                        }
                        QLato {
                            width: 123
                            height: 24
                            anchors.verticalCenter: parent.verticalCenter
                            text:STR.STR_QML_472
                            font.weight: Font.Bold
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                    }
                }
                QListView {
                    id: _members
                    width: parent.width
                    height: 350
                    model: dashboardInfo.members
                    clip: true
                    ScrollBar.vertical: ScrollBar { active: true }
                    spacing: 16
                    delegate: QMemberDelegate {
                        width: ListView.view.width - 24
                        member: modelData
                    }
                }
            }
        }
    }
}
