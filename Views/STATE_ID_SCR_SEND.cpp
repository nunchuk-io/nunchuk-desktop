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
#include "STATE_ID_SCR_SEND.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/AppSetting.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"

void SCR_SEND_Entry(QVariant msg) {
    AppModel::instance()->setTransactionInfo(NULL);
    if(AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->walletEscrow()){
        AppModel::instance()->showToast(0, STR_CPP_083, EWARNING::WarningType::WARNING_MSG);
    }
    AppModel::instance()->startGetEstimatedFee();
}

void SCR_SEND_Exit(QVariant msg) {

}

void EVT_SEND_ADD_DESTINATION_REQUEST_HANDLER(QVariant msg) {

}

void EVT_SEND_BACK_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setTransactionInfo(NULL);
}

void EVT_SEND_CREATE_TRANSACTION_REQUEST_HANDLER(QVariant msg) {
    if (auto w = AppModel::instance()->walletInfo()) {
        w->CreateDraftTransaction(E::EVT_SEND_CREATE_TRANSACTION_SUCCEED, msg);
    }
}

void EVT_SEND_CREATE_TRANSACTION_SUCCEED_HANDLER(QVariant msg) {

}

void EVT_SEND_BACK_HOME_SHARED_WL_HANDLER(QVariant msg) {

}

