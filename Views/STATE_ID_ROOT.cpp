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

void ROOT_Entry(QVariant msg) {
    Q_UNUSED(msg);
}

void ROOT_Exit(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ONS_CLOSE_REQUEST_HANDLER(QVariant msg) {
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
        QWarningMessage warningmsg;
        bridge::nunchukMakeInstance("", warningmsg);
        if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
            AppModel::instance()->requestInitialData();
            if(isPrimaryKey > 0){
                QQuickViewer::instance()->sendEvent(E::EVT_GOTO_APP_SETTINGS_TAB,msg);
            }else{
                QQuickViewer::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
            }
        }
        else if((int)EWARNING::WarningType::EXCEPTION_MSG == warningmsg.type() && nunchuk::NunchukException::INVALID_PASSPHRASE == warningmsg.code()){
            QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_DB_REQUEST, E::EVT_STARTING_APPLICATION_LOCALMODE);
        }
        else{
            AppModel::instance()->showToast(warningmsg.code(),
                                            warningmsg.what(),
                                            (EWARNING::WarningType)warningmsg.type(),
                                            "");
        }
    }
    else {
        AppModel::instance()->requestInitialData();
        if(isPrimaryKey > 0){
            QQuickViewer::instance()->sendEvent(E::EVT_GOTO_APP_SETTINGS_TAB,msg);
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
        QQuickViewer::instance()->sendEvent(E::EVT_GOTO_HOME_CHAT_TAB);
    }
    else{
        QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_MATRIX_REQUEST);
        QTimer::singleShot(100, []() {
            if(CLIENT_INSTANCE->checkStayLoggedIn()){
                QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_ONLINE_LOGIN_SUCCEED);
            }
        });
    }
    QTimer::singleShot(1000, []() {
        emit CLIENT_INSTANCE->contactsChanged();
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


