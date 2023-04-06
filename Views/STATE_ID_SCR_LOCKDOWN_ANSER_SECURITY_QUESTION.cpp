
#include "STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION.h"
#include "QQuickViewer.h"
#include "Chats/QUserWallets.h"

void SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_Entry(QVariant msg) {

}

void SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_Exit(QVariant msg) {

}

void EVT_INPUT_LOCKDOWN_ANSER_REQUEST_HANDLER(QVariant msg) {
    //Do anything for check password
    if (QUserWallets::instance()->secQuesAnswer()) {
        if (QUserWallets::instance()->lockdownByAnswerSecQues()) {
            QQuickViewer::instance()->sendEvent(E::SCR_LOCKDOWN_SUCCESS_REQUEST);
        }
    }
}

void EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK_HANDLER(QVariant msg) {

}

void SCR_LOCKDOWN_SUCCESS_REQUEST_HANDLER(QVariant msg) {

}

