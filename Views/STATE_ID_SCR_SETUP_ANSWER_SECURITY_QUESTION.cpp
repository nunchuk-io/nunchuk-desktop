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


#include "STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION.h"
#include "AppModel.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QKeyRecovery.h"
void SCR_SETUP_ANSWER_SECURITY_QUESTION_Entry(QVariant msg) {

}

void SCR_SETUP_ANSWER_SECURITY_QUESTION_Exit(QVariant msg) {

}

void EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ_HANDLER(QVariant msg) {
    int type = ServiceSetting::instance()->servicesTagPtr()->reqiredSignaturesInfo().type;
    if ((int)REQUIRED_SIGNATURE_TYPE_INT::CONFIRMATION_CODE == type) {
        QString code = msg.toString();
        if (ServiceSetting::instance()->servicesTagPtr()->verifyConfirmationCode(code)) {
            DBG_INFO << "success: " << code;
            ServiceSetting::instance()->servicesTagPtr()->keyRecoveryPtr()->setRequireQuestions({});
            if (ServiceSetting::instance()->servicesTagPtr()->keyRecoveryPtr()->UpdateSecurityQuestions()) {
                QEventProcessor::instance()->sendEvent(E::EVT_SETUP_SECURITY_QUESTION_REQUEST);
                QString msg_name = QString("Security questions updated");
                AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
    }
}

void EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK_HANDLER(QVariant msg) {

}

