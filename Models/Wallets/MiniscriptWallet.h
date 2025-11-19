#ifndef MINISCRIPTWALLET_H
#define MINISCRIPTWALLET_H

#include "SandboxWallet.h"

#include "nunchuk.h"
#include "miniscript/timeline.h"
#include "TypeDefine.h"
#include <QTimeZone>

class ScriptNodeHelper : public QObject {
    Q_OBJECT

public:
    enum class Type {
        NONE = static_cast<int>(nunchuk::ScriptNode::Type::NONE),
        PK = static_cast<int>(nunchuk::ScriptNode::Type::PK),
        OLDER = static_cast<int>(nunchuk::ScriptNode::Type::OLDER),
        AFTER = static_cast<int>(nunchuk::ScriptNode::Type::AFTER),
        HASH160 = static_cast<int>(nunchuk::ScriptNode::Type::HASH160),
        HASH256 = static_cast<int>(nunchuk::ScriptNode::Type::HASH256),
        RIPEMD160 = static_cast<int>(nunchuk::ScriptNode::Type::RIPEMD160),
        SHA256 = static_cast<int>(nunchuk::ScriptNode::Type::SHA256),
        AND = static_cast<int>(nunchuk::ScriptNode::Type::AND),
        OR = static_cast<int>(nunchuk::ScriptNode::Type::OR),
        ANDOR = static_cast<int>(nunchuk::ScriptNode::Type::ANDOR),
        THRESH = static_cast<int>(nunchuk::ScriptNode::Type::THRESH),
        MULTI = static_cast<int>(nunchuk::ScriptNode::Type::MULTI),        
        OR_TAPROOT = static_cast<int>(nunchuk::ScriptNode::Type::OR_TAPROOT),
        MUSIG = static_cast<int>(nunchuk::ScriptNode::Type::MUSIG),
    };
    Q_ENUM(Type)

    enum class TimelockBased {
        NONE = static_cast<int>(nunchuk::Timelock::Based::NONE),
        TIME_LOCK = static_cast<int>(nunchuk::Timelock::Based::TIME_LOCK),
        HEIGHT_LOCK = static_cast<int>(nunchuk::Timelock::Based::HEIGHT_LOCK),
    };
    Q_ENUM(TimelockBased)
    
    enum class TimelockType {
        LOCKTYPE_ABSOLUTE = static_cast<int>(nunchuk::Timelock::Type::LOCKTYPE_ABSOLUTE),
        LOCKTYPE_RELATIVE = static_cast<int>(nunchuk::Timelock::Type::LOCKTYPE_RELATIVE),
    };
    Q_ENUM(TimelockType)
};

class QWalletTimezoneModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int          currentIndex        READ currentIndex       WRITE setCurrentIndex       NOTIFY currentIndexChanged)
    Q_PROPERTY(QString      selectedTimezone    READ selectedTimezone                               NOTIFY currentIndexChanged)
public:
    QWalletTimezoneModel();
    ~QWalletTimezoneModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString formatTimeZone(const QTimeZone &tz) const;
    qint64 offsetFromUtc(const QTimeZone &tz) const;

    void timezones();
    void printAllTimezones() const;
    enum Timezones {
        timezone_id_Role,
        timezone_name_Role,
        timezone_offsetFromUtc_Role,
    };

    int currentIndex() const;
    void setCurrentIndex(int newCurrentIndex);

    QString selectedTimezone();
    QByteArray selectedTimezoneId();
    void setSelectedTimezone(const QByteArray &timezone);
    QByteArray convertFormattedToTimezoneId(const QString &formatted);
    void setSelectedTimezone(const QString &formatted);
signals:
    void currentIndexChanged();

private:
    static QList<QByteArray>  m_timezones;
    static QByteArray         m_systemTimezone;
    QByteArray                m_selectedTimezone;
    int                       m_currentIndex {0};
};
typedef OurSharedPointer<QWalletTimezoneModel> QWalletTimezoneModelPtr;


