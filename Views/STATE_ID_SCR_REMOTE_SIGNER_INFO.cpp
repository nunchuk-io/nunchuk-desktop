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
#include "STATE_ID_SCR_REMOTE_SIGNER_INFO.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_REMOTE_SIGNER_INFO_Entry(QVariant msg) {

}

void SCR_REMOTE_SIGNER_INFO_Exit(QVariant msg) {
    AppModel::instance()->startReloadRemoteSigners();
}

void EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK_HANDLER(QVariant msg) {
    if(AppModel::instance()->singleSignerInfoPtr()){
        AppModel::instance()->startHealthCheckRemoteSigner();
    }
}

void EVT_REMOTE_SIGNER_INFO_EDIT_NAME_HANDLER(QVariant msg) {
    if(AppModel::instance()->singleSignerInfo()){
        if(msg.toString() != AppModel::instance()->singleSignerInfo()->name()){
            bridge::nunchukUpdateRemoteSigner(msg.toString());
            AppModel::instance()->updateSingleSignerInfoName(msg.toString());
            if(AppModel::instance()->remoteSignerList()){
                AppModel::instance()->remoteSignerList()->requestSort(SingleSignerListModel::SingleSignerRoles::single_signer_name_Role, Qt::AscendingOrder);
            }
        }
    }
}

void EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(AppModel::instance()->singleSignerInfo() && (file_path != "")){
        QString signature = bridge::nunchukImportHealthCheckSignature(file_path);
        AppModel::instance()->singleSignerInfo()->setSignature(signature);
    }
}

void EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(AppModel::instance()->singleSignerInfo() && (file_path != "")){
        bridge::nunchukExportHealthCheckMessage(AppModel::instance()->singleSignerInfo()->message(), file_path);
    }
}

void EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST_HANDLER(QVariant msg) {
    QString master_fingerprint = msg.toMap().value("master_fingerprint").toString();
    QString derivation_path    = msg.toMap().value("derivation_path").toString();
    if(bridge::nunchukDeleteRemoteSigner(master_fingerprint, derivation_path)){
        QSingleSignerListModelPtr remoteSigners = bridge::nunchukGetRemoteSigners();
        if(remoteSigners){
            AppModel::instance()->setRemoteSignerList(remoteSigners);
        }
        QQuickViewer::instance()->sendEvent(E::EVT_REMOTE_SIGNER_INFO_BACK_HOME);
        AppModel::instance()->setSingleSignerInfo(QSingleSignerPtr(new SingleSigner()));
    }
}

void EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO_HANDLER(QVariant msg) {

}

void EVT_REMOTE_SIGNER_INFO_BACK_HOME_HANDLER(QVariant msg) {

}

void EVT_REMOTE_SIGNER_INFO_GET_XPUBS_HANDLER(QVariant msg) {

}
