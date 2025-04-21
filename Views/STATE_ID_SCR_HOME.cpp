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
#include "STATE_ID_SCR_HOME.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Servers/Draco.h"
#include <QClipboard>
#include "localization/STR_CPP.h"
#include "ifaces/Chats/matrixbrigde.h"
#include "ProfileSetting.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QUserWallets.h"
#include "Premiums/QSharedWallets.h"
#include "Premiums/QGroupWalletHealthCheck.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QRecurringPayment.h"
#include "Premiums/QSignerManagement.h"
#include "Premiums/GroupSandboxModel.h"
#include "QPDFPrinter.h"

void SCR_HOME_Entry(QVariant msg) {
    AppModel::instance()->setTabIndex((int)ENUNCHUCK::TabSelection::WALLET_TAB);
    ProfileSetting::instance()->createCurrencies();
    AppModel::instance()->requestSyncGroups();
    if(CLIENT_INSTANCE->rooms()){
        CLIENT_INSTANCE->rooms()->stopCountdown();
    }
    timeoutHandler(3000,[]{
        auto wallet = AppModel::instance()->walletInfoPtr();
        if (wallet) {
            if (wallet.data()->isGlobalGroupWallet()) {
                wallet.data()->GetReplaceGroups();
            }
        }
    });
    AppModel::instance()->requestCreateUserWallets();
}

void SCR_HOME_Exit(QVariant msg) {

}

void EVT_HOME_WALLET_SELECTED_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    QString group_id = maps["group_id"].toString();
    QString wallet_id = maps["wallet_id"].toString();
    DBG_INFO << type << "group:" << group_id;
    if (qUtils::strCompare(type, "selected")) {
        int index = maps["data"].toInt();
        if (index >= 0) {
            AppModel::instance()->setWalletListCurrentIndex(index);
        }
        QString myRole = AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->dashboard() ? AppModel::instance()->walletInfo()->dashboard()->myRole() : "";
        DBG_INFO << myRole;
        if(qUtils::strCompare(myRole, "KEYHOLDER_LIMITED")){
            QGroupWallets::instance()->dashboard(group_id, wallet_id);
        }
        else {
            if (QGroupWallets::instance()->dashboardInfoPtr()) {
                QGroupWallets::instance()->dashboardInfoPtr()->setShowDashBoard(false);
            }
            QGroupWallets::instance()->setDashboardInfo(group_id);
        }
        if (auto w = AppModel::instance()->walletInfo()) {
            w->setIsViewCoinShow(false);
            if (w->isGlobalGroupWallet()) {
                w->GetReplaceGroups();
            }
        }
    }
    else if (qUtils::strCompare(type, "dashboard")) {
        QGroupWallets::instance()->dashboard(group_id, wallet_id);
    }
    else if (qUtils::strCompare(type, "wallet_dashboard")) {
        QGroupWallets::instance()->dashboard(group_id, wallet_id);
        int index = maps["data"].toInt();
        if (index >= 0) {
            AppModel::instance()->setWalletListCurrentIndex(index);
        }
    }
    else if (qUtils::strCompare(type, "deny")) {
        QGroupWallets::instance()->deny(group_id);
    }
    else if (qUtils::strCompare(type, "accept")) {
        QGroupWallets::instance()->accept(group_id);
    }
    else if (qUtils::strCompare(type, "reset")) {
        QGroupWallets::instance()->reset(group_id);
    }
    else if (qUtils::strCompare(type, "setup-group-wallet")) {
        QSharedWallets::instance()->GetGroup(group_id);
    }
    else if (qUtils::strCompare(type, "show-manage-chat-history")) {
        if (auto w = AppModel::instance()->walletInfoPtr()) {
            w->GetGroupWalletConfig();
        }
    }
    else{}
}

