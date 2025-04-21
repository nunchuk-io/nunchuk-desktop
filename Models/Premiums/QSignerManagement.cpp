#include "QSignerManagement.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"
#include "Models/MasterSignerModel.h"

QSignerManagement::QSignerManagement()
{

}

QSignerManagement::~QSignerManagement()
{

}

QSignerManagement *QSignerManagement::instance()
{
    static QSignerManagement mInstance;
    return &mInstance;
}

void QSignerManagement::registerCreateMasterSigner(Execute func)
{
    m_executeMaster = func;
}

bool QSignerManagement::finishCreateMasterSigner()
{
    if (m_executeMaster != nullptr) {
        if (m_executeMaster()) {
            if (auto s = AppModel::instance()->masterSignerInfoPtr()) {
                QString msg = QString("<b>%1</b> has been added").arg(s->name());
                AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
        m_executeMaster = nullptr;
        return true;
    }
    return false;
}

void QSignerManagement::registerCreateRemoteSigner(Execute func)
{
    m_executeRemote = func;
}

bool QSignerManagement::finishCreateRemoteSigner()
{
    if (m_executeRemote != nullptr) {
        if (m_executeRemote()) {
            if (auto s = AppModel::instance()->singleSignerInfoPtr()) {
                QString msg = QString("<b>%1</b> has been added").arg(s->name());
                AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
        m_executeRemote = nullptr;
        return true;
    }
    return false;
}

void QSignerManagement::registerCreateSoftwareSigner(Execute func)
{
    m_executeSoftware = func;
}

bool QSignerManagement::finishCreateSoftwareSigner()
{
    if (m_executeSoftware != nullptr) {
        if (m_executeSoftware()) {
            if (auto s = AppModel::instance()->masterSignerInfoPtr()) {
                QString msg = QString("<b>%1</b> has been added").arg(s->name());
                AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
        m_executeSoftware = nullptr;
        return true;
    }
    return false;
}

void QSignerManagement::registerCreateSoftwareSignerXprv(Execute func)
{
    m_executeSoftXprv = func;
}

bool QSignerManagement::finishCreateSoftwareSignerXprv()
{
    if (m_executeSoftXprv != nullptr) {
        if (m_executeSoftXprv()) {
            if (auto s = AppModel::instance()->masterSignerInfoPtr()) {
                QString msg = QString("<b>%1</b> has been added").arg(s->name());
                AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
            }
        }
        m_executeSoftXprv = nullptr;
        return true;
    }
    return false;
}

void QSignerManagement::clearExecute()
{
    if (m_executeMaster != nullptr) {
        m_executeMaster = nullptr;
    }
    if (m_executeRemote != nullptr) {
        m_executeRemote = nullptr;
    }
    if (m_executeSoftware != nullptr) {
        m_executeSoftware = nullptr;
    }
    if (m_executeSoftXprv != nullptr) {
        m_executeSoftXprv = nullptr;
    }
}

void QSignerManagement::createHotKey()
{
    auto listSS = AppModel::instance()->masterSignerListPtr()->fullList();
    QList<int> listId;
    for (auto signer: listSS) {
        if (signer->signerType() == (int)nunchuk::SignerType::SOFTWARE) {
            if (signer->name().startsWith("My key")) {
                if (qUtils::strCompare(signer->name(), "My key")) {
                    listId.append(0);
                } else {
                    QStringList listName = signer->name().split("My key");
                    QString lastName = listName.at(1);
                    lastName.replace("#", "");
                    bool isOK = false;
                    int id = lastName.toInt(&isOK);
                    DBG_INFO << listName << lastName << id << isOK;
                    if (isOK) {
                        listId.append(id);
                    }
                }
            }
        }
    }
    std::sort(listId.begin(), listId.end());
    QString nameKey = listId.isEmpty() ? "My key" : "My key #" + QString::fromStdString(std::to_string(listId.last() + 1));
    QString mnemonic = qUtils::GenerateMnemonic();
    QWarningMessage msg;
    DBG_INFO << nameKey << listId;
    auto ss = bridge::nunchukCreateSoftwareSigner(nameKey, mnemonic, "", false, false, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        ss->setNeedBackup(true);
        bridge::nunchukUpdateMasterSigner(ss);
        emit AppModel::instance()->startReloadMasterSigners();
        AppModel::instance()->setMasterSignerInfo(ss);
        QSignerManagement::instance()->finishCreateMasterSigner();
    }
}

