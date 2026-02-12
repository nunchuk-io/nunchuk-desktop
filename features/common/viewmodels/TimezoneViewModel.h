#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "core/viewmodel/ActionViewModel.h"
#include "features/common/models/QWalletTimezoneModel.h"

namespace features::common::viewmodels {
using core::viewmodels::ActionViewModel;
using features::common::models::QWalletTimezoneModelPtr;
using features::common::models::QWalletTimezoneModel;
class TimezoneViewModel : public ActionViewModel {
    Q_OBJECT
    Q_PROPERTY(QWalletTimezoneModel *timezones READ timezones NOTIFY timezonesChanged)
  public:
    explicit TimezoneViewModel(QObject *parent = nullptr);
    ~TimezoneViewModel() override = default;

    void onInit() override;
    void onDispose() override;

    QWalletTimezoneModel *timezones() const;
    QWalletTimezoneModelPtr timezonesPtr() const;
  signals:
    void timezonesChanged();

  private:
    QWalletTimezoneModelPtr m_timezones;
};
} // namespace features::common::viewmodels
