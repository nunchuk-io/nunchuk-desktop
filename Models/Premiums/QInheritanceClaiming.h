#ifndef QINHERITANCECLAIMING_H
#define QINHERITANCECLAIMING_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QStringList>
#include <QMap>
#include "Commons/QStateFlow.h"
#include "TransactionModel.h"
#include "ServiceSetting.h"
#include "WalletModel.h"
#include "features/claiming/usecases/InitializeInheritanceClaimingUseCase.h"

// Forward declarations for external types used
namespace nunchuk {
    class Wallet;
    class Transaction;
    class SingleSigner;
    class MasterSigner;
}

using features::claiming::usecases::InitializeInheritanceClaimingResult;

class QInheritanceClaiming  : public QStateFlow
{
    Q_OBJECT
    // Claim-related properties moved from QWalletServicesTag
    Q_PROPERTY(QVariant inheritanceInfo READ inheritanceInfo NOTIFY inheritanceInfoChanged)
    Q_PROPERTY(QVariant inheritanceCheckStatus READ inheritanceCheckStatus NOTIFY inheritanceCheckStatusChanged)
    Q_PROPERTY(QVariant inheritanceClaimPlan READ inheritanceClaimPlan NOTIFY inheritanceClaimPlanChanged)
    Q_PROPERTY(qint64 claimInheritanceCustomAmount READ claimInheritanceCustomAmount NOTIFY claimInheritanceCustomAmountChanged)
public:
    struct inheritance_t {
        QString m_destinationAddress = {};
        nunchuk::Wallet wallet = {};
        nunchuk::Transaction tx = {};
    };

    QInheritanceClaiming();
    ~QInheritanceClaiming();

    // Claim logic exposed to QML
    Q_INVOKABLE void clearClaimAnInheritance();
    Q_INVOKABLE bool inheritanceCheck(const QString &magic = "", const QString &environment = "PRODUCTION");
    Q_INVOKABLE int inheritanceDownloadBackup(const QString &magic, const QString &backup_password, const QString &backup_password_two);
    Q_INVOKABLE bool inheritanceClaimRequest(const nunchuk::Wallet wallet, const nunchuk::Transaction txSigned, const QString &magic);
    Q_INVOKABLE bool inheritanceClaimStatus(const QString &magic);
    Q_INVOKABLE bool inheritanceCreateTx(const QJsonObject &data, const QStringList &authos);
    Q_INVOKABLE void setInheritanceAddress(const QString &to_wallet_id);
    Q_INVOKABLE void setInheritanceAddressNewTransaction(const QString &address);
    Q_INVOKABLE bool inheritanceCreateDraftTransaction(double fee_rate = 1000.0, bool anti_fee_sniping = false, bool subtract_fee_from_amount = false);
    Q_INVOKABLE bool inheritanceSignTransaction();
    bool inheritanceUpdateLibTransaction();

    QVariant inheritanceInfo() const;

    qint64 claimInheritanceCustomAmount() const;
    Q_INVOKABLE void setClaimInheritanceCustomAmount(qint64 amountSats);

    QVariant inheritanceCheckStatus() const;
    Q_INVOKABLE void updateInheritanceCheckStatus(const QJsonObject &status);
    Q_INVOKABLE void updateInheritanceCheckStatus(const QString &key, const QVariant &value);
    Q_INVOKABLE void clearInheritanceCheckStatus();

    QVariant inheritanceClaimPlan() const;
    Q_INVOKABLE void setInheritanceClaimPlan(const QJsonObject &claim);

    Q_INVOKABLE void clearBufferPeriodCountdown();
    Q_INVOKABLE void clearInheritance();

    // Expose raw json objects when needed
    QJsonObject inheritanceClaimPlanJs() const { return m_inheritanceClaimPlan; }
    QJsonObject inheritanceCheckStatusJs() const { return m_inheritanceCheckStatus; }

    void clearInheritanceKeys();
    void setClaimInit(const InitializeInheritanceClaimingResult& init);
    void claimInitSaveBsms(const QString& bsms, const nunchuk::Wallet& wallet);
    nunchuk::SingleSigner RecoverAnExistingSeed(const QString &mnemonic);
    void onChainProcessErrorCode(int error_code, const QString &error_msg);
public slots:
    QJsonArray inheritanceKeys() const;
    bool requestDownloadWallet();
    bool requestClaimInheritanceFromSigner(const nunchuk::SingleSigner &signer);

    bool requestDownloadWalletViaSeedPhrase(const QString &seedPhrase);
    bool requestDownloadWalletWithIndexAsync(const QString &xfp);
    bool requestDownloadWalletViaQR(const QStringList &qr_data);
    bool requestDownloadWalletViaImportFile(const QString &fileName);
    
signals:
    // Signals mirrored from QWalletServicesTag for claim logic
    void notPaidAlert();
    void isExpiredAlert();
    void comeBackLaterAlert(const QString &errormsg);
    void securityDepositRequiredAlert(const QString &errormsg);
    void hasNotBeenActivatedYetAlert();

    void inheritanceInfoChanged();
    void inheritanceCheckStatusChanged();
    void inheritanceClaimPlanChanged();
    void claimInheritanceCustomAmountChanged();
    void resultClaimInheritanceAlert(const QString &result);
private:
    QJsonObject m_inheritanceCheckStatus;
    QJsonObject m_inheritanceClaimPlan;
    int64_t m_claimInheritanceCustomAmount {0};

    inheritance_t mInheritance = {};
    std::vector<nunchuk::SingleSigner> single_signers;
    std::vector<nunchuk::MasterSigner> master_signers;
    InitializeInheritanceClaimingResult m_claimInit {};
    QJsonArray m_inheritanceKeys{};
    QJsonObject m_walletInfoClaim{};
    QJsonObject m_claimWallet{};
    QJsonObject m_claimTransaction{};
};
#endif // QINHERITANCECLAIMING_H
