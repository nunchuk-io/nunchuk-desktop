
#include "STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION.h"
#include "Chats/QUserWallets.h"

void SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_Entry(QVariant msg) {

}

void SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_Exit(QVariant msg) {

}

void EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST_HANDLER(QVariant msg) {
    if (QUserWallets::instance()->downloadBackup()) {
        QQuickViewer::instance()->sendEvent(E::EVT_ENTER_BACKUP_PASSWORD_RERQUEST);
    }
}

void EVT_ANSER_SECURITY_QUESTION_BACK_HANDLER(QVariant msg) {

}

void EVT_ENTER_BACKUP_PASSWORD_RERQUEST_HANDLER(QVariant msg) {

}

