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
#include "STATE_ID_ROOT.h"
#include "Models/AppModel.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Draco.h"
#include "Chats/ClientController.h"
#include "ProfileSetting.h"
#include "ServiceSetting.h"

void ROOT_Entry(QVariant msg) {
    Q_UNUSED(msg);
}

void ROOT_Exit(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ONS_CLOSE_REQUEST_HANDLER(QVariant msg) {
    QQuickViewer::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_NONE);
    switch (msg.toInt()) {
    case E::STATE_ID_SCR_SEND:
    case E::STATE_ID_SCR_ADD_WALLET:
    case E::STATE_ID_SCR_ADD_WALLET_CONFIRMATION:
    case E::STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION:
    case E::STATE_ID_SCR_ADD_NEW_SIGNER:
    {
        AppModel::instance()->resetSignersChecked();
        break;
    }
    case E::STATE_ID_SCR_TRANSACTION_INFO :
    {
        AppModel::instance()->setTransactionInfo(NULL);
        AppModel::instance()->setUtxoInfo(QUTXOPtr(new UTXO()));
    }
        break;
    case E::STATE_ID_SCR_UTXOS :
    case E::STATE_ID_SCR_UTXO_OUTPUT :
    {
        AppModel::instance()->setUtxoInfo(QUTXOPtr(new UTXO()));
    }
        break;
    default:
        break;
    }
}

void EVT_STARTING_APPLICATION_LOCALMODE_HANDLER(QVariant msg) {
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    int isPrimaryKey = msg.toInt();
    if(!AppModel::instance()->inititalized()){
        if(isPrimaryKey > 0){
            QQuickViewer::instance()->sendEvent(E::EVT_GOTO_APP_SETTINGS_TAB, msg);
        }else{
            QQuickViewer::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
        }
        QMap<QString, QVariant> makeInstanceData;
        makeInstanceData["state_id"] = E::STATE_ID_SCR_HOME;
        AppModel::instance()->makeNunchukInstance(makeInstanceData,"");
    }
    else {
        AppModel::instance()->requestInitialData();
        if(isPrimaryKey > 0){
            QQuickViewer::instance()->sendEvent(E::EVT_GOTO_APP_SETTINGS_TAB, msg);
        }else{
            QQuickViewer::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
        }
    }
    qApp->restoreOverrideCursor();
}

void EVT_GOTO_HOME_WALLET_TAB_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ROOT_PROMT_PIN_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ROOT_PROMT_PASSPHRASE_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_STARTING_APPLICATION_ONLINEMODE_HANDLER(QVariant msg) {
    if(CLIENT_INSTANCE->isNunchukLoggedIn()){
        QQuickViewer::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
        timeoutHandler(1000, []() {
            emit CLIENT_INSTANCE->contactsChanged();
        });
    }
    else {
        if (CLIENT_INSTANCE->checkStayLoggedIn()) {
            Draco::instance()->getMe();
            if(Draco::instance()->Uid() == CLIENT_INSTANCE->getMe().email){
                bridge::nunchukSetCurrentMode(ONLINE_MODE);
                QQuickViewer::instance()->notifySendEvent(E::EVT_NUNCHUK_LOGIN_SUCCEEDED);
            }
            else{
                QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_MATRIX_REQUEST);
            }
        }
        else{
            QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_MATRIX_REQUEST);
        }
    }
    timeoutHandler(1000, []() {
        AppModel::instance()->timerFeeRatesHandle();
    });
}

void EVT_ONLINE_ONS_CLOSE_REQUEST_HANDLER(QVariant msg) {
    switch (msg.toInt()) {
    case E::STATE_ID_SCR_TRANSACTION_INFO :
    {
        AppModel::instance()->setTransactionInfo(NULL);
        AppModel::instance()->setUtxoInfo(QUTXOPtr(new UTXO()));
    }
        break;
    case E::STATE_ID_SCR_UTXOS :
    case E::STATE_ID_SCR_UTXO_OUTPUT :
    {
        AppModel::instance()->setUtxoInfo(QUTXOPtr(new UTXO()));
    }
        break;
    default:
        break;
    }
}

void EVT_STARTING_APPLICATION_ONLINE_HWL_HANDLER(QVariant msg) {
    QQuickViewer::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
}

void EVT_SHOW_TOAST_MESSAGE_HANDLER(QVariant msg) {

}

void EVT_ROOT_UPDATE_PROFILE_HANDLER(QVariant msg) {

}

void EVT_SETTING_ACCOUNT_CHANGE_PASSWORD_HANDLER(QVariant msg) {
    QString oldPasswordInput = msg.toMap().value("oldPassword").toString();
    QString newPasswordInput = msg.toMap().value("newPassword").toString();
    Draco::instance()->changePassword(oldPasswordInput, newPasswordInput);
}

void EVT_SHOW_CREATE_ACCOUNT_REQUEST_HANDLER(QVariant msg) {

}

void EVT_SETTING_ONS_CLOSE_REQUEST_HANDLER(QVariant msg) {

}

void EVT_GOTO_HOME_CHAT_TAB_HANDLER(QVariant msg) {

}

void EVT_LOGIN_DB_REQUEST_HANDLER(QVariant msg) {

}

void EVT_LOGIN_MATRIX_REQUEST_HANDLER(QVariant msg) {

}

void EVT_GOTO_APP_SETTINGS_TAB_HANDLER(QVariant msg) {

}

void EVT_SIGN_IN_REQUEST_HANDLER(QVariant msg) {

}

void EVT_ONS_CLOSE_ALL_REQUEST_HANDLER(QVariant msg) {
    QQuickViewer::instance()->closeAllPopup();
}

void EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg)
{
    QQuickViewer::instance()->sendEvent(E::EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST,true);
}

void EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg)
{
    QQuickViewer::instance()->sendEvent(E::EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST,false);
}

void EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg) {

}

void EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST_HANDLER(QVariant msg)
{

}

void EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST_HANDLER(QVariant msg) {

}

void EVT_NUNCHUK_LOGIN_SUCCEEDED_HANDLER(QVariant msg) {
    AppModel::instance()->requestClearData();
    timeoutHandler(1500, [](){
        QMap<QString, QVariant> makeInstanceData;
        makeInstanceData["state_id"] = E::STATE_ID_SCR_HOME_ONLINE;
        AppModel::instance()->makeInstanceForAccount(makeInstanceData,"");
    });
}

void EVT_GOTO_SERVICE_SETTING_TAB_HANDLER(QVariant msg)
{

}
