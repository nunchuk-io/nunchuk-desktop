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
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR
QScreen {
    id: roots
    popupWidth: 800

    QOnScreenContent {
        id: contenCenter
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_336
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET)
        }
        QImage {
            id: compressButton
            width: 24
            height: 24
            anchors {
                right: parent.right
                rightMargin: 84
                top: parent.top
                topMargin: 44
            }
            transformOrigin: Item.Center
            source: "qrc:/Images/Images/OnlineMode/compress-24px.png"
            scale: compressmouse.pressed ? 1: compressmouse.containsMouse ? 1.1 : 1
            MouseArea {
                id: compressmouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET)
                }
            }
        }
        QText {
            anchors.left: bgContent.left
            anchors.bottom: bgContent.top
            anchors.bottomMargin: 4
            text: STR.STR_QML_337
            color: "#031F2B"
            font.pixelSize: 16
            font.family: "Lato"
            font.weight: Font.Bold
        }
        QText {
            anchors.right: bgContent.right
            anchors.bottom: bgContent.top
            anchors.bottomMargin: 4
            text: RoomWalletData.pendingSigners + STR.STR_QML_338
            color: "#031F2B"
            font.pixelSize: 12
            font.family: "Lato"
        }
        Rectangle {
            id: bgContent
            width: 540
            height: 456
            radius: 12
            color: "#FFFFFF"
            border.color: "#EAEAEA"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 125
            anchors.leftMargin: 36
            Flickable {
                id: flickerSignerList
                anchors.fill: parent
                anchors.margins: 12
                clip: true
                flickableDirection: Flickable.VerticalFlick
                interactive: true
                contentHeight: contentDisplay.height
                ScrollBar.vertical: ScrollBar { active: true }
                property int numberSelected: 0
                Column {
                    id: contentDisplay
                    QListView {
                        id: mastersignerlist
                        width: flickerSignerList.width
                        height: 70*mastersignerlist.count
                        model: AppModel.masterSignerList
                        interactive: false
                        delegate: QAssignSignerDelegate {
                            width: flickerSignerList.width
                            height: 70
                            needXpub: model.master_signer_need_xpub && (model.master_signer_type === NUNCHUCKTYPE.HARDWARE || model.master_signer_type === NUNCHUCKTYPE.SOFTWARE )
                            signername : model.master_signer_name
                            signerXFP: "XFP: " + model.master_signer_fingerPrint
                            devicetype: model.master_signer_deviceType
                            signerType: model.master_signer_type
                            checkedState: model.master_signer_checked
                            isPrimaryKey: model.master_signer_primary_key
                            onItemChecked: {
                                if(model.master_signer_primary_key && master_signer_checked === false){
                                    _warning.model = model
                                    _warning.open()
                                }else{
                                    if(model.master_signer_checked){
                                        model.master_signer_checked = false
                                        flickerSignerList.numberSelected--
                                        //                                    flickerSignerList.numberSelected = Math.max(0, flickerSignerList.numberSelected)
                                    }
                                    else{
                                        if(flickerSignerList.numberSelected < RoomWalletData.pendingSigners){
                                            model.master_signer_checked = true
                                            flickerSignerList.numberSelected++
                                        }
                                        else{ displayLimitAssign.open() }
                                    }
                                }
                            }
                            onGetXpubRequest: { startCacheXpubs(model.master_signer_id, model.master_signer_name) }
                        }
                    }
                    QListView {
                        id: remoteSignerlist
                        width: flickerSignerList.width
                        height: 70*remoteSignerlist.count
                        model: AppModel.remoteSignerList
                        interactive: false
                        delegate: QAssignSignerDelegate {
                            width: flickerSignerList.width
                            height: 70
                            signername : model.singleSigner_name
                            signerXFP: "XFP: " + model.singleSigner_masterFingerPrint
                            signerType: NUNCHUCKTYPE.AIRGAP
                            checkedState: model.single_signer_checked
                            devicetype: model.single_signer_devicetype
                            onItemChecked: {
                                if(model.single_signer_checked){
                                    model.single_signer_checked = false
                                    flickerSignerList.numberSelected--
//                                    flickerSignerList.numberSelected = Math.max(0, flickerSignerList.numberSelected)
                                }
                                else{
                                    if(flickerSignerList.numberSelected < RoomWalletData.pendingSigners){
                                        model.single_signer_checked = true
                                        flickerSignerList.numberSelected++
                                    }
                                    else{ displayLimitAssign.open() }
                                }
                            }
                        }
                    }
                }
            }
        }
        QButtonTextLink {
            height: 48
            label: STR.STR_QML_059
            fontPixelSize: 16
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG)
            }
        }
        QTextButton {
            id: buttonBottomR
            width: 100
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            anchors {
                right: parent.right
                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {
                flickerSignerList.numberSelected = 0
                RoomWalletData.currentRoom.joinAndLeaveWallet()
            }
        }
    }
    function startCacheXpubs(id, name) {
        getXPUPTimer.master_signer_id = id
        getXPUPTimer.master_signer_name = name
        displayGetXpub.open()
        getXPUPTimer.restart()
    }
    QPopupInfoVertical {
        id: _warning
        property var model
        title: STR.STR_QML_661
        contentText: STR.STR_QML_669
        labels: [STR.STR_QML_670,STR.STR_QML_035]
        onConfirmNo: close()
        onConfirmYes: {
            close()
            model.master_signer_checked = true
        }
    }
    Timer {
        id: getXPUPTimer
        property string master_signer_name: ""
        property string master_signer_id: ""
        interval: 1000
        onTriggered: {
            if(getXPUPTimer.master_signer_id != ""){
                RoomWalletData.currentRoom.getXpub(getXPUPTimer.master_signer_id)
                displayGetXpub.close()
                getXPUPTimer.master_signer_id = ""
                getXPUPTimer.master_signer_name = ""
            }
        }
    }
    Popup {
        id: displayGetXpub
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        Rectangle {
            id: displayAddressMask
            width: popupWidth
            height: popupHeight
            radius: 8
            color: Qt.rgba(0, 0, 0, 0.9)
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: popupWidth
                    height: popupHeight
                    radius: 24
                }
            }
            Column {
                spacing: 8
                anchors.centerIn: parent
                Item {
                    width: 52
                    height: 52
                    QBusyIndicator {
                        width: 44
                        height: 44
                        anchors.centerIn: parent
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                QText {
                    width: 500
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 24
                    font.weight: Font.DemiBold
                    wrapMode: Text.WrapAnywhere
                    font.family: "Montserrat"
                    text: STR.STR_QML_074 + getXPUPTimer.master_signer_name
                    lineHeight: 36
                    lineHeightMode: Text.FixedHeight
                }
                QText {
                    width: 328
                    height: 42
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    color: "#F6D65D"
                    font.pixelSize: 16
                    font.family: "Lato"
                    text: STR.STR_QML_075
                }
            }
        }
    }
    Popup {
        id: displayLimitAssign
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
        background: Item{}
        Rectangle {
            width: popupWidth
            height: popupHeight
            radius: 8
            color: Qt.rgba(255, 255, 255, 0.7)
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: popupWidth
                    height: popupHeight
                    radius: 24
                }
            }
            Rectangle {
                id: bgcontentlimit
                width: 300
                height: 254
                anchors.centerIn: parent
                color: "#FFFFFF"
                radius: 24
                Column {
                    spacing: 12
                    anchors.centerIn: parent
                    QText {
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.family: "Lato"
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        text: STR.STR_QML_339
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 20
                    }
                    QText {
                        width: 252
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.family: "Lato"
                        font.pixelSize: 16
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: STR.STR_QML_340
                    }
                    QTextButton {
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: 120
                        height: 36
                        label.text: STR.STR_QML_341
                        label.font.pixelSize: 12
                        type: eTypeA
                        onButtonClicked: {
                            displayLimitAssign.close()
                        }
                    }
                }
            }
            DropShadow {
                anchors.fill: bgcontentlimit
                horizontalOffset: 3
                verticalOffset: 5
                spread: 0
                radius: 8
                samples: 30
                color: "#aa000000"
                source: bgcontentlimit
            }
        }
    }
}
