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
#include "STATE_ID_SCR_CONSOLIDATE_OUTPUT.h"
#include "Models/AppModel.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"

void SCR_CONSOLIDATE_Entry(QVariant msg) {
    AppModel::instance()->setTransactionInfo(NULL);
    if(AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->walletEscrow()){
        AppModel::instance()->showToast(0, STR_CPP_072, EWARNING::WarningType::WARNING_MSG);
    }
}

void SCR_CONSOLIDATE_Exit(QVariant msg) {

}

void EVT_CONSOLIDATE_BACK_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setTransactionInfo(NULL);
}

void EVT_CONSOLIDATE_MAKE_TRANSACTION_HANDLER(QVariant msg) {
    if(auto w = AppModel::instance()->walletInfo()){
        w->CreateDraftTransaction(E::EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED, msg);
    }
}


void EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED_HANDLER(QVariant msg) {

}


