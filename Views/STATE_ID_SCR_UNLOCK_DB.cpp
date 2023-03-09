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
#include "STATE_ID_SCR_UNLOCK_DB.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Draco.h"
#include "localization/STR_CPP.h"
#include "Chats/ClientController.h"
#include "Chats/matrixbrigde.h"

static QVariant makeInstanceObject = QVariant();
void SCR_LOGIN_Entry(QVariant msg) {
    makeInstanceObject = msg;
}

void SCR_LOGIN_Exit(QVariant msg) {
    makeInstanceObject = QVariant();
}

void EVT_LOGIN_PERFORM_PASSWORD_REQUEST_HANDLER(QVariant msg) {
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    QString dbPassphrase = msg.toString();
    QMap<QString, QVariant> dataMap = makeInstanceObject.toMap();
    int state_id = dataMap.value("state_id").toInt();
    DBG_INFO << state_id;
    switch (state_id) {
    case (int)E::STATE_ID_SCR_HOME:
    {
        bool ret = AppModel::instance()->makeNunchukInstance(makeInstanceObject,dbPassphrase);
        if(ret){
            QQuickViewer::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
        }
        break;
    }
    case (int)E::STATE_ID_SCR_HOME_ONLINE:
    {
        bool ret = AppModel::instance()->makeInstanceForAccount(makeInstanceObject,dbPassphrase);
        if(ret){
            QQuickViewer::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
        }
        break;
    }
    case (int)E::STATE_ID_SCR_SIGN_IN_MANUALLY:
    {
        bool ret = AppModel::instance()->makeInstanceForAccount(makeInstanceObject, dbPassphrase);
        if(ret){
            QQuickViewer::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
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
        break;
    }
    case (int)E::STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION:
    {
        bool ret = AppModel::instance()->makeInstanceForAccount(makeInstanceObject, dbPassphrase);
        if(ret){
            QString signername = dataMap.value("signername").toString();
            QString passphrase = dataMap.value("passphrase").toString();
            QString mnemonic   = dataMap.value("mnemonic").toString();
            AppModel::instance()->startCreateSoftwareSigner(signername, mnemonic, passphrase);
        }
        break;
    }
    case (int)E::STATE_ID_SCR_PRIMARY_KEY_ACCOUNT:
    {
        bool ret = AppModel::instance()->makeNunchukInstanceForAccount(makeInstanceObject, dbPassphrase);
        if(ret){
            QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST,dataMap);
        }
        break;
    }
    case (int)E::STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY:
    {
        bool ret = AppModel::instance()->makeNunchukInstanceForAccount(makeInstanceObject, dbPassphrase);
        if(ret){
            QString signername = dataMap.value("signername").toString();
            QString passphrase = dataMap.value("passphrase").toString();
            QString mnemonic   = dataMap.value("mnemonic").toString();
            QTimer::singleShot(3000,[signername,passphrase,mnemonic](){
                AppModel::instance()->startCreateSoftwareSigner(signername, mnemonic, passphrase);
            });
        }
        break;
    }
    default: {
        DBG_INFO << state_id;
        /*TBD*/
        break;
    }
    }
    qApp->restoreOverrideCursor();
}



