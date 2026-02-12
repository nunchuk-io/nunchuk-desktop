#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/common/usecases/TimeLockConvertUseCase.h"

namespace features::common::viewmodels {
using core::viewmodels::ActionViewModel;
using features::common::usecases::TimeLockConvertUseCase;
class OnChainTimelockViewModel : public ActionViewModel {
    Q_OBJECT
  public:
    explicit OnChainTimelockViewModel(QObject *parent = nullptr);
    ~OnChainTimelockViewModel() override = default;

    DEFINE_QT_PROPERTY(QString, valueTime)
    DEFINE_QT_PROPERTY(QString, valueDate)
    DEFINE_QT_PROPERTY(QString, valueTimezone)
    DEFINE_QT_PROPERTY(QString, based)
    DEFINE_QT_PROPERTY(int, blockHeight)
    DEFINE_QT_PROPERTY(bool, isShowBlockHeight)
    DEFINE_QT_PROPERTY(bool, isInit)
    DEFINE_QT_PROPERTY(int, xYearsLock)
  public:
    void clearTimeLock(QJsonObject timelock);
  public slots:
    void slotTimeChanged();
  signals:
    void invalidDateAlert();
  private:
    TimeLockConvertUseCase m_timeLockConvertUC;
};
} // namespace features::common::viewmodels
