#ifndef APPSETTING_H
#define APPSETTING_H

#include <QObject>
#include <QSettings>

#define MAINNET_SERVER  "mainnet.nunchuk.io:51001"
#define TESTNET_SERVER  "testnet.nunchuk.io:50001"
#define HWI_PATH        "hwi"
#define LOCAL_ADDRESS   "127.0.0.1"
#define TOR_PORT        9050
#define CORERPC_MAINNET_PORT        8332
#define CORERPC_TESTNET_PORT        18332
#define BLOCKSTREAM_TESTNET "https://blockstream.info/testnet/tx/"
#define BLOCKSTREAM_MAINNET "https://blockstream.info/tx/"

class AppSetting : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int unit                        READ unit                       WRITE setUnit                       NOTIFY unitChanged)
    Q_PROPERTY( QString mainnetServer           READ mainnetServer              WRITE setMainnetServer              NOTIFY mainnetServerChanged)
    Q_PROPERTY( QString testnetServer           READ testnetServer              WRITE setTestnetServer              NOTIFY testnetServerChanged)
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
    Q_PROPERTY( int changePassphraseResult      READ changePassphraseResult     WRITE setChangePassphraseResult     NOTIFY changePassphraseResultChanged)
    Q_PROPERTY( bool enableCertificateFile      READ enableCertificateFile      WRITE setEnableCertificateFile      NOTIFY enableCertificateFileChanged)
    Q_PROPERTY( QString certificateFile         READ certificateFile            WRITE setCertificateFile            NOTIFY certificateFileChanged)
    Q_PROPERTY( bool enableCoreRPC              READ enableCoreRPC              WRITE setEnableCoreRPC              NOTIFY enableCoreRPCChanged)
    Q_PROPERTY( QString coreRPCAddress          READ coreRPCAddress             WRITE setCoreRPCAddress             NOTIFY coreRPCAddressChanged)
    Q_PROPERTY( int     coreRPCPort             READ coreRPCPort                WRITE setCoreRPCPort                NOTIFY coreRPCPortChanged)
    Q_PROPERTY( QString coreRPCName             READ coreRPCName                WRITE setCoreRPCName                NOTIFY coreRPCNameChanged)
    Q_PROPERTY( QString coreRPCPassword         READ coreRPCPassword            WRITE setCoreRPCPassword            NOTIFY coreRPCPasswordChanged)

    Q_PROPERTY( int connectionState             READ connectionState                                                NOTIFY connectionStateChanged)
    Q_PROPERTY( int syncPercent                 READ syncPercent                                                    NOTIFY syncPercentChanged)
    Q_PROPERTY( bool firstTimeCoreRPC           READ firstTimeCoreRPC           WRITE setFirstTimeCoreRPC           NOTIFY firstTimeCoreRPCChanged)
    Q_PROPERTY( bool firstTimePassPhrase        READ firstTimePassPhrase        WRITE setFirstTimePassPhrase        NOTIFY firstTimePassPhraseChanged)
public:
    static AppSetting *instance();
    AppSetting(AppSetting &other) = delete;
    AppSetting(AppSetting const &other) = delete;
    void operator=(const AppSetting &other) = delete;

    void resetSetting();

    int unit();
    void setUnit(int unit);

    QString mainnetServer();
    void setMainnetServer(const QString &mainnetServer);

    QString testnetServer();
    void setTestnetServer(const QString &testnetServer);

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

    QString storagePath() const;
    void setStoragePath(const QString &storagePath);

    QString executePath() const;
    void setExecutePath(const QString &executePath);

    int connectionState() const;
    void setConnectionState(int connectionState);

    int changePassphraseResult() const;
    void setChangePassphraseResult(int changePassphraseResult);

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

    bool firstTimeCoreRPC();
    void setFirstTimeCoreRPC(bool firstTimeCoreRPC);

    bool firstTimePassPhrase();
    void setFirstTimePassPhrase(bool firstTimePassPhrase);

    enum class Chain : int {
        MAIN,
        TESTNET,
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

    enum class ChangePassphraseResult : int {
        NOT_YET_SET,
        INVALID_PASSPHRASE,
        PASSPHRASE_ALREADY_USED,
        CHANGE_ERROR,
        CHANGE_SUCCEED
    };

private:
    AppSetting();
    ~AppSetting();

    int unit_;
    QString mainnetServer_;
    QString testnetServer_;
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
    QString executePath_;
    int connectionState_;
    int syncPercent_;
    int changePassphraseResult_;
    bool enableCertificateFile_;
    QString certificateFile_;

    bool enableCoreRPC_;
    QString coreRPCAddress_;
    int coreRPCPort_;
    QString coreRPCName_;
    QString coreRPCPassword_;

    bool firstTimeCoreRPC_;
    bool firstTimePassPhrase_;

    // Settings
    QSettings settings_;
    void updateUnit();
signals:
    void unitChanged();
    void mainnetServerChanged();
    void testnetServerChanged();
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
};

#endif // APPSETTING_H
