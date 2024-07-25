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
#include "STATE_ID_SCR_LOCKDOWN_SUCCESS.h"
#include "Chats/ClientController.h"
#include "AppModel.h"
#include "localization/STR_CPP.h"
#include "Chats/ClientController.h"
#include "Premiums/QGroupWallets.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"

void SCR_LOCKDOWN_SUCCESS_Entry(QVariant msg) {

}

void SCR_LOCKDOWN_SUCCESS_Exit(QVariant msg) {

}

void EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST_HANDLER(QVariant msg) {
    if (auto w = ServiceSetting::instance()->walletInfoPtr()) {
        if (w->isUserWallet()) { // Lockdown account
            timeoutHandler(0,[=]{
                ClientController::instance()->requestSignout();
            });
            AppModel::instance()->showToast(0, STR_CPP_114, EWARNING::WarningType::SUCCESS_MSG);
        }
        else {
            if (auto dash = w->dashboard()) { // Lock wallet
                dash->GetMemberInfo();
                AppModel::instance()->requestSyncWalletDb(dash->wallet_id());
                QEventProcessor::instance()->sendEvent(E::EVT_CLOSE_LOCKDOWN_SUCCESS);
                ServiceSetting::instance()->servicesTagPtr()->ConfigServiceTag();
            }
        }
    } else { // Lockdown account
        timeoutHandler(0,[=]{
            ClientController::instance()->requestSignout();
        });
        AppModel::instance()->showToast(0, STR_CPP_114, EWARNING::WarningType::SUCCESS_MSG);
    }
}

void EVT_CLOSE_LOCKDOWN_SUCCESS_HANDLER(QVariant msg)
{
//    QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
}
