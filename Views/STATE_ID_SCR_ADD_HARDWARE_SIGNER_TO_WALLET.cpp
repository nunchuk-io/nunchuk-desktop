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
#include "STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Models/Chats/ClientController.h"

void SCR_ADD_HARDWARE_SIGNER_TO_WALLET_Entry(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->setSingleSignerInfo(QSingleSignerPtr(new QSingleSigner()));
    AppModel::instance()->setAddSignerStep(-1);
    AppModel::instance()->setAddSignerPercentage(0);
    AppModel::instance()->setNewKeySignMessage("");
}

void SCR_ADD_HARDWARE_SIGNER_TO_WALLET_Exit(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->setNewKeySignMessage("");
    AppModel::instance()->setAddSignerStep(-1);
    AppModel::instance()->setAddSignerPercentage(0);
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_HANDLER(QVariant msg) {
    QString key_name = msg.toMap().value("key_name").toString();
    int deviceIndexSelected    = msg.toMap().value("deviceIndexSelected").toInt();
    bool yesBtn      = msg.toMap().value("key_yes_accept").toBool();
    if(AppModel::instance()->deviceList()){
        QDevicePtr selectedDv = AppModel::instance()->deviceList()->getDeviceByIndex(deviceIndexSelected);
        if (selectedDv) {
            bool isSubscribed = ClientController::instance()->isSubscribed();
            auto request = [&]() {
                if(selectedDv){
                    if(selectedDv.data()->needsPinSent() || selectedDv.data()->needsPassPhraseSent()){
                        AppModel::instance()->showToast(0, STR_CPP_095, EWARNING::WarningType::WARNING_MSG);
                    }
                    else {
                        AppModel::instance()->startCreateMasterSigner(key_name, selectedDv->masterFingerPrint());
                    }
                }
            };
            if (isSubscribed) {
                if (AppModel::instance()->masterSignerListPtr()->containsFingerPrint(selectedDv->masterFingerPrint())) {
                    if (yesBtn) {
                        request();
                    } else {
                        auto oldKey = AppModel::instance()->masterSignerListPtr()->getMasterSignerByXfp(selectedDv->masterFingerPrint());
                        if (oldKey->originMasterSigner().get_type() == nunchuk::SignerType::SOFTWARE) {
                            emit AppModel::instance()->notifySignerExist(true, selectedDv->masterFingerPrint());
                        } else {
                            emit AppModel::instance()->notifySignerExist(false, selectedDv->masterFingerPrint());
                        }
                    }
                } else {
                    request();
                }
            } else {
                request();
            }
        }
    }
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_HANDLER(QVariant msg) {
    // Trimmed input
    QString key_name    = msg.toMap().value("key_name").toString().simplified();
    QString key_tag     = msg.toMap().value("key_tag").toString();
    QString key_spec    = msg.toMap().value("key_spec").toString();
    bool key_yes_accept = msg.toMap().value("key_yes_accept").toBool();
    DBG_INFO << msg.toMap();

    QWarningMessage warningmsg;
    nunchuk::SingleSigner singleSigner = qUtils::ParseSignerString(key_spec, warningmsg);
    if((int)EWARNING::WarningType::EXCEPTION_MSG == warningmsg.type()) {
        AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
    }
    else{
        singleSigner.set_name(key_name.toStdString());
        if (ClientController::instance()->isSubscribed()) {
            if(key_yes_accept){
                nunchuk::SignerTag tag;
                try {
                    tag = SignerTagFromStr(key_tag.toStdString());
                    singleSigner.set_tags({tag});
                }
                catch (const nunchuk::BaseException &ex) {
                    DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
                }
                catch (std::exception &e) {
                    DBG_INFO << "THROW EXCEPTION " << e.what();
                }
                AppModel::instance()->startCreateRemoteSigner(QString::fromStdString(singleSigner.get_name()),
                                                              QString::fromStdString(singleSigner.get_xpub()),
                                                              QString::fromStdString(singleSigner.get_public_key()),
                                                              QString::fromStdString(singleSigner.get_derivation_path()),
                                                              QString::fromStdString(singleSigner.get_master_fingerprint()),
                                                              nunchuk::SignerType::AIRGAP,
                                                              singleSigner.get_tags(),
                                                              true, // Replace existing signer
                                                              E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER);
            }
            else{
                bool isExisted = false;
                isExisted = AppModel::instance()->masterSignerList() ? AppModel::instance()->masterSignerList()->containsFingerPrint(QString::fromStdString(singleSigner.get_master_fingerprint())) : false;;
                if(isExisted){
                    DBG_INFO << isExisted;
                    QMasterSignerPtr old_key = AppModel::instance()->masterSignerList()->getMasterSignerByXfp(QString::fromStdString(singleSigner.get_master_fingerprint()));
                    emit AppModel::instance()->notifySignerExist(old_key ? (int)ENUNCHUCK::SignerType::SOFTWARE == old_key.data()->signerType() : false,
                                                                 QString::fromStdString(singleSigner.get_master_fingerprint()));
                }
                else{
                    isExisted = AppModel::instance()->remoteSignerList() ? AppModel::instance()->remoteSignerList()->containsSigner(QString::fromStdString(singleSigner.get_master_fingerprint()), QString::fromStdString(singleSigner.get_derivation_path())) : false;
                    DBG_INFO << isExisted;
                    if(isExisted){
                        QSingleSignerPtr old_key = AppModel::instance()->remoteSignerList()->getSingleSignerByFingerPrint(QString::fromStdString(singleSigner.get_master_fingerprint()));
                        emit AppModel::instance()->notifySignerExist(old_key ? (int)ENUNCHUCK::SignerType::SOFTWARE == old_key.data()->signerType() : false,
                                                                     QString::fromStdString(singleSigner.get_master_fingerprint()));
                    }
                    else{
                        AppModel::instance()->startCreateRemoteSigner(QString::fromStdString(singleSigner.get_name()),
                                                                      QString::fromStdString(singleSigner.get_xpub()),
                                                                      QString::fromStdString(singleSigner.get_public_key()),
                                                                      QString::fromStdString(singleSigner.get_derivation_path()),
                                                                      QString::fromStdString(singleSigner.get_master_fingerprint()),
                                                                      nunchuk::SignerType::AIRGAP,
                                                                      singleSigner.get_tags(),
                                                                      false, // Key doesn't existing
                                                                      E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER);
                    }
                }
            }
        }
        else{
            AppModel::instance()->startCreateRemoteSigner(QString::fromStdString(singleSigner.get_name()),
                                                          QString::fromStdString(singleSigner.get_xpub()),
                                                          QString::fromStdString(singleSigner.get_public_key()),
                                                          QString::fromStdString(singleSigner.get_derivation_path()),
                                                          QString::fromStdString(singleSigner.get_master_fingerprint()),
                                                          nunchuk::SignerType::AIRGAP,
                                                          singleSigner.get_tags(),
                                                          true, // Replace existing signer
                                                          E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER);
        }
    }
    emit AppModel::instance()->finishedCreateRemoteSigner();
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
    QMap<QString,QVariant> data;
    data["state_id"] = E::STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET;
    AppModel::instance()->startScanDevices(QVariant::fromValue(data));
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN_HANDLER(QVariant msg) {

}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN_HANDLER(QVariant msg) {

}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}


void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

