#pragma once
#include "TypeDefine.h"
#include "core/enums/EnumMacros.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

DECLARE_QML_ENUM(features::inheritance::offchain::viewmodels, FallbackOption, FallbackFirst, FallbackSecond, None)
namespace features::inheritance::offchain::viewmodels {
using core::viewmodels::ActionViewModel;
class FallbackSettingsViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, valueDate)
    DEFINE_QT_PROPERTY(QString, selectedFallback)
    DEFINE_QT_PROPERTY(QString, inactivityUnit)
    DEFINE_QT_PROPERTY(int, fallbackOption)
    DEFINE_QT_PROPERTY(int, repeatEvery)
  public:
    explicit FallbackSettingsViewModel(QObject *parent = nullptr);

    void onInit() override;

  public slots:
    void slotValueDateChanged();
    void onContinueClicked();
};
} // namespace features::inheritance::offchain::viewmodels
