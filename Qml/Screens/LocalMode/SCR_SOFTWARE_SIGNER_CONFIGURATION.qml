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
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: rootAddsignerToWallet
    property string signerNameInputted: ""
    QOnScreenContent {
        id: _content
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        enableHeader: false
        Loader {
            id: mainloader
            anchors.fill: parent
            sourceComponent: nameConfiguration
        }
        Component {
            id: nameConfiguration
            QOnScreenContent {
                label.text: STR.STR_QML_266
                onCloseClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
                }
                QTextInputBoxTypeB {
                    id: signername
                    label: STR.STR_QML_267
                    boxWidth: 540
                    boxHeight: 48
                    maxLength: 20
                    enableLengthLimit: true
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        top: parent.top
                        topMargin: 130
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
                        QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION)
                    }
                }
                QTextButton {
                    width: 200
                    height: 48
                    anchors {
                        right: parent.right
                        rightMargin: 40
                        bottom: parent.bottom
                        bottomMargin: 32
                    }
                    label.text: STR.STR_QML_097
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled: signername.textInputted !== ""
                    onButtonClicked: {
                        rootAddsignerToWallet.signerNameInputted = signername.textInputted
                        if(NUNCHUCKTYPE.FLOW_PRIMARY_KEY === QMLHandle.currentFlow || NUNCHUCKTYPE.FLOW_REPLACE_PRIMARY_KEY === QMLHandle.currentFlow)
                        {
                            mainloader.sourceComponent = passPhraseConfigPrimaryKey
                        }
                        else{
                            mainloader.sourceComponent = passPhraseConfiguration
                        }
                    }
                }
            }
        }
        Component {
            id: passPhraseConfiguration
            QOnScreenContent {
                label.text: STR.STR_QML_268
                onCloseClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
                }
                Column {
                    spacing: 20
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        top: parent.top
                        topMargin: 90
                    }
                    QText {
                        width: 539
                        height: 84
                        text: STR.STR_QML_269
                        verticalAlignment: Text.AlignVCenter
                        color: "#031F2B"
                        font.family: "Lato"
                        font.pixelSize: 16
                        wrapMode: Text.WordWrap
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                    }
                    QTextInputBoxTypeB {
                        id: passphrase
                        label: STR.STR_QML_270
                        boxWidth: 540
                        boxHeight: 48
                        textweight: Font.Normal
                        isPassword: true
                        onTypingFinished: {
                            confirmpassphrases.isValid = true
                            confirmpassphrases.errorText = ""
                        }
                    }
                    QTextInputBoxTypeB {
                        id: confirmpassphrases
                        label: STR.STR_QML_271
                        boxWidth: 540
                        boxHeight: 48
                        isPassword: true
                        onTypingFinished: {
                            confirmpassphrases.isValid = true
                            confirmpassphrases.errorText = ""
                        }
                    }
                }
                Rectangle {
                    width: 728
                    height: 60
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 120
                    anchors.horizontalCenter: parent.horizontalCenter
                    radius: 8
                    color: "#EAEAEA"
                    Row {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8
                        QImage {
                            height: parent.height
                            width: height
                            source: "qrc:/Images/Images/info-60px.png"
                        }
                        QText {
                            height: parent.height
                            text: STR.STR_QML_272
                            color: "#031F2B"
                            font.family: "Lato"
                        }
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
                        mainloader.sourceComponent = nameConfiguration
                    }
                }
                Row {
                    spacing: 12
                    anchors {
                        right: parent.right
                        rightMargin: 40
                        bottom: parent.bottom
                        bottomMargin: 32
                    }
                    QTextButton {
                        width: 200
                        height: 48
                        label.text: STR.STR_QML_273
                        label.font.pixelSize: 16
                        type: eTypeB
                        enabled: (passphrase.textInputted !== "") && (confirmpassphrases.textInputted !== "")
                        onButtonClicked: {
                            if(confirmpassphrases.textInputted !== passphrase.textInputted){
                                confirmpassphrases.isValid = false
                                confirmpassphrases.errorText = STR.STR_QML_274
                            }
                            else{
                                var signerObj = { "signername"    : rootAddsignerToWallet.signerNameInputted,
                                                  "passphrase"    : passphrase.textInputted};
                                QMLHandle.sendEvent(EVT.EVT_SOFTWARE_SIGNER_REQUEST_CREATE, signerObj)
                                stateAddSigner.visible = true
                            }
                        }
                    }

                    QTextButton {
                        width: 230
                        height: 48
                        label.text: STR.STR_QML_275
                        label.font.pixelSize: 16
                        type: eTypeE
                        enabled: rootAddsignerToWallet.signerNameInputted !== ""
                        onButtonClicked: {
                            passphrase.textInputted = ""
                            var signerObj = {
                                "signername"    : rootAddsignerToWallet.signerNameInputted,
                                "passphrase"    : ""
                            };
                            QMLHandle.sendEvent(EVT.EVT_SOFTWARE_SIGNER_REQUEST_CREATE, signerObj)
                            stateAddSigner.visible = true
                        }
                    }
                }

                Connections {
                    target: AppModel
                    onNotifySignerExist: {
                        _content.showPopupInfo(isSoftware, fingerPrint)
                    }
                }
                Connections {
                    target: _info
                    onYesClicked: {
                        var signerObj = {
                            "signername"    : rootAddsignerToWallet.signerNameInputted,
                            "passphrase"    : passphrase.textInputted,
                            "key_yes_accept": true
                        };
                        QMLHandle.sendEvent(EVT.EVT_SOFTWARE_SIGNER_REQUEST_CREATE, signerObj)
                    }
                }
            }
        }
        Component {
            id: passPhraseConfigPrimaryKey
            QOnScreenContent {
                label.text: STR.STR_QML_268_PKEY
                onCloseClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
                }
                Column {
                    spacing: 20
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        top: parent.top
                        topMargin: 90
                    }
                    Item{
                        width: 539
                        height: 140
                        QText {
                            anchors.top: parent.top
                            width: 539
                            text: STR.STR_QML_665
                            verticalAlignment: Text.AlignVCenter
                            color: "#031F2B"
                            font.family: "Lato"
                            font.pixelSize: 16
                            wrapMode: Text.WordWrap
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                        }
                        QText {
                            anchors.bottom: parent.bottom
                            width: 539
                            text: STR.STR_QML_666
                            verticalAlignment: Text.AlignVCenter
                            color: "#A66800"
                            font.family: "Lato"
                            font.pixelSize: 16
                            wrapMode: Text.WordWrap
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                        }
                    }

                    QTextInputBoxTypeB {
                        id: passphrase
                        label: STR.STR_QML_270
                        optional: "(optional)"
                        boxWidth: 540
                        boxHeight: 48
                        textweight: Font.Normal
                        isPassword: true
                        onTypingFinished: {
                            confirmpassphrases.isValid = true
                            confirmpassphrases.errorText = ""
                        }
                    }
                    QTextInputBoxTypeB {
                        id: confirmpassphrases
                        label: STR.STR_QML_271
                        boxWidth: 540
                        boxHeight: 48
                        isPassword: true
                        onTypingFinished: {
                            confirmpassphrases.isValid = true
                            confirmpassphrases.errorText = ""
                        }
                    }
                }
                Rectangle {
                    width: 728
                    height: 60
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 120
                    anchors.horizontalCenter: parent.horizontalCenter
                    radius: 8
                    color: "#EAEAEA"
                    Row {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8
                        QImage {
                            height: parent.height
                            width: height
                            source: "qrc:/Images/Images/info-60px.png"
                        }
                        QText {
                            height: parent.height
                            text: STR.STR_QML_272
                            color: "#031F2B"
                            font.family: "Lato"
                        }
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
                        mainloader.sourceComponent = nameConfiguration
                    }
                }
                Row {
                    spacing: 12
                    anchors {
                        right: parent.right
                        rightMargin: 40
                        bottom: parent.bottom
                        bottomMargin: 32
                    }
                    QTextButton {
                        width: 230
                        height: 48
                        label.text: STR.STR_QML_275
                        label.font.pixelSize: 16
                        type: eTypeE
                        enabled: rootAddsignerToWallet.signerNameInputted !== ""
                        onButtonClicked: {
                            confirmPrimaryKey.open()
                        }
                    }
                    QTextButton {
                        width: 200
                        height: 48
                        label.text: STR.STR_QML_273
                        label.font.pixelSize: 16
                        type: eTypeB
                        enabled: (passphrase.textInputted !== "") && (confirmpassphrases.textInputted !== "")
                        onButtonClicked: {
                            if(confirmpassphrases.textInputted !== passphrase.textInputted){
                                confirmpassphrases.isValid = false
                                confirmpassphrases.errorText = STR.STR_QML_274
                            }
                            else{
                                var signerObj = { "signername"    : rootAddsignerToWallet.signerNameInputted,
                                                  "passphrase"    : passphrase.textInputted};
                                QMLHandle.sendEvent(EVT.EVT_SOFTWARE_SIGNER_REQUEST_CREATE, signerObj)
                            }
                        }
                    }
                }
            }
        }
        Rectangle {
            id: stateAddSigner
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
                sourceComponent: stateAddSigner.visible ? progressAddSigner : null
            }
        }
        Component {
            id: progressAddSigner
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
                    text: STR.STR_QML_276
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
                    text: STR.STR_QML_277
                }
            }
        }
        Connections {
            target: AppModel
            onAddSignerPercentageChanged : {
                if(AppModel.addSignerPercentage === 100){
                    stateAddSigner.visible = false
                }
            }
        }
        QConfirmYesNoPopup{
            id:confirmPrimaryKey
            contentText: STR.STR_QML_642
            rightBtnLabel: STR.STR_QML_427
            leftBtnLabel: STR.STR_QML_035
            onConfirmYes: {
                var signerObj = {
                    "signername"    : rootAddsignerToWallet.signerNameInputted,
                    "passphrase"    : ""
                };
                QMLHandle.sendEvent(EVT.EVT_SOFTWARE_SIGNER_REQUEST_CREATE, signerObj)
            }
            onConfirmNo: close()
        }

        QPopupInfoTwoButtons {
            id: _info
            signal yesClicked()
            title: STR.STR_QML_661
            labels: [STR.STR_QML_433,STR.STR_QML_432]
            funcs: [
                function() { yesClicked() },
                function() {}
            ]
        }
        function showPopupInfo(isSoftware, fingerPrint){
            _info.contentText = STR.STR_QML_1283.arg(fingerPrint.toUpperCase())
            _info.contentTextTwo = ""
            _info.open()
        }
    }
}