void EVT_HOME_ADD_WALLET_REQUEST_HANDLER(QVariant msg) {
    QWalletPtr newWallet(new Wallet());
    newWallet.data()->setCapableCreate(false);
    newWallet.data()->setWalletAddressType((int)nunchuk::AddressType::NATIVE_SEGWIT);
    AppModel::instance()->setNewWalletInfo(newWallet);
    AppModel::instance()->resetSignersChecked();
    AppModel::instance()->setSingleSignerInfo(QSingleSignerPtr(new QSingleSigner()));
    QEventProcessor::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_ADD_WALLET);
}

void EVT_HOME_MASTER_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {
    DBG_INFO << msg;
    QString masterFingerPrint = msg.toString();
    QMasterSignerPtr it = bridge::nunchukGetMasterSignerFingerprint(masterFingerPrint);
    AppModel::instance()->setMasterSignerInfo(it);
    AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByMasterSigner(AppModel::instance()->masterSignerInfo()->id()));
    if (auto signer = AppModel::instance()->masterSignerInfoPtr()) {
        signer->GetHistorySignerList();
        if (auto wallet = AppModel::instance()->walletInfoPtr()) {
            if (auto dashboard = wallet->dashboard()) {
                if (auto health = dashboard->healthPtr()) {
                    health->setKeyXfp(signer->fingerPrint());
                }
            }
        }
    }
}

void EVT_HOME_REMOTE_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {
    DBG_INFO << msg;
    QString masterFingerPrint = msg.toString();
    QSingleSignerPtr it = bridge::nunchukGetRemoteSigner(masterFingerPrint);
    if(it) {
        AppModel::instance()->setSingleSignerInfo(it);
        AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByFingerPrint(it.data()->masterFingerPrint()));
    }
    if (auto signer = AppModel::instance()->singleSignerInfoPtr()) {
        signer->GetHistorySignerList();
        if (auto wallet = AppModel::instance()->walletInfoPtr()) {
            if (auto dashboard = wallet->dashboard()) {
                if (auto health = dashboard->healthPtr()) {
                    health->setKeyXfp(signer->masterFingerPrint());
                }
            }
        }
    }
}

void EVT_HOME_SEND_REQUEST_HANDLER(QVariant msg) {
    if(auto w = AppModel::instance()->walletInfo()){
        QString rollover_address = msg.toString();
        if(rollover_address != "" && rollover_address != "selected"){
            w->setTranReplace(true);
            timeoutHandler(100, [=](){
                emit AppModel::instance()->walletInfo()->rollOverProcess(rollover_address);
            });
            w->RequestGetCoins();
        }
        else {
            w->setTranReplace(false);
            w->setReuse(msg.toString() == "selected");// w->unUseAddress();//
            w->RequestGetCoins();
            timeoutHandler(100, [=](){
                if (msg.toString() == "selected") {
                    emit w->requestCreateTransaction("");
                }
            });
        }
    }
}

void EVT_HOME_RECEIVE_REQUEST_HANDLER(QVariant msg) {

}

void EVT_HOME_TRANSACTION_HISTORY_REQUEST_HANDLER(QVariant msg) {

}

void EVT_HOME_WALLET_INFO_REQUEST_HANDLER(QVariant msg) {
    if(auto w = AppModel::instance()->walletInfo()){
        w->setIsDeleting(false);
        QGroupWallets::instance()->setDashboardInfo(w->groupId());
        w->GetWalletAlias();
    }
}

void EVT_HOME_WALLET_COPY_ADDRESS_HANDLER(QVariant msg) {
    if(!msg.toString().isEmpty()){
        qApp->clipboard()->setText(msg.toString());
    }
}

