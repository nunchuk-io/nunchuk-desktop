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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: rootWalletConfirmation
    property string masterSignerName: AppModel.masterSignerInfo.masterSignername
    property string masterSignerSpec: AppModel.masterSignerInfo.masterSignerDevice.deviceMasterFingerPrint
    property int masterSignerHealth: AppModel.masterSignerInfo.masterSignerHealth

    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_021
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_WALLET_CONFIRMATION)
        }
        Row {
            id: step
            width: 644
            height: 31
            spacing: 8
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 78
            }
            QAddStep {
                step: 1
                stepName: STR.STR_QML_022
                currentStep: 3
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle {
                width: 30
                height: 2
                radius: 4
                color: "#839096"
                anchors.verticalCenter: parent.verticalCenter
            }
            QAddStep {
                step: 2
                stepName: STR.STR_QML_023
                currentStep: 3
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle {
                width: 30
                height: 2
                radius: 4
                color: "#839096"
                anchors.verticalCenter: parent.verticalCenter
            }
            QAddStep {
                step: 3
                stepName: STR.STR_QML_024
                currentStep: 3
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        QText {
            id: confirmtit
            anchors {
                left: parent.left
                leftMargin: 40
                top: step.bottom
                topMargin: 32
            }
            color: "#031F2B"
            font.pixelSize: 16
            font.weight: Font.DemiBold
            text: STR.STR_QML_061
        }
        Column {
            spacing: 24
            anchors {
                left: parent.left
                leftMargin: 40
                top: confirmtit.bottom
                topMargin: 16
            }
            QTextInputBox {
                id: signerName
                width: 720
                heightMin: 54
                color: "Transparent"
                mode: eREADONLY_MODE
                placeholder.text: STR.STR_QML_025
                textOutput: AppModel.newWalletInfo.walletName
            }
            QTextInputBox {
                id: mandn
                width: 269
                heightMin: 54
                color: "Transparent"
                mode: eREADONLY_MODE
                placeholder.text: STR.STR_QML_027
                textOutput: AppModel.newWalletInfo.walletEscrow ? STR.STR_QML_029 : STR.STR_QML_028
            }
            QTextInputBox {
                id: addresstype
                readonly property var addressType_Value: [
                    STR.STR_QML_065,
                    STR.STR_QML_064,
                    STR.STR_QML_063,
                    STR.STR_QML_062,
                    STR.STR_QML_553
                ]
                width: 269
                heightMin: 54
                color: "Transparent"
                mode: eREADONLY_MODE
                placeholder.text: STR.STR_QML_066
                textOutput: addresstype.addressType_Value[AppModel.newWalletInfo.walletAddressType]
            }
            QTextInputBox {
                id: createdate
                width: 269
                heightMin: 54
                color: "Transparent"
                mode: eREADONLY_MODE
                placeholder.text: STR.STR_QML_067
                textOutput: AppModel.newWalletInfo.walletCreateDate
            }
        }

        QTextInputBox {
            id: signerConfiguration
            anchors {
                left: parent.left
                leftMargin: 350
                top: parent.top
                topMargin: 265
            }
            width: 269
            heightMin: 54
            color: "Transparent"
            mode: eREADONLY_MODE
            placeholder.text: STR.STR_QML_022
            placeholder.font.pixelSize: 14
            placeholder.font.capitalization:Font.MixedCase
            textOutput: qsTr("%1 Of %2 %3")
                        .arg(AppModel.newWalletInfo.walletM)
                        .arg(AppModel.newWalletInfo.walletN)
                        .arg( AppModel.newWalletInfo.walletN > 1 ? STR.STR_QML_069 : STR.STR_QML_070)
        }
        QText {
            anchors {
                left: parent.left
                leftMargin: 357
                top: parent.top
                topMargin: 346
            }
            color: "#323E4A"
            font.pixelSize: 10
            font.weight: Font.Bold
            text: STR.STR_QML_015
            font.capitalization: Font.AllUppercase
        }
        QListView {
            id: signerAssigned
            width: 343
            height: 197
            spacing: 8
            clip: true
            anchors {
                left: parent.left
                leftMargin: 357
                top: parent.top
                topMargin: 364
            }
            ScrollBar.vertical: ScrollBar { active: true }
            model: AppModel.newWalletInfo.walletSingleSignerAssigned
            delegate: Item {
                id: signerAssigneddlg
                property int          signerType: model.single_signer_type
                property bool isRemoteSigner: model.single_signer_type === NUNCHUCKTYPE.AIRGAP || model.single_signer_type === NUNCHUCKTYPE.FOREIGN_SOFTWARE
                property bool isNeedGetXpubs:  model.single_signer_need_Topup_Xpub
                width: 343
                height: signerAssigneddlg.isRemoteSigner ? 73 : 53
                Rectangle {
                    id: rect
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 341
                    height: signerAssigneddlg.isRemoteSigner ? 70 : 50
                    color: Qt.rgba(255, 255, 255)
                    border.color: signerAssigneddlg.isNeedGetXpubs ? "#FF7A00" : "transparent"
                }
                DropShadow {
                    anchors.fill: rect
                    verticalOffset: 2
                    cached: true
                    radius: 8
                    samples: 16
                    color: Qt.rgba(0, 0, 0, 0.15)
                    source: rect
                }
                Rectangle {
                    id: indicatiorValid
                    width: 4
                    height: parent.height - 18
                    radius: 2
                    color: model.single_signer_need_Topup_Xpub ? "#C9DEF1" : "#FF7A00"
                    anchors {
                        left: parent.left
                        leftMargin: 16
                        verticalCenter: rect.verticalCenter
                    }
                }
                Column {
                    id: txt
                    width: 300
                    height: signerAssigneddlg.isRemoteSigner ? 53 : 37
                    anchors {
                        left: parent.left
                        leftMargin: 28
                        verticalCenter: indicatiorValid.verticalCenter
                    }
                    Item {
                        width: parent.width
                        height: 21
                        QText {
                            width: parent.width - (signerAssigneddlg.isNeedGetXpubs ?  90 : signerAssigneddlg.isRemoteSigner ? 70 : 20)
                            height: 21
                            verticalAlignment: Text.AlignVCenter
                            text: model.singleSigner_name
                            font.pixelSize: 14
                            font.weight: Font.DemiBold
                            font.family: "Montserrat"
                            color: "#031F2B"
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Rectangle {
                            width: _txt.paintedWidth + 8*2
                            height: 21
                            color: "#FDD95C"
                            visible: model.single_signer_primary_key
                            radius: 4
                            anchors{
                                verticalCenter: parent.verticalCenter
                                right: _type.left
                                rightMargin: 4
                            }
                            QText {
                                id:_txt
                                text: STR.STR_QML_641
                                font.family: "Lato"
                                font.weight: Font.Bold
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                color: "#031F2B"
                            }
                        }

                        Rectangle {
                            id:_type
                            width: 70
                            height: 21
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            color: "#C9DEF1"
                            radius: 4
                            visible: !signerAssigneddlg.isNeedGetXpubs
                            QText {
                                text: GlobalData.signerNames(signerAssigneddlg.signerType)
                                font.family: "Lato"
                                font.weight: Font.Bold
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                color: "#031F2B"
                            }
                        }
                    }
                    QText {
                        width: parent.width
                        height: 16
                        font.pixelSize: 12
                        color: "#031F2B"
                        font.family: "Lato"
                        text: "XFP: " + model.singleSigner_masterFingerPrint
                        font.capitalization: Font.AllUppercase
                    }
                    QText {
                        width: parent.width
                        height: signerAssigneddlg.isRemoteSigner ? 16 : 0
                        visible: signerAssigneddlg.isRemoteSigner
                        font.pixelSize: 10
                        color: "#839096"
                        font.family: "Lato"
                        text: "BIP32 Path: " + model.singleSigner_derivationPath
                    }
                }
                QTextButton {
                    width: 100
                    height: 24
                    label.text: STR.STR_QML_071
                    label.font.pixelSize: 10
                    type: eTypeD
                    visible: signerAssigneddlg.isNeedGetXpubs
                    enabled: visible
                    anchors {
                        right: parent.right
                        rightMargin: 8
                        verticalCenter: rect.verticalCenter
                    }
                    onButtonClicked: {
                        startCacheXpubs(index, singleSigner_name)
                    }
                }
            }
        }
        QText {
            visible: !AppModel.newWalletInfo.capableCreate
            width: 344
            height: 48
            font.pixelSize: 12
            color: "#323E4A"
            font.family: "Lato"
            wrapMode: Text.WordWrap
            anchors {
                horizontalCenter: signerAssigned.horizontalCenter
                top: signerAssigned.bottom
                topMargin: 4
            }
            text: STR.STR_QML_072
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
                QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST)
            }
        }
        QTextButton {
            width: 120
            height: 48
            label.text: STR.STR_QML_035
            label.font.pixelSize: 16
            type: eTypeB
            anchors {
                left: parent.left
                leftMargin: 424
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_WALLET_CONFIRMATION)
            }
        }
        QTextButton {
            width: 200
            height: 48
            label.text: STR.STR_QML_073
            label.font.pixelSize: 16
            type: eTypeE
            enabled: AppModel.newWalletInfo.capableCreate
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                startCreateWallet()
            }
        }
        Rectangle {
            id: busyOverlay
            visible: false
            anchors.fill: parent
            color: Qt.rgba(0, 0, 0, 0.9)
            anchors.bottom: parent.bottom
            radius: 24
            MouseArea {
                anchors.fill: parent
                onClicked: {}
            }
            QCloseButton {
                icon: "qrc:/Images/Images/close_24px.png"
                anchors {
                    right: parent.right
                    rightMargin: 36
                    top: parent.top
                    topMargin: 36
                }
                onClicked: {
                    busyOverlay.visible = false
                    busyIndi.sourceComponent = null
                }
            }
            Loader {
                id: busyIndi
                anchors.centerIn: parent
            }
        }
        Component {
            id: cacheXpubsComp
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
                    width: 500
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 24
                    font.weight: Font.DemiBold
                    wrapMode: Text.WrapAnywhere
                    font.family: "Montserrat"
                    text: STR.STR_QML_074 + nameSignerNeedGetXpub
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
        Component {
            id: downloadDescriptor
            Column {
                spacing: 8
                Item {
                    width: 52
                    height: 52
                    visible: false
                    QBusyIndicator {
                        width: 44
                        height: 44
                        anchors.centerIn: parent
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                QText {
                    width: 214
                    height: 36
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 24
                    font.weight: Font.DemiBold
                    font.family: "Montserrat"
                    text: STR.STR_QML_076
                }
                QText {
                    width: 450
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 14
                    wrapMode: Text.WordWrap
                    font.family: "Lato"
                    text: STR.STR_QML_077
                }
                Item {
                    width: 382
                    height: 16
                }
                QTextButton {
                    width: 265
                    height: 48
                    anchors.horizontalCenter: parent.horizontalCenter
                    label.text: STR.STR_QML_078
                    label.font.pixelSize: 16
                    type: eTypeA
                    onButtonClicked: {
                        savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                + RoomWalletData.getValidFilename(AppModel.newWalletInfo.walletName)
                                + ".bsms"
                        savefileDialog.open()
                    }
                }
                QTextButton {
                    width: 265
                    height: 48
                    anchors.horizontalCenter: parent.horizontalCenter
                    label.text: STR.STR_QML_079
                    label.font.pixelSize: 16
                    type: eTypeC
                    onButtonClicked: {
                        startCreateWalletWithoutBackupDescriptor()
                    }
                }
                QText {
                    width: 328
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    color: "#F6D65D"
                    font.pixelSize: 12
                    font.family: "Lato"
                    text: STR.STR_QML_080
                }
            }
        }
        Component {
            id: createBusy
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
                    width: 214
                    height: 36
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 24
                    font.weight: Font.DemiBold
                    font.family: "Montserrat"
                    text: STR.STR_QML_081
                }
                QText {
                    width: 328
                    height: 42
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 14
                    wrapMode: Text.WordWrap
                    font.family: "Lato"
                    text: STR.STR_QML_082
                }
            }
        }
    }
    FileDialog {
        id: savefileDialog
        fileMode: FileDialog.SaveFile
        onAccepted: {
            startDownloadDescriptor()
        }
        onRejected: {
            busyOverlay.visible = false
            busyIndi.sourceComponent = null
        }
    }

    property int indexSignerNeedGetXpub: -1
    property string nameSignerNeedGetXpub: ""
    function startCreateWalletWithoutBackupDescriptor(){
        busyIndi.sourceComponent = createBusy
        busyOverlay.visible = true
        createWalletWithoutBackupTimer.restart()
    }
    function startCreateWallet(){
        busyIndi.sourceComponent = downloadDescriptor
        busyOverlay.visible = true
    }
    function startDownloadDescriptor(){
        busyIndi.sourceComponent = createBusy
        busyOverlay.visible = true
        downloadDescriptorTimer.restart()
    }
    function startCacheXpubs(num, name) {
        busyIndi.sourceComponent = cacheXpubsComp
        busyOverlay.visible = true
        indexSignerNeedGetXpub = num
        nameSignerNeedGetXpub = name
        topUpXpubSignerTimer.restart()
    }
    Timer {
        id: topUpXpubSignerTimer
        interval: 1000
        repeat: false
        onTriggered: {
            QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST, indexSignerNeedGetXpub)
        }
    }
    Timer {
        id: createWalletWithoutBackupTimer
        interval: 1000
        onTriggered: {
            QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_CONFIRM_CREATE)
        }
    }
    Timer {
        id: downloadDescriptorTimer
        interval: 1000
        onTriggered: {
            QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR, savefileDialog.currentFile)
        }
    }
    Connections {
        target: AppModel
        onFinishGenerateSigner : {
            busyOverlay.visible = false
            indexSignerNeedGetXpub = -1
            nameSignerNeedGetXpub = ""
            busyIndi.sourceComponent = null
        }
        onFinishedCreateWallet: {
            busyIndi.sourceComponent = null
            busyOverlay.visible = false
            createWalletWithoutBackupTimer.stop()
        }
    }
}
