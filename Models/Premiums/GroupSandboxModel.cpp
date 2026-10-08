#include "GroupSandboxModel.h"
#include "Chats/ClientController.h"
#include "Models/AppModel.h"
#include "Models/QWarningMessage.h"
#include "Premiums/QSharedWallets.h"
#include "QEventProcessor.h"
#include "Signers/QSignerManagement.h"
#include "ViewsEnums.h"
#include "core/restapi/RestApi.h"
#include "ifaces/nunchuckiface.h"

QGroupSandbox::QGroupSandbox() : QMiniscriptSandbox() {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QGroupSandbox::QGroupSandbox(nunchuk::GroupSandbox sandbox) : QMiniscriptSandbox(sandbox) {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QGroupSandbox::~QGroupSandbox() {}

GroupSandboxModel::GroupSandboxModel() {
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

GroupSandboxModel::~GroupSandboxModel() {
    m_data.clear();
}

int GroupSandboxModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.size();
}

QVariant GroupSandboxModel::data(const QModelIndex &index, int role) const {
    if (auto data = m_data[index.row()]) {
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
        case group_isInviter_Role:
            return data->isInviter();
        case group_InviterEmail_Role:
            return data->groupEmail();
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QHash<int, QByteArray> GroupSandboxModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[group_Id_Role] = "group_id";
    roles[group_Name_Role] = "group_name";
    roles[group_M_Role] = "group_M";
    roles[group_N_Role] = "group_N";
    roles[group_AddressType_Role] = "group_AddressType";
    roles[group_isInviter_Role] = "group_isInviter";
    roles[group_InviterEmail_Role] = "group_InviterEmail";
    return roles;
}

int GroupSandboxModel::count() const {
    return m_data.size();
}

void GroupSandboxModel::GetGroups() {
    QPointer<GroupSandboxModel> safeThis(this);
    runInThread(
        this,
        []() -> std::vector<nunchuk::GroupSandbox> {
            QWarningMessage msg;
            return bridge::GetGroups(msg);
        },
        [safeThis](std::vector<nunchuk::GroupSandbox> sandboxs) {
            if (!safeThis)
                return;
            safeThis->m_sandboxs = std::move(sandboxs);
            safeThis->GetSharedWalletInvitations();
        });
}

void GroupSandboxModel::GetSharedWalletInvitations() {
    QPointer<GroupSandboxModel> safeThis(this);
    DBG_INFO << "Getting shared wallet invitations";
    runInThread(
        this,
        []() -> std::vector<SharedWalletInvitation> {
            QJsonObject output;
            QString errorMsg;
            bool ret = Shared::instance()->GetSharedWalletInvitations(output, errorMsg);
            if (!ret) {
                DBG_ERROR << "Failed to get shared wallet invitations:" << errorMsg;
                return {};
            }
            std::vector<SharedWalletInvitation> sandboxs;
            for (const auto &invitation : output.value("invitations").toArray()) {
                QJsonObject invitationObj = invitation.toObject();
                DBG_INFO << "Invitation:" << invitationObj;
                QString inviter_email = invitationObj.value("inviter_email").toString();
                QString current_user_email = ClientController::instance()->getMe().email;
                if (qUtils::strCompare(inviter_email, current_user_email)) {
                    continue; // Skip invitations sent by the current user
                }
                QString groupId = invitationObj.value("group_id").toString();
                QString inviter_name = invitationObj.value("inviter_name").toString();
                QString invitation_id = invitationObj.value("id").toString();
                SharedWalletInvitation sandbox;
                sandbox.group_id = groupId;
                sandbox.inviter_name = inviter_name;
                sandbox.inviter_email = inviter_email;
                sandbox.id = invitation_id;
                sandboxs.push_back(sandbox);
            }
            return sandboxs;
        },
        [safeThis](std::vector<SharedWalletInvitation> sandboxs) {
            if (!safeThis)
                return;

            safeThis->beginResetModel();
            safeThis->m_data.clear();
            safeThis->m_data.reserve(static_cast<int>(sandboxs.size() + safeThis->m_sandboxs.size()));
            for (const auto &sandbox : safeThis->m_sandboxs) {
                safeThis->m_data.append(QGroupSandboxPtr(new QGroupSandbox(sandbox)));
            }
            for (const auto &sandbox : sandboxs) {
                nunchuk::GroupSandbox groupSandbox(sandbox.group_id.toStdString());
                groupSandbox.set_name(sandbox.inviter_name.toStdString());
                auto newSandbox = QGroupSandboxPtr(new QGroupSandbox(groupSandbox));
                newSandbox->setgroupEmail(sandbox.inviter_email);
                newSandbox->setisInviter(true);
                newSandbox->setinviterId(sandbox.id);
                safeThis->m_data.append(newSandbox);
            }
            safeThis->endResetModel();
        });
}

QGroupSandboxPtr GroupSandboxModel::GetGroup(const QString &sandbox_id) {
    for (auto sandbox : m_data) {
        if (qUtils::strCompare(sandbox_id, sandbox->groupId()))
            return sandbox;
    }
    return NULL;
}

void GroupSandboxModel::updateSandox(const nunchuk::GroupSandbox sandbox) {
    for (int i = 0; i < m_data.count(); i++) {
        if (m_data.at(i) && qUtils::strCompare(QString::fromStdString(m_data.at(i).data()->sandbox().get_id()), QString::fromStdString(sandbox.get_id()))) {
            m_data.at(i).data()->setSandbox(sandbox);
            emit dataChanged(index(i), index(i));
        }
    }
}

bool GroupSandboxModel::contains(const QString &sandbox_id) {
    for (auto it : m_data) {
        if (it && qUtils::strCompare(it->groupId(), sandbox_id)) {
            return true;
        }
    }
    return false;
}

void GroupSandboxModel::clearOccupied() {
    for (auto it : m_data) {
        if (it) {
            it->clearOccupied();
        }
    }
}

void GroupSandboxModel::cleardata() {
    beginResetModel();
    m_data.clear();
    endResetModel();
}
