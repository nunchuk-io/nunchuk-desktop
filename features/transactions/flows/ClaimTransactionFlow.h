#pragma once
#include "core/flow/BaseFlow.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/claiming/usecases/ClaimInheritanceUseCase.h"
#include "features/claiming/usecases/CreateTransactionUseCase.h"
#include "features/transactions/flows/TransactionFlow.h"

namespace features::transactions::flows {
using core::flow::FlowContext;
using features::claiming::usecases::ClaimInheritanceResult;
using features::claiming::usecases::ClaimInheritanceUseCase;
using features::claiming::usecases::CreateTransactionResult;
using features::claiming::usecases::CreateTransactionUseCase;
using features::transactions::flows::TransactionFlow;

class ClaimTransactionFlow : public TransactionFlow {
    Q_OBJECT
  public:
    explicit ClaimTransactionFlow(FlowContext *ctx, QObject *parent = nullptr);
    QString id() const override {
        return "ClaimTransactionFlow";
    }
    DEFINE_SET_GET(QString, magicWord)
    DEFINE_SET_GET(QStringList, tokenList)
    DEFINE_SET_GET(QString, messageId)
    DEFINE_SET_GET(QString, bsms) // bsms != "" use for on-chain claiming
    DEFINE_SET_GET(QString, withdrawAddress)
    DEFINE_SET_GET(double, fee_rate)
    DEFINE_SET_GET(bool, anti_fee_sniping)
    DEFINE_SET_GET(bool, subtract_fee_from_amount)
    DEFINE_SET_GET(qint64, withdrawAmountSats)
    DEFINE_SET_GET(CreateTransactionResult, txResult)
    DEFINE_SET_GET(std::vector<nunchuk::SingleSigner>, backupSigners)

    void autoSignClaimingTransaction() override;
    void proceedTransaction(QVariant feeInput) override;
    void proceedTransaction(const nunchuk::Transaction &tx) override;
    void proceedTransaction(const QString &psbt_signed) override;
    void proceedClaimTransactionResult(const CreateTransactionResult &txData);
    bool isAllreadySigned() override;
    void requestClaimInheritance();
    void manualClaimingTransaction(const nunchuk::Transaction &tx);
    void proceedClaimingResult(const ClaimInheritanceResult &claim);
  public slots:
    void bind(QObject *vm) override;

  private:
    CreateTransactionUseCase m_createTransactionUC;
    ClaimInheritanceUseCase m_claimInheritanceUC;
};
} // namespace features::transactions::flows
