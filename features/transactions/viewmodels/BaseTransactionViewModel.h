#pragma once
#include "Models/TransactionModel.h"
#include "Models/WalletModel.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include <nunchuk.h>

namespace features::transactions::viewmodels {
using core::viewmodels::ActionViewModel;
class BaseTransactionViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY_PTR(Transaction, transactionInfo)
    DEFINE_QT_PROPERTY_PTR(Wallet, walletInfo)
    DEFINE_QT_PROPERTY(bool, isOffChain)
  public:
    explicit BaseTransactionViewModel(QObject *parent = nullptr);
    ~BaseTransactionViewModel();
    DEFINE_SET_GET(bool, isClaimTx)
    DEFINE_SET_GET(nunchuk::Transaction, nunTx)
    DEFINE_SET_GET(nunchuk::Wallet, nunWallet)
    DEFINE_SET_GET(QString, walletId)
    void onInit() override;
    void displayTransactionInfo();

  public slots:
    void close() override;
    virtual void proceedTransactionResult() {};
};
} // namespace features::transactions::viewmodels
