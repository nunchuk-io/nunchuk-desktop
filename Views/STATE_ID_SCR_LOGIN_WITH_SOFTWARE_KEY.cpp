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
#include "STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY.h"
#include "Models/AppModel.h"
#include "QQuickViewer.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Draco.h"
#include "Chats/ClientController.h"
#include "Chats/matrixbrigde.h"
#include "STATE_ID_SCR_LOGIN_ONLINE.h"
#include "localization/STR_CPP.h"

void SCR_LOGIN_WITH_SOFTWARE_KEY_Entry(QVariant msg) {
    QObject *obj = QQuickViewer::instance()->getQmlObj().first();
    if(obj){
        obj->setProperty("primary_key",msg);
    }
}

void SCR_LOGIN_WITH_SOFTWARE_KEY_Exit(QVariant msg) {

}

void EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST_HANDLER(QVariant msg) {
    QTimer::singleShot(0,[msg](){
        QVariantMap maps = msg.toMap();
        QString passphrase = maps["passphrase"].toString();
        QVariantMap primary_key = maps["primary_key"].toMap();
        QString account = primary_key["account"].toString();
        QString address = primary_key["address"].toString();
        bool isAvailable = Draco::instance()->pkey_username_availability(account);
        DBG_INFO << isAvailable;
        if(isAvailable){
            Draco::instance()->setUid(account);
            AppModel::instance()->nunchukLogin();
            AppModel::instance()->setPrimaryKey(account);
            QMasterSignerPtr pKey = AppModel::instance()->getPrimaryKey();
            QString masterId = pKey->id();
            QString nonce = Draco::instance()->get_pkey_nonce(address,account);
            QString message = QString("%1%2").arg(account).arg(nonce);
            QWarningMessage warnMsg;
            bridge::nunchukSendSignerPassphrase( masterId, passphrase, warnMsg);
            if(warnMsg.type() == (int)EWARNING::WarningType::NONE_MSG){
                QString signature = bridge::SignLoginMessage(masterId,message);
                Draco::instance()->pkey_signin(address,account,signature);
            }
            else{
                Draco::instance()->pkey_signin(address,account,"");
            }
        }else{
            AppModel::instance()->setToast(-1,
                                            STR_CPP_104,
                                            EWARNING::WarningType::ERROR_MSG,
                                            "");
        }
    });
}

void EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED_HANDLER(QVariant msg) {
    AppModel::instance()->matrixLogin();
    QMasterSignerPtr pKey = AppModel::instance()->getPrimaryKey();
    if(pKey){
        QTimer::singleShot(3000,[pKey](){
            AppModel::instance()->showToast(0,
                                           STR_CPP_108.arg(pKey->name()),
                                           EWARNING::WarningType::SUCCESS_MSG,
                                           STR_CPP_108.arg(pKey->name()));
        });
    }
}

void EVT_LOGIN_WITH_SOFTWARE_KEY_BACK_HANDLER(QVariant msg) {

}


