#include "GroupSandboxModel.h"
#include "QEventProcessor.h"
#include "ViewsEnums.h"
#include "Models/QWarningMessage.h"
#include "ifaces/nunchuckiface.h"
#include "Models/AppModel.h"
#include "Chats/ClientController.h"
#include "Premiums/QSharedWallets.h"
#include "Signers/QSignerManagement.h"


QGroupSandbox::QGroupSandbox()
    : QMiniscriptSandbox()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QGroupSandbox::QGroupSandbox(nunchuk::GroupSandbox sandbox)
    : QMiniscriptSandbox(sandbox)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QGroupSandbox::~QGroupSandbox()
{

}

GroupSandboxModel::GroupSandboxModel()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

GroupSandboxModel::~GroupSandboxModel()
{
    m_data.clear();
}

int GroupSandboxModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.size();
}

QVariant GroupSandboxModel::data(const QModelIndex &index, int role) const
{
    if(auto data = m_data[index.row()]){
        switch (role) {
        case group_Id_Role:
            return data->groupId();
        case group_Name_Role:
            DBG_INFO << data->groupName();
            return data->groupName();
        case group_M_Role:
            return data->groupM();
        case group_N_Role:
            return data->groupN();
        case group_AddressType_Role:
            return data->addressType();
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QHash<int, QByteArray> GroupSandboxModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[group_Id_Role]   = "group_id";
    roles[group_Name_Role] = "group_name";
    roles[group_M_Role]    = "group_M";
    roles[group_N_Role]    = "group_N";
    roles[group_AddressType_Role]  = "group_AddressType";
    return roles;
}

int GroupSandboxModel::count() const
{
    return m_data.size();
}

void GroupSandboxModel::GetGroups()
{
    QPointer<GroupSandboxModel> safeThis(this);
    runInThread([]() ->std::vector<nunchuk::GroupSandbox>{
        QWarningMessage msg;
        return bridge::GetGroups(msg);
    },[safeThis](std::vector<nunchuk::GroupSandbox> sandboxs) {
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        ptrLamda->beginResetModel();
        ptrLamda.data()->m_data.clear();
        for(auto sandbox : sandboxs) {
            ptrLamda.data()->m_data.append(QGroupSandboxPtr(new QGroupSandbox(sandbox)));
        }
        ptrLamda->endResetModel();
        DBG_INFO << ptrLamda->count();
    });
}

QGroupSandboxPtr GroupSandboxModel::GetGroup(const QString &sandbox_id)
{
    for(auto sandbox: m_data) {
        if (qUtils::strCompare(sandbox_id, sandbox->groupId())) return sandbox;
    }
    return NULL;
}

void GroupSandboxModel::updateSandox(const nunchuk::GroupSandbox sandbox)
{
    for(int i = 0; i < m_data.count(); i++){
        if(m_data.at(i) && qUtils::strCompare(QString::fromStdString(m_data.at(i).data()->sandbox().get_id()), QString::fromStdString(sandbox.get_id()))){
            m_data.at(i).data()->setSandbox(sandbox);
            emit dataChanged(index(i),index(i));
        }
    }
}

bool GroupSandboxModel::contains(const QString &sandbox_id)
{
    for(auto it : m_data){
        if(it && qUtils::strCompare(it->groupId(), sandbox_id)){
            return true;
        }
    }
    return false;
}

void GroupSandboxModel::clearOccupied()
{
    for(auto it : m_data){
        if(it){
            it->clearOccupied();
        }
    }
}

void GroupSandboxModel::cleardata()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}
