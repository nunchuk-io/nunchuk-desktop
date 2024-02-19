
#include "STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION.h"
#include "AppModel.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QKeyRecovery.h"
#include "Premiums/QGroupWallets.h"

void SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_Entry(QVariant msg) {

}

void SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_Exit(QVariant msg) {

}

void EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST_HANDLER(QVariant msg) {
    int type = ServiceSetting::instance()->servicesTagPtr()->reqiredSignaturesInfo().type;
    if ((int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX == type) {

    } else if ((int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION == type) {
        if (ServiceSetting::instance()->servicesTagPtr()->keyRecoveryPtr()->KeyRecoveryDownloadBackup()) {
            QQuickViewer::instance()->sendEvent(E::EVT_ENTER_BACKUP_PASSWORD_RERQUEST);
        }
    } else if ((int)REQUIRED_SIGNATURE_TYPE_INT::CONFIRMATION_CODE == type) {
        QString code = msg.toString();
        if (ServiceSetting::instance()->servicesTagPtr()->verifyConfirmationCode(code)) {
            QJsonObject js = ServiceSetting::instance()->servicesTagPtr()->reqiredSignaturesJs();
            // After confirm code
            QString step = js["step"].toString();
            if (step == "REQUEST_RECOVER") {
                ServiceSetting::instance()->servicesTagPtr()->keyRecoveryPtr()->UserKeysRequestRecoveryKey();
            }
        }
    }  else if ((int)REQUIRED_SIGNATURE_TYPE_INT::NONE == type) {
        QJsonObject js = ServiceSetting::instance()->servicesTagPtr()->reqiredSignaturesJs();
        QString step = js["step"].toString();
        if (step == "RECOVER") {
            QString password = msg.toString();
            if (ServiceSetting::instance()->servicesTagPtr()->keyRecoveryPtr()->startRecovery(password)) {
                QQuickViewer::instance()->sendEvent(E::EVT_ENTER_BACKUP_PASSWORD_RERQUEST);
                if (ServiceSetting::instance()->servicesTagPtr()->keyRecoveryPtr()->UserKeysMarkRecoverStatus())
                {
                    if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
                        dashboard->GetAlertsInfo();
                    }
                }
            }
        }
    }
}

void EVT_ANSER_SECURITY_QUESTION_BACK_HANDLER(QVariant msg) {

}

void EVT_ENTER_BACKUP_PASSWORD_RERQUEST_HANDLER(QVariant msg) {

}

