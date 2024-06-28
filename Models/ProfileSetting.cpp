#include "ProfileSetting.h"
#include "ViewsEnums.h"
#include "Servers/Draco.h"
#include "localization/STR_CPP.h"
#include "ServiceSetting.h"
#include "Premiums/QWalletServicesTag.h"
#include "Premiums/QGroupWalletDummyTx.h"
#include "Chats/ClientController.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QGroupWallets.h"

ProfileSetting::ProfileSetting(QObject *parent) : QObject(parent)
{
    m_currencies.clear();
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

ProfileSetting *ProfileSetting::instance() {
    static ProfileSetting mInstance;
    return &mInstance;
}

QVariantList ProfileSetting::currencies() const
{
    return m_currencies;
}

void ProfileSetting::createCurrencies()
{
    if (m_currencies.size() > 0){
        setCurrency(AppSetting::instance()->currency());
    }
    else {
        QtConcurrent::run([=]() {
            QJsonObject currencies;
            QString errormsg;
            bool ret = Draco::instance()->getCurrencies(currencies,errormsg);
            if (ret) {
                QMap<QString, QVariant> cs = currencies.toVariantMap();
                for (QMap<QString, QVariant>::const_iterator it = cs.constBegin(); it != cs.constEnd(); ++it) {
                    QMap<QString,QVariant> currency;
                    currency["currency"] = it.key();
                    currency["displayName"] = QString("%1 (%2)").arg(it.value().toString()).arg(it.key());
                    m_currencies.append(QVariant::fromValue(currency));
                }
                setCurrency(AppSetting::instance()->currency());
                emit currenciesChanged();
            }
            else {
                QMap<QString,QVariant> currency;
                currency["currency"] = "USD";
                currency["displayName"] = "United States Dollar (USD)";
                m_currencies.append(QVariant::fromValue(currency));
                setCurrency("USD");
                emit currenciesChanged();
            }
        });
    }
}

int ProfileSetting::findCurrency(const QString &currency)
{
    int i = 0;
    for (auto var : m_currencies) {
        QMap<QString,QVariant> maps = var.toMap();
        if (maps["currency"].toString() == currency) {
            return i;
        }
        i++;
    }
    return i;
}

int ProfileSetting::currencyIndex() const
{
    return m_currencyIndex;
}

void ProfileSetting::setCurrency(const QString &currency)
{
    AppSetting::instance()->setCurrency(currency);
    m_currencyIndex = findCurrency(currency);
    emit currencyIndexChanged();
    QtConcurrent::run([=]() {
        Draco::instance()->exchangeRates(currency);
    });
}

int ProfileSetting::optionIndex() const
{
    return m_optionIndex;
}

void ProfileSetting::setOptionIndex(int optionIndex)
{
    if (m_optionIndex == optionIndex)
        return;

    m_optionIndex = optionIndex;
    emit optionIndexChanged();
}

void ProfileSetting::GetMainNetServer()
{
    QtConcurrent::run([=]() {
        QJsonObject output;
        QString errormsg;
        bool ret = Draco::instance()->GetElectrumServers(output, errormsg);
        DBG_INFO << errormsg;
        if (ret) {
            m_mainnetList = output["mainnet"].toArray();
            loadMainnetServers();
        }
    });
}


QVariantList ProfileSetting::mainnetServers() const
{
    return m_mainnetServers.toVariantList();
}

void ProfileSetting::updateMainnetServers()
{
    m_mainnetServers = m_mainnetList;
    if (m_storeMainnetServers.size() > 0) {
        for (auto js : m_storeMainnetServers) {
            m_mainnetServers.append(js);
        }
    }
    QString urlSelected = AppSetting::instance()->mainnetServer();
    for (int i = 0; i < m_mainnetServers.size(); i++) {
        QJsonObject item = m_mainnetServers[i].toObject();
        if (item["url"].toString() == urlSelected) {
            setMainnetIndex(i);
        }
    }
    emit mainnetServersChanged();
}

void ProfileSetting::resetDefaultMainnetServers()
{
    QString urlSelected = AppSetting::instance()->mainnetServer();
    bool isSetIndex {false};
    for (int i = 0; i < m_mainnetServers.size(); i++) {
        QJsonObject item = m_mainnetServers[i].toObject();
        if (item["url"].toString() == urlSelected) {
            setMainnetIndex(i);
            isSetIndex = true;
        }
    }
    DBG_INFO << isSetIndex;
    if (!isSetIndex) {
        setMainnetIndex(0);
        QJsonObject item = m_mainnetServers[0].toObject();
        emit AppSetting::instance()->mainnetServerSelected(item["url"].toString());
    }
}

int ProfileSetting::mainnetIndex() const
{
    return m_mainnetIndex;
}

void ProfileSetting::setMainnetIndex(int mainnetIndex)
{
    if (m_mainnetIndex == mainnetIndex)
        return;

    m_mainnetIndex = mainnetIndex;
    emit mainnetIndexChanged();
}

bool ProfileSetting::CalculateRequireSignaturesForChangingEmail()
{
    auto tag = ServiceSetting::instance()->servicesTagPtr();
    QJsonObject output;
    QString errormsg;
    bool ret = Draco::instance()->CalculateRequireSignaturesForChangingEmail(newEmail(), output, errormsg);
    if (ret) {
        DBG_INFO << output;
        QJsonObject resultObj = output["result"].toObject();
        tag->setReqiredSignatures(resultObj);
        ReqiredSignaturesInfo required_question = tag->reqiredSignaturesInfo();
        if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX) {
            if (ProfileSetting::instance()->ChangeEmail()) {
                if(auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
                    dashboard->setFlow((int)AlertEnum::E_Alert_t::CHANGE_EMAIL);
                    QEventProcessor::instance()->sendEvent(E::EVT_HEALTH_CHECK_STARTING_REQUEST);
                }
            }
            return true;
        }
        else if (required_question.type == (int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION) {
            tag->CreateSecurityQuestionsAnswered();
            return true;
        }
        else {
            ProfileSetting::instance()->ChangeEmailNone();
        }
    }
    return false;
}

bool ProfileSetting::ChangeEmail()
{
    auto tag = ServiceSetting::instance()->servicesTagPtr();
    QJsonObject output;
    QString errormsg;
    QJsonObject data = tag->confirmCodeNonceBody();
    DBG_INFO << tag->passwordToken() << tag->confirmToken();
    bool ret = Draco::instance()->ChangeEmail(data,
                                              {},
                                              tag->passwordToken(),
                                              {},
                                              tag->confirmToken(),
                                              true,
                                              output,
                                              errormsg);
    if (ret) {
        DBG_INFO << output;
        bool success = output["success"].toBool();
        if (!success) {
            QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
            QString wallet_local_id = dummy_transaction["wallet_local_id"].toString();
            if (auto wallet = AppModel::instance()->walletListPtr()->getWalletById(wallet_local_id)) {
                QGroupWallets::instance()->setDashboardInfo(wallet);
                if (auto dummy = wallet->groupDummyTxPtr()) {
                    dummy->setDummyTxData(dummy_transaction);
                }
            }
            return true;
        }
    }
    return false;
}

bool ProfileSetting::ChangeEmailNone()
{
    auto tag = ServiceSetting::instance()->servicesTagPtr();
    QJsonObject output;
    QString errormsg;
    QJsonObject data = tag->confirmCodeNonceBody();
    DBG_INFO << tag->passwordToken() << tag->confirmToken();
    bool ret = Draco::instance()->ChangeEmail(data,
                                              {},
                                              tag->passwordToken(),
                                              {},
                                              tag->confirmToken(),
                                              false,
                                              output,
                                              errormsg);
    if (ret) {
        DBG_INFO << output;
        bool success = output["success"].toBool();
        if (success) {
            QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
            ClientController::instance()->requestSignout();
            QString msg_name = QString("Email has been changed. Please sign in again.");
            AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
            return true;
        }
    }
    return false;
}

bool ProfileSetting::seccurityQuestion()
{
    auto tag = ServiceSetting::instance()->servicesTagPtr();
    if (tag->secQuesAnswer()) {
        QJsonObject output;
        QString errormsg;
        QJsonObject data = tag->confirmCodeNonceBody();
        DBG_INFO << tag->passwordToken()
                 << tag->secQuesToken()
                 << tag->confirmToken();
        bool ret = Draco::instance()->ChangeEmail(data,
                                                  {},
                                                  tag->passwordToken(),
                                                  tag->secQuesToken(),
                                                  tag->confirmToken(),
                                                  false,
                                                  output,
                                                  errormsg);
        if (ret) {
            DBG_INFO << output;
            bool success = output["success"].toBool();
            if (success) {
                QEventProcessor::instance()->sendEvent(E::EVT_ONS_CLOSE_ALL_REQUEST);
                ClientController::instance()->requestSignout();
                QString msg_name = QString("Email has been changed. Please sign in again.");
                AppModel::instance()->showToast(0, msg_name, EWARNING::WarningType::SUCCESS_MSG);
                return true;
            }
        }
    }
    return false;
}


bool ProfileSetting::addMainnetServer(const QVariant &server)
{
    QVariantMap map = server.toMap();
    DBG_INFO << map;
    QString name = map["name"].toString();
    QString url = map["url"].toString();
    for (int i = 0; i < m_storeMainnetServers.size(); i++) {
        QJsonObject item = m_storeMainnetServers[i].toObject();
        if (item["name"].toString() == name && item["url"].toString() == url) {
            return false;
        }
    }
    m_storeMainnetServers.append(server.toJsonObject());
    emit AppSetting::instance()->mainnetServerSelected(url);
    updateMainnetServers();
    saveMainnetServers();
    QString msg = QString("Server added");
    AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
    return true;
}

void ProfileSetting::removeMainnetServer(const QVariant &server)
{
    QVariantMap map = server.toMap();
    QString name = map["name"].toString();
    QString url = map["url"].toString();
    for (int i = 0; i < m_storeMainnetServers.size(); i++) {
        QJsonObject item = m_storeMainnetServers[i].toObject();
        if (item["name"].toString() == name && item["url"].toString() == url) {
            m_storeMainnetServers.removeAt(i);
        }
    }
    updateMainnetServers();
}

void ProfileSetting::saveMainnetServers()
{
    AppSetting::instance()->setMainnetList(m_storeMainnetServers.toVariantList());
}

void ProfileSetting::loadMainnetServers()
{
    QVariantList list = AppSetting::instance()->getMainnetList().toList();
    m_storeMainnetServers = QJsonArray::fromVariantList(list);

    updateMainnetServers();
}

QString ProfileSetting::newEmail() const
{
    return m_newEmail;
}

void ProfileSetting::setNewEmail(const QString &newNew_email)
{
    if (m_newEmail == newNew_email)
        return;
    m_newEmail = newNew_email;
    emit newEmailChanged();
}
