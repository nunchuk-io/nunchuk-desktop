#ifndef GROUPSANDBOXMODEL_H
#define GROUPSANDBOXMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <nunchuk.h>
#include "QOutlog.h"
#include "TypeDefine.h"
#include "Commons/QStateFlow.h"
#include "Sandbox/QMiniscriptSandbox.h"
#include <QTimer>

class QGroupSandbox : public QMiniscriptSandbox
{
    Q_OBJECT
public:
    QGroupSandbox();
    QGroupSandbox(nunchuk::GroupSandbox sandbox);
    ~QGroupSandbox();
};

class GroupSandboxModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count                READ count              NOTIFY groupSandboxModelChanged)
public:
    GroupSandboxModel();
    ~GroupSandboxModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int,QByteArray> roleNames() const;
    int count() const;
    void GetGroups();
    QGroupSandboxPtr GetGroup(const QString& sandbox_id);

    void updateSandox(const nunchuk::GroupSandbox sandbox);
    bool contains(const QString& sandbox_id);
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
    };
signals:
    void groupSandboxModelChanged();
private:
    QList<QGroupSandboxPtr> m_data;
};

#endif // GROUPSANDBOXMODEL_H
