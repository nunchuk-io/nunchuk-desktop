#include "QServerKey.h"
#include <QQmlEngine>
#include <QJsonArray>
#include <QJsonObject>
#include "qUtils.h"
#include "WalletModel.h"
#include "QGroupDashboard.h"
#include "AppModel.h"
#include "WalletModel.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "localization/STR_CPP.h"
#include "ServiceSetting.h"
#include "QWalletServicesTag.h"
#include "Premiums/QGroupWalletHealthCheck.h"
#include "Premiums/QGroupWalletDummyTx.h"

QServerKey::QServerKey(const QString &wallet_id)
    : QBasePremium(wallet_id)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    QJsonObject maps;
    maps["auto_broadcast_transaction"] = true;
    maps["signing_delay_seconds"] = 0;
    m_keyCoSigning = maps;
}

QVariant QServerKey::policies() const
{
    return QVariant::fromValue(m_policies);
}

void QServerKey::setPolicies(QJsonObject policies)
{
    m_policies = policies;
    emit policiesChanged();
}

QVariant QServerKey::policiesOld() const
{
    return QVariant::fromValue(m_policiesOld);
}

void QServerKey::setPoliciesOld(QJsonObject policies)
{
    if (m_policiesOld == policies)
        return;

    m_policiesOld = policies;
    emit policiesOldChanged();
}

QVariant QServerKey::policiesNew() const
{
    return QVariant::fromValue(m_policiesNew);
}

void QServerKey::setPoliciesNew(QJsonObject policies)
{
    if (m_policiesNew == policies)
        return;
    m_policiesNew = policies;
    emit policiesNewChanged();
}

QVariant QServerKey::broadcastDelay() const
{
    QJsonObject broadcastDelay;
    broadcastDelay["auto_broadcast_transaction"] = m_policiesNew["auto_broadcast_transaction"];
    broadcastDelay["signing_delay_seconds"] = m_policiesNew["signing_delay_seconds"];
    broadcastDelay["auto_broadcast_transaction_isChanged"] = false;
    broadcastDelay["signing_delay_seconds_isChanged"] = false;
    if (m_policiesNew["auto_broadcast_transaction"] != m_policiesOld["auto_broadcast_transaction"]) {
        broadcastDelay["auto_broadcast_transaction_isChanged"] = true;
    }
    if (m_policiesNew["signing_delay_seconds"] != m_policiesOld["signing_delay_seconds"]) {
        broadcastDelay["signing_delay_seconds_isChanged"] = true;
    }
    return QVariant::fromValue(broadcastDelay);
}

void QServerKey::UpdateFromDummyTx(QJsonObject data)
{
    DBG_INFO << data;
    setPoliciesOld(ConvertToDisplayQml(data["payload"].toObject()["old_policies"].toObject()));
    setPoliciesNew(ConvertToDisplayQml(data["payload"].toObject()["new_policies"].toObject()));
    emit spendingLimitChangeChanged();
}

void QServerKey::UpdateFromWallet(QJsonObject data)
{
    setPolicies(ConvertToDisplayQml(data["server_key"].toObject()["policies"].toObject()));
}

void QServerKey::UpdateFromServerKey(QJsonObject data)
{
    setPolicies(ConvertToDisplayQml(data));
}

