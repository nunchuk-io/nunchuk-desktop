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
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"
import "../../Components/customizes/Signers"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: signerConfigRoot
    readonly property int walletAddressType: AppModel.newWalletInfo.walletAddressType
    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_021
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION)
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
                currentStep: 2
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
                currentStep: 2
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
                currentStep: 2
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Rectangle {
            width: 269
            height: 454
            radius: 8
            color: Qt.rgba(255, 255, 255, 0.5)
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 139
            }
            QText {
                anchors {
                    left: parent.left
                    leftMargin: 16
                    top: parent.top
                    topMargin: 16
                }
                text: STR.STR_QML_043
                color: "#031F2B"
                font.pixelSize: 14
                font.weight: Font.Bold
            }
            Flickable {
                id: flickerSignerList
                width: 280
                height: 190
                clip: true
                flickableDirection: Flickable.VerticalFlick
                interactive: true
                contentHeight: contentDisplay.height
                ScrollBar.vertical: ScrollBar { active: true }
                anchors {
                    left: parent.left
                    leftMargin: 0
                    top: parent.top
                    topMargin: 45
                }
                Column {
                    id: contentDisplay
                    QListView {
                        id: mastersigners
                        width: 280
                        height: 50*mastersigners.count
                        clip: true
                        interactive: false
                        model: AppModel.masterSignerList
                        delegate: Rectangle {
                            id: delegateMastersigner
                            color: master_signer_checked ? Qt.rgba(0, 0, 0, 0.1): "transparent"
                            width: 269
                            height: 50
                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#C9DEF1"
                                anchors.bottom: parent.bottom
                            }
                            Row {
                                width: parent.width
                                spacing: 8
                                anchors {
                                    left: parent.left
                                    leftMargin: 8
                                    verticalCenter: parent.verticalCenter
                                }
                                QIcon {
                                    iconSize: 24
                                    source: master_signer_checked ? "qrc:/Images/Images/SignerChecked.png" : "qrc:/Images/Images/SignerUnChecked.png"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Column {
                                    width: parent.width - 40
                                    anchors.verticalCenter: parent.verticalCenter
                                    spacing: 4
                                    QText {
                                        width: 200
                                        elide: Text.ElideRight
                                        font.pixelSize: 14
                                        color: master_signer_checked ? "#9CAEB8" :  "#031F2B"
                                        text: master_signer_name
                                    }
                                    Item{
                                        width: parent.width
                                        height: 16
                                        QText {
                                            height: 16
                                            font.pixelSize: 12
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            text: "XFP: " + model.master_signer_fingerPrint
                                            font.capitalization: Font.AllUppercase
                                        }
                                        QRowSingleSignerType {
                                            isPrimaryKey: model.master_signer_primary_key
                                            signerType: model.master_signer_type
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.right: parent.right
                                        }
                                    }
                                }
                            }
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked:{
                                    if(master_signer_checked){
                                        if(!signerAssigned.containsSigner(master_signer_fingerPrint)){ model.master_signer_checked = false}
                                    }
                                    else{
                                        if(master_signer_type == NUNCHUCKTYPE.SOFTWARE || model.master_signer_primary_key){
                                            if (master_signer_need_passphrase) {
                                                var signerObj = {
                                                    "mastersigner_id"    : master_signer_id,
                                                    "mastersigner_index" : index
                                                };
                                                QMLHandle.sendEvent(EVT.EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE, signerObj)
                                            } else {
                                                _warning.model = model
                                                _warning.open()
                                            }
                                        }
                                        else{
                                            if(signerConfigRoot.walletAddressType === NUNCHUCKTYPE.TAPROOT){
                                                var assignedCnt = AppModel.newWalletInfo.walletSingleSignerAssigned.signerCount()
                                                var selectedCnt = AppModel.masterSignerList.signerSelectedCount() + AppModel.remoteSignerList.signerSelectedCount()
                                                console.log(Math.max(assignedCnt, selectedCnt))
                                                if(Math.max(assignedCnt, selectedCnt) === 0) { model.master_signer_checked = true }
                                            }
                                            else{ model.master_signer_checked = true}
                                        }
                                    }
                                }
                            }
                        }
                    }
                    QListView {
                        id: remotesigners
                        width: 280
                        height: 50*remotesigners.count
                        clip: true
                        interactive: false
                        model: AppModel.remoteSignerList
                        delegate: Rectangle {
                            id: delegateRemoteSigner
                            color: single_signer_checked ? Qt.rgba(0, 0, 0, 0.1): "transparent"
                            width: 269
                            height: 50
                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#C9DEF1"
                                anchors.bottom: parent.bottom
                            }
                            Row {
                                width: parent.width
                                spacing: 8
                                anchors {
                                    left: parent.left
                                    leftMargin: 8
                                    verticalCenter: parent.verticalCenter
                                }
                                QIcon {
                                    iconSize: 24
                                    source: single_signer_checked ? "qrc:/Images/Images/SignerChecked.png" : "qrc:/Images/Images/SignerUnChecked.png"
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Column {
                                    width: parent.width - 40
                                    anchors.verticalCenter: parent.verticalCenter
                                    spacing: 4
                                    QText {
                                        width: 200
                                        elide: Text.ElideRight
                                        font.pixelSize: 14
                                        color: single_signer_checked ? "#9CAEB8" :  "#031F2B"
                                        text: singleSigner_name
                                    }
                                    Item{
                                        width: parent.width
                                        height: 16
                                        QText {
                                            id: xfptext
                                            height: 16
                                            font.pixelSize: 12
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            text: "XFP: " + model.singleSigner_masterFingerPrint
                                            font.capitalization: Font.AllUppercase
                                        }
                                        QRowSingleSignerType {
                                            isPrimaryKey: model.single_signer_primary_key
                                            signerType: model.single_signer_type
                                            accountIndex: model.single_signer_account_index
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.right: parent.right
                                        }
                                    }
                                }
                            }
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked:{
                                    if(model.single_signer_checked){
                                        if(!signerAssigned.containsSigner(model.single_signer_checked)){ model.single_signer_checked = false}
                                    }
                                    else{
                                       if(signerConfigRoot.walletAddressType === NUNCHUCKTYPE.TAPROOT){
                                           var assignedCnt = AppModel.newWalletInfo.walletSingleSignerAssigned.signerCount()
                                           var selectedCnt = AppModel.masterSignerList.signerSelectedCount() + AppModel.remoteSignerList.signerSelectedCount()
                                           console.log(Math.max(assignedCnt, selectedCnt))
                                           if(Math.max(assignedCnt, selectedCnt) === 0) { model.single_signer_checked = true }
                                       }
                                       else{ model.single_signer_checked = true}
                                    }
                                }
                            }
                        }
                    }
                }
            }

            QIconTextButton {
                id: assignTowallet
                width: 161
                height: 32
                iconInRight: true
                type: eTypeB
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 257
                }
                label: STR.STR_QML_048
                icons: ["d_chevron_24px_031F2B.png","d_chevron_24px_9CAEB8.png","d_chevron_24px_F1FAFE.png","d_chevron_24px_F1FAFE.png"]
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER)
                    QMLHandle.sendEvent(EVT.EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER)
                }
            }

            QText {
                anchors {
                    left: parent.left
                    leftMargin: 16
                    top: parent.top
                    topMargin: 337
                }
                text: STR.STR_QML_049
                font.family: "Montserrat"
                color: "#031F2B"
                font.weight: Font.DemiBold
                font.pixelSize: 14
            }

            QButtonMedium {
                id: addNewWallet
                width: 160
                height: 32
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 400
                }
                type: eOUTLINE_NORMAL
                label: STR.STR_QML_050
                fontPixelSize: 10
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ADD_SIGNER_TO_WALLET_REQUEST)
                }
            }
            QText {
                text: STR.STR_QML_051
                anchors.horizontalCenter: addNewWallet.horizontalCenter
                anchors.bottom: addNewWallet.top
                anchors.bottomMargin: 4
                font.pixelSize: 10
                font.family: "Lato"
                color: "#35ABEE"
                visible: false
            }
        }
        QText {
            anchors {
                left: parent.left
                leftMargin: 349
                top: parent.top
                topMargin: 147
            }
            font.pixelSize: 16
            font.weight: Font.Bold
            color: "#031F2B"
            text: STR.STR_QML_052
        }
        Row {
            anchors {
                left: parent.left
                leftMargin: 349
                top: parent.top
                topMargin: 184
            }
            width: 187
            height: 21
            spacing: 4
            QText {
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 10
                font.weight: Font.DemiBold
                color: "#031F2B"
                text: STR.STR_QML_053
            }
            QText {
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 16
                font.weight: Font.DemiBold
                color: "#35ABEE"
                text: AppModel.newWalletInfo.walletN
            }
            QText {
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
                color: "#323E4A"
                text: STR.STR_QML_054
            }
        }
        QListView {
            id: signerAssigned
            width: 418
            height: 195
            spacing: 8
            clip: true
            anchors {
                left: parent.left
                leftMargin: 341
                top: parent.top
                topMargin: 216
            }
            ScrollBar.vertical: ScrollBar { active: true }
            model: AppModel.newWalletInfo.walletSingleSignerAssigned
            delegate: Item {
                id: signerAssigneddlg
                property string sigerXfp: model.singleSigner_masterFingerPrint
                property bool isRemoteSigner: model.single_signer_type === NUNCHUCKTYPE.AIRGAP
                width: 418
                height: signerAssigneddlg.isRemoteSigner ? 73 : 53
                Rectangle {
                    id: rect
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 415
                    height: signerAssigneddlg.isRemoteSigner ? 70 : 50
                    color: Qt.rgba(255, 255, 255)
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
                    color: "#C9DEF1"
                    anchors {
                        left: parent.left
                        leftMargin: 16
                        verticalCenter: rect.verticalCenter
                    }
                }
                Column {
                    id: txt
                    width: 350
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
                            width: parent.width - (signerAssigneddlg.isRemoteSigner ? 70 : 20)
                            height: 21
                            verticalAlignment: Text.AlignVCenter
                            text: model.singleSigner_name
                            font.pixelSize: 14
                            font.weight: Font.DemiBold
                            font.family: "Montserrat"
                            color: "#031F2B"
                            anchors.verticalCenter: parent.verticalCenter
                            elide: Text.ElideRight
                        }
                    }
                    Item{
                        width: parent.width
                        height: 16
                        Row{
                            height: 16
                            spacing: 5
                            QText {
                                width: paintedWidth
                                height: 16
                                font.pixelSize: 12
                                color: "#031F2B"
                                font.family: "Lato"
                                text: "XFP: " + model.singleSigner_masterFingerPrint
                                font.capitalization: Font.AllUppercase
                            }
                            QRowSingleSignerType {
                                isPrimaryKey: model.single_signer_primary_key
                                signerType: model.single_signer_type
                                accountIndex: model.single_signer_account_index
                            }
                        }
                    }
                    QText {
                        width: parent.width
                        height: signerAssigneddlg.isRemoteSigner ? 16 : 0
                        visible: signerAssigneddlg.isRemoteSigner
                        font.pixelSize: 10
                        color: "#839096"
                        font.family: "Lato"
                        text: STR.STR_QML_055.arg(model.singleSigner_derivationPath)
                    }
                }
                QImage {
                    anchors {
                        right: parent.right
                        rightMargin: 20
                        verticalCenter: indicatiorValid.verticalCenter
                    }
                    width: 32
                    height: 32
                    source: "qrc:/Images/Images/Delete.png"
                    MouseArea {
                        id: dlgRightMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            QMLHandle.sendEvent(EVT.EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER, index)
                        }
                    }
                }
            }

            function containsSigner(xfp){
                for(var i = 0; i < signerAssigned.count; i++){
                    var signerxfp = signerAssigned.contentItem.children[i].sigerXfp
                    if(signerxfp.toUpperCase() === xfp.toUpperCase()){ return true;}
                }
                return false;
            }
        }

        Rectangle {
            width: 419
            height: 132
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 111
            }
            color: Qt.rgba(201, 222, 241, 0.5)
            Row {
                id: requiredNumberInfo
                height: 21
                anchors {
                    left: parent.left
                    leftMargin: 8
                    top: parent.top
                    topMargin: 14
                }
                spacing: 4
                QText {
                    height: 21
                    font.pixelSize: 14
                    font.weight: Font.Bold
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#323E4A"
                    text: STR.STR_QML_056
                }
                Rectangle{
                    width: _config.paintedWidth + 10*2
                    height: 21
                    radius: 4
                    color: "#F1FAFE"
                    QText {
                        id:_config
                        height: 21
                        width: paintedWidth
                        anchors.centerIn: parent
                        text: {
                            var ret = ""
                            if(AppModel.newWalletInfo.walletM === 0){
                                ret = STR.STR_QML_557
                            }
                            else if(AppModel.newWalletInfo.walletN === 1 && AppModel.newWalletInfo.walletM === 1){
                                ret = STR.STR_QML_558
                            }
                            else if(AppModel.newWalletInfo.walletN > 1 && AppModel.newWalletInfo.walletM >= 1){
                                ret = STR.STR_QML_559.arg(AppModel.newWalletInfo.walletM).arg(AppModel.newWalletInfo.walletN)
                            }
                            return ret;
                        }
                        font.weight: Font.Bold
                        font.pixelSize: 14
                        font.family: "Lato"
                        color: "#323E4A"

                    }
                }
            }
            QText {
                anchors {
                    left: parent.left
                    leftMargin: 8
                    top: requiredNumberInfo.bottom
                }
                text: STR.STR_QML_058
                font.pixelSize: 12
                font.family: "Lato"
                color: "#031F2B"
            }

            Rectangle {
                width: 120
                height: 48
                anchors {
                    left: parent.left
                    leftMargin: 8
                    top: parent.top
                    topMargin: 63
                }
                color: Qt.rgba(255, 255, 255, 0.5)
                border.color: "#C9DEF1"
                radius: 4
                Row {
                    height: 21
                    anchors.centerIn: parent
                    spacing: 10
                    Rectangle {
                        width: 21
                        height: 21
                        radius: 21
                        color: minus.pressed ? "#031F2B" : "#F1FAFE"
                        border.color: minus.pressed ?"#F1FAFE" : "#031F2B"
                        Rectangle {
                            width: 8
                            height: 2
                            color: minus.pressed ?"#F1FAFE" : "#031F2B"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            id: minus
                            anchors.fill: parent
                            onClicked: {
                                if(AppModel.newWalletInfo.walletM > 0){
                                    AppModel.newWalletInfo.walletM--;
                                }
                            }
                        }
                    }
                    Item {
                        width: 21
                        height: 21
                        QText {
                            anchors.centerIn: parent
                            font.pixelSize: 16
                            font.weight: Font.DemiBold
                            color: "#031F2B"
                            text: AppModel.newWalletInfo.walletM
                        }
                    }
                    Rectangle {
                        width: 21
                        height: 21
                        radius: 21
                        color: plusbtn.pressed ? "#031F2B" : "#F1FAFE"
                        border.color: plusbtn.pressed ?"#F1FAFE" : "#031F2B"
                        Rectangle {
                            width: 8
                            height: 2
                            color: plusbtn.pressed ?"#F1FAFE" : "#031F2B"
                            anchors.centerIn: parent
                        }
                        Rectangle {
                            width: 2
                            height: 8
                            color: plusbtn.pressed ?"#F1FAFE" : "#031F2B"
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            id: plusbtn
                            anchors.fill: parent
                            onClicked: {
                                if(AppModel.newWalletInfo.walletM < signerAssigned.count){
                                    AppModel.newWalletInfo.walletM++;
                                }
                            }
                        }
                    }
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
                QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK)
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
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION)
            }
        }
        QTextButton {
            width: 200
            height: 48
            label.text: STR.STR_QML_060
            label.font.pixelSize: 16
            type: eTypeE
            enabled: (AppModel.newWalletInfo.walletM > 0) && (signerAssigned.count > 0) && (AppModel.newWalletInfo.walletM <= signerAssigned.count)
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_SIGNER_CONFIGURATION_TRY_REVIEW)
            }
        }
        QPopupInfoTwoButtons {
            id: _warning
            property var model
            title: STR.STR_QML_661
            contentText: STR.STR_QML_669
            labels: [STR.STR_QML_670,STR.STR_QML_035]
            funcs: [
                function() { model.master_signer_checked = true; },
                function() {}
            ]
        }
    }
}
