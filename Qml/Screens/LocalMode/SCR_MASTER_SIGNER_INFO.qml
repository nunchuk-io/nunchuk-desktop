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
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property string masterSignerName: AppModel.masterSignerInfo.masterSignername
    property string masterSignerSpec: AppModel.masterSignerInfo.fingerPrint
    property int    masterSignerHealth: -1
    property bool   deviceNeedsPinSent:       AppModel.masterSignerInfo.needPinSent
    property bool   deviceNeedsPassphraseSent:AppModel.masterSignerInfo.needPassphraseSent
    property int    signerType: AppModel.masterSignerInfo.signerType
    property bool   isPrimaryKey: AppModel.masterSignerInfo.isPrimaryKey
    property string messageToSign: AppModel.masterSignerInfo.message
    property string messageToSignSHA256: AppModel.masterSignerInfo.messageSHA256
    property bool   isHardwareSigner: (AppModel.masterSignerInfo.signerType === NUNCHUCKTYPE.HARDWARE)

    QOnScreenContent {
        id:_content
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_126
        onCloseClicked: {
            GlobalData.customMsgHealthcheck = false
            if(QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO){
                QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO)
            }
            else {
                QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_BACK_REQUEST)
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
        Column {
            id: columnInfo
            spacing: 16
            anchors {
                left: parent.left
                leftMargin: 41
                top: parent.top
                topMargin: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO) ? 81 : 97
            }
            QTextInputBox {
                id: signerName
                width: 343
                heightMin: 54
                mode: ePREVIEW_MODE
                color: "Transparent"
                placeholder.text: STR.STR_QML_102
                textOutput: masterSignerName
                rightPading: 50
                maximumLength: 50
                onTypingFinished: {
                    if(currentText !== masterSignerName && currentText !== ""){
                        QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_EDIT_NAME, currentText)
                    }
                }
            }
            QTextInputBox {
                id: signerSpec
                width: 343
                heightMin: 54
                mode: eREADONLY_MODE
                color: "Transparent"
                placeholder.text: STR.STR_QML_127
                textOutput: masterSignerSpec
                textOutputCapitalization: Font.AllUppercase
            }
            Column {
                width: 343
                visible: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO)
                spacing: 4
                anchors {
                    left: parent.left
                    leftMargin: 16
                }
                QText {
                    text: "XPUB"
                    font.family: "Lato"
                    font.weight: Font.Bold
                    color: "#031F2B"
                    font.pixelSize: 10
                }
                QTextEdit {
                    id: xpubValue
                    width: parent.width
                    clip: true
                    readOnly: true
                    color: "#031F2B"
                    font.family: "Lato"
                    font.pixelSize: 16
                    wrapMode: Text.WrapAnywhere
                    text: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO) ? AppModel.singleSignerInfo.signerXpub : ""
                }
                Rectangle {
                    width: 341
                    height: 2
                    color: "#C9DEF1"
                    anchors {
                        left: parent.left
                        leftMargin: -16
                    }
                }
            }
            QTextInputBox {
                id: derpath
                visible: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO)
                width: 343
                heightMin: 54
                mode: eREADONLY_MODE
                color: "Transparent"
                placeholder.text: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO) ? STR.STR_QML_150 : ""
                textOutput: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO) ? AppModel.singleSignerInfo.signerDerivationPath : ""
            }
        }
        QText {
            width: 500
            anchors {
                left: parent.left
                leftMargin: 56
                top: columnInfo.bottom
                topMargin: 24
            }
            text: STR.STR_QML_128
            color: "#323E4A"
            font.weight: Font.Bold
            font.family: "Lato"
            font.pixelSize: 10
            visible: inWallets.count
        }
        Rectangle{
            width: 344
            height: 160
            clip: true
            radius: 12
            border.color:"#EAEAEA"
            border.width: 2
            color: "transparent"
            anchors {
                left: parent.left
                leftMargin: 40
                top: columnInfo.bottom
                topMargin: 42
            }
            visible: inWallets.count > 0
            QListView {
                id: inWallets
                width: 344
                height: 160
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
        }

        QText {
            id: healthStatus
            anchors {
                left: parent.left
                leftMargin: 418
                top: parent.top
                topMargin: 98
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
            source: 0 === masterSignerHealth ? "qrc:/Images/Images/check_circle_outline_24px.png" :
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
            width: 355
            height: Math.min(350, contentHeight)
            flickableDirection: Flickable.VerticalFlick
            clip: true
            interactive: healthCheckInput.height < healthCheckInput.contentHeight
            contentHeight: (switchCutomizeMessage.switchOn ?  custommessage.height : 0) + onOffCutomizeMessage.height + healthStatustext.height + 48
            ScrollBar.vertical: ScrollBar { active: true }
            anchors {
                right: parent.right
                rightMargin: 40
                top: healthStatus.bottom
                topMargin: 20
            }
            Row {
                id: onOffCutomizeMessage
                height: 32
                spacing: 56
                QText {
                    width: 203
                    height: 32
                    text: STR.STR_QML_130
                    color: "#323E4A"
                    font.family: "Montserrat"
                    font.weight: Font.DemiBold
                    font.pixelSize: 14
                    anchors.verticalCenter: parent.verticalCenter
                }
                QSwitchTypeB {
                    id: switchCutomizeMessage
                    width: 56
                    height: 32
                    switchOn: GlobalData.customMsgHealthcheck
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            GlobalData.customMsgHealthcheck = !GlobalData.customMsgHealthcheck
                            if(!switchCutomizeMessage.switchOn){AppModel.masterSignerInfo.message = ""}
                        }
                    }
                }
            }
            Column {
                id: custommessage
                spacing: 16
                width: 344
                visible: switchCutomizeMessage.switchOn
                anchors {
                    top: onOffCutomizeMessage.bottom
                    topMargin: 16
                }
                QTextInputBoxTypeD  {
                    id: messageInput
                    boxWidth: 342
                    boxHeight: 48
                    label: STR.STR_QML_131
                    placeHoder: STR.STR_QML_561
                    isValid: true
                    textInputted: messageToSign
                    onTypingFinished: { AppModel.masterSignerInfo.message = currentText}

                }
                QTextInputBoxTypeD  {
                    id: signatureInput
                    boxWidth: 342
                    boxHeight: 76
                    label: STR.STR_QML_132
                    textInputted: AppModel.masterSignerInfo.signature
                    enableLengthLimit:true
                    isValid: true
                    maxLength:80
                    mode:eREADONLY_MODE

                }
            }
            Rectangle {
                id: healthStatustext
                width: 343
                visible: false
                height: visible ? (0 === masterSignerHealth) || (-1 === masterSignerHealth) ? 94 : 110 : 0
                anchors {
                    top: switchCutomizeMessage.switchOn ? custommessage.bottom : onOffCutomizeMessage.bottom
                    topMargin: 16
                }
                radius: 4
                border.width: 2
                border.color: (0 === masterSignerHealth )? "#35ABEE" : (-1 === masterSignerHealth) ? "Transparent" :"#E02247"
                QText {
                    width: 314
                    height: (-1 === masterSignerHealth)  ? 48 : 72
                    anchors {
                        top: parent.top
                        topMargin: 8
                        horizontalCenter: parent.horizontalCenter
                    }
                    wrapMode: Text.WordWrap
                    text: (-1 === masterSignerHealth) ? STR.STR_QML_133 : (0 === masterSignerHealth) ? STR.STR_QML_134 : STR.STR_QML_135
                    color: (-1 === masterSignerHealth) ? "#9CAEB8" : "#031F2B"
                    font.pixelSize: 16
                    font.family: "Lato"
                }
            }
        }
        QText {
            anchors{
                horizontalCenter: healthCheckInput.horizontalCenter
                top: healthCheckInput.bottom
                topMargin: 5
            }
            height: 28
            width: paintedWidth
            text: STR.STR_QML_136
            color: "#031F2B"
            font.family: "Lato"
            font.pixelSize: 14
            visible: (signerType === NUNCHUCKTYPE.HARDWARE)
        }

        Row{
            spacing: 12
            layoutDirection:Qt.LeftToRight
            anchors {
                left: parent.left
                leftMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }

            QTextButton {
                width: label.paintedWidth + 2*20
                height: 48
                label.text: STR.STR_QML_241
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    var ret = AppModel.walletList.removeOrNot(masterSignerSpec)
                    if(signerType === NUNCHUCKTYPE.FOREIGN_SOFTWARE){
                        _info2.open()
                    }
                    else if(ret.used_in_assisted_wallet) {
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
        }

        Row{
            spacing: 12
            layoutDirection:Qt.RightToLeft
            anchors {
                right: parent.right

                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            QTextButton {
                width: label.paintedWidth + 2*20
                height: 48
                label.text: STR.STR_QML_139
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    _content.closeClicked()
                }
            }

            QTextButton {
                id: btnhealthCheck
                width: label.paintedWidth + 2*20
                height: 48
                type: eTypeB
                label.text: STR.STR_QML_137
                label.font.pixelSize: 16
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_HEALTH_CHECK, AppModel.masterSignerInfo.masterSignerId)
                }
            }
            QTextButton {
                id: btnTopUpXPUBs
                width: label.paintedWidth + 2*20
                height: 48
                type: eTypeB
                label.text: STR.STR_QML_560
                label.font.pixelSize: 16
                visible: signerType !== NUNCHUCKTYPE.SOFTWARE && signerType !== NUNCHUCKTYPE.FOREIGN_SOFTWARE
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_GET_XPUBS, AppModel.masterSignerInfo.fingerPrint)
                }
            }
        }

        /*=============================Popup=============================*/
        QPopupInfo{
            id:_info1
            contentText: STR.STR_QML_554
        }
        QPopupInfo{
            id:_info2
            contentText: STR.STR_QML_555
        }

        QConfirmYesNoPopup{
            id:_confirm
            contentText:STR.STR_QML_243
            onConfirmNo: close()
            onConfirmYes: {
                close()
                QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST, AppModel.masterSignerInfo.masterSignerId)
            }
        }

        QPopupBusyLoading{
            id:_busyTopUp
            warningText1:STR.STR_QML_582
        }

        /*=============================Popup=============================*/

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
            Loader {
                id: busyIndi
                anchors.centerIn: parent
                sourceComponent: createBusy
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
                        width: 700
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        color: "#F6D65D"
                        font.pixelSize: 24
                        font.weight: Font.DemiBold
                        font.family: "Montserrat"
                        wrapMode: Text.WrapAnywhere
                        text: STR.STR_QML_140 + masterSignerName
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
                        text: STR.STR_QML_141
                    }
                    Rectangle {
                        width: 500
                        height: 50
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: Qt.rgba(255, 255, 255, 0.2)
                        radius: 4
                        QText {
                            width: parent.width - 50
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            color: "#F1FAFE"
                            font.pixelSize: 14
                            wrapMode: Text.WrapAnywhere
                            font.family: "Lato"
                            text: messageToSign
                        }
                    }
                    QText {
                        width: 328
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        color: "#F6D65D"
                        font.pixelSize: 14
                        wrapMode: Text.WordWrap
                        font.family: "Lato"
                        text: "Hash"
                        visible: isHardwareSigner
                    }
                    Rectangle {
                        width: 500
                        height: 50
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: Qt.rgba(255, 255, 255, 0.2)
                        radius: 4
                        visible: isHardwareSigner
                        QText {
                            width: parent.width - 50
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            color: "#F1FAFE"
                            font.pixelSize: 14
                            wrapMode: Text.WrapAnywhere
                            font.family: "Lato"
                            text: messageToSignSHA256
                            font.capitalization: Font.AllUppercase
                        }
                    }
                }
            }
        }
        Connections {
            target: AppModel
            onStartHealthCheckMasterSigner: {
                masterSignerHealth = -1
                busyOverlay.visible = true
                healthStatustext.visible = false
            }
            onFinishedHealthCheckMasterSigner : {
                masterSignerHealth = AppModel.masterSignerInfo.masterSignerHealth
                busyOverlay.visible = false
                healthStatustext.visible = true
                console.log(masterSignerHealth)
            }
            onStartTopXPUBsMasterSigner:{
                _busyTopUp.open()
            }
            onFinishedTopXPUBsMasterSigner:{
                _busyTopUp.close()
            }
        }
        QContextMenu {
            id: optionMenu
            menuWidth: 120
            onItemClicked: {
                functions[index]()
            }
        }
    }
}
