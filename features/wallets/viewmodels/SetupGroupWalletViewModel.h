#pragma once
#include "core/viewmodel/ActionViewModel.h"

namespace features::wallets::viewmodels {
using core::viewmodels::ActionViewModel;

class SetupGroupWalletViewModel : public ActionViewModel {
    Q_OBJECT
public:
    explicit SetupGroupWalletViewModel(QObject *parent = nullptr);

public slots:
    void onPlatformKeyConfigClicked(const QString &groupid);
    void onAddKeyClicked(const QVariant& msg);
};
} // namespace features::wallets::viewmodels
