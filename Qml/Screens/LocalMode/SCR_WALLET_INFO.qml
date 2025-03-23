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
import QRCodeItem 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/QRCodes"
import "../../Components/customizes/Popups"
import "../../Components/customizes/services"
import "../../Components/customizes/Wallets"
import "../../../localization/STR_QML.js" as STR
QScreen {
    property bool isDelete: AppModel.walletInfo.isDeleting
    readonly property int eWALLET_CONFIG: 1
    readonly property int eWALLET_SET_ALIAS: 2
    readonly property int eWALLET_CHANGE_ALIAS: 3
    readonly property int eWALLET_PRIMARY_OWNER: 4
    property int eFlow: 1
    Loader {
        id: contenCenter
        anchors.centerIn: parent
        sourceComponent: {
            if (eFlow === eWALLET_CONFIG) {
                return isDelete ? _security_question : wallet_config
            }
            else if (eFlow === eWALLET_SET_ALIAS || eFlow === eWALLET_CHANGE_ALIAS) {
                return wallet_alias
            }
            else if (eFlow === eWALLET_PRIMARY_OWNER) {
                return wallet_primary_owner
            }
            else {
                return isDelete ? _security_question : wallet_config
            }
        }
    }

    Component {
        id: wallet_config
        QWalletConfig {
        }
    }

    Component {
        id: wallet_alias
        QWalletAlias {
        }
    }

    Component {
        id: wallet_primary_owner
        QWalletPrimaryOwner {

        }
    }

    Component {
        id: _security_question
        QAnswerSecurityQuestion {
            onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
            onPrevClicked: {
                AppModel.walletInfo.isDeleting = false
            }
            onNextClicked: {
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_REMOVE, AppModel.walletInfo)
            }
        }
    }

    QConfirmYesNoPopup {
        id: confirmDeleteWallet
        title: STR.STR_QML_661
        contentText: STR.STR_QML_662
        onConfirmNo: close()
        onConfirmYes: {
            close()
            QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_REMOVE, AppModel.walletInfo)
        }
    }
    QQrExportResultPDF {
        id: qrcodeExportResult
        model: AppModel.qrExported
    }
    FileDialog {
        id: exportwalletDialog
        property int exportFormat: -1
        fileMode: FileDialog.SaveFile
        onAccepted: {
            if(exportFormat === NUNCHUCKTYPE.DB){
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_DB, exportwalletDialog.currentFile)
            }
            else if(exportFormat === NUNCHUCKTYPE.DESCRIPTOR){
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_DESCRIPTOR, exportwalletDialog.currentFile)
            }
            else if(exportFormat === NUNCHUCKTYPE.COLDCARD){
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_COLDCARD, exportwalletDialog.currentFile)
            }
            else if(exportFormat === NUNCHUCKTYPE.TRANSACTION_CSV){
                var csvTxObj = { "csv_type"     : 1,
                                 "file_path"    : exportwalletDialog.currentFile};
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_CSV, csvTxObj)
            }
            else if(exportFormat === NUNCHUCKTYPE.UTXO_CSV){
                var csvUtxoObj = { "csv_type"     : -1,
                                   "file_path"    : exportwalletDialog.currentFile};
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_CSV, csvUtxoObj)
            }
            else{}
        }
    }
    FileDialog {
        id: openfileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_IMPORT_PSBT, openfileDialog.file)
        }
    }
    QPopupDisplayAddressBusyBox {
        id: displayAddressBusybox
        width: parent.width
        height: parent.height
    }

    /*=================Force refresh (advanced)=================*/
    QConfirmYesNoPopup {
        id: forceRefresh
        title: STR.STR_QML_334
        contentText: AppModel.walletInfo.isAssistedWallet ? STR.STR_QML_689 : STR.STR_QML_689_free
        onConfirmNo: close()
        onConfirmYes: {
            close()
            forceRefreshBusybox.open()
            QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_REFRESH_WALLET_REQUEST,AppModel.walletInfo.walletId);
        }
    }
    QPopupForceRefreshBusybox {
        id: forceRefreshBusybox
    }

    QPopupToast{
        id:_warning
        x:contenCenter.x + 36
        y:contenCenter.y + 520
    }
    QPopupGapLimit {
        id: gaplimit
    }

    /*==========================================================*/
    Connections {
        target: AppModel
        onStartDisplayAddress: {
            if(isOnTop) displayAddressBusybox.open()
            else displayAddressBusybox.close()
        }
        onFinishedDisplayAddress: {
            displayAddressBusybox.close()
        }
    }
}

