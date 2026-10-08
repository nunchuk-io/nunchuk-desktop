#ifndef GROUPSANDBOXMODEL_H
#define GROUPSANDBOXMODEL_H

#include "Commons/QStateFlow.h"
#include "QOutlog.h"
#include "Sandbox/QMiniscriptSandbox.h"
#include "TypeDefine.h"
#include <QAbstractListModel>
#include <QObject>
#include <QTimer>
#include <nunchuk.h>

class QGroupSandbox : public QMiniscriptSandbox {
    Q_OBJECT
  public:
    QGroupSandbox();
    QGroupSandbox(nunchuk::GroupSandbox sandbox);
    ~QGroupSandbox();
};

class GroupSandboxModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY groupSandboxModelChanged)
  public:
    GroupSandboxModel();
    ~GroupSandboxModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    int count() const;
    void GetGroups();
    void GetSharedWalletInvitations();
    QGroupSandboxPtr GetGroup(const QString &sandbox_id);

    void updateSandox(const nunchuk::GroupSandbox sandbox);
    bool contains(const QString &sandbox_id);
    void clearOccupied();
    void cleardata();

  public:
  private:
    enum GroupRoles {
        group_Id_Role,
        group_Name_Role,
        group_M_Role,
        group_N_Role,
        group_AddressType_Role,
        group_isInviter_Role,
        group_InviterEmail_Role,
    };
  signals:
    void groupSandboxModelChanged();

  private:
    struct SharedWalletInvitation {
        QString id;
        QString group_id;
        QString inviter_name;
        QString inviter_email;
    };
    QList<QGroupSandboxPtr> m_data;
    std::vector<nunchuk::GroupSandbox> m_sandboxs;
};

#endif // GROUPSANDBOXMODEL_H