#define CreateMiniscript public
#define Miniscript public

class MiniscriptWallet : public SandboxWallet {
    Q_OBJECT
    Q_PROPERTY(int              newWalletN              READ newWalletN                 WRITE setNewWalletN                 NOTIFY newWalletNChanged)
    Q_PROPERTY(int              newWalletM              READ newWalletM                 WRITE setNewWalletM                 NOTIFY newWalletMChanged)
    Q_PROPERTY(bool             reUseKeys               READ reUseKeys                  WRITE setReUseKeys                  NOTIFY reUseKeysChanged)
    Q_PROPERTY(QString          customizeMiniscript     READ customizeMiniscript        WRITE setCustomizeMiniscript        NOTIFY customizeMiniscriptChanged)
    Q_PROPERTY(int              timelockType            READ timelockType               WRITE setTimelockType               NOTIFY timelockTypeChanged)
    Q_PROPERTY(int              timeUnit                READ timeUnit                   WRITE setTimeUnit                   NOTIFY timeUnitChanged)
    Q_PROPERTY(QVariant         timeMini                READ timeMini                                                       NOTIFY timeMiniChanged)
    Q_PROPERTY(QVariantList     treeMiniscript          READ treeMiniscript                                                 NOTIFY treeMiniscriptChanged)
    Q_PROPERTY(QVariantList     keypaths                READ keypaths                                                       NOTIFY keypathsChanged)
    Q_PROPERTY(bool             enoughSigners           READ enoughSigners                                                  NOTIFY treeMiniscriptChanged)
    Q_PROPERTY(bool             keyPathActivated        READ keyPathActivated                                               CONSTANT)
    Q_PROPERTY(QWalletTimezoneModel*   timezones        READ timezones                                                      NOTIFY timezonesChanged)
    Q_PROPERTY(QVariantList            timelocklist     READ timelocklist                                                   NOTIFY timelocklistChanged)
    Q_PROPERTY(bool                    timeLocked       READ timeLocked                                                     NOTIFY timeLockedChanged)

public:
    MiniscriptWallet(const nunchuk::Wallet &w);
    ~MiniscriptWallet() override = default;

Miniscript:
    void convert(const nunchuk::Wallet w) override;
    void convertToMiniscript(const nunchuk::Wallet &w);

CreateMiniscript:
    int newWalletN() const;
    void setNewWalletN(int n);
    int newWalletM() const;
    void setNewWalletM(int m);
    bool reUseKeys() const;
    void setReUseKeys(bool reUse);
    QString scriptTemplate();
    QString customizeMiniscript();
    void setCustomizeMiniscript(const QString &miniscript);
    void reformatMiniscript(const QString &miniscript);

    int timelockType() const;
    void setTimelockType(int type);
    int timeUnit() const;
    void setTimeUnit(int unit);

    QVariantList treeMiniscript() const;
    QJsonArray treeMiniscriptJs() const;
    void setTreeMiniscript(const QJsonArray &tree);

    QVariant timeMini() const;
    quint64  timeMiniValue() const;

    QJsonObject getKeyDetails(const QJsonObject &oldKey, const QString &key);

    QJsonArray createTreeMiniscript(const nunchuk::ScriptNode &node, QList<int> &levels);

    QVariantList keypaths();
    QJsonArray keypathsJs();
    QJsonArray createKeypaths(const QStringList &keypaths);
    void setKeypaths(const QJsonArray &keypath);

    std::map<std::string, nunchuk::SingleSigner> signersCreateWallet();

    QString keySelected() const;
    void setKeySelected(const QString &key);

    bool enoughSigners() const;

    bool AddMasterToWallet();
    bool AddRemoteToWallet();
    bool AddSignerToWallet(const QSingleSignerPtr &signer);

    void registerSigners();
    void makeExistingSigners();

    QStringList keySameList(const QString& key_i_j);

    QString miniscript() const;

    bool keyPathActivated();
    SingleSignerListModel* singleSignersKeyPath();

