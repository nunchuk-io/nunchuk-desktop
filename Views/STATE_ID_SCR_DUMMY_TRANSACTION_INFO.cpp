/************************************************************************* 
* This file is part of the Nunchuk software (https://nunchuk.io/)        * 
* Copyright (C) 2020-2022 Enigmo                                         * 
* Copyright (C) 2022 Nunchuk                                             * 
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

#include "STATE_ID_SCR_DUMMY_TRANSACTION_INFO.h"
#include "bridgeifaces.h"
#include "AppModel.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QGroupWalletHealthCheck.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QUserWalletDummyTx.h"

void SCR_DUMMY_TRANSACTION_INFO_Entry(QVariant msg) {

}

void SCR_DUMMY_TRANSACTION_INFO_Exit(QVariant msg) {

}

void EVT_DUMMY_TRANSACTION_INFO_BACK_HANDLER(QVariant msg) {

}

void EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST_HANDLER(QVariant msg)
{
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    if (auto w = AppModel::instance()->walletInfoPtr()) {
        DBG_INFO << type;
        if (type == "address-to-verify") {
            QString address = maps["address"].toString();
            AppModel::instance()->startDisplayAddress(w->walletId(), address);
        }
        else if (type == "memo-notify") {
            //Not use for dummy tx
        }
        else if (type == "scan-device") {
            AppModel::instance()->startScanDevices(E::STATE_ID_SCR_TRANSACTION_INFO);
        }
        else if (type == "dummy-tx-sign") {
            QtConcurrent::run([maps, w]() {
                QString xfp = maps["xfp"].toString();
                if (AppModel::instance()->isSignIn()) {
                    AppModel::instance()->SignInRequestSignTx(xfp);
                }
                else {
                    if (auto dummy = w->groupDummyTxPtr()) {
                        dummy->requestSignTx(xfp);
                    }
                }
            });
        }
        else if (type == "dummy-tx-import-qr") {
            QStringList tags = maps["tags"].toStringList();
            if (!tags.isEmpty()) {
                if (AppModel::instance()->isSignIn()) {
                    AppModel::instance()->SignInRequestSignTxViaQR(tags);
                }
                else {
                    if (auto dummy = w->groupDummyTxPtr()) {
                        dummy->requestSignTxViaQR(tags);
                    }
                }
            }
        }
        else if (type == "dummy-tx-import") {
            QString file = maps["file"].toString();
            QString file_path = qUtils::QGetFilePath(file);
            if(file_path != ""){
                if (AppModel::instance()->isSignIn()) {
                    AppModel::instance()->SignInRequestSignTxViaFile(file_path);
                }
                else {
                    if (auto dummy = w->groupDummyTxPtr()) {
                        dummy->requestSignTxViaFile(file_path);
                    }
                }
            }
        }
        else if (type == "dummy-tx-export") {
            QString file = maps["file"].toString();
            QString file_path = qUtils::QGetFilePath(file);
            if(file_path != ""){
                if (auto dummy = w->groupDummyTxPtr()) {
                    dummy->ExportPsbtViaFile(file_path);
                }
            }
        }
        else if (type == "register-wallet") {
            QGroupWallets::instance()->dashboardInfoPtr()->setFlow((int)AlertEnum::E_Alert_t::GROUP_WALLET_SETUP);
            if (QGroupWallets::instance()->dashboardInfoPtr()->register_wallet()) {
                QEventProcessor::instance()->sendEvent(E::EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST);
            }
        }
        else if (type == "dummy-tx-export-qr") {
            AppModel::instance()->setQrExported(QStringList());
            QWarningMessage msgwarning;
            QStringList qrtags {};
            if (auto dummy = w->groupDummyTxPtr()) {
                if (AppModel::instance()->isSignIn()) {
                    qrtags = AppModel::instance()->ExportPsbtViaQR();
                }
                else {
                    qrtags = dummy->ExportPsbtViaQR(msgwarning);
                }
            }
            if(!qrtags.isEmpty()){
                AppModel::instance()->setQrExported(qrtags);
            }
            else{
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
            }
        }
        else if (type == "force-sync-dummy-tx") {
            if (auto dummy = w->groupDummyTxPtr()) {
                dummy->requestForceSyncTx(w->groupId(), w->walletId(), dummy->tx_id());
            }
        }
        else if (type == "dummy-tx-export-bbqr") {
            AppModel::instance()->setQrExported(QStringList());
            QWarningMessage msgwarning;
            QStringList qrtags {};
            if (auto dummy = w->groupDummyTxPtr()) {
                if (AppModel::instance()->isSignIn()) {
                    qrtags = AppModel::instance()->ExportPsbtViaBBQR();
                }
                else {
                    qrtags = dummy->ExportPsbtViaBBQR(msgwarning);
                }
            }
            if(!qrtags.isEmpty()){
                AppModel::instance()->setQrExported(qrtags);
            }
            else{
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type() );
            }
        }
        else{}
    }
}
