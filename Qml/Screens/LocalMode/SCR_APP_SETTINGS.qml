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
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: rootSettings
    width: QAPP_DEVICE_WIDTH
    height: QAPP_DEVICE_HEIGHT
    property bool anyChanged: unitselection.anyChanged
                              || servernodeselection.anyChanged
                              || devselection.anyChanged
                              || corerpcinputdata.anyChanged
                              || torproxytitle.anyChanged
                              || setonCert.anyChanged
                              || certPath.anyChanged
                              || torinputdata.anyChanged
                              || hwidrivertitle.anyChanged
                              || hwipath.anyChanged

    function applySettings(){
        unitselection.applySettings()
        servernodeselection.applySettings()
        devselection.applySettings()
        corerpcinputdata.applySettings()
        torproxytitle.applySettings()
        setonCert.applySettings()
        certPath.applySettings()
        torinputdata.applySettings()
        hwidrivertitle.applySettings()
        hwipath.applySettings()
    }
    function closeRequest(to) {
        if(rootSettings.anyChanged){
            modelConfirmUpdateSetting.requestTo = to
            modelConfirmUpdateSetting.open()
            return false
        }
        else{
            if(NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex){
                QMLHandle.sendEvent(EVT.EVT_APP_SETTING_BACK_TO_ONLINE_MODE)
            }
            else{
                QMLHandle.sendEvent(EVT.EVT_APP_SETTING_BACK_REQUEST)
            }
        }
        return true
    }
    Row {
        anchors.fill: parent
        Item {
            id: pannel_left
            width: 377
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
            QMenuBar {
                width: 72
                height: parent.height
                anchors.verticalCenter: parent.verticalCenter
                currentIndex: 3
                labels: [
                    STR.STR_QML_460,
                    STR.STR_QML_461,
                    ClientController.isNunchukLoggedIn ? STR.STR_QML_462 : STR.STR_QML_533
                ]
                enables: [
                    ClientController.isNunchukLoggedIn,
                    false,
                    true
                ]
                onLocalModeRequest: {
                    if(closeRequest(NUNCHUCKTYPE.WALLET_TAB) && NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex){
                        if(0 === AppModel.nunchukMode){
                            QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE)
                        }
                        else{
                            QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINE_HWL)
                        }
                    }
                }
                onOnlineModeRequest: {
                    if(closeRequest(NUNCHUCKTYPE.CHAT_TAB) && NUNCHUCKTYPE.WALLET_TAB === AppModel.tabIndex){
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                    }
                }
                onOpenSettingRequest: {

                }
                onSignoutRequest: {
                    ClientController.requestSignout()
                }
                onUpdateProfileRequest: {
                    QMLHandle.sendEvent(EVT.EVT_ROOT_UPDATE_PROFILE)
                }
                onSigninRequest: {
                    if(rootSettings.anyChanged){
                        modelConfirmUpdateSetting.requestTo = NUNCHUCKTYPE.CHAT_TAB
                        modelConfirmUpdateSetting.open()
                    }
                    else{
                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                    }
                }
            }
            Item {
                id: settingpanel
                width: 304
                height: parent.height
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                Column {
                    anchors.fill: parent
                    anchors.topMargin: 24
                    spacing: 12
                    Item{
                        width: 304
                        height: ClientController.isNunchukLoggedIn ? userPanel.childrenRect.height : guestPanel.childrenRect.height
                        QGuestSettingPanel{
                            id:guestPanel
                            visible: !ClientController.isNunchukLoggedIn
                        }
                        QUserSettingPanel{
                            id:userPanel
                            visible: ClientController.isNunchukLoggedIn
                            onClickManageSubscription: {
                                _ManagerSub.open()
                            }
                        }
                    }
                    Column {
                        spacing: 0
                        Repeater {
                            id: itemsSetting
                            readonly property int _DISPLAY_UNIT_BTC: 0
                            readonly property int _ACCOUNT_SETTINGS: 1
                            readonly property int _NETWORK_SETTINGS: 2
                            readonly property int _HARDWARE_DRIVER: 3
                            readonly property int _DATABASE_ENCRYTION: 4
                            readonly property int _DEVELOPER_SETTINGS: 5
                            readonly property int _ABOUT: 6
                            readonly property var setting_map: [
                                {screen:_DISPLAY_UNIT_BTC,      title:STR.STR_QML_513, icon: "qrc:/Images/Images/display-unit-light.svg"        },
                                {screen:_ACCOUNT_SETTINGS,      title:STR.STR_QML_537, icon: "qrc:/Images/Images/accounts-settings-light.svg"   },
                                {screen:_NETWORK_SETTINGS,      title:STR.STR_QML_514, icon: "qrc:/Images/Images/network-settings-light.svg"    },
                                {screen:_HARDWARE_DRIVER,       title:STR.STR_QML_515, icon: "qrc:/Images/Images/hardware-driver-light.svg"     },
                                {screen:_DATABASE_ENCRYTION,    title:STR.STR_QML_530, icon: "qrc:/Images/Images/change-password-light.svg"     },
                                {screen:_DEVELOPER_SETTINGS,    title:STR.STR_QML_588, icon: "qrc:/Images/Images/developer-settings-light.svg"  },
                                {screen:_ABOUT,                 title:STR.STR_QML_518, icon: "qrc:/Images/Images/info-filled-light.svg"         }
                            ]

                            model: setting_map.length
                            Rectangle {
                                width: settingpanel.width
                                height: 48
                                color: GlobalData.settingIndex === index ? Qt.rgba(255, 255, 255, 0.3) : itemSetingMouse.containsMouse ?  Qt.rgba(255, 255, 255, 0.1) : "transparent"
                                Row {
                                    spacing: 8
                                    anchors.fill: parent
                                    Rectangle {
                                        width: 8
                                        height: parent.height
                                        color: GlobalData.settingIndex === index ? "#F6D65D" : "transparent"
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    Item {width: 4; height: parent.height}
                                    QImage {
                                        width: 24
                                        height: 24
                                        anchors.verticalCenter: parent.verticalCenter
                                        source: itemsSetting.setting_map[index].icon
                                    }
                                    QText {
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: itemsSetting.setting_map[index].title
                                        color: "#FFFFFF"
                                        font.family: "Lato"
                                        font.pixelSize: 16
                                    }
                                }
                                MouseArea {
                                    id: itemSetingMouse
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onClicked: {
                                        GlobalData.settingIndex = itemsSetting.setting_map[index].screen
                                    }
                                }
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
                anchors.fill: parent
                visible: GlobalData.settingIndex === itemsSetting._DISPLAY_UNIT_BTC
                enabled: visible
                Column {
                    id: unitselection
                    anchors.fill: parent
                    anchors.margins: 24
                    spacing: 24
                    property int unitSelected: AppSetting.unit
                    property bool enableFixedPrecision: AppSetting.enableFixedPrecision
                    QText {
                        font.family: "Lato"
                        font.pixelSize: 28
                        font.weight: Font.Bold
                        text: STR.STR_QML_519
                    }
                    QRadioButtonTypeA {
                        id: btc
                        width: 627
                        height: 24
                        label: "BTC"
                        fontFamily: "Lato"
                        fontPixelSize: 16
                        fontWeight: btc.selected ? Font.ExtraBold : Font.DemiBold
                        selected: (unitselection.unitSelected === NUNCHUCKTYPE.BTC)
                        onButtonClicked: {
                            unitselection.unitSelected = NUNCHUCKTYPE.BTC
                            unitselection.applySettingImmediately()
                        }
                    }
                    Row {
                        spacing: 8
                        width: 627
                        enabled: (unitselection.unitSelected === NUNCHUCKTYPE.BTC)
                        QText {
                            width: parent.width - icon.width - 8
                            height: 16
                            anchors.verticalCenter: parent.verticalCenter
                            text: STR.STR_QML_520
                            font.family: "Lato"
                            font.pixelSize: 12
                            font.weight: unitselection.enableFixedPrecision && btc.selected ? Font.Bold : Font.Normal
                        }
                        QImage {
                            id: icon
                            width: 24
                            height: 24
                            anchors.verticalCenter: parent.verticalCenter
                            source: unitselection.enableFixedPrecision ? "qrc:/Images/Images/checkbox-checked-dark.svg" : "qrc:/Images/Images/checkbox-dark.svg"
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    unitselection.enableFixedPrecision = !unitselection.enableFixedPrecision
                                    unitselection.applySettingImmediately()
                                }
                            }
                        }
                    }
                    Rectangle { width: 627; height: 1; color: "#EAEAEA"}
                    QRadioButtonTypeA {
                        id: satoshi
                        width: 627
                        height: 24
                        label: "Satoshi (1 BTC = 100,000,000 sat)"
                        fontFamily: "Lato"
                        fontWeight: satoshi.selected ? Font.ExtraBold : Font.DemiBold
                        fontPixelSize: 16
                        selected: (unitselection.unitSelected === NUNCHUCKTYPE.SATOSHI)
                        onButtonClicked: {
                            unitselection.unitSelected = NUNCHUCKTYPE.SATOSHI
                            unitselection.applySettingImmediately()
                        }
                    }
                    property bool anyChanged: (unitselection.unitSelected !== AppSetting.unit) || (unitselection.enableFixedPrecision !== AppSetting.enableFixedPrecision)
                    function applySettings(){
                        AppSetting.unit = unitselection.unitSelected
                        AppSetting.enableFixedPrecision = unitselection.enableFixedPrecision
                    }
                    function applySettingImmediately() {
                        if(unitselection.anyChanged) { unitselection.applySettings() }
                    }
                }
            }
            Item {
                anchors.fill: parent
                visible: GlobalData.settingIndex === itemsSetting._NETWORK_SETTINGS
                enabled: visible
                Flickable{
                    width: 651; height: 910
                    contentWidth: 651; contentHeight: 946
                    interactive: _netCol.childrenRect.height > 890
                    Column {
                        id: _netCol
                        anchors.fill: parent
                        anchors.margins: 24
                        spacing: 10
                        QText {
                            font.family: "Lato"
                            font.pixelSize: 28
                            font.weight: Font.Bold
                            text: STR.STR_QML_521
                        }
                        Row {
                            spacing: 0
                            QText {
                                width: 627-84
                                text: STR.STR_QML_522
                                color: "#000000"
                                font.pixelSize: 16
                                font.family: "Lato"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QSwitchTypeB {
                                id: electrumSwitcher
                                width: 84
                                height: 48
                                switchOn: !AppSetting.enableCoreRPC
                                anchors.verticalCenter: parent.verticalCenter
                                onSwitchOnChanged: {
                                    corerpcSwitcher.switchOn = !electrumSwitcher.switchOn
                                }
                            }
                        }
                        Column {
                            id: devselection
                            visible: electrumSwitcher.switchOn
                            enabled: visible
                            property int primaryServer: AppSetting.primaryServer
                            property string mainnetServer: AppSetting.mainnetServer
                            property string testnetServer: AppSetting.testnetServer
                            property string signetServer: AppSetting.signetServer

                            spacing: 10
                            QRadioButtonTypeB {
                                id: mainnetsever
                                width: 627
                                selected: (devselection.primaryServer === NUNCHUCKTYPE.MAIN)
                                text.placeholder.text: STR.STR_QML_173
                                text.textOutput: devselection.mainnetServer
                                onButtonClicked: { devselection.primaryServer = NUNCHUCKTYPE.MAIN }
                                onButtonResetClicked: { mainnetsever.text.textOutput = MAINNET_SERVER}
                            }
                            QRadioButtonTypeB {
                                id: testnetsever
                                width: 627
                                selected: (devselection.primaryServer === NUNCHUCKTYPE.TESTNET)
                                text.placeholder.text: STR.STR_QML_174
                                text.textOutput: devselection.testnetServer
                                onButtonClicked: { devselection.primaryServer = NUNCHUCKTYPE.TESTNET }
                                onButtonResetClicked: { testnetsever.text.textOutput = TESTNET_SERVER}
                            }
                            QRadioButtonTypeB {
                                id: signetsever
                                width: 627
                                selected: (devselection.primaryServer === NUNCHUCKTYPE.SIGNET)
                                text.placeholder.text: STR.STR_QML_583
                                text.textOutput: devselection.signetServer
                                onButtonClicked: { devselection.primaryServer = NUNCHUCKTYPE.SIGNET }
                                onButtonResetClicked: { signetsever.text.textOutput = SIGNET_SERVER}
                                visible: SIGNET_SERVER != ""
                            }
                            property bool anyChanged: (AppSetting.primaryServer !== devselection.primaryServer)
                                                      || (AppSetting.mainnetServer !== mainnetsever.text.textOutput)
                                                      || (AppSetting.testnetServer !== testnetsever.text.textOutput)
                                                      || (AppSetting.signetServer !== signetsever.text.textOutput)
                            function applySettings(){
                                AppSetting.primaryServer = devselection.primaryServer
                                AppSetting.mainnetServer = mainnetsever.text.textOutput
                                AppSetting.testnetServer = testnetsever.text.textOutput
                                AppSetting.signetServer = signetsever.text.textOutput
                            }
                        }
                        Rectangle { width: 627; height: 1; color: "#EAEAEA"}
                        Row {
                            id: _streamEnable
                            visible: signetsever.visible && signetsever.selected
                            spacing: 0
                            QText {
                                width: 627-84
                                text: STR.STR_QML_585
                                color: "#000000"
                                font.pixelSize: 16
                                font.family: "Lato"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QSwitchTypeB {
                                id: streamswitch
                                width: 84
                                height: 48
                                switchOn: AppSetting.enableSignetStream
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            property bool anyChanged: (AppSetting.enableSignetStream !== streamswitch.switchOn)
                            function applySettings(){
                                AppSetting.enableSignetStream = streamswitch.switchOn
                            }
                        }
                        Row {
                            id:_streamLink
                            spacing: 16
                            visible: streamswitch.switchOn && _streamEnable.visible
                            QTextInputBoxTypeB {
                                id: streamLinkText
                                label: ""
                                textInputted: AppSetting.signetStream
                                boxWidth: 627-16
                                boxHeight: 48
                                isValid: true
                            }
                            property bool anyChanged: (AppSetting.signetStream !== streamLinkText.textInputted)
                            function applySettings(){
                                AppSetting.signetStream = streamLinkText.textInputted
                            }
                        }
                        Rectangle { width: 627; height: 1; color: "#EAEAEA"; visible: _streamEnable.visible }
                        Row {
                            id: servernodeselection
                            spacing: 0
                            QText {
                                width: 627-84
                                text: STR.STR_QML_172
                                color: "#000000"
                                font.pixelSize: 16
                                font.family: "Lato"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QSwitchTypeB {
                                id: corerpcSwitcher
                                width: 84
                                height: 48
                                switchOn: AppSetting.enableCoreRPC
                                anchors.verticalCenter: parent.verticalCenter
                                onSwitchOnChanged: {
                                    electrumSwitcher.switchOn = !corerpcSwitcher.switchOn
                                }
                            }
                            property bool anyChanged: (corerpcSwitcher.switchOn !== AppSetting.enableCoreRPC)
                            function applySettings(){
                                AppSetting.enableCoreRPC = corerpcSwitcher.switchOn
                            }
                        }
                        Column {
                            id: corerpcinputdata
                            visible: corerpcSwitcher.switchOn
                            enabled: visible
                            spacing: 10
                            QText {
                                id: waringtit
                                width: 627
                                text: STR.STR_QML_182
                                wrapMode: Text.WordWrap
                                font.family: "Montserrat"
                                font.pixelSize: 10
                                color: "#031F2B"
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignJustify
                            }
                            Row {
                                spacing: 10
                                QTextInputBoxTypeB {
                                    id: corerpcaddress
                                    label: STR.STR_QML_183
                                    boxWidth: 607*2/3
                                    boxHeight: 48
                                    isValid: true
                                    textInputted: AppSetting.coreRPCAddress
                                }
                                QTextInputBoxTypeB {
                                    id: corerpcport
                                    label: STR.STR_QML_184
                                    boxWidth: 607*1/3
                                    boxHeight: 48
                                    isValid: true
                                    textInputted: AppSetting.coreRPCPort
                                }
                            }
                            Row {
                                spacing: 10
                                QTextInputBoxTypeB {
                                    id: corerpcproxyname
                                    label: STR.STR_QML_185
                                    boxWidth: 607*1/2
                                    boxHeight: 48
                                    isValid: true
                                    textInputted: AppSetting.coreRPCName
                                }
                                QTextInputBoxTypeB {
                                    id: corerpcproxypassword
                                    label: STR.STR_QML_186
                                    boxWidth: 607*1/2
                                    boxHeight: 48
                                    isValid: true
                                    textInputted: AppSetting.coreRPCPassword
                                }
                            }
                            Row {
                                spacing: 16
                                QTextInputBoxTypeB {
                                    id: inputHeightRescan
                                    label: STR.STR_QML_187
                                    boxWidth: 627-16-83
                                    boxHeight: 48
                                    isValid: true
                                }
                                QTextButton {
                                    width: 83
                                    height: 48
                                    label.text: STR.STR_QML_189
                                    label.font.pixelSize: 16
                                    type: eTypeB
                                    anchors.bottom: inputHeightRescan.bottom
                                    anchors.bottomMargin: 4
                                    onButtonClicked: {
                                        QMLHandle.sendEvent(EVT.EVT_APP_SETTING_REQUEST_RESCAN, inputHeightRescan.textInputted)
                                        QMLHandle.sendEvent(EVT.EVT_APP_SETTING_BACK_REQUEST)
                                    }
                                }
                            }
                            property bool anyChanged: (AppSetting.coreRPCAddress !== corerpcaddress.textInputted)
                                                      || ((AppSetting.coreRPCPort).toString() !== corerpcport.textInputted)
                                                      || (AppSetting.coreRPCName !== corerpcproxyname.textInputted)
                                                      || (AppSetting.coreRPCPassword !== corerpcproxypassword.textInputted)
                            function applySettings(){
                                AppSetting.coreRPCAddress = corerpcaddress.textInputted
                                AppSetting.coreRPCPort = corerpcport.textInputted
                                AppSetting.coreRPCName = corerpcproxyname.textInputted
                                AppSetting.coreRPCPassword = corerpcproxypassword.textInputted
                            }
                        }
                        Rectangle { width: 627; height: 1; color: "#EAEAEA"}
                        Row {
                            id: setonCert
                            spacing: 0
                            QText {
                                width: 627-84
                                text: STR.STR_QML_175
                                color: "#000000"
                                font.pixelSize: 16
                                font.family: "Lato"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QSwitchTypeB {
                                id: certswitch
                                width: 84
                                height: 48
                                switchOn: AppSetting.enableCertificateFile
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            property bool anyChanged: (AppSetting.enableCertificateFile !== certswitch.switchOn)
                            function applySettings(){
                                AppSetting.enableCertificateFile = certswitch.switchOn
                            }
                        }
                        Row {
                            id: certPath
                            spacing: 16
                            visible: certswitch.switchOn
                            QTextInputBoxTypeB {
                                id: certPathText
                                label: STR.STR_QML_523
                                boxWidth: 627-16-139
                                boxHeight: 48
                                isValid: true
                            }
                            QTextButton {
                                width: 139
                                height: 48
                                label.text: STR.STR_QML_524
                                label.font.pixelSize: 16
                                type: eTypeB
                                anchors.bottom: certPathText.bottom
                                anchors.bottomMargin: 4
                                onButtonClicked: {
                                    openFileDialog.open()
                                }
                            }
                            property bool anyChanged: (AppSetting.certificateFile !== certPathText.textInputted)
                            function applySettings(){
                                AppSetting.certificateFile = certPathText.textInputted
                            }
                        }
                        Rectangle { width: 627; height: 1; color: "#EAEAEA"}
                        Row {
                            id: torproxytitle
                            spacing: 0
                            QText {
                                width: 627-84
                                text: STR.STR_QML_177
                                color: "#000000"
                                font.pixelSize: 16
                                font.family: "Lato"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QSwitchTypeB {
                                id: torSwitcher
                                width: 84
                                height: 48
                                switchOn: AppSetting.enableTorProxy
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            property bool anyChanged: (AppSetting.enableTorProxy !== torSwitcher.switchOn)
                            function applySettings(){
                                AppSetting.enableTorProxy = torSwitcher.switchOn
                            }
                        }
                        Column {
                            id: torinputdata
                            spacing: 10
                            visible: torSwitcher.switchOn
                            Row {
                                spacing: 10
                                QTextInputBoxTypeB {
                                    id: toraddress
                                    label: STR.STR_QML_178
                                    boxWidth: 607*2/3
                                    boxHeight: 48
                                    isValid: true
                                    textInputted: AppSetting.torProxyAddress
                                }
                                QTextInputBoxTypeB {
                                    id: torport
                                    label: STR.STR_QML_179
                                    boxWidth: 607*1/3
                                    boxHeight: 48
                                    isValid: true
                                    textInputted: AppSetting.torProxyPort
                                }
                            }
                            Row {
                                spacing: 10
                                QTextInputBoxTypeB {
                                    id: torproxyname
                                    label: STR.STR_QML_180
                                    boxWidth: 607*1/2
                                    boxHeight: 48
                                    isValid: true
                                    textInputted: AppSetting.torProxyName
                                }
                                QTextInputBoxTypeB {
                                    id: torproxypassword
                                    label: STR.STR_QML_181
                                    boxWidth: 607*1/2
                                    boxHeight: 48
                                    isValid: true
                                    textInputted: AppSetting.torProxyPassword
                                }
                            }
                            property bool anyChanged: (AppSetting.torProxyAddress !== toraddress.textInputted)
                                                      || ((AppSetting.torProxyPort).toString() !== torport.textInputted)
                                                      || (AppSetting.torProxyName !== torproxyname.textInputted)
                                                      || (AppSetting.torProxyPassword !== torproxypassword.textInputted)
                            function applySettings(){
                                AppSetting.torProxyAddress = toraddress.textInputted
                                AppSetting.torProxyPort = torport.textInputted
                                AppSetting.torProxyName = torproxyname.textInputted
                                AppSetting.torProxyPassword = torproxypassword.textInputted
                            }
                        }
                        Item {
                            width: 627
                            height: 48
                            Row {
                                height: 48
                                spacing: 12
                                anchors.right: parent.right
                                QTextButton {
                                    width: 268
                                    height: 48
                                    label.text: STR.STR_QML_529
                                    label.font.pixelSize: 16
                                    label.font.family: "Lato"
                                    type: eTypeB
                                    onButtonClicked: {
                                        _netCol.resetNetworkSettings()
                                        _netCol.saveNetworkSettings()
                                    }
                                }
                                QTextButton {
                                    width: 189
                                    height: 48
                                    label.text: STR.STR_QML_528
                                    label.font.pixelSize: 16
                                    label.font.family: "Lato"
                                    type: eTypeE
                                    enabled: servernodeselection.anyChanged
                                             || devselection.anyChanged
                                             || corerpcinputdata.anyChanged
                                             || torproxytitle.anyChanged
                                             || torinputdata.anyChanged
                                             || setonCert.anyChanged
                                             || certPath.anyChanged
                                             || _streamLink.anyChanged
                                             || _streamEnable.anyChanged
                                    onButtonClicked: {
                                        _netCol.saveNetworkSettings()
                                    }
                                }
                            }
                        }

                        function saveNetworkSettings() {
                            if(servernodeselection.anyChanged) { servernodeselection.applySettings() }
                            if(devselection.anyChanged){ devselection.applySettings() }
                            if(corerpcinputdata.anyChanged){ corerpcinputdata.applySettings() }
                            if(torproxytitle.anyChanged){ torproxytitle.applySettings() }
                            if(torinputdata.anyChanged){ torinputdata.applySettings() }
                            if(setonCert.anyChanged){ setonCert.applySettings() }
                            if(certPath.anyChanged){ certPath.applySettings() }
                            if(_streamLink.anyChanged){_streamLink.applySettings() }
                            if(_streamEnable.anyChanged){_streamEnable.applySettings() }
                            if(AppModel.updateSettingRestartRequired()){
                                modelRestartApp.open()
                            }
                        }

                        function resetNetworkSettings() {
                            corerpcSwitcher.switchOn = false
                            devselection.primaryServer = NUNCHUCKTYPE.MAIN
                            mainnetsever.text.textOutput = "mainnet.nunchuk.io:51001"
                            testnetsever.text.textOutput = "testnet.nunchuk.io:50001"
                            signetsever.text.textOutput  = "signet.nunchuk.io:50002"
                            streamswitch.switchOn = false
                            streamLinkText.textInputted = "https://explorer.bc-2.jp/"

                            corerpcaddress.textInputted = "127.0.0.1"
                            corerpcport.textInputted = "8332"
                            corerpcproxyname.textInputted = ""
                            corerpcproxypassword.textInputted = ""

                            certswitch.switchOn = false
                            certPathText.textInputted = ""

                            torSwitcher.switchOn = false
                            toraddress.textInputted = "127.0.0.1"
                            torport.textInputted = "9050"
                            torproxyname.textInputted = ""
                            torproxypassword.textInputted = ""
                        }
                    }
                }
            }
            Item {
                anchors.fill: parent
                visible: GlobalData.settingIndex === itemsSetting._HARDWARE_DRIVER
                enabled: visible
                Column {
                    anchors.fill: parent
                    anchors.margins: 24
                    spacing: 10
                    QText {
                        font.family: "Lato"
                        font.pixelSize: 28
                        font.weight: Font.Bold
                        text: STR.STR_QML_515
                    }
                    Row {
                        id: hwidrivertitle
                        spacing: 0
                        QText {
                            width: 627-84
                            text: STR.STR_QML_168
                            color: "#000000"
                            font.pixelSize: 16
                            font.family: "Lato"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QSwitchTypeB {
                            id: hwidriverswitch
                            width: 84
                            height: 48
                            switchOn: AppSetting.enableCustomizeHWIDriver
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        property bool anyChanged: (AppSetting.enableCustomizeHWIDriver !== hwidriverswitch.switchOn)
                        function applySettings(){
                            AppSetting.enableCustomizeHWIDriver = hwidriverswitch.switchOn
                        }
                        function applySettingImmediately() {
                            if(hwidrivertitle.anyChanged) { hwidrivertitle.applySettings() }
                            if(AppModel.updateSettingRestartRequired()){
                                modelRestartApp.open()
                            }
                        }
                    }
                    QTextInputBoxTypeB {
                        id: hwipath
                        label: STR.STR_QML_169
                        boxWidth: 627
                        boxHeight: 48
                        isValid: true
                        visible: hwidriverswitch.switchOn
                        textInputted: AppSetting.hwiPath
                        property bool anyChanged: (AppSetting.hwiPath !== hwipath.textInputted)
                        function applySettings(){
                            AppSetting.hwiPath = hwipath.textInputted
                        }
                    }
                    Item {
                        width: 627
                        height: 48
                        visible: hwidriverswitch.switchOn
                        QTextButton {
                            width: 189
                            height: 48
                            anchors.right: parent.right
                            label.text: STR.STR_QML_534
                            label.font.pixelSize: 16
                            label.font.family: "Lato"
                            type: eTypeE
                            enabled: hwidrivertitle.anyChanged
                            onButtonClicked: {
                                if(hwidrivertitle.anyChanged) { hwidrivertitle.applySettings() }
                                if(hwipath.anyChanged) { hwipath.applySettings() }
                                if(AppModel.updateSettingRestartRequired()){
                                    modelRestartApp.open()
                                }
                            }
                        }
                    }
                }
            }
            Item {
                anchors.fill: parent
                visible: GlobalData.settingIndex === itemsSetting._ACCOUNT_SETTINGS
                enabled: visible
                Flickable{
                    width: 651; height: 910
                    contentWidth: 651; contentHeight: _netAcc.childrenRect.height
                    visible: ClientController.isNunchukLoggedIn === true
                    interactive: _netAcc.childrenRect.height > 890
                    Column{
                        id:_netAcc
                        anchors{
                            top:parent.top
                            topMargin: 24
                        }
                        spacing: 1
                        QText {
                            anchors{
                                left: parent.left
                                leftMargin: 24
                            }
                            font.family: "Lato"
                            font.pixelSize: 28
                            font.weight: Font.Bold
                            text: STR.STR_QML_537
                        }
                        Item{
                            visible: ClientController.user.isPrimaryKey
                            width: 627
                            height: 48 + 24
                            anchors{
                                left: parent.left
                                leftMargin: 24
                            }
                            QText {
                                anchors{
                                    bottom: parent.bottom
                                    bottomMargin: 10
                                }
                                font.family: "Lato"
                                font.pixelSize: 20
                                font.weight: Font.Bold
                                text: STR.STR_QML_641
                            }
                            QTextButton {
                                id: _replaceBtn
                                anchors{
                                    bottom: parent.bottom
                                    bottomMargin: 0
                                }
                                width: 200
                                height: 48
                                label.text: STR.STR_QML_657
                                label.font.pixelSize: 16
                                type: eTypeB
                                anchors.right: parent.right
                                enabled: AppModel.primaryKey
                                onButtonClicked: {
                                    QMLHandle.sendEvent(EVT.EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST)
                                }
                            }
                        }

                        Item{
                            id: changePassword
                            visible: !ClientController.user.isPrimaryKey
                            width: 651
                            height: 28+ 20 + currentpassword.height + newpassword.height + confirmPassword.height + updateBtn.height + 10 * 5 + 24*2
                            Column {
                                anchors.fill: parent
                                anchors.margins: 24
                                spacing: 10
                                QText {
                                    font.family: "Lato"
                                    font.pixelSize: 20
                                    font.weight: Font.Bold
                                    text: STR.STR_QML_525
                                }
                                QTextInputBoxTypeB {
                                    id: currentpassword
                                    label: STR.STR_QML_526
                                    isPassword: true
                                    boxWidth: 627
                                    boxHeight: 48
                                    isValid: true
                                    onTextInputtedChanged: {
                                        if(!currentpassword.isValid){
                                            currentpassword.isValid = true
                                            currentpassword.errorText = ""
                                        }
                                        currentpassword.showError = false
                                    }
                                }
                                QTextInputBoxTypeB {
                                    id: newpassword
                                    label: STR.STR_QML_402
                                    isPassword: true
                                    boxWidth: 627
                                    boxHeight: 48
                                    isValid: true
                                    onTextInputtedChanged: {
                                        if(!newpassword.isValid){
                                            newpassword.isValid = true
                                            newpassword.errorText = ""
                                        }
                                        newpassword.showError = false
                                    }
                                }
                                QTextInputBoxTypeB {
                                    id: confirmPassword
                                    label: STR.STR_QML_403
                                    isPassword: true
                                    boxWidth: 627
                                    boxHeight: 48
                                    isValid: textInputted === "" || textInputted === newpassword.textInputted
                                    errorText: STR.STR_QML_404
                                }
                                QTextButton {
                                    id: updateBtn
                                    width: 200
                                    height: 48
                                    label.text: STR.STR_QML_527
                                    label.font.pixelSize: 16
                                    type: eTypeE
                                    anchors.right: parent.right
                                    onButtonClicked: {
                                        if(newpassword.textInputted === currentpassword.textInputted ||
                                                newpassword.textInputted === "" ||
                                                currentpassword.textInputted === ""){
                                            if(newpassword.textInputted === ""){
                                                newpassword.isValid = false
                                                newpassword.errorText = STR.STR_QML_396
                                                newpassword.showError = true
                                            }
                                            if(currentpassword.textInputted === ""){
                                                currentpassword.isValid = false
                                                currentpassword.errorText = STR.STR_QML_396
                                                currentpassword.showError = true
                                            }
                                            else if(newpassword.textInputted === currentpassword.textInputted){
                                                newpassword.isValid = false
                                                currentpassword.isValid = false
                                                currentpassword.errorText = STR.STR_QML_406
                                                currentpassword.showError = true
                                            }
                                        }
                                        else{
                                            var requestBody = {
                                                "oldPassword" : currentpassword.textInputted,
                                                "newPassword" : newpassword.textInputted,
                                            };
                                            QMLHandle.sendEvent(EVT.EVT_SETTING_ACCOUNT_CHANGE_PASSWORD, requestBody)
                                        }
                                    }
                                }
                            }
                            function processingChangePasswordResult(https_code, error_code, error_msg){
                                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                                    AppModel.showToast(0,
                                                       STR.STR_QML_545,
                                                       EWARNING.SUCCESS_MSG,
                                                       STR.STR_QML_545);
                                }
                                else{
                                    if(https_code === DRACO_CODE.SUCCESSFULL){
                                        currentpassword.isValid = false
                                        currentpassword.errorText = error_msg
                                        currentpassword.showError = true
                                    }
                                }
                            }
                        }
                        Rectangle {
                            width: 627
                            height: 1
                            color: "#EAEAEA"
                            anchors.left: parent.left
                            anchors.leftMargin: 24
                            visible: false
                        }
                        Item {
                            id: loggedInDevice
                            width: 651
                            height: loggedBox.height + 74

                            Column {
                                anchors.fill: parent
                                anchors.margins: 24
                                spacing: 14
                                Item{
                                    width: 627
                                    height: 20
                                    QText {
                                        anchors.left: parent.left
                                        font.family: "Lato"
                                        font.pixelSize: 20
                                        font.weight: Font.Bold
                                        text: STR.STR_QML_517
                                    }
                                    QText {

                                        anchors.right: parent.right
                                        font.family: "Lato"
                                        font.pixelSize: 16
                                        font.weight: Font.Bold
                                        color: "#CF4018"
                                        text: STR.STR_QML_547
                                        scale: btnSignOutAll.containsMouse ? 1.1 : 1.0
                                        MouseArea {
                                            id: btnSignOutAll
                                            hoverEnabled: true
                                            cursorShape: Qt.PointingHandCursor
                                            anchors.fill: parent
                                            onClicked: {
                                                loggedBox.typePopup = loggedBox._SignOutAll
                                                _loggerDevice.contentText = STR.STR_QML_578
                                                _loggerDevice.open()
                                            }
                                        }
                                    }
                                }

                                QLoggedInDevicesBox{
                                    id: loggedBox
                                    width: 627
                                    boxHeight: 48*5
                                    onSignOutClicked: {
                                        loggedBox.device_id = id
                                        loggedBox.device_name = name
                                        loggedBox.typePopup = _SignOutSingle
                                        _loggerDevice.contentText = STR.STR_QML_576.arg(name)
                                        _loggerDevice.open()
                                    }
                                    onMarkClicked: {
                                        loggedBox.device_id = id
                                        loggedBox.device_name = name
                                        loggedBox.typePopup = _MarkAsCompromised
                                        _loggerDevice.contentText = STR.STR_QML_577.arg(name)
                                        _loggerDevice.open()

                                    }
                                }
                            }

                            function loggedInDeviceChanged(https_code, error_code, error_msg){
                                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                                    var ret = ""
                                    switch(loggedBox.typePopup){
                                    case loggedBox._SignOutSingle:
                                        ret = STR.STR_QML_581.arg(loggedBox.device_name)
                                        break
                                    case loggedBox._SignOutAll:
                                        ret = STR.STR_QML_580
                                        break
                                    case loggedBox._MarkAsCompromised:
                                        ret = STR.STR_QML_579.arg(loggedBox.device_name)
                                        break
                                    default:break
                                    }
                                    if(ret !== ""){
                                        AppModel.showToast(0,
                                                           ret,
                                                           EWARNING.SUCCESS_MSG,
                                                           ret);
                                    }


                                }
                                else{
                                    if(https_code === DRACO_CODE.SUCCESSFULL){

                                    }
                                }
                            }
                        }
                        Rectangle {
                            width: 627
                            height: 1
                            color: "#EAEAEA"
                            anchors.left: parent.left
                            anchors.leftMargin: 24
                        }

                        Item{
                            width: 627
                            height: 25*2 + 48
                            anchors.left: parent.left
                            anchors.leftMargin: 24
                            QText {
                                width: 309
                                height: 28
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                                horizontalAlignment: Text.AlignLeft
                                text: STR.STR_QML_593
                                font.family: "Lato"
                                font.pixelSize: 20
                                font.weight: Font.Bold
                                color: "#031F2B"
                            }

                            QSwitchTypeB {
                                id: multiSyncDevice
                                anchors.right: parent.right
                                width: 84
                                height: 48
                                switchOn: AppSetting.enableMultiDeviceSync
                                anchors.verticalCenter: parent.verticalCenter
                                onSwitchOnChanged: {
                                    AppSetting.enableMultiDeviceSync = multiSyncDevice.switchOn
                                }
                            }
                        }

                        Rectangle {
                            width: 627
                            height: 1
                            color: "#EAEAEA"
                            anchors.left: parent.left
                            anchors.leftMargin: 24
                        }
                        Item {
                            id: deleteAccount
                            width: 651
                            height: 124
                            Column {
                                anchors.fill: parent
                                anchors.margins: 24
                                spacing: 16
                                QText {
                                    font.family: "Lato"
                                    font.pixelSize: 20
                                    font.weight: Font.Bold
                                    text: STR.STR_QML_539
                                }
                                Item{
                                    width: parent.width
                                    height: 28
                                    QText {
                                        width: 309
                                        height: 28
                                        anchors.left: parent.left
                                        horizontalAlignment: Text.AlignLeft
                                        text: STR.STR_QML_540
                                        font.family: "Lato"
                                        font.pixelSize: 16
                                        color: "#031F2B"
                                    }
                                    QText {
                                        width: 108
                                        height: 20
                                        anchors{
                                            right: parent.right
                                            verticalCenter: parent.verticalCenter
                                        }
                                        horizontalAlignment: Text.AlignLeft
                                        text: STR.STR_QML_539
                                        font.family: "Lato"
                                        font.pixelSize: 16
                                        color: "#CF4018"
                                        enabled: ClientController.user.isPrimaryKey ? AppModel.primaryKey : true
                                        opacity: enabled ? 1.0 : 0.3
                                        MouseArea {
                                            id: btnMouseDeleteccount
                                            hoverEnabled: true
                                            cursorShape: Qt.PointingHandCursor
                                            anchors.fill: parent
                                            onClicked: {
                                                if(ClientController.user.isPrimaryKey){
                                                    accountConfirmDeletePrimaryKey.open()
                                                }
                                                else{
                                                    accountConfirmDelete.open()
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                Item {
                    id: addAccount
                    width: 651
                    height: 124
                    anchors.top:parent.top
                    visible: ClientController.isNunchukLoggedIn === false
                    Column {
                        anchors.fill: parent
                        anchors.margins: 24
                        spacing: 24
                        QText {
                            font.family: "Lato"
                            font.pixelSize: 28
                            font.weight: Font.Bold
                            text: STR.STR_QML_537
                        }
                        QText {
                            font.family: "Lato"
                            font.pixelSize: 20
                            font.weight: Font.Bold
                            text: STR.STR_QML_538
                        }
                        Item{
                            width: parent.width
                            height: 28
                            QText {
                                width: 309
                                height: 28
                                anchors.left: parent.left
                                horizontalAlignment: Text.AlignLeft
                                text: STR.STR_QML_541
                                font.family: "Lato"
                                font.pixelSize: 16
                                color: "#031F2B"
                            }
                            QText {
                                width: 108
                                height: 20
                                anchors{
                                    right: parent.right
                                    verticalCenter: parent.verticalCenter
                                }
                                horizontalAlignment: Text.AlignLeft
                                text: STR.STR_QML_538
                                font.family: "Lato"
                                font.pixelSize: 16
                                color: "#CF4018"
                                MouseArea {
                                    id: btnMouseAddccount
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    anchors.fill: parent
                                    onClicked: {
                                        QMLHandle.sendEvent(EVT.EVT_LOGIN_MATRIX_REQUEST)
                                    }
                                }
                            }
                        }
                    }
                }

            }
            Item {
                anchors.fill: parent
                visible: GlobalData.settingIndex === itemsSetting._DATABASE_ENCRYTION
                enabled: visible
                onVisibleChanged: { if(!visible){ dbencriptionswitch.switchOn = AppSetting.enableDBEncryption } }
                Column {
                    id: dbencryptionComp
                    anchors.fill: parent
                    anchors.margins: 24
                    spacing: 10
                    QText {
                        font.family: "Lato"
                        font.pixelSize: 28
                        font.weight: Font.Bold
                        text: STR.STR_QML_530
                    }
                    Row {
                        spacing: 0
                        QText {
                            width: 627-84
                            text: STR.STR_QML_530
                            color: "#000000"
                            font.pixelSize: 16
                            font.family: "Lato"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QSwitchTypeB {
                            id: dbencriptionswitch
                            width: 84
                            height: 48
                            switchOn: AppSetting.enableDBEncryption
                            anchors.verticalCenter: parent.verticalCenter
                            property bool anyChanged: (AppSetting.enableDBEncryption !== dbencriptionswitch.switchOn)
                            onSwitchOnChanged: {
                                if(!switchOn){
                                    inputpassphrase.textOutput = ""
                                    confirmpassphrase.textOutput = ""
                                    pleasewait.open()
                                    setpassphraseTime.start()
                                }
                            }
                            function applySettings(){
                                AppSetting.enableDBEncryption = dbencriptionswitch.switchOn
                            }
                        }
                    }
                    Column {
                        anchors.margins: 24
                        spacing: 24
                        visible: dbencriptionswitch.switchOn
                        QTextInputBox {
                            id: inputpassphrase
                            width: 627
                            border.color: "#C9DEF1"
                            placeholder.text: STR.STR_QML_162
                            echoMode: showpass.visiblity ? TextInput.Normal : TextInput.Password
                            errorText.visible: !validInput
                            validInput: (inputpassphrase.length === 0 ) || (inputpassphrase.length >= 8)
                            color: "#FFFFFF"
                            errorText.text: STR.STR_QML_163
                            errorText.color: "#E02247"
                            QImage {
                                id: showpass
                                width: 24
                                height: 24
                                property bool visiblity: false
                                source: showpass.visiblity ? "qrc:/Images/Images/visibility-dark.svg":"qrc:/Images/Images/visibility-off-dark.svg"
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 17
                                visible: (inputpassphrase.textOutput !== "")
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: showpass.visiblity =! showpass.visiblity
                                }
                            }
                        }
                        QTextInputBox {
                            id: confirmpassphrase
                            width: 627
                            border.color: "#C9DEF1"
                            placeholder.text: STR.STR_QML_166
                            echoMode: showconfirmpass.visiblity ? TextInput.Normal : TextInput.Password
                            color: "#FFFFFF"
                            errorText.visible: !validInput
                            validInput: ((confirmpassphrase.length === 0 ) || (confirmpassphrase.length >= 8)) && (inputpassphrase.textOutput === confirmpassphrase.textOutput)
                            errorText.text: {
                                if(((confirmpassphrase.length === 0 ) || (confirmpassphrase.length >= 8)) && (inputpassphrase.textOutput !== confirmpassphrase.textOutput)) {return STR.STR_QML_531}
                                else { return STR.STR_QML_163 }
                            }
                            errorText.color: "#E02247"
                            QImage {
                                id: showconfirmpass
                                width: 24
                                height: 24
                                property bool visiblity: false
                                source: showconfirmpass.visiblity ? "qrc:/Images/Images/visibility.png":"qrc:/Images/Images/visibility-off.png"
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 17
                                visible: (confirmpassphrase.textOutput !== "")
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: showconfirmpass.visiblity =! showconfirmpass.visiblity
                                }
                            }
                        }
                        Item {
                            width: 627
                            height: 48
                            QTextButton {
                                id: changepassbtn
                                width: 162
                                label.text: STR.STR_QML_167
                                label.font.pixelSize: 14
                                type: eTypeB
                                anchors.right: parent.right
                                enabled: (inputpassphrase.length >= 8) && (confirmpassphrase.length >= 8) &&
                                         (inputpassphrase.textOutput === confirmpassphrase.textOutput)
                                onButtonClicked: {
                                    pleasewait.open()
                                    setpassphraseTime.start()
                                }
                            }
                        }
                    }
                    Timer {
                        id: setpassphraseTime
                        interval: 1000
                        onTriggered: {
                            pleasewait.close()
                            if(AppModel.enableDatabaseEncryption(inputpassphrase.textOutput)){
                                if(dbencriptionswitch.anyChanged) { dbencriptionswitch.applySettings() }
                            }
                            else{
                                dbencriptionswitch.switchOn = AppSetting.enableDBEncryption
                            }
                        }
                    }
                }
            }
            Item {
                anchors.fill: parent
                visible: GlobalData.settingIndex === itemsSetting._DEVELOPER_SETTINGS
                enabled: visible
                Column {
                    id: developer_setting
                    anchors.fill: parent
                    anchors.margins: 24
                    spacing: 10
                    QText {
                        font.family: "Lato"
                        font.pixelSize: 28
                        font.weight: Font.Bold
                        text: STR.STR_QML_588
                    }
                    Row {
                        spacing: 0
                        QText {
                            width: 627-84
                            text: STR.STR_QML_589
                            color: "#000000"
                            font.pixelSize: 16
                            font.family: "Lato"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QSwitchTypeB {
                            id: developerswitch
                            width: 84
                            height: 48
                            switchOn: AppSetting.enableDebugMode
                            anchors.verticalCenter: parent.verticalCenter
                            property bool anyChanged: (AppSetting.enableDebugMode !== developerswitch.switchOn)
                            onSwitchOnChanged: {
                                applySettings()
                            }
                            function applySettings(){
                                AppSetting.enableDebugMode = developerswitch.switchOn
                            }
                        }
                    }

                }
            }
            Item {
                anchors.fill: parent
                visible: GlobalData.settingIndex === itemsSetting._ABOUT
                enabled: visible
                Column {
                    anchors.fill: parent
                    anchors.margins: 24
                    spacing: 24
                    Item {
                        height: 50
                        width: parent.width
                    }
                    QImage {
                        width: 240
                        height: 96
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "qrc:/Images/Images/logo-vertical-light.svg"
                    }
                    QText {
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.family: "Lato"
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        text: qsTr("Version %1").arg(qapplicationVersion)
                    }
                    Row {
                        spacing: 48
                        anchors.horizontalCenter: parent.horizontalCenter
                        MouseArea {
                            width: 96
                            height: 80
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            property string twitterLink: "https://twitter.com/nunchuk_io"
                            onClicked: {
                                Qt.openUrlExternally(twitterLink)
                            }
                            Column {
                                anchors.fill: parent
                                spacing: 12
                                QImage {
                                    width: 48
                                    height: 48
                                    source: "qrc:/Images/Images/twt.png"
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }
                                QText {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                    text: qsTr("@nunchuk_io")
                                }
                            }
                        }
                        MouseArea {
                            width: 96
                            height: 80
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            property string contactLink: "https://mail.google.com/mail/?view=cm&fs=1&tf=1&to=contact@nunchuk.io"
                            onClicked: {
                                Qt.openUrlExternally(contactLink)
                            }
                            Column {
                                anchors.fill: parent
                                spacing: 12
                                QImage {
                                    width: 48
                                    height: 48
                                    source: "qrc:/Images/Images/email.png"
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }
                                QText {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                    text: qsTr("support@nunchuk.io")
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    QConfirmDeletingTypeA {
        id: accountConfirmDelete
        modalWidth: 488
        modalHeight: 317
        label: STR.STR_QML_024
        content: STR.STR_QML_542
        leftBtnLabel: STR.STR_QML_432
        rightBtnLabel: STR.STR_QML_433
        onDeleteRequest: {
            QMLHandle.sendEvent(EVT.EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST,ClientController.user.isPrimaryKey)
            if(!ClientController.user.isPrimaryKey){
                accountConfirmCodeDelete.open()
            }
        }
    }

    QConfirmDeletingTypeB {
        id: accountConfirmDeletePrimaryKey
        modalWidth: 320
        modalHeight: 386
        label: STR.STR_QML_024
        content_1: STR.STR_QML_672
        content_2: STR.STR_QML_673
        leftBtnLabel: STR.STR_QML_035
        rightBtnLabel: STR.STR_QML_427
        onDeleteRequest: {
            QMLHandle.sendEvent(EVT.EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST,ClientController.user.isPrimaryKey)
            accountConfirmDeletePrimaryKey.close()
        }
    }

    QConfirmCodeDeletion {
        id: accountConfirmCodeDelete
        modalWidth: 600
        modalHeight: 416
        label: STR.STR_QML_539
        content: STR.STR_QML_544.arg(ClientController.user.email)
        leftBtnLabel: STR.STR_QML_245
        rightBtnLabel: STR.STR_QML_543
        placeholderText: ""
        onDeleteRequest: {
            if(Draco.sendDeleteConfirmation(emailCode.textInputted)){
                accountConfirmCodeDelete.close()
                ClientController.requestSignout()
            }else{
                emailCode.isValid = false
                errorConfirm.open()
            }
        }
    }

    QPopupInfo{
        id: errorConfirm
        title: STR.STR_QML_594
        contentText: STR.STR_QML_595
        btnLabel: STR.STR_QML_596
        coverColor: Qt.rgba(255, 255, 255, 0)
        onGotItClicked: {
            close()
        }
    }

    Popup {
        id: pleasewait
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        Rectangle {
            id: pleasewaitContent
            width: 340
            height: 56
            color: "#FFFFFF"
            radius: 8
            anchors.centerIn: parent
            QText {
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                text: STR.STR_QML_190
                color: "#031F2B"
                font.weight: Font.DemiBold
                font.family: "Montserrat"
                font.pixelSize: 14
            }
        }
        DropShadow {
            anchors.fill: pleasewaitContent
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
            source: pleasewaitContent
        }
    }
    // ModelConfirm settting
    Popup {
        id: modelConfirmUpdateSetting
        property int requestTo: -1
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        Rectangle {
            id: modelConfirmUpdateSettingmask
            width: 488
            height: 250
            radius: 8
            color: "#FFFFFF"
            anchors.centerIn: parent
            QText {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 51
                horizontalAlignment: Text.AlignHCenter
                text: STR.STR_QML_191
                color: "#031F2B"
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 24
            }
            QText {
                width: 355
                height: 63
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 91
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                text: STR.STR_QML_192
                font.family: "Lato"
                font.pixelSize: 14
            }
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50
                spacing: 16
                QTextButton {
                    width: 100
                    height: 32
                    label.text: STR.STR_QML_193
                    label.font.pixelSize: 10
                    type: eTypeB
                    radius: 12
                    onButtonClicked: {
                        modelConfirmUpdateSetting.close()
                        if(modelConfirmUpdateSetting.requestTo === AppModel.tabIndex){
                            QMLHandle.sendEvent(EVT.EVT_APP_SETTING_BACK_REQUEST)
                        }
                        else{
                            if(NUNCHUCKTYPE.CHAT_TAB === modelConfirmUpdateSetting.requestTo){
                                QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                            }
                            else {
                                if(0 === AppModel.nunchukMode){
                                    QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE)
                                }
                                else{
                                    QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINE_HWL)
                                }
                            }
                        }
                    }
                }
                QTextButton {
                    width: 100
                    height: 32
                    label.text: STR.STR_QML_194
                    radius: 12
                    label.font.pixelSize: 10
                    type: eTypeA
                    onButtonClicked: {
                        rootSettings.applySettings()
                        modelConfirmUpdateSetting.close()
                        if(AppModel.updateSettingRestartRequired()){
                            modelRestartApp.open()
                        }
                        else{
                            if(modelConfirmUpdateSetting.requestTo === AppModel.tabIndex){
                                QMLHandle.sendEvent(EVT.EVT_APP_SETTING_BACK_REQUEST)
                            }
                            else{
                                if(NUNCHUCKTYPE.CHAT_TAB === modelConfirmUpdateSetting.requestTo){
                                    QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINEMODE)
                                }
                                else {
                                    if(0 === AppModel.nunchukMode){
                                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_LOCALMODE)
                                    }
                                    else{
                                        QMLHandle.sendEvent(EVT.EVT_STARTING_APPLICATION_ONLINE_HWL)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        DropShadow {
            anchors.fill: modelConfirmUpdateSettingmask
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
            source: modelConfirmUpdateSettingmask
        }
    }
    // ModelConfirm restart
    Popup {
        id: modelRestartApp
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        Rectangle {
            id: modelRestartAppmask
            width: 488
            height: 250
            radius: 8
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 488
                    height: 250
                    radius: 8
                }
            }
            QText {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 51
                horizontalAlignment: Text.AlignHCenter
                text: STR.STR_QML_195
                color: "#031F2B"
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 24
            }
            QText {
                width: 355
                height: 63
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 91
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                text: STR.STR_QML_196
                font.family: "Lato"
                font.pixelSize: 14
            }
            QTextButton {
                width: 100
                height: 32
                label.text: STR.STR_QML_197
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50
                label.font.pixelSize: 10
                type: eTypeB
                radius: 12
                onButtonClicked: {
                    modelRestartApp.close()
                    QMLHandle.sendEvent(EVT.EVT_APP_SETTING_REQUEST_RESTART)
                }
            }
        }
        DropShadow {
            anchors.fill: modelRestartAppmask
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
            source: modelRestartAppmask
        }
    }
    FileDialog {
        id: openFileDialog
        flags: FolderDialog.ReadOnly
        onAccepted: {
            certPathText.textInputted = AppModel.getFilePath(openFileDialog.file)
        }
    }
    FolderDialog {
        id: openFolderDialog
        flags: FolderDialog.ReadOnly
        onAccepted: {
            certPathText.textInputted = openFolderDialog.file
        }
    }

    QConfirmYesNoPopup {
        id: _loggerDevice
        contentText: ""
        onConfirmNo: close()
        onConfirmYes: {
            close()
            switch(loggedBox.typePopup){
            case loggedBox._SignOutSingle:
                Draco.signoutDeice(loggedBox.device_id)
                break
            case loggedBox._SignOutAll:
                Draco.signoutAllDeices()
                break
            case loggedBox._MarkAsCompromised:
                Draco.markAsCompromised(loggedBox.device_id)
                break
            default:break
            }
        }
    }

    QPopupInfoVertical {
        id: _ManagerSub
        property string link: "https://nunchuk.io/"
        lwidths:[252,252]
        title: STR.STR_QML_339
        contentText: STR.STR_QML_684
        labels: [STR.STR_QML_341,STR.STR_QML_683]
        onConfirmNo: {
            close()
            Qt.openUrlExternally(link)
        }
        onConfirmYes: {
            close()
        }
    }

    Connections {
        target: Draco

        onChangePasswordResult: {
            changePassword.processingChangePasswordResult(https_code, error_code, error_msg)
        }
        onLoggedInDeviceChanged:{
            loggedInDevice.loggedInDeviceChanged(https_code, error_code, error_msg)
        }
    }
}
