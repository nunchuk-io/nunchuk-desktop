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


#include "STATE_ID_SCR_ADD_TREZOR.h"
#include "Models/AppModel.h"
#include "localization/STR_CPP.h"

void SCR_ADD_TREZOR_Entry(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->setNewKeySignMessage("");
    AppModel::instance()->setAddSignerWizard(0);
    AppModel::instance()->setAddSignerPercentage(0);
}

void SCR_ADD_TREZOR_Exit(QVariant msg) {
    Q_UNUSED(msg);
    AppModel::instance()->setNewKeySignMessage("");
    AppModel::instance()->setAddSignerWizard(0);
    AppModel::instance()->setAddSignerPercentage(0);
}

void EVT_SCAN_TREZOR_DEVICE_REQUEST_HANDLER(QVariant msg)
{
    Q_UNUSED(msg);
    AppModel::instance()->startScanDevices(E::STATE_ID_SCR_ADD_HARDWARE_SIGNER);
}

void EVT_ADD_TREZOR_DEVICE_REQUEST_HANDLER(QVariant msg)
{
    DBG_INFO;
    QString signerNameInputted = msg.toMap().value("signerNameInputted").toString();
    int deviceIndexSelected    = msg.toMap().value("deviceIndexSelected").toInt();
    if (AppModel::instance()->deviceList()) {
        QDevicePtr selectedDv = AppModel::instance()->deviceList()->getDeviceByIndex(deviceIndexSelected);
        if (selectedDv) {
            if(selectedDv.data()->needsPinSent() || selectedDv.data()->needsPassPhraseSent()){
                AppModel::instance()->showToast(0,
                                                0,
                                                EWARNING::WarningType::WARNING_MSG,
                                                STR_CPP_095);
            } else {
                AppModel::instance()->setAddSignerWizard(2);//_LOADING: 2
                AppModel::instance()->startCreateMasterSigner(signerNameInputted, deviceIndexSelected);
            }
        }
    }
}
