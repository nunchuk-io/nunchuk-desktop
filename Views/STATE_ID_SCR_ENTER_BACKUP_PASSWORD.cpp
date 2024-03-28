
#include "STATE_ID_SCR_ENTER_BACKUP_PASSWORD.h"
#include "QEventProcessor.h"
#include "AppModel.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QKeyRecovery.h"

void SCR_ENTER_BACKUP_PASSWORD_Entry(QVariant msg) {

}

void SCR_ENTER_BACKUP_PASSWORD_Exit(QVariant msg) {

}

void EVT_INPUT_BACKUP_PASSWORD_REQUEST_HANDLER(QVariant msg) {
    QString password = msg.toString();
    if (ServiceSetting::instance()->servicesTagPtr()->keyRecoveryPtr()->startRecovery(password)) {
        QEventProcessor::instance()->sendEvent(E::EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST);
    }
}

void EVT_ENTER_BACKUP_PASSWORD_BACK_HANDLER(QVariant msg) {

}

void EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST_HANDLER(QVariant msg) {
}

