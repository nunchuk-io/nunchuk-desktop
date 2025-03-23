#ifndef GROUPSANDBOXMODEL_H
#define GROUPSANDBOXMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <nunchuk.h>
#include "QOutlog.h"
#include "TypeDefine.h"
#include "Commons/QStateFlow.h"
#include <QTimer>

class QGroupSandbox : public QStateFlow
{
    Q_OBJECT
    Q_PROPERTY(QString      groupId          READ groupId                                              NOTIFY groupSandboxChanged)
    Q_PROPERTY(int          groupM           READ groupM                    WRITE setGroupM            NOTIFY groupSandboxChanged)
    Q_PROPERTY(int          groupN           READ groupN                    WRITE setGroupN            NOTIFY groupSandboxChanged)
    Q_PROPERTY(QString      groupName        READ groupName                 WRITE setGroupName         NOTIFY groupSandboxChanged)
    Q_PROPERTY(int          addressType      READ addressType               WRITE setAddressType       NOTIFY groupSandboxChanged)
    Q_PROPERTY(QVariantList groupKeys        READ groupKeys                                            NOTIFY groupSandboxChanged)
    Q_PROPERTY(QString      url              READ url                                                  NOTIFY groupSandboxChanged)
    Q_PROPERTY(int          userCount        READ userCount                                            NOTIFY groupSandboxChanged)
    Q_PROPERTY(bool enoughSigner             READ enoughSigner                                         NOTIFY groupSandboxChanged)

public:
    QGroupSandbox();
    QGroupSandbox(nunchuk::GroupSandbox sandbox);
    ~QGroupSandbox();
    void initialize();

    QString groupId() const;
    int groupM() const;
    void setGroupM(int m);
    int groupN() const;
    void setGroupN(int n);
    QString groupName() const;
    void setGroupName(const QString& name);
    int addressType() const;
    void setAddressType(int addressType);
    QString url() const;
    QVariantList groupKeys() const;
    int userCount() const;


    void setUserCount(int number);

    bool enoughSigner();

    bool HasEnoughSigner();
    bool HasOneKeyRecovery();
    bool FinalizeGroup();

public:
    void CreateAGroup(const QString& name, int m, int n, int addType);
    void UpdateGroup(const QString& name, int m, int n, int addType);
    void setSandbox(const nunchuk::GroupSandbox& sandbox);
    bool JoinGroup(const QString& url);
    bool GetGroup(const QString &group_id);
    bool AddSignerToGroup(const nunchuk::SingleSigner& signer);
    bool AddMasterToGroup();
    bool AddRemoteToGroup();
    void setIndex(int idx);
    nunchuk::GroupSandbox sandbox() const;
    void DeleteGroup(const QString& sandbox_id);
    void GetNumberForAGroup();
    void BackToSetupGroupWallet();
    void setFingerprintRecovery(const QString &newFingerprintRecovery);

    QString filePathRecovery() const;
    void setFilePathRecovery(const QString &newFilePathRecovery);

    void CreateSignerListReviewWallet();
    QSet<size_t> ValueKeyset();
public slots:
    bool checkWaitingOthers();
    void deleteGroup();
    void removeKey(int index);
    void requestAddOrRepaceKey(const QVariant& msg);
    void slotClearOccupied();
    void clearOccupied();
    bool editBIP32Path(int index, const QString &master_id, const QString& path);
    void slotFinishSandboxWallet();
signals:
    void groupSandboxChanged();
    void finishSandboxWallet();
    void recoverKeyError();
    void editBIP32PathSuccess(int typeError);
private:
    QVariantList            m_groupKeys {};
    nunchuk::GroupSandbox   m_sandbox {""};
    int                     m_index {0};
    int                     m_userCount {1};
    bool                    m_isCreateWallet {false};
    QString                 m_fingerprintRecovery {""};
    QTimer                  m_occupied;
    QString                 m_filePathRecovery {""};
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
