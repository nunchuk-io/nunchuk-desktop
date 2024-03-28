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
#include "STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "Servers/Draco.h"

void SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_Entry(QVariant msg){
    AppModel::instance()->setMnemonic("");
    AppModel::instance()->setSuggestMnemonics(bridge::nunchuckGetBIP39WordList());
    qUtils::SetChain((nunchuk::Chain)AppSetting::instance()->primaryServer());
}

void SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_Exit(QVariant msg){
}

void EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST_HANDLER(QVariant msg){
    QMap<QString,QVariant> maps = msg.toMap();
    QString mnemonic = maps["mnemonic"].toString();
    QString passphrase = maps["passphrase"].toString();

    QString address = qUtils::GetPrimaryKeyAddress(mnemonic,passphrase);
    Draco::instance()->getMepKey(address);
    DracoUser user = CLIENT_INSTANCE->getMe();
    QString nonce = Draco::instance()->get_pkey_nonce(address,user.username);
    QString message = QString("%1%2").arg(user.username).arg(nonce);
    QString signature = qUtils::SignLoginMessage(mnemonic,passphrase,message);
    Draco::instance()->pkey_signin(address,user.username,signature);
}

void EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST_HANDLER(QVariant msg){
    QString signername = msg.toMap().value("signername").toString();
    QString passphrase = msg.toMap().value("passphrase").toString();
    QString mnemonic = AppModel::instance()->getMnemonic();
    QMap<QString, QVariant> makeInstanceData;
    makeInstanceData["state_id"] = E::STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY;
    makeInstanceData["signername"] = signername;
    makeInstanceData["passphrase"] = passphrase;
    makeInstanceData["mnemonic"] = mnemonic;

    bool ret = AppModel::instance()->makeNunchukInstanceForAccount(makeInstanceData,"");
    if(ret){
        AppModel::instance()->startCreateSoftwareSigner(signername, mnemonic, passphrase);
    }
}

void EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST_HANDLER(QVariant msg){
    QString mnemonicinput = msg.toString();
    bool checkmnemonic = qUtils::CheckMnemonic(mnemonicinput);
    if(checkmnemonic){
        QObject *obj = QEventProcessor::instance()->getCurrentScreen();
        if(obj){
            obj->setProperty("whereIn",1);
        }
        AppModel::instance()->setMnemonic(mnemonicinput);
    }
    else{
        AppModel::instance()->setMnemonic("-101");
        AppModel::instance()->showToast(0, STR_CPP_081, EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED_HANDLER(QVariant msg){
    AppModel::instance()->makeMatrixInstanceForAccount();
    QMasterSignerPtr pKey = AppModel::instance()->getPrimaryKey();
    if(pKey){
        timeoutHandler(3000,[pKey](){
            AppModel::instance()->showToast(0, STR_CPP_108.arg(pKey->name()), EWARNING::WarningType::SUCCESS_MSG);
            QWarningMessage msg;
            bridge::nunchukClearSignerPassphrase(pKey->fingerPrint(),msg);
        });
    }
}

void EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK_HANDLER(QVariant msg){
}

