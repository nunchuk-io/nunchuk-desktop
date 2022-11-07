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
#include "STATE_ID_SCR_LOGIN.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Draco.h"
#include "localization/STR_CPP.h"
#include "Chats/ClientController.h"
#include "Chats/matrixbrigde.h"

static int loginDBEvent = -1;

void SCR_LOGIN_Entry(QVariant msg) {
    loginDBEvent = msg.toInt();
}

void SCR_LOGIN_Exit(QVariant msg) {
    loginDBEvent = -1;
}

void EVT_LOGIN_PERFORM_PASSWORD_REQUEST_HANDLER(QVariant msg) {
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    QString dbPassphrase = msg.toString();
    if(loginDBEvent == (int)E::EVT_STARTING_APPLICATION_LOCALMODE){
        bridge::nunchukSetCurrentMode(LOCAL_MODE);
        QWarningMessage warningmsg;
        bridge::nunchukMakeInstance(dbPassphrase, warningmsg);
        if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
            QQuickViewer::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
            AppModel::instance()->requestInitialData();
        }
        else{
            AppModel::instance()->showToast(warningmsg.code(),
                                            warningmsg.what(),
                                            (EWARNING::WarningType)warningmsg.type(),
                                            "");
        }
    }
    else if(loginDBEvent == (int)E::STATE_ID_SCR_LOGIN_ONLINE){
        bridge::nunchukSetCurrentMode(ONLINE_MODE);
        QWarningMessage nunchukMsg;
        QString account = Draco::instance()->Uid();
        bridge::nunchukMakeInstanceForAccount(account,
                                              dbPassphrase,
                                              nunchukMsg);
        if((int)EWARNING::WarningType::NONE_MSG == nunchukMsg.type()){
            AppModel::instance()->requestGetWallets();
            AppModel::instance()->requestGetSigners();
            QString device_id = QString("%1%2").arg(Draco::instance()->deviceId()).arg(Draco::instance()->chatId());
            CLIENT_INSTANCE->requestLogin();
            CLIENT_INSTANCE->saveStayLoggedInData();
            CLIENT_INSTANCE->setIsNunchukLoggedIn(true);
            QQuickViewer::instance()->sendEvent(E::EVT_GOTO_HOME_CHAT_TAB);

            QWarningMessage matrixMsg;
            matrixbrigde::makeMatrixInstance(account,
                                             device_id,
                                             matrixMsg);
            if((int)EWARNING::WarningType::NONE_MSG != matrixMsg.type()){
                AppModel::instance()->showToast(matrixMsg.code(),
                                                matrixMsg.what(),
                                                (EWARNING::WarningType)matrixMsg.type(),
                                                "");
            }
        }
        else{
            DBG_INFO << "COULD NOT MAKE NUNCHUCK INSTANCE" << nunchukMsg.code();
            AppModel::instance()->showToast(nunchukMsg.code(),
                                            nunchukMsg.what(),
                                            (EWARNING::WarningType)nunchukMsg.type(),
                                            "");
        }
    }
    else{ /*TBD*/ }
    qApp->restoreOverrideCursor();
}



