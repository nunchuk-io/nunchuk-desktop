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
#include "STATE_ID_SCR_LOGIN_ONLINE.h"
#include "Models/AppModel.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Draco.h"
#include "Chats/ClientController.h"
#include "Chats/matrixbrigde.h"

void SCR_LOGIN_ONLINE_Entry(QVariant msg) {
    AppModel::instance()->timerFeeRatesHandle();
    bridge::nunchukSetCurrentMode(LOCAL_MODE);
}

void SCR_LOGIN_ONLINE_Exit(QVariant msg) {

}

void EVT_LOGIN_ONLINE_CREATE_ACCOUNT_HANDLER(QVariant msg) {
    AppModel::instance()->create_account(msg);
}

void EVT_LOGIN_ONLINE_SIGN_IN_HANDLER(QVariant msg) {
    AppModel::instance()->loginNunchuk(msg);
}

void EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE_HANDLER(QVariant msg) {
    DBG_INFO;
}

void EVT_LOGIN_ONLINE_CHANGE_PASSWORD_HANDLER(QVariant msg) {
    QString oldPasswordInput = msg.toMap().value("oldPassword").toString();
    QString newPasswordInput = msg.toMap().value("newPassword").toString();
    Draco::instance()->changePassword(oldPasswordInput, newPasswordInput);
}

void EVT_LOGIN_ONLINE_FORGOT_PASSWORD_HANDLER(QVariant msg) {
    QString emailInput = msg.toString();
    Draco::instance()->forgotPassword(emailInput);
}

void EVT_LOGIN_ONLINE_STAY_SIGNED_IN_HANDLER(QVariant msg) {

}

void EVT_LOGIN_ONLINE_LOGIN_SUCCEED_HANDLER(QVariant msg) {
    QMap<QString, QVariant> makeInstanceData;
    makeInstanceData["state_id"] = E::STATE_ID_SCR_HOME_ONLINE;
    AppModel::instance()->makeInstanceForAccount(makeInstanceData,"");
}

void EVT_LOGIN_ONLINE_RECOVER_PASSWORD_HANDLER(QVariant msg) {
    QString email = Draco::instance()->emailRequested();
    QString token = msg.toMap().value("token").toString();
    QString passw = msg.toMap().value("passw").toString();
    Draco::instance()->recoverPassword(email, token, passw);
}

void EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE_HANDLER(QVariant msg) {
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    QString pin = msg.toString();
    if(pin != ""){
        Draco::instance()->verifyNewDevice(pin);
    }
    qApp->restoreOverrideCursor();
}

void EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE_HANDLER(QVariant msg) {
    Draco::instance()->resendVerifyNewDeviceCode();
}