    QWalletTimezoneModel* timezones() const;
    QWalletTimezoneModelPtr timezonesPtr() const;
    void setTimezones(QWalletTimezoneModelPtr timezones);

    bool needCheckDuplicate() const;
    void setNeedCheckDuplicate(bool needCheck);

    QVariantList timelocklist();
    void setTimelocklist(const std::vector<int64_t> timelocks);

    bool timeLocked() const;
    void setTimeLocked(bool newTimeLocked);

public slots:
    bool isValidTapscriptTemplate(const QString &userInput);
    bool isValidMiniscriptTemplate(const QString &userInput);
    bool enterCustomMiniscript(const QString &userInput);
    bool miniscriptTemplateSelected(const QString &userSelect);
    void updateTimeMiniscript(const QString &key, const QVariant &value);
    void updateTimeMiniscript(const QMap<QString, QVariant>& dataMap);
    void clearTimeMiniscript(const QString &userInput = "");
    bool configureWallet(const QString &script_tmpl = "");
    bool importMiniscriptFile(const QString &filePath);
    void updateSignersMiniscript(const QString &key, const QSingleSignerPtr &value, bool autoreuse = true);
    void removeSignersMiniscript(const QString &key);
    void clearSignersMiniscript();
    void requestAddSigner(const QString group, const QString &key);
    void requestRemoveSigner(const QString group, const QString &key);
    void requestAddNewKey();
    void requestAddExistKey(const QString &xfp);
    void requestChangeWalletTypeToTaproot();
    virtual bool editBIP32Path(const QVariant &singleData, const QVariant &customData, const QString &path);
    QMap<QString, int> getKeyStrCount(const QJsonArray &scriptPaths, const QJsonArray &keyPaths);
    QJsonArray scriptPathRefresh(const QJsonArray &tree, const QMap<QString, int> &duplicateCheck);
    QJsonArray keyPathRefresh(const QJsonArray &tree, const QMap<QString, int> &duplicateCheck);
    void checkDuplicateKey();

signals:
    void newWalletNChanged();
    void newWalletMChanged();
    void setupTimelockChanged();
    void reUseKeysChanged();
    void customizeMiniscriptChanged();
    void timelockTypeChanged();
    void timeUnitChanged();
    void timeMiniChanged();
    void treeMiniscriptChanged();
    void keypathsChanged();
    void editBIP32PathSuccess(int typeError);
    void duplicateKeyError(const QVariant &signerData, const QVariant &customData);
    void timezonesChanged();
    void needTopUpXpub();
    void timelocklistChanged();

    void timeLockedChanged();

private:
    QString m_customizeMiniscript {};
    int m_newWalletN {0};
    int m_newWalletM {0};
    bool m_reUseKeys {false}; // Whether to reuse keys in the new wallet
    int m_timelockType {static_cast<int>(nunchuk::Timelock::Type::LOCKTYPE_ABSOLUTE)};
    int m_timeUnit {static_cast<int>(nunchuk::Timelock::Based::TIME_LOCK)}; // 0: timestamp, 1: block 
    QMap<QString, QVariant> m_timeMiniData {};
    QJsonArray m_treeMiniscript {};
    QJsonArray m_keypaths {}; // Keypath information for the wallet, if applicable
    QMap<QString, QSingleSignerPtr> m_signersMiniscript {}; // <key, Signers>
    QString m_keySelected {}; // Selected key for operations
    QSingleSignerListModelPtr   m_signersKeyPath {nullptr}; // Model for keypath signers
    QWalletTimezoneModelPtr m_timezones; // Model for timezones
    QMap<QString, int> m_defaultKeys;
    bool m_needCheckDuplicate {false};
    QVariantList m_timelocklist {};
    bool    m_timeLocked {false};
    struct CheckBoxInfo {
        bool hasCheckBox {false};
        bool checked {true};
    };
    QMap<QString, bool> m_checkBoxInfo; // Store checkbox info for script nodes
};

#endif // MINISCRIPTWALLET_H