void EVT_HOME_TRANSACTION_INFO_REQUEST_HANDLER(QVariant msg) {
    qApp->setOverrideCursor(Qt::WaitCursor);
    QString txid = msg.toString();
    QWalletPtr wallet = AppModel::instance()->walletInfoPtr();
    if(txid != "" && wallet && wallet.data()->transactionHistory()){
        QTransactionPtr it = wallet.data()->transactionHistory()->getTransactionByTxid(txid);
        if(it){
            QString wallet_id = it.data()->walletId();
            if(CLIENT_INSTANCE->isNunchukLoggedIn() && CLIENT_INSTANCE->isMatrixLoggedIn() && CLIENT_INSTANCE->rooms()){
                QString room_id = CLIENT_INSTANCE->rooms()->getRoomIdByWalletId(wallet_id);
                if(room_id != ""){
                    QWarningMessage msgwarning;
                    std::vector<nunchuk::RoomTransaction> results = matrixifaces::instance()->GetPendingTransactions(room_id.toStdString(), msgwarning);
                    foreach (nunchuk::RoomTransaction tx, results) {
                        if(qUtils::strCompare(QString::fromStdString(tx.get_tx_id()), txid)){
                            it->setInitEventId(QString::fromStdString(tx.get_init_event_id()));
                        }
                    }
                }
            }
            if(wallet.data()->isAssistedWallet()){
                QJsonObject data = wallet.data()->GetServerKeyInfo(txid);
                if(!data.isEmpty()){
                    it->setServerKeyMessage(data);
                }
            }
            it->setHasMoreBtn(true);
        }
        AppModel::instance()->setTransactionInfo(it);
    }
    qApp->restoreOverrideCursor();
}

void EVT_HOME_SETTING_REQUEST_HANDLER(QVariant msg) {

}

void EVT_HOME_DISPLAY_ADDRESS_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()){
        AppModel::instance()->startDisplayAddress(AppModel::instance()->walletInfo()->walletId(),
                                                  msg.toString());
    }
}

void EVT_HOME_ADD_NEW_SIGNER_REQUEST_HANDLER(QVariant msg) {
    QEventProcessor::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_ADD_SIGNER);
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    if (type == "add-key-free") {
        AppModel::instance()->setNewWalletInfo(nullptr);
    } else if (type == "add-key-shared-group-wallet") {

    } else if (type == "request-add-a-key") {

    }
    QSignerManagement::instance()->setScreenFlow("add-a-key");
}

