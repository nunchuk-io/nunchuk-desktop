#pragma once

#include "BaseTransaction.h"

class MiniscriptTransaction : public BaseTransaction
{
    Q_OBJECT
    Q_PROPERTY(QVariantList scriptPaths         READ scriptPaths                                        NOTIFY scriptPathsChanged)
    Q_PROPERTY(QVariantList miniTreeForSigning  READ miniTreeForSigning                                 NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(bool         isScriptPath        READ isScriptPath                                       NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QVariant     timelockedUntil     READ timelockedUntil                                    NOTIFY nunchukTransactionChanged)
public:
    MiniscriptTransaction();
    MiniscriptTransaction(const nunchuk::Transaction &tx);
    ~MiniscriptTransaction() = default;

    bool isScriptPath() const;

    // Mini script
    qint64 get_lock_time() const;
    void set_lock_time(qint64 value);

    std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount>> signingPaths();
    void setSigningPaths(const std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount>> &data);
    int signingPathsCount() const;

    QVariantList scriptPaths();
    void setScriptPaths(const QVariantList &newScriptPaths);
    nunchuk::SigningPath signingPathSelected();
    void setSigningPathSelected(const nunchuk::SigningPath path, bool cached = false);
    Q_INVOKABLE void setSigningPathSelected(const QString &path);

    QVariantList miniTreeForSigning();
    QVariant timelockedUntil();
    void loadTimelockedUntil();

    void refreshScanDevices() override;
    void createScriptPaths();
private:
    QJsonArray createTreeMiniscriptTransaction(const nunchuk::ScriptNode &node,
                                               int64_t chain_tip,
                                               const std::vector<nunchuk::UnspentOutput> &coins,
                                               const nunchuk::CoinsGroup &coinsGroup,
                                               bool parentSatisfiable = true);

    QJsonObject createScriptPath(const QJsonArray &scriptTree, const std::pair<nunchuk::SigningPath, nunchuk::Amount> &signingPath);
    QJsonValue processNodeJson(const QJsonValue& value, const QStringList& nodes);
    QJsonValue processSignNodeJson(const QJsonValue& walletJs, const QJsonValue& txJs);
    QJsonObject processKeyNodeJson(const QJsonObject& value, QString xfp, int keyset_status, bool keyset_signed);
    QJsonObject generateCoinsGroup(const nunchuk::CoinsGroup &coinsGroup);
public slots:
    // Add any public slots specific to MiniscriptTransaction here
    int numberOfPolices();
signals:
    void scriptPathsChanged();
    void signingPathSelectionRequested();
    void timelockedUntilChanged();
private:
    std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount>> m_signing_paths;
    QVariantList            m_scriptPaths;
    nunchuk::SigningPath    m_selectedSigningPath {}; // Store the selected signing path
    QVariant                m_timelockedUntil;
};