QVariantList QServerKey::spendingLimitChange() const
{
    QJsonArray membersLimit;
    QJsonObject groupInfo;
    if (walletInfoPtr() && walletInfoPtr()->dashboard()) {
        groupInfo = walletInfoPtr()->dashboard()->groupInfo();
    }
    QJsonArray new_policies = m_policiesNew["members_spending_limit"].toArray();
    QJsonArray old_policies = m_policiesOld["members_spending_limit"].toArray();

    auto find_id = [&](const QString& membership_id) ->QJsonObject{
        for (auto new_member_limit : new_policies) {
            QString new_membership_id_limit = new_member_limit.toObject()["membership_id"].toString();
            if (old_policies.size() > 0) {
                for (auto old_member_limit : old_policies) {
                    QString old_membership_id_limit = old_member_limit.toObject()["membership_id"].toString();
                    if (qUtils::strCompare(membership_id, new_membership_id_limit) && qUtils::strCompare(new_membership_id_limit, old_membership_id_limit)) {
                        QJsonObject member_limit = new_member_limit.toObject();
                        if (new_member_limit != old_member_limit) {
                            member_limit["isChanged"] = true;
                        } else {
                            member_limit["isChanged"] = false;
                        }
                        return member_limit;
                    }
                }
            }
            else if (qUtils::strCompare(membership_id, new_membership_id_limit))
            {
                QJsonObject member_limit = new_member_limit.toObject();
                member_limit["isChanged"] = true;
                return member_limit;
            }
        }
        return {};
    };

    QJsonArray members = groupInfo["members"].toArray();
    for (auto member : members) {
        QString membership_id = member.toObject()["membership_id"].toString();
        QJsonObject membership = member.toObject();
        QString role = membership["role"].toString();
        if (role == "OBSERVER") continue;
        membership["spending_limit"] = find_id(membership_id)["spending_limit"];
        membership["isChanged"] = find_id(membership_id)["isChanged"];
        membersLimit.append(membership);
    }
    return membersLimit.toVariantList();
}

QVariantList QServerKey::spendingLimitCurrent() const
{
    QJsonObject groupInfo;
    if (walletInfoPtr() && walletInfoPtr()->dashboard()) {
        groupInfo = walletInfoPtr()->dashboard()->groupInfo();
    }

    QJsonArray members_spending_limit = m_policies["members_spending_limit"].toArray();
    QJsonArray members = groupInfo["members"].toArray();
    QJsonArray membersLimit;
    for (auto member : members) {
        QString membership_id = member.toObject()["membership_id"].toString();
        for (auto member_limit : members_spending_limit) {
            QString membership_id_limit = member_limit.toObject()["membership_id"].toString();
            if (qUtils::strCompare(membership_id, membership_id_limit)) {
                QJsonObject membership = member.toObject();
                membership["spending_limit"] = member_limit.toObject()["spending_limit"];
                membersLimit.append(membership);
            }
        }
    }
    return membersLimit.toVariantList();
}

QVariant QServerKey::hbSpendingLimitChange() const
{
    QJsonValue new_member_limit = m_policiesNew["spending_limit"];
    QJsonValue old_member_limit = m_policiesOld["spending_limit"];
    QJsonObject member_limit = new_member_limit.toObject();
    if (new_member_limit != old_member_limit) {
        member_limit["isChanged"] = true;
    } else {
        member_limit["isChanged"] = false;
    }
    QJsonObject membership;
    membership["spending_limit"] = member_limit;
    membership["isChanged"] = member_limit["isChanged"];
    return QVariant::fromValue(membership);
}

QString QServerKey::groupId() const
{
    QWalletPtr w = ServiceSetting::instance()->walletInfoPtr();
    if (w.isNull()) return "";
    return w->groupId();
}

nunchuk::SingleSigner QServerKey::ServerKey()
{
    QWarningMessage msgWallet;
    if (m_server_key.get_master_fingerprint() == "") {
        nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(wallet_id(), msgWallet);
        for (auto key : wallet.get_signers()) {
            if (key.get_type() == nunchuk::SignerType::SERVER) {
                m_server_key = key;
            }
        }
    }
    return m_server_key;
}

bool QServerKey::serverKeyGetCurrentPolicies()
{
    QJsonObject output;
    QString errormsg = "";
    bool ret {false};
    if (isUserWallet()) {
        ret = Draco::instance()->ServerKeysGet(QString::fromStdString(ServerKey().get_master_fingerprint()),
                                               QString::fromStdString(ServerKey().get_derivation_path()),
                                               output,
                                               errormsg);
    }
    else {
        ret = Byzantine::instance()->ServerKeysGet(groupId(),
                                                   QString::fromStdString(ServerKey().get_master_fingerprint()),
                                                   QString::fromStdString(ServerKey().get_derivation_path()),
                                                   output,
                                                   errormsg);
    }
    if(ret){
        DBG_INFO << output;
        QJsonObject policies = output["policies"].toObject();
        UpdateFromServerKey(policies);
    } else {
        DBG_INFO << errormsg;
    }
    return ret;
}

