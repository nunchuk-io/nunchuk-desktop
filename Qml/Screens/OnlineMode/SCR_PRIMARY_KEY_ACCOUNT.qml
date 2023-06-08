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
    readonly property int _NOT_EXIST_PRIMARY_KEY: 0
    readonly property int _EXIST_PRIMARY_KEY: 1
    property bool isSignIn: true
    property var primaryKeyList
    property int primaryKeySize: 0
    property var _SCREENS: [notHavePrimaryKeyAccount,existPrimaryKeyAccount]
    property var whereIn: (primaryKeySize > 0 && isSignIn === true) ? _EXIST_PRIMARY_KEY : _NOT_EXIST_PRIMARY_KEY
    Loader {
        id: loadercontent
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: _SCREENS[whereIn]
        QPopupToast{
            id:_warning
            x:36
            y:520
            warningType:AppModel.warningMessage.type
            warningCode: AppModel.warningMessage.code
            warningWhat: AppModel.warningMessage.what
            warningContent: AppModel.warningMessage.contentDisplay
            warningExplain: AppModel.warningMessage.explaination
        }
    }
    Component{
        id:notHavePrimaryKeyAccount
        QOnScreenContent {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_629
            onCloseClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
            }
            Column{
                anchors{
                    top: parent.top
                    topMargin: 144
                    horizontalCenter: parent.horizontalCenter
                }
                spacing: 24
                Rectangle{
                    width: 96
                    height: 96
                    radius: 96/2
                    color: "#F5F5F5"
                    anchors.horizontalCenter: parent.horizontalCenter
                    QImage {
                        anchors.centerIn: parent
                        width: 64
                        height: 64
                        source: "qrc:/Images/Images/primary-key-dark.svg"
                    }
                }

                QText {
                    width: 500
                    height: 56
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    text:STR.STR_QML_630
                }
                QText {
                    width: 500
                    height: 56
                    font.family: "Lato"
                    font.pixelSize: 16
                    color: "#031F2B"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    text:STR.STR_QML_631
                }

            }
            QButtonTextLink {
                height: 24
                label: STR.STR_QML_059
                anchors {
                    left: parent.left
                    leftMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                onButtonClicked: {
                    if(QMLHandle.onsRequester() === EVT.STATE_ID_ROOT){
                        if(primaryKeySize > 0 && isSignIn){
                            whereIn = _EXIST_PRIMARY_KEY
                        }
                        else{
                            QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
                        }
                    }
                    else{
                        if(isSignIn){
                            QMLHandle.sendEvent(EVT.EVT_PRIMARY_KEY_BACK_TO_SIGN_IN)
                        }
                        else{
                            QMLHandle.sendEvent(EVT.EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT)
                        }
                    }
                }
            }

            QTextButton {
                width: 200
                height: 48
                label.text: STR.STR_QML_265
                label.font.pixelSize: 16
                type: eTypeE
                visible: !isSignIn
                anchors {
                    right: parent.right
                    rightMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ADD_PRIMARY_KEY_REQUEST)
                }
            }
            QButtonLargeTail {
                id: signIn
                width: 198
                height: 48
                anchors {
                    right: parent.right
                    rightMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                visible: isSignIn
                label: STR.STR_QML_265
                type: eSECONDARY
                optionVisible: signInMenu.visible
                onButtonClicked: {
                    signInMenu.x = signIn.width - signInMenu.width
                    signInMenu.y = 20 - signInMenu.height
                    signInMenu.open()
                }
                QContextMenu {
                    id: signInMenu
                    menuWidth: 300
                    labels: [
                        STR.STR_QML_632,
                        STR.STR_QML_633,
                        STR.STR_QML_634
                    ]
                    icons: [
                        "",
                        "",
                        ""
                    ]
                    enables: [false,true,true]
                    onItemClicked: {
                        switch(index){
                        case 0: // Sign in remotely via QR

                            break;
                        case 1: // Sign in manually
                            QMLHandle.sendEvent(EVT.EVT_SCR_SIGN_IN_MANUALLY_REQUEST)
                            break;
                        case 2: // Import Primary Keys
                            QMLHandle.sendEvent(EVT.EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST)
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }
    Component{
        id:existPrimaryKeyAccount
        QOnScreenContent {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_653
            onCloseClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
            }
            Column{
                spacing: 32
                anchors {
                    left: parent.left
                    leftMargin: 36
                    top: parent.top
                    topMargin: 96
                }
                QText {
                    width: 539
                    height: 56
                    text: STR.STR_QML_654
                    color: "#031F2B"
                    font.weight: Font.Normal
                    wrapMode: Text.WrapAnywhere
                    font.family: "Lato"
                    font.pixelSize: 16
                }
                Rectangle{
                    id:_primaryKeyScreen
                    width: 539
                    height: 396
                    radius: 12
                    border.color:"#EAEAEA"
                    border.width: 2
                    color: "transparent"
                    InnerShadow {
                        id: drshd
                        anchors.fill: parent
                        source: parent
                    }
                    QListView {
                        id: primaryKeys
                        width: parent.width - 12*2
                        height: Math.min(primaryKeys.contentHeight,396 - 12*2)
                        model: primaryKeyList
                        anchors {
                            left: parent.left
                            leftMargin: 12
                            top: parent.top
                            topMargin: 12
                        }
                        clip: true
                        delegate: Item {
                            width: primaryKeys.width
                            height: 84
                            Row{
                                width: primaryKeys.width
                                height: 84
                                anchors.left: parent.left
                                anchors.leftMargin: 0
                                spacing: 8
                                Rectangle {
                                    width: 48
                                    height: 48
                                    radius: width
                                    color: "#F5F5F5"
                                    anchors.verticalCenter: parent.verticalCenter
                                    QImage {
                                        width: 24
                                        height: 24
                                        anchors.centerIn: parent
                                        source: GlobalData.icons("software",NUNCHUCKTYPE.SOFTWARE)
                                        sourceSize.width: 100
                                        sourceSize.height: 100
                                    }
                                }
                                Item{
                                    width: 146
                                    height: 60
                                    anchors.verticalCenter: parent.verticalCenter
                                    Column{
                                        spacing: 4
                                        QText {
                                            width: 146
                                            height: 20
                                            text: modelData.name
                                            color: "#031F2B"
                                            font.weight: Font.Normal
                                            font.family: "Lato"
                                            font.pixelSize: 16
                                        }
                                        QText {
                                            width: 146
                                            height: 20
                                            text: modelData.account
                                            color: "#595959"
                                            font.weight: Font.Normal
                                            font.family: "Lato"
                                            font.pixelSize: 12
                                        }
                                        QText {
                                            width: 146
                                            height: 20
                                            text: "XFP:" + modelData.master_fingerprint
                                            color: "#595959"
                                            font.weight: Font.Normal
                                            font.capitalization: Font.AllUppercase
                                            font.family: "Lato"
                                            font.pixelSize: 12
                                        }
                                    }
                                }
                            }
                            QImage {
                                id: _arrow
                                width: 24
                                height: 24
                                source: "qrc:/Images/Images/right-arrow-dark.svg"
                                scale: primaryKeyMouse.containsMouse ? 1.1 : 1.0
                                anchors{
                                    right: parent.right
                                    rightMargin: 0
                                    verticalCenter: parent.verticalCenter
                                }
                            }
                            MouseArea {
                                id: primaryKeyMouse
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                anchors.fill: parent
                                onClicked: {
                                    QMLHandle.sendEvent(EVT.EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST,modelData)
                                }
                            }
                        }
                        ScrollBar.vertical: ScrollBar { }
                    }
                }
            }
            QButtonTextLink {
                height: 24
                label: STR.STR_QML_059
                anchors {
                    left: parent.left
                    leftMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                onButtonClicked: {
                    if(QMLHandle.onsRequester() === EVT.STATE_ID_ROOT){
                        QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
                    }
                    else{
                        if(isSignIn){
                            QMLHandle.sendEvent(EVT.EVT_PRIMARY_KEY_BACK_TO_SIGN_IN)
                        }
                        else{
                            QMLHandle.sendEvent(EVT.EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT)
                        }
                    }
                }
            }

            QTextButton {
                width: 200
                height: 48
                label.text: STR.STR_QML_655
                label.font.pixelSize: 16
                type: eTypeE
                anchors {
                    right: parent.right
                    rightMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                onButtonClicked: {
                    whereIn = _NOT_EXIST_PRIMARY_KEY
                }
            }
        }
    }

}
