
#include "STATE_ID_SCR_LOGIN.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void SCR_LOGIN_Entry(QVariant msg) {

}

void SCR_LOGIN_Exit(QVariant msg) {

}

void EVT_LOGIN_PERFORM_PASSWORD_REQUEST_HANDLER(QVariant msg) {
    QWarningMessage warningmsg;
    bool ret =  bridge::nunchukMakeInstance(msg.toString(), warningmsg);
    if(ret && (int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
        QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_SUCCESSFULL);
        AppSetting::instance()->setEnableDBEncryption(true);
        emit AppModel::instance()->loginResult(true);
    }
    else{
        emit AppModel::instance()->loginResult(false);
        DBG_INFO << "CAN NOT MAKE NUNCHUCK INSTANCE";
    }
}

void EVT_LOGIN_SUCCESSFULL_HANDLER(QVariant msg) {

}