QJsonObject QServerKey::serverKeyBody()
{
    QJsonObject data;
    data["name"] = walletInfoPtr()->walletName();
    data["wallet"] = wallet_id();
    data["policies"] = m_keyCoSigning;
    return data;
}

bool QServerKey::ServerKeyRequiredSignature()
{
    QJsonObject data = serverKeyBody();
    QString errormsg = "";
    QJsonObject output;
    DBG_INFO << data;
    bool ret {false};
    if (isUserWallet()) {
        ret = Draco::instance()->ServerKeysRequiredSignature(QString::fromStdString(ServerKey().get_master_fingerprint()),
                                                             QString::fromStdString(ServerKey().get_derivation_path()),
                                                             data,
                                                             output,
                                                             errormsg);
    }
    else {
        ret = Byzantine::instance()->ServerKeysRequiredSignature(groupId(),
                                                                 QString::fromStdString(ServerKey().get_master_fingerprint()),
                                                                 QString::fromStdString(ServerKey().get_derivation_path()),
                                                                 data,
                                                                 output,
                                                                 errormsg);
    }
    if (ret) {
        QJsonObject resultObj = output["result"].toObject();
        servicesTagPtr()->setReqiredSignatures(resultObj);
        ReqiredSignaturesInfo required_question = servicesTagPtr()->reqiredSignaturesInfo();
        if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION) {
            servicesTagPtr()->CreateSecurityQuestionsAnswered();
            return true;
        } else if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX) {
            ServerKeyUpdatePoliciesChange();
            //Show popup support in moble
            emit serverKeyDummyTransactionAlert();
        } else {
            ServerKeyUpdatePoliciesSucceed();
            return false;
        }
    }
    return ret;
}

bool QServerKey::ServerKeyUpdate(QJsonObject &output, QString &errormsg, bool isDraft)
{
    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"] = serverKeyBody();
    QStringList authorizations;
    bool ret {false};
    if (isUserWallet()) {
        ret = Draco::instance()->ServerKeysUpdate(servicesTagPtr()->passwordToken(),
                                                  servicesTagPtr()->secQuesToken(),
                                                  QString::fromStdString(ServerKey().get_master_fingerprint()),
                                                  QString::fromStdString(ServerKey().get_derivation_path()),
                                                  authorizations,
                                                  data,
                                                  output,
                                                  errormsg);
    }
    else {
        ret = Byzantine::instance()->ServerKeysUpdate(groupId(),
                                                      servicesTagPtr()->passwordToken(),
                                                      servicesTagPtr()->secQuesToken(),
                                                      QString::fromStdString(ServerKey().get_master_fingerprint()),
                                                      QString::fromStdString(ServerKey().get_derivation_path()),
                                                      authorizations,
                                                      data,
                                                      isDraft,
                                                      output,
                                                      errormsg);
    }
    return ret;
}

bool QServerKey::ServerKeyUpdatePoliciesChange()
{
    QJsonObject output;
    QString errormsg = "";
    bool ret = ServerKeyUpdate(output, errormsg, true);
    DBG_INFO << ret;
    if(ret){
        QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
        if (auto dummy = groupDummyTxPtr()) {
            dummy->setDummyTxData(dummy_transaction);
        }
    }
    else {
        //Show toast ?
    }
    return ret;
}

bool QServerKey::ServerKeyUpdatePoliciesSucceed()
{
    QJsonObject output;
    QString errormsg = "";
    if (auto dashBoard = dashBoardPtr()) {
        UpdateFromWallet(dashBoard->walletJson());
        ServiceSetting::instance()->clearWalletInfo();
        ServiceSetting::instance()->setWalletInfo(walletInfoPtr());
    }
    bool ret = ServerKeyUpdate(output, errormsg, false);
    if (ret) {
        emit securityQuestionClosed();
        AppModel::instance()->showToast(0, STR_CPP_117, EWARNING::WarningType::SUCCESS_MSG );
        timeoutHandler(2 * 60 *1000,[=]{
            serverKeyGetCurrentPolicies();
            ServiceSetting::instance()->clearWalletInfo();
            ServiceSetting::instance()->setWalletInfo(walletInfoPtr());
        });
    }
    return ret;
}

