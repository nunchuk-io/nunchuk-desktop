// features/auth/ClaimingFlow.h
#pragma once
#include "features/claiming/usecases/ClaimStatusUseCase.h"
#include "features/claiming/usecases/CreateTransactionUseCase.h"
#include "features/signers/flows/KeyProceedFlow.h"
#include <QMap>

namespace features::claiming::flows {
using core::flow::FlowContext;
using features::claiming::usecases::ClaimStatusResult;
using features::claiming::usecases::ClaimStatusUseCase;
using features::claiming::usecases::CreateTransactionResult;
using features::claiming::usecases::CreateTransactionUseCase;
using features::signers::flows::KeyProceedFlow;

class ClaimingFlow : public KeyProceedFlow {
    Q_OBJECT
  public:
    explicit ClaimingFlow(FlowContext *ctx, QObject *parent = nullptr);
    QString id() const override {
        return "ClaimingFlow";
    }
    DEFINE_SET_GET(std::vector<nunchuk::SingleSigner>, backupSigners)
    DEFINE_SET_GET(std::vector<nunchuk::SingleSigner>, signers)
    DEFINE_SET_GET(nunchuk::Wallet, nunWallet)
    DEFINE_SET_GET(nunchuk::Transaction, nunTx)
    DEFINE_SET_GET(QString, magicWord)
    DEFINE_SET_GET(QStringList, tokenList)
    DEFINE_SET_GET(QString, messageId)
    DEFINE_SET_GET(int, inheritance_key_count)
    DEFINE_SET_GET(QString, bsms) // bsms != "" use for on-chain claiming
    DEFINE_SET_GET(QString, withdrawAddress)
    DEFINE_SET_GET(double, fee_rate)
    DEFINE_SET_GET(bool, anti_fee_sniping)
    DEFINE_SET_GET(bool, subtract_fee_from_amount)
    DEFINE_SET_GET(bool, isRegistered)
    DEFINE_SET_GET(int, balanceSats)
    DEFINE_SET_GET(QString, balanceDisplay)
    DEFINE_SET_GET(QString, balanceCurrency)
    DEFINE_SET_GET(QString, note)
    DEFINE_SET_GET(QString, remaining_display_name)
    DEFINE_SET_GET(qint64, withdrawAmountSats)
    DEFINE_SET_GET(bool, isAddressFlow)

    bool addSingleSigner(const nunchuk::SingleSigner &single);
    void displayStatusInfo(const ClaimStatusResult &status);
    void claimStatus();
    void createTransaction();
    void proceedClaimStatusResult(const ClaimStatusResult &status);
    virtual void proceedClaimTransactionResult(const CreateTransactionResult &txData) = 0;
    bool addAuthorization(const QString &token);

    virtual bool isManualClaiming() = 0;
  signals:
    void forwardTransaction();
  public slots:
    void bind(QObject *vm) override;

  private:
    CreateTransactionUseCase m_createTransactionUC;
    ClaimStatusUseCase m_claimStatusUC;
};
} // namespace features::claiming::flows
