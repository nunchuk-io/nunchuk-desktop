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
import Qt.labs.platform 1.1
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
import "../../Components/customizes/services"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: rootSettings
    width: QAPP_DEVICE_WIDTH
    height: QAPP_DEVICE_HEIGHT
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
                QMenuBar {
                    width: 72
                    height: pannel_left.height
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
                }
                Item {
                    id: leftpanel
                    width: 314
                    height: pannel_left.height
                    anchors.verticalCenter: parent.verticalCenter
                    Item {
                        visible: !ServiceSetting.isSubscriber
                        anchors.fill: parent
                        anchors.topMargin: 24
                        Item {
                            width: 289
                            height: 168
                            anchors.centerIn: parent
                            Column {
                                anchors.fill: parent
                                spacing: 16
                                Rectangle {
                                    width: 96
                                    height: 96
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    color: "#F5F5F5"
                                    radius: 48
                                    QImage {
                                        width: 60
                                        height: 60
                                        anchors.centerIn: parent
                                        source: "qrc:/Images/Images/services-dark.svg"
                                    }
                                }
                                QText {
                                    width: parent.width
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: STR.STR_QML_753
                                    color: "#FFFFFF"
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                    font.weight: Font.Normal
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                    Column {
                        visible: ServiceSetting.isSubscriber
                        anchors.fill: parent
                        anchors.topMargin: 24
                        spacing: 12
                        Column {
                            spacing: 0
                            Repeater {
                                id: itemsSetting
                                readonly property int _EMERGENCY: 0
                                readonly property int _EMERGENCY_LOCKDOWN: 1
                                readonly property int _KEY_RECOVERY: 2
                                readonly property int _INHERITANCE_PLANING: 3
                                function isHoneyBadger(){
                                    return ClientController.user.plan_slug === "honey_badger"
                                }
                                readonly property int _VIEW_INHERITANCE_PLANING: 4
                                readonly property int _CLAIM_AN_INHERITANCE: 5
                                readonly property int _YOUR_SUBSCRIPTION: 6
                                readonly property int _PLATFORM_KEY_CO_SIGNING_POLICIES: 7
                                readonly property int _GET_ADDITIONAL_WALLETS: 8
                                readonly property int _ORDER_NEW_HARDWARE: 9
                                readonly property int _MANAGE_SUBSCRIPTION: 10
                                readonly property var setting_map: [
                                    {screen:_EMERGENCY,             visible: true, enable:false,  title:STR.STR_QML_702, icon: "qrc:/Images/Images/emergency-light.svg"   ,action: function(){} },
                                    {screen:_EMERGENCY_LOCKDOWN,    visible: true, enable:true,  title:STR.STR_QML_697, icon: ""   ,action: function(){} },
                                    {screen:_KEY_RECOVERY,          visible: true, enable:true,  title:STR.STR_QML_698, icon: ""   ,action: function(){} },
                                    {screen:_INHERITANCE_PLANING,   visible: isHoneyBadger(), enable:false,  title:STR.STR_QML_736, icon: "qrc:/Images/Images/inheritance-light.svg"   ,action: function(){} },
                                    {screen:_VIEW_INHERITANCE_PLANING,  visible: isHoneyBadger() && ServiceSetting.qAssistedSetuped.length > 0, enable:true,  title:STR.STR_QML_875, icon: ""   ,action: function(){
                                        ServiceSetting.qInheritanceWalletName = ""
                                        if (ServiceSetting.qAssistedSetuped.length > 1) {
                                            GlobalData.serviceIndex = _VIEW_INHERITANCE_PLANING
                                        } else if (ServiceSetting.qAssistedSetuped.length === 1) {
                                            QMLHandle.sendEvent(EVT.EVT_SERVICE_SELECT_WALLET_REQUEST, ServiceSetting.qAssistedSetuped[0])
                                            GlobalData.serviceIndex = _VIEW_INHERITANCE_PLANING
                                        }
                                    } },
                                    {screen:_CLAIM_AN_INHERITANCE,  visible: isHoneyBadger(), enable:true,  title:STR.STR_QML_737, icon: ""   ,action: function(){
                                        GlobalData.serviceIndex = _CLAIM_AN_INHERITANCE
                                        ServiceSetting.claimInheritanceStatus = ServiceType.CI_NONE
                                        _claim.claimClear()
                                    } },
                                    {screen:_YOUR_SUBSCRIPTION,     visible: true, enable:false,  title:STR.STR_QML_699, icon: "qrc:/Images/Images/subscription-light.svg" ,action: function(){} },
                                    {screen:_PLATFORM_KEY_CO_SIGNING_POLICIES,  visible: true, enable:true,  title:STR.STR_QML_738, icon: ""   ,action: function(){
                                            var screenObj = { "state_id" : EVT.STATE_ID_SCR_SERVICE_SETTINGS }
                                            QMLHandle.sendEvent(EVT.EVT_REENTER_YOUR_PASSWORD_REQUEST,screenObj)
                                    } },
                                    {screen:_GET_ADDITIONAL_WALLETS,visible: true, enable:true,  title:STR.STR_QML_707, icon: ""   ,action: function(){
                                        UserWallet.additionalGetWalletConfig()
                                        _InfoVer.link = "https://nunchuk.io/claim";
                                        var remainCount = ServiceSetting.qRemainingAssistedWalletCount;
                                        _InfoVer.contentText = (remainCount > 1 ? STR.STR_QML_841 : STR.STR_QML_842).arg(remainCount);
                                        _InfoVer.labels = [STR.STR_QML_341,STR.STR_QML_683];
                                        _InfoVer.open();
                                    } },
                                    {screen:_ORDER_NEW_HARDWARE,    visible: true, enable:true,  title:STR.STR_QML_700, icon: ""   ,action: function(){_InfoVer.link = "https://nunchuk.io/hardware-replacement";_InfoVer.contentText = STR.STR_QML_735;_InfoVer.labels = [STR.STR_QML_341,STR.STR_QML_683];_InfoVer.open();} },
                                    {screen:_MANAGE_SUBSCRIPTION,   visible: true, enable:true,  title:STR.STR_QML_682, icon: ""   ,action: function(){_InfoVer.link = "https://nunchuk.io/my-plan";_InfoVer.contentText = STR.STR_QML_684;_InfoVer.labels = [STR.STR_QML_341,STR.STR_QML_683];_InfoVer.open();}}
                                ]

                                model: setting_map.length
                                Rectangle {
                                    id: _item
                                    width: leftpanel.width
                                    height: 48
                                    visible: itemsSetting.setting_map[index].visible
                                    enabled: itemsSetting.setting_map[index].enable
                                    color: GlobalData.serviceIndex === index ? Qt.rgba(255, 255, 255, 0.3) : itemSetingMouse.containsMouse ?  Qt.rgba(255, 255, 255, 0.1) : "transparent"
                                    Row {
                                        spacing: 8
                                        anchors.fill: parent
                                        Item {
                                            width: 8;
                                            height: parent.height
                                            Rectangle {
                                                width: 4
                                                height: parent.height
                                                color: GlobalData.serviceIndex === index ? "#F6D65D" : "transparent"
                                                anchors.verticalCenter: parent.verticalCenter
                                            }
                                        }
                                        QImage {
                                            id:_icon
                                            width: 24
                                            height: 24
                                            anchors.verticalCenter: parent.verticalCenter
                                            source: itemsSetting.setting_map[index].icon
                                            visible: source != ""
                                            sourceSize.width: 100
                                            sourceSize.height: 100
                                        }
                                        QText {
                                            anchors.verticalCenter: parent.verticalCenter
                                            text: itemsSetting.setting_map[index].title
                                            color: "#FFFFFF"
                                            font.family: "Lato"
                                            font.pixelSize: 16
                                            font.weight: _icon.visible ? Font.DemiBold : Font.Normal
                                        }
                                    }
                                    MouseArea {
                                        id: itemSetingMouse
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        onClicked: {
                                            var _item = itemsSetting.setting_map[index]
                                            switch(_item.screen) {
                                            case itemsSetting._EMERGENCY_LOCKDOWN:
                                            case itemsSetting._KEY_RECOVERY:
                                                GlobalData.serviceIndex = _item.screen
                                                break
                                            default:
                                                _item.action()
                                                break

                                            }


                                        }
                                    }
                                    Component.onCompleted: {
                                        GlobalData.serviceIndex = itemsSetting._EMERGENCY_LOCKDOWN
                                    }
                                }
                            }
                        }
                        QIconTextButton {
                            width: 289
                            height: 48
                            type: eTypeG
                            label: STR.STR_QML_701
                            anchors.horizontalCenter: parent.horizontalCenter
                            icons: ["contact-support-dark.svg", "contact-support-light.svg", "contact-support-dark.svg", "contact-support-light.svg"]
                            fontPixelSize: 16
                            enabled: ClientController.isMatrixLoggedIn && ClientController.readySupport
                            onButtonClicked: {
                                QMLHandle.sendEvent(EVT.EVT_SERVICE_SUPPORT_REQUEST)
                            }
                        }
                    }
                }
            }
        }
        Rectangle {
            width: parent.width - pannel_left.width
            height: parent.height
            color: "#FFFFFF"
            Item {
                id:_clainNot
                anchors.fill: parent
                visible: !ServiceSetting.isSubscriber && GlobalData.serviceIndex !== itemsSetting._CLAIM_AN_INHERITANCE
                enabled: visible
                Rectangle {
                    width: parent.width
                    height: 60
                    color: "#D0E2FF"
                    visible: ClientController.isNunchukLoggedIn
                    QLato {
                        text: STR.STR_QML_737
                        font.weight: Font.Bold
                        anchors{
                            left: parent.left
                            leftMargin: 24
                            verticalCenter: parent.verticalCenter
                        }
                    }
                    QIconButton {
                        iconSize: 24
                        anchors {
                            right: parent.right
                            rightMargin: 24
                            verticalCenter: parent.verticalCenter
                        }
                        icon: "qrc:/Images/Images/right-arrow-dark.svg"
                        onButtonClicked: {
                            if (UserWallet.inheritanceCheck() === ServiceType.CI_NONE) {
                                GlobalData.serviceIndex = itemsSetting._CLAIM_AN_INHERITANCE
                                ServiceSetting.claimInheritanceStatus = ServiceType.CI_NONE
                            } else {
                                _clainNot.showPopup()
                            }
                        }
                        bgColor: "transparent"
                    }
                }
                Column {
                    anchors.fill: parent
                    spacing: 24
                    anchors{
                        left: parent.left
                        leftMargin: 24
                        top: parent.top
                        topMargin: 24 + (ClientController.isNunchukLoggedIn ? 60 : 0)
                    }
                    Rectangle {
                        width: parent.width - 24
                        height: 300
                        radius: 24
                        color: "#D0E2FF"
                        QImage {
                            anchors.centerIn: parent
                            width: sourceSize.width
                            height: sourceSize.height
                            source: "qrc:/Images/Images/assisted-wallet.svg"
                        }
                    }
                    Item {
                        width: parent.width - 24
                        height: parent.height * 0.45
                        Column {
                            width: parent.width
                            spacing: 16
                            QText {
                                text: STR.STR_QML_754
                                color: "#031F2B"
                                font.family: "Montserrat"
                                font.pixelSize: 32
                                font.weight: Font.DemiBold
                            }
                            QText {
                                text: STR.STR_QML_755
                                color: "#031F2B"
                                font.family: "Montserrat"
                                font.pixelSize: 16
                                font.weight: Font.Normal
                            }
                            Item {
                                width: parent.width
                                height: 322
                                Flickable {
                                    id: _flick
                                    anchors.fill: parent
                                    contentWidth: parent.width; contentHeight: 480
                                    clip: true
                                    ScrollBar.vertical: ScrollBar { active: true }
                                    Flow {
                                        width: parent.width
                                        spacing: 16
                                        Repeater {
                                            id: _guide
                                            width: parent.width
                                            readonly property var content_map: [
                                                {visible: true, height: 136, headline:STR.STR_QML_756, content: STR.STR_QML_757 , icon: "qrc:/Images/Images/mulitsig-dark.svg"             },
                                                {visible: true, height: 136, headline:STR.STR_QML_736, content: STR.STR_QML_759 , icon: "qrc:/Images/Images/inheritance-dark.svg"          },
                                                {visible: true, height: 108, headline:STR.STR_QML_697, content: STR.STR_QML_762 , icon: "qrc:/Images/Images/emergency-lockdown-dark.svg"   },
                                                {visible: true, height: 108, headline:STR.STR_QML_763, content: STR.STR_QML_764 , icon: "qrc:/Images/Images/signing-policy-dark.svg"       },
                                                {visible: true, height: 108, headline:STR.STR_QML_765, content: STR.STR_QML_766 , icon: "qrc:/Images/Images/key-recovery-dark.svg"         },
                                                {visible: true, height: 108, headline:STR.STR_QML_767, content: STR.STR_QML_768 , icon: "qrc:/Images/Images/contact-support-dark.svg"      },
                                                {visible: ClientController.isNunchukLoggedIn, height: 108, headline:STR.STR_QML_790, content: STR.STR_QML_791 , icon: "qrc:/Images/Images/member-discount-dark.svg"      },
                                            ]
                                            model: content_map.length
                                            Rectangle {
                                                property var _item: _guide.content_map[index]
                                                width: 414
                                                height: _item.height
                                                visible: _item.visible
                                                Row {
                                                    spacing: 12
                                                    Rectangle {
                                                        width: 48
                                                        height: 48
                                                        radius: 48
                                                        color: "#F5F5F5"
                                                        visible: _ico.source != ""
                                                        QImage {
                                                            id: _ico
                                                            anchors.centerIn: parent
                                                            width: 24
                                                            height: 24
                                                            source: _item.icon
                                                        }
                                                    }
                                                    Column {
                                                        width: 354
                                                        height: _item.height
                                                        spacing: 4
                                                        QText {
                                                            width: 354
                                                            text: _item.headline
                                                            color: "#031F2B"
                                                            font.family: "Lato"
                                                            font.pixelSize: 16
                                                            font.weight: Font.DemiBold
                                                            horizontalAlignment: Text.AlignLeft
                                                            verticalAlignment: Text.AlignVCenter
                                                        }
                                                        QText {
                                                            width: 354
                                                            text: _item.content
                                                            color: "#031F2B"
                                                            font.family: "Lato"
                                                            font.pixelSize: 16
                                                            lineHeightMode: Text.FixedHeight
                                                            lineHeight: 28
                                                            wrapMode: Text.WordWrap
                                                            horizontalAlignment: Text.AlignLeft
                                                            verticalAlignment: Text.AlignVCenter
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
                }
                QTextButton {
                    width: 123
                    height: 48
                    anchors{
                        right: parent.right
                        rightMargin: 24
                        bottom: parent.bottom
                        bottomMargin: 16
                    }
                    label.text: STR.STR_QML_770
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        _clainNot.showPopup()
                    }
                }
                function showPopup() {
                    _InfoVer.link = "https://nunchuk.io/claim"
                    _InfoVer.linkTop = true
                    _InfoVer.contentText = STR.STR_QML_771
                    _InfoVer.labels = [STR.STR_QML_772,STR.STR_QML_341]
                    _InfoVer.open();
                }
            }
            Item {
                anchors.fill: parent
                visible: ServiceSetting.isSubscriber && GlobalData.serviceIndex === itemsSetting._EMERGENCY_LOCKDOWN
                enabled: visible
                Column {
                    anchors.fill: parent
                    spacing: 24
                    anchors{
                        left: parent.left
                        leftMargin: 24
                        top: parent.top
                        topMargin: 24
                    }
                    Rectangle {
                        width: parent.width - 24
                        height: parent.height * 0.42
                        radius: 24
                        color: "#D0E2FF"
                        QImage {
                            anchors.centerIn: parent
                            width: sourceSize.width
                            height: sourceSize.height
                            source: "qrc:/Images/Images/emergency-lockdown-illustrations.svg"
                        }
                    }
                    Item {
                        width: parent.width - 24
                        height: parent.height * 0.45
                        Column {
                            width: parent.width
                            spacing: 16
                            QText {
                                text: STR.STR_QML_697
                                color: "#031F2B"
                                font.family: "Montserrat"
                                font.pixelSize: 32
                                font.weight: Font.DemiBold
                            }
                            Repeater {
                                id: _description
                                width: parent.width
                                readonly property var content_map: [
                                    {height: 84, title:STR.STR_QML_703, icon: ""   },
                                    {height: 56, title:STR.STR_QML_704, icon: "qrc:/Images/Images/1.Active.svg"   },
                                    {height: 28, title:STR.STR_QML_705, icon: "qrc:/Images/Images/2.Active.svg"   },
                                    {height: 28, title:STR.STR_QML_706, icon: "qrc:/Images/Images/3.Active.svg"    },
                                ]
                                model: content_map.length
                                Rectangle {
                                    width: parent.width
                                    height: _description.content_map[index].height
                                    Row {
                                        spacing: 8
                                        QImage {
                                            id:_numicon
                                            width: 24
                                            height: 24
                                            source: _description.content_map[index].icon
                                            visible: source != ""
                                        }
                                        QText {
                                            id:_numText
                                            width: 591
                                            height: _description.content_map[index].height
                                            anchors.verticalCenter: parent.verticalCenter
                                            text: _description.content_map[index].title
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            font.pixelSize: 16
                                            lineHeightMode: Text.FixedHeight
                                            lineHeight: 28
                                            horizontalAlignment: Text.AlignLeft
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                }
                            }
                            QTextButton {
                                width: 98
                                height: 48
                                label.text: STR.STR_QML_097
                                label.font.pixelSize: 16
                                type: eTypeE
                                onButtonClicked: {
                                    var screenObj = { "state_id" : EVT.STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD }
                                    QMLHandle.sendEvent(EVT.EVT_REENTER_YOUR_PASSWORD_REQUEST,screenObj)
                                }
                            }
                        }
                    }
                }
            }
            Item {
                anchors.fill: parent
                visible: ServiceSetting.isSubscriber && GlobalData.serviceIndex === itemsSetting._KEY_RECOVERY
                enabled: visible
                Column {
                    anchors.fill: parent
                    anchors.margins: 24
                    spacing: 10
                    QText {
                        font.family: "Lato"
                        font.pixelSize: 28
                        color: "#031F2B"
                        font.weight: Font.Bold
                        text: STR.STR_QML_698
                    }
                    Row {
                        spacing: 0
                        QText {
                            width: 627-24
                            text: STR.STR_QML_708
                            color: "#031F2B"
                            font.pixelSize: 16
                            font.family: "Lato"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QImage {
                            width: 24
                            height: 24
                            source: "qrc:/Images/Images/right-arrow-dark.svg"
                            scale: arrow.containsMouse ? 1.1 : 1
                            MouseArea {
                                id: arrow
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    var screenObj = { "state_id" : EVT.STATE_ID_SCR_KEY_RECOVERY }
                                    QMLHandle.sendEvent(EVT.EVT_REENTER_YOUR_PASSWORD_REQUEST,screenObj)
                                }
                            }
                        }
                    }
                }
            }
            Item {
                anchors.fill: parent
                visible: GlobalData.serviceIndex === itemsSetting._VIEW_INHERITANCE_PLANING
                enabled: visible
                QViewInheritancePlaning {
                    anchors.fill: parent
                    visible: ServiceSetting.qInheritanceWalletName !== ""
                }
                QSelectAnAssistedWallet {
                    visible: ServiceSetting.qAssistedSetuped.length > 1 && ServiceSetting.qInheritanceWalletName === ""
                }
                Connections {
                    target: UserWallet
                    onSecurityQuestionChanged: {
                        if (GlobalData.serviceIndex === itemsSetting._VIEW_INHERITANCE_PLANING) {
                            _Security.action = function() {
                                QMLHandle.sendEvent(EVT.EVT_INHERITANCE_PLAN_FINALIZE_REQUEST, 3)
                            }
                            _Security.open();
                        }
                    }
                    onSecurityQuestionClosed: {
                        if (GlobalData.serviceIndex === itemsSetting._VIEW_INHERITANCE_PLANING) {
                            _Security.close()
                        }
                    }
                    onInheritanceDummyTransactionAlert: {
                        if (GlobalData.serviceIndex === itemsSetting._VIEW_INHERITANCE_PLANING) {
                            _info1.contentText = STR.STR_QML_897
                            _info1.open();
                        }
                    }
                    onInheritanceInvalidActivationDateAlert: {
                        if (GlobalData.serviceIndex === itemsSetting._VIEW_INHERITANCE_PLANING) {
                            _info1.contentText = STR.STR_QML_896
                            _info1.open();
                        }
                    }
                    onInheritanceDiscardChangeAlert: {
                        if (GlobalData.serviceIndex === itemsSetting._VIEW_INHERITANCE_PLANING) {
                            _confirm.title = STR.STR_QML_024
                            _confirm.contentText = STR.STR_QML_919
                            _confirm.open();
                            _confirm.fClose = function() {
                                ServiceSetting.qInheritanceWalletName = ""
                                GlobalData.serviceIndex = itemsSetting._CLAIM_AN_INHERITANCE
                            }
                        }
                    }
                }
            }
            Item {
                anchors.fill: parent
                visible: GlobalData.serviceIndex === itemsSetting._CLAIM_AN_INHERITANCE
                enabled: visible
                Connections {
                    target: UserWallet
                    onNotPaidAlert: {
                        _InfoVer.link = "https://nunchuk.io/claim"
                        _InfoVer.linkTop = true
                        _InfoVer.contentText = STR.STR_QML_771
                        _InfoVer.labels = [STR.STR_QML_772,STR.STR_QML_341]
                        _InfoVer.open();
                    }
                    onIsExpiredAlert: {
                        _InfoVer.link = "https://nunchuk.io/claim"
                        _InfoVer.linkTop = true
                        _InfoVer.contentText = STR.STR_QML_750
                        _InfoVer.labels = [STR.STR_QML_751,STR.STR_QML_079]
                        _InfoVer.open();
                    }
                    onHasNotBeenActivatedYetAlert: {
                        _info1.open()
                    }
                }
                Column {
                    id: _claim
                    visible: ServiceSetting.claimInheritanceStatus === ServiceType.CI_NONE ||
                             ServiceSetting.claimInheritanceStatus === ServiceType.CI_IS_NOT_PAID ||
                             ServiceSetting.claimInheritanceStatus === ServiceType.CI_IS_ERROR
                    anchors.fill: parent
                    spacing: 24
                    anchors{
                        left: parent.left
                        leftMargin: 24
                        top: parent.top
                        topMargin: 24
                    }
                    function claimClear() {
                        magicPhrase.textInputted = ""
                        backupPassword.textInputted = ""
                    }
                    Rectangle {
                        width: parent.width - 24
                        height: parent.height * 0.42
                        radius: 24
                        color: "#D0E2FF"
                        QImage {
                            anchors.centerIn: parent
                            width: sourceSize.width
                            height: sourceSize.height
                            source: "qrc:/Images/Images/claim-inheritance-illustration.svg"
                        }
                    }
                    Item {
                        width: parent.width - 24
                        height: parent.height * 0.58 - 24
                        Column {
                            id: _header
                            width: parent.width
                            spacing: 16
                            QText {
                                text: STR.STR_QML_747
                                color: "#031F2B"
                                font.family: "Montserrat"
                                font.pixelSize: 32
                                font.weight: Font.DemiBold
                            }
                            QText {
                                text: STR.STR_QML_748
                                color: "#031F2B"
                                font.family: "Lato"
                                font.pixelSize: 16
                                font.weight: Font.Normal
                            }
                        }
                        QTextInputBoxTypeB {
                            id: magicPhrase
                            anchors{
                                top: _header.bottom
                                topMargin: 16
                            }
                            z: backupPassword.z + 1
                            label: STR.STR_QML_749
                            boxWidth: 537
                            boxHeight: 48
                            isValid: true
                            onTextInputtedChanged: {
                                if(!magicPhrase.isValid){
                                    magicPhrase.isValid = true
                                    magicPhrase.errorText = ""
                                }
                                magicPhrase.showError = false;

                                var targetY = magicPhrase.y + magicPhrase.height
                                var targetX = magicPhrase.x
                                if(textBoxFocus){
                                    sglst.currentIndex = 0
                                    _claim.searchingText(magicPhrase.textInputted)
                                }
                            }
                            onTextBoxFocusChanged: {
                                if(textBoxFocus){
                                    sglst.currentIndex = 0
                                    suggestionList.y = magicPhrase.y + magicPhrase.height
                                    suggestionList.x = magicPhrase.x
                                    _claim.searchingText(magicPhrase.textInputted)
                                }
                                else{
                                    sglst.currentIndex = -1
                                }
                            }
                            onDownKeyRequest: {suggestionList.downKeyRequest()  }
                            onUpKeyRequest:   {suggestionList.upKeyRequest()    }
                            onEnterKeyRequest:{suggestionList.enterKeyRequest() }
                            onTabKeyRequest: { suggestionList.enterKeyRequest() }

                            Item {
                                id: suggestionList
                                width: suggestionList.visible ? magicPhrase.width : 0
                                height: suggestionList.visible ? Math.min(sglst.contentHeight + 10, 250): 0
                                visible: sglst.currentIndex !== -1
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {}
                                }

                                function downKeyRequest() {sglst.downKeyRequest() }
                                function upKeyRequest()   {sglst.upKeyRequest()   }
                                function enterKeyRequest(){sglst.enterKeyRequest()}

                                Rectangle {
                                    id: bg
                                    anchors {
                                        fill: parent
                                        leftMargin: 5
                                        rightMargin: 5
                                        topMargin: 5
                                        bottomMargin: 5
                                    }
                                    radius: 4
                                    color: "#FFFFFF"
                                    visible: false
                                }

                                DropShadow {
                                    anchors.fill: bg
                                    verticalOffset: 3
                                    cached: true
                                    radius: 16
                                    samples: 24
                                    color: Qt.rgba(0, 0, 0, 0.4)
                                    source: bg
                                    ListView {
                                        id: sglst
                                        anchors.centerIn: parent
                                        width: parent.width - 10
                                        height: suggestionList.visible ? Math.min(sglst.contentHeight, 240): 0
                                        model: modelSearch
                                        clip: true
                                        ScrollBar.vertical: ScrollBar { active: true }
                                        delegate: Rectangle {
                                            width: sglst.width
                                            property bool shown: textsuggest.visible
                                            height: 240/10
                                            color: suggestMouse.containsMouse ? "#F1FAFE" : "#FFFFFF"
                                            QText {
                                                id: textsuggest
                                                anchors.left: parent.left
                                                text: modelData
                                                color: suggestMouse.pressed ?"#031F2B" :  (suggestMouse.containsMouse || sglst.currentIndex === index) ? "#35ABEE" : "#031F2B"
                                                font.pixelSize: 16
                                                font.weight: (suggestMouse.containsMouse || sglst.currentIndex === index) ? Font.ExtraBold : Font.Normal
                                            }

                                            MouseArea {
                                                id: suggestMouse
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                cursorShape: Qt.PointingHandCursor
                                                onClicked: {sglst.mouseKeyRequest(index)}
                                            }
                                        }

                                        function downKeyRequest() { if(sglst.currentIndex < sglst.count - 1) {sglst.currentIndex += 1}; }
                                        function upKeyRequest() {if(sglst.currentIndex > 0) {sglst.currentIndex -= 1};}
                                        function enterKeyRequest() {
                                            if (sglst.currentIndex !== -1) {
                                                var splitString = magicPhrase.textInputted.split(" ");
                                                splitString[splitString.length - 1] = modelSearch.get(sglst.currentIndex).mnemonic;
                                                magicPhrase.textInputted = splitString.toString(" ");
                                                sglst.currentIndex = -1;
                                            }
                                        }
                                        function mouseKeyRequest(index) {
                                            if(index !== -1) {
                                                var splitString = magicPhrase.textInputted.split(" ");
                                                splitString[splitString.length - 1] = modelSearch.get(index).mnemonic;
                                                magicPhrase.textInputted = splitString.toString(" ");
                                            }
                                            sglst.currentIndex = -1;
                                        }
                                    }
                                }
                            }
                        }
                        QTextInputBoxTypeB {
                            id: backupPassword
                            anchors{
                                top: _header.bottom
                                topMargin: 16 + 72 + 16
                            }
                            label: STR.STR_QML_727
                            boxWidth: 537
                            boxHeight: 48
                            isValid: true
                            onTextInputtedChanged: {
                                if(!backupPassword.isValid){
                                    backupPassword.isValid = true
                                    backupPassword.errorText = ""
                                }
                                backupPassword.showError = false;
                            }
                        }

                        QTextButton {
                            width: 98
                            height: 48
                            anchors {
                                right: parent.right
                                bottom: parent.bottom
                                bottomMargin: 16
                            }
                            label.text: STR.STR_QML_265
                            label.font.pixelSize: 16
                            type: eTypeE
                            onButtonClicked: {
                                suggestionList.enterKeyRequest()
                                var input = {
                                    "magic":magicPhrase.textInputted,
                                    "backupPassword":backupPassword.textInputted
                                }
                                QMLHandle.sendEvent(EVT.EVT_CLAIM_INHERITANCE_CHECK_REQUEST,input)
                            }
                        }
                    }
                    function searchingText(str){
                        // SPIT TEXT
                        const words = str.split(" ");
                        const lastIdx = words.length - 1;
                        const lastWord = words[lastIdx];

                        modelSearch.clear()
                        for(var i = 0; i < AppModel.suggestMnemonics.length; i++){
                            if(str === ""){
                                var data = {'mnemonic': AppModel.suggestMnemonics[i]};
                                modelSearch.append(data)
                            }
                            else{
                                var currentText = AppModel.suggestMnemonics[i].toLowerCase().substring(0, lastWord.length)
                                if (currentText.toLowerCase() === lastWord.toLowerCase()){
                                    var datafilter = {'mnemonic': AppModel.suggestMnemonics[i]};
                                    modelSearch.append(datafilter)
                                }
                            }
                        }
                        sglst.currentIndex = 0
                        return true
                    }
                    ListModel {
                        id: modelSearch
                    }
                }
                Column {
                    visible: ServiceSetting.claimInheritanceStatus === ServiceType.CI_IS_PAID
                    anchors.fill: parent
                    spacing: 24
                    anchors{
                        left: parent.left
                        leftMargin: 24
                        top: parent.top
                        topMargin: 24
                    }
                    Rectangle {
                        width: parent.width - 24
                        height: parent.height * 0.42
                        radius: 24
                        color: "#D0E2FF"
                        QImage {
                            anchors.centerIn: parent
                            width: sourceSize.width
                            height: sourceSize.height
                            source: "qrc:/Images/Images/buffer-period-illustration.svg"
                        }
                    }
                    Item {
                        width: parent.width - 24
                        height: parent.height * 0.58 - 24
                        Column {
                            width: parent.width
                            spacing: 16
                            QText {
                                text: STR.STR_QML_773
                                color: "#031F2B"
                                font.family: "Montserrat"
                                font.pixelSize: 32
                                font.weight: Font.DemiBold
                            }
                            QText {
                                text: STR.STR_QML_774
                                color: "#031F2B"
                                font.family: "Lato"
                                font.pixelSize: 16
                                font.weight: Font.Normal
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 28
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                            QText {
                                text: STR.STR_QML_774_.arg(ServiceSetting.claimInheritancePeriod)
                                color: "#031F2B"
                                font.family: "Lato"
                                font.pixelSize: 16
                                font.weight: Font.Normal
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 28
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                        QTextButton {
                            width: 98
                            height: 48
                            anchors {
                                right: parent.right
                                bottom: parent.bottom
                                bottomMargin: 16
                            }
                            label.text: STR.STR_QML_341
                            label.font.pixelSize: 16
                            type: eTypeE
                            onButtonClicked: {
                                ServiceSetting.claimInheritanceStatus = ServiceType.CI_NONE
                            }
                        }
                    }
                }
                Column {
                    visible: ServiceSetting.claimInheritanceStatus === ServiceType.CI_IS_VALID
                    anchors.fill: parent
                    spacing: 24
                    anchors{
                        left: parent.left
                        leftMargin: 24
                        top: parent.top
                        topMargin: 24
                    }
                    Rectangle {
                        width: parent.width - 24
                        height: parent.height * 0.42
                        radius: 24
                        color: "#D0E2FF"
                        Column {
                            anchors.centerIn: parent
                            width: 798
                            height: 128
                            spacing: 12
                            QLato {
                                width: parent.width
                                text: STR.STR_QML_775
                                font.pixelSize: 20
                                font.weight: Font.Bold
                                horizontalAlignment: Text.AlignHCenter
                            }
                            QMontserrat {
                                width: parent.width
                                text: ServiceSetting.inheritance.balance + (AppSetting.unit === NUNCHUCKTYPE.BTC ? " BTC" : " sats")
                                font.pixelSize: 32
                                font.weight: Font.Medium
                                horizontalAlignment: Text.AlignHCenter
                            }
                            QLato {
                                width: parent.width
                                text: qsTr("%1 %2").arg(AppSetting.currency).arg(ServiceSetting.inheritance.balanceCurrency)
                                font.pixelSize: 20
                                font.weight: Font.Normal
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }
                    Item {
                        width: parent.width - 24
                        height: parent.height * 0.58 - 24
                        Column {
                            width: parent.width
                            spacing: 16
                            QText {
                                width: 627
                                text: STR.STR_QML_776
                                color: "#031F2B"
                                font.family: "Montserrat"
                                font.pixelSize: 32
                                font.weight: Font.DemiBold
                                wrapMode: Text.WordWrap
                            }
                            QText {
                                width: 343
                                text: STR.STR_QML_781
                                color: "#031F2B"
                                font.family: "Lato"
                                font.pixelSize: 20
                                font.weight: Font.Normal
                                visible: _message.text !== ""
                            }
                            Rectangle {
                                width: 627
                                height: _message.paintedHeight + 12*2
                                color: "#F5F5F5"
                                radius: 12
                                visible: _message.text !== ""
                                QText {
                                    id: _message
                                    width: 627 - 12*2
                                    anchors {
                                        horizontalCenter: parent.horizontalCenter
                                        top: parent.top
                                        topMargin: 12
                                    }
                                    text: ServiceSetting.inheritance.note
                                    color: "#000000"
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                    font.weight: Font.Normal
                                    wrapMode: Text.WordWrap
                                }
                            }
                        }
                        Row {
                            spacing: 12
                            anchors {
                                right: parent.right
                                bottom: parent.bottom
                                bottomMargin: 16
                            }
                            QTextButton {
                                width: 71
                                height: 48
                                label.text: STR.STR_QML_777
                                label.font.pixelSize: 16
                                type: eTypeB
                                onButtonClicked: {
                                    ServiceSetting.claimInheritanceStatus = ServiceType.CI_NONE
                                }
                            }
                            QButtonLargeTail {
                                width: 220
                                height: 48
                                type: eTHIRT
                                label: STR.STR_QML_778
                                optionVisible: imExContextMenu.visible
                                onButtonClicked: {
                                    imExContextMenu.x = 20
                                    imExContextMenu.y = 20 - imExContextMenu.height
                                    imExContextMenu.open()
                                }
                                QContextMenu {
                                    id: imExContextMenu
                                    menuWidth: 320
                                    labels: [
                                        STR.STR_QML_779,
                                        STR.STR_QML_780,
                                    ]
                                    icons: [
                                        "qrc:/Images/Images/wallet-dark.svg",
                                        "qrc:/Images/Images/OnlineMode/monetization_on-24px_2.png",
                                    ]
                                    onItemClicked: {
                                        QMLHandle.sendEvent(EVT.EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST,index)
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Item {
                anchors.fill: parent
                visible: ServiceSetting.isSubscriber && GlobalData.serviceIndex === itemsSetting._PLATFORM_KEY_CO_SIGNING_POLICIES
                enabled: visible
                Connections {
                    target: UserWallet
                    onServerKeyVerifyPasswordAlert: {
                        GlobalData.serviceIndex = itemsSetting._PLATFORM_KEY_CO_SIGNING_POLICIES
                    }
                    onSecurityQuestionChanged: {
                        if (GlobalData.serviceIndex === itemsSetting._PLATFORM_KEY_CO_SIGNING_POLICIES) {
                            _Security.action = function() {
                                QMLHandle.sendEvent(EVT.EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED)
                            }
                            _Security.open();
                        }
                    }
                    onServerKeyDummyTransactionAlert: {
                        if (GlobalData.serviceIndex === itemsSetting._PLATFORM_KEY_CO_SIGNING_POLICIES) {
                            _info1.contentText = STR.STR_QML_806
                            _info1.open();
                        }
                    }
                    onSecurityQuestionClosed: {
                        if (GlobalData.serviceIndex === itemsSetting._PLATFORM_KEY_CO_SIGNING_POLICIES) {
                            _Security.close()
                        }
                    }
                    onThereNoAssistedWalletAlert: {
                        _info1.contentText = STR.STR_QML_839
                        _info1.open();
                    }
                }
                Connections {
                    target: ServiceSetting
                    onKeyCoSigningChanged: {
                        _currency.loadCompleted()
                        _interal.loadCompleted()
                    }
                }
                Column {
                    anchors.fill: parent
                    anchors.margins: 24
                    spacing: 24
                    QText {
                        font.family: "Lato"
                        font.pixelSize: 28
                        color: "#031F2B"
                        font.weight: Font.Bold
                        text: STR.STR_QML_803
                    }
                    Column {
                        id: honey_badger
                        visible: ClientController.user.plan_slug === "honey_badger"
                        spacing: 16
                        Item {
                            width: 627
                            height: 52
                            Column {
                                anchors.fill: parent
                                spacing: 4
                                QLato {
                                    width: parent.width
                                    height: 20
                                    font.weight: Font.Bold
                                    text: STR.STR_QML_799
                                }
                                QLato {
                                    width: parent.width
                                    height: 28
                                    text: STR.STR_QML_800
                                }
                            }
                        }
                        Item {
                            width: 627
                            height: 72
                            Row {
                                anchors.fill: parent
                                spacing: 14
                                QTextInputBoxTypeB {
                                    id: co_signing
                                    label: STR.STR_QML_801
                                    boxWidth: 306
                                    boxHeight: 48
                                    isValid: true
                                    textInputted: ServiceSetting.keyCoSigning.limit
                                    onTextInputtedChanged: {
                                        if(!co_signing.isValid){
                                            co_signing.isValid = true
                                            co_signing.errorText = ""
                                        }
                                        co_signing.showError = false;
                                    }
                                    function limit() {
                                        return co_signing.textInputted
                                    }
                                    property bool dataChanged: ServiceSetting.keyCoSigning.limit !== co_signing.textInputted
                                }
                                QComboBox {
                                    id: _currency
                                    anchors.bottom: co_signing.bottom
                                    width: 128
                                    height: 48
                                    displayText: ServiceSetting.keyCoSigning.currency
                                    model: list()
                                    textRole: "displayName"
                                    function list() {
                                        var ls = []
                                        ls.push({ displayName: qsTr("%1").arg(AppSetting.currency) })
                                        ls.push({ displayName: qsTr("BTC") })
                                        ls.push({ displayName: qsTr("sat") })
                                        return ls
                                    }
                                    function getIndex(displayName) {
                                        var lys = list()
                                        for (var i = 0; i < lys.length; i++) {
                                            if (lys[i].displayName === displayName) {
                                                return i;
                                            }
                                        }
                                        return -1;
                                    }
                                    function currency() {
                                        return currentIndex < 0 ? AppSetting.currency : _currency.model[currentIndex].displayName
                                    }
                                    function loadCompleted() {
                                        _currency.currentIndex = _currency.getIndex(ServiceSetting.keyCoSigning.currency)
                                    }
                                    property bool dataChanged: _currency.getIndex(ServiceSetting.keyCoSigning.currency) !== _currency.currentIndex
                                }
                                QComboBox {
                                    id: _interal
                                    anchors.bottom: co_signing.bottom
                                    currentIndex: _interal.getIndex(currentText)
                                    width: 165
                                    height: 48
                                    model:  [
                                        { displayName: qsTr("DAILY") },
                                        { displayName: qsTr("WEEKLY") },
                                        { displayName: qsTr("MONTHLY") },
                                        { displayName: qsTr("YEARLY") }
                                    ]
                                    textRole: "displayName"
                                    function getIndex(displayName) {
                                        var lys = ["DAILY", "WEEKLY", "MONTHLY", "YEARLY"]
                                        for (var i = 0; i < lys.length; i++) {
                                            if (lys[i] === displayName) {
                                                return i;
                                            }
                                        }
                                        return 0;
                                    }
                                    function interval() {
                                        return _interal.model[currentIndex].displayName
                                    }
                                    property bool dataChanged: _interal.getIndex(ServiceSetting.keyCoSigning.interval) !== _interal.currentIndex
                                    function loadCompleted() {
                                        _interal.currentIndex = _interal.getIndex(ServiceSetting.keyCoSigning.interval)
                                    }
                                }
                            }
                        }
                    }
                    Rectangle {
                        visible: honey_badger.visible
                        width: 627
                        height: 1
                        color: "#EAEAEA"
                    }
                    Column {
                        spacing: 16
                        Item {
                            width: 627
                            height: 52
                            Column {
                                anchors.fill: parent
                                spacing: 4
                                QLato {
                                    width: parent.width
                                    height: 20
                                    font.weight: Font.Bold
                                    text: STR.STR_QML_792
                                }
                                QLato {
                                    width: parent.width
                                    height: 28
                                    text: STR.STR_QML_793
                                }
                            }
                        }
                        Item {
                            width: 627
                            height: 48
                            QLato {
                                width: parent.width
                                height: 28
                                anchors.verticalCenter: parent.verticalCenter
                                text: STR.STR_QML_794
                            }
                            QSwitchTypeB {
                                id: autoBroadcast
                                width: 84
                                height: 48
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    right: parent.right
                                }
                                switchOn: ServiceSetting.keyCoSigning.auto_broadcast_transaction
                                function auto(){
                                    return autoBroadcast.switchOn
                                }
                                property bool dataChanged: ServiceSetting.keyCoSigning.auto_broadcast_transaction !== autoBroadcast.switchOn
                            }
                        }
                        Item {
                            width: 627
                            height: 64
                            Column {
                                anchors.fill: parent
                                spacing: 4
                                QLato {
                                    width: parent.width
                                    height: 28
                                    text: STR.STR_QML_795
                                }
                                QLato {
                                    width: 531
                                    height: 32
                                    font.pixelSize: 12
                                    font.weight: Font.Medium
                                    color: "#595959"
                                    text: STR.STR_QML_796
                                    lineHeight: 16
                                    lineHeightMode: Text.FixedHeight
                                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                                }
                            }
                            QSwitchTypeB {
                                id: _enableCoSigning
                                width: 84
                                height: 48
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    right: parent.right
                                }
                                switchOn: AppSetting.enableCoSigning
                                function applySettings(){
                                    AppSetting.enableCoSigning = _enableCoSigning.switchOn
                                }
                                property bool dataChanged: AppSetting.enableCoSigning !== _enableCoSigning.switchOn
                            }
                        }
                        Item {
                            width: 627
                            height: 72
                            visible: _enableCoSigning.switchOn
                            Row {
                                anchors.fill: parent
                                spacing: 15
                                QTextInputBoxTypeB {
                                    id: _hours
                                    label: STR.STR_QML_797
                                    boxWidth: 306
                                    boxHeight: 48
                                    isValid: true
                                    validator: IntValidator {bottom: 0; top: 167}
                                    textInputted: ServiceSetting.keyCoSigning.hours
                                    onTextInputtedChanged: {
                                        if(!_hours.isValid){
                                            _hours.isValid = true
                                            _hours.errorText = ""
                                        }
                                        _hours.showError = false;
                                        if (parseInt(textInputted) > 167) {
                                            AppModel.showToast(-1,
                                                           STR.STR_QML_808,
                                                           EWARNING.ERROR_MSG,
                                                           "");
                                        }
                                    }
                                    function hours() {
                                        return _hours.textInputted
                                    }
                                    property bool dataChanged:ServiceSetting.keyCoSigning.hours !== _hours.textInputted
                                }
                                QTextInputBoxTypeB {
                                    id: _minutes
                                    label: STR.STR_QML_798
                                    boxWidth: 306
                                    boxHeight: 48
                                    isValid: true
                                    validator: RegExpValidator { regExp: /[0-5]?[0-9]/; }
                                    textInputted: ServiceSetting.keyCoSigning.minutes
                                    onTextInputtedChanged: {
                                        if(!_minutes.isValid){
                                            _minutes.isValid = true
                                            _minutes.errorText = ""
                                        }
                                        _minutes.showError = false;
                                    }
                                    function minutes() {
                                        return _minutes.textInputted
                                    }
                                    property bool dataChanged:ServiceSetting.keyCoSigning.minutes !== _minutes.textInputted
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    id: normalRect
                    width: 894
                    height: 80
                    anchors{
                        right: parent.right
                        bottom: parent.bottom
                    }
                    layer.enabled: true
                    layer.effect: DropShadow {
                        radius: 4
                        samples: radius * 2
                        source: normalRect
                        color: Qt.rgba(0, 0, 0, 0.5)
                    }
                    Row {
                        anchors{
                            right: parent.right
                            rightMargin: 24
                            bottom: parent.bottom
                            bottomMargin: 16
                        }
                        spacing: 12
                        layoutDirection: Qt.RightToLeft
                        QTextButton {
                            id: _save
                            width: 214
                            height: 48
                            label.text: STR.STR_QML_804
                            label.font.pixelSize: 16
                            type: eTypeE
                            enabled: anyChanged
                            onButtonClicked: {
                                if (parseInt(_hours.textInputted) > 167) {
                                    AppModel.showToast(-1,
                                                   STR.STR_QML_808,
                                                   EWARNING.ERROR_MSG,
                                                   "");
                                    return;
                                }
                                _enableCoSigning.applySettings()
                                if (_enableCoSigning.switchOn) {
                                    var ls = {
                                        "limit" : co_signing.limit(),
                                        "currency" : _currency.currency(),
                                        "interval" : _interal.interval(),
                                        "auto_broadcast_transaction" : autoBroadcast.auto(),
                                        "hours" : _hours.hours(),
                                        "minutes" : _minutes.minutes()
                                    }
                                    ServiceSetting.keyCoSigning = ls
                                } else {
                                    var ls1 = {
                                        "limit" : co_signing.limit(),
                                        "currency" : _currency.currency(),
                                        "interval" : _interal.interval(),
                                        "auto_broadcast_transaction" : autoBroadcast.auto(),
                                    }
                                    ServiceSetting.keyCoSigning = ls1
                                }
                                QMLHandle.sendEvent(EVT.EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST)
                            }

                        }
                        QTextButton {
                            width: 148
                            height: 48
                            label.text: STR.STR_QML_805
                            label.font.pixelSize: 16
                            type: eTypeF
                            onButtonClicked: {

                            }
                        }
                    }
                }
            }
        }
    }
    property bool anyChanged: co_signing.dataChanged ||
                             _currency.dataChanged ||
                             _interal.dataChanged ||
                             _hours.dataChanged ||
                             _minutes.dataChanged ||
                             _enableCoSigning.dataChanged ||
                             autoBroadcast.dataChanged

    QPopupInfoVertical {
        id: _InfoVer
        property string link:""
        property bool linkTop: false
        lwidths:[252,252]
        title: STR.STR_QML_339
        contentText: STR.STR_QML_684
        labels: [STR.STR_QML_341,STR.STR_QML_683]
        onConfirmNo: {
            close()
            if (!linkTop)
                Qt.openUrlExternally(link)
        }
        onConfirmYes: {
            close()
            if (linkTop)
                Qt.openUrlExternally(link)
        }
    }
    QPopupInfo{
        id:_info1
        contentText: STR.STR_QML_752
        onGotItClicked: {
            close()
        }
    }
    QConfirmYesNoPopup{
        id:_confirm
        property var fClose
        contentText: STR.STR_QML_919
        onConfirmNo: close()
        onConfirmYes: {
            close()
            fClose()
        }
    }
    QPopupEmpty {
        id: _Security
        property var action
        content: QAnswerSecurityQuestion {
            id:_popup
            width: 600
            height: 320 + 36
            onCloseClicked: {
                _Security.close()
                _popup.clearText()
            }
            bottomLeft: Row {
                spacing: 8
                QTextButton {
                    width: 120
                    height: 48
                    label.text: STR.STR_QML_245
                    label.font.pixelSize: 16
                    type: eTypeF
                    onButtonClicked: {
                        prevClicked()
                    }
                }
            }
            onPrevClicked: {
                _Security.close()
                _popup.clearText()
            }

            onNextClicked: {
                _Security.action()
                _popup.clearText()
            }
        }
    }
}
