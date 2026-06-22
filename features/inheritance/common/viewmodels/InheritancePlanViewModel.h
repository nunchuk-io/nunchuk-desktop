#pragma once
#include "core/viewmodel/BaseViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::inheritance::common::viewmodels {
using core::viewmodels::BaseViewModel;

class InheritancePlanViewModel : public BaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, distribution_method)
    DEFINE_QT_PROPERTY(QString, beneficiary_mode)
    DEFINE_QT_PROPERTY(QString, buffer_apply_on)
    DEFINE_QT_PROPERTY(bool, is_buffer_apply_on_changed)
    DEFINE_QT_PROPERTY(QVariant, buffer_period)
    DEFINE_QT_PROPERTY(bool, is_buffer_period_changed)
    DEFINE_QT_PROPERTY(QString, title)
    DEFINE_QT_PROPERTY(QString, walletName)
    DEFINE_QT_PROPERTY(QVariant, requester_user)
    DEFINE_QT_PROPERTY(QString, old_activation_date)
    DEFINE_QT_PROPERTY(QString, new_activation_date)
    DEFINE_QT_PROPERTY(QString, activation_date)
    DEFINE_QT_PROPERTY(int, pending_signatures)
    DEFINE_QT_PROPERTY(QString, timelockDisplay)
    DEFINE_QT_PROPERTY(QString, timezoneDisplay)
    DEFINE_QT_PROPERTY(bool, isTimezoneDisplayChanged)
    DEFINE_QT_PROPERTY(bool, isOnChain)
    DEFINE_QT_PROPERTY(QString, activation_timezone)
    DEFINE_QT_PROPERTY(QString, note)
    DEFINE_QT_PROPERTY(bool, is_note_changed)
    DEFINE_QT_PROPERTY(QVariant, notification_preferences)
    DEFINE_QT_PROPERTY(QString, release_method)
    DEFINE_QT_PROPERTY(bool, is_release_method_changed)
    DEFINE_QT_PROPERTY(QVariantList, beneficiariesData)
    DEFINE_QT_PROPERTY(QVariantList, assetAllocation)
    DEFINE_QT_PROPERTY(QVariant, fallback_policy)
    DEFINE_QT_PROPERTY(int, inactivity_interval_count)
    DEFINE_QT_PROPERTY(bool, isInactivityIntervalCountChanged)
    DEFINE_QT_PROPERTY(QString, display_emails)
    DEFINE_QT_PROPERTY(bool, isDisplay_emailsChanged)
    DEFINE_QT_PROPERTY(bool, notifyEnabled)
    DEFINE_QT_PROPERTY(bool, isNotifyEnabledChanged)
    DEFINE_QT_PROPERTY(QString, firstWithdrawalDate)
    DEFINE_QT_PROPERTY(bool, isFirstWithdrawalDateChanged)
  public:
    explicit InheritancePlanViewModel(QObject *parent = nullptr);

    void onInit() override;
    void processAlertData();
    void processWalletData();
    void processTimeData();

  public slots:
    void slotUpdateInheritancePlan();
    void onSignDummyTxClicked();
};
} // namespace features::inheritance::common::viewmodels
