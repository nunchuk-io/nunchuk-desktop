#pragma once
#include "core/flow/BaseFlow.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/transactions/usecases/LibSignTransactionUseCase.h"
#include <nunchuk.h>

namespace features::transactions::flows {
using core::flow::FlowContext;
using features::transactions::usecases::LibSignTransactionUseCase;
class TransactionFlow : public core::flow::BaseFlow {
    Q_OBJECT
  public:
    explicit TransactionFlow(FlowContext *ctx, QObject *parent = nullptr);
    QString id() const override {
        return "TransactionFlow";
    }
    DEFINE_SET_GET(QString, workFlowId)
    DEFINE_SET_GET(bool, isClaimTx)
    DEFINE_SET_GET(nunchuk::Transaction, nunTx)
    DEFINE_SET_GET(nunchuk::Wallet, nunWallet)
    bool isOffClaim();
    bool isOnClaim();
    bool isNormal();

    virtual void autoSignClaimingTransaction() {};
    virtual void proceedTransaction(QVariant feeInput) {};
    virtual void proceedTransaction(const nunchuk::Transaction &tx) {};
    virtual void proceedTransaction(const QString &psbt_signed) {};
    virtual bool isAllreadySigned() {
        return false;
    };
    virtual void manualClaimingTransaction(const nunchuk::Transaction &tx) {};
  public slots:
    void bind(QObject *vm) override;
  signals:
    void forwardTransaction();

  protected:
    LibSignTransactionUseCase m_libSignTransactionUC;
};
} // namespace features::transactions::flows
