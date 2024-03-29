
#include "STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION.h"
#include "QEventProcessor.h"
#include "AppModel.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"

void SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_Entry(QVariant msg) {

}

void SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_Exit(QVariant msg) {

}

void EVT_INPUT_LOCKDOWN_ANSER_REQUEST_HANDLER(QVariant msg) {
    //Do anything for check password
    int type = ServiceSetting::instance()->servicesTagPtr()->reqiredSignaturesInfo().type;
    if ((int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX == type) {

    } else if ((int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION == type) {
        if (ServiceSetting::instance()->servicesTagPtr()->secQuesAnswer()) {
            if (ServiceSetting::instance()->servicesTagPtr()->lockdownByAnswerSecQues()) {
                QEventProcessor::instance()->sendEvent(E::EVT_LOCKDOWN_SUCCESS_REQUEST);
            }
        }
    } else if ((int)REQUIRED_SIGNATURE_TYPE_INT::CONFIRMATION_CODE == type) {
        QString code = msg.toString();
        if (ServiceSetting::instance()->servicesTagPtr()->verifyConfirmationCode(code)) {
            if (ServiceSetting::instance()->servicesTagPtr()->lockdownByConfirmationCode()) {
                QEventProcessor::instance()->sendEvent(E::EVT_LOCKDOWN_SUCCESS_REQUEST);
            }
        }
    }
}

void EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK_HANDLER(QVariant msg) {

}
