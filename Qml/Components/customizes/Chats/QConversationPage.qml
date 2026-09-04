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
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Chats"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Row {
    id: conversationPageRoot
    spacing: 0
    property bool  createRoom: false
    property alias modelCoversation: listView.model
    property bool chatInfoOpen: false
    property bool creationPending: false
    property int selectedRecipientCount: 0
    property string pendingCreatedRoomId: ""
    readonly property bool attachmentSupported: ClientController.attachmentEnable
    readonly property bool canShowChatInfo: !createRoom && RoomWalletData.currentRoom !== null
    readonly property real chatInfoWidth: chatInfoOpen && canShowChatInfo ? 266 : 0

    onCanShowChatInfoChanged: {
        if (!canShowChatInfo) {
            chatInfoOpen = false
        }
    }

    signal createRoomDone()
    signal triggerEditGroupName()
    signal triggerAddMembers()
    signal triggerLeaveGroup()
    signal triggerRemoveMember(string memberId, string memberName)
    signal requestCancelWallet()
    function completeRoomCreationIfSelected() {
        var rooms = ClientController.rooms
        if (!conversationPageRoot.createRoom
                || conversationPageRoot.pendingCreatedRoomId === ""
                || !rooms
                || !rooms.currentRoom
                || rooms.currentRoom.roomid !== conversationPageRoot.pendingCreatedRoomId
                || rooms.getIndex(conversationPageRoot.pendingCreatedRoomId) < 0) {
            return
        }

        conversationPageRoot.creationPending = false
        conversationPageRoot.pendingCreatedRoomId = ""
        roomSelectionTimeout.stop()
        conversationPageRoot.createRoomDone()
    }
    Timer {
        id: roomSelectionTimeout
        interval: 30000
        repeat: false
        onTriggered: {
            if (!conversationPageRoot.createRoom
                    || conversationPageRoot.pendingCreatedRoomId === "") {
                return
            }
            conversationPageRoot.creationPending = false
            AppModel.showToast(
                        -1,
                        qsTr("The room was created but is still syncing. Select it from the room list when it appears."),
                        EWARNING.EXCEPTION_MSG)
        }
    }
    Connections {
        target: ClientController.rooms
        function onRoomCreationSucceeded(roomId) {
            if (conversationPageRoot.createRoom && conversationPageRoot.creationPending) {
                conversationPageRoot.pendingCreatedRoomId = roomId
                roomSelectionTimeout.restart()
                conversationPageRoot.completeRoomCreationIfSelected()
            }
        }
        function onRoomCreationFailed(error) {
            if (conversationPageRoot.createRoom) {
                roomSelectionTimeout.stop()
                conversationPageRoot.pendingCreatedRoomId = ""
                conversationPageRoot.creationPending = false
            }
        }
        function onCurrentRoomChanged() {
            conversationPageRoot.completeRoomCreationIfSelected()
        }
    }
    Column {
        id: panelColumn
        width: parent.width - conversationInfo.width
        height: parent.height
        Item {
            id: conversationheader
            width: parent.width
            height: 80
            Rectangle {
                id: bgheader
                width: parent.width - 4
                height: parent.height - 4
                anchors.centerIn: parent
                color: "#FFFFFF"
                visible: false
            }
            DropShadow {
                anchors.fill: bgheader
                verticalOffset: 2
                cached: true
                radius: 8
                samples: 16
                color: Qt.rgba(0, 0, 0, 0.15)
                source: bgheader
                Loader {
                    id: headerLoader
                    anchors.fill: parent
                    sourceComponent: createRoom ? userlist : roomtitle
                }
                Component {
                    id: userlist
                    Item {
                        id: userlistItem
                        property var requestlist: []
                        property var requestlistChatId: []
                        anchors.fill: parent
                        QText {
                            id: totext
                            color: "#000000"
                            font.family: "Lato"
                            font.weight: Font.Bold
                            font.pixelSize: 20
                            text: "To:"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 12
                        }
                        Flickable {
                            anchors {
                                fill: parent
                                rightMargin: 6
                                topMargin: 6 + ((flowuser.implicitHeight > 36) ? 0 : 14)
                                leftMargin: 50
                                bottomMargin: 6
                            }
                            clip: true
                            contentHeight: flowuser.implicitHeight
                            interactive: contentHeight > height
                            flickableDirection: Flickable.VerticalFlick
                            contentY : contentHeight > height ? contentHeight - height : 0
                            ScrollBar.vertical: QScrollBar { }
                            Flow {
                                id: flowuser
                                spacing: 6
                                width: parent.width
                                anchors.verticalCenter: parent.verticalCenter
                                Repeater {
                                    id: userrepeat
                                    model: 0
                                    Rectangle {
                                        id: background
                                        width: emailelement.width + 16
                                        height: 36
                                        radius: 36
                                        color: "#F5F5F5"
                                        Row {
                                            id: emailelement
                                            width: emailtext.width + 60
                                            height: 24
                                            spacing: 6
                                            anchors.verticalCenter: parent.verticalCenter
                                            QAvatar {
                                                width: 36
                                                height: 36
                                                avatarUrl: ""
                                                username: userlistItem.requestlist[index]
                                                anchors.verticalCenter: parent.verticalCenter
                                                displayStatus: false
                                            }
                                            QText {
                                                id: emailtext
                                                text: userlistItem.requestlist[index]
                                                height: 24
                                                anchors.verticalCenter: parent.verticalCenter
                                            }
                                            QIcon {
                                                iconSize: 24
                                                source: "qrc:/Images/Images/close_24px.png"
                                                anchors.verticalCenter: parent.verticalCenter
                                                MouseArea {
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        suggestItems.visible = false
                                                        userlistItem.requestlist.splice(index, 1)
                                                        userlistItem.requestlistChatId.splice(index, 1)
                                                        userrepeat.model = userlistItem.requestlist.length
                                                        conversationPageRoot.selectedRecipientCount = userrepeat.model
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                QTextField {
                                    id: input
                                    width: Math.min(flowuser.width,
                                                    Math.max(320, flowuser.width * 0.5))
                                    height: 36
                                    verticalAlignment: Text.AlignVCenter
                                    font.family: "Lato"
                                    color: "#031F2B"
                                    font.pixelSize: 20
                                    wrapMode: Text.NoWrap
                                    clip: true
                                    placeholderText: "Name or email"
                                    background: Rectangle {color: "transparent"}
                                    visible: focus ? true : userlistItem.requestlist.length > 0 ? false : true
                                    onTextChanged: {
                                        suggestItems.friends = ClientController.contactsByStringList()
                                        suggestItems.visible = (text !== "")
                                        suggestItems.searchUser(text)
                                    }
                                    Keys.onReturnPressed: {
                                        if (userlistItem.requestlistChatId.length > 0)
                                            userlistItem.processCreateRoom("")
                                    }
                                    Keys.onEnterPressed: {
                                        if (userlistItem.requestlistChatId.length > 0)
                                            userlistItem.processCreateRoom("")
                                    }
                                    function finishInputUser(index){
                                        if(index >= 0) {
                                            userlistItem.requestlist[userlistItem.requestlist.length] = suggestModel.get(index).name
                                            userlistItem.requestlistChatId[userlistItem.requestlistChatId.length] = suggestModel.get(index).chat_id
                                            suggestModel.setProperty(index, "selected", true)
                                            userrepeat.model = userlistItem.requestlist.length
                                            conversationPageRoot.selectedRecipientCount = userrepeat.model
                                        }
                                        input.text = ""
                                    }
                                }
                            }
                        }
                        Rectangle {
                            id: suggestItems
                            property var friends
                            width: parent.width - 50
                            height: suggestItemsBg.height
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.top: parent.bottom
                            anchors.topMargin: 12
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
                                                displayStatus: false
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
                                                source: "qrc:/Images/Images/check_circle_24px copy.png"
                                            }
                                        }
                                        MouseArea {
                                            id: suggestMouse
                                            anchors.fill: parent
                                            hoverEnabled: true
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: {input.finishInputUser(index)}
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
                                        var data = {'id': id, 'name': name, 'email': mail, 'chat_id': chat_id, 'selected': userlistItem.requestlist.includes(name)};
                                        suggestModel.append(data)
                                    }
                                }
                            }
                        }
                        Component.onCompleted: { suggestItems.friends = ClientController.contactsByStringList() }
                        function processCreateRoom(firstMessage){
                            if (userlistItem.requestlistChatId.length === 0
                                    || conversationPageRoot.creationPending
                                    || conversationPageRoot.pendingCreatedRoomId !== ""
                                    || (ClientController.rooms
                                        && ClientController.rooms.roomCreationInProgress)) {
                                return
                            }
                            conversationPageRoot.creationPending = true
                            if(userlistItem.requestlistChatId.length == 1){
                                ClientController.createRoomDirectChat(userlistItem.requestlistChatId[0], userlistItem.requestlist[0], firstMessage)
                            }
                            else{
                                ClientController.createRoomChat(userlistItem.requestlistChatId, userlistItem.requestlist, firstMessage)
                            }
                        }
                    }
                }
                Component {
                    id: roomtitle
                    Item {
                        anchors {
                            fill: parent
                            leftMargin: 22
                            rightMargin: 36
                            topMargin: 22
                            bottomMargin: 22
                            verticalCenter: parent.verticalCenter
                        }
                        Row {
                            id: avartarArray
                            spacing: -8
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            Repeater {
                                model: (RoomWalletData.currentRoom !== null) ? RoomWalletData.currentRoom.talkersName : 0
                                QAvatar {
                                    id: avatarheader
                                    width: 36
                                    height: 36
                                    username:  RoomWalletData.currentRoom.talkersName[index] + RoomWalletData.currentRoom.users.change
                                    avatarUrl: RoomWalletData.currentRoom.talkersAvatar[index] + RoomWalletData.currentRoom.users.change
                                    displayStatus: false
                                }
                            }
                        }
                        QText {
                            elide: Text.ElideRight
                            color: "#000000"
                            font.family: "Lato"
                            font.weight: Font.Bold
                            font.pixelSize: 20
                            text: RoomWalletData.currentRoom ? RoomWalletData.currentRoom.roomName : STR.STR_QML_502
                            anchors {
                                left: avartarArray.right
                                leftMargin: 12
                                right: chatinfoMouse.left
                                rightMargin: 12
                                verticalCenter: parent.verticalCenter
                            }
                        }
                        MouseArea {
                            id: chatinfoMouse
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            // currentRoom is the single source of truth for both
                            // visibility and interaction; rooms.count may be transiently
                            // zero while the model is being reset or hydrated.
                            enabled: conversationPageRoot.canShowChatInfo
                            visible: conversationPageRoot.canShowChatInfo
                            onEnabledChanged: {
                                if(chatinfoMouse.enabled === false){
                                    conversationPageRoot.chatInfoOpen = false
                                }
                            }

                            width: ico.width
                            height: ico.height
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            QIcon {
                                iconSize: 24
                                id: ico
                                scale: chatinfoMouse.containsMouse ? 1.1 : 1
                                transformOrigin: Item.Center
                                source: "qrc:/Images/Images/tooltip.png"
                                layer.enabled: !conversationPageRoot.canShowChatInfo
                                layer.effect: Desaturate {
                                    anchors.fill: ico
                                    source: ico
                                    desaturation: 1.0
                                }
                            }
                            onClicked:  { conversationPageRoot.chatInfoOpen = true }
                        }
                    }
                }
            }
        }
        spacing: 0
        QListView {
            id: listView
            property bool instantiated: false
            property bool followLatest: true
            readonly property bool noNeedMoreContent: RoomWalletData.currentRoom === null ? true : RoomWalletData.currentRoom.allHisLoaded
            readonly property int indicatorY: (contentY-originY) * (height/contentHeight)
            readonly property int indicatorMaxY: height-scrollContact.size*height
            readonly property int minDelegateHeight: 20
            readonly property bool showSrollBottom: (listView.indicatorY < (listView.indicatorMaxY - minDelegateHeight)) && listView.count > 0
            width: parent.width
            height: parent.height - conversationheader.height - conversationfooter.height
            clip: true
            focus: true
            enabled: RoomWalletData.currentRoom !== null && !createRoom
            delegate: MouseArea{
                width: listView.width
                height: cons.height == 0 ? 0 : (cons.height + 8)
                QConversationDelegate {
                    id:cons
                    anchors.verticalCenter: parent.verticalCenter
                    width: listView.width
                    isSentByMe: model.sendByMe
                    avatarLink: model.avatar
                    sendername: model.sender
                    messageContent: model.message
                    messageTime: model.timestamp
                    isStateEvent: model.isStateEvent
                    timeSection: model.timesection
                    messageType: model.messageType
                    init_event_id: model.init_event_id
                    txnId: model.txnId
                    evtId: model.evtId
                    progressObject: model.progressInfo
                    file_path: model.file_path
                    onRequestCancelWallet: conversationPageRoot.requestCancelWallet()
                }
                onWheel: {
                    wheel.accepted = false
                    if(wheel.angleDelta.y !== 0){
                        if(scrollContact)
                        scrollContact.wheel(wheel.angleDelta.y > 0)
                    }
                }
            }
            onCountChanged: {
                if(instantiated && followLatest) {
                    Qt.callLater(function() {
                        listView.positionViewAtEnd()
                        listView.markVisibleMessagesRead()
                    })
                }
            }
            onContentYChanged: {
                ensurePreviousContent()
                if (moving || dragging || flicking)
                    followLatest = !showSrollBottom
            }
            Component.onCompleted: {
                positionViewAtEnd()
                instantiated = true;
            }
            function ensurePreviousContent() {
                if (noNeedMoreContent) return
                if(contentY < originY){ if(RoomWalletData.currentRoom) { RoomWalletData.currentRoom.getMoreContents() } }
            }
            function positionViewAtEnd() {
                // Scroll to the last item in the list
                var lastIndex = listView.count - 1
                if (lastIndex >= 0) {
                    listView.positionViewAtIndex(lastIndex, ListView.End)
                }
                followLatest = true
            }
            function markVisibleMessagesRead() {
                var room = RoomWalletData.currentRoom
                if (!room || count <= 0)
                    return
                if (!showSrollBottom) {
                    room.markAllMessagesAsRead()
                    return
                }
                var y = contentY + height - 1
                var visibleIndex = -1
                for (var offset = 0; offset < height && visibleIndex < 0; offset += 8)
                    visibleIndex = indexAt(width / 2, y - offset)
                if (visibleIndex >= 0 && model) {
                    for (var row = visibleIndex; row >= 0; --row) {
                        var eventId = model.eventIdAt(row)
                        if (eventId !== "") {
                            room.markMessagesAsRead(eventId)
                            break
                        }
                    }
                }
            }
            section
            {
                property: "timesection"
                criteria: ViewSection.FullString
                delegate: Item {
                    width: listView.width
                    height: section.length > 0 ? 36 : 0
                    visible: section.length > 0
                    QText {
                        text: section
                        font.pixelSize: 12
                        font.family: "Lato"
                        anchors.centerIn: parent
                        color: "#595959"
                    }
                }
            }
            ScrollBar.vertical: QScrollBar {
                id: scrollContact;
                function wheel(up)
                {
                    if(up){
                        //                        decrease();
                    }else{
                        //                        increase();
                        listView.markVisibleMessagesRead()
                    }
                }
            }

            onFlickEnded: {
                listView.followLatest = !listView.showSrollBottom
                listView.markVisibleMessagesRead()
            }

            Item {
                id: typingItem
                width: parent.width
                height: 20
                anchors.bottom: parent.bottom
                visible: RoomWalletData.currentRoom !== null && RoomWalletData.currentRoom.typingNames !== ""
                QText {
                    text: RoomWalletData.currentRoom !== null ? RoomWalletData.currentRoom.typingNames : ""
                    font.pixelSize: 12
                    font.family: "Lato"
                    anchors.centerIn: parent
                    color: "#595959"
                }
            }
            MouseArea {
                id: btnScrollTopMouse
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                width: btnScrollTop.width
                height: btnScrollTop.height
                visible: RoomWalletData.currentRoom !== null && RoomWalletData.currentRoom.unreadCount > 0
                enabled: visible
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 16
                }

                Rectangle{
                    id: btnScrollTop
                    width: 112
                    height: 36
                    color: "#031F2B"
                    radius: 24
                    QText {
                        id: _unread
                        color: "#FFFFFF"
                        font.family: "Lato"
                        font.pixelSize: 16
                        text: (RoomWalletData.currentRoom !== null ? RoomWalletData.currentRoom.unreadCount : 0)  + " Unread"
                        anchors{
                            verticalCenter: parent.verticalCenter
                            left: parent.left
                            leftMargin: 12
                        }
                    }
                    QIcon {
                        iconSize: 24
                        anchors{
                            verticalCenter: parent.verticalCenter
                            right: parent.right
                            rightMargin: 2
                        }
                        scale: btnScrollTopMouse.pressed ? 1 : btnScrollTopMouse.containsMouse ? 1.1 : 1
                        transformOrigin: Item.Center
                        source: "qrc:/Images/Images/close_24px.png"
                    }
                }

                onClicked:  {
                    listView.positionViewAtIndex(listView.count - 1, ListView.End)
                    listView.followLatest = true
                    listView.markVisibleMessagesRead()
                }
            }

            MouseArea {
                id: btnScrollBottomMouse
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                width: btnScrollBottom.width
                height: btnScrollBottom.height
                visible: RoomWalletData.currentRoom !== null && listView.showSrollBottom
                enabled: visible
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 16
                QIcon {
                    iconSize: 36
                    id: btnScrollBottom
                    scale: btnScrollBottomMouse.pressed ? 1 : btnScrollBottomMouse.containsMouse ? 1.1 : 1
                    transformOrigin: Item.Center
                    source: "qrc:/Images/Images/downend.png"
                }
                onClicked:  {
                    listView.positionViewAtEnd()
                    listView.markVisibleMessagesRead()
                }
            }
        }
        Item {
            id: conversationfooter
            width: parent.width
            height: 80
            readonly property bool hasMessageContent: messageField.text.length > 0
                                                       || messageField.preeditText.length > 0
            readonly property bool hasAttachment: !conversationPageRoot.createRoom
                                                   && attachmentSupported
                                                   && attachedFile.fileLocalPath !== ""
            readonly property bool canCreateRoom: conversationPageRoot.createRoom
                                                  && conversationPageRoot.selectedRecipientCount > 0
                                                  && !conversationPageRoot.creationPending
                                                  && conversationPageRoot.pendingCreatedRoomId === ""
                                                  && !(ClientController.rooms
                                                       && ClientController.rooms.roomCreationInProgress)
            readonly property bool canSend: conversationPageRoot.createRoom
                                             ? canCreateRoom
                                             : RoomWalletData.currentRoom !== null
                                               && (hasMessageContent || hasAttachment)
            Item {
                anchors.fill: parent
                anchors.margins: 15
                Row {
                    anchors.fill: parent
                    spacing: 12
                    Item {
                        width: parent.width - emojiPicker.width - 12
                               - (selectFileBtn.visible ? (selectFileBtn.width+12) : 0)
                               - (collabWalletBtn.visible ? (collabWalletBtn.width+12) : 0)
                               - sendMessageBtn.width - 12
                        height: parent.height
                        QTextField {
                            id: messageField
                            anchors.fill: parent
                            placeholderText: "Type your message..."
                            inputMethodHints: Qt.ImhNone
                            Keys.onReturnPressed: function(event) { messageField.handleSendKey(event) }
                            Keys.onEnterPressed: function(event) { messageField.handleSendKey(event) }
                            clip: true
                            color: "#031F2B"
                            font.pixelSize: 16
                            selectByMouse: true
                            onTextEdited: {
                                emojiPicker.replaceCompletedEmoticon()
                                if (!conversationPageRoot.createRoom && RoomWalletData.currentRoom)
                                    RoomWalletData.currentRoom.setTyping(text.length > 0)
                            }
                            background: Rectangle {
                                anchors.fill: parent
                                radius: 8
                                border.color: attachedFile.containsDrag ? "red" : "#DEDEDE"
                                color: "#FFFFFF"
                            }
                            onActiveFocusChanged: {
                                if (conversationPageRoot.createRoom || !RoomWalletData.currentRoom)
                                    return
                                if (activeFocus)
                                    listView.markVisibleMessagesRead()
                                else
                                    RoomWalletData.currentRoom.setTyping(false)
                            }
                            DropArea {
                                id: attachedFile
                                property string fileLocalPath: ""
                                property int    file_mimType: filePlaceHolder._FILE_OTHER
                                enabled: attachmentSupported
                                         && !conversationPageRoot.createRoom
                                         && RoomWalletData.currentRoom !== null
                                anchors.fill: parent
                                onDropped: {
                                    attachedFile.fileLocalPath = ""
                                    if(drop.urls.length > 1){ AppModel.showToast(-696, STR.STR_QML_696, EWARNING.EXCEPTION_MSG); }
                                    else{
                                        var fileDropped = drop.urls[0]
                                        attachedFile.validateFileExtension(fileDropped)
                                        attachedFile.fileLocalPath = fileDropped
                                    }
                                }
                                function validateFileExtension(filePath) {
                                    const filters_image = ['png', 'jpg', 'jpeg'];
                                    const filters_video = ['mp4'];
                                    const filters_other = ['mp4'];
                                    var extension = filePath.split('.').pop();
                                    if(filters_image.includes(extension.toLowerCase())) { attachedFile.file_mimType = filePlaceHolder._FILE_IMAGE}
                                    else if(filters_video.includes(extension.toLowerCase())) { attachedFile.file_mimType = filePlaceHolder._FILE_VIDEO}
                                    else{ attachedFile.file_mimType = filePlaceHolder._FILE_OTHER }
                                }
                            }
                            function handleSendKey(event) {
                                if (inputMethodComposing) {
                                    event.accepted = false
                                    return
                                }
                                event.accepted = true
                                conversationfooter.sendMessage()
                            }
                        }
                        Rectangle {
                            width: 60
                            height: 60
                            radius: 4
                            anchors.right: messageField.right
                            anchors.rightMargin: messageField.leftPadding
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.verticalCenterOffset: -height/2
                            visible: attachmentSupported && (attachedFile.fileLocalPath !== "")
                            z: 200
                            QAttachment {
                                id: filePlaceHolder
                                width: 60
                                height: 60
                                sourceSize.width: filePlaceHolder.width
                                sourceSize.height: filePlaceHolder.height
                                fillMode: Image.PreserveAspectCrop
                                file_mimeType: attachedFile.file_mimType
                                file_path: attachedFile.fileLocalPath
                                layer.enabled: true
                                layer.effect: OpacityMask {
                                    maskSource: Rectangle {
                                        width: 60
                                        height: 60
                                        radius: 4
                                    }
                                }
                                Rectangle {
                                    anchors.fill: parent
                                    radius: 4
                                    color: "transparent"
                                    border.color: "#000000"
                                }
                                opacity: (attachMouse.containsMouse || removeAttachMouse.containsMouse) ? 0.6 : 1
                            }
                            MouseArea {
                                id: attachMouse
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                            }
                            QIcon {
                                iconSize: 32
                                sourceSize.width: width
                                sourceSize.height: height
                                source: "qrc:/Images/Images/remove-button.png"
                                anchors.left: parent.right
                                anchors.leftMargin: -16
                                anchors.bottom: parent.top
                                anchors.bottomMargin: -16
                                visible: attachMouse.containsMouse || removeAttachMouse.containsMouse
                                MouseArea {
                                    id: removeAttachMouse
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked:{
                                        attachedFile.fileLocalPath = ""
                                    }
                                }
                            }
                        }
                    }
                    QEmojiPicker {
                        id: emojiPicker
                        targetInput: messageField
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    MouseArea {
                        id: selectFileBtn
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        width: childrenRect.width
                        height: childrenRect.height
                        visible: (!conversationPageRoot.createRoom
                                  && attachmentSupported
                                  && RoomWalletData.currentRoom
                                  && (RoomWalletData.currentRoom.roomType === NUNCHUCKTYPE.SUPPORT_ROOM))
                        enabled: visible
                        anchors.verticalCenter: parent.verticalCenter
                        QIcon {
                            iconSize: 24
                            scale: selectFileBtn.pressed ? 1: selectFileBtn.containsMouse ? 1.1 : 1
                            transformOrigin: Item.Center
                            source: "qrc:/Images/Images/attach_file.svg"
                        }
                        onClicked:{
                            console.log(selectFileBtn.mouseX, selectFileBtn.mouseY, " | " , selectFileBtn.x, selectFileBtn.y)
                            fileContextMenu.x = selectFileBtn.mouseX - fileContextMenu.menuWidth
                            fileContextMenu.y = selectFileBtn.mouseY - fileContextMenu.implicitHeight
                            fileContextMenu.open()
                        }
                        QContextMenu {
                            id: fileContextMenu
                            menuWidth: 250
                            labels: [
                                STR.STR_QML_745,
                                STR.STR_QML_746
                            ]
                            icons: [
                                "qrc:/Images/Images/description.png",
                                "qrc:/Images/Images/import_031F2B.png"
                            ]
                            enables: [
                                true,
                                true
                            ]
                            onItemClicked: {
                                switch(index){
                                case 0: // Send Photos or videos
                                    openfileDialog.nameFilters = ["Image files (*.png *.jpg *.jpeg)", "Video files (*.mp4)"]
                                    openfileDialog.open()
                                    break;
                                case 1: // Send any file
                                    openfileDialog.nameFilters = ["All files (*.*)"]
                                    openfileDialog.open()
                                    break;
                                default:
                                    break;
                                }
                            }
                        }
                    }
                    Loader {
                        id: collabWalletBtn
                        width: childrenRect.width
                        height: childrenRect.height
                        sourceComponent: RoomWalletData.roomWalletCreated ? btnCreateTransaction : (AppSetting.enableColab ? btnCreateSharedWallet : null)
                        anchors.verticalCenter: parent.verticalCenter
                        visible: !conversationPageRoot.createRoom
                                 && (RoomWalletData.roomWalletCreated
                                     ? (RoomWalletData.currentRoom
                                        && (RoomWalletData.currentRoom.roomType !== NUNCHUCKTYPE.SUPPORT_ROOM))
                                     : AppSetting.enableColab)
                    }
                    MouseArea {
                        id: sendMessageBtn
                        width: 40
                        height: 40
                        anchors.verticalCenter: parent.verticalCenter
                        enabled: conversationfooter.canSend
                        hoverEnabled: true
                        cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                        ToolTip.visible: enabled && containsMouse
                        ToolTip.delay: 500
                        ToolTip.text: STR.STR_QML_002
                        QIcon {
                            anchors.centerIn: parent
                            iconSize: 24
                            scale: sendMessageBtn.pressed
                                   ? 0.9
                                   : (sendMessageBtn.containsMouse && sendMessageBtn.enabled ? 1.1 : 1)
                            source: sendMessageBtn.enabled
                                    ? "qrc:/Images/Images/SendMessage-dark.png"
                                    : "qrc:/Images/Images/SendMessage.png"
                        }
                        onClicked: {
                            var creatingRoom = conversationPageRoot.createRoom
                            messageField.forceActiveFocus()
                            Qt.inputMethod.commit()
                            Qt.callLater(function() {
                                conversationfooter.sendMessage()
                                if (!creatingRoom)
                                    messageField.forceActiveFocus()
                            })
                        }
                    }
                }
            }
            Connections{
                target: ClientController.rooms
                function onCurrentRoomChanged() {
                    attachedFile.fileLocalPath = ""
                    emojiPicker.closePicker()
                    messageField.clear()
                    listView.followLatest = true
                    if (!conversationPageRoot.canShowChatInfo) {
                        conversationPageRoot.chatInfoOpen = false
                    }
                }
            }
            function sendMessage(){
                emojiPicker.closePicker()
                var outgoingText = emojiPicker.convertEmoticons(messageField.text)
                if(createRoom){
                    if (!conversationfooter.canCreateRoom)
                        return
                    headerLoader.item.processCreateRoom(outgoingText)
                    return
                }

                var room = RoomWalletData.currentRoom
                if (!room || (outgoingText === "" && !conversationfooter.hasAttachment))
                    return

                if(conversationfooter.hasAttachment){
                    room.sendFile(outgoingText, attachedFile.fileLocalPath)
                    attachedFile.fileLocalPath = ""
                }
                else{
                    room.sendMessage(outgoingText)
                }
                messageField.clear()
                room.setTyping(false)
            }
        }
    }
    QConversationInfo{
        id: conversationInfo
        width: conversationPageRoot.chatInfoWidth
        height: parent.height
        visible: width > 0
        onCloseRequested: conversationPageRoot.chatInfoOpen = false
        onRequestRemoveMember: function(memberId, memberName) {
            conversationPageRoot.triggerRemoveMember(memberId, memberName)
        }
    }
    Component {
        id: btnCreateSharedWallet
        MouseArea {
            id: createSharedBottom
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            width: icoSwl.width
            height: icoSwl.height
            QIcon {
                iconSize: 24
                id: icoSwl
                scale: createSharedBottom.pressed ? 1: createSharedBottom.containsMouse ? 1.1 : 1
                transformOrigin: Item.Center
                source: "qrc:/Images/Images/collab-wallet-dark.svg"
            }
            enabled: RoomWalletData.currentRoom
            onClicked:{
                if(RoomWalletData.roomWalletInitialized){
                    QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WALLET_CONFIGURE, RoomWalletData.currentRoom.roomid)
                }
                else{
                    QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_CREATE_SHARED_WALLET)
                }
            }
        }
    }
    Component {
        id: btnCreateTransaction
        MouseArea {
            id: createTXBottom
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            width: icoSwl.width
            height: icoSwl.height
            QIcon {
                iconSize: 24
                id: icoSwl
                scale: createTXBottom.pressed ? 1: createTXBottom.containsMouse ? 1.1 : 1
                transformOrigin: Item.Center
                source: "qrc:/Images/Images/bitcoin-dark.svg"
            }
            enabled: RoomWalletData.currentRoom
            onClicked:{
                QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WL_SEND_REQUEST)
            }
        }
    }
    FileDialog {
        id: openfileDialog
        fileMode: FileDialog.OpenFile
        nameFilters: ["Image files (*.png *.jpg *.jpeg)", "Video files (*.mp4)"]
        onAccepted: {
            if(attachmentSupported && (openfileDialog.file !== "")){
                RoomWalletData.currentRoom.sendFile("", openfileDialog.file)
                attachedFile.fileLocalPath = ""
            }
        }
    }
}