QVariant QServerKey::keyCoSigning() const
{
    return QVariant::fromValue(m_keyCoSigning);
}

bool QServerKey::hasServerKey()
{
    // if (auto dash = dashBoardPtr()) {
    //     auto js = dash->walletJson()["server_key"].toObject();
    //     DBG_INFO << walletInfoPtr()->name() << js;
    //     return !js.isEmpty();
    // }
    return ServerKey().get_master_fingerprint() != "";
}

void QServerKey::setKeyCoSigning(const QVariant &val)
{
    QMap<QString,QVariant> maps = val.toMap();
    QJsonObject data = m_keyCoSigning;
    if (maps["apply_same_spending_limit"].isValid()) {
        data["apply_same_spending_limit"] = maps["apply_same_spending_limit"].toBool();
    }
    if (maps["signing_delay_seconds"].isValid()) {
        data["signing_delay_seconds"] = maps["signing_delay_seconds"].toDouble();
    }
    if (maps["auto_broadcast_transaction"].isValid()) {
        data["auto_broadcast_transaction"] = maps["auto_broadcast_transaction"].toBool();
    }
    if (maps["members_spending_limit"].isValid()) {
        data["members_spending_limit"] = maps["members_spending_limit"].toJsonArray();
    }
    if (maps["spending_limit"].isValid()) {
        data["spending_limit"] = maps["spending_limit"].toJsonValue();
    }
    m_keyCoSigning = data;
    DBG_INFO << data;
    emit keyCoSigningChanged();
}

QString QServerKey::secondsToString(qint64 total_seconds)
{
    qint64 data = total_seconds;
    qint64 days = data / 86400;
    qint64 hours = data / 3600;
    qint64 minutes = (data % 3600) / 60;
    qint64 seconds = data % 60;

    QString result = "";
    if (days > 0) {
        result += QString("%1 day").arg(days);
        if (days > 1) {
            result.append("s");
        }
    }

    if (hours > 0) {
        if (!result.isEmpty()) {
            result.append(" ");
        }
        result += QString("%1 hour").arg(hours);
        if (hours > 1) {
            result.append("s");
        }
    }

    if (minutes > 0) {
        if (!result.isEmpty()) {
            result.append(" ");
        }
        result += QString("%1 minute").arg(minutes);
        if (minutes > 1) {
            result.append("s");
        }
    }

    if (seconds > 0) {
        if (!result.isEmpty()) {
            result.append(" ");
        }
        result += QString("%1 second").arg(seconds);
        if (seconds > 1) {
            result.append("s");
        }
    }
    if (total_seconds == 0) {
        result = QString("0 hour 0 minute").arg(minutes);
    }
    return result;
}

QVariant QServerKey::secondsToTime(qint64 signing_delay_seconds)
{
    qint64 data = signing_delay_seconds;
    qint64 days = data / 86400;
    qint64 hours = data / 3600;
    qint64 minutes = (data % 3600) / 60;
    qint64 seconds = data % 60;

    QJsonObject time;
    time["days"] = days;
    time["hours"] = hours;
    time["minutes"] = minutes;
    time["seconds"] = seconds;
    return QVariant::fromValue(time);
}

qint64 QServerKey::timeToSeconds(const QVariant &time)
{
    QMap<QString, QVariant> maps = time.toMap();
    return maps["hours"].toInt()*(60*60) + maps["minutes"].toInt()*60;
}

QJsonObject QServerKey::ConvertToDisplayQml(QJsonObject data)
{
    QJsonObject spending_limit = data["spending_limit"].toObject();
    if (spending_limit.isEmpty()) {
        spending_limit["interval"] = "DAILY";
        spending_limit["currency"] = "USD";
        spending_limit["limit"] = 0;
        data["spending_limit"] = spending_limit;
    }
    return data;
}
