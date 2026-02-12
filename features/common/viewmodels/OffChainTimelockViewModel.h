#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::common::viewmodels {
using core::viewmodels::ActionViewModel;
class OffChainTimelockViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit OffChainTimelockViewModel(QObject *parent = nullptr);
    ~OffChainTimelockViewModel() override = default;

    DEFINE_QT_PROPERTY(QString, valueTime)
    DEFINE_QT_PROPERTY(QString, valueDate)
    DEFINE_QT_PROPERTY(QString, valueTimezone)
    DEFINE_QT_PROPERTY(bool, isCurrentTime)
  public:
    void clearTimeLock(QJsonObject timelock);
  signals:
    void invalidDateAlert();

  private:
    QWalletTimezoneModelPtr m_timezones;
};
} // namespace features::common::viewmodels
