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


#include "STATE_ID_SCR_HEALTH_CHECK_STARTING.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QGroupWalletHealthCheck.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QUserWalletDummyTx.h"
#include <QJsonObject>

void SCR_HEALTH_CHECK_STARTING_Entry(QVariant msg) {

}

void SCR_HEALTH_CHECK_STARTING_Exit(QVariant msg) {

}

void EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << type;
    auto dashboard = QGroupWallets::instance()->dashboardInfoPtr();
    QJsonObject payload = dashboard->alertJson()["payload"].toObject();
    DBG_INFO << payload;
    if (type == "Ill-do-this-later") {
        QString dummy_transaction_id = payload["dummy_transaction_id"].toString();
        if (!dummy_transaction_id.isEmpty()) {
            if (auto health = dashboard->healthPtr()) {
                if (health->HealthCheckPendingForTx(dummy_transaction_id)) {
                    QQuickViewer::instance()->sendEvent(E::EVT_ONS_CLOSE_REQUEST);
                    dashboard->GetAlertsInfo();
                }
            }
        }
    }
    else if (type == "Sign-dummy-transaction") {
        QString dummy_transaction_id = payload["dummy_transaction_id"].toString();
        if (!dummy_transaction_id.isEmpty()) {
            if (auto health = dashboard->healthPtr()) {
                if (health->HealthCheckPendingForTx(dummy_transaction_id)) {
                    QQuickViewer::instance()->sendEvent(E::EVT_DUMMY_TRANSACTION_INFO_REQUEST);
                    dashboard->GetAlertsInfo();
                }
            }
        }
    }
    else {}
}

void EVT_HEALTH_CHECK_STARTING_CLOSE_HANDLER(QVariant msg) {

}

