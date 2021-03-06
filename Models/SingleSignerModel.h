#ifndef SIGNERLISTMODEL_H
#define SIGNERLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "QWarningMessage.h"
#include "QOutlog.h"

class SingleSigner : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString signerName               READ name               WRITE setName               NOTIFY nameChanged)
    Q_PROPERTY(QString signerXpub               READ xpub               WRITE setXpub               NOTIFY xpubChanged)
    Q_PROPERTY(QString signerPublickey          READ publickey          WRITE setPublickey          NOTIFY publickeyChanged)
    Q_PROPERTY(QString signerDerivationPath     READ derivationPath     WRITE setDerivationPath     NOTIFY derivationPathChanged)
    Q_PROPERTY(QString signerMasterFingerPrint  READ masterFingerPrint  WRITE setMasterFingerPrint  NOTIFY masterFingerPrintChanged)
    Q_PROPERTY(QString signerMasterSignerId     READ masterSignerId                                 NOTIFY masterSignerIdChanged)
    Q_PROPERTY(QString signerLastHealthCheck    READ lastHealthCheck                                NOTIFY lastHealthCheckChanged)
    Q_PROPERTY(QString signerSigned             READ signerSigned                                   NOTIFY signerSignedChanged)
    Q_PROPERTY(bool signerIsRemote              READ isRemote                                       NOTIFY isRemoteChanged)
    Q_PROPERTY(bool signerIsValid               READ isValid                                        NOTIFY isValidChanged)
    Q_PROPERTY(QString signerMessage            READ message            WRITE setMessage            NOTIFY messageChanged)
    Q_PROPERTY(QString signerSignature          READ signature          WRITE setSignature          NOTIFY signatureChanged)
    Q_PROPERTY(int     signerHealth             READ health                                         NOTIFY healthChanged)
    Q_PROPERTY(bool    isColdCard               READ isColdCard                                     NOTIFY isColdCardChanged)
    Q_PROPERTY(QWarningMessage* warningMessage  READ warningMessage NOTIFY warningMessageChanged)

    // For add signer to wallet
    Q_PROPERTY(bool isBeingUsed                 READ isBeingUsed    NOTIFY checkedChanged)
public:
    SingleSigner();
    SingleSigner(const QString& pr_name,
                 const QString& pr_xpub,
                 const QString& pr_public_key,
                 const QString& pr_derivation_path,
                 const QString& pr_master_fingerprint,
                 const QString& pr_master_signer_id = "-1",
                 const QDateTime& pr_last_health_check = QDateTime(QDate(2020, 25, 8), QTime(20, 20, 20)));

    ~SingleSigner();
    QString name() const;
    QString xpub() const;
    QString publickey() const;
    QString derivationPath() const;
    QString masterFingerPrint() const;
    QString masterSignerId() const;
    QString lastHealthCheck();
    QDateTime lastHealthCheckDateTime() const;
    bool signerSigned() const;

    void setName(const QString& d);
    void setXpub(const QString& d);
    void setPublickey(const QString& d);
    void setDerivationPath(const QString& d);
    void setMasterFingerPrint(const QString& d);
    void setMasterSignerId(const QString& d);
    void setlastHealthCheck(const QDateTime& d);
    void setSignerSigned(const bool d);
    bool isRemote() const;
    void setIsRemote(bool isRemote);
    bool isValid() const;
    void setIsValid(bool isValid);
    QString message() const;
    void setMessage(const QString &message);
    QString signature() const;
    void setSignature(const QString &signature);
    int health() const;
    void setHealth(int health);

    bool isColdCard() const;
    void setIsColdCard(const bool isColdCard);

    QWarningMessage* warningMessage() const;
    QSharedPointer<QWarningMessage> warningMessagePtr() const;
    void setWarningMessage(const QSharedPointer<QWarningMessage> &warningMessage);

    // For add signer to wallet
    bool checked() const;
    void setChecked(const bool checked);

    bool isBeingUsed() const;
    void setIsBeingUsed(bool isBeingUsed);

private:
    QString name_;
    QString xpub_;
    QString public_key_;
    QString derivation_path_;
    QString master_fingerprint_;
    QString master_signer_id_;
    QDateTime last_health_check_;
    bool isBeingUsed_;
    bool signed_;           // Using for transaction only
    bool isRemote_;
    bool isValid_;          // Using for create wallet

    QString message_;
    QString signature_;
    int health_;

    bool isColdCard_;
    QSharedPointer<QWarningMessage> warningMessage_;

    // For add signer to wallet
    bool checked_;
