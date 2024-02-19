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


#include "STATE_ID_SCR_ADD_HARDWARE.h"
#include "Models/AppModel.h"
#include "localization/STR_CPP.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QUserWallets.h"

void SCR_ADD_HARDWARE_Entry(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->setNewKeySignMessage("");
    AppModel::instance()->setAddSignerWizard(0);
    AppModel::instance()->setAddSignerPercentage(0);
}

void SCR_ADD_HARDWARE_Exit(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->setNewKeySignMessage("");
    AppModel::instance()->setAddSignerWizard(0);
    AppModel::instance()->setAddSignerPercentage(0);
}

void EVT_SCAN_HARDWARE_DEVICE_REQUEST_HANDLER(QVariant msg)
{
    Q_UNUSED(msg);
}

void EVT_ADD_HARDWARE_DEVICE_REQUEST_HANDLER(QVariant msg)
{
    DBG_INFO << msg;
    QString signerNameInputted = msg.toMap().value("signerNameInputted").toString();
    int deviceIndexSelected    = msg.toMap().value("deviceIndexSelected").toInt();
    QString action = msg.toMap().value("action").toString();
    if (action == "import-coldcard-via-file") {
        QString file_path = msg.toMap().value("file_path").toString();
        AppModel::instance()->setAddSignerWizard(2);//eADD_LOADING: 2
        using Enum = QAssistedDraftWallets::ImportColdcard_t;
        auto ret = QAssistedDraftWallets::IsByzantine() ? QGroupWallets::instance()->ImportColdcardViaFile(file_path) : QUserWallets::instance()->ImportColdcardViaFile(file_path);
        if (ret == Enum::eOK) {
            AppModel::instance()->setAddSignerWizard(3);
            if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
                dashboard->GetAlertsInfo();
            }
        }
        else if (ret == Enum::eError_Keep_Screen) {
            AppModel::instance()->setAddSignerWizard(5);
        }
        else if (ret == Enum::eError_Back) {

        }
    }
    else {
        DeviceListModel *deviceList = QAssistedDraftWallets::IsByzantine() ? QGroupWallets::instance()->refreshDeviceList() : QUserWallets::instance()->refreshDeviceList();
        if (deviceList && AppModel::instance()->deviceList()) {
            QDevicePtr selectedDv = deviceList->getDeviceByIndex(deviceIndexSelected);
            if (selectedDv) {
                int actualIndex = AppModel::instance()->deviceList()->getDeviceIndexByXfp(selectedDv->masterFingerPrint());
                if (selectedDv.data()->needsPinSent() || selectedDv.data()->needsPassPhraseSent()) {
                    AppModel::instance()->showToast(0, STR_CPP_095, EWARNING::WarningType::WARNING_MSG);
                } else {
                    AppModel::instance()->setAddSignerWizard(2);//eADD_LOADING: 2
                    DBG_INFO << signerNameInputted << actualIndex;
                    AppModel::instance()->startCreateMasterSigner(signerNameInputted, actualIndex);
                }
            }
        }
    }
}
