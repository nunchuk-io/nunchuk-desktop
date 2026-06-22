#pragma once
#include "TypeDefine.h"
#include "features/common/viewmodels/SignaturesRequiredBaseViewModel.h"

namespace features::inheritance::offchain::viewmodels {
using features::common::viewmodels::SignaturesRequiredBaseViewModel;
class SignaturesRequiredViewModel : public SignaturesRequiredBaseViewModel {
    Q_OBJECT
  public:
    explicit SignaturesRequiredViewModel(QObject *parent = nullptr);

    void onInit() override;

  private:
};
} // namespace features::inheritance::offchain::viewmodels
