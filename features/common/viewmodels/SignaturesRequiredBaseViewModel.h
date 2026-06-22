#pragma once
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::common::viewmodels {
using core::viewmodels::ActionViewModel;

class SignaturesRequiredBaseViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, amountLimit)
    DEFINE_QT_PROPERTY(int, pending_signatures)
    DEFINE_QT_PROPERTY(QString, describeOne)
    DEFINE_QT_PROPERTY(QString, describeTwo)

  public:
    explicit SignaturesRequiredBaseViewModel(QObject *parent = nullptr);
    virtual ~SignaturesRequiredBaseViewModel() = default;

  protected:
    virtual void onInit() override;

  private:
};
} // namespace features::common::viewmodels
