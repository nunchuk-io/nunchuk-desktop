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
#include "STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"

void SCR_ADD_HARDWARE_SIGNER_TO_WALLET_Entry(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->setSingleSignerInfo(QSingleSignerPtr(new SingleSigner()));
    AppModel::instance()->setAddSignerStep(-1);
    AppModel::instance()->setAddSignerPercentage(0);
    AppModel::instance()->setMsgKeyHealthcheck("");
}

void SCR_ADD_HARDWARE_SIGNER_TO_WALLET_Exit(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->setMsgKeyHealthcheck("");
    AppModel::instance()->setAddSignerStep(-1);
    AppModel::instance()->setAddSignerPercentage(0);
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_HANDLER(QVariant msg) {
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

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_HANDLER(QVariant msg) {
    AppModel::instance()->setSingleSignerInfo(QSingleSignerPtr(new SingleSigner()));
    // Trimmed input
    QString signerNameInputted = msg.toMap().value("signerNameInputted").toString().simplified();
    QString xpubOrPublInputted = msg.toMap().value("xpubOrPublInputted").toString().simplified();
    QString bip32Inputted = msg.toMap().value("bip32Inputted").toString().simplified();
    QString masterFingerPrintInputted = msg.toMap().value("masterFingerPrintInputted").toString().simplified();

    bool isValidDerivationPath = qUtils::QIsValidDerivationPath(bip32Inputted) ;
    bool isValidFingerPrint = qUtils::QIsValidFingerPrint(masterFingerPrintInputted);
    bool isValidXpub = false;
    bool isValidPublicKey = false;
    bool inputValid = false;

    QString publickey = "";
    QString xpub = "";
    if(AppModel::instance()->newWalletInfo()->escrow()){
        isValidPublicKey = qUtils::QIsValidPublicKey(xpubOrPublInputted);
        if(isValidPublicKey){
            DBG_INFO << "Input is Public key";
            publickey = xpubOrPublInputted;
            xpub = "";
        }
        else{
            isValidXpub = qUtils::QIsValidXPub(xpubOrPublInputted, xpub);
            if(isValidXpub){ publickey = ""; }
        }
        inputValid = isValidDerivationPath && isValidFingerPrint && (isValidXpub || isValidPublicKey);
    }
    else {
        isValidXpub = qUtils::QIsValidXPub(xpubOrPublInputted, xpub);
        if(isValidXpub){ publickey = ""; }
        inputValid = isValidDerivationPath && isValidFingerPrint && isValidXpub;
    }
    if(inputValid){
        AppModel::instance()->startCreateRemoteSigner(signerNameInputted,
                                                      xpub,
                                                      "",
                                                      bip32Inputted,
                                                      masterFingerPrintInputted,
                                                      E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER);
    }
    else{
       if(AppModel::instance()->singleSignerInfo()){
           AppModel::instance()->singleSignerInfo()->setDerivationPath(!isValidDerivationPath ? "false" : "");
           AppModel::instance()->singleSignerInfo()->setMasterFingerPrint(!isValidFingerPrint ? "false" : "");
           AppModel::instance()->singleSignerInfo()->setXpub(!isValidXpub ? "false" : "");
           AppModel::instance()->singleSignerInfo()->setPublickey(!isValidPublicKey ? "false" : "");
       }
       emit AppModel::instance()->finishedCreateRemoteSigner();
    }
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->startScanDevices(E::STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET);
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN_HANDLER(QVariant msg) {

}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN_HANDLER(QVariant msg) {

}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}


void EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

