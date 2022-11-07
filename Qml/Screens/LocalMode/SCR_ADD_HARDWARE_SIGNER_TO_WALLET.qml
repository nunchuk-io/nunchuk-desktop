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
import NUNCHUCKTYPE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: rootAddsignerToWallet
    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_101
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK)
        }
        QTextInputBox {
            id: signerName
            mode: eEDIT_MODE
            width: 532
            heightMin: 56
            placeholder.text: STR.STR_QML_102
            maximumLength: 106
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 100
            }
            color: Qt.rgba(255, 255, 255, 0.5)
            border.color: "#C9DEF1"
        }
        TabView {
            id: tabselect
            width: 728
            height: 505
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 197
            }
            Tab {
                title: STR.STR_QML_103
                Item {
                    id: tabmastersigner
                    QImage {
                        id: nodevice
                        visible: !devicelist.visible
                        anchors {
                            left: parent.left
                            top: parent.top
                            topMargin: 16
                        }
                        source: "qrc:/Images/Images/Signer_Level2.png"
                    }
                    QListView {
                        id: devicelist
                        property bool needPin: false
                        visible: devicelist.count
                        width: 342
                        height: Math.min(230, (devicelist.count*44) + ((devicelist.count-1)*8))
                        model: AppModel.deviceList
                        anchors {
                            left: parent.left
                            top: parent.top
                            topMargin: 16
                        }
                        spacing: 8
                        currentIndex: (devicelist.count) == 1 && AppModel.deviceList.containsAddable? 0 : -1
                        clip: true
                        interactive : devicelist.count > 3
                        ScrollBar.vertical: ScrollBar { active: true }
                        delegate: Item {
                            width: 342
                            height: 44
                            Rectangle {
                                id: rect
                                anchors.horizontalCenter: parent.horizontalCenter
                                width: parent.width - 2
                                height: 40
                                color: Qt.rgba(255, 255, 255)
                                layer.enabled: true
                                layer.effect: DropShadow {
                                    source: rect
                                    verticalOffset: 2
                                    radius: 8
                                    samples: 16
                                    color: Qt.rgba(0, 0, 0, 0.15)
                                }
                                Rectangle {
                                    visible: index == devicelist.currentIndex
                                    width: 8
                                    height: parent.height
                                    color: "#F6D65D"
                                }
                                QImage {
                                    anchors {
                                        left: parent.left
                                        leftMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                    source: index == devicelist.currentIndex ? "qrc:/Images/Images/RadioEnabled.png" : "qrc:/Images/Images/RadioDeselected.png"
                                }
                                Column {
                                    width: 290
                                    height: 37
                                    anchors {
                                        left: parent.left
                                        leftMargin: 48
                                        verticalCenter: parent.verticalCenter
                                    }
                                    QText {
                                        width: parent.width
                                        height: 21
                                        font.family: "Montserrat"
                                        font.pixelSize: 14
                                        color: "#031F2B"
                                        font.weight: Font.DemiBold
                                        text: device_type
                                    }
                                    QText {
                                        width: parent.width
                                        height: 16
                                        font.family: "Lato"
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        text: "XFP: " + device_master_fingerprint
                                        font.capitalization: Font.AllUppercase
                                    }
                                }
                            }
                            Rectangle {
                                anchors.fill: parent
                                visible: !device_usable_to_add
                                color: Qt.rgba(255, 255, 255, 0.5)
                                Rectangle {
                                    width: 56
                                    height: 21
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.right: parent.right
                                    anchors.rightMargin: 9
                                    color: "#C9DEF1"
                                    radius: 4
                                    QText {
                                        text: STR.STR_QML_104
                                        font.pixelSize: 10
                                        font.weight: Font.ExtraBold
                                        font.family: "Lato"
                                        color: "#F1FAFE"
                                        anchors.centerIn: parent
                                    }
                                }
                            }
                            MouseArea {
                                anchors.fill: parent
                                enabled: device_usable_to_add
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: { devicelist.currentIndex = index }
                            }
                            Component.onCompleted: {
                                if(0 === devicelist.currentIndex && index === devicelist.currentIndex){
                                    devicelist.currentIndex = index
                                }
                            }
                        }
                    }
                    QRefreshButton {
                        width: 160
                        height: 32
                        label: STR.STR_QML_105
                        fontPixelSize: 14
                        anchors {
                            left: parent.left
                            top:  parent.top
                            topMargin: 32 + (devicelist.count > 0 ? ( devicelist.height) : nodevice.height)
                        }
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH)
                        }
                    }
                    QTextButton {
                        width: 150
                        height: 48
                        label.text: STR.STR_QML_106
                        label.font.pixelSize: 16
                        label.font.family: "Lato"
                        type: eTypeE
                        enabled: (signerName.textOutput !== "") && (devicelist.currentIndex !== -1)
                        anchors {
                            right: parent.right
                            bottom: parent.bottom
                            bottomMargin: 40
                        }
                        onButtonClicked: {
                            if(devicelist.currentIndex !== -1){
                                if(devicelist.currentIndex !== -1){
                                    var masterSignerObj = { "signerNameInputted"    : signerName.textOutput,
                                                            "deviceIndexSelected"   : devicelist.currentIndex};
                                    QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER, masterSignerObj)
                                }
                            }
                        }
                    }
                }
            }
            Tab {
                title: STR.STR_QML_107
                Item {
                    id: tabremoteSigner
                    QText {
                        anchors {
                            left: parent.left
                            top: parent.top
                            topMargin: 24
                        }
                        text: STR.STR_QML_108
                        color: "#323E4A"
                        font.pixelSize: 16
                    }
                    Flickable {
                        id: signerinforInput
                        width: 532
                        height: Math.min(300, contentHeight)
                        flickableDirection: Flickable.VerticalFlick
                        clip: true
                        interactive: signerinforInput.height < signerinforInput.contentHeight
                        contentHeight: xpubOrPubl.height + bip32.height + master_key_fgp.height + 80
                        ScrollBar.vertical: ScrollBar { active: true }
                        anchors {
                            left: parent.left
                            top: parent.top
                            topMargin: 61
                        }
                        QTextInputBox {
                            id: xpubOrPubl
                            width: 532
                            heightMin: 56
                            mode: eEDIT_MODE
                            placeholder.text: AppModel.newWalletInfo.walletEscrow ? STR.STR_QML_123 : STR.STR_QML_109
                            color: Qt.rgba(255, 255, 255, 0.5)
                            border.color: "#C9DEF1"
                            validInput: AppModel.newWalletInfo.walletEscrow ? (AppModel.singleSignerInfo.signerPublickey !== "false")
                                                                            : (AppModel.singleSignerInfo.signerXpub !== "false")
                            errorText.text: STR.STR_QML_110
                            onTypingFinished: AppModel.newWalletInfo.walletEscrow ? (AppModel.singleSignerInfo.signerPublickey = "")
                                                                                  : (AppModel.singleSignerInfo.signerXpub = "")
                        }
                        QText {
                            id: asssitXpub
                            anchors.top: xpubOrPubl.bottom
                            visible: xpubOrPubl.validInput
                            text: STR.STR_QML_124
                            color: "#839096"
                            font.family: "Lato"
                            font.pixelSize: 12
                        }
                        QTextInputBox {
                            id: bip32
                            width: 532
                            heightMin: 56
                            mode: eEDIT_MODE
                            placeholder.text: STR.STR_QML_111 + (bip32.textOutput !== "" ? "" : !bip32.textActiveFocus ? " (e.g. \"m/48h/0h/1h\")" : "")
                            anchors {
                                top: xpubOrPubl.bottom
                                topMargin: 32
                            }
                            color: Qt.rgba(255, 255, 255, 0.5)
                            border.color: "#C9DEF1"
                            validInput: (AppModel.singleSignerInfo.signerDerivationPath !== "false")
                            errorText.text: STR.STR_QML_110
                            onTypingFinished: AppModel.singleSignerInfo.signerDerivationPath = ""
                        }
                        QTextInputBox {
                            id: master_key_fgp
                            width: 532
                            heightMin: 56
                            mode: eEDIT_MODE
                            placeholder.text: STR.STR_QML_112
                            anchors {
                                top: bip32.bottom
                                topMargin: 24
                            }
                            color: Qt.rgba(255, 255, 255, 0.5)
                            border.color: "#C9DEF1"
                            validInput: (AppModel.singleSignerInfo.signerMasterFingerPrint !== "false")
                            errorText.text: STR.STR_QML_110
                            onTypingFinished: AppModel.singleSignerInfo.signerMasterFingerPrint = ""
                        }
                        Connections {
                            target: qrscaner
                            onTagFound: {
                                var jsonstring = "";
                                jsonstring = AppModel.parseQRSigners(qrscaner.tags)
                                if(jsonstring !== ""){
                                    var jsonobj = JSON.parse(jsonstring);
                                    var xpub = ('xpub' in jsonobj) ? jsonobj.xpub : ""
                                    var publickey = ('publickey' in jsonobj) ? jsonobj.publickey : ""
                                    var fingerprint = ('fingerprint' in jsonobj) ? jsonobj.fingerprint : ""
                                    var derivationpath = ('derivationpath' in jsonobj) ? jsonobj.derivationpath : ""
                                    var descriptor = ('descriptor' in jsonobj) ? jsonobj.descriptor : ""
                                    if(xpub !== ""){ xpubOrPubl.textOutput = xpub }
                                    else{
                                        if(publickey !== ""){ xpubOrPubl.textOutput = publickey }
                                        else{ xpubOrPubl.textOutput = "Invalid QR" }
                                    }
                                    if(fingerprint !== ""){ master_key_fgp.textOutput = fingerprint }
                                    else{master_key_fgp.textOutput = "Invalid QR"}
                                    if(derivationpath !== ""){ bip32.textOutput = derivationpath }
                                    else{bip32.textOutput = "Invalid QR"}
                                    if(descriptor !== ""){ }
                                    else{}
                                    qrscaner.close()
                                }
                            }
                        }
                    }
                    Column {
                        id: hozlinespace
                        spacing: 12
                        anchors.verticalCenter: signerinforInput.verticalCenter
                        anchors.left: signerinforInput.right
                        anchors.leftMargin: 10
                        Rectangle {
                            width: 4
                            height: 110
                            radius: 2
                            anchors.horizontalCenter: parent.horizontalCenter
                            border.color: "#839096"
                        }
                        QText {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: STR.STR_QML_125
                            color: "#595959"
                            font.pixelSize: 16
                            font.family: "Lato"
                            font.weight: Font.ExtraBold
                        }
                        Rectangle {
                            width: 4
                            height: 110
                            radius: 2
                            anchors.horizontalCenter: parent.horizontalCenter
                            border.color: "#839096"
                        }
                    }
                    QText {
                        anchors {
                            left: hozlinespace.right
                            leftMargin: 20
                            top: parent.top
                            topMargin: 24
                        }
                        text: STR.STR_QML_113
                        color: "#323E4A"
                        font.pixelSize: 16
                    }
                    QQrButton {
                        width: 100
                        height: 100
                        anchors.left: hozlinespace.right
                        anchors.leftMargin: 20
                        anchors.verticalCenter: hozlinespace.verticalCenter
                        onButtonClicked: {
                            qrscaner.open()
                        }
                    }
                    QTextButton {
                        width: 150
                        height: 48
                        label.text: STR.STR_QML_106
                        label.font.pixelSize: 16
                        label.font.family: "Lato"
                        type: eTypeE
                        enabled: (signerName.textOutput !== "") &&
                                 (xpubOrPubl.textOutput !== "") &&
                                 (bip32.textOutput !== "") &&
                                 (master_key_fgp.textOutput !== "")
                        anchors {
                            right: parent.right
                            bottom: parent.bottom
                            bottomMargin: 40
                        }
                        onButtonClicked: {
                            tabremoteSigner.startAddSigner()
                        }
                    }
                    function startAddSigner(){
                        createRemoteBusyBox.open()
                        timerRemoteSigner.start()
                    }
                    Timer {
                        id: timerRemoteSigner
                        interval: 1000
                        repeat: false
                        onTriggered: {
                            var remoteSignerObj = { "signerNameInputted"    : signerName.textOutput,
                                                    "xpubOrPublInputted"    : xpubOrPubl.textOutput,
                                                    "bip32Inputted"         : bip32.textOutput ,
                                                    "masterFingerPrintInputted" : master_key_fgp.textOutput };
                            QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER, remoteSignerObj)
                            timerRemoteSigner.stop()
                        }
                    }
                }
            }
            style: TabViewStyle {
                frameOverlap: 1
                tab: Rectangle {
                    implicitWidth: 364
                    implicitHeight: 48
                    color: "transparent"
                    QText {
                        id: text
                        anchors.centerIn: parent
                        text: styleData.title
                        color: styleData.selected ? "#031F2B" : "#839096"
                        font.pixelSize: 14
                        font.weight: Font.DemiBold
                        font.family: "Montserrat"
                    }
                    Rectangle {
                        color: "#F6D65D"
                        implicitWidth: 364
                        height: 4
                        anchors.bottom: parent.bottom
                        visible: styleData.selected
                    }
                }

                frame: Rectangle { color: "transparent" }
            }
        }
        QButtonTextLink {
            height: 24
            label: STR.STR_QML_059
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK)
            }
        }
        Rectangle {
            id: stateAddSigner
            visible: AppModel.addSignerStep !== -1
            anchors.fill: parent
            color: Qt.rgba(0, 0, 0, 0.9)
            anchors.bottom: parent.bottom
            radius: 24
            MouseArea {
                anchors.fill: parent
                onClicked: {}
            }
            Loader {
                id: busyIndi
                anchors.centerIn: parent
                sourceComponent: (AppModel.addSignerStep < 0 || AppModel.addSignerStep > 2) ? null : rootAddsignerToWallet.addSignerComp[AppModel.addSignerStep]
            }
        }
        QQrImportScanner {
            id: qrscaner
        }
    }
    property var addSignerComp: [addSignerStep0, addSignerStep1, addSignerStep2]
    Component {
        id: addSignerStep0
        Column {
            spacing: 8
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
                width: 186
                height: 36
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                color: "#F6D65D"
                font.pixelSize: 24
                font.weight: Font.DemiBold
                font.family: "Montserrat"
                text: STR.STR_QML_083
            }
            QText {
                width: 263
                height: 24
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                color: "#F6D65D"
                font.pixelSize: 16
                font.family: "Lato"
                text: STR.STR_QML_117
            }
        }
    }
    Component {
        id: addSignerStep1
        Column {
            spacing: 8
            width: 300
            Item {
                width: 52
                height: visible ? 52 : 0
                anchors.horizontalCenter: parent.horizontalCenter
                QBusyIndicator {
                    width: 44
                    height: 44
                    anchors.centerIn: parent
                }
            }
            QText {
                width: 186
                height: 36
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#F6D65D"
                font.pixelSize: 24
                font.weight: Font.DemiBold
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.family: "Montserrat"
                text: STR.STR_QML_118
            }
            QText {
                width: 420
                height: 24*lineCount
                wrapMode: Text.WordWrap
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#F6D65D"
                font.pixelSize: 14
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                horizontalAlignment: Text.AlignHCenter
                text: STR.STR_QML_119
            }
            Item {
                width: 275
                height: 48
                anchors.horizontalCenter: parent.horizontalCenter
                Rectangle {
                    width: 275
                    height: 40
                    anchors.centerIn: parent
                    color: Qt.rgba(255, 255, 255, 0.2)
                    radius: 4
                    QText {
                        width: 186
                        height: 36
                        anchors.centerIn: parent
                        horizontalAlignment: Text.AlignHCenter
                        color: "#F1FAFE"
                        font.pixelSize: 12
                        font.family: "Lato"
                        text: AppModel.msgKeyHealthcheck
                    }
                }
            }
        }
    }
    Component {
        id: addSignerStep2
        Column {
            spacing: 8
            QProgressbar {
                id: progresBar
                anchors.horizontalCenter: parent.horizontalCenter
                percentage: AppModel.addSignerPercentage
            }
            QText {
                width: 186
                height: 36
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                color: "#F6D65D"
                font.pixelSize: 24
                font.weight: Font.DemiBold
                font.family: "Montserrat"
                text: STR.STR_QML_120
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
                text: STR.STR_QML_120
            }
        }
    }
    Popup {
        id: createRemoteBusyBox
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        Rectangle {
            id: boxMask
            width: popupWidth
            height: popupHeight
            radius: 24
            color: Qt.rgba(0, 0, 0, 0.8)
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: boxMask.width
                    height: boxMask.height
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
                    width: 700
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 24
                    font.weight: Font.DemiBold
                    font.family: "Montserrat"
                    wrapMode: Text.WrapAnywhere
                    text: STR.STR_QML_121 + signerName.textOutput
                }
                QText {
                    width: 328
                    height: 42
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 18
                    wrapMode: Text.WordWrap
                    font.family: "Lato"
                    text: STR.STR_QML_122
                }
            }
        }
    }
    Connections {
        target: AppModel
        onStartCreateRemoteSigner: { }
        onFinishedCreateRemoteSigner: {
            createRemoteBusyBox.close()
        }
    }
}
