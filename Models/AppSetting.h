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

#ifndef APPSETTING_H
#define APPSETTING_H

#include <QObject>
#include <QSettings>
#include <QMap>

#define HWI_PATH        "hwi"
#define LOCAL_ADDRESS   "127.0.0.1"
#define TOR_PORT        9050
#define CORERPC_MAINNET_PORT        8332
#define CORERPC_TESTNET_PORT        18332
#define EXPLORER_MAINNET "https://mempool.space/tx/"
#define EXPLORER_TESTNET "https://mempool.space/testnet4/tx/"
#define EXPLORER_SIGNNET "https://mempool.space/signet/tx/"
#define MAINNET_SERVER   "mainnet.nunchuk.io:51001"
#define TESTNET_SERVER   "testnet.nunchuk.io:50001"
#define SIGNET_SERVER    "ssl://mempool.space:60602"

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class QWalletCached {
public:
    QWalletCached(){}
    T1 groupId;
    T2 slug;
    T3 myRole;
    T4 status;
    T5 backedup;
    T6 hideFiatCurrency;
};

class  NunchukSettings : public QSettings {

public:
    NunchukSettings();
    ~NunchukSettings();
    QString groupSetting() const;
    void setGroupSetting(QString group);
    bool contains(const QString& key) const;
    void setValue(const QString &key, const QVariant &value);
    void setValueCommon(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    QVariant valueCommon(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void removeKey(const QString &key);
    bool containsCommon(const QString& key) const;
    void setCommonValue(const QString &key, const QVariant &value);
    QVariant commonValue(const QString &key, const QVariant &defaultValue = QVariant()) const;
private:
    QString m_group;
};

class AppSetting : public NunchukSettings
{
    Q_OBJECT
    Q_PROPERTY( int unit                        READ unit                       WRITE setUnit                       NOTIFY unitChanged)
    Q_PROPERTY( QVariant mainnetServer          READ mainnetServer                                                  NOTIFY mainnetServerChanged)
    Q_PROPERTY( QString testnetServer           READ testnetServer              WRITE setTestnetServer              NOTIFY testnetServerChanged)
    Q_PROPERTY( QString signetServer            READ signetServer               WRITE setSignetServer               NOTIFY signetServerChanged)
    Q_PROPERTY( bool enableDualServer           READ enableDualServer           WRITE setEnableDualServer           NOTIFY enableDualServerChanged)
    Q_PROPERTY( bool enableCustomizeHWIDriver   READ enableCustomizeHWIDriver   WRITE setEnableCustomizeHWIDriver   NOTIFY enableCustomizeHWIDriverChanged)
    Q_PROPERTY( QString hwiPath                 READ hwiPath                    WRITE setHwiPath                    NOTIFY hwiPathChanged)
    Q_PROPERTY( bool enableDBEncryption         READ enableDBEncryption         WRITE setEnableDBEncryption         NOTIFY enableDBEncryptionChanged)
    Q_PROPERTY( bool enableTorProxy             READ enableTorProxy             WRITE setEnableTorProxy             NOTIFY enableTorProxyChanged)
    Q_PROPERTY( QString torProxyAddress         READ torProxyAddress            WRITE setTorProxyAddress            NOTIFY torProxyAddressChanged)
    Q_PROPERTY( int torProxyPort                READ torProxyPort               WRITE setTorProxyPort               NOTIFY torProxyPortChanged)
    Q_PROPERTY( QString torProxyName            READ torProxyName               WRITE setTorProxyName               NOTIFY torProxyNameChanged)
    Q_PROPERTY( QString torProxyPassword        READ torProxyPassword           WRITE setTorProxyPassword           NOTIFY torProxyPasswordChanged)
    Q_PROPERTY( int primaryServer               READ primaryServer              WRITE setPrimaryServer              NOTIFY primaryServerChanged)
    Q_PROPERTY( QString secondaryServer         READ secondaryServer            WRITE setSecondaryServer            NOTIFY secondaryServerChanged)
    Q_PROPERTY( bool enableFixedPrecision       READ enableFixedPrecision       WRITE setEnableFixedPrecision       NOTIFY enableFixedPrecisionChanged)
    Q_PROPERTY( bool enableCertificateFile      READ enableCertificateFile      WRITE setEnableCertificateFile      NOTIFY enableCertificateFileChanged)
    Q_PROPERTY( QString certificateFile         READ certificateFile            WRITE setCertificateFile            NOTIFY certificateFileChanged)
    Q_PROPERTY( bool enableCoreRPC              READ enableCoreRPC              WRITE setEnableCoreRPC              NOTIFY enableCoreRPCChanged)
    Q_PROPERTY( QString coreRPCAddress          READ coreRPCAddress             WRITE setCoreRPCAddress             NOTIFY coreRPCAddressChanged)
    Q_PROPERTY( int     coreRPCPort             READ coreRPCPort                WRITE setCoreRPCPort                NOTIFY coreRPCPortChanged)
    Q_PROPERTY( QString coreRPCName             READ coreRPCName                WRITE setCoreRPCName                NOTIFY coreRPCNameChanged)
    Q_PROPERTY( QString coreRPCPassword         READ coreRPCPassword            WRITE setCoreRPCPassword            NOTIFY coreRPCPasswordChanged)

    Q_PROPERTY( int connectionState             READ connectionState                                                NOTIFY connectionStateChanged)
    Q_PROPERTY( int syncPercent                 READ syncPercent                                                    NOTIFY syncPercentChanged)
    Q_PROPERTY( QString signetStream            READ signetStream               WRITE setSignetStream               NOTIFY signetStreamChanged)
    Q_PROPERTY( bool enableSignetStream         READ enableSignetStream         WRITE setEnableSignetStream         NOTIFY enableSignetStreamChanged)

    Q_PROPERTY(bool enableDebugMode             READ enableDebug                WRITE setEnableDebug                NOTIFY enableDebugChanged)
    Q_PROPERTY(bool isStarted                   READ isStarted                                                      NOTIFY isStartedChanged)
    Q_PROPERTY(bool enableMultiDeviceSync       READ enableMultiDeviceSync      WRITE setEnableMultiDeviceSync      NOTIFY enableMultiDeviceSyncChanged)
    Q_PROPERTY(QString currency                 READ currency                   WRITE setCurrency                   NOTIFY currencyChanged)
    Q_PROPERTY(QString currencySymbol           READ currencySymbol                                                 NOTIFY currencyChanged)
    Q_PROPERTY(bool isFirstTimeOnboarding       READ isFirstTimeOnboarding      WRITE setIsFirstTimeOnboarding      NOTIFY isFirstTimeOnboardingChanged)
    Q_PROPERTY(bool enableColab                 READ enableColab                WRITE setEnableColab                NOTIFY enableColabChanged)
    Q_PROPERTY(QStringList favoriteAddresses    READ favoriteAddresses                                              NOTIFY favoriteAddressesChanged)
    Q_PROPERTY(int feeSetting                   READ feeSetting                 WRITE setFeeSetting                 NOTIFY feeSettingChanged)
    Q_PROPERTY(bool enableAntiFeeSniping        READ enableAntiFeeSniping       WRITE setEnableAntiFeeSniping       NOTIFY enableAntiFeeSnipingChanged)
    Q_PROPERTY(bool enableAutoFeeFunction       READ enableAutoFeeFunction                                          CONSTANT)
    Q_PROPERTY(bool enableAutoFeeSelection      READ enableAutoFeeSelection     WRITE setEnableAutoFeeSelection     NOTIFY enableAutoFeeSelectionChanged)
    Q_PROPERTY(QString thresholdPercentDisplay  READ thresholdPercentDisplay    WRITE setthresholdPercentDisplay    NOTIFY thresholdPercentChanged)
    Q_PROPERTY(int thresholdPercent             READ thresholdPercent           WRITE setthresholdPercent           NOTIFY thresholdPercentChanged)
    Q_PROPERTY(double thresholdAmount           READ thresholdAmount            WRITE setthresholdAmount            NOTIFY thresholdAmountChanged)
    Q_PROPERTY(QString thresholdAmountDisplay   READ thresholdAmountDisplay     WRITE setthresholdAmountDisplay     NOTIFY thresholdAmountChanged)

public:
    enum class Chain : int {
        MAIN,
        TESTNET,
        SIGNET,
        REGTEST
    };

    enum class Unit : int {
        BTC,
        SATOSHI
    };

    enum class ConnectionStatus : int {
        OFFLINE,
        SYNCING,
        ONLINE
    };
public:
    static AppSetting *instance();
    AppSetting(AppSetting &other) = delete;
    AppSetting(AppSetting const &other) = delete;
    void operator=(const AppSetting &other) = delete;

    void refresh();

    void setGroupSetting(QString group);
    void resetSetting();
    Q_INVOKABLE void resetNetworkSetting(bool forceReset = false);

    int unit();
    void setUnit(int unit);
    // Settings
    void updateUnit();

    QVariant mainnetServer();
    Q_INVOKABLE void setMainnetServer(const QVariant& value);

    QString testnetServer();
    void setTestnetServer(const QString &testnetServer);

    QString signetServer();
    void setSignetServer(const QString &signetServer);

    bool enableDualServer();
    void setEnableDualServer(bool enableDualServer);

    bool enableCustomizeHWIDriver();
    void setEnableCustomizeHWIDriver(bool enableCustomizeHWIDriver);

    QString hwiPath();
    void setHwiPath(const QString &hwiPath);

    bool enableDBEncryption();
    void setEnableDBEncryption(bool enableDBEncryption);

    bool enableTorProxy();
    void setEnableTorProxy(bool enableTorProxy);

    QString torProxyAddress();
    void setTorProxyAddress(const QString &torProxyAddress);

    int torProxyPort();
    void setTorProxyPort(const int torProxyPort);

    QString torProxyName();
    void setTorProxyName(const QString &torProxyName);

    QString torProxyPassword();
    void setTorProxyPassword(const QString &torProxyPassword);

    int primaryServer();
    void setPrimaryServer(int primaryServer);

    QString secondaryServer();
    void setSecondaryServer(const QString &secondaryServer);

    bool enableFixedPrecision();
    void setEnableFixedPrecision(bool enableFixedPrecision);

    bool enableCertificateFile();
    void setEnableCertificateFile(bool enableCertificateFile);

    QString certificateFile();
    void setCertificateFile(const QString &certificateFile);

    QString storagePath();

    QString executePath() const;
    void setExecutePath(const QString &executePath);

    int connectionState() const;
    void setConnectionState(int connectionState);

    bool enableCoreRPC();
    void setEnableCoreRPC(bool enableCoreRPC);

    QString coreRPCAddress();
    void setCoreRPCAddress(const QString &coreRPCAddress);

    int coreRPCPort();
    void setCoreRPCPort(const int coreRPCPort);

    QString coreRPCName();
    void setCoreRPCName(const QString &coreRPCName);

    QString coreRPCPassword();
    void setCoreRPCPassword(const QString &coreRPCPassword);

    int syncPercent() const;
    void setSyncPercent(int syncPercent);

    QString signetStream();
    void setSignetStream(const QString &signetStream);

    bool enableSignetStream();
    void setEnableSignetStream(const bool &enableSignetStream);

    bool enableDebug();
    void setEnableDebug(bool enableDebugMode);

    bool enableColab();
    void setEnableColab(bool enableColab);

    bool isStarted();
    Q_INVOKABLE void setIsStarted(bool isStarted, bool isSetting);

    bool enableMultiDeviceSync();
    void setEnableMultiDeviceSync(bool enableMultiDeviceSync);

    QString currencySymbol();
    QString currency();
    void setCurrency(QString currency);

    void setWalletCached(QString id, QWalletCached<QString /*group id*/,
                                                   QString /*group slug*/,
                                                   QString /*group role*/,
                                                   QString /*wallet status*/,
                                                   bool    /*wallet cached*/,
                                                   bool    /*wallet hide fiat currency*/> input);

    bool getwalletCached(QString id, QWalletCached<QString /*group id*/,
                                                   QString /*group slug*/,
                                                   QString /*group role*/,
                                                   QString /*wallet status*/,
                                                   bool    /*wallet cached*/,
                                                   bool    /*wallet hide fiat currency*/> &output);
    void deleteWalletCached(QString id);

    void setTransactionKeysetIndex(QString id, const int keyset_index);

    int  getTransactionKeysetIndex(QString id);

    void deleteTransactionKeysetIndex(QString id);

    bool isFirstTimeOnboarding();
    void setIsFirstTimeOnboarding(bool isFirstTime);

    QVariant getMainnetList();
    void setMainnetList(const QVariant &servers);

    QVariant getReminderStates();
    void setReminderStates(const QVariant &states);

    QStringList favoriteAddresses();
    Q_INVOKABLE void loadFavoriteAddresses();
    void setFavoriteAddresses(const QStringList &newFavoriteAddresses);
    Q_INVOKABLE void removeFavoriteAddress(const QString &label, const QString &address);
    Q_INVOKABLE void addFavoriteAddress(const QString &label, const QString &address);
    Q_INVOKABLE void updateFavoriteAddress(const QString &label, const QString &address);
    Q_INVOKABLE bool validateAddress(const QString &address);

    int feeSetting();
    void setFeeSetting(int fee);

    bool enableAntiFeeSniping();
    void setEnableAntiFeeSniping(bool enable);

    bool enableAutoFeeFunction();
    bool enableAutoFeeSelection();
    void setEnableAutoFeeSelection(bool newEnableAutoFeeSelection);

    int thresholdPercent();
    void setthresholdPercent(int newthresholdPercent);
    QString thresholdPercentDisplay();
    void setthresholdPercentDisplay(const QString &newthresholdPercentDisplay);

    double thresholdAmount();
    void setthresholdAmount(double newthresholdAmount);
    QString thresholdAmountDisplay();
    void setthresholdAmountDisplay(const QString &newthresholdAmountDisplay);

    void setOrderWalletList(const QStringList &orderWalletList);
    QStringList orderWalletList();

private:
    AppSetting();
    ~AppSetting();

    QJsonArray sortJsonArrayById(const QJsonArray& array);
    int unit_;
    QMap<QString, QVariant> mainnetServer_;
    QString testnetServer_;
    QString signetServer_;
    bool enableDualServer_;
    bool enableCustomizeHWIDriver_;
    QString hwiPath_;
    bool enableDBEncryption_;
    bool enableTorProxy_;
    QString torProxyAddress_;
    int torProxyPort_;
    QString torProxyName_;
    QString torProxyPassword_;
    int primaryServer_;
    QString secondaryServer_;
    bool enableFixedPrecision_;
    QString storagePath_;
    int connectionState_;
    int syncPercent_;
    bool enableCertificateFile_;
    QString certificateFile_;

    bool enableCoreRPC_;
    QString coreRPCAddress_;
    int coreRPCPort_;
    QString coreRPCName_;
    QString coreRPCPassword_;

    bool enableSignetStream_;
    QString signetStream_;

    bool enableDebugMode_;
    bool enableMultiDeviceSync_;
    bool isStarted_;
    bool isFirstTimeOnboarding_;
    bool enableColab_ {false};
    QStringList m_favoriteAddresses{};

    int  feeSetting_;
    bool enableAntiFeeSniping_{false};
    bool enableAutoFeeSelection_{false};
    int  thresholdPercent_{10};
    double  thresholdAmount_{0.20};

    QStringList m_orderWalletList {};

signals:
    void unitChanged();
    void mainnetServerChanged();
    void testnetServerChanged();
    void signetServerChanged();
    void enableDualServerChanged();
    void enableCustomizeHWIDriverChanged();
    void hwiPathChanged();
    void enableDBEncryptionChanged();
    void enableTorProxyChanged();
    void torProxyAddressChanged();
    void torProxyPortChanged();
    void torProxyNameChanged();
    void torProxyPasswordChanged();
    void primaryServerChanged();
    void secondaryServerChanged();
    void enableFixedPrecisionChanged();
    void storagePathChanged();
    void executePathChanged();
    void connectionStateChanged();
    void changePassphraseResultChanged();
    void enableCertificateFileChanged();
    void certificateFileChanged();
    void enableCoreRPCChanged();
    void coreRPCAddressChanged();
    void coreRPCPortChanged();
    void coreRPCNameChanged();
    void coreRPCPasswordChanged();
    void syncPercentChanged();
    void firstTimeCoreRPCChanged();
    void firstTimePassPhraseChanged();
    void signetStreamChanged();
    void enableSignetStreamChanged();
    void enableDebugChanged();
    void isStartedChanged();
    void enableMultiDeviceSyncChanged();
    void currencyChanged();
    void isFirstTimeOnboardingChanged();
    void enableColabChanged();
    void favoriteAddressesChanged();
    void mainnetServerSelected(const QVariant network);
    void feeSettingChanged();
    void enableAntiFeeSnipingChanged();
    void enableAutoFeeSelectionChanged();
    void thresholdPercentChanged();
    void thresholdAmountChanged();
};

#endif // APPSETTING_H
