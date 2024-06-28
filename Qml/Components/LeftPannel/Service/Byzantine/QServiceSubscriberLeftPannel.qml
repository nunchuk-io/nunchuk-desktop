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
import "../../../origins"
import "../../../customizes"
import "../../../customizes/Chats"
import "../../../customizes/Texts"
import "../../../customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Item {
    id: leftpanel
    property bool isPremierUser             : ClientController.user.isByzantineUserPremier
    property bool isContainsPremierWallet   : AppModel.walletList.isContainsPremier

    readonly property var option_map_premier:[
        {screen:_EMERGENCY,                          visible: hasEmergency,                 enable:false,  title:STR.STR_QML_702, icon: "qrc:/Images/Images/emergency-light.svg"    ,action: function(){ return true; } },
        {screen:_EMERGENCY_LOCKDOWN,                 visible: hasWalletLockdown,            enable:true,   title:STR.STR_QML_697, icon: ""                                          ,action: function(){ return emergencyLockdown(); } },
        {screen:_KEY_RECOVERY,                       visible: hasKeyRecovery,               enable:true,   title:STR.STR_QML_698, icon: ""                                          ,action: function(){ return true;} },
        {screen:_YOUR_SUBSCRIPTION,                  visible: hasYourSubsciption(),           enable:false,  title:STR.STR_QML_699, icon: "qrc:/Images/Images/subscription-light.svg" ,action: function(){ return true;} },
        {screen:_PLATFORM_KEY_CO_SIGNING_POLICIES,   visible: hasPlatformKeyCoSign,         enable:true,   title:STR.STR_QML_738, icon: ""                                          ,action: function(){ return platformKeyCosigningPolicies() } },
        {screen:_GET_ADDITIONAL_WALLETS,             visible: hasGetAdditionalWallet,       enable:true,   title:STR.STR_QML_707, icon: ""                                          ,action: function(){ return getAdditionalWallets() } },
        {screen:_REPLACE_KEY_IN_AN_ASSISTED_WALLET,  visible: true,                         enable:true,   title:STR.STR_QML_1352,icon: ""                                          ,action: function(){ return replace_keys() } },
        {screen:_ROLL_OVER_TO_A_NEW_ASSISTED_WALLET, visible: hasRollOverNewAssistedWallet, enable:true,   title:STR.STR_QML_1019,icon: ""                                          ,action: function(){ return rollOverNewAssistedWallet() } },
        {screen:_MANAGE_SUBSCRIPTION,                visible: hasManageSubscription,        enable:true,   title:STR.STR_QML_682, icon: ""                                          ,action: function(){ return manageSubscription() }}
    ]
    readonly property var option_map_default: [
        {screen:_EMERGENCY,                          visible: hasEmergency,                 enable:false,  title:STR.STR_QML_702, icon: "qrc:/Images/Images/emergency-light.svg"    ,action: function(){ return true; } },
        {screen:_EMERGENCY_LOCKDOWN,                 visible: hasWalletLockdown,            enable:true,   title:STR.STR_QML_697, icon: ""                                          ,action: function(){ return emergencyLockdown(); } },
        {screen:_KEY_RECOVERY,                       visible: hasKeyRecovery,               enable:true,   title:STR.STR_QML_698, icon: ""                                          ,action: function(){ return true;} },
        {screen:_INHERITANCE_PLANING,                visible: hasInheritancePlaning,        enable:false,  title:STR.STR_QML_736, icon: "qrc:/Images/Images/inheritance-light.svg"  ,action: function(){ return true;} },
        {screen:_VIEW_INHERITANCE_PLANING,           visible: hasViewInheritancePlan,       enable:true,   title:STR.STR_QML_875, icon: ""                                          ,action: function(){ return viewInheritancePlan() } },
        {screen:_CLAIM_AN_INHERITANCE,               visible: hasClaimAnInheritance,        enable:true,   title:STR.STR_QML_737, icon: ""                                          ,action: function(){ ServiceSetting.servicesTag.clearClaimAnInheritance(); return true; }},
        {screen:_YOUR_SUBSCRIPTION,                  visible: hasYourSubsciption(),           enable:false,  title:STR.STR_QML_699, icon: "qrc:/Images/Images/subscription-light.svg" ,action: function(){ return true;} },
        {screen:_PLATFORM_KEY_CO_SIGNING_POLICIES,   visible: hasPlatformKeyCoSign,         enable:true,   title:STR.STR_QML_738, icon: ""                                          ,action: function(){ return platformKeyCosigningPolicies() } },
        {screen:_GET_ADDITIONAL_WALLETS,             visible: hasGetAdditionalWallet,       enable:true,   title:STR.STR_QML_707, icon: ""                                          ,action: function(){ return getAdditionalWallets() } },
        {screen:_REPLACE_KEY_IN_AN_ASSISTED_WALLET,  visible: true,                         enable:true,   title:STR.STR_QML_1352,icon: ""                                          ,action: function(){ return replace_keys() } },
        {screen:_ORDER_NEW_HARDWARE,                 visible: hasOrderNewHardware,          enable:true,   title:STR.STR_QML_700, icon: ""                                          ,action: function(){ return orderNewHardware() } },
        {screen:_ROLL_OVER_TO_A_NEW_ASSISTED_WALLET, visible: hasRollOverNewAssistedWallet, enable:true,   title:STR.STR_QML_1019,icon: ""                                          ,action: function(){ return rollOverNewAssistedWallet() } },
        {screen:_MANAGE_SUBSCRIPTION,                visible: hasManageSubscription,        enable:true,   title:STR.STR_QML_682, icon: ""                                          ,action: function(){ return manageSubscription() } }
    ]
    readonly property var option_map: isPremierUser || isContainsPremierWallet ? option_map_premier : option_map_default

    property var itemOption: option_map.find(function(e) { if (e.screen === ServiceSetting.optionIndex) return true; else return false } )
    property var highestPermissionAccount: GroupWallet.highestPermissionAccount
    property string role: highestPermissionAccount.role
    property bool hasPro: highestPermissionAccount.hasPro
    property bool hasEmergency: (role === "KEYHOLDER" || role === "KEYHOLDER_LIMITED" || role === "MASTER" || role === "ADMIN")
    property bool hasKeyRecovery: role === "KEYHOLDER" || role === "KEYHOLDER_LIMITED" || role === "MASTER" || role === "ADMIN"
    property bool hasInheritancePlaning: hasPro && (role === "KEYHOLDER" || role === "MASTER" || role === "ADMIN")
    property bool hasViewInheritancePlan: ServiceSetting.servicesTag.listInheritantPlans.length > 0 && hasPro && (role === "KEYHOLDER" || role === "MASTER" || role === "ADMIN")
    property bool hasClaimAnInheritance: hasPro && (role === "KEYHOLDER" || role === "KEYHOLDER_LIMITED" || role === "MASTER" || role === "ADMIN")
    property bool hasPlatformKeyCoSign: ServiceSetting.servicesTag.listPolicy.length > 0 && hasPro && (role === "KEYHOLDER" ||  role === "MASTER" || role === "ADMIN")
    property bool hasGetAdditionalWallet: role === "MASTER"
    property bool hasOrderNewHardware: role === "KEYHOLDER" || role === "KEYHOLDER_LIMITED" || role === "MASTER" || role === "ADMIN"
    property bool hasRollOverNewAssistedWallet: role === "MASTER"
    property bool hasManageSubscription: role === "MASTER"

    function hasYourSubsciption() {
        if (isPremierUser || isContainsPremierWallet) {
            return hasPlatformKeyCoSign || hasGetAdditionalWallet || hasRollOverNewAssistedWallet || hasManageSubscription
        }
        else {
            return hasPlatformKeyCoSign || hasGetAdditionalWallet || hasOrderNewHardware || hasRollOverNewAssistedWallet || hasManageSubscription
        }
    }

    Component.onCompleted: {
        var access = option_map.find(function(e) { if (e.screen === ServiceSetting.optionIndex) return true; else return false } )
        var isOK = access.visible && access.enable
        if (!isOK) {
            var index = option_map.find(function(e) { if (e.visible && e.enable) return true; else return false } ).screen
            ServiceSetting.optionIndex = index
        }
    }

    function emergencyLockdown() {
        ServiceSetting.clearWalletInfo()
        return true;
    }

    function platformKeyCosigningPolicies(){
        ServiceSetting.clearWalletInfo()
        return true;
    }

    function orderNewHardware() {
        _InfoVer.link = "https://nunchuk.io/hardware-replacement";
        _InfoVer.contentText = STR.STR_QML_735;
        _InfoVer.labels = [STR.STR_QML_341,STR.STR_QML_683];
        _InfoVer.open();
        return false;
    }

    function getAdditionalWallets(){
        ServiceSetting.servicesTag.additionalGetWalletConfig()
        _InfoVer.link = "https://nunchuk.io/my-plan";
        var remainCount = ServiceSetting.servicesTag.walletConfig.remaining_wallet_count !== undefined ? ServiceSetting.servicesTag.walletConfig.remaining_wallet_count : 0;
         _InfoVer.contentText = (remainCount === 1 ? STR.STR_QML_842.arg(remainCount) : STR.STR_QML_841.arg(remainCount));
        _InfoVer.labels = [STR.STR_QML_341,STR.STR_QML_683];
        _InfoVer.open();
        return false;
    }

    function rollOverNewAssistedWallet(){
        _InfoVer.link = "https://nunchuk.io/hardware-replacement";
        _InfoVer.contentText = STR.STR_QML_735;
        _InfoVer.labels = [STR.STR_QML_341,STR.STR_QML_683];
        _InfoVer.open();
        return false;
    }

    function replace_keys() {
        _info1.title = STR.STR_QML_339
        _info1.contentText = STR.STR_QML_1347
        _info1.open()
        return true;
    }

    function viewInheritancePlan() {
        ServiceSetting.clearWalletInfo()
        return true;
    }

    function manageSubscription() {
        _InfoVer.link = "https://nunchuk.io/my-plan";
        _InfoVer.contentText = STR.STR_QML_684;
        _InfoVer.labels = [STR.STR_QML_341,STR.STR_QML_683];
        _InfoVer.open();
        return false;
    }

    Column {
        anchors.fill: parent
        anchors.topMargin: 24
        spacing: 12
        Column {
            spacing: 0
            Repeater {
                model: option_map.length
                Rectangle {
                    id: _item
                    width: leftpanel.width
                    height: 48
                    visible: option_map[index].visible
                    enabled: option_map[index].enable
                    color: itemOption.screen === option_map[index].screen ? Qt.rgba(255, 255, 255, 0.3) : itemSetingMouse.containsMouse ?  Qt.rgba(255, 255, 255, 0.1) : "transparent"
                    Row {
                        spacing: 8
                        anchors.fill: parent
                        Item {
                            width: 8;
                            height: parent.height
                            Rectangle {
                                width: 4
                                height: parent.height
                                color: itemOption.screen === option_map[index].screen ? "#F6D65D" : "transparent"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        QIcon {
                            id:_icon
                            iconSize: 24
                            anchors.verticalCenter: parent.verticalCenter
                            source: option_map[index].icon
                            visible: source != ""
                        }
                        QText {
                            anchors.verticalCenter: parent.verticalCenter
                            text: option_map[index].title
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
                            var _item = option_map[index]
                            if (_item.action()) {
                                ServiceSetting.optionIndex = _item.screen
                            }
                        }
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
