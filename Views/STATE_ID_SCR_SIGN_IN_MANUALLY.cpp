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
#include "STATE_ID_SCR_SIGN_IN_MANUALLY.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Draco.h"
#include "STATE_ID_SCR_LOGIN_ONLINE.h"

void SCR_SIGN_IN_MANUALLY_Entry(QVariant msg) {
}

void SCR_SIGN_IN_MANUALLY_Exit(QVariant msg) {

}

void EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED_HANDLER(QVariant msg) {
    QTimer::singleShot(1000,[](){
        QMap<QString, QVariant> makeInstanceData;
        makeInstanceData["state_id"] = E::STATE_ID_SCR_SIGN_IN_MANUALLY;
        bool ret = AppModel::instance()->makeInstanceForAccount(makeInstanceData,"");
        if(ret){
            QString username = ClientController::instance()->getMe().username;
            AppModel::instance()->setPrimaryKey(username);
            QMasterSignerPtr pKey = AppModel::instance()->getPrimaryKey();
            if(pKey){
                QTimer::singleShot(3000,[pKey](){
                    AppModel::instance()->showToast(0,
                                                    STR_CPP_108.arg(pKey->name()),
                                                    EWARNING::WarningType::SUCCESS_MSG,
                                                    STR_CPP_108.arg(pKey->name()));
                    QWarningMessage msg;
                    bridge::nunchukClearSignerPassphrase(pKey->fingerPrint(),msg);
                });
            }
        }
    });
}

void EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST_HANDLER(QVariant msg) {
    QMap<QString,QVariant> maps = msg.toMap();
    QString username = maps["username"].toString();
    QString signature = maps["signature"].toString();
    std::vector<nunchuk::PrimaryKey> primaryKeys = qUtils::GetPrimaryKeys(AppSetting::instance()->storagePath(),
                                                                          (nunchuk::Chain)AppSetting::instance()->primaryServer());
    nunchuk::PrimaryKey pkey;
    for(nunchuk::PrimaryKey key: primaryKeys){
        if(key.get_account() == username.toStdString()){
            pkey = key;
            break;
        }
    }
    Draco::instance()->pkey_signin(QString::fromStdString(pkey.get_address()),username,signature);
}

void EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST_HANDLER(QVariant msg) {
    QString username = msg.toString();
    bool isAvailable = Draco::instance()->pkey_username_availability(username);
    DBG_INFO << isAvailable;
    if(isAvailable){
        QObject *obj = QQuickViewer::instance()->getQmlObj().first();
        if(obj){
            obj->setProperty("whereIn",1);
            QString nonce = Draco::instance()->pkey_manual_nonce("",username,"");
            if(nonce.isEmpty()){
                AppModel::instance()->setToast(-1,
                                                STR_CPP_107,
                                                EWARNING::WarningType::ERROR_MSG,
                                                "");
            }else{
                obj->setProperty("challengemessage",nonce);
            }
        }
    }else{
        AppModel::instance()->setToast(-1,
                                        STR_CPP_104,
                                        EWARNING::WarningType::ERROR_MSG,
                                        "");
    }
}

void EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST_HANDLER(QVariant msg) {
    QString username = msg.toString();
    QObject *obj = QQuickViewer::instance()->getQmlObj().first();
    if(obj){
        QString nonce = Draco::instance()->pkey_manual_nonce("",username,"");
        if(nonce.isEmpty()){
            AppModel::instance()->setToast(-1,
                                            STR_CPP_107,
                                            EWARNING::WarningType::ERROR_MSG,
                                            "");
        }else{
            obj->setProperty("challengemessage",nonce);
        }
    }
}

