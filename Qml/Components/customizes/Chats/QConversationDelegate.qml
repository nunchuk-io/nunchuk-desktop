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
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
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

Item {
    id: conversationRoot
    height: contentLoader.height
    property string init_event_id: ""
    property string messageTime: ""
    property string messageContent: ""
    property string sendername: ""
    property string avatarLink: ""
    property bool isSentByMe: false
    property bool isStateEvent: false
    property string timeSection: "value"
    property string txnId: ""
    property string evtId: ""
    property var  progressObject: null
    property string file_path: ""
    property int messageType: 0     // EVT.ROOM_EVT

    readonly property var componentType: [
        initConversationComponent,  // EVT.ROOM_EVT.INITIALIZE = 0,
        purestringComponent,        // EVT.ROOM_EVT.PLAIN_TEXT,
        matrixEventComponent,       // EVT.ROOM_EVT.STATE_EVT
        pastWalletComponent,        // EVT.ROOM_EVT.WALLET_PAST,
        initWalletComponent,        // EVT.ROOM_EVT.WALLET_INIT,
        walletSignersComponent,     // EVT.ROOM_EVT.WALLET_JOIN,
        walletSignersComponent,     // EVT.ROOM_EVT.WALLET_LEAVE,
        signersReadyComponent,      // EVT.ROOM_EVT.WALLET_READY,
        walletCreatedComponent,     // EVT.ROOM_EVT.WALLET_CREATE,
        walletSignersComponent,     // EVT.ROOM_EVT.WALLET_CANCEL,
        walletBackupComponent,      // EVT.ROOM_EVT.WALLET_BACKUP,
        pastTransactionComponent,   // EVT.ROOM_EVT.TX_CANCELED,
        initTransactionComponent,   // EVT.ROOM_EVT.TX_INIT,
        transactionEvtComponent,    // EVT.ROOM_EVT.TX_SIGN,
        broadcastTxComponent,       // EVT.ROOM_EVT.TX_BROADCAST,
        signersReadyComponent,      // EVT.ROOM_EVT.TX_READY,
        transactionEvtComponent,    // EVT.ROOM_EVT.TX_CANCEL,
        receiveTxEvtComponent,      // EVT.ROOM_EVT.TX_RECEIVE,
        exceptionComponent,         // EVT.ROOM_EVT.EXCEPTION,
        fileImageComponent,         // EVT.ROOM_EVT.FILE_IMAGE,
        fileVideoComponent,         // EVT.ROOM_EVT.FILE_VIDEO,
        fileOtherComponent,         // EVT.ROOM_EVT.FILE_OTHER,
    ]
    signal requestCancelWallet()
    signal conversationLeftClicked()

    Loader {
        id: contentLoader
        width: parent.width
        sourceComponent: componentType[messageType]
    }

    Component {
        id: initConversationComponent
        Item {
            width: conversationRoot.width
            height: contentInit.height
            Column {
                id: contentInit
                spacing: 8
                anchors.horizontalCenter: parent.horizontalCenter
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: -8
                    Repeater {
                        model: Math.min((RoomWalletData.currentRoom !== null) ? RoomWalletData.currentRoom.userCount : 0, 9)
                        QAvatar {
                            id: avatarheader
                            width: 64
                            height: 64
                            avatarUrl: RoomWalletData.currentRoom.users.get(index).avatar + RoomWalletData.currentRoom.users.change
                            username: RoomWalletData.currentRoom.users.get(index).name + RoomWalletData.currentRoom.users.change
                            displayStatus: false
                        }
                    }
                }
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: conversationRoot.width * 4/5
                    color: "#000000"
                    font.family: "Montserrat"
                    font.weight: Font.Medium
                    font.pixelSize: 32
                    horizontalAlignment: Text.AlignHCenter
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 40
                    wrapMode: Text.WordWrap
                    text: RoomWalletData.currentRoom ? RoomWalletData.currentRoom.roomName : STR.STR_QML_471
                }
                QText {
                    color: "#000000"
                    font.family: "Lato"
                    font.pixelSize: 16
                    text: (RoomWalletData.currentRoom ? RoomWalletData.currentRoom.userCount : 0) + STR.STR_QML_472
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Rectangle {
                    width: 480
                    height: 224
                    radius: 8
                    color: "#F5F5F5"
                    anchors.horizontalCenter: parent.horizontalCenter
                    Row {
                        anchors.centerIn: parent
                        spacing: 12
                        QImage {
                            width: 36
                            height: 36
                            source: "qrc:/Images/Images/OnlineMode/Joint wallet_031F2B.png"
                        }
                        Column {
                            spacing: 12
                            QText {
                                width: 364
                                color: "#031F2B"
                                font.family: "Lato"
                                font.weight: Font.Bold
                                font.pixelSize: 16
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 20
                                text: STR.STR_QML_473
                            }
                            QText {
                                id:_readOurGuide
                                width: 364
                                color: "#031F2B"
                                font.family: "Lato"
                                font.weight: Font.Normal
                                font.pixelSize: 16
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 20
                                wrapMode: Text.WordWrap
                                property string webLink: "https://resources.nunchuk.io/getting-started/"
                                text: STR.STR_QML_474.arg(webLink)
                                onLinkActivated: Qt.openUrlExternally(link)
                                MouseArea {
                                    anchors.fill: parent
                                    cursorShape: _readOurGuide.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                                    acceptedButtons: Qt.NoButton
                                }
                            }
                            Row {
                                spacing: 12
                                anchors.right: parent.right
                                QButtonTextLink {
                                    height: 36
                                    label: STR.STR_QML_475
                                    displayIcon: false
                                    onButtonClicked: {
                                        RoomWalletData.currentRoom.conversation.initConsShow = false
                                    }
                                }
                                QTextButton {
                                    width: label.paintedWidth + 10*2
                                    height: 36
                                    label.text: STR.STR_QML_476
                                    label.font.pixelSize: 12
                                    type: eTypeB
                                    onButtonClicked: {
                                        if(RoomWalletData.roomWalletInitialized){
                                            QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WALLET_CONFIGURE, RoomWalletData.currentRoom.roomid)
                                        }
                                        else{
                                            QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_CREATE_SHARED_WALLET)
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
    Component {
        id: matrixEventComponent
        Item {
            width: conversationRoot.width
            height: eventText.height + 20
            Rectangle {
                width: 460
                height: eventText.height + 20
                radius: 44
                anchors.centerIn: parent
                color: "#F5F5F5"
                QIcon {
                    iconSize: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/OnlineMode/error_outline_24px_n.png"
                }
                QText {
                    id: eventText
                    text: messageContent
                    color: "#031F2B"
                    width: 406
                    anchors.left: parent.left
                    anchors.leftMargin: 42
                    wrapMode: Text.WrapAnywhere
                    font.family: "Lato"
                    textFormat: Text.RichText
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
    Component {
        id: purestringComponent
        Row {
            LayoutMirroring.enabled: isSentByMe
            LayoutMirroring.childrenInherit: true
            spacing: 8
            Item {width: 12; height: 12}
            QAvatar {
                id: avatar
                width: 36
                height: 36
                avatarUrl: avatarLink
                username: sendername
                visible: !isSentByMe
                displayStatus: false
            }
            Column {
                id: contentChat
                spacing: 4
                QText {
                    id: contentChatHeader
                    height: 16
                    text: sendername
                    color: "#595959"
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                    anchors.left: parent.left
                    visible: !isSentByMe
                }
                Row {
                    spacing: 4
                    anchors.left: parent.left
                    Column {
                        spacing: 0
                        Item {
                            id: chatmessageItem
                            width: chatMessageBox.width
                            height: chatMessageBox.height
                            Item {
                                id: chatMessageBox
                                height: messageText.height + 24 + 2
                                width: Math.max(40, messageText.width + 24 + 2)
                                anchors.left: parent.left
                                Rectangle {
                                    id: maskChat
                                    width: parent.width - 2
                                    height: parent.height - 2
                                    anchors.centerIn: parent
                                    radius: 24
                                    Rectangle {
                                        width: parent.radius
                                        height: parent.radius
                                        anchors {
                                            top: parent.top
                                            left: parent.left
                                        }
                                        color: parent.color
                                    }
                                    visible: false
                                }
                                Rectangle {
                                    id: backgroundChat
                                    width: parent.width - 2
                                    height: parent.height - 2
                                    anchors.centerIn: parent
                                    color: isSentByMe ? "#D0E2FF" : "#FFFFFF"
                                    visible: false
                                }
                                OpacityMask {
                                    id: backgroundOpacity
                                    anchors.fill: backgroundChat
                                    source: backgroundChat
                                    anchors.centerIn: parent
                                    maskSource: maskChat
                                }
                            }
                            InnerShadow {
                                anchors.fill: chatMessageBox
                                cached: true
                                horizontalOffset: 0
                                verticalOffset: 0
                                radius: 1
                                samples: 24
                                color: Qt.rgba(0, 0, 0, 0.8)
                                smooth: true
                                source: chatMessageBox
                                visible: !isSentByMe
                            }
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton | Qt.RightButton
                                onClicked: {
                                    if (mouse.button === Qt.RightButton) {
                                        optionMenu.labels = ["Copy message"]
                                        optionMenu.icons = ["qrc:/Images/Images/Copy_031F2B.png"]
                                        optionMenu.functions = [function(){
                                            var txt = ClientController.getPlainText(messageText.text)
                                            ClientController.copyMessage(txt)
                                        }]
                                        optionMenu.popup()
                                    }
                                    else{
//                                        contentChatFooter.visible = true
                                        conversationLeftClicked()
                                    }
                                }
                            }
                            QText {
                                id: messageText
                                text: messageContent
                                color: "#031F2B"
                                width: Math.min(conversationRoot.width*0.70, implicitWidth)
                                anchors.centerIn: parent
                                wrapMode: Text.WrapAnywhere
                                font.family: "Lato"
                                textFormat: Text.RichText
                                lineHeight: 1.5
                                horizontalAlignment: Text.AlignJustify
                                onLinkActivated: Qt.openUrlExternally( link )
                                MouseArea {
                                    anchors.fill: parent
                                    cursorShape: messageText.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                                    acceptedButtons: Qt.NoButton
                                }
                            }
                        }
                    }
                    QText {
                        id: contentChatFooter
                        text: messageTime
                        height: 16
                        color: "#595959"
                        font.pixelSize: 12
                        font.weight: Font.DemiBold
                        anchors.verticalCenter: parent.verticalCenter
                        visible: false
                        onVisibleChanged: {
                            if(visible) countDown.start()
                        }
                        NumberAnimation {
                            id: countDown
                            target: contentChatFooter
                            property: "visible"
                            duration: 5000
                            easing.type: Easing.InOutQuad
                            onStopped: contentChatFooter.visible = false
                        }
                    }
                }
                QText {
                    color: "#595959"
                    font.pixelSize: 14
                    anchors.left: parent.left
                    text: (evtId !== "") ? STR.STR_QML_477 : STR.STR_QML_478
                    visible: isSentByMe ? ((evtId === "") || (index === (RoomWalletData.currentRoom.conversation.count - 1))) : false
                }
            }
        }
    }
    Component {
        id: pastWalletComponent
        Item {
            id: itemPastWallet
            height: contentCreatingWl.height
            LayoutMirroring.enabled: isSentByMe
            LayoutMirroring.childrenInherit: true

            function getWalletName() {
                var obj = JSON.parse(messageContent)
                if('name' in obj){ return obj.name }
                else{ return STR.STR_QML_479 }
            }

            function getWalletInfo() {
                var obj = JSON.parse(messageContent)
                if('m' in obj && 'n' in obj && 'is_escrow' in obj){
                    return STR.STR_QML_480.arg(obj.m).arg(obj.n).arg(obj.is_escrow ? STR.STR_QML_481 : STR.STR_QML_482)
                }
                else{ return STR.STR_QML_479 }
            }

            QAvatar {
                id: avatar
                width: visible ? 36 : 0
                height: 36
                avatarUrl: avatarLink
                username: sendername
                visible: !isSentByMe
                anchors.left: parent.left
                anchors.leftMargin: 20
                displayStatus: false
            }
            QContentCreateWL {
                id: contentCreatingWl
                width: 400
                height: 228
                anchors.left: avatar.right
                anchors.leftMargin: 8
                Row {
                    anchors.fill: parent
                    anchors.margins: 24
                    layoutDirection: isSentByMe ? Qt.RightToLeft : Qt.LeftToRight
                    Item {  width: 1; height: 24 }
                    Column {
                        spacing: 12
                        Rectangle {
                            width: creatingState.width + 20
                            height: creatingState.height + 8
                            radius: 20
                            color: "#FFFFFF"
                            border.color: "#EAEAEA"
                            QText {
                                id: creatingState
                                anchors.centerIn: parent
                                text: STR.STR_QML_483
                                font.family: "Lato"
                                font.pixelSize: 12
                                font.weight: Font.Medium
                                color: "#031F2B"
                            }
                        }
                        QText {
                            id: walletconfig
                            text: itemPastWallet.getWalletInfo()
                            font.family: "Lato"
                            font.pixelSize: 12
                            color: "#031F2B"
                        }
                        QText {
                            id: wlName
                            font.pixelSize: 32
                            font.family: "Montserrat"
                            font.weight: Font.Medium
                            color: "#031F2B"
                            text: itemPastWallet.getWalletName()
                        }
                    }
                }
                QTextButton {
                    width: 360
                    height: 36
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: 24
                        horizontalCenter: parent.horizontalCenter
                    }
                    label.text: STR.STR_QML_484
                    label.font.pixelSize: 12
                    type: eTypeE
                    enabled: false
                    onButtonClicked: {}
                }
            }
        }
    }
    Component {
        id: initWalletComponent
        Item {
            id: initWalletComponentItem
            height: contentCreatingWl.height
            LayoutMirroring.enabled: isSentByMe
            LayoutMirroring.childrenInherit: true
            QAvatar {
                id: avatar
                width: visible ? 36 : 0
                height: 36
                avatarUrl: avatarLink
                username: sendername
                visible: !isSentByMe
                anchors.left: parent.left
                anchors.leftMargin: 20
                displayStatus: false
            }
            QContentCreateWL {
                id: contentCreatingWl
                width: 400
                height: 268
                anchors.left: avatar.right
                anchors.leftMargin: 8
                Row {
                    anchors {
                        top: parent.top
                        topMargin: 24
                    }
                    layoutDirection: isSentByMe ? Qt.RightToLeft : Qt.LeftToRight
                    Item {  width: 24; height: 24 }
                    Column {
                        spacing: 12
                        Rectangle {
                            width: creatingState.width + 20
                            height: creatingState.height + 8
                            radius: 20
                            color: RoomWalletData.roomWalletCreated ? "#A7F0BA" : RoomWalletData.fullSigners ? "#FDEBD2" : "#FFD7D9"
                            QText {
                                id: creatingState
                                anchors.centerIn: parent
                                text: RoomWalletData.roomWalletCreated ? STR.STR_QML_376 : RoomWalletData.fullSigners ? STR.STR_QML_377 : STR.STR_QML_378
                                font.family: "Lato"
                                font.pixelSize: 14
                                color: "#031F2B"
                            }
                        }
                        QText {
                            id: walletconfig
                            text: STR.STR_QML_485.arg(RoomWalletData.multisigConfiguration).arg(RoomWalletData.walletType)
                            font.family: "Lato"
                            font.pixelSize: 16
                            color: "#031F2B"
                        }
                        QText {
                            id: wlName
                            font.pixelSize: 32
                            font.family: "Montserrat"
                            font.weight: Font.Medium
                            color: "#031F2B"
                            text: RoomWalletData.walletName
                        }
                        Row {
                            width: parent.width
                            height: 24
                            spacing: 8
                            layoutDirection: isSentByMe ? Qt.RightToLeft : Qt.LeftToRight
                            QIcon {
                                iconSize: 24
                                anchors.verticalCenter: parent.verticalCenter
                                source: RoomWalletData.fullSigners ? "qrc:/Images/Images/OnlineMode/check_circle_24px_n.png" :
                                                                     "qrc:/Images/Images/OnlineMode/pending_actions-24px 1.png"
                            }
                            QText {
                                text: RoomWalletData.fullSigners ? STR.STR_QML_439 : (RoomWalletData.pendingSigners <= 1 ? STR.STR_QML_440_: STR.STR_QML_440.arg(RoomWalletData.pendingSigners))
                                color: "#031F2B"
                                font.family: "Lato"
                                font.pixelSize: 16
                                font.weight: Font.Medium
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                }
                Loader {
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: 24
                        horizontalCenter: parent.horizontalCenter
                    }
                    sourceComponent: RoomWalletData.roomWalletCreated ? recipient1Button : isSentByMe ? senderButtons : RoomWalletData.fullSigners ? recipient1Button : recipient2Buttons
                }
            }
            Component {
                id: senderButtons
                Row {
                    id: bottombuttons
                    spacing: 12
                    layoutDirection: isSentByMe ? Qt.RightToLeft : Qt.LeftToRight
                    anchors.centerIn: parent
                    QButtonTextLink {
                        width: 170
                        height: 36
                        displayIcon: false
                        label: isSentByMe ? STR.STR_QML_441 : STR.STR_QML_487
                        fontPixelSize: 12
                        onButtonClicked: {
                            conversationRoot.requestCancelWallet()
                        }
                    }
                    QTextButton {
                        width: 170
                        height: 36
                        label.text: RoomWalletData.fullSigners ? isSentByMe ? STR.STR_QML_442 : STR.STR_QML_484 : STR.STR_QML_484
                        label.font.pixelSize: 12
                        type: eTypeE
                        enabled: RoomWalletData.roomWalletReady && init_event_id === RoomWalletData.walletInitId
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WALLET_CONFIGURE)
                        }
                    }
                }
            }
            Component {
                id: recipient1Button
                QTextButton {
                    width: 340
                    height: 36
                    label.text: STR.STR_QML_484
                    label.font.pixelSize: 12
                    type: eTypeE
                    enabled: RoomWalletData.roomWalletReady && init_event_id === RoomWalletData.walletInitId
                    onButtonClicked: {
                        QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WALLET_CONFIGURE)
                    }
                }
            }
            Component {
                id: recipient2Buttons
                Row {
                    id: bottombuttons
                    spacing: 12
                    layoutDirection: isSentByMe ? Qt.RightToLeft : Qt.LeftToRight
                    anchors.centerIn: parent
                    QButtonTextLink {
                        width: 170
                        height: 36
                        displayIcon: false
                        label: STR.STR_QML_487
                        fontPixelSize: 12
                        onButtonClicked: {
                            conversationRoot.requestCancelWallet()
                        }
                    }
                    QTextButton {
                        width: 170
                        height: 36
                        label.text: STR.STR_QML_484
                        label.font.pixelSize: 12
                        type: eTypeE
                        enabled: RoomWalletData.roomWalletReady && init_event_id === RoomWalletData.walletInitId
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WALLET_CONFIGURE)
                        }
                    }
                }
            }
        }
    }
    Component {
        id: walletSignersComponent
        Item {
            width: conversationRoot.width
            height: eventText.height + 20
            Rectangle {
                width: 460
                height: eventText.height + 20
                radius: 44
                anchors.centerIn: parent
                color: "#F5F5F5"
                QIcon {
                    iconSize: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/OnlineMode/error_outline_24px_n.png"
                }

                QText {
                    id: eventText
                    text: messageContent
                    color: "#031F2B"
                    width: 320
                    anchors.left: parent.left
                    anchors.leftMargin: 42
                    wrapMode: Text.WrapAnywhere
                    font.family: "Lato"
                    textFormat: Text.RichText
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    anchors.verticalCenter: parent.verticalCenter
                }

                QText {
                    id: viewDetails
                    text: STR.STR_QML_488
                    color: "#031F2B"
                    anchors.right: parent.right
                    anchors.rightMargin: 12
                    font.weight: Font.Bold
                    font.family: "Lato"
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        enabled: RoomWalletData.roomWalletReady && init_event_id === RoomWalletData.walletInitId
                        onClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WALLET_CONFIGURE)
                        }
                    }
                }
            }
        }
    }
    Component {
        id: signersReadyComponent
        Item {
            width: conversationRoot.width
            height: eventText.height + 20
            Rectangle {
                width: 460
                height: eventText.height + 20
                radius: 44
                anchors.centerIn: parent
                color: "#F5F5F5"
                QIcon {
                    iconSize: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/OnlineMode/error_outline_24px_n.png"
                }

                QText {
                    id: eventText
                    text: messageContent
                    color: "#031F2B"
                    width: 406
                    anchors.left: parent.left
                    anchors.leftMargin: 42
                    wrapMode: Text.WrapAnywhere
                    font.family: "Lato"
                    textFormat: Text.RichText
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
    Component {
        id: walletCreatedComponent
        Item {
            width: conversationRoot.width
            height: eventText.height + 20
            Rectangle {
                width: 460
                height: eventText.height + 20
                radius: 44
                anchors.centerIn: parent
                color: "#A7F0BA"
                QIcon {
                    iconSize: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/OnlineMode/check_circle_outline_24px.png"
                }
                QText {
                    id: eventText
                    text: messageContent
                    color: "#031F2B"
                    width: 320
                    anchors.left: parent.left
                    anchors.leftMargin: 42
                    wrapMode: Text.WrapAnywhere
                    font.family: "Lato"
                    textFormat: Text.RichText
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    anchors.verticalCenter: parent.verticalCenter
                }
                QText {
                    id: viewDetails
                    text: STR.STR_QML_488
                    color: "#031F2B"
                    anchors.right: parent.right
                    anchors.rightMargin: 12
                    font.weight: Font.Bold
                    font.family: "Lato"
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        enabled: RoomWalletData.roomWalletReady && init_event_id === RoomWalletData.walletInitId
                        onClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WALLET_CONFIGURE)
                        }
                    }
                }
            }
        }
    }
    Component {
        id: walletBackupComponent
        Item {
            width: conversationRoot.width
            height: eventText.height + 20
            Rectangle {
                width: 460
                height: eventText.height + 20
                radius: 44
                anchors.centerIn: parent
                color: "#F5F5F5"
                QIcon {
                    iconSize: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/OnlineMode/error_outline_24px_n.png"
                }
                QText {
                    id: eventText
                    text: messageContent
                    color: "#031F2B"
                    width: 330
                    anchors.left: parent.left
                    anchors.leftMargin: 42
                    wrapMode: Text.WrapAnywhere
                    font.family: "Lato"
                    textFormat: Text.RichText
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    anchors.verticalCenter: parent.verticalCenter
                }
                QText {
                    id: viewDetails
                    text: STR.STR_QML_489
                    color: "#031F2B"
                    anchors.right: parent.right
                    anchors.rightMargin: 12
                    font.weight: Font.Bold
                    font.family: "Lato"
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        enabled: RoomWalletData.roomWalletReady && init_event_id === RoomWalletData.walletInitId
                        onClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_BACKUP_SHARED_WALLET)
                        }
                    }
                }
            }
        }
    }
    Component {
        id: pastTransactionComponent
        Item {
            id: itemPastTx
            height: contentCreatingWl.height
            LayoutMirroring.enabled: isSentByMe
            LayoutMirroring.childrenInherit: true
            property QtObject txObject: model.transaction.info
            property string to_addr: txObject ? txObject.destinationList.reciever : STR.STR_QML_490
            property string tx_amount: txObject ? txObject.total + RoomWalletData.unitValue : STR.STR_QML_490

            QAvatar {
                id: avatar
                width: visible ? 36 : 0
                height: 36
                avatarUrl: avatarLink
                username: sendername
                visible: !isSentByMe
                anchors.left: parent.left
                anchors.leftMargin: 20
                displayStatus: false
            }
            QContentCreateWL {
                id: contentCreatingWl
                width: 400
                height: 228
                anchors.left: avatar.right
                anchors.leftMargin: 8
                Row {
                    anchors.fill: parent
                    anchors.margins: 24
                    layoutDirection: isSentByMe ? Qt.RightToLeft : Qt.LeftToRight
                    Item {  width: 1; height: 24 }
                    Column {
                        spacing: 12
                        Rectangle {
                            width: creatingState.width + 20
                            height: creatingState.height + 8
                            radius: 20
                            color: "#FFFFFF"
                            border.color: "#EAEAEA"
                            QText {
                                id: creatingState
                                anchors.centerIn: parent
                                text: STR.STR_QML_483
                                font.family: "Lato"
                                font.pixelSize: 12
                                font.weight: Font.Medium
                                color: "#031F2B"
                            }
                        }
                        Row {
                            spacing: 0
                            layoutDirection: isSentByMe ? Qt.RightToLeft : Qt.LeftToRight
                            QText {
                                id: txTitle
                                text: STR.STR_QML_491
                                font.family: "Lato"
                                font.pixelSize: 14
                                color: "#031F2B"
                            }
                            QText {
                                width: 200
                                text: itemPastTx.to_addr
                                font.family: "Lato"
                                font.pixelSize: 12
                                color: "#031F2B"
                                elide: Text.ElideMiddle
                                font.weight: Font.Bold
                            }
                        }
                        QText {
                            font.pixelSize: 32
                            font.family: "Montserrat"
                            font.weight: Font.Medium
                            color: "#031F2B"
                            text: itemPastTx.tx_amount
                        }
                    }
                }
                QTextButton {
                    width: 360
                    height: 36
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: 24
                        horizontalCenter: parent.horizontalCenter
                    }
                    label.text: STR.STR_QML_484
                    label.font.pixelSize: 12
                    type: eTypeE
                    enabled: false
                    onButtonClicked: {}
                }
            }
        }
    }
    Component {
        id: initTransactionComponent
        Item {
            id: iteminit
            height: contentCreatingWl.height
            LayoutMirroring.enabled: isSentByMe
            LayoutMirroring.childrenInherit: true
            property QtObject txObject: model.transaction ? model.transaction.info : null
            property int tx_status: txObject ? txObject.status : -1
            property string tx_id: txObject ? txObject.txid : ""
            property string to_addr: txObject ? txObject.destinationList.reciever : ""
            property string tx_amount: txObject ? txObject.total + RoomWalletData.unitValue : ""
            property int tx_m: txObject ? txObject.m : 0
            property int tx_signeds:txObject ? txObject.numberSigned : 0
            property int tx_pending_signatures: Math.max(0, iteminit.tx_m - iteminit.tx_signeds)
            property int tx_createByMe: txObject ? txObject.createByMe : true
            QAvatar {
                id: avatar
                width: visible ? 36 : 0
                height: 36
                avatarUrl: avatarLink
                username: sendername
                visible: !isSentByMe
                anchors.left: parent.left
                anchors.leftMargin: 20
                displayStatus: false
            }
            QContentCreateWL {
                id: contentCreatingWl
                width: 400
                height: 268
                anchors.left: avatar.right
                anchors.leftMargin: 8
                Row {
                    anchors {
                        top: parent.top
                        topMargin: 24
                    }
                    layoutDirection: isSentByMe ? Qt.RightToLeft : Qt.LeftToRight
                    Item {  width: 24; height: 24 }
                    Column {
                        spacing: 12
                        Rectangle {
                            width: creatingState.width + 20
                            height: creatingState.height + 8
                            radius: 20
                            color: {
                                if(iteminit.tx_status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return "#FFD7D9" }
                                else if(iteminit.tx_status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return "#FDEBD2" }
                                else if(iteminit.tx_status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return "#E8DAFF" }
                                else if(iteminit.tx_status === NUNCHUCKTYPE.CONFIRMED){ return "#D0E2FF" }
                                else if(iteminit.tx_status === NUNCHUCKTYPE.NETWORK_REJECTED){ return "#CF4018" }
                                else{ return "#FFD7D9" }
                            }
                            QText {
                                id: creatingState
                                anchors.centerIn: parent
                                text: {
                                    if(iteminit.tx_status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return STR.STR_QML_283 }
                                    else if(iteminit.tx_status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return STR.STR_QML_284 }
                                    else if(iteminit.tx_status === NUNCHUCKTYPE.NETWORK_REJECTED){ return STR.STR_QML_285 }
                                    else if(iteminit.tx_status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return STR.STR_QML_286 }
                                    else if(iteminit.tx_status === NUNCHUCKTYPE.CONFIRMED){ return STR.STR_QML_492 }
                                    else{ return STR.STR_QML_082 }
                                }
                                font.family: "Lato"
                                font.pixelSize: 14
                                color: "#031F2B"
                            }
                        }
                        Row {
                            spacing: 0
                            layoutDirection: isSentByMe ? Qt.RightToLeft : Qt.LeftToRight
                            QText {
                                id: txTitle
                                text: STR.STR_QML_491
                                font.family: "Lato"
                                font.pixelSize: 16
                                color: "#031F2B"
                            }
                            QText {
                                width: 200
                                text: iteminit.to_addr
                                font.family: "Lato"
                                font.pixelSize: 16
                                color: "#031F2B"
                                elide: Text.ElideMiddle
                                font.weight: Font.Bold
                            }
                        }
                        QText {
                            id: txAmountText
                            font.pixelSize: 32
                            font.family: "Montserrat"
                            font.weight: Font.Medium
                            color: "#031F2B"
                            text: iteminit.tx_amount
                        }
                        Row {
                            width: parent.width
                            height: 24
                            spacing: 8
                            layoutDirection: isSentByMe ? Qt.RightToLeft : Qt.LeftToRight
                            QIcon {
                                iconSize: 24
                                anchors.verticalCenter: parent.verticalCenter
                                source: {
                                    if(iteminit.tx_status === NUNCHUCKTYPE.CONFIRMED) return "qrc:/Images/Images/OnlineMode/check_circle_24px_n.png"
                                    else return iteminit.tx_pending_signatures === 0 ? "qrc:/Images/Images/OnlineMode/check_circle_24px_n.png" :
                                                                                       "qrc:/Images/Images/OnlineMode/pending_actions-24px 1.png"
                                }
                            }
                            QText {
                                text: {
                                    if(iteminit.tx_status === NUNCHUCKTYPE.CONFIRMED) return STR.STR_QML_493
                                    else return iteminit.tx_pending_signatures === 0 ? STR.STR_QML_493 :  qsTr("%1 %2 %3").arg(STR.STR_QML_469)
                                                                                                                            .arg(iteminit.tx_pending_signatures)
                                                                                                                            .arg((iteminit.tx_pending_signatures > 1 ? STR.STR_QML_494:
                                                                                                                                                                       STR.STR_QML_495))
                                }

                                color: "#031F2B"
                                font.family: "Lato"
                                font.pixelSize: 16
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                }
                Loader {
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: 24
                        horizontalCenter: parent.horizontalCenter
                    }
                    sourceComponent: {
                        if(iteminit.tx_status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return pendingSignaturesBtn }
                        else if(iteminit.tx_status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return pendingBroadcastBtn }
                        else if(iteminit.tx_status === NUNCHUCKTYPE.NETWORK_REJECTED){ return viewdetailsBtn }
                        else if(iteminit.tx_status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return viewdetailsBtn }
                        else if(iteminit.tx_status === NUNCHUCKTYPE.CONFIRMED){ return viewdetailsBtn }
                        else{ return viewdetailsBtn}
                    }
                }
            }
            MouseArea {
                anchors.fill: contentCreatingWl
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                propagateComposedEvents: true
                onClicked: {
                    if (mouse.button === Qt.RightButton) {
                        optionMenu.width = 200
                        optionMenu.labels = [STR.STR_QML_691]
                        optionMenu.icons = ["qrc:/Images/Images/Copy_031F2B.png"]
                        optionMenu.functions = [function(){
                            ClientController.copyMessage(iteminit.to_addr)
                        }]
                        optionMenu.popup()
                    }else{
                        mouse.accepted = false
                    }
                }
            }
            Component {
                id: viewdetailsBtn
                QTextButton {
                    width: 350
                    height: 36
                    label.text: STR.STR_QML_488
                    label.font.pixelSize: 12
                    anchors.centerIn: parent
                    type: eTypeB
                    onButtonClicked: {
                        QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST, model.transaction)
                    }
                }
            }
            Component {
                id: pendingSignaturesBtn
                Row {
                    id: bottombuttons
                    spacing: 12
                    layoutDirection: isSentByMe ? Qt.RightToLeft : Qt.LeftToRight
                    anchors.centerIn: parent
                    QButtonTextLink {
                        width: 170
                        height: 36
                        displayIcon: false
                        label: STR.STR_QML_296
                        fontPixelSize: 12
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_CANCEL_TRANSACTION, model.transaction)
                        }
                    }
                    QTextButton {
                        width: 170
                        height: 36
                        label.text: STR.STR_QML_496
                        label.font.pixelSize: 12
                        type: eTypeE
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST, model.transaction)
                        }
                    }
                }
            }
            Component {
                id: pendingBroadcastBtn
                Row {
                    id: bottombuttons
                    spacing: 12
                    layoutDirection: isSentByMe ? Qt.RightToLeft : Qt.LeftToRight
                    anchors.centerIn: parent
                    QButtonTextLink {
                        width: 170
                        height: 36
                        displayIcon: false
                        label: STR.STR_QML_296
                        fontPixelSize: 12
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_CANCEL_TRANSACTION, model.transaction)
                        }
                    }
                    QTextButton {
                        width: 170
                        height: 36
                        label.text: iteminit.tx_createByMe ? STR.STR_QML_497 : STR.STR_QML_488
                        label.font.pixelSize: 12
                        type: eTypeE
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST, model.transaction)
                        }
                    }
                }
            }
        }
    }
    Component {
        id: transactionEvtComponent
        Item {
            id: itemTxEvent
            width: conversationRoot.width
            height: eventText.height + 20
            Rectangle {
                width: 460
                height: eventText.height + 20
                radius: 44
                anchors.centerIn: parent
                color: "#F5F5F5"
                QIcon {
                    iconSize: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/OnlineMode/error_outline_24px_n.png"
                }
                QText {
                    id: eventText
                    text: model.message
                    color: "#031F2B"
                    width: 320
                    anchors.left: parent.left
                    anchors.leftMargin: 42
                    wrapMode: Text.Wrap
                    font.family: "Lato"
                    textFormat: Text.RichText
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                QText {
                    id: viewDetails
                    text: STR.STR_QML_488
                    color: "#031F2B"
                    anchors.right: parent.right
                    anchors.rightMargin: 12
                    font.weight: Font.Bold
                    font.family: "Lato"
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        enabled: model.transaction
                        onClicked: {
                            if(model.transaction.cancelEventId === ""){
                                QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST, model.transaction)
                            }
                        }
                    }
                }
            }
        }
    }
    Component {
        id: broadcastTxComponent
        Item {
            id: itemTxEvent
            width: conversationRoot.width
            height: eventText.height + 20
            property QtObject txObject: model.transaction ? model.transaction.info : null
            property int tx_status: txObject ? txObject.status : -1
            property string to_addr: txObject ? txObject.destinationList.reciever : ""
            function getElideText(text, pos){
                var num = text.length/4
                if(num > 0){
                    if(pos === 0) {return text.slice(0, 5)}
                    else{ return text.substr(-4)}
                }
                else {return text}
            }

            Rectangle {
                width: 460
                height: eventText.height + 20
                radius: 44
                anchors.centerIn: parent
                color: "#F5F5F5"
                QIcon {
                    iconSize: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/OnlineMode/error_outline_24px_n.png"
                }
                QText {
                    id: eventText
                    text: (itemTxEvent.txObject && itemTxEvent.tx_status === NUNCHUCKTYPE.CONFIRMED) ?
                              STR.STR_QML_498
                              .arg(itemTxEvent.getElideText(itemTxEvent.to_addr, 0))
                              .arg(itemTxEvent.getElideText(itemTxEvent.to_addr, 1))
                            : model.message
                    color: "#031F2B"
                    width: 320
                    anchors.left: parent.left
                    anchors.leftMargin: 42
                    wrapMode: Text.Wrap
                    font.family: "Lato"
                    textFormat: Text.RichText
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                QText {
                    id: viewDetails
                    text: STR.STR_QML_488
                    color: "#031F2B"
                    anchors.right: parent.right
                    anchors.rightMargin: 12
                    font.weight: Font.Bold
                    font.family: "Lato"
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        enabled: model.transaction && (model.transaction.cancelEventId === "")
                        onClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST, model.transaction)
                        }
                    }
                }
            }
        }
    }
    Component {
        id: receiveTxEvtComponent
        Item {
            width: conversationRoot.width
            height: eventText.height + 20
            Rectangle {
                width: 460
                height: eventText.height + 20
                radius: 44
                anchors.centerIn: parent
                color: "#F5F5F5"
                QIcon {
                    iconSize: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/OnlineMode/error_outline_24px_n.png"
                }
                QText {
                    id: eventText
                    text: model.transaction && model.transaction.info ?
                              qsTr("%1 <b>%2 %3</b> in <b>%4</b>")
                              .arg(model.transaction.info.status === NUNCHUCKTYPE.CONFIRMED ? STR.STR_QML_499 : STR.STR_QML_500)
                              .arg(model.transaction.info.subtotal)
                              .arg(RoomWalletData.unitValue)
                              .arg(RoomWalletData.walletName)
                            : model.message
                    color: "#031F2B"
                    width: 320
                    anchors.left: parent.left
                    anchors.leftMargin: 42
                    wrapMode: Text.WrapAnywhere
                    font.family: "Lato"
                    textFormat: Text.RichText
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    anchors.verticalCenter: parent.verticalCenter
                }
                QText {
                    id: viewDetails
                    text: STR.STR_QML_488
                    color: "#031F2B"
                    anchors.right: parent.right
                    anchors.rightMargin: 12
                    font.weight: Font.Bold
                    font.family: "Lato"
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        enabled: model.transaction
                        onClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST, model.transaction)
                        }
                    }
                }
            }
        }
    }
    Component {
        id: exceptionComponent
        Item {
            width: conversationRoot.width
            height: AppSetting.enableDebugMode ? eventText.height + 20 : 0
            visible: AppSetting.enableDebugMode
            Rectangle {
                width: 460
                height: eventText.height + 20
                radius: 44
                anchors.centerIn: parent
                color: "#F5F5F5"
                QIcon {
                    iconSize: 24
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/OnlineMode/error_outline_24px.png"
                }
                QText {
                    id: eventText
                    text: messageContent
                    color: "#CF4018"
                    width: 406
                    anchors.left: parent.left
                    anchors.leftMargin: 42
                    wrapMode: Text.WrapAnywhere
                    font.family: "Lato"
                    textFormat: Text.RichText
                    lineHeight: 16
                    lineHeightMode: Text.FixedHeight
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
    Component {
        id: fileImageComponent
        Row {
            LayoutMirroring.enabled: isSentByMe
            LayoutMirroring.childrenInherit: true
            spacing: 8
            Item {width: 12; height: 12}
            QAvatar {
                id: avatar
                width: 36
                height: 36
                avatarUrl: avatarLink
                username: sendername
                visible: !isSentByMe
                displayStatus: false
            }
            Column {
                id: contentChat
                spacing: 4
                QText {
                    id: contentChatHeader
                    height: 16
                    text: sendername
                    color: "#595959"
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                    anchors.left: parent.left
                    visible: !isSentByMe
                }
                Row {
                    spacing: 4
                    anchors.left: parent.left
                    Column {
                        spacing: 0
                        Item {
                            id: chatmessageItem
                            width: chatMessageBox.width
                            height: chatMessageBox.height
                            Item {
                                id: chatMessageBox
                                height: messageCol.height + 26
                                width: Math.max(40, messageCol.width + 50)
                                anchors.left: parent.left
                                Rectangle {
                                    id: maskChat
                                    width: parent.width - 2
                                    height: parent.height - 2
                                    anchors.centerIn: parent
                                    radius: 24
                                    Rectangle {
                                        width: parent.radius
                                        height: parent.radius
                                        anchors {
                                            top: parent.top
                                            left: parent.left
                                        }
                                        color: parent.color
                                    }
                                    visible: false
                                }
                                Rectangle {
                                    id: backgroundChat
                                    width: parent.width - 2
                                    height: parent.height - 2
                                    anchors.centerIn: parent
                                    color: isSentByMe ? "#D0E2FF" : "#FFFFFF"
                                    visible: false
                                }
                                OpacityMask {
                                    id: backgroundOpacity
                                    anchors.fill: backgroundChat
                                    source: backgroundChat
                                    anchors.centerIn: parent
                                    maskSource: maskChat
                                }
                            }
                            InnerShadow {
                                anchors.fill: chatMessageBox
                                cached: true
                                horizontalOffset: 0
                                verticalOffset: 0
                                radius: 1
                                samples: 24
                                color: Qt.rgba(0, 0, 0, 0.8)
                                smooth: true
                                source: chatMessageBox
                                visible: !isSentByMe
                            }
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton | Qt.RightButton
                                onClicked: {
                                    if (mouse.button === Qt.RightButton) {
                                        optionMenu.labels = ["Copy message"]
                                        optionMenu.icons = ["qrc:/Images/Images/Copy_031F2B.png"]
                                        optionMenu.functions = [function(){
                                            var txt = ClientController.getPlainText(messageText.text)
                                            ClientController.copyMessage(txt)
                                        }]
                                        optionMenu.popup()
                                    }
                                    else{
                                        conversationLeftClicked()
                                    }
                                }
                            }
                            Column {
                                id: messageCol
                                anchors.left: parent.left
                                anchors.leftMargin: 24
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 8
                                QAttachment {
                                    roomEventID: conversationRoot.evtId
                                    progressObject: conversationRoot.progressObject
                                    file_mimeType: _FILE_IMAGE
                                    file_path: conversationRoot.file_path
                                }
                                QText {
                                    text: messageContent
                                    color: "#031F2B"
                                    width: Math.min(conversationRoot.width*0.70, implicitWidth)
                                    wrapMode: Text.WrapAnywhere
                                    font.family: "Lato"
                                    textFormat: Text.RichText
                                    lineHeight: 1.5
                                    horizontalAlignment: Text.AlignJustify
                                    onLinkActivated: Qt.openUrlExternally( link )
                                }
                            }
                        }
                    }
                    QText {
                        id: contentChatFooter
                        text: messageTime
                        height: 16
                        color: "#595959"
                        font.pixelSize: 12
                        font.weight: Font.DemiBold
                        anchors.verticalCenter: parent.verticalCenter
                        visible: false
                        onVisibleChanged: {
                            if(visible) countDown.start()
                        }
                        NumberAnimation {
                            id: countDown
                            target: contentChatFooter
                            property: "visible"
                            duration: 5000
                            easing.type: Easing.InOutQuad
                            onStopped: contentChatFooter.visible = false
                        }
                    }
                }
                QText {
                    color: "#595959"
                    font.pixelSize: 14
                    anchors.left: parent.left
                    text: (evtId !== "") ? STR.STR_QML_477 : STR.STR_QML_478
                    visible: isSentByMe ? ((evtId === "") || (index === (RoomWalletData.currentRoom.conversation.count - 1))) : false
                }
            }
        }
    }
    Component {
        id: fileVideoComponent
        Row {
            LayoutMirroring.enabled: isSentByMe
            LayoutMirroring.childrenInherit: true
            spacing: 8
            Item {width: 12; height: 12}
            QAvatar {
                id: avatar
                width: 36
                height: 36
                avatarUrl: avatarLink
                username: sendername
                visible: !isSentByMe
                displayStatus: false
            }
            Column {
                id: contentChat
                spacing: 4
                QText {
                    id: contentChatHeader
                    height: 16
                    text: sendername
                    color: "#595959"
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                    anchors.left: parent.left
                    visible: !isSentByMe
                }
                Row {
                    spacing: 4
                    anchors.left: parent.left
                    Column {
                        spacing: 0
                        Item {
                            id: chatmessageItem
                            width: chatMessageBox.width
                            height: chatMessageBox.height
                            Item {
                                id: chatMessageBox
                                height: messageCol.height + 26
                                width: Math.max(40, messageCol.width + 50)
                                anchors.left: parent.left
                                Rectangle {
                                    id: maskChat
                                    width: parent.width - 2
                                    height: parent.height - 2
                                    anchors.centerIn: parent
                                    radius: 24
                                    Rectangle {
                                        width: parent.radius
                                        height: parent.radius
                                        anchors {
                                            top: parent.top
                                            left: parent.left
                                        }
                                        color: parent.color
                                    }
                                    visible: false
                                }
                                Rectangle {
                                    id: backgroundChat
                                    width: parent.width - 2
                                    height: parent.height - 2
                                    anchors.centerIn: parent
                                    color: isSentByMe ? "#D0E2FF" : "#FFFFFF"
                                    visible: false
                                }
                                OpacityMask {
                                    id: backgroundOpacity
                                    anchors.fill: backgroundChat
                                    source: backgroundChat
                                    anchors.centerIn: parent
                                    maskSource: maskChat
                                }
                            }
                            InnerShadow {
                                anchors.fill: chatMessageBox
                                cached: true
                                horizontalOffset: 0
                                verticalOffset: 0
                                radius: 1
                                samples: 24
                                color: Qt.rgba(0, 0, 0, 0.8)
                                smooth: true
                                source: chatMessageBox
                                visible: !isSentByMe
                            }
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton | Qt.RightButton
                                onClicked: {
                                    if (mouse.button === Qt.RightButton) {
                                        optionMenu.labels = ["Copy message"]
                                        optionMenu.icons = ["qrc:/Images/Images/Copy_031F2B.png"]
                                        optionMenu.functions = [function(){
                                            var txt = ClientController.getPlainText(messageText.text)
                                            ClientController.copyMessage(txt)
                                        }]
                                        optionMenu.popup()
                                    }
                                    else{
                                        conversationLeftClicked()
                                    }
                                }
                            }
                            Column {
                                id: messageCol
                                anchors.left: parent.left
                                anchors.leftMargin: 24
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 8
                                QAttachment {
                                    roomEventID: conversationRoot.evtId
                                    progressObject: conversationRoot.progressObject
                                    file_mimeType: _FILE_VIDEO
                                    file_path: conversationRoot.file_path
                                }
                                QText {
                                    text: messageContent
                                    color: "#031F2B"
                                    width: Math.min(conversationRoot.width*0.70, implicitWidth)
                                    wrapMode: Text.WrapAnywhere
                                    font.family: "Lato"
                                    textFormat: Text.RichText
                                    lineHeight: 1.5
                                    horizontalAlignment: Text.AlignJustify
                                    onLinkActivated: Qt.openUrlExternally( link )
                                }
                            }
                        }
                    }
                    QText {
                        id: contentChatFooter
                        text: messageTime
                        height: 16
                        color: "#595959"
                        font.pixelSize: 12
                        font.weight: Font.DemiBold
                        anchors.verticalCenter: parent.verticalCenter
                        visible: false
                        onVisibleChanged: {
                            if(visible) countDown.start()
                        }
                        NumberAnimation {
                            id: countDown
                            target: contentChatFooter
                            property: "visible"
                            duration: 5000
                            easing.type: Easing.InOutQuad
                            onStopped: contentChatFooter.visible = false
                        }
                    }
                }
                QText {
                    color: "#595959"
                    font.pixelSize: 14
                    anchors.left: parent.left
                    text: (evtId !== "") ? STR.STR_QML_477 : STR.STR_QML_478
                    visible: isSentByMe ? ((evtId === "") || (index === (RoomWalletData.currentRoom.conversation.count - 1))) : false
                }
            }
        }
    }
    Component {
        id: fileOtherComponent
        Row {
            LayoutMirroring.enabled: isSentByMe
            LayoutMirroring.childrenInherit: true
            spacing: 8
            Item {width: 12; height: 12}
            QAvatar {
                id: avatar
                width: 36
                height: 36
                avatarUrl: avatarLink
                username: sendername
                visible: !isSentByMe
                displayStatus: false
            }
            Column {
                id: contentChat
                spacing: 4
                QText {
                    id: contentChatHeader
                    height: 16
                    text: sendername
                    color: "#595959"
                    font.pixelSize: 12
                    font.weight: Font.DemiBold
                    anchors.left: parent.left
                    visible: !isSentByMe
                }
                Row {
                    spacing: 4
                    anchors.left: parent.left
                    Column {
                        spacing: 0
                        Item {
                            id: chatmessageItem
                            width: chatMessageBox.width
                            height: chatMessageBox.height
                            Item {
                                id: chatMessageBox
                                height: messageCol.height + 26
                                width: Math.max(40, messageCol.width + 50)
                                anchors.left: parent.left
                                Rectangle {
                                    id: maskChat
                                    width: parent.width - 2
                                    height: parent.height - 2
                                    anchors.centerIn: parent
                                    radius: 24
                                    Rectangle {
                                        width: parent.radius
                                        height: parent.radius
                                        anchors {
                                            top: parent.top
                                            left: parent.left
                                        }
                                        color: parent.color
                                    }
                                    visible: false
                                }
                                Rectangle {
                                    id: backgroundChat
                                    width: parent.width - 2
                                    height: parent.height - 2
                                    anchors.centerIn: parent
                                    color: isSentByMe ? "#D0E2FF" : "#FFFFFF"
                                    visible: false
                                }
                                OpacityMask {
                                    id: backgroundOpacity
                                    anchors.fill: backgroundChat
                                    source: backgroundChat
                                    anchors.centerIn: parent
                                    maskSource: maskChat
                                }
                            }
                            InnerShadow {
                                anchors.fill: chatMessageBox
                                cached: true
                                horizontalOffset: 0
                                verticalOffset: 0
                                radius: 1
                                samples: 24
                                color: Qt.rgba(0, 0, 0, 0.8)
                                smooth: true
                                source: chatMessageBox
                                visible: !isSentByMe
                            }
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton | Qt.RightButton
                                onClicked: {
                                    if (mouse.button === Qt.RightButton) {
                                        optionMenu.labels = ["Copy message"]
                                        optionMenu.icons = ["qrc:/Images/Images/Copy_031F2B.png"]
                                        optionMenu.functions = [function(){
                                            var txt = ClientController.getPlainText(messageText.text)
                                            ClientController.copyMessage(txt)
                                        }]
                                        optionMenu.popup()
                                    }
                                    else{
                                        conversationLeftClicked()
                                    }
                                }
                            }
                            Column {
                                id: messageCol
                                anchors.left: parent.left
                                anchors.leftMargin: 24
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 8
                                QAttachment {
                                    roomEventID: conversationRoot.evtId
                                    progressObject: conversationRoot.progressObject
                                    file_mimeType: _FILE_OTHER
                                    file_path: conversationRoot.file_path
                                }
                                QText {
                                    text: messageContent
                                    color: "#031F2B"
                                    width: Math.min(conversationRoot.width*0.70, implicitWidth)
                                    wrapMode: Text.WrapAnywhere
                                    font.family: "Lato"
                                    textFormat: Text.RichText
                                    lineHeight: 1.5
                                    horizontalAlignment: Text.AlignJustify
                                    onLinkActivated: Qt.openUrlExternally( link )
                                }
                            }
                        }
                    }
                    QText {
                        id: contentChatFooter
                        text: messageTime
                        height: 16
                        color: "#595959"
                        font.pixelSize: 12
                        font.weight: Font.DemiBold
                        anchors.verticalCenter: parent.verticalCenter
                        visible: false
                        onVisibleChanged: {
                            if(visible) countDown.start()
                        }
                        NumberAnimation {
                            id: countDown
                            target: contentChatFooter
                            property: "visible"
                            duration: 5000
                            easing.type: Easing.InOutQuad
                            onStopped: contentChatFooter.visible = false
                        }
                    }
                }
                QText {
                    color: "#595959"
                    font.pixelSize: 14
                    anchors.left: parent.left
                    text: (evtId !== "") ? STR.STR_QML_477 : STR.STR_QML_478
                    visible: isSentByMe ? ((evtId === "") || (index === (RoomWalletData.currentRoom.conversation.count - 1))) : false
                }
            }
        }
    }
    QContextMenu {
        id: optionMenu
        menuWidth: 180
        labels: [
            "Copy message"
        ]
        icons: [
            "qrc:/Images/Images/Copy_031F2B.png"
        ]
        onItemClicked: {
            functions[index]()
        }
    }
}