void EVT_HOME_IMPORT_PSBT_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    QWalletPtr wallet = AppModel::instance()->walletInfoPtr();
    if (file_path != "" && wallet){
        QString wallet_id = wallet->walletId();
        QWarningMessage msgwarning;
        QTransactionPtr trans = bridge::nunchukImportTransaction(wallet_id, file_path, msgwarning);
        if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
            if(trans){
                AppModel::instance()->setTransactionInfo(trans);
                wallet.data()->CreateAsisstedTxs(trans.data()->txid(), trans.data()->psbt(), trans.data()->memo());
                AppModel::instance()->requestSyncWalletDb(wallet_id);
                AppModel::instance()->showToast(0, STR_CPP_091, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
        else{
            AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
        }
    }
}

void EVT_HOME_EXPORT_BSMS_HANDLER(QVariant msg) {
    if(AppModel::instance()->walletInfo()) {
        QMap<QString, QVariant> maps = msg.toMap();
        QString export_type = maps["export_type"].toString();
        QString export_path = maps["export_path"].toString();
        QString file_path = qUtils::QGetFilePath(export_path);

        if("bsms" == export_type){
            if(file_path != ""){
                bool ret = bridge::nunchukExportWallet(AppModel::instance()->walletInfo()->walletId(), file_path, nunchuk::ExportFormat::BSMS);
                if(ret){
                    AppModel::instance()->walletInfo()->setNeedBackup(false);
                }
            }
        }
        else if("csv" == export_type){
            if(file_path != ""){
                bool ret = bridge::nunchukExportTransactionHistory(AppModel::instance()->walletInfo()->walletId(),
                                                                   file_path,
                                                                   nunchuk::ExportFormat::CSV);
            }
        }
        else {
            QtConcurrent::run([maps, file_path]() {
                QList<QVariant> export_data = maps["export_data"].toList();
                QList<QVariant> final_data;
                final_data.clear();
                for (const auto &item : export_data)
                {
                    QObject *obj = item.value<QObject *>();
                    if (obj)
                    {
                        QQuickItem *grabItem = qobject_cast<QQuickItem *>(obj);
                        if (grabItem)
                        {
                            QString txid = grabItem->property("txid").toString();
                            QTransactionPtr it = AppModel::instance()->walletInfo()->transactionHistory()->getTransactionByTxid(txid);
                            if(it && it.data()->status() == (int)ENUNCHUCK::TransactionStatus::CONFIRMED){
                                final_data.append(grabItem->property("invoiceContent"));
                            }
                        }
                    }
                }
                DBG_INFO << final_data.count();
                QPDFPrinter::instance()->printInvoicesToPdf(file_path, final_data);
            });
        }
    }
}

void EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST_HANDLER(QVariant msg) {
    QString masterFingerPrint = msg.toString();
    QSingleSignerPtr it = bridge::nunchukGetRemoteSigner(masterFingerPrint);
    if (it) {
        QMasterSignerPtr signer = QMasterSignerPtr(new QMasterSigner());
        signer.data()->setId(it->masterSignerId());
        signer.data()->setName(it->name());
        signer.data()->setSignerType((int)it.data()->signerType());
        signer.data()->setFingerPrint(it.data()->masterFingerPrint());
        signer.data()->setDeviceType(it.data()->devicetype());
        AppModel::instance()->setSingleSignerInfo(it);
        AppModel::instance()->setMasterSignerInfo(signer);
        AppModel::instance()->setWalletsUsingSigner(AppModel::instance()->walletList()->walletListByMasterSigner(AppModel::instance()->masterSignerInfo()->id()));
        if (auto signer = AppModel::instance()->masterSignerInfoPtr()) {
            signer->GetHistorySignerList();
            if (auto wallet = AppModel::instance()->walletInfoPtr()) {
                if (auto dashboard = wallet->dashboard()) {
                    if (auto health = dashboard->healthPtr()) {
                        health->setKeyXfp(signer->fingerPrint());
                    }
                }
            }
        }
    }
}

void EVT_ASK_HARDWARE_REQ_HANDLER(QVariant msg) {
    DBG_INFO;
    QGroupWallets::instance()->setDashboardInfo("");
}

void EVT_EXIST_HARDWARE_REQ_HANDLER(QVariant msg) {
    DBG_INFO;
}

void EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST_HANDLER(QVariant msg)
{
    DBG_INFO;
}

void EVT_KEY_HEALTH_CHECK_STATUS_REQUEST_HANDLER(QVariant msg)
{
    QGroupDashboardPtr dash = QGroupWallets::instance()->dashboardInfoPtr() ? QGroupWallets::instance()->dashboardInfoPtr() : AppModel::instance()->walletInfoPtr()->dashboard();
    if (dash) {
        dash->setFlow((int)AlertEnum::E_Alert_t::HEALTH_CHECK_STATUS);
        dash->GetWalletInfo();
        QtConcurrent::run([dash]() {
            if(dash){
                dash->GetHealthCheckInfo();
            }
        });
    }
}

void EVT_RECURRING_PAYMENTS_REQUEST_HANDLER(QVariant msg)
{
    bool callFromAlert = msg.toBool();
    if (callFromAlert) return;
    if (auto w = AppModel::instance()->walletInfoPtr()) {
        if (auto payment = w->recurringPaymentPtr()) {
            payment->clearFlow();
            payment->setPaymentCurrent(payment->DefaultPayment());
            if (payment->HasExistingPayments()) {
                payment->addFlow((int)PaymentEnum::Enum_t::RECURRING_PAYMENTS_POPULATED_SATE);
            } else {
                payment->addFlow((int)PaymentEnum::Enum_t::EMPTY_STATE);
            }
        }
    }
}

void EVT_ADD_HARDWARE_REQUEST_HANDLER(QVariant msg) {
    DBG_INFO;
}

void EVT_ADD_SOFTWARE_SIGNER_RESULT_HANDLER(QVariant msg) {

}

void EVT_REPLACE_KEYS_REQUEST_HANDLER(QVariant msg) {
    bool isFirst = msg.toMap()["isFirst"].toBool();
    if(isFirst){
        AppModel::instance()->setNewWalletInfo(AppModel::instance()->walletInfoPtr()->clone());
    }
}


