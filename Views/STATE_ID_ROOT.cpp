
#include "STATE_ID_ROOT.h"
#include "Models/AppModel.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"

void ROOT_Entry(QVariant msg) {
    Q_UNUSED(msg);
}

void ROOT_Exit(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ONS_CLOSE_REQUEST_HANDLER(QVariant msg) {
    switch (msg.toInt()) {
    case E::STATE_ID_SCR_SEND:
    case E::STATE_ID_SCR_ADD_WALLET:
    case E::STATE_ID_SCR_ADD_WALLET_CONFIRMATION:
    case E::STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION:
    case E::STATE_ID_SCR_ADD_NEW_SIGNER:
    {
        AppModel::instance()->resetSignersChecked();
        break;
    }
    case E::STATE_ID_SCR_TRANSACTION_INFO :
    {
        AppModel::instance()->setTransactionInfo(QSharedPointer<Transaction>(new Transaction()));
        if(AppModel::instance()->transactionInfo()){
            AppModel::instance()->transactionInfo()->warningMessage()->resetWarningMessage();
        }
        AppModel::instance()->setUtxoInfo(QSharedPointer<UTXO>(new UTXO()));
    }
        break;
    case E::STATE_ID_SCR_UTXOS :
    case E::STATE_ID_SCR_UTXO_OUTPUT :
    {
        AppModel::instance()->setUtxoInfo(QSharedPointer<UTXO>(new UTXO()));
    }
        break;
    default:
        break;
    }
}

void EVT_LOGIN_REQUEST_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_STARTING_APPLICATION_LOCALMODE_HANDLER(QVariant msg) {
    if(AppSetting::instance()->enableDBEncryption()){
        QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_REQUEST);
    }
    else{
        QWarningMessage warningmsg;
        bool ret =  bridge::nunchukMakeInstance("", warningmsg);
        if(ret && (int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
            QQuickViewer::instance()->sendEvent(E::EVT_HOME_REQUEST_NOPASS);
        }
        else if((int)EWARNING::WarningType::EXCEPTION_MSG == warningmsg.type() && nunchuk::NunchukException::INVALID_PASSPHRASE == warningmsg.code()){
            DBG_INFO << "Even encription is disabled, it look like last state was enabled encript, but setting was removed, and it loaded with default";
            QQuickViewer::instance()->sendEvent(E::EVT_LOGIN_REQUEST);
        }
        else{
            DBG_INFO << "CAN NOT MAKE NUNCHUCK INSTANCE";
        }
    }
}

void EVT_HOME_REQUEST_NOPASS_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ROOT_PROMT_PIN_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ROOT_PROMT_PASSPHRASE_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}
