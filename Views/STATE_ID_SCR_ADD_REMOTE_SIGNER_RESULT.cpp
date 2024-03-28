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
#include "STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_REMOTE_SIGNER_RESULT_Entry(QVariant msg) {

}

void SCR_REMOTE_SIGNER_RESULT_Exit(QVariant msg) {

}

void EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK_HANDLER(QVariant msg) {
    if(AppModel::instance()->singleSignerInfo()){
        AppModel::instance()->startHealthCheckRemoteSigner(E::STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT,
                                                           AppModel::instance()->singleSignerInfo()->masterFingerPrint(),
                                                           AppModel::instance()->singleSignerInfo()->signerType(),
                                                           AppModel::instance()->singleSignerInfo()->message());
    }
}

void EVT_REMOTE_SIGNER_RESULT_EDIT_NAME_HANDLER(QVariant msg) {

}

void EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG_HANDLER(QVariant msg) {

}

void EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE_HANDLER(QVariant msg) {
    QString file_path = qUtils::QGetFilePath(msg.toString());
    if(AppModel::instance()->singleSignerInfo() && (file_path != "")){
        QString signature = bridge::nunchukImportHealthCheckSignature(file_path);
        AppModel::instance()->singleSignerInfo()->setSignature(signature);
    }
}

void EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE_HANDLER(QVariant msg) {
    QString file_txt = msg.toMap().value("file").toString();
    QString signature    = msg.toMap().value("signature").toString();
    QString file_path = qUtils::QGetFilePath(file_txt);
    QFile file(file_path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream st(&file);
        st.setCodec("UTF-8");
        st << signature << endl;
        st.flush();
        file.close();
    }
}

void EVT_REMOTE_SIGNER_RESULT_GET_XPUBS_HANDLER(QVariant msg) {

}



void EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST_HANDLER(QVariant msg) {
    QString master_fingerprint = msg.toMap().value("master_fingerprint").toString();
    QString derivation_path    = msg.toMap().value("derivation_path").toString();
    if(bridge::nunchukDeleteRemoteSigner(master_fingerprint, derivation_path)){
        QSingleSignerListModelPtr remoteSigners = bridge::nunchukGetRemoteSigners();
        if(remoteSigners){
            AppModel::instance()->setRemoteSignerList(remoteSigners);
        }
        QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST,E::STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT);
        AppModel::instance()->setSingleSignerInfo(QSingleSignerPtr(new QSingleSigner()));
    }
}


