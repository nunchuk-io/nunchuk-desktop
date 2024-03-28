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
import QtQuick.Layouts 1.3
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
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: homeroot
    Component {
        id: step1
        QHomeInitialStep1 {
            anchors.fill: parent
        }
    }
    Component {
        id: step2
        QHomeInitialStep2 {
            anchors.fill: parent
        }
    }
    Component {
        id: step3
        QHomeInitialStep3 {
            anchors.fill: parent
            existWallet: walletmanagerlst.count > 0
        }
    }
    Component {
        id: pendingWallet
        QHomePendingWallet {
            anchors.fill: parent
        }
    }

    Row {
        Item {
            id: pannel_left
            width: 386
            height: homeroot.height
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
            QMenuBar {
                width: 72
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                enables: [
                    ClientController.isNunchukLoggedIn,
                    true,
                    true
                ]
                onLocalModeRequest: {
                    if(0 === AppModel.nunchukMode){
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE)
                    }
                    else{
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINE_HWL)
                    }
                }
                onOnlineModeRequest: {
                    QMLHandle.sendEvent(EVT.EVT_GOTO_HOME_CHAT_TAB)
                }
                onOpenSettingRequest: {
                    QMLHandle.sendEvent(EVT.EVT_GOTO_APP_SETTINGS_TAB)
                }
                onSignoutRequest: {
                    ClientController.requestSignout()
                }
                onUpdateProfileRequest: {
                    QMLHandle.sendEvent(EVT.EVT_ROOT_UPDATE_PROFILE)
                }
                onSigninRequest: {
                    QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                }
                onServiceRequest: {
                    QMLHandle.sendEvent(EVT.EVT_GOTO_SERVICE_SETTING_TAB)
                }
            }
            Item {
                width: 314
                height: parent.height
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                ColumnLayout {
                    anchors.fill: parent
                    anchors.topMargin: 24
                    anchors.bottomMargin: 0
                    spacing: 24
                    QHomeManagerWallets {
                        id: walletmanagerlst
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        onAskDeny: {
                            _confirm.data = deny
                            _confirm.contentText = STR.STR_QML_963
                            _confirm.open()
                        }
                    }
                    QHomeManagerSigners {
                        id: mastersignerlist
                        Layout.fillWidth: true
                        Layout.preferredHeight: (parent.height - 150)/2
                    }
                    QHomeFreeRate {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 150
                    }
                }
            }
        }
        Rectangle {
            width: homeroot.width - pannel_left.width
            height: homeroot.height
            color: "#FFFFFF"
            Loader {
                width: parent.width
                height: parent.height
                anchors.centerIn: parent
                sourceComponent: {
                    var dashboard = GroupWallet.dashboardInfo ? GroupWallet.dashboardInfo : AppModel.walletInfo.dashboardInfo
                    var isShowDashBoard = (dashboard && dashboard.groupId !== "") ? dashboard.isShowDashBoard : false
                    if (isShowDashBoard) {
                        return pendingWallet
                    }
                    else {
                        if(walletmanagerlst.count > 0) {
                            return step3;
                        } else {
                            if(mastersignerlist.count > 0){ return step2;}
                            else{ return step1 ;}
                        }
                    }
                }
            }
        }
    }

    Popup {
        id: displayAddressBusybox
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape
        background: Item{}
        property string addrToVerify: ""
        Rectangle {
            id: displayAddressMask
            width: 500
            height: 250
            radius: 8
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 500
                    height: 250
                    radius: 8
                }
            }
            QCloseButton {
                anchors {
                    right: parent.right
                    rightMargin: 16
                    top: parent.top
                    topMargin: 16
                }
                onClicked: {displayAddressBusybox.close()}
            }
            Column {
                spacing: 16
                anchors.centerIn: parent
                QBusyIndicator {
                    width: 70
                    height: 70
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 14
                    font.weight: Font.Bold
                    text: STR.STR_QML_008
                }
                Rectangle {
                    width: 450
                    height: 60
                    color: Qt.rgba(0, 0, 0, 0.1)
                    anchors.horizontalCenter: parent.horizontalCenter
                    QText {
                        anchors.fill: parent
                        anchors.leftMargin: 5
                        anchors.rightMargin: 5
                        anchors.topMargin: 5
                        anchors.bottomMargin: 5
                        wrapMode: Text.WrapAnywhere
                        horizontalAlignment: Text.AlignHCenter
                        text: AppModel.walletInfo.walletAddress
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "#323E4A"
                    }
                }
            }
        }
        DropShadow {
            anchors.fill: displayAddressMask
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: displayAddressMask
        }
    }
    Connections {
        target: AppModel
        onStartDisplayAddress: {
            if(isOnTop) displayAddressBusybox.open()
            else displayAddressBusybox.close()
        }
        onFinishedDisplayAddress: {
            displayAddressBusybox.close()
        }
    }
    QPopupInfo{
        id:_info1
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1048
    }

    QPopupInfoTwoButtons {
        id: _info2
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1115
        labels: [STR.STR_QML_341,STR.STR_QML_1114]
        isVertical: true
        funcs: [
            function() {},
            function() {
                _content.signalChangeDate()
            }
        ]
    }

     Component.onCompleted: {
         if (ClientController.isNunchukLoggedIn === false) {
            if (AppSetting.isFirstTimeOnboarding === false) {
                OnBoarding.state = "onboarding"
                QMLHandle.sendEvent(EVT.EVT_ONBOARDING_REQUEST)
            }
         }
     }

    QConfirmYesNoPopup{
        id:_confirm
        property var data
        contentText:STR.STR_QML_963
        onConfirmNo: close()
        onConfirmYes: {
            close()
            QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, data)
        }
    }

    /*=========================================SYNC=========================================*/
    Connections {
        target: AppModel
        onOpenPromtNunchukSync: {
            syncProgressBox.open()
        }
        onClosePromtNunchukSync: {
            syncProgressBox.close()
        }
    }

    Timer {
        id: syncPopupTimeOut
        interval: 10000
        repeat: false
        running: false
        onTriggered: syncProgressBox.close()
    }

    Popup {
        id: syncProgressBox
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape
        background: Item{}
        Loader {
            id: boxmask
            anchors.centerIn: parent
            sourceComponent: first4sSync
        }
        DropShadow {
            anchors.fill: boxmask
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: boxmask
        }
        onOpened: {
            boxmask.sourceComponent = first4sSync
            timer4s.restart()
        }
        onClosed: {
            boxmask.sourceComponent = null
            timer4s.stop()
        }
        Timer {
            id: timer4s
            interval: 4000
            repeat: false
            running: false
            onTriggered: {
                boxmask.sourceComponent = after4sSync
                syncPopupTimeOut.restart()
            }
        }
    }
    Component {
        id: first4sSync
        Rectangle {
            width: 300
            height: 128
            radius: 24
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 300
                    height: 128
                    radius: 24
                }
            }
            Column {
                spacing: 12
                anchors.centerIn: parent
                QBusyIndicator {
                    width: 70
                    height: 70
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    text: STR.STR_QML_387
                }
            }
        }
    }
    Component {
        id: after4sSync
        Rectangle {
            width: 300
            height: 252
            radius: 24
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 300
                    height: 252
                    radius: 24
                }
            }
            Column {
                spacing: 12
                anchors.centerIn: parent
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.DemiBold
                    text: STR.STR_QML_388
                }
                QText {
                    width: 228
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    text: STR.STR_QML_389
                    horizontalAlignment: Text.AlignHCenter
                }
                ProgressBar {
                    width: 228
                    height: 8
                    from: 0
                    to: 100
                    value: AppSetting.syncPercent
                    onValueChanged: {
                        if(value < 100){
                            syncPopupTimeOut.restart()
                        }
                    }
                }
            }
        }
    }
}
