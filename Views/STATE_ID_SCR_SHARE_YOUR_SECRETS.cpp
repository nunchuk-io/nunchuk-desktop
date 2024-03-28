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


#include "STATE_ID_SCR_SHARE_YOUR_SECRETS.h"
#include "ServiceSetting.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QGroupDashboard.h"

void SCR_SHARE_YOUR_SECRETS_Entry(QVariant msg) {

}

void SCR_SHARE_YOUR_SECRETS_Exit(QVariant msg) {

}

void EVT_UPDATE_YOUR_SECRET_REQUEST_HANDLER(QVariant msg) {
    QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
    if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
        if (dashboard->flow() == (int)AlertEnum::E_Alert_t::CREATE_INHERITANCE_PLAN_SUCCESS) {
            QtConcurrent::run([dashboard](){
                dashboard->DismissAlert();
                dashboard->GetAlertsInfo();
            });
        }
    }
}

