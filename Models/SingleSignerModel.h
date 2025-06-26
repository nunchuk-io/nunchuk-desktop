/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/

#ifndef SIGNERLISTMODEL_H
#define SIGNERLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "QOutlog.h"
#include "qUtils.h"
#include "Chats/QWalletSignersModel.h"
#include "Servers/DracoDefines.h"
#include <QJsonArray>
#include "TypeDefine.h"
#include "Commons/WalletKeys.h"

class QSingleSigner : public QObject, public WalletKeys {
    Q_OBJECT
    Q_PROPERTY(QString signerName               READ name               WRITE setName               NOTIFY nameChanged)
    Q_PROPERTY(QString signerXpub               READ xpub               WRITE setXpub               NOTIFY xpubChanged)
    Q_PROPERTY(QString signerPublickey          READ publickey          WRITE setPublickey          NOTIFY publickeyChanged)
    Q_PROPERTY(QString signerDerivationPath     READ derivationPath     WRITE setDerivationPath     NOTIFY derivationPathChanged)
    Q_PROPERTY(QString signerMasterFingerPrint  READ masterFingerPrint  WRITE setMasterFingerPrint  NOTIFY masterFingerPrintChanged)
    Q_PROPERTY(QString signerMasterSignerId     READ masterSignerId                                 NOTIFY masterSignerIdChanged)
    Q_PROPERTY(QString signerLastHealthCheck    READ lastHealthCheck                                NOTIFY healthChanged)
    Q_PROPERTY(bool    signerSigned             READ signerSigned                                   NOTIFY signerSignedChanged)
    Q_PROPERTY(bool    signerNeedTopUpXpub      READ needTopUpXpub                                  NOTIFY needTopUpXpubChanged)
    Q_PROPERTY(QString message                  READ message            WRITE setMessage            NOTIFY messageChanged)
    Q_PROPERTY(QString signature                READ signature          WRITE setSignature          NOTIFY signatureChanged)
    Q_PROPERTY(int     signerHealth             READ health                                         NOTIFY healthChanged)
    Q_PROPERTY(bool    isColdCard               READ isColdCard                                     NOTIFY isColdCardChanged)
    Q_PROPERTY(int     signerType               READ signerType                                     NOTIFY signerTypeChanged)
    Q_PROPERTY(QString devicetype               READ devicetype                                     NOTIFY devicetypeChanged)
    Q_PROPERTY(bool     isPrimaryKey            READ isPrimaryKey                                   NOTIFY isPrimaryKeyChanged)
    Q_PROPERTY(QString tag                      READ tag                                            CONSTANT)
    Q_PROPERTY(bool hasSignBtn                  READ hasSignBtn                                     CONSTANT)
    Q_PROPERTY(int  accountIndex                READ accountIndex                                   CONSTANT)
    Q_PROPERTY(QVariantList healthCheckHistory  READ healthCheckHistory                             NOTIFY healthCheckHistoryChanged)
    Q_PROPERTY(QString address                  READ address            WRITE setAddress            NOTIFY addressChanged)
    Q_PROPERTY(QString descriptor               READ descriptor                                     CONSTANT)
    Q_PROPERTY(bool isMine                      READ isMine                                         CONSTANT)
    Q_PROPERTY(QString cardId                   READ cardId                                         CONSTANT)
    Q_PROPERTY(bool taprootSupported            READ taprootSupported                               CONSTANT)
    Q_PROPERTY(bool keysetIndex                 READ keysetIndex                                    NOTIFY keysetIndexChanged)
    Q_PROPERTY(bool keysetStatus                READ keysetStatus                                   NOTIFY keysetStatusChanged)
    Q_PROPERTY(bool valueKey                    READ valueKey                                       NOTIFY valueKeyChanged)

public:
    QSingleSigner();
    QSingleSigner(const nunchuk::SingleSigner& singleKey);

    ~QSingleSigner();

    void convert(const nunchuk::SingleSigner& src);

    nunchuk::SingleSigner originSingleSigner() const;

    nunchuk::PrimaryKey originPrimaryKey();

    QString name();
    void setName(const QString& d);

    QString xpub();
    void setXpub(const QString& d);

    QString publickey();
    void setPublickey(const QString& d);

    QString derivationPath();
    void setDerivationPath(const QString& d);

    QString fingerPrint() const final;
    QString masterFingerPrint() const;
    void setMasterFingerPrint(const QString& d);

    QString masterSignerId();
    void setMasterSignerId(const QString& d);

    int signerType() const;
    void setSignerType(int signer_type);

    std::vector<nunchuk::SignerTag> signerTags() const;
    void setSignerTags(const std::vector<nunchuk::SignerTag> signer_tags);

    QString lastHealthCheck();
    QDateTime lastHealthCheckDateTime() const;

    bool signerSigned() const;
    void setSignerSigned(const bool d);

    bool needTopUpXpub();
    void setNeedTopUpXpub(bool needTopUpXpub);

