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
#include "STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"

void SCR_RECOVER_SOFTWARE_SIGNER_Entry(QVariant msg) {
    AppModel::instance()->setMnemonic("");
    AppModel::instance()->setSuggestMnemonics(bridge::nunchuckGetBIP39WordList());
}

void SCR_RECOVER_SOFTWARE_SIGNER_Exit(QVariant msg) {

}

void EVT_RECOVER_SOFTWARE_SIGNER_REQUEST_HANDLER(QVariant msg) {
    QString mnemonicinput = msg.toString();
    bool checkmnemonic = qUtils::CheckMnemonic(mnemonicinput);
    if(checkmnemonic){
        QEventProcessor::instance()->sendEvent(E::EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED);
        AppModel::instance()->setMnemonic(mnemonicinput);
    }
    else{
        AppModel::instance()->setMnemonic("-101");
        AppModel::instance()->showToast(0, STR_CPP_081, EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void EVT_RECOVER_SOFTWARE_SIGNER_BACK_HANDLER(QVariant msg) {

}

void EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED_HANDLER(QVariant msg) {

}

void EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK_HANDLER(QVariant msg) {

}

void EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION_HANDLER(QVariant msg) {

}
