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


#include "STATE_ID_SCR_CHANGE_EMAIL.h"
#include "QEventProcessor.h"
#include "ProfileSetting.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"

void SCR_CHANGE_EMAIL_Entry(QVariant msg) {
    ProfileSetting::instance()->setNewEmail("");
    ServiceSetting::instance()->servicesTagPtr()->setReqiredSignatures({});
}

void SCR_CHANGE_EMAIL_Exit(QVariant msg) {

}

void EVT_CHANGE_EMAIL_ENTER_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    if (type == "email-name") {
        QString new_email = maps["new_email"].toString();
        if(ServiceSetting::instance()->servicesTagPtr()->RequestConfirmationChangeEmail(new_email)) {
            ProfileSetting::instance()->setNewEmail(new_email);
        }
    }
    else if (type == "email-code") {
        QString codeEmail = maps["codeEmail"].toString();
        if (ServiceSetting::instance()->servicesTagPtr()->verifyConfirmationCode(codeEmail)) {
            ProfileSetting::instance()->CalculateRequireSignaturesForChangingEmail();
        }
    }
}

