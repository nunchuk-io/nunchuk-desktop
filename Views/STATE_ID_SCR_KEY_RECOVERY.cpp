
#include "STATE_ID_SCR_KEY_RECOVERY.h"
#include "AppModel.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QKeyRecovery.h"

void SCR_KEY_RECOVERY_Entry(QVariant msg) {

}

void SCR_KEY_RECOVERY_Exit(QVariant msg) {

}

void EVT_INPUT_TAPSIGNER_SELECT_REQUEST_HANDLER(QVariant msg) {
    QMap<QString,QVariant> map = msg.toMap();
    QString fingerPrint = map["key_xfp"].toString();
    if (auto tag = ServiceSetting::instance()->servicesTagPtr()) {
        if (auto key = tag->keyRecoveryPtr()) {
            key->UserKeysCalculateRequiredSignatures(fingerPrint);
        }
    }
}

void EVT_ANSER_SECURITY_QUESTION_REQUEST_HANDLER(QVariant msg) {

}

