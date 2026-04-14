#pragma once

#include "features/transactions/flows/TransactionFlow.h"

namespace features::transactions::flows {
using core::flow::FlowContext;

class DummyTransactionFlow : public TransactionFlow {
    Q_OBJECT
  public:
    explicit DummyTransactionFlow(FlowContext *ctx, QObject *parent = nullptr);

    QString id() const override {
        return "DummyTransactionFlow";
    }

    DEFINE_SET_GET(QString, amountLimit)
    DEFINE_SET_GET(int, pendingSignatures)
    DEFINE_SET_GET(nunchuk::GroupDummyTransaction, dummyTx)

  public slots:
    void bind(QObject *vm) override;

  private:
};

} // namespace features::transactions::flows