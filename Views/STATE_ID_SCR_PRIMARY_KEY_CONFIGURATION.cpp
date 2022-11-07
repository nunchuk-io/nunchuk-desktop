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
#include "STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Draco.h"
#include "STATE_ID_SCR_LOGIN_ONLINE.h"

void SCR_PRIMARY_KEY_CONFIGURATION_Entry(QVariant msg) {
    QString signername = msg.toMap().value("signername").toString();
    QString passphrase = msg.toMap().value("passphrase").toString();
    QString mnemonic = AppModel::instance()->getMnemonic();
    QString fingerprint = qUtils::GetMasterFingerprint(mnemonic,passphrase);
    DBG_INFO << fingerprint;
    QObject *obj = QQuickViewer::instance()->getQmlObj().first();
    if(obj){
        obj->setProperty("primaryKeyUsername",fingerprint);
        obj->setProperty("primaryKeyPassphrase",passphrase);
        obj->setProperty("primaryKeySignername",signername);
    }
}

void SCR_PRIMARY_KEY_CONFIGURATION_Exit(QVariant msg) {

}

void EVT_PRIMARY_KEY_SIGN_IN_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setMasterSignerInfo( QMasterSignerPtr(new MasterSigner()));
    QMap<QString,QVariant> maps = msg.toMap();
    QString mnemonic = maps["mnemonic"].toString();
    QString username = maps["username"].toString();
    QString passphrase = maps["passphrase"].toString();
    bool isAvail = Draco::instance()->pkey_username_availability(username);
    if(!isAvail){
        QString address = qUtils::GetPrimaryKeyAddress(mnemonic,passphrase);
        QString nonce = Draco::instance()->get_pkey_nonce(address,username);
        QString message = QString("%1%2").arg(username).arg(nonce);
        QString signature = qUtils::SignLoginMessage(mnemonic,passphrase,message);
        Draco::instance()->pkey_signup(address,username,signature);
    }else{
        AppModel::instance()->setToast(-1,
                                        STR_CPP_105,
                                        EWARNING::WarningType::ERROR_MSG,
                                        "");
    }
}

void EVT_PRIMARY_KEY_CONFIGURATION_BACK_HANDLER(QVariant msg) {

}

void EVT_PRIMARY_KEY_CONFIGURATION_FINISHED_HANDLER(QVariant msg) {
    QTimer::singleShot(200,[](){
        AppModel::instance()->showToast(0,
                                        STR_CPP_106,
                                        EWARNING::WarningType::SUCCESS_MSG,
                                        STR_CPP_106);
        AppModel::instance()->setPrimaryKey(Draco::instance()->Uid());
    });
}

void EVT_PRIMARY_KEY_SIGN_IN_SUCCEED_HANDLER(QVariant msg)
{
    AppModel::instance()->loginSucceed();
    QString signername = msg.toMap().value("signername").toString();
    QString passphrase = msg.toMap().value("passphrase").toString();
    QString mnemonic = AppModel::instance()->getMnemonic();
    AppModel::instance()->startCreateSoftwareSigner(signername, mnemonic, passphrase);
}
