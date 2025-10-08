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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Popups"
import "../../../Components/customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_126
    property string signerName: ""
    property string signerSpec: ""
    property string signerXfp: ""
    property string signerDerivationPath: ""
    property string signerCardId: ""
    property int    signerType: 0
    property bool   isPrimaryKey: false
    property string messageToSign: ""
    property string messageToSignSHA256: ""
    property bool   isHardwareSigner: (signerType === NUNCHUCKTYPE.HARDWARE)
    property bool isConnected: false
    property bool isTopXPubs: true
    property string masterSignerId: ""
    property bool needbackup: false
    property var signerInfo

    signal requestGetExpubs()
    signal requestHealthCheck()
    signal requestDeleteKey()
    signal healthReminderClicked()
    signal healthReminderEditClicked(var edit)
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
    content: Item {
        Row {
            spacing: 36
            QSignerInfo {
                height: _remider.height
            }
            QRemiderAndHistory {
                id: _remider
                height: needbackup ? 400 : 452
                onReminderClicked: {
                    healthReminderClicked()
                }
            }
        }
        QWarningBg {
            width: 728
            iSize: 36
            icon: "qrc:/Images/Images/warning-dark.svg"
            txt.text: STR.STR_QML_1733
            txt.textFormat: Text.RichText
            anchors.bottom: parent.bottom
            color: "#FDEBD2"
            visible: needbackup
            onHyperlinkClicked: {
                SignerManagement.requestBackupHotKey(masterSignerId)
            }
        }        
    }

    property int    state_id: EVT.STATE_ID_SCR_ADD_HARDWARE
    bottomLeft: Item {
        height: 48
        width: 300
        id: _row
        Row {
            spacing: 0
            visible: isHardwareSigner
            QIconTextButton {
                id: btn
                width: isConnected ? 98 : 114
                height: 24
                label: isConnected ? STR.STR_QML_1216 : STR.STR_QML_1217
                icons: isConnected ? ["connected.svg", "connected.svg", "connected.svg","connected.svg"]
                                   : ["disconnected.svg", "disconnected.svg", "disconnected.svg","disconnected.svg"]
                anchors.verticalCenter: _row.verticalCenter
                fontPixelSize: 12
                iconSize: 16
                type: eTypeB
                onButtonClicked: {

                }
            }
            QRefreshButtonA {
                width: 114
                height: 48
                anchors.verticalCenter: btn.verticalCenter
                visible: !isConnected
                label: STR.STR_QML_189
                fontPixelSize: 16
                color: "transparent"
                border.color: "transparent"
                onButtonClicked: AppModel.startScanDevices(state_id)
            }
        }
    }

    bottomRight: Row{
        spacing: 12
        QButtonLargeTail {
            id: otheraction
            width: 102
            height: 48
            label: STR.STR_QML_1094
            type: eSECONDARY
            optionVisible: othersContextMenu.visible
            layoutDirection: Qt.RightToLeft
            onButtonClicked: {
                othersContextMenu.x = 20
                othersContextMenu.y = 20 - othersContextMenu.height
                othersContextMenu.open()
            }
            QContextMenu {
                id: othersContextMenu
                menuWidth: 300
                labels: {
                    var ls = [];
                    ls.push(STR.STR_QML_1208)
                    ls.push(STR.STR_QML_560)
                    ls.push(STR.STR_QML_241)
                    return ls
                }
                icons:{
                    var ls = [];
                    ls.push("")
                    ls.push("")
                    ls.push("")
                    return ls
                }
                enables: [true, true, true]
                visibles: [ (signerType === NUNCHUCKTYPE.SOFTWARE || signerType === NUNCHUCKTYPE.HARDWARE),
                    isTopXPubs,
                    signerType !== NUNCHUCKTYPE.FOREIGN_SOFTWARE
                ]
                functions: {
                    var ls = [];
                    ls.push(function(){
                        AppModel.startScanDevices(state_id)
                        action_type = eSIGN_MESSAGE
                    })
                    ls.push(function(){ requestGetExpubs() })
                    ls.push(function(){
                        var ret = AppModel.walletList.removeOrNot(signerXfp, signerDerivationPath)
                        if(signerType === NUNCHUCKTYPE.FOREIGN_SOFTWARE){
                            _info.use_nfc_to_health_check()
                        }
                        else if(ret.used_in_assisted_wallet) {
                            _info.use_key_used_in_assisted()
                        }
                        else if (ret.used_in_hot_wallet) {
                            _confirm.requestDeleteKeyInHotWallet()
                        }
                        else if (ret.used_in_free_wallet) {
                            _confirm.requestDeleteKeyInFreeWallet()
                        }
                        else {
                            _confirm.requestDeleteKeyNotUsedInWallet()
                        }
                    })

                    return ls
                }
                colors:{
                    var ls = [];
                    ls.push("#031F2B")
                    ls.push("#031F2B")
                    ls.push("#CF4018")
                    return ls
                }
                onItemClicked: {
                    functions[index]()
                }
            }
        }

        QTextButton {
            id: btnhealthCheck
            width: label.paintedWidth + 2*20
            height: 48
            type: eTypeB
            label.text: STR.STR_QML_129
            label.font.pixelSize: 16
            enabled: {
                if (signerType === NUNCHUCKTYPE.AIRGAP) {
                    var list = signerInfo.getWalletList()
                    return list.length > 0
                }
                return true
            }

            onButtonClicked: {
                if (signerType === NUNCHUCKTYPE.NFC) {
                    _info.open()
                    return
                }
                walletList = signerInfo.getWalletList()
                if (walletList.length > 0) {
                    _select_wallet.open()
                } else {
                    requestHealthCheck()
                }
            }
        }
    }
    property var walletList:[]
    /*=============================Popup=============================*/
    QPopupInfo{
        id:_info
        title: STR.STR_QML_339
        contentText: STR.STR_QML_998
        usingMin: true
        function use_nfc_to_health_check() {
            _info.title = STR.STR_QML_339
            _info.contentText = STR.STR_QML_998
            _info.open()
        }
        function use_key_used_in_assisted() {
            _info.title = STR.STR_QML_024
            _info.contentText = STR.STR_QML_554
            _info.open()
        }
        function use_key_is_foreign_software() {
            _info.title = STR.STR_QML_024
            _info.contentText = STR.STR_QML_555
            _info.open()
        }
    }

    QConfirmYesNoPopup{
        id:_confirm
        property var action: function() { requestDeleteKey() }
        contentText:STR.STR_QML_243
        onConfirmNo: close()
        onConfirmYes: {
            close()
            if (action) {
                action()
            }            
        }
        function requestDeleteKeyInHotWallet() {
            _confirm.contentText = STR.STR_QML_243_used
            _confirm.action = function() {
                _confirm.action = function() { requestDeleteKey() }
                _confirm.contentText = STR.STR_QML_1772
                _confirm.open()
            }
            _confirm.open()
        }
        function requestDeleteKeyInFreeWallet() {
            _confirm.action = function() { requestDeleteKey() }
            _confirm.contentText = STR.STR_QML_243_used
            _confirm.open()
        }
        function requestDeleteKeyNotUsedInWallet() {
            _confirm.action = function() { requestDeleteKey() }
            _confirm.contentText = STR.STR_QML_243
            _confirm.open()
        }
    }

    QPopupBusyLoading{
        id:_busyTopUp
        warningText1:STR.STR_QML_582
    }

    QPopupEmpty {
        id: _select_wallet
        content: QOnScreenContentTypeB {
            width: 600
            height: 516
            anchors.centerIn: parent
            label.text: STR.STR_QML_837
            extraHeader: Item {}
            onCloseClicked: { _select_wallet.close() }
            content: Flickable {
                width: parent.width
                height: parent.height
                clip: true
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    spacing: 16
                    Repeater {
                        model: walletList
                        QAssistedWalletDelegate {
                            id: dele
                            width: 528
                            height: dele.visible ? 92 : 0
                            walletName: modelData.wallet_name
                            walletBalance: modelData.wallet_Balance
                            walletCurrency: modelData.wallet_Balance_Currency
                            walletM: modelData.wallet_M
                            walletN: modelData.wallet_N
                            isLocked: modelData.wallet_dashboard ? (modelData.wallet_dashboard.isLocked || modelData.wallet_isLocked || modelData.wallet_isReplaced) : false
                            walletType: modelData.wallet_walletType
                            onButtonClicked: {
                                _select_wallet.close()
                                modelData.wallet_dashboard.requestHealthCheck(signerXfp)
                            }
                        }
                    }
                }
            }

            isShowLine: true
            onPrevClicked: { _select_wallet.close() }
            bottomRight: Item {}
        }
    }

    QPopupInfoTwoButtons {
        id: _syncing_wallet_remove_key
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1290
        labels: [STR.STR_QML_035, STR.STR_QML_1291]
        isVertical: false
        funcs: [
            function() {
                AppModel.confirmSyncingWalletFromServer(false, true)
            },
            function() {
                AppModel.confirmSyncingWalletFromServer(true, false)
            }
        ]
    }
    /*=============================Popup=============================*/

    Rectangle {
        id: busyOverlay
        visible: false
        anchors.fill: parent
        anchors.bottom: parent.bottom
        radius: 24
        MouseArea {
            anchors.fill: parent
            onClicked: {}
        }
        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 24
                top: parent.top
                topMargin: 24
            }
            onClicked: {
                busyOverlay.visible = false
            }
        }
        QButtonTextLink {
            height: 24
            label: STR.STR_QML_059
            anchors {
                left: parent.left
                leftMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            onButtonClicked: {
                busyOverlay.visible = false
            }
        }

        Loader {
            id: busyIndi
            anchors.centerIn: parent
            sourceComponent: createBusy
        }
        Component {
            id: createBusy
            Column {
                spacing: 16
                Item {
                    width: 48
                    height: 48
                    QBusyIndicator {
                        width: 48
                        height: 48
                        anchors.centerIn: parent
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Column {
                    spacing: 24
                    QLato {
                        width: 376
                        height: 28
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 16
                        font.weight: Font.Normal
                        text: STR.STR_QML_140
                    }

                    Rectangle {
                        width: 222
                        height: 60
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "#EAEAEA"
                        radius: 12
                        QLato {
                            width: parent.width - 50
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 14
                            wrapMode: Text.WrapAnywhere
                            text: messageToSign
                        }
                    }
                }
            }
        }
    }
    Connections {
        target: AppModel
        onStartHealthCheckMasterSigner: {
            busyOverlay.visible = true
        }
        onFinishedHealthCheckMasterSigner : {
            busyOverlay.visible = false
            if (isResult) { 
                AppModel.showToast(0, STR.STR_QML_155.arg(signerName), EWARNING.SUCCESS_MSG);
            }
        }
        onStartTopXPUBsMasterSigner:{
            _busyTopUp.open()
        }
        onFinishedTopXPUBsMasterSigner:{
            _busyTopUp.close()
        }
        onFinishedHealthCheckRemoteSigner: {
            busyOverlay.visible = false
            if (isResult) { 
                AppModel.showToast(0, STR.STR_QML_155.arg(signerName), EWARNING.SUCCESS_MSG);
            }
        }
        onSyncingConfirmWalletRemoveKey: {
            _syncing_wallet_remove_key.contentText = STR.STR_QML_1290.arg(fingerPrint.toUpperCase())
            _syncing_wallet_remove_key.open()
        }
    }
}

