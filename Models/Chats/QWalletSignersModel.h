#ifndef QWALLETSIGNERSMODEL_H
#define QWALLETSIGNERSMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include "qUtils.h"

struct SignerAssigned {
    bool is_localuser = false;
    QString name = "";
    QString xfp = "";
    QString xpub = "";
    QString bip32path = "";
    QString joid_id = "";
    int     type = 0;
    bool    isPrimaryKey = false;
};

class QWalletSignersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    QWalletSignersModel();
    ~QWalletSignersModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    int signersAssignedCount();
    void addSigner(SignerAssigned signer);
    QString getJoinEventId(const QString& xfp);
    QStringList xfplist();
    bool constains(const QString& xfp);
    SignerAssigned getSignerByIndex(const int index);
    QList<SignerAssigned> fullList() const;
    nunchuk::PrimaryKey containPrimaryKey(const QString& fingerprint);
    enum ConversationRoles {
        role_name,
        role_xfp,
        role_xpub,
        role_bip32,
        role_joid_id,
        role_is_localuser,
        role_signer_type,
        role_signer_primary_key
    };
private:
    QList<SignerAssigned> m_data;
    std::vector<nunchuk::PrimaryKey> primaryKeys;
signals:
    void countChanged();
};
typedef QSharedPointer<QWalletSignersModel> QWalletSignersModelPtr;

bool sortWalletSigners(const SignerAssigned &v1, const SignerAssigned &v2);

#endif // QWALLETSIGNERSMODEL_H
