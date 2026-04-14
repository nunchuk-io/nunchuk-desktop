#pragma once

#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::transactions::viewmodels {
using core::viewmodels::ActionViewModel;

class SignaturesRequiredViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, amountLimit)
    DEFINE_QT_PROPERTY(int, pending_signatures)
    DEFINE_QT_PROPERTY(QString, describeOne)
    DEFINE_QT_PROPERTY(QString, describeTwo)
  public:
    explicit SignaturesRequiredViewModel(QObject *parent = nullptr);
    DEFINE_SET_GET(nunchuk::GroupDummyTransaction, dummyTx)

  public slots:
    void doThisLater();
    void signDummyTransaction();
    void slotStartDummyTransaction();
};

} // namespace features::transactions::viewmodels
