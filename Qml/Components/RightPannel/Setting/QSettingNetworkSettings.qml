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
import "./../../origins"
import "./../../customizes"
import "./../../customizes/Chats"
import "./../../customizes/Texts"
import "./../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    readonly property bool anyChanged: servernodeselection.anyChanged
                              || devselection.anyChanged
                              || corerpcinputdata.anyChanged
                              || torproxytitle.anyChanged
                              || setonCert.anyChanged
                              || certPath.anyChanged
                              || torinputdata.anyChanged

    function applySettings(){
        servernodeselection.applySettings()
        devselection.applySettings()
        corerpcinputdata.applySettings()
        torproxytitle.applySettings()
        setonCert.applySettings()
        certPath.applySettings()
        torinputdata.applySettings()
    }

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
                Connections {
                    target: AppSetting
                    onMainnetServerSelected: {
                        mainnetsever.text.textOutput = url
                    }
                }

                spacing: 10
                QRadioButtonTypeB {
                    id: mainnetsever
                    width: 627
                    selected: (devselection.primaryServer === NUNCHUCKTYPE.MAIN)
                    text.placeholder.text: STR.STR_QML_173
                    text.textOutput: devselection.mainnetServer
                    showArrow: true
                    onButtonClicked: { devselection.primaryServer = NUNCHUCKTYPE.MAIN }
                    onButtonArrowClicked: {
                        QMLHandle.sendEvent(EVT.EVT_SELECT_SERVER_REQUEST)
                    }
                }
                QRadioButtonTypeB {
                    id: testnetsever
                    width: 627
                    selected: (devselection.primaryServer === NUNCHUCKTYPE.TESTNET)
                    text.placeholder.text: STR.STR_QML_174
                    text.textOutput: devselection.testnetServer
                    onButtonClicked: { devselection.primaryServer = NUNCHUCKTYPE.TESTNET }
                }
                QRadioButtonTypeB {
                    id: signetsever
                    width: 627
                    selected: (devselection.primaryServer === NUNCHUCKTYPE.SIGNET)
                    text.placeholder.text: STR.STR_QML_583
                    text.textOutput: devselection.signetServer
                    onButtonClicked: { devselection.primaryServer = NUNCHUCKTYPE.SIGNET }
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

    QWarningBg {
        icon: "qrc:/Images/Images/info-60px.png"
        txt.text: STR.STR_QML_1301.arg("https://github.com/nunchuk-io/resources/tree/main/docs/connection-guide")
        txt.textFormat: Text.RichText
        txt.font.weight: Font.Bold
        anchors{
            bottom: normalRect.top
            bottomMargin: 16
            left: parent.left
            leftMargin: 26
            right: parent.right
            rightMargin: 24
        }
    }
    Rectangle {
        id: normalRect
        height: 80
        anchors{
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        Row {
            anchors{
                right: parent.right
                rightMargin: 24
                bottom: parent.bottom
                bottomMargin: 16
            }
            spacing: 12
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
    FileDialog {
        id: openFileDialog
        flags: FolderDialog.ReadOnly
        onAccepted: {
            certPathText.textInputted = AppModel.getFilePath(openFileDialog.file)
        }
    }
}
