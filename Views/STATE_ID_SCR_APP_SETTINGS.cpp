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
#include "STATE_ID_SCR_APP_SETTINGS.h"
#include "Models/AppModel.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include <QProcess>
#include "Servers/Draco.h"
#include "ProfileSetting.h"
#include "Signers/QSignerManagement.h"

void SCR_APP_SETTING_Entry(QVariant msg) {
    AppModel::instance()->setTabIndex(-1);
    ProfileSetting::instance()->createCurrencies();
    ProfileSetting::instance()->GetMainNetServer();
}

void SCR_APP_SETTING_Exit(QVariant msg) {

}

void EVT_APP_SETTING_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST_HANDLER(QVariant msg) {
    if(msg.toBool() == false){
            Draco::instance()->deleteCurrentUser();//account normal
    }
    else{
        QMasterSignerPtr pKey = AppModel::instance()->getPrimaryKey();
        if(pKey->needPassphraseSent()){
            QMap<QString, QVariant> passPhraseData;
            passPhraseData["state_id"] = E::STATE_ID_SCR_APP_SETTINGS;
            passPhraseData["master_signer_id"] = pKey->id();
            QEventProcessor::instance()->sendEvent(E::EVT_ROOT_PROMT_PASSPHRASE, passPhraseData);
        }else{
            QEventProcessor::instance()->sendEvent(E::EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST);
        }
    }
}

void EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST_HANDLER(QVariant msg) {
    QMasterSignerPtr pKey = AppModel::instance()->getPrimaryKey();
    if(pKey){
        QString address = QString::fromStdString(pKey->originPrimaryKey().get_address());
        QString username = QString::fromStdString(pKey->originPrimaryKey().get_account());
        QString nonce = Draco::instance()->get_pkey_nonce(address,username);
        QString message = QString("%1%2").arg(username).arg(nonce);
        QString signature = bridge::SignLoginMessage(pKey->id(),message);
        if(Draco::instance()->pkey_delete_confirmation(signature)){
            bridge::nunchukDeletePrimaryKey();
            AppModel::instance()->showToast(0, STR_CPP_109, EWARNING::WarningType::SUCCESS_MSG );
        }
    }
}

void EVT_APP_SETTING_CHANGE_PASSPHRASE_HANDLER(QVariant msg) {

}

void EVT_APP_SETTING_REQUEST_RESTART_HANDLER(QVariant msg) {
    qApp->quit();
#if defined (Q_OS_WIN)
    QString command = QString("cmd /C \"%1 \"").arg(qApp->applicationFilePath());
    QProcess::startDetached(command);
#else
    QProcess::startDetached(qApp->applicationFilePath());
#endif
    DBG_INFO << "Restart new application's instance";
}

void EVT_APP_SETTING_REQUEST_RESCAN_HANDLER(QVariant msg) {
    AppModel::instance()->startRescanBlockchain(max(msg.toInt(), 0), -1);
}

void EVT_APP_SETTING_BACK_TO_ONLINE_MODE_HANDLER(QVariant msg) {

}

void EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg)
{
    QSignerManagement::instance()->requestReplacePrimaryKey();
    QEventProcessor::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY);
    QSignerManagement::instance()->setScreenFlow("add-a-key");
}

void EVT_SELECT_SERVER_REQUEST_HANDLER(QVariant msg) {

}

void EVT_CHANGE_EMAIL_REQUEST_HANDLER(QVariant msg) {

}
