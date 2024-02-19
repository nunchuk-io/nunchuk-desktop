/************************************************************************* 
* This file is part of the Nunchuk software (https://nunchuk.io/)        * 
* Copyright (C) 2020-2022 Enigmo                                         * 
* Copyright (C) 2022 Nunchuk                                             * 
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


#include "STATE_ID_SCR_ADD_HARDWARE_EXIST.h"
#include "AppModel.h"
#include "Premiums/QUserWallets.h"
#include "Premiums/QGroupWallets.h"
#include "ServiceSetting.h"

void SCR_ADD_HARDWARE_EXIST_Entry(QVariant msg) {
    AppModel::instance()->setAddSignerWizard(0);
}

void SCR_ADD_HARDWARE_EXIST_Exit(QVariant msg) {
    AppModel::instance()->setAddSignerWizard(0);
}

void EVT_ADD_EXIST_HARDWARE_REQUEST_HANDLER(QVariant msg) {
    DBG_INFO << QAssistedDraftWallets::IsByzantine() << msg;
    QString action = msg.toMap().value("action").toString();
    if (action == "import-coldcard-via-file") {
        QString file_path = msg.toMap().value("file_path").toString();
        int new_index = msg.toMap().value("new_index").toInt();
        using Enum = QAssistedDraftWallets::ImportColdcard_t;
        QAssistedDraftWallets *instance = QAssistedDraftWallets::IsByzantine() ? dynamic_cast<QAssistedDraftWallets*>(QGroupWallets::instance()) : dynamic_cast<QAssistedDraftWallets*>(QUserWallets::instance());
        auto ret = instance->ImportColdcardViaFile(file_path, new_index);
        if (ret == Enum::eOK) {
            emit instance->reuseKeyGetSignerResult(1);
            if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
                dashboard->GetAlertsInfo();
            }
        }
        else if (ret == Enum::eError_Keep_Screen) {
            // Don't need emit
        }
        else if (ret == Enum::eError_Back) {
            emit instance->reuseKeyGetSignerResult(2);
        }
    }
    else {
        if (QAssistedDraftWallets::IsByzantine()) {
            QGroupWallets::instance()->setSelectFingerPrint(msg.toString());
            QGroupWallets::instance()->AddOrUpdateAKeyToDraftWallet();
        } else {
            QUserWallets::instance()->setSelectFingerPrint(msg.toString());
            QUserWallets::instance()->AddOrUpdateAKeyToDraftWallet();
        }
    }
}

void EVT_ADD_NEW_HARDWARE_REQUEST_HANDLER(QVariant msg) {

}

