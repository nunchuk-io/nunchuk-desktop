#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <nunchuk.h>
#include "QOutlog.h"
#include "TypeDefine.h"
#include "Commons/QStateFlow.h"
#include <QTimer>

class QNormalSandbox : public QStateFlow
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
    Q_PROPERTY(bool isReplace                READ isReplace                         CONSTANT)
    Q_PROPERTY(bool isRecovery               READ isRecovery                        CONSTANT)
    Q_PROPERTY(bool isCreate                 READ isCreate                        CONSTANT)

public:
    QNormalSandbox();
    QNormalSandbox(nunchuk::GroupSandbox sandbox);
    ~QNormalSandbox();
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

    virtual bool enoughSigner();

    bool HasEnoughSigner();
    bool HasOneKeyRecovery();
    bool FinalizeGroup();

    bool isReplace() const;
    bool isRecovery() const;
    bool isCreate() const;

public:
    virtual void CreateAGroup(const QString& name, int m, int n, int addType);
    virtual void UpdateGroup(const QString& name, int m, int n, int addType);
    virtual void setSandbox(const nunchuk::GroupSandbox& sandbox);
    void setCurrentSandbox(const nunchuk::GroupSandbox& sandbox);
    bool JoinGroup(const QString& url);
    bool GetGroup(const QString &group_id);
    virtual bool AddSignerToGroup(const nunchuk::SingleSigner& signer);
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
    bool CreateReplaceGroup(const QString& wallet_id);
    bool AcceptReplaceGroup(const QString& wallet_id, const QString& sandbox_id);
    bool DeclineReplaceGroup(const QString& wallet_id, const QString& sandbox_id);
    void CloseCurrentSandboxSetup();
    QStringList qrTagsRecovery() const;
    void setQrTagsRecovery(const QStringList &newQrTagsRecovery);
    void registerSigners();
    virtual void clearOccupied();
public slots:
    bool checkWaitingOthers();
    void deleteGroup();
    void removeKey(int index);
    void requestAddOrRepaceKey(const QVariant& msg);
    void requestAddNewKey();
    void slotClearOccupied();
    virtual bool editBIP32Path(int index, const QString &master_id, const QString& path);
    void slotFinishSandboxWallet(const QString &wallet_id, bool reqClose);
signals:
    void groupSandboxChanged();
    void finishSandboxWallet(const QString &wallet_id, bool reqClose);
    void recoverKeyError();
    void editBIP32PathSuccess(int typeError);
    void needTopUpXpub();
protected:
    QVariantList            m_groupKeys {};
    nunchuk::GroupSandbox   m_sandbox {""};
    int                     m_index {0};
    int                     m_userCount {1};
    bool                    m_isCreateWallet {false};
    QString                 m_fingerprintRecovery {""};
    QTimer                  m_occupied;
    QString                 m_filePathRecovery {""};
    QStringList             m_qrTagsRecovery {};
};