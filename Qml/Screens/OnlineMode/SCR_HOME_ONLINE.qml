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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR
QScreen {
    id: homeonlineroot
    Row {
        anchors.fill: parent
        Item {
            id: pannel_left
            width: 386
            height: parent.height
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
            Row {
                id: leftSide
                QMenuBar {
                    width: 72
                    height: pannel_left.height
                    onLocalModeRequest: {
                        if(0 === AppModel.nunchukMode){
                            QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE)
                        }
                        else{
                            QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINE_HWL)
                        }
                    }
                    onOnlineModeRequest: {
                    }
                    onOpenSettingRequest: {
                        QMLHandle.sendEvent(EVT.EVT_GOTO_APP_SETTINGS_TAB)
                    }
                    onSignoutRequest: {
                        ClientController.requestSignout()
                    }
                    onSigninRequest: {
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                    }
                    onUpdateProfileRequest: {
                        QMLHandle.sendEvent(EVT.EVT_ROOT_UPDATE_PROFILE)
                    }
                    onServiceRequest: {
                        QMLHandle.sendEvent(EVT.EVT_GOTO_SERVICE_SETTING_TAB)
                    }
                }
                Rectangle {
                    width: 314
                    height: pannel_left.height
                    color: "#031F2B"
                    Rectangle {
                        id: newMessageUnread
                        width: 24
                        height: 24
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width/2 + 10
                        anchors.top: parent.top
                        anchors.topMargin: 20
                        color: "#CF4018"
                        radius: 24
                        visible: ClientController.rooms ? ClientController.rooms.totalUnread > 0 : false
                        QText {
                            anchors.centerIn: parent
                            text: ClientController.rooms ? ClientController.rooms.totalUnread : ""
                            color: "#FFFFFF"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                            font.family: "Lato"
                        }
                    }
                    Rectangle {
                        id: newContactInvites
                        width: 24
                        height: 24
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        anchors.top: parent.top
                        anchors.topMargin: 20
                        color: "#CF4018"
                        radius: 24
                        visible: ClientController.contactsReceived ? ClientController.contactsReceived.count > 0 : false
                        QText {
                            anchors.centerIn: parent
                            text: ClientController.contactsReceived ? ClientController.contactsReceived.count : ""
                            color: "#FFFFFF"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                            font.family: "Lato"
                        }
                    }
                    TabView {
                        id: tabselect
                        anchors.fill: parent
                        Tab {
                            title: STR.STR_QML_366
                            Item {
                                id: chanelList
                                Column {
                                    width: parent.width
                                    spacing: 12
                                    Item {
                                        width: parent.width
                                        height: 4
                                    }
                                    Row {
                                        width: parent.width - 26
                                        height: 36
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        QText{
                                            text: STR.STR_QML_367
                                            width: parent.width - iconDirect.width
                                            anchors.verticalCenter: parent.verticalCenter
                                            font.family: "Lato"
                                            font.pixelSize: 16
                                            color: "#FFFFFF"
                                            font.weight: Font.Bold
                                        }
                                        QIconButton{
                                            id: iconDirect
                                            width: 24
                                            height: 24
                                            anchors.verticalCenter: parent.verticalCenter
                                            icon:"qrc:/Images/Images/compose-dark.svg"
                                            enabled: ClientController.isNunchukLoggedIn
                                            onClicked: {
                                                conversationContentLoader.changeNewRoomComponent()
                                            }
                                        }
                                    }
                                    QText {
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        text: STR.STR_QML_625
                                        color: "#FFFFFF"
                                        font.family: "Lato"
                                        font.pixelSize: 16
                                        visible: ClientController.rooms ? ClientController.rooms.count === 0 : false
                                    }
                                    Loader {
                                        id: contactEmptyMsg
                                        width: parent.width
                                        visible: ClientController.contacts.count === 0
                                        height: sourceComponent !== null ? 132 : 0
                                        sourceComponent : ClientController.contacts.count === 0 ? contactEmptyMsgComp : null
                                    }
                                    Loader {
                                        id: roomlist
                                        width: parent.width
                                        height: leftSide.height - 104 - (contactEmptyMsg.sourceComponent !== null ? (contactEmptyMsg.height + 12*3) : 12*2)
                                        sourceComponent: {
                                            if(ClientController.rooms){
                                                return ClientController.rooms.count > 0 ? roomListComp : busyRoomsComp
                                            }
                                        }
                                        Connections {
                                            target: ClientController.rooms
                                            onFinishedDownloadRoom: {
                                                roomlist.sourceComponent = roomListComp
                                            }
                                        }
                                    }
                                }
                                Component {
                                    id: contactEmptyMsgComp
                                    Column {
                                        width: parent.width
                                        spacing: 12
                                        QText {
                                            width: parent.width - 32
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            horizontalAlignment: Text.AlignHCenter
                                            text: STR.STR_QML_368
                                            color: "#FFFFFF"
                                            font.family: "Lato"
                                            font.pixelSize: 16
                                            lineHeightMode: Text.FixedHeight
                                            lineHeight: 28
                                            wrapMode: Text.WordWrap
                                        }
                                        QButtonMedium {
                                            width: 146
                                            height: 40
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            label: STR.STR_QML_369
                                            fontPixelSize: 16
                                            fontWeight: Font.Normal
                                            type: eOUTLINE_DARK
                                            enabled: ClientController.isNunchukLoggedIn
                                            onButtonClicked: {
                                                QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_ADD_CONTACT)
                                            }
                                        }
                                    }
                                }
                                Component {
                                    id: roomListComp
                                    QRoomsPage {
                                        anchors.fill: parent
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        onItemClicked: {
                                            conversationContentLoader.changeCurrentRoomComponent()
                                        }
                                    }
                                }
                                Component {
                                    id: busyRoomsComp
                                    Item {
                                        anchors.fill: parent
                                        BusyIndicator {
                                            anchors.centerIn: parent
                                            running: !ClientController.isMatrixLoggedIn
                                        }
                                    }
                                }
                            }
                        }
                        Tab {
                            title: STR.STR_QML_370
                            Column {
                                width: 305
                                spacing: 16
                                Column {
                                    id: pendingCtItems
                                    width: parent.width
                                    visible: ClientController.contactsReceived.count > 0 || ClientController.contactsSent.count > 0
                                    Row {
                                        spacing: 0
                                        width: parent.width
                                        height: 56
                                        Item {
                                            width: parent.width/2
                                            height: parent.height
                                            QText{
                                                text: STR.STR_QML_371
                                                font.family: "Lato"
                                                font.pixelSize: 16
                                                color: "#FFFFFF"
                                                font.weight: Font.Bold
                                                elide: Text.ElideRight
                                                anchors.verticalCenter: parent.verticalCenter
                                                anchors.left: parent.left
                                                anchors.leftMargin: 16
                                            }
                                        }
                                        Item {
                                            width: parent.width/2
                                            height: parent.height
                                            QText{
                                                text: STR.STR_QML_372
                                                font.family: "Lato"
                                                font.pixelSize: 12
                                                color: "#FFFFFF"
                                                font.weight: Font.Bold
                                                elide: Text.ElideRight
                                                anchors.verticalCenter: parent.verticalCenter
                                                anchors.right: parent.right
                                                anchors.rightMargin: 19
                                                MouseArea{
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        QMLHandle.sendEvent(EVT.EVT_HOME_SHOW_ALL_PENDING_CONTACT)
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    Flickable {
                                        width: 540
                                        height: 100
                                        flickableDirection: Flickable.VerticalFlick
                                        clip: true
                                        contentHeight: columnContactPending.height
                                        interactive: false
                                        Column {
                                            id: columnContactPending
                                            width: parent.width
                                            QListView {
                                                width: parent.width
                                                height: count*100
                                                model: ClientController.contactsReceived
                                                clip: true
                                                interactive: false
                                                visible: count > 0
                                                delegate: QContactDelegatePendingRecv {
                                                    contactname: model.name
                                                    contactmail: model.email
                                                    onItemAcceptClicked: {
                                                        ClientController.acceptFriendRequest(model.id)
                                                    }
                                                    onItemIgnoreClicked: {
                                                        ClientController.ignoreFriendRequest(model.id)
                                                    }
                                                }
                                            }
                                            QListView {
                                                width: parent.width
                                                height: count*100
                                                model: ClientController.contactsSent
                                                clip: true
                                                interactive: false
                                                visible: count > 0
                                                delegate: QContactDelegatePendingSent {
                                                    contactname: model.name
                                                    contactmail: model.email
                                                    onItemIgnoreClicked: {
                                                        ClientController.cancelFriendRequest(model.id)
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                Column {
                                    width: parent.width
                                    Row {
                                        spacing: 0
                                        width: parent.width
                                        height: 56
                                        Item {
                                            width: parent.width/2
                                            height: parent.height
                                            QText{
                                                text: STR.STR_QML_373
                                                font.family: "Lato"
                                                font.pixelSize: 16
                                                color: "#FFFFFF"
                                                font.weight: Font.Bold
                                                elide: Text.ElideRight
                                                anchors.verticalCenter: parent.verticalCenter
                                                anchors.left: parent.left
                                                anchors.leftMargin: 16
                                            }
                                        }
                                        Item {
                                            width: parent.width/2
                                            height: parent.height
                                            QIconButton{
                                                width: 24
                                                height: 24
                                                anchors.verticalCenter: parent.verticalCenter
                                                anchors.right: parent.right
                                                anchors.rightMargin: 19
                                                icon:"qrc:/Images/Images/add-light.svg"
                                                enabled: ClientController.isNunchukLoggedIn
                                                onClicked: {
                                                    QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_ADD_CONTACT)
                                                }
                                            }
                                        }
                                    }
                                    Loader {
                                        width: parent.width
                                        height: leftSide.height - pendingCtItems.height - 48
                                        sourceComponent: ClientController.contacts.count > 0 ? contactListComp : contactEmptyState
                                    }
                                    Component {
                                        id: contactListComp
                                        QListView {
                                            id: contactList
                                            anchors.fill: parent
                                            model: ClientController.contacts
                                            clip: true
                                            currentIndex: ClientController.contacts.currentIndex
                                            ScrollBar.vertical: ScrollBar { id: scrollContact; active: true ; function wheel(up){if(up){decrease()}else{increase()}}}
                                            MouseArea { anchors.fill: parent;z: 10;propagateComposedEvents: true;onWheel: { scrollContact.wheel(wheel.angleDelta.y > 0);}}
                                            delegate: QContactDelegate {
                                                contactname: model.name
                                                contactAvt: model.avatar
                                                isCurrentItem: index === contactList.currentIndex
                                                onItemLeftClicked: {
                                                    ClientController.contacts.currentIndex = index
                                                }
                                                onItemRightClicked: {
                                                    optionMenu.popup()
                                                }
                                                QContextMenu {
                                                    id: optionMenu
                                                    menuWidth: 180
                                                    icons: [
                                                        "qrc:/Images/Images/Delete.svg"
                                                    ]
                                                    labels: [
                                                        STR.STR_QML_374
                                                    ]
                                                    colors: [
                                                        "#CF4018"
                                                    ]
                                                    onItemClicked: {
                                                        if(ClientController.rooms.hasContact(model.id)){
                                                            deleteContactInfo.open()
                                                        }
                                                        else{
                                                            confirmDeleteContact.idContact = model.id
                                                            confirmDeleteContact.open()
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    Component {
                                        id: contactEmptyState
                                        Column {
                                            anchors.fill: parent
                                            spacing: 12
                                            QText {
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                horizontalAlignment: Text.AlignHCenter
                                                text: STR.STR_QML_375
                                                color: "#FFFFFF"
                                                font.family: "Lato"
                                                font.pixelSize: 16
                                            }
                                            QButtonMedium {
                                                width: 146
                                                height: 40
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                label: STR.STR_QML_369
                                                fontPixelSize: 16
                                                fontWeight: Font.Normal
                                                type: eOUTLINE_DARK
                                                enabled: ClientController.isNunchukLoggedIn
                                                onButtonClicked: {
                                                    QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_ADD_CONTACT)
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        style: TabViewStyle {
                            frameOverlap: 1
                            tab: Rectangle {
                                implicitWidth: tabselect.width/2
                                implicitHeight: 60
                                color: "transparent"
                                QText {
                                    id: text
                                    anchors.centerIn: parent
                                    text: styleData.title
                                    color: "#FFFFFF"
                                    font.pixelSize: 16
                                    font.weight: styleData.selected ? Font.Bold: Font.Normal
                                    font.family: "Lato"
                                }

                                Rectangle {
                                    color: styleData.selected ? "#FFFFFF" : "#595959"
                                    implicitWidth: tabselect.width/2
                                    height: styleData.selected ? 2 : 1
                                    anchors.bottom: parent.bottom
                                }
                            }
                            frame: Rectangle { color: "transparent" }
                        }
                    }
                }
            }
        }
        Rectangle {
            id: rightSide
            width: homeonlineroot.width - leftSide.width
            height: homeonlineroot.height
            color: "#FFFFFF"
            Loader {
                id: conversationContentLoader
                property int eCURRENT_MODE:  (ClientController.rooms !== null) && (ClientController.contacts !== null) ?
                                                 (ClientController.rooms.count === 0 && ClientController.contacts.count === 0) ?
                                                     eRIGHT_EMPTY_ROOM : eRIGHT_EXIST_ROOM : eRIGHT_EMPTY_ROOM
                readonly property var sourceItems: [emptyroomstate,newroom,roomChat]
                sourceComponent: {
                    var ret = null
                    if(ClientController.isNunchukLoggedIn){
                        if(tabselect.currentIndex == 0){
                            ret = conversationContentLoader.sourceItems[conversationContentLoader.eCURRENT_MODE]
                        }
                        else{
                            if(ClientController.contacts.count !== 0){ ret = contactInfo }
                            else{ ret = emptyroomstate }
                        }
                    }
                    else{ ret = requireLogin }
                    return ret
                }
                anchors.fill: parent
                function changeNewRoomComponent( ){
                    conversationContentLoader.eCURRENT_MODE = eRIGHT_A_NEW_ROOM
                }
                function changeCurrentRoomComponent( ){
                    if(ClientController.rooms.count === 0 && ClientController.contacts.count === 0){
                        conversationContentLoader.eCURRENT_MODE = eRIGHT_EMPTY_ROOM
                    }
                    else{
                        conversationContentLoader.eCURRENT_MODE = eRIGHT_EXIST_ROOM
                    }
                }
            }
            Connections {
                target: ClientController.rooms
                onFinishedDownloadRoom: {
                    conversationContentLoader.changeCurrentRoomComponent()
                }
            }
            Connections {
                target: ClientController
                onContactsChanged: {
                    conversationContentLoader.changeCurrentRoomComponent()
                }
            }
            Component {
                id: roomChat
                Item {
                    anchors.fill: parent
                    QConversationPage {
                        anchors.fill: parent
                        modelCoversation: (RoomWalletData.currentRoom !== null) ? RoomWalletData.currentRoom.conversation : 0
                        onTriggerEditGroupName: { editRoomnameModal.visible = true}
                        onTriggerAddMembers: {
                            addMoreMemberModel.visible = true
                            suggestItems.userSelected = ""
                            suggestItems.userSelectedId = ""
                            addMemberInput.textInputted = ""
                        }
                        onTriggerLeaveGroup: {ClientController.leaveCurrentRoom()}
                        onRequestCancelWallet: {confirmCancelWallet.open()}
                    }
                    QBoxWalletInfo{
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        anchors.top: parent.top
                        anchors.topMargin: 100
                        visible: RoomWalletData.roomWalletInitialized && !RoomWalletData.roomWalletCreated
                    }
                    QBoxTransactionInfo{
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        anchors.top: parent.top
                        anchors.topMargin: 100
                    }
                }
            }
            Component {
                id: newroom
                QConversationPage {
                    anchors.fill: parent
                    createRoom: true
                    modelCoversation: 0
                    onCreateRoomDone: conversationContentLoader.changeCurrentRoomComponent()
                }
            }
            Component {
                id: emptyroomstate
                QAddWelcome{
                    anchors.fill: parent
                    btnTextLink: STR.STR_QML_369
                    titleSuggest: STR.STR_QML_369
                    titleWelcome: STR.STR_QML_379
                    content: STR.STR_QML_368
                    icon:"qrc:/Images/Images/OnlineMode/addContact.png"
                    enabled: ClientController.isMatrixLoggedIn && ClientController.readySupport && !preventTimer.running
                    onAddButtonClicked: {
                        preventTimer.restart()
                        QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_ADD_CONTACT)
                    }
                    onSupportButtonClicked: {
                        preventTimer.restart()
                        QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ)
                    }
                    Timer {
                        id: preventTimer
                        interval: 5000
                        repeat: false
                    }
                    Component.onCompleted: {if(!ClientController.readySupport) {preventTimer.restart()}}
                }
            }
            Component {
                id: contactInfo
                QContactInfoPage{
                    anchors.fill: parent
                    onSend_a_messageClicked: {
                        tabselect.currentIndex = 0
                        ClientController.createRoomDirectChat(user.chat_id, user.name)
                        conversationContentLoader.changeCurrentRoomComponent()
                    }
                    onSend_removeContact: {
                        if(ClientController.rooms.hasContact(id)){
                            deleteContactInfo.open()
                        }
                        else{
                            confirmDeleteContact.idContact = id
                            confirmDeleteContact.open()
                        }
                    }
                }
            }
            Component {
                id: requireLogin
                QRequireLogin {
                    anchors.fill: parent
                }
            }
        }
    }

    readonly property int eRIGHT_EMPTY_ROOM: 0
    readonly property int eRIGHT_A_NEW_ROOM: 1
    readonly property int eRIGHT_EXIST_ROOM: 2

    Rectangle {
        id: editRoomnameModal
        anchors.fill: parent
        visible: false
        MouseArea {anchors.fill: parent; onClicked: {}}
        color: Qt.rgba(0, 0, 0, 0.7)
        Rectangle {
            id: editRoomnameContent
            width: 600
            height: 324
            color: "#FFFFFF"
            anchors.centerIn: parent
            radius: 8
            QText {
                anchors {
                    left: parent.left
                    leftMargin: 36
                    top: parent.top
                    topMargin: 36
                }
                text: STR.STR_QML_381
                color: "#031F2B"
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 32
            }
            QCloseButton {
                anchors {
                    right: parent.right
                    rightMargin: 41
                    top: parent.top
                    topMargin: 49
                }
                onClicked: { editRoomnameModal.visible = false }
            }
            QTextInputBoxTypeB {
                id: groupnameInput
                label: STR.STR_QML_382
                boxWidth: 528
                boxHeight: 48
                isValid: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 124
                onTextInputtedChanged: {

                }
            }
            QTextButton {
                width: 120
                height: 48
                label.text: STR.STR_QML_245
                label.font.pixelSize: 16
                type: eTypeB
                anchors {
                    left: parent.left
                    leftMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                onButtonClicked: { editRoomnameModal.visible = false }
            }
            QTextButton {
                width: 170
                height: 48
                label.text: STR.STR_QML_383
                label.font.pixelSize: 16
                type: eTypeA
                anchors {
                    right: parent.right
                    rightMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                onButtonClicked: {
                    if(RoomWalletData.currentRoom !== null) {
                        RoomWalletData.currentRoom.roomName = groupnameInput.textInputted
                        editRoomnameModal.visible = false
                        groupnameInput.textInputted = ""
                    }
                }
            }
        }
        DropShadow {
            anchors.fill: editRoomnameContent
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
            source: editRoomnameContent
        }
    }
    Rectangle {
        id: addMoreMemberModel
        anchors.fill: parent
        visible: false
        MouseArea {anchors.fill: parent; onClicked: {}}
        color: Qt.rgba(0, 0, 0, 0.7)

        Rectangle {
            id: addMoreMemberContent
            width: 600
            height: 324
            color: "#FFFFFF"
            anchors.centerIn: parent
            radius: 8
            QText {
                anchors {
                    left: parent.left
                    leftMargin: 36
                    top: parent.top
                    topMargin: 36
                }
                text: STR.STR_QML_384
                color: "#031F2B"
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 32
            }
            QCloseButton {
                anchors {
                    right: parent.right
                    rightMargin: 41
                    top: parent.top
                    topMargin: 49
                }
                onClicked: {
                    addMoreMemberModel.visible = false
                    suggestItems.userSelected = ""
                    suggestItems.userSelectedId = ""
                    suggestItems.visible = false
                }
            }
            QTextInputBoxTypeB {
                id: addMemberInput
                label: STR.STR_QML_385
                boxWidth: 528
                boxHeight: 48
                isValid: true
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 124
                onTextInputtedChanged: {
                    suggestItems.friends = ClientController.contactsByStringList()
                    suggestItems.visible = (textInputted !== "")
                    suggestItems.searchUser(textInputted)
                }
            }
            QTextButton {
                width: 120
                height: 48
                label.text: STR.STR_QML_386
                label.font.pixelSize: 16
                type: eTypeA
                anchors {
                    right: parent.right
                    rightMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                enabled: (addMemberInput.textInputted === suggestItems.userSelected) && (addMemberInput.textInputted !== "")
                onButtonClicked: {
                    if(RoomWalletData.currentRoom) { RoomWalletData.currentRoom.inviteToRoom(suggestItems.userSelectedId)}
                    addMoreMemberModel.visible = false
                    suggestItems.userSelected = ""
                    suggestItems.userSelectedId = ""
                    suggestItems.visible = false
                }
            }
        }

        DropShadow {
            anchors.fill: addMoreMemberContent
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
            source: addMoreMemberContent
        }

        Rectangle {
            id: suggestItems
            property var friends
            property string userSelected: ""
            property string userSelectedId: ""
            width: addMoreMemberContent.width*2/3
            height: suggestItemsBg.height
            anchors.horizontalCenter: addMoreMemberContent.horizontalCenter
            anchors.top: addMoreMemberContent.top
            anchors.topMargin: 190
            visible: false
            MouseArea {anchors.fill: parent; onClicked: {}}
            color: Qt.rgba(255, 255, 255, 0.7)
            radius: 8
            Rectangle {
                id: suggestItemsBg
                width: parent.width
                height: suggestlist.height
                color: "#FFFFFF"
                anchors.centerIn: parent
                radius: 8
                QListView {
                    id: suggestlist
                    width: parent.width
                    spacing: 0
                    height: Math.min(count*48, 10*48)
                    model: suggestModel
                    visible: count > 0
                    clip: true
                    ScrollBar.horizontal: ScrollBar { active: true }
                    delegate: Rectangle {
                        width: suggestlist.width
                        height: 48
                        radius: 8
                        color: suggestMouse.containsMouse ? "#EFEFEF" : "transparent"
                        Row {
                            width: parent.width - 32
                            height: 48
                            anchors.centerIn: parent
                            spacing: 8
                            QAvatar {
                                width: 36
                                height: 36
                                avatarUrl: ""
                                username: model.name
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QText {
                                color: "#000000"
                                font.family: "Lato"
                                font.weight: Font.DemiBold
                                font.pixelSize: 16
                                text: model.name
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QText {
                                color: "#000000"
                                font.family: "Lato"
                                font.pixelSize: 12
                                text: "(" + model.email + ")"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QIcon {
                                iconSize: 24
                                anchors.verticalCenter: parent.verticalCenter
                                visible: model.selected
                                source: "qrc:/Images/Images/OnlineMode/check_circle_24px copy.png"
                            }
                        }
                        MouseArea {
                            id: suggestMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                suggestItems.userSelected = model.name + " (" + model.email + ")"
                                suggestItems.userSelectedId = model.chat_id
                                suggestItems.visible = false
                                addMemberInput.textInputted = suggestItems.userSelected
                            }
                        }
                    }
                }
                ListModel {
                    id: suggestModel
                }
            }
            DropShadow {
                anchors.fill: suggestItemsBg
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
                source: suggestItemsBg
            }
            function searchUser( word ){
                suggestModel.clear()
                if(word === "") return;
                for(var i = 0; i < suggestItems.friends.length; i++){
                    var id = suggestItems.friends[i].split('|')[0]
                    var name = suggestItems.friends[i].split('|')[1]
                    var mail = suggestItems.friends[i].split('|')[2]
                    var chat_id = suggestItems.friends[i].split('|')[3]
                    if(name.toLowerCase().includes(word.toLowerCase()) || mail.toLowerCase().includes(word.toLowerCase())){
                        var data = {'id': id, 'name': name, 'email': mail, 'chat_id': chat_id, 'selected': false};
                        suggestModel.append(data)
                    }
                }
            }
        }
    }
    QConfirmYesNoPopup {
        id: confirmCancelWallet
        onConfirmNo: close()
        onConfirmYes: {
            close()
            if(RoomWalletData.currentRoom) {RoomWalletData.currentRoom.cancelWallet()}
        }
    }
    QPopupInfo{
        id: newDevicePopup
        contentText: STR.STR_QML_590
        onGotItClicked: {
            ClientController.isNewDevice = false
        }
        Component.onCompleted: {
            if(ClientController.isNewDevice){
                newDevicePopup.open()
            }
        }
    }
    /*=========================================Delete contact============================================*/
    QConfirmYesNoPopup {
        id: confirmDeleteContact
        property var idContact
        contentText: STR.STR_QML_609
        onConfirmNo: close()
        onConfirmYes: {
            close()
            ClientController.removeContact(idContact)
        }
    }
    QPopupInfo{
        id: deleteContactInfo
        contentText: STR.STR_QML_610
    }
    /*=========================================Delete contact end=========================================*/
}