private:
    QString timeGapCalculation(QDateTime in);
    QString timeGapCalculationShort(QDateTime in);
signals:
    void nameChanged();
    void xpubChanged();
    void publickeyChanged();
    void derivationPathChanged();
    void masterFingerPrintChanged();
    void masterSignerIdChanged();
    void lastHealthCheckChanged();
    void signerSignedChanged();
    void isRemoteChanged();
    void isValidChanged();
    void messageChanged();
    void signatureChanged();
    void healthChanged();
    void isColdCardChanged();
    void warningMessageChanged();

    // For add signer to wallet
    void checkedChanged();
    void isBeingUsedChanged();
};

bool sortSingleSignerByNameAscending(const QSharedPointer<SingleSigner> &v1, const QSharedPointer<SingleSigner> &v2);
bool sortSingleSignerByNameDescending(const QSharedPointer<SingleSigner> &v1, const QSharedPointer<SingleSigner> &v2);

class SingleSignerListModel  : public QAbstractListModel
{
    Q_OBJECT
public:
    SingleSignerListModel();
    ~SingleSignerListModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QHash<int,QByteArray> roleNames() const;
    void addSingleSigner(const QString& pr_name = "",
                         const QString& pr_xpub = "",
                         const QString& pr_public_key = "",
                         const QString& pr_derivation_path = "",
                         const QString& pr_master_fingerprint = "",
                         const QString& pr_master_signer_id = "",
                         const QDateTime& pr_last_health_check = QDateTime(QDate(2020, 25, 8), QTime(20, 20, 20)));
    void replaceSingleSigner(int index, const QSharedPointer<SingleSigner> &value);
    void addSingleSigner(const QSharedPointer<SingleSigner> &d);
    void updateSingleSignerList(const QSharedPointer<SingleSignerListModel> &d);
    void updateSignatures(const QString& masterfingerprint, const bool value);
    bool needTopUpXpubs() const;
    void initIsColdCard();
    void setIsColdCard(const QString &id);
    void initSignatures();
    QSharedPointer<SingleSigner> getSingleSignerByIndex(const int index);
    bool containsMasterSignerId(const QString& masterSignerId);
    bool removeSingleSignerByIndex(const int index);
    bool removeSingleSignerByFingerprint(const QString &fp);
    QString getMasterSignerIdByIndex(const int index);
    QString getMasterSignerXfpByIndex(const int index);
    QSharedPointer<SingleSigner> getSingleSignerByFingerPrint(const QString &fp);
    int getnumberSigned();
    bool contains(const QString& masterFingerPrint);
    bool containsSigner(const QString &xfp, const QString &path);
    bool checkUsableToSign(const QString& masterFingerPrint);
    void updateSignerHealthStatus(const QString &masterSignerId, const int status, const time_t time);
    void notifyMasterSignerDeleted(const QString &masterSignerId);
    void notifyMasterSignerRenamed(const QString &masterSignerId, const QString &newname);
    void notifyRemoteSignerRenamed(const QString &fingerprint, const QString &newname);
    void resetUserChecked();
    void setUserCheckedByFingerprint(const bool state, const QString fp);
    void setUserChecked(const bool state, const int index);
    void updateHealthCheckTime();
    bool containsDevicesConnected(const QStringList xfp);
    enum SingleSignerRoles {
        single_signer_name_Role,
        single_signer_xpub_Role,
        single_signer_public_key_Role,
        single_signer_derivation_path_Role,
        single_signer_master_fingerprint_Role,
        single_signer_master_signer_id_Role,
        single_signer_last_health_check_Role,
        single_signer_isRemote_Role,
        single_signer_health_Role,
        single_signer_signed_status_Role,
        single_signer_signed_isValid_Role,
        single_signer_signed_message_Role,
        single_signer_signed_signature_Role,
        single_signer_isColdcard_Role,
        single_signer_checked_Role
    };
    void requestSort(int role, int order);
private:
    QList<QSharedPointer<SingleSigner>> d_;
};

#endif // SIGNERLISTMODEL_H