    QString message() const;
    void setMessage(const QString &message);

    QString signature() const;
    void setSignature(const QString &signature);

    int health() const;
    void setHealth(int health);

    // For add signer to wallet
    bool checked() const;
    void setChecked(const bool checked);

    bool readyToSign() const;

    bool isColdCard();

    bool isLocalSigner();

    bool isMine() const;

    bool isPrimaryKey() const;

    QString devicetype() const;
    void setDevicetype(QString devicetype);

    QString cardId();
    void setCardId(const QString &card_id);
    nunchuk::SingleSigner singleSigner() const;
    QString email() const;
    void setEmail(const QString &email);

    QString tag() const;
    QStringList tags() const;

    bool hasSignBtn() const;
    void setHasSignBtn(bool hasSignBtn);

    int accountIndex();

    QString address() const;
    void setAddress(const QString &address);

    QString descriptor() const;

    Q_INVOKABLE QVariantList getWalletList();
    bool isReplaced() const;
    void setIsReplaced(bool newIsReplaced);

    QSingleSignerPtr keyReplaced() const;
    void setKeyReplaced(const QSingleSignerPtr &keyReplaced);

    bool taprootSupported();
    int  keysetIndex() const;
    void setKeysetIndex(const int index);
    int  keysetStatus() const;
    void setKeysetStatus(const int status);
    int  keysetPendingNumber() const;
    void setKeysetPendingNumber(const int number);
    bool valueKey() const;
    void setValueKey(const bool data);
    bool isValid();
    bool isOccupied() const;
    void setIsOccupied(bool newIsOccupied);

    bool needPassphrase() const;
    void setNeedPassphrase(bool newNeed_passphrase);

    bool needBackup();
    void setNeedBackup(bool val);
    bool allowAssignToWallet() const;

private:
    QString xpub_ = "";
    QString public_key_ = "";
    QString derivation_path_ = "";
    QString master_fingerprint_ = "";
    QString master_signer_id_ = "";
    bool signed_ = false;           // Using for transaction only
    bool needTopUpXpub_ = false;          // Using for create wallet
    QString message_ = qUtils::QGenerateRandomMessage();
    QString signature_ = "";
    int health_ = -1;
    // For add signer to wallet
    bool checked_ = false;
    QString m_devicetype = "";
    QString cardId_ = "";
    QString m_email = "";
    nunchuk::PrimaryKey     primaryKey_;
    nunchuk::SingleSigner   singleSigner_;
    bool isDraft = true;
    bool m_hasSignBtn {true};
    // QJsonArray m_healthCheckHistory {};
    QString m_address;
    bool m_isReplaced {false};
    QSingleSignerPtr m_keyReplaced;

    // Taproot only
    int  m_keyset_index {0};
    int  m_keyset_status {0};
    int  m_keyset_pendingnumber {0};
    bool m_valuekey {false};
    bool m_isOccupied {false};
    bool m_need_passphrase {false};
    bool m_need_backup {false};
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
    void signerTypeChanged();
    void signerSignedChanged();
    void isRemoteChanged();
    void needTopUpXpubChanged();
    void messageChanged();
    void signatureChanged();
    void healthChanged();
    void isColdCardChanged();
    // For add signer to wallet
    void checkedChanged();
    void isSoftwareChanged();
    void readyToSignChanged();
    void isLocalSignerChanged();
    void devicetypeChanged();
    void isPrimaryKeyChanged();
    void emailChanged();
    void healthCheckHistoryChanged() final;
    void addressChanged();
    void notifySignerExist(bool isSoftware);
    void keysetIndexChanged();
    void keysetStatusChanged();
    void keysetPendingNumberChanged();
    void valueKeyChanged();
};

bool sortSingleSignerByNameAscending(const QSingleSignerPtr &v1, const QSingleSignerPtr &v2);
bool sortSingleSignerByKeysetIndexAscending(const QSingleSignerPtr &v1, const QSingleSignerPtr &v2);
bool sortSingleSignerByKetsetSelected(const QSingleSignerPtr &v1, const QSingleSignerPtr &v2);

class SingleSignerListModel  : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int  keyinfo                 READ keyinfo                                            NOTIFY keyinfoChanged)
    Q_PROPERTY(int  signerCount             READ signerCount                                        NOTIFY signerCountChanged)
    Q_PROPERTY(int  signerSelectedCount     READ signerSelectedCount                                NOTIFY signerSelectedCountChanged)
    Q_PROPERTY(int  signerValueKeyCount     READ signerValueKeyCount                                NOTIFY signerValueKeyCountChanged)
    Q_PROPERTY(bool needTopUpXpubs          READ needTopUpXpubs                                     NOTIFY needTopUpXpubsChanged)

