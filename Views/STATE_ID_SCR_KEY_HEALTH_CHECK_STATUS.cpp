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


#include "STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QGroupWalletHealthCheck.h"
#include "Premiums/QGroupWalletDummyTx.h"

void SCR_KEY_HEALTH_CHECK_STATUS_Entry(QVariant msg) {

}

void SCR_KEY_HEALTH_CHECK_STATUS_Exit(QVariant msg) {

}

void EVT_KEY_HEALTH_CHECK_STATUS_ENTER_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << type;
    if (type == "health-check") {
        QGroupWallets::instance()->dashboardInfoPtr()->setConfigFlow("health-check-procedure");
        QQuickViewer::instance()->sendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
        QString xfp = maps["xfp"].toString();
        QGroupWallets::instance()->dashboardInfoPtr()->healthPtr()->HealthCheckForKey(xfp);
    } else if (type == "request-health-check") {
        QString xfp = maps["xfp"].toString();
        QGroupWallets::instance()->dashboardInfoPtr()->healthPtr()->RequestHealthCheckForKey(xfp);
    } else if (type == "health-check-request-sent") {
        QGroupWallets::instance()->dashboardInfoPtr()->setConfigFlow("health-check-request-sent");
        QQuickViewer::instance()->sendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
    }
}

