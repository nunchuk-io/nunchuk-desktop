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


#include "STATE_ID_SCR_SELECT_SERVER.h"
#include "Chats/ClientController.h"
#include "ProfileSetting.h"

void SCR_SELECT_SERVER_Entry(QVariant msg) {

}

void SCR_SELECT_SERVER_Exit(QVariant msg) {

}

void EVT_SELECT_SERVER_ENTER_HANDLER(QVariant msg) {
    QMap<QString, QVariant> data = msg.toMap();
    QString type = data["type"].toString();
    if (type == "enter-add-server") {
        QString serverAddress = data["serverAddress"].toString();
        QMap<QString, QVariant> server;
        server["url"] = serverAddress;
        server["name"] = serverAddress;
        server["isLocal"] = true;
        if (ProfileSetting::instance()->addMainnetServer(QVariant::fromValue(server))) {
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
        }
    } else if (type == "enter-remove-server") {
        QString serverAddress = data["serverAddress"].toString();
        QString serverName = data["serverName"].toString();
        QMap<QString, QVariant> server;
        server["url"] = serverAddress;
        server["name"] = serverName;
        ProfileSetting::instance()->removeMainnetServer(QVariant::fromValue(server));
    } else if (type == "save-enter-edit") {
        ProfileSetting::instance()->resetDefaultMainnetServers();
        ProfileSetting::instance()->saveMainnetServers();
        QString msg = QString("Server list updated");
        AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
    }
}

