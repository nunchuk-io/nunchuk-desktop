#pragma once

#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/common/viewmodels/SignaturesRequiredBaseViewModel.h"

namespace features::transactions::viewmodels {
using features::common::viewmodels::SignaturesRequiredBaseViewModel;

class SignaturesRequiredViewModel : public SignaturesRequiredBaseViewModel {
    Q_OBJECT
  public:
    explicit SignaturesRequiredViewModel(QObject *parent = nullptr);
    DEFINE_SET_GET(nunchuk::GroupDummyTransaction, dummyTx)

  public slots:
    void doThisLater();
    void signDummyTransaction();
    void slotStartDummyTransaction();
};

} // namespace features::transactions::viewmodels
