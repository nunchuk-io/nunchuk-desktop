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


#include "STATE_ID_SCR_EDIT_MEMBERS.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QGroupWallets.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"

void SCR_EDIT_MEMBERS_Entry(QVariant msg) {

}

void SCR_EDIT_MEMBERS_Exit(QVariant msg) {

}

void EVT_EDIT_MEMBERS_ENTER_REQUEST_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << type;
    if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
        if (type == "add-new-member") {
            QString roleId = maps["roleId"].toString();
            QJsonObject member;
            member["email_or_username"] = "";
            member["role"] = roleId;
            member["permissions"] = {};
            dashboard->addMember(member);
        } else if (type == "remove-member") {
            QString email_or_username = maps["email_or_username"].toString();
            QJsonObject member;
            member["email_or_username"] = email_or_username;
            member["role"] = {};
            member["permissions"] = {};
            dashboard->removeMember(member);
        } else if (type == "replace-new-member") {
            QString roleId = maps["roleId"].toString();
            QString email_or_username = maps["email_or_username"].toString();
            QString indexMember = maps["indexMember"].toString();
            QJsonObject member;
            member["email_or_username"] = email_or_username;
            member["role"] = roleId;
            member["permissions"] = {};
            dashboard->editMembers(member, indexMember.toInt());
        } else if (type == "save-member-changed") {
            QMap<QString, QVariant> data;
            data["state_id"] = E::STATE_ID_SCR_EDIT_MEMBERS;
            QEventProcessor::instance()->sendEvent(E::EVT_REENTER_YOUR_PASSWORD_REQUEST, data);
        } else if (type == "edit-member-email-code") {
            QString codeEmail = maps["codeEmail"].toString();
            if (ServiceSetting::instance()->servicesTagPtr()->verifyConfirmationCode(codeEmail)) {
                dashboard->EditGroupMembers();
            }
        }
    }
}

void EVT_EDIT_MEMBERS_CLOSE_HANDLER(QVariant msg) {

}

