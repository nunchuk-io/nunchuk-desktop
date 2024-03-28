
#include "STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD.h"
#include "QEventProcessor.h"
#include "AppModel.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"

void SCR_SELECT_YOUR_LOCKDOWN_PERIOD_Entry(QVariant msg) {

}

void SCR_SELECT_YOUR_LOCKDOWN_PERIOD_Exit(QVariant msg) {

}

void EVT_INPUT_DAYS_VALUE_REQUEST_HANDLER(QVariant msg) {
    //Do anything in here
    DBG_INFO << msg.toMap();
    QString period_id = msg.toMap()["id"].toString();
    if (ServiceSetting::instance()->servicesTagPtr()->lockdownRequired(period_id)) {
        int type = ServiceSetting::instance()->servicesTagPtr()->reqiredSignaturesInfo().type;
        if ((int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX == type) {

        } else if ((int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION == type) {
            if (ServiceSetting::instance()->servicesTagPtr()->CreateSecurityQuestionsAnswered()) {
                QEventProcessor::instance()->sendEvent(E::EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST);
            }
        } else if ((int)REQUIRED_SIGNATURE_TYPE_INT::CONFIRMATION_CODE == type) {
            if (ServiceSetting::instance()->servicesTagPtr()->RequestConfirmationCodeEmergencyLockdown()) {
                QEventProcessor::instance()->sendEvent(E::EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST);
            }
        }
    }
}

void EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST_HANDLER(QVariant msg) {

}

