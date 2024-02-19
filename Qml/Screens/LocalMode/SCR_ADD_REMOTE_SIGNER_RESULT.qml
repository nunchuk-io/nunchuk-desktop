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
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property string remoteSignerName: AppModel.singleSignerInfo.signerName
    property string remoteSignerXpub: AppModel.singleSignerInfo.signerXpub
    property string remoteSignerPublickey: AppModel.singleSignerInfo.signerPublickey
    property string remoteSignerbip32: AppModel.singleSignerInfo.signerDerivationPath
    property string remoteSignerfingerprint: AppModel.singleSignerInfo.signerMasterFingerPrint
    property bool   isPrimaryKey: AppModel.singleSignerInfo.isPrimaryKey
    property int    signerType: AppModel.singleSignerInfo.signerType
    property int    remoteSignerHealthStatus: -1
    property bool   walletEscrow: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO) ? AppModel.walletInfo.walletEscrow :
                                  (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_ADD_SIGNER_TO_WALLET) ? AppModel.newWalletInfo.walletEscrow : false
    QOnScreenContent {
        id:_content
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_126
        onCloseClicked: {
            if(NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex){
                QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT)
            }
            else{
                if(NUNCHUCKTYPE.FLOW_ADD_WALLET === QMLHandle.currentFlow){
                    QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG)
                }
                else {
                    QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT)
                }
            }
        }
        extraHeader: Row {
            spacing: 8
            QBadge {
                text: STR.STR_QML_641
                visible: isPrimaryKey
                color: "#FDD95C"
                anchors.verticalCenter: parent.verticalCenter
            }
            QBadge {
                text: GlobalData.signers(signerType)
                color: "#EAEAEA"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        QNotification {
            id: notification
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 81
            }
            visible: false
            label: STR.STR_QML_146
        }
        QTextInputBox {
            id: signerName
            width: 343
            heightMin: 54
            mode: eREADONLY_MODE
            placeholder.text: STR.STR_QML_102
            textOutput: remoteSignerName
            leftPading: 0
            rightPading: 50
            maximumLength: 100
            color: "transparent"
            fontFamily: "Lato"
            anchors {
                left: parent.left
                leftMargin: 41
                top: notification.bottom
                topMargin: 17
            }
        }
        QText {
            id: spectit
            anchors {
                left: parent.left
                leftMargin: 41
                top: signerName.bottom
                topMargin: 24
            }
            text: STR.STR_QML_147
            font.family: "Lato"
            font.pixelSize: 10
            font.weight: Font.Bold
        }
        Flickable {
            id: specinfo
            width: 328
            height: Math.min(250, contentHeight)
            flickableDirection: Flickable.VerticalFlick
            clip: true
            interactive: specinfo.height < specinfo.contentHeight
            contentHeight: specinfoContent.height
            ScrollBar.vertical: ScrollBar { active: true }
            anchors {
                left: parent.left
                leftMargin: 41
                top: spectit.bottom
                topMargin: 14
            }
            Column {
                id: specinfoContent
                width: 328
                height: xpubdisplay.height + bip32display.height + fingerPdisplay.height
                Item {
                    id: xpubdisplay
                    width: 328
                    height: 40 + (xpubValue.lineCount-1)*24
                    Row {
                        width: 328
                        height: parent.height
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 24
                        QText {
                            width: 152
                            height: 21
                            color: "#839096"
                            font.family: "Lato"
                            font.pixelSize: 14
                            anchors.verticalCenter: parent.verticalCenter
                            text: walletEscrow ? STR.STR_QML_148 : STR.STR_QML_149
                        }
                        QText {
                            id: xpubValue
                            width: 133
                            height: lineCount*24
                            clip: true
                            color: "#031F2B"
                            font.family: "Lato"
                            font.pixelSize: 16
                            anchors.verticalCenter: parent.verticalCenter
                            wrapMode: Text.WrapAnywhere
                            text: walletEscrow ? remoteSignerPublickey : remoteSignerXpub
                        }
                    }
                    Rectangle {
                        width: 328
                        height: 1
                        color: "#C9DEF1"
                        anchors.bottom: parent.bottom
                    }
                }
                Item {
                    id: bip32display
                    width: 328
                    height: 40 + (bip32Value.lineCount-1)*24
                    Row {
                        width: 328
                        height: parent.height
                        spacing: 24
                        QText {
                            width: 152
                            height: 21
                            color: "#839096"
                            font.family: "Lato"
                            font.pixelSize: 14
                            anchors.verticalCenter: parent.verticalCenter
                            text: STR.STR_QML_150
                        }
                        QText {
                            id: bip32Value
                            width: 133
                            height: lineCount*24
                            clip: true
                            color: "#031F2B"
                            font.family: "Lato"
                            font.pixelSize: 16
                            anchors.verticalCenter: parent.verticalCenter
                            wrapMode: Text.WrapAnywhere
                            text: remoteSignerbip32
                        }
                    }
                    Rectangle {
                        width: 328
                        height: 1
                        color: "#C9DEF1"
                        anchors.bottom: parent.bottom
                    }
                }
                Item {
                    id: fingerPdisplay
                    width: 328
                    height: 40 + (fingerprintValue.lineCount-1)*24
                    Row {
                        width: 328
                        height: parent.height
                        spacing: 24
                        QText {
                            width: 152
                            height: 21
                            clip: true
                            color: "#839096"
                            font.family: "Lato"
                            text: STR.STR_QML_151
                            font.pixelSize: 14
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QText {
                            id: fingerprintValue
                            width: 133
                            height: lineCount*24
                            color: "#031F2B"
                            font.family: "Lato"
                            font.pixelSize: 16
                            wrapMode: Text.WrapAnywhere
                            anchors.verticalCenter: parent.verticalCenter
                            text: remoteSignerfingerprint
                        }
                    }
                    Rectangle {
                        width: 328
                        height: 1
                        color: "#C9DEF1"
                        anchors.bottom: parent.bottom
                    }
                }
            }
        }
        QText {
            text: STR.STR_QML_128
            font.pixelSize: 10
            font.family: "Lato"
            color: "#323E4A"
            font.weight: Font.Bold
            anchors.top: specinfo.bottom
            anchors.topMargin: 24
            anchors.left: parent.left
            anchors.leftMargin: 56
            visible: usedInWallets.count
        }
        QListView {
            id: usedInWallets
            width: 328
            height: 200
            anchors.top: specinfo.bottom
            anchors.topMargin: 42
            anchors.left: parent.left
            anchors.leftMargin: 40
            clip: true
            model: AppModel.walletsUsingSigner
            delegate: Item {
                width: 344
                height: 40
                QIcon {
                    iconSize: 24
                    source: "qrc:/Images/Images/wallet_031F2B.png"
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 18
                    }
                }
                QText {
                    text: modelData
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 48
                    }
                    font.pixelSize: 16
                    color: "#031F2B"
                    font.family: "Lato"
                }
                Rectangle {
                    width: 468
                    height: 1
                    color: "#C9DEF1"
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                    }
                }
            }
        }
        QText {
            id: healthStatus
            anchors {
                left: parent.left
                leftMargin: 418
                top: notification.bottom
                topMargin: 17
            }
            text: STR.STR_QML_129
            color: "#323E4A"
            font.family: "Montserrat"
            font.weight: Font.DemiBold
            font.pixelSize: 14
        }
        QIcon {
            iconSize: 24
            id: healthIndicator
            source: 0 === remoteSignerHealthStatus ? "qrc:/Images/Images/check_circle_outline_24px.png" :
                                                     "qrc:/Images/Images/error_outline_24px.png"
            anchors {
                left: healthStatus.right
                leftMargin: 10
                verticalCenter: healthStatus.verticalCenter
            }
            visible: healthStatustext.visible
        }
        Flickable {
            id: healthCheckInput
            width: 344
            height: Math.min(notification.visible ? 300 : 350, contentHeight)
            flickableDirection: Flickable.VerticalFlick
            clip: true
            interactive: healthCheckInput.height < healthCheckInput.contentHeight
            contentHeight: messageInput.height + signatureInput.height + healthStatustext.height + 48
            ScrollBar.vertical: ScrollBar { active: true }
            anchors {
                right: parent.right
                rightMargin: 40
                top: healthStatus.bottom
                topMargin: 20
            }
            QTextInputBox {
                id: messageInput
                width: 344
                heightMin: 56
                placeholder.text: STR.STR_QML_131
                textOutput: AppModel.singleSignerInfo.signerMessage
                maximumLength: 280
                rightPading: 135
                onTypingFinished: {AppModel.singleSignerInfo.signerMessage = currentText}
                QTextButton {
                    width: 127
                    height: 24
                    label.text: STR.STR_QML_152
                    label.font.pixelSize: 10
                    type: eTypeB
                    anchors {
                        right: parent.right
                        rightMargin: 8
                        verticalCenter: parent.verticalCenter
                    }
                    onButtonClicked: {
                        exportMessage.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                + RoomWalletData.getValidFilename(AppModel.singleSignerInfo.signerMessage)
                                + "-message_to_sign.txt"
                        exportMessage.open()
                    }
                }
            }
            QTextInputBox {
                id: signatureInput
                width: 344
                heightMin: 80
                placeholder.text: STR.STR_QML_132
                rightPading: 145
                anchors {
                    top: messageInput.bottom
                    topMargin: 16
                }
                QTextButton {
                    width: 135
                    height: 24
                    label.text: STR.STR_QML_153
                    label.font.pixelSize: 10
                    type: eTypeB
                    anchors {
                        right: parent.right
                        rightMargin: 8
                        verticalCenter: parent.verticalCenter
                    }
                    onButtonClicked: {
                        importSignature.open()
                    }
                }
            }
            Rectangle {
                id: healthStatustext
                width: 343
                visible: false
                height: visible ? (0 === remoteSignerHealthStatus) || (-1 === remoteSignerHealthStatus) ? 94 : 110 : 0
                anchors {
                    top: signatureInput.bottom
                    topMargin: 16
                }
                radius: 4
                border.color: (0 === remoteSignerHealthStatus )? "#35ABEE" : (-1 === remoteSignerHealthStatus) ? "Transparent" :"#E02247"
                QText {
                    width: 314
                    height: (-1 === remoteSignerHealthStatus)  ? 48 : 72
                    anchors {
                        top: parent.top
                        topMargin: 8
                        horizontalCenter: parent.horizontalCenter
                    }
                    wrapMode: Text.WordWrap
                    text: (-1 === remoteSignerHealthStatus) ? STR.STR_QML_154 : (0 === remoteSignerHealthStatus) ? STR.STR_QML_155 : STR.STR_QML_156
                    color: (-1 === remoteSignerHealthStatus) ? "#9CAEB8" : "#031F2B"
                    font.pixelSize: 16
                    font.family: "Lato"
                }
            }
        }
        QIconTextButton {
            id: btnhealthCheck
            width: 280
            height: 48
            anchors {
                right: parent.right
                rightMargin: 72
                top: healthCheckInput.bottom
                topMargin: 24
            }
            type: eTypeA
            label: STR.STR_QML_137
            icons: ["health_031F2B.png", "health_C9DEF1.png", "health_C9DEF1.png", "health_031F2B.png"]
            fontPixelSize: 16
            onButtonClicked: {
                heathCheckBusyBox.open()
                QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK)
            }
        }
        QTextButton {
            id: btn_remove_key
            width: 280
            height: 48
            label.text: STR.STR_QML_241
            label.font.pixelSize: 16
            type: eTypeB
            anchors {
                right: parent.right
                rightMargin: 72
                top: btnhealthCheck.bottom
                topMargin: 16
            }
            onButtonClicked: {
                var ret = AppModel.walletList.removeOrNot(remoteSignerfingerprint, remoteSignerbip32)
                if(ret.used_in_assisted_wallet){
                    _info1.open()
                }
                else if (ret.used_in_free_wallet) {
                    _confirm.contentText = STR.STR_QML_243_used
                    _confirm.open()
                }
                else {
                    _confirm.contentText = STR.STR_QML_243
                    _confirm.open()
                }
            }
        }

        QTextButton {
            width: 222
            height: 48
            label.text: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_ADD_HARDWARE_SIGNER) ? "Close" : "Confirm & Close"
            label.font.pixelSize: 16
            type: eTypeB
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: btn_remove_key.bottom
                topMargin: 16
            }
            onButtonClicked: {
                if(NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex){
                    QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT)
                }
                else{
                    if(NUNCHUCKTYPE.FLOW_ADD_WALLET === QMLHandle.currentFlow){
                        QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG)
                    }
                    else {
                        QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT)
                    }
                }
            }
        }
    }
    FileDialog {
        id: exportMessage
        fileMode: FileDialog.SaveFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE, exportMessage.currentFile)
        }
    }
    FileDialog {
        id: importSignature
        fileMode: FileDialog.OpenFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE, importSignature.file)
        }
    }
    /*=============================Popup=============================*/
    QPopupInfo{
        id:_info1
        contentText: STR.STR_QML_554
    }
    QConfirmYesNoPopup{
        id:_confirm
        contentText:STR.STR_QML_243
        onConfirmNo: close()
        onConfirmYes: {
            close()
            var remoteSignerObj = { "master_fingerprint"    : remoteSignerfingerprint,
                                    "derivation_path"       : remoteSignerbip32 };
            QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST, remoteSignerObj)
        }
    }

    Popup {
        id: heathCheckBusyBox
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
                    text: STR.STR_QML_140 + remoteSignerName
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
                    text: STR.STR_QML_122
                }
            }
        }
    }
    Connections {
        target: AppModel
        onFinishedHealthCheckRemoteSigner: {
            heathCheckBusyBox.close()
            remoteSignerHealthStatus = AppModel.singleSignerInfo.signerHealth
            healthStatustext.visible = true
        }
    }
}
