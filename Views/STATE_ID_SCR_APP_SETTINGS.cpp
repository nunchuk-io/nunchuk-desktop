
#include "STATE_ID_SCR_APP_SETTINGS.h"
#include "Models/AppModel.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"
#include <QProcess>

void SCR_APP_SETTING_Entry(QVariant msg) {

}

void SCR_APP_SETTING_Exit(QVariant msg) {

}

void EVT_APP_SETTING_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_APP_SETTING_UPDATE_SETTING_HANDLER(QVariant msg) {
    QWarningMessage warningmsg;
    bridge::nunchukUpdateAppSettings(warningmsg);
    if((int)EWARNING::WarningType::EXCEPTION_MSG != warningmsg.type()){
        QQuickViewer::instance()->sendEvent(E::EVT_APP_SETTING_BACK_REQUEST);
    }
    else {
        if(nunchuk::NunchukException::APP_RESTART_REQUIRED == warningmsg.code()){
            emit AppModel::instance()->requireRestartApp();
        }
    }
}

void EVT_APP_SETTING_CHANGE_PASSPHRASE_HANDLER(QVariant msg) {
    QWarningMessage warningmsg;
    bool ret = bridge::nunchukSetPassphrase(msg.toString(), warningmsg);
    if(!ret || (int)EWARNING::WarningType::EXCEPTION_MSG == warningmsg.type()){
        if((int)nunchuk::NunchukException::PASSPHRASE_ALREADY_USED == warningmsg.code()){
            AppSetting::instance()->setChangePassphraseResult((int)AppSetting::ChangePassphraseResult::PASSPHRASE_ALREADY_USED);
        }
        else if((int)nunchuk::NunchukException::INVALID_PASSPHRASE == warningmsg.code()){
            AppSetting::instance()->setChangePassphraseResult((int)AppSetting::ChangePassphraseResult::INVALID_PASSPHRASE);
        }
        else{
            AppSetting::instance()->setChangePassphraseResult((int)AppSetting::ChangePassphraseResult::CHANGE_ERROR);
        }
    }
    else{
        AppSetting::instance()->setChangePassphraseResult((int)AppSetting::ChangePassphraseResult::CHANGE_SUCCEED);
    }
    DBG_INFO << "Set passphrase:" << ret;
}

void EVT_APP_SETTING_REQUEST_RESTART_HANDLER(QVariant msg) {
    qApp->quit();
    QProcess::startDetached(qApp->applicationFilePath());
    DBG_INFO << "Restart new application's instance";
}

void EVT_APP_SETTING_REQUEST_RESCAN_HANDLER(QVariant msg) {
    AppModel::instance()->startRescanBlockchain(max(msg.toInt(), 0), -1);
}
