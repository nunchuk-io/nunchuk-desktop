#pragma once

#include "BaseTransaction.h"

class MiniscriptTransaction : public BaseTransaction
{
    Q_OBJECT
    Q_PROPERTY(QVariantList scriptPaths         READ scriptPaths                                        NOTIFY scriptPathsChanged)
    Q_PROPERTY(QVariantList miniTreeForSigning  READ miniTreeForSigning                                 NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(bool         isScriptPath        READ isScriptPath                                       NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QVariant     timelockedUntil     READ timelockedUntil                                    NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(bool         hasSigningPathChecked       READ hasSigningPathChecked                      NOTIFY hasSigningPathCheckedChanged)
    Q_PROPERTY(QVariantList     treeMiniscript          READ treeMiniscript                             NOTIFY treeMiniscriptChanged)
public:
    MiniscriptTransaction();
    MiniscriptTransaction(const nunchuk::Transaction &tx);
    ~MiniscriptTransaction() = default;

    bool isScriptPath() const;

    QJsonArray walletTreeMiniscriptJs() const;
    QVariantList treeMiniscript() const;
    void setTreeMiniscript(const QJsonArray &tree);

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
    QVariantList miniTreeForSigningScriptPath();
    QVariantList miniTreeForSigningKeyPath();
    QVariant timelockedUntil();
    void loadTimelockedUntil();

    void refreshScanDevices() override;
    void createScriptPaths(const std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount>> &signing_paths);

public: // For make checkboxes
    int getLevel(const QString &path);
    int getMaxLevel(const QJsonArray &tree);
    QString getParentPath(const QString &path);
    QStringList findPaths(const QJsonArray &tree, int level);
    QStringList findPathsOfParentChecked(const QSet<QString> &checkedList, const QString &parentPath);
    QJsonObject findSigningPathParent(const QJsonArray &tree, const QString &path);
    QJsonArray refreshTreeChecked(const QJsonArray &tree, const QSet<QString> &checkeds);
    QJsonArray processCheckedBoxes(const QJsonArray &tree, const QJsonArray &newTreeTmp, int level, const QString& path, QSet<QString>& checkeds, QSet<QString>& allowAutos);
    int numberFromPath(const QString &path);
    QJsonArray processLevels(const QJsonArray &tree, QSet<QString>& checkeds, QSet<QString>& allowAutos);
    bool compareNodes(const QJsonValue &a, const QJsonValue &b);
    QJsonArray sortJsonArrayByFirstLine(const QJsonArray &arr);
    void firstCreateChooseScriptPath();
    void createChooseScriptPath();
    bool hasSigningPathChecked() const;
private:
    QJsonArray createTreeMiniscriptTransaction(const nunchuk::ScriptNode &node,
                                               int64_t chain_tip,
                                               const std::vector<nunchuk::UnspentOutput> &coins,
                                               const nunchuk::CoinsGroup &coinsGroup,
                                               bool parentSatisfiable = true);
    QJsonArray createTreeMiniscriptTransaction(const std::vector<std::string> &keypaths);
    QJsonObject createScriptPath(const QJsonArray &scriptTree, const std::pair<nunchuk::SigningPath, nunchuk::Amount> &signingPath);
    QJsonValue processNodeJson(const QJsonValue& value, const QStringList& nodes);
    QJsonValue processSignNodeJson(const QJsonValue& walletJs, const QJsonValue& txJs);
    QJsonObject processKeyNodeJson(const QJsonObject& value, QString xfp, int keyset_status, bool keyset_signed);
    QJsonObject generateCoinsGroup(const nunchuk::CoinsGroup &coinsGroup);
public slots:
    // Add any public slots specific to MiniscriptTransaction here
    void clearSigningPaths();
    int numberOfPolices();
    bool enterPreimageInput(const QString &hasData, const QString &preimage, int typeNode);
    void setCheckedStateForPath(const QString &path, bool checked);
    bool createTransactionOrSingingPolicy();
signals:
    void scriptPathsChanged();
    void signingPathSelectionRequested();
    void timelockedUntilChanged();
    void preimageInputAlert();
    void hasSigningPathCheckedChanged();
    void treeMiniscriptChanged();
private:
    std::vector<std::pair<nunchuk::SigningPath, nunchuk::Amount>> m_signing_paths;
    QVariantList            m_scriptPaths;
    nunchuk::SigningPath    m_selectedSigningPath {}; // Store the selected signing path
    QVariant                m_timelockedUntil;
    QSet<QString>          m_signingPathsChecked {};
    QSet<QString>          m_signingPathsAutoChecked {};
    int                    m_maxLevel = 0; 
    QJsonArray             m_treeMiniscript {};
};
