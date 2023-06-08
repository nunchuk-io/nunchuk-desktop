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
#include "STATE_ID_SCR_ADD_HARDWARE_SIGNER.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"

void SCR_ADD_HARDWARE_SIGNER_Entry(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->setNewKeySignMessage("");
    AppModel::instance()->setAddSignerStep(-1);
    AppModel::instance()->setAddSignerPercentage(0);
}

void SCR_ADD_HARDWARE_SIGNER_Exit(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->setNewKeySignMessage("");
    AppModel::instance()->setAddSignerStep(-1);
    AppModel::instance()->setAddSignerPercentage(0);
}

void EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST_HANDLER(QVariant msg) {
    QString signerNameInputted = msg.toMap().value("signerNameInputted").toString();
    int deviceIndexSelected    = msg.toMap().value("deviceIndexSelected").toInt();
    if(AppModel::instance()->deviceList()){
        QDevicePtr selectedDv = AppModel::instance()->deviceList()->getDeviceByIndex(deviceIndexSelected) ;
        if(selectedDv){
            if(selectedDv.data()->needsPinSent() || selectedDv.data()->needsPassPhraseSent()){
                AppModel::instance()->showToast(0,
                                                0,
                                                EWARNING::WarningType::WARNING_MSG,
                                                STR_CPP_095);
            }
            else{
                AppModel::instance()->startCreateMasterSigner(signerNameInputted, deviceIndexSelected);
            }
        }
    }
}

void EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setSingleSignerInfo(QSingleSignerPtr(new QSingleSigner()));
    // Trimmed input
    QString signerNameInputted = msg.toMap().value("signerNameInputted").toString().simplified();
    QString xpubInputted = msg.toMap().value("xpubInputted").toString().simplified();
    QString bip32Inputted = msg.toMap().value("bip32Inputted").toString().simplified();
    QString masterFingerPrintInputted = msg.toMap().value("masterFingerPrintInputted").toString().simplified();
    bool isValidFingerPrint = false;

    // Adding a Remote Signer whose Master Signer already exists: disallowed
    if(AppModel::instance()->masterSignerList() && AppModel::instance()->masterSignerList()->containsFingerPrint(masterFingerPrintInputted)){
        isValidFingerPrint = false;
    }
    else{
        isValidFingerPrint = qUtils::QIsValidFingerPrint(masterFingerPrintInputted);
    }
    bool isValidDerivationPath = qUtils::QIsValidDerivationPath(bip32Inputted);
    QString xpubOutput = "";
    bool isValidXpub = qUtils::QIsValidXPub(xpubInputted, xpubOutput);
    bool inputValid = isValidDerivationPath && isValidFingerPrint && isValidXpub;
    if(inputValid){
        AppModel::instance()->startCreateRemoteSigner(signerNameInputted,
                                                      xpubOutput,
                                                      "",
                                                      bip32Inputted,
                                                      masterFingerPrintInputted,
                                                      E::EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST);
    }
    else{
        AppModel::instance()->singleSignerInfo()->setDerivationPath(!isValidDerivationPath ? "false" : "");
        AppModel::instance()->singleSignerInfo()->setMasterFingerPrint(!isValidFingerPrint ? "false" : "");
        AppModel::instance()->singleSignerInfo()->setXpub(!isValidXpub ? "false" : "");
        AppModel::instance()->singleSignerInfo()->setPublickey("");
        emit AppModel::instance()->finishedCreateRemoteSigner();
    }
}

void EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ADD_MASTER_SIGNER_RESULT_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->startScanDevices(E::STATE_ID_SCR_ADD_HARDWARE_SIGNER);
}

void EVT_ADD_REMOTE_SIGNER_RESULT_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST_HANDLER(QVariant msg) {

}

void EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST_HANDLER(QVariant msg) {

}
