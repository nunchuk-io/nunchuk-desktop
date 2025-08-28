#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <nunchuk.h>
#include "QOutlog.h"
#include "TypeDefine.h"
#include "QNormalSandbox.h"
#include <QTimer>

class QMiniscriptSandbox : public QNormalSandbox
{
    Q_OBJECT
    Q_PROPERTY(int          walletType      READ walletType               NOTIFY groupSandboxChanged)
public:
    QMiniscriptSandbox();
    QMiniscriptSandbox(nunchuk::GroupSandbox sandbox);
    ~QMiniscriptSandbox();

    void setKeyName(const QString &keyName);
    int walletType();

    bool enoughSigner() override;

    void clearOccupied() override;
    void CreateAGroup(const QString& name, int m, int n, int addType) override;
    void UpdateGroup(const QString& name, int m, int n, int addType) override;
    void CreateAGroup(const QString& name, const QString& script_tmpl, int addType);
    void UpdateGroup(const QString &name, const QString& script_tmpl, int addType);
    void setSandbox(const nunchuk::GroupSandbox& sandbox) override;
    bool AddSignerToGroup(const nunchuk::SingleSigner& signer) override;
public slots:
    void removeKeyName(const QString &group, const QString &keyName);
    bool editBIP32Path(int index, const QString &master_id, const QString& path) override;
    bool editBIP32Path(const QString &keyName, const QString &master_id, const QString &path);
    void requestAddOrRepaceKey(const QVariant &msg);
    int getIndexColor(const QVariant & obj);
signals:
    
private:
    QString m_keyName {};
    mutable int m_walletType {-1};
};