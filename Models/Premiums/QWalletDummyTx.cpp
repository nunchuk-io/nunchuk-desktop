#include "QWalletDummyTx.h"
#include "Servers/Byzantine.h"
#include "bridgeifaces.h"
#include "qUtils.h"
#include "Servers/Draco.h"
#include <QFile>
#include <QTextStream>
#include "QGroupWalletHealthCheck.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QServerKey.h"
#include "Premiums/QInheritancePlan.h"
#include "ViewsEnums.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Premiums/QUserWalletDummyTx.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"

QString QDummyTransaction::dummyXfp() const
{
    return mDummyXfp;
}

void QDummyTransaction::setDummyXfp(const QString &xfp)
{
    mDummyXfp = xfp;
}

bool QDummyTransaction::isDummyTx() const
{
    return true;
}

QWalletDummyTx::QWalletDummyTx(WalletId wallet_id)
    : QBasePremium(wallet_id)
    , m_transaction(QDummyTransactionPtr(new QDummyTransaction()))
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QWalletDummyTx::~QWalletDummyTx()
{
    m_transaction.clear();
    mGroup.clear();
    mUser.clear();
}

QString QWalletDummyTx::amountLimit() const
{
    return qUtils::currencyLocale(10000);
}

int QWalletDummyTx::pending_signatures() const
{
    if (auto dash = dashBoardPtr()) {
        QJsonObject payload = dash->alertJson()["payload"].toObject();
        int pending_signatures {0};
        if (!payload.contains("pending_signatures")) {
            QJsonObject reqiredSignatures = ServiceSetting::instance()->servicesTagPtr()->reqiredSignaturesJs();
            pending_signatures = reqiredSignatures["required_signatures"].toInt();
        } else {
            pending_signatures = payload["pending_signatures"].toInt();
        }
        return pending_signatures;
    }
    else {
        QJsonObject reqiredSignatures = ServiceSetting::instance()->servicesTagPtr()->reqiredSignaturesJs();
        int required_signatures = reqiredSignatures["required_signatures"].toInt();
        return required_signatures;
    }
}

bool QWalletDummyTx::CreateDummyTx(const QString& bodyString)
{
    QWarningMessage warningmsg;
    nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(wallet_id(), warningmsg);
    if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
        warningmsg.resetWarningMessage();
        QString tx_to_sign = qUtils::GetHealthCheckDummyTx(wallet, bodyString, warningmsg);
        if((int)EWARNING::WarningType::NONE_MSG == warningmsg.type()){
            warningmsg.resetWarningMessage();
            nunchuk::Transaction tx = qUtils::DecodeDummyTx(wallet, tx_to_sign, warningmsg);
            m_transaction->setNunchukTransaction(tx);
            m_transaction->setWalletId(wallet_id());
            return true;
        }
    }
    return false;
}

QDummyTransactionPtr QWalletDummyTx::transactionPtr() const
{
    return m_transaction;
}

QDummyTransaction *QWalletDummyTx::transactionInfo() const
{
    return m_transaction.data();
}

void QWalletDummyTx::setTransactionInfo(QDummyTransactionPtr tx)
{
    if(m_transaction != tx){
        m_transaction = tx;
        emit transactionInfoChanged();
    }
}

void QWalletDummyTx::finishScanDevices()
{
    if (!m_transaction) return;
    DBG_INFO << m_transaction->singleSignersAssigned()->rowCount();
    emit m_transaction->singleSignerAssignedChanged();
}

QString QWalletDummyTx::tx_id() const
{
    if (m_transaction) {
        return m_transaction->txid();
    }
    return {};
}

QStringList QWalletDummyTx::ExportPsbtViaQR(const QString& request_body, QWarningMessage &msg)
{
    nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(wallet_id(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        msg.resetWarningMessage();
        QString tx_to_sign = qUtils::GetHealthCheckDummyTx(wallet, request_body, msg);
        if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
            msg.resetWarningMessage();
            QStringList qrtags = qUtils::ExportQRTransaction(tx_to_sign, msg);
            if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
                msg.resetWarningMessage();
                return qrtags;
            }
        }
    }
    return {};
}

QString QWalletDummyTx::ImportPsbtViaFile(const QString &filepath)
{
    if (QFile::exists(filepath)) {
        QFile file(filepath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString fileContent = in.readAll();
            file.close();
            return fileContent;
        }
    }
    return "";
}

void QWalletDummyTx::ExportPsbtViaFile(const QString &filepath)
{
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream st(&file);
        st.setCodec("UTF-8");
        st << transactionPtr()->psbt() << endl;
        st.flush();
        file.close();
    }
}

QJsonObject QWalletDummyTx::nonceBody() const
{
    return m_nonceBody;
}

QString QWalletDummyTx::bodyString() const
{
    QJsonDocument doc(m_nonceBody);
    QString bodyString(doc.toJson());
    return bodyString;
}

void QWalletDummyTx::init(WalletId wallet_id)
{
    mGroup = QGroupWalletDummyTxPtr(new QGroupWalletDummyTx(wallet_id));
    mUser = QUserWalletDummyTxPtr(new QUserWalletDummyTx(wallet_id));
}


template<>
QGroupWalletDummyTxPtr QWalletDummyTx::get<QGroupWalletDummyTxPtr>()
{
    return mGroup;
}

template<>
QUserWalletDummyTxPtr QWalletDummyTx::get<QUserWalletDummyTxPtr>()
{
    return mUser;
}
