#ifndef PRIMARYKEYACCOUNT_H
#define PRIMARYKEYACCOUNT_H

#include "HotKey.h"
#include <QMap>

class PrimaryKeyAccount : public HotKey {
    Q_OBJECT

    Q_PROPERTY(QVariant primaryKeyData READ primaryKeyData NOTIFY primaryKeyDataChanged)

public:
    PrimaryKeyAccount() {};
    ~PrimaryKeyAccount() {};

    QVariant primaryKeyData() const;
    bool replacePrimaryKey(const QString &mnemonic, const QString &passphrase);
    void requestCreateSoftwareSignerPrimaryKey(const QString &signername,
                                          const QString &mnemonic,
                                          const QString &passphrase,
                                          bool replace = false);
    void requestCreateSoftwareSignerXprvPrimaryKey(const QString &signername,
                                              const QString &xprv,
                                              bool replace = false);

    void loginPrimaryKeySeed();
    void loginPrimaryKeyXprv();
    void loginPrimaryKey();

    void loginPrimaryKeySeedSuccess();
    void loginPrimaryKeyXprvSuccess();
    void loginPrimaryKeySuccess();

public slots:
    void requestCreateSignerFromCreatePrimaryKeyScreen();
    void requestSignInFromImportPrimaryKey();
    void requestSignInByManuallyPrimaryKey();
    void requestReplacePrimaryKey();

    void updatePrimaryKeyData(const QString &key, const QVariant &value);
    void updatePrimaryKeyData(const QMap<QString, QVariant>& dataMap);
    void clearPrimaryKeyData();
signals:
    void primaryKeyDataChanged();

private:
    QMap<QString, QVariant> m_primaryKeyData;
};

#endif // PRIMARYKEYACCOUNT_H