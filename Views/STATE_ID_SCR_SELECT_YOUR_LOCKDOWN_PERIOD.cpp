
#include "STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD.h"
#include "QQuickViewer.h"
#include "Chats/QUserWallets.h"

void SCR_SELECT_YOUR_LOCKDOWN_PERIOD_Entry(QVariant msg) {

}

void SCR_SELECT_YOUR_LOCKDOWN_PERIOD_Exit(QVariant msg) {

}

void EVT_INPUT_DAYS_VALUE_REQUEST_HANDLER(QVariant msg) {
    //Do anything in here
    DBG_INFO << msg.toMap();
    QString period_id = msg.toMap()["id"].toString();
    if (QUserWallets::instance()->lockdownRequired(period_id)) {
        int type = QUserWallets::instance()->lockdownType();
        if ((int)LOCKDOWN_REQUIRED_TYPE_INT::SIGN_DUMMY_TX == type) {
            QUserWallets::instance()->createDummyTx();
            QQuickViewer::instance()->sendEvent(E::EVT_DUMMY_TRANSACTION_INFO_REQUEST);
        } else if ((int)LOCKDOWN_REQUIRED_TYPE_INT::SECURITY_QUESTION == type) {
            if (QUserWallets::instance()->createSecurityQuestions()) {
                QQuickViewer::instance()->sendEvent(E::EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST);
            }
        }
    }
}

void EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST_HANDLER(QVariant msg) {

}

void EVT_DUMMY_TRANSACTION_INFO_REQUEST_HANDLER(QVariant msg) {

}


