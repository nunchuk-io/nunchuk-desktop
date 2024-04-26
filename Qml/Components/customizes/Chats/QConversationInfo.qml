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
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../customizes"
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    id: conversationInfo
    width: 0
    height: parent.height
    color: "#031F2B"
    Behavior on width {NumberAnimation{duration: 200}}
    visible: width > 0
    QLato {
        color: "#FFFFFF"
        font.family: "Lato"
        font.weight: Font.Bold
        font.pixelSize: 16
        text: STR.STR_QML_501
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.topMargin: 26
    }
    MouseArea {
        id: closeinfoMouse
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        width: icoClose.width
        height: icoClose.height
        anchors {
            right: parent.right
            rightMargin: 33
            top: parent.top
            topMargin: 29
        }
        QIcon {
            iconSize: 24
            id: icoClose
            scale: closeinfoMouse.containsMouse ? 1.1 : 1
            transformOrigin: Item.Center
            source: "qrc:/Images/Images/OnlineMode/close_24px_white.png"
        }
        onClicked:  { conversationInfo.width = 0 }
    }
    QAvatar {
        id: avatarInfo
        width: 64
        height: 64
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 70
        groupSize:28
        userCount: RoomWalletData.currentRoom ? RoomWalletData.currentRoom.userCount : 0
        avatarUrl: RoomWalletData.currentRoom ? RoomWalletData.currentRoom.roomAvatar : ""
        username: RoomWalletData.currentRoom ? RoomWalletData.currentRoom.roomName : STR.STR_QML_502
        displayStatus: userCount > 2
    }
    QLato {
        id: roomnameInfo
        width: 234
        lineHeight: 28
        lineHeightMode: Text.FixedHeight
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        color: "#FFFFFF"
        font.family: "Lato"
        font.weight: Font.Bold
        font.pixelSize: 16
        elide: Text.ElideRight
        text: RoomWalletData.currentRoom ? RoomWalletData.currentRoom.roomName : STR.STR_QML_502
        anchors.horizontalCenter: avatarInfo.horizontalCenter
        anchors.top: avatarInfo.bottom
        anchors.topMargin: 12
    }
    QLato {
        id: memberCntInfo
        color: "#FFFFFF"
        font.family: "Lato"
        font.pixelSize: 12
        text: (RoomWalletData.currentRoom ? RoomWalletData.currentRoom.userCount : 0) + STR.STR_QML_472
        anchors.horizontalCenter: roomnameInfo.horizontalCenter
        anchors.top: roomnameInfo.bottom
        anchors.topMargin: 2
    }
    Row {
        id: optionRow
        height: 88
        anchors.top: memberCntInfo.bottom
        anchors.topMargin: 24
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 0
        Item {
            width: 117
            height: parent.height
            visible: RoomWalletData.roomWalletCreated ? true : !RoomWalletData.isIgnoredCollabWallet
            Rectangle {
                width: 48
                height: 48
                radius: 48
                color: "#595959"
                anchors.horizontalCenter: parent.horizontalCenter
                QIcon {
                    iconSize: 24
                    source: RoomWalletData.roomWalletCreated ? "qrc:/Images/Images/OnlineMode/monetization_on-24px_2.png" :
                                                               "qrc:/Images/Images/OnlineMode/Joint wallet_FFFFFF.png"
                    anchors.centerIn: parent
                    scale: createsharedWlMouse.pressed ? 1 : createsharedWlMouse.containsMouse ? 1.1 : 1
                    transformOrigin: Item.Center
                }
                MouseArea {
                    id: createsharedWlMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    enabled: RoomWalletData.currentRoom
                    onClicked: {
                        if(RoomWalletData.roomWalletCreated){
                            QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WL_SEND_REQUEST)
                        }
                        else{
                            QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_CREATE_SHARED_WALLET)
                        }
                    }
                }
            }
            QLato {
                width: 109
                height: 32
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                color: "#FFFFFF"
                font.family: "Lato"
                font.pixelSize: 12
                text: RoomWalletData.roomWalletCreated ? STR.STR_QML_570 : STR.STR_QML_357
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Item {
            width: 117
            height: parent.height
            Rectangle {
                width: 48
                height: 48
                radius: 48
                color: "#595959"
                anchors.horizontalCenter: parent.horizontalCenter
                QIcon {
                    iconSize: 24
                    source: "qrc:/Images/Images/OnlineMode/more_horiz_24px.png"
                    anchors.centerIn: parent
                    scale: moreOptionsMouse.pressed ? 1 : moreOptionsMouse.containsMouse ? 1.1 : 1
                    transformOrigin: Item.Center
                }
                MouseArea {
                    id: moreOptionsMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: optionMenu.popup()
                }
            }
            QLato {
                width: 109
                height: 32
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                color: "#FFFFFF"
                font.family: "Lato"
                font.pixelSize: 12
                text: STR.STR_QML_444
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }
            Menu {
                id: optionMenu
                background: Rectangle {
                    implicitWidth: 180
                    implicitHeight: addMembersMenu.visible ? 3*48 : 2*48
                    radius: 8
                    color: "#FFFFFF"
                }

                MenuItem {
                    id: editNameMenu
                    height: 48
                    text: STR.STR_QML_381
                    icon.source: "qrc:/Images/Images/OnlineMode/Inline Edit Button.png"
                    onTriggered: {triggerEditGroupName()}
                    enabled: (RoomWalletData.currentRoom && RoomWalletData.currentRoom.userCount > 2)
                    background: Rectangle {
                        implicitWidth: 180
                        implicitHeight: 48
                        radius: 8
                        color: editNameMenu.hovered ? "#F5F5F5": "#FFFFFF"
                    }
                }
                MenuItem {
                    id: addMembersMenu
                    height: visible ? 48 : 0
                    text: STR.STR_QML_503
                    icon.source: "qrc:/Images/Images/OnlineMode/person_add-24px.png"
                    onTriggered: {triggerAddMembers()}
                    visible: RoomWalletData.currentRoom && !RoomWalletData.currentRoom.roomWallet
                    enabled: true
                    background: Rectangle {
                        implicitWidth: 180
                        implicitHeight: 48
                        radius: 8
                        color: addMembersMenu.hovered ? "#F5F5F5": "#FFFFFF"
                    }
                }
                MenuItem {
                    id: leavRoomMenu
                    text: STR.STR_QML_504
                    height: 48
                    icon.source: "qrc:/Images/Images/OnlineMode/cancel_red_24dp.png"
                    onTriggered: {triggerLeaveGroup()}
                    enabled: true
                    background: Rectangle {
                        implicitWidth: 180
                        implicitHeight: 48
                        radius: 8
                        color: leavRoomMenu.hovered ? "#F5F5F5": "#FFFFFF"
                    }
                }
            }
        }
    }
    Flickable {
        width: parent.width
        height: 580
        anchors.top: optionRow.bottom
        anchors.topMargin: 24
        clip: true
        contentHeight: infoColumn.implicitHeight
        interactive: contentHeight > height
        flickableDirection: Flickable.VerticalFlick
        ScrollBar.vertical: ScrollBar { active: true }
        Column {
            id: infoColumn
            width: parent.width
            spacing: 24
            Item {
                width: 234
                height: 80
                anchors.horizontalCenter: parent.horizontalCenter
                visible: RoomWalletData.roomWalletInitialized
                Rectangle {
                    anchors.fill: parent
                    radius: 8
                    Loader {
                        anchors.fill: parent
                        sourceComponent: RoomWalletData.roomWalletCreated ? walletCompleted : RoomWalletData.roomWalletReady ? walletDraft : null
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WALLET_CONFIGURE, RoomWalletData.currentRoom.roomid)
                        }
                    }
                }
            }
            Rectangle { width: parent.width ; height: 1; color: "#595959" ; visible: pendingTxs.count > 0}
            Item {
                width: 234
                height: 24
                anchors.horizontalCenter: parent.horizontalCenter
                visible: pendingTxs.count > 0
                Row {
                    height: 24
                    spacing: 8
                    QLato {
                        height: 24
                        color: "#FFFFFF"
                        font.family: "Lato"
                        font.weight: Font.Bold
                        font.pixelSize: 16
                        text: STR.STR_QML_505
                        verticalAlignment: Text.AlignBottom
                    }
                    QLato {
                        height: 24
                        color: "#FFFFFF"
                        font.family: "Lato"
                        font.pixelSize: 12
                        text: "(" + pendingTxs.count + ")"
                        verticalAlignment: Text.AlignBottom
                    }
                }
                MouseArea {
                    id: collapseTx
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    width: txCollapse.width
                    height: txCollapse.height
                    anchors.right: parent.right
                    property bool isCollapsed: false
                    QIcon {
                        iconSize: 24
                        id: txCollapse
                        scale: closeinfoMouse.containsMouse ? 1.1 : 1
                        transformOrigin: Item.Center
                        rotation: collapseTx.isCollapsed ? 0 : 180
                        source: "qrc:/Images/Images/OnlineMode/expand_less_24px_FFFFFF.png"
                    }
                    onClicked:  { collapseTx.isCollapsed = !collapseTx.isCollapsed }
                }
            }
            QListView {
                id: pendingTxs
                width: 234
                readonly property int displayCount: Math.min(3, count)
                height: visible ? (80*displayCount+(spacing*(displayCount-1))) : 0
                visible: !collapseTx.isCollapsed && pendingTxs.count > 0
                anchors.horizontalCenter: parent.horizontalCenter
                model: RoomWalletData.currentRoom ? RoomWalletData.currentRoom.pendingTxs : 0
                spacing: 16
                clip: true
                interactive: count > 3
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: Item {
                    id: iteminit
                    width: 234
                    height: 80
                    anchors.horizontalCenter: parent.horizontalCenter
                    readonly property QtObject txObject: model.room_tx_transaction
                    property int    tx_status   : txObject ? txObject.status : -1
                    property string tx_id       : txObject ? txObject.txid : ""
                    property string to_addr     : txObject ? txObject.destinationList.reciever : ""
                    property string tx_amount   : txObject ? txObject.total + RoomWalletData.unitValue : ""
                    property int    tx_m        : txObject ? txObject.m : 0
                    property int    tx_signeds  : txObject ? txObject.numberSigned : 0
                    property int    tx_pending_signatures: Math.max(0, iteminit.tx_m - iteminit.tx_signeds)

                    Rectangle {
                        anchors.fill: parent
                        radius: 8
                        Row {
                            anchors.fill: parent
                            anchors.margins: 8
                            spacing: 8
                            QImage {
                                width: 18
                                height: 18
                                source: "qrc:/Images/Images/OnlineMode/PendingTransaction.png"
                            }                            
                            Column {
                                spacing: 8
                                Rectangle {
                                    height: 16
                                    width: currentState.width + 20
                                    radius: 20
                                    color: {
                                        if(iteminit.tx_status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return "#FFD7D9" }
                                        else if(iteminit.tx_status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return "#FDEBD2" }
                                        else if(iteminit.tx_status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return "#E8DAFF" }
                                        else if(iteminit.tx_status === NUNCHUCKTYPE.CONFIRMED){ return "#D0E2FF" }
                                        else if(iteminit.tx_status === NUNCHUCKTYPE.NETWORK_REJECTED){ return "#CF4018" }
                                        else{ return "#EAEAEA" }
                                    }
                                    QLato {
                                        id: currentState
                                        font.family: "Lato"
                                        font.pixelSize: 10
                                        text: {
                                            if(iteminit.tx_status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return STR.STR_QML_283 }
                                            else if(iteminit.tx_status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return STR.STR_QML_284 }
                                            else if(iteminit.tx_status === NUNCHUCKTYPE.NETWORK_REJECTED){ return STR.STR_QML_285 }
                                            else if(iteminit.tx_status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return STR.STR_QML_286 }
                                            else if(iteminit.tx_status === NUNCHUCKTYPE.CONFIRMED){ return STR.STR_QML_492 }
                                            else{ return STR.STR_QML_288 }
                                        }
                                        anchors.centerIn: parent
                                        color: "#031F2B"
                                        font.weight: Font.Bold
                                    }
                                }
                                Row {
                                    spacing: 0
                                    height: 16
                                    QLato {
                                        text: STR.STR_QML_491
                                        font.family: "Lato"
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        height: parent.height
                                    }
                                    QLato {
                                        width: 120
                                        height: parent.height
                                        text: iteminit.to_addr
                                        font.family: "Lato"
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        elide: Text.ElideMiddle
                                        font.weight: Font.Bold
                                    }
                                }
                                QLato {
                                    height: 16
                                    font.pixelSize: 12
                                    font.family: "Lato"
                                    font.weight: Font.Bold
                                    color: "#031F2B"
                                    text: iteminit.tx_amount
                                }
                            }
                        }
                    }
                }
            }
            Rectangle { width: parent.width ; height: 1; color: "#595959" }
            Item {
                width: 234
                height: 24
                anchors.horizontalCenter: parent.horizontalCenter
                Row {
                    height: 24
                    spacing: 8
                    QLato {
                        height: 24
                        color: "#FFFFFF"
                        font.family: "Lato"
                        font.weight: Font.Bold
                        font.pixelSize: 16
                        text: STR.STR_QML_472
                        verticalAlignment: Text.AlignBottom
                    }
                    QLato {
                        height: 24
                        color: "#FFFFFF"
                        font.family: "Lato"
                        font.pixelSize: 12
                        text: "(" + roomMembers.count + ")"
                        verticalAlignment: Text.AlignBottom
                    }
                }
                MouseArea {
                    id: collapseMember
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    width: icoCollapse.width
                    height: icoCollapse.height
                    anchors.right: parent.right
                    property bool isCollapsed: false
                    QIcon {
                        iconSize: 24
                        id: icoCollapse
                        scale: closeinfoMouse.containsMouse ? 1.1 : 1
                        transformOrigin: Item.Center
                        rotation: collapseMember.isCollapsed ? 0 : 180
                        source: "qrc:/Images/Images/OnlineMode/expand_less_24px_FFFFFF.png"
                    }
                    onClicked:  { collapseMember.isCollapsed = !collapseMember.isCollapsed }
                }
            }
            QListView {
                id: roomMembers
                width: 234
                height: visible ? 140 : 0
                visible: !collapseMember.isCollapsed
                anchors.horizontalCenter: parent.horizontalCenter
                model: RoomWalletData.currentRoom ? RoomWalletData.currentRoom.users : 0
                spacing: 16
                clip: true
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: QContactDelegate {
                    width: roomMembers.width
                    height: 36
                    contactname: model.name
                    contactAvt: model.avatar
                    onItemRightClicked:  memberMenu.popup()
                    onItemDoubleClicked: {
                        ClientController.createRoomDirectChat(model.id, model.name)
                        conversationContentLoader.changeCurrentRoomComponent()
                    }

                    Menu {
                        id: memberMenu
                        background: Rectangle {
                            implicitWidth: 180
                            implicitHeight: 1*48
                            radius: 8
                            color: "#FFFFFF"
                        }
                        MenuItem {
                            id: removeMemberMenu
                            height: 48
                            text: "Remove"
                            icon.source: "qrc:/Images/Images/OnlineMode/cancel_red_24dp.png"
                            onTriggered: { if(RoomWalletData.currentRoom) RoomWalletData.currentRoom.kickMember(model.id) }
                            background: Rectangle {
                                implicitWidth: 180
                                implicitHeight: 48
                                radius: 8
                                color: removeMemberMenu.hovered ? "#F5F5F5": "#FFFFFF"
                            }
                        }
                    }
                }
            }
            Rectangle { width: parent.width ; height: 1; color: "#595959" }
        }
    }

    Component {
        id: walletDraft
        Item {
            anchors.fill: parent
            visible: RoomWalletData.roomWalletReady
            Rectangle {
                id: bgSharedWallet
                width: parent.width - 4
                height: parent.height - 4
                anchors.centerIn: parent
                color: "#FFFFFF"
                visible: false
                radius: 8
            }
            DropShadow {
                anchors.fill: bgSharedWallet
                verticalOffset: 2
                cached: true
                radius: 8
                samples: 16
                color: Qt.rgba(0, 0, 0, 0.15)
                source: bgSharedWallet
                Row {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 4
                    QImage {
                        width: 18
                        height: 18
                        source: "qrc:/Images/Images/OnlineMode/Joint wallet_031F2B.png"
                    }
                    Column {
                        spacing: 8
                        Rectangle {
                            height: 16
                            width: currentState.width + 20
                            radius: 20
                            color: RoomWalletData.roomWalletCreated ? "#A7F0BA" : RoomWalletData.fullSigners ? "#FDEBD2" : "#FFD7D9"
                            QLato {
                                id: currentState
                                font.family: "Lato"
                                font.pixelSize: 10
                                text: RoomWalletData.roomWalletCreated ? STR.STR_QML_376 : RoomWalletData.fullSigners ? STR.STR_QML_377 : STR.STR_QML_378
                                anchors.centerIn: parent
                                color: "#031F2B"
                                font.weight: Font.Bold
                            }
                        }

                        QLato {
                            font.family: "Lato"
                            font.pixelSize: 12
                            text: RoomWalletData.multisigConfiguration + " " + RoomWalletData.addressType
                            color: "#031F2B"
                        }

                        QLato {
                            font.family: "Lato"
                            font.pixelSize: 12
                            text: RoomWalletData.walletName
                            color: "#031F2B"
                            font.weight: Font.Bold
                        }
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                enabled: parent.visible
                onClicked: {}
            }
        }
    }
    Component {
        id: walletCompleted
        Item {
            anchors.fill: parent
            visible: RoomWalletData.roomWalletReady
            Rectangle {
                id: bgSharedWallet
                width: parent.width - 4
                height: parent.height - 4
                anchors.centerIn: parent
                color: "#FFFFFF"
                visible: false
                radius: 8
            }
            DropShadow {
                anchors.fill: bgSharedWallet
                verticalOffset: 2
                cached: true
                radius: 8
                samples: 16
                color: Qt.rgba(0, 0, 0, 0.15)
                source: bgSharedWallet
                Column {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 4
                    Row {
                        height: 24
                        width: parent.width
                        spacing: 4
                        QLato {
                            width: parent.width - 28
                            font.family: "Lato"
                            font.pixelSize: 16
                            text: RoomWalletData.walletName
                            color: "#031F2B"
                            font.weight: Font.Bold
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QIcon {
                            iconSize: 24
                            source: "qrc:/Images/Images/OnlineMode/Escrow Wallet.png"
                            anchors.verticalCenter: parent.verticalCenter
                            visible: RoomWalletData.isEscrow
                        }
                    }

                    Item {
                        height: 16
                        width: parent.width
                        QLato {
                            font.family: "Lato"
                            font.pixelSize: 12
                            text: RoomWalletData.amount
                            color: "#031F2B"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Rectangle {
                            width: 67
                            height: 16
                            radius: 20
                            color: "#EAEAEA"
                            anchors.right: parent.right
                            Row {
                                spacing: 4
                                anchors.centerIn: parent
                                QImage {
                                    width: 12
                                    height: 12
                                    source: "qrc:/Images/Images/OnlineMode/Joint wallet_031F2B.png"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                QLato {
                                    font.family: "Lato"
                                    font.pixelSize: 10
                                    text: STR.STR_QML_438
                                    color: "#031F2B"
                                    font.weight: Font.Bold
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                    }
                    Item {
                        height: 16
                        width: parent.width
                        QLato {
                            font.family: "Lato"
                            font.pixelSize: 12
                            text: RoomWalletData.amountCurrency
                            color: "#031F2B"
                        }
                        Rectangle {
                            width: 80
                            height: 16
                            radius: 20
                            color: "#EAEAEA"
                            anchors.right: parent.right
                            QLato {
                                font.family: "Lato"
                                font.pixelSize: 10
                                text: qsTr("%1 %2").arg(RoomWalletData.multisigConfiguration).arg(STR.STR_QML_069)
                                color: "#031F2B"
                                font.weight: Font.Bold
                                anchors.centerIn: parent
                            }
                        }
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                enabled: parent.visible
                onClicked: {}
            }
        }
    }
}
