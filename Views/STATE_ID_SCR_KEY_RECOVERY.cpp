
#include "STATE_ID_SCR_KEY_RECOVERY.h"
#include "Chats/QUserWallets.h"

void SCR_KEY_RECOVERY_Entry(QVariant msg) {

}

void SCR_KEY_RECOVERY_Exit(QVariant msg) {

}

void EVT_INPUT_TAPSIGNER_SELECT_REQUEST_HANDLER(QVariant msg) {
    QMap<QString,QVariant> map = msg.toMap();
    QUserWallets::instance()->keyRecovery(map["fingerPrint"].toString());
    if (QUserWallets::instance()->createSecurityQuestions()) {
        QQuickViewer::instance()->sendEvent(E::EVT_ANSER_SECURITY_QUESTION_REQUEST);
    }
}

void EVT_ANSER_SECURITY_QUESTION_REQUEST_HANDLER(QVariant msg) {

}

