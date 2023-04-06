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
#include "STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Draco.h"

void SCR_SOFTWARE_SIGNER_CONFIGURATION_Entry(QVariant msg) {
    AppModel::instance()->setQrExported(QStringList());
    AppModel::instance()->setAddSignerPercentage(0);
}

void SCR_SOFTWARE_SIGNER_CONFIGURATION_Exit(QVariant msg) {
    AppModel::instance()->setAddSignerPercentage(0);
}

bool replaceKey(const QString &mnemonic, const QString &passphrase){
    QMasterSignerPtr curKey = AppModel::instance()->getPrimaryKey();
    if(!curKey) return false;
    QString curAddress = QString::fromStdString(curKey->originPrimaryKey().get_address());
    QString curUsername = QString::fromStdString(curKey->originPrimaryKey().get_account());
    QString address = qUtils::GetPrimaryKeyAddress(mnemonic,passphrase);
    QString nonce = Draco::instance()->pkey_manual_nonce(curAddress,curUsername,address,"change_pkey");
    QString curSignature = bridge::SignLoginMessage(curKey->id(),nonce);
    QString new_signature = qUtils::SignLoginMessage(mnemonic,passphrase,nonce);
    return Draco::instance()->pkey_change_pkey(address,curSignature,new_signature);
}

void EVT_SOFTWARE_SIGNER_REQUEST_CREATE_HANDLER(QVariant msg) {
    QMap<QString,QVariant> dataMap = msg.toMap();
    QString signername = dataMap.value("signername").toString();
    QString passphrase = dataMap.value("passphrase").toString();
    QString mnemonic = AppModel::instance()->getMnemonic();
    if(QQuickViewer::instance()->currentFlow() != (int)ENUNCHUCK::IN_FLOW::FLOW_PRIMARY_KEY &&
       QQuickViewer::instance()->currentFlow() != (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY){
        AppModel::instance()->startCreateSoftwareSigner(signername, mnemonic, passphrase);
    }
    else{
        DBG_INFO << QQuickViewer::instance()->currentFlow();
        if(QQuickViewer::instance()->currentFlow() == (int)ENUNCHUCK::IN_FLOW::FLOW_REPLACE_PRIMARY_KEY){
            if(replaceKey(mnemonic,passphrase)){
                DBG_INFO << "startCreateSoftwareSigner";
                AppModel::instance()->startCreateSoftwareSigner(signername, mnemonic, passphrase);
            }
        }
        else{
            DBG_INFO << dataMap;
            QQuickViewer::instance()->sendEvent(E::EVT_PRIMARY_KEY_CONFIGURATION_REQUEST,msg);
        }
    }
}

void EVT_ADD_SOFTWARE_SIGNER_RESULT_HANDLER(QVariant msg) {

}

void EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_HANDLER(QVariant msg) {

}

void EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION_HANDLER(QVariant msg) {

}

void EVT_PRIMARY_KEY_CONFIGURATION_REQUEST_HANDLER(QVariant msg)
{

}
