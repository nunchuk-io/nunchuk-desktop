#include "QSignerManagement.h"
#include "Models/AppModel.h"

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

