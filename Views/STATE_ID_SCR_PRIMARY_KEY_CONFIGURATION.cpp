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
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Servers/Draco.h"
#include "STATE_ID_SCR_LOGIN_ONLINE.h"

void SCR_PRIMARY_KEY_CONFIGURATION_Entry(QVariant msg) {
    QMap<QString,QVariant> dataMap = msg.toMap();
    QString signername = dataMap.value("signername").toString();
    QString passphrase = dataMap.value("passphrase").toString();
    QString mnemonic = AppModel::instance()->getMnemonic();
    qUtils::SetChain((nunchuk::Chain)AppSetting::instance()->primaryServer());
    QString fingerprint = qUtils::GetMasterFingerprint(mnemonic,passphrase);
    QObject *obj = QEventProcessor::instance()->getCurrentScreen();
    DBG_INFO << fingerprint << dataMap;
    if(obj){
        obj->setProperty("primaryKeyUsername",fingerprint);
        obj->setProperty("primaryKeyPassphrase",passphrase);
        obj->setProperty("primaryKeySignername",signername);
    }
}

void SCR_PRIMARY_KEY_CONFIGURATION_Exit(QVariant msg) {

}

void EVT_PRIMARY_KEY_SIGN_IN_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setMasterSignerInfo( QMasterSignerPtr(new QMasterSigner()));
    QMap<QString,QVariant> maps = msg.toMap();
    QString mnemonic = maps["mnemonic"].toString();
    QString username = maps["username"].toString();
    QString passphrase = maps["passphrase"].toString();
    DBG_INFO << username;
    bool isAvail = Draco::instance()->pkey_username_availability(username);
    if(!isAvail){
        QString address = qUtils::GetPrimaryKeyAddress(mnemonic,passphrase);
        QString nonce = Draco::instance()->get_pkey_nonce(address,username);
        QString message = QString("%1%2").arg(username).arg(nonce);
        QString signature = qUtils::SignLoginMessage(mnemonic,passphrase,message);
        if(Draco::instance()->pkey_signup(address,username,signature)){
            QEventProcessor::instance()->sendEvent(E::EVT_PRIMARY_KEY_CONFIGURATION_FINISHED);
        }
    }
}

void EVT_PRIMARY_KEY_CONFIGURATION_BACK_HANDLER(QVariant msg) {

}

void EVT_PRIMARY_KEY_CONFIGURATION_FINISHED_HANDLER(QVariant msg) {
    DBG_INFO;
    timeoutHandler(200,[](){
        AppModel::instance()->showToast(0, STR_CPP_106, EWARNING::WarningType::SUCCESS_MSG);
        AppModel::instance()->setPrimaryKey(Draco::instance()->Uid());
    });
}

void EVT_PRIMARY_KEY_SIGN_IN_SUCCEED_HANDLER(QVariant msg)
{
    QString signername = msg.toMap().value("signername").toString();
    QString passphrase = msg.toMap().value("passphrase").toString();
    QString mnemonic = AppModel::instance()->getMnemonic();

    QMap<QString, QVariant> makeInstanceData;
    makeInstanceData["state_id"] = E::STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION;
    makeInstanceData["signername"] = signername;
    makeInstanceData["passphrase"] = passphrase;
    makeInstanceData["mnemonic"] = mnemonic;

    bool ret = AppModel::instance()->makeInstanceForAccount(makeInstanceData,"");
    if(ret){
        AppModel::instance()->startCreateSoftwareSigner(signername, mnemonic, passphrase);
    }
}
