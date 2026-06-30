#pragma once
#include "TypeDefine.h"
#include "core/viewmodel/ActionViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include <QJsonArray>

namespace features::inheritance::offchain::viewmodels {
using core::viewmodels::ActionViewModel;
class FirstWithdrawalDateViewModel : public ActionViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QVariant, stage)
    DEFINE_QT_PROPERTY(QString, valueTime)
    DEFINE_QT_PROPERTY(QString, valueDate)
  public:
    explicit FirstWithdrawalDateViewModel(QObject *parent = nullptr);
    DEFINE_SET_GET(QJsonArray, stagesArray)

    void onInit() override;

  public slots:
    void onContinueClicked();
};
} // namespace features::inheritance::offchain::viewmodels