public:
    SingleSignerListModel();
    ~SingleSignerListModel();
    enum SingleSignerRoles {
        single_signer_name_Role,
        single_signer_xpub_Role,
        single_signer_public_key_Role,
        single_signer_derivation_path_Role,
        single_signer_master_fingerprint_Role,
        single_signer_master_signer_id_Role,
        single_signer_last_health_check_Role,
        single_signer_type_Role,
        single_signer_health_Role,
        single_signer_signed_status_Role,
        single_signer_need_Topup_Xpub_Role,
        single_signer_signed_message_Role,
        single_signer_signed_signature_Role,
        single_signer_isColdcard_Role,
        single_signer_checked_Role,
        single_signer_readyToSign_Role,
        single_signer_is_local_Role,
        single_signer_primary_key_Role,
        single_signer_devicetype_Role,
        single_signer_device_cardid_Role,
        single_signer_tag_Role,
        single_signer_has_sign_btn_Role,
        single_signer_account_index_Role,
        single_signer_isReplaced_Role,
        single_signer_keyReplaced_Role,
        single_signer_taproot_supported_Role,
        single_signer_keyset_index_Role,
        single_signer_keyset_status_Role,
        single_signer_keyset_remaining_Role,
        single_signer_value_key_Role,
        single_signer_isOccupied_Role,
        single_signer_needPassphrase_Role,
        single_signer_needBackup_Role,
        single_signer_allowAssignToWallet_Role,
    };
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QHash<int,QByteArray> roleNames() const;
    static QHash<int,QByteArray> roleSignerNames();
    static QVariant dataSigner(const QSingleSignerPtr &data, int role);
    static QVariant useQml(const QSingleSignerPtr &data);
    void replaceSingleSigner(int index, const QSingleSignerPtr &value);
    void addSingleSigner(const QSingleSignerPtr &d);
    void addKeysetSigner(const QSingleSignerPtr &signer, const int keyset_index);
    void updateKeysetPendingnumber(const int keyset_index, const int number);
    void updateSignatures(const QString& masterfingerprint, const bool value, const QString& signature);
    bool needTopUpXpubs() const;
    void initSignatures();
    QSingleSignerPtr getSingleSignerByIndex(const int index);
    bool containsMasterSignerId(const QString& masterSignerId);
    bool removeSingleSignerByIndex(const int index);
    bool removeSingleSignerByType(const int type);
    QString getMasterSignerIdByIndex(const int index);
    QString getMasterSignerXfpByIndex(const int index);
    QSingleSignerPtr getSingleSignerByFingerPrint(const QString &xfp);
    QSingleSignerPtr getSingleSignerByFingerPrint(const QString &xfp, const QString& name);
    int getIndexByFingerPrint(const QString &fingerprint);
    int getnumberSigned();
    bool containsHardwareKey();
    bool containsFingerPrint(const QString& masterFingerPrint);
    bool containsKeyset(const QString &xfp, const int keyset_index);
    bool containsSigner(const QString &xfp, const QString &path);
    bool containsColdcard();
    bool checkUsableToSign(const QString& masterFingerPrint);
    void updateSignerHealthStatus(const QString &xfp, const int status, const time_t time);
    void notifyMasterSignerDeleted(const QString &masterSignerId);
    void updateSignerOfRoomWallet(const SignerAssigned &signer);
    void resetUserChecked();
    void setUserCheckedByFingerprint(const bool state, const QString fp);
    void setUserChecked(const bool state, const int index);
    void updateHealthCheckTime();
    nunchuk::PrimaryKey containPrimaryKey(const QString& fingerprint);
    void syncNunchukEmail(QList<DracoUser> users);
    bool needSyncNunchukEmail();
    void requestSort(bool force = false);
    void requestSortKeyset();
    void requestSortKeysetSelected();
    QList<QSingleSignerPtr> fullList() const;
    std::vector<nunchuk::SingleSigner> signers() const;
    std::vector<nunchuk::SingleSigner> localSigners() const;
    QSharedPointer<SingleSignerListModel> clone() const;
    QSharedPointer<SingleSignerListModel> cloneKeysets(std::vector<nunchuk::KeysetStatus> keyset_status) const;
    QSharedPointer<SingleSignerListModel> cloneFinalSigners(std::map<std::string, bool> final_signers) const;

    void cleardata();
    QStringList getKeyNames();
    void setCardIDList(const QMap<QString, QString> &card_ids);

    // Fortaproot
    int keyinfo() const;
    int signerValueKeyCount();

public slots:
    int signerCount() const;
    int signerSelectedCount() const;
    int keysetStatus(int keyset_index);
    int keysetRemaingSignarure(int keyset_index);

signals:
    void keyinfoChanged();
    void signerCountChanged();
    void signerSelectedCountChanged();
    void signerValueKeyCountChanged();
    void needTopUpXpubsChanged();
    void keysetSelectedChanged();

private:
    QList<QSingleSignerPtr> m_data;
};
typedef QSharedPointer<SingleSignerListModel> QSingleSignerListModelPtr;

#endif // SIGNERLISTMODEL_H
