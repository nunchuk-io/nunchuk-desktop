#include "HomePendingAcceptedViewModel.h"
#include "core/ui/UiServices.inc"
#include "features/signers/flows/SetupPlatformKeyPolicyFlow.h"
#include "features/transactions/flows/DummyTransactionFlow.h"
#include "features/wallets/usecases/MarkGroupWalletAlertViewedUseCase.h"
#include "features/wallets/usecases/DismissGroupWalletAlertUseCase.h"

namespace features::wallets::viewmodels {
using namespace core::viewmodels;
using namespace features::transactions::flows;
using namespace features::signers::flows;
using namespace features::transactions::usecases;
using namespace features::wallets::usecases;

HomePendingAcceptedViewModel::HomePendingAcceptedViewModel(QObject *parent) : ActionViewModel(parent) {}

HomePendingAcceptedViewModel::~HomePendingAcceptedViewModel() {}

void HomePendingAcceptedViewModel::onInit() {}

void HomePendingAcceptedViewModel::onViewClicked(const QString &alertId) {
    GUARD_GROUP_WALLETS()
    if (auto dashboard = groupWallets->dashboardInfoPtr()) {
        dashboard->setAlertId(alertId);
        dashboard->proceedpayload();
        switch ((AlertEnum::E_Alert_t)dashboard->flow()) {
        case AlertEnum::E_Alert_t::POLICY_CHANGE_IN_PROGRESS: {
            auto alertJson = dashboard->alertJson();
            handlePolicyChangInProgress(alertJson);
        } break;
        case AlertEnum::E_Alert_t::POLICY_CHANGED: {
            auto alertJson = dashboard->alertJson();
            handlePolicyChange(alertJson);
        } break;
        case AlertEnum::E_Alert_t::REPLACE_WALLET: {
        } break;
        default:
            groupWallets->markRead(alertId);
            break;
        }
    }
}

void HomePendingAcceptedViewModel::onDismissClicked(const QString &alertId) {
    GUARD_GROUP_WALLETS()
    if (auto dashboard = groupWallets->dashboardInfoPtr()) {
        DBG_INFO << "Dismissing alert with ID: " << alertId;
        dashboard->setAlertId(alertId);
        dashboard->proceedpayload();
        switch ((AlertEnum::E_Alert_t)dashboard->flow()) {
        case AlertEnum::E_Alert_t::POLICY_CHANGED: {
            auto alertJson = dashboard->alertJson();
            handlePolicyChange(alertJson);
        } break;
        default:
            groupWallets->dismiss(alertId);
            break;
        }
    }
}

void HomePendingAcceptedViewModel::handlePolicyChange(QJsonObject alertJson) {
    DBG_INFO << "Handling policy change alert: " << alertJson;
    QString alertId = alertJson["id"].toString();
    GUARD_APP_MODEL()
    auto wallet = appModel->walletInfoPtr();
    if (!wallet) {
        DBG_ERROR << "No wallet info available in app model";
        return;
    }
    auto walletId = wallet->walletId();
    
    DismissGroupWalletAlertInput input;
    input.wallet_id = walletId;
    input.alert_id = alertId;
    
    m_dismissAlertUseCase.executeAsync(input, [this, alertId](const core::usecase::Result<DismissGroupWalletAlertResult> &result) {
        if (result.isSuccess()) {
            DBG_INFO << "Alert dismissed successfully: " << alertId;
            GUARD_GROUP_WALLETS()
            GUARD_APP_MODEL()
            if (auto dashboard = groupWallets->dashboardInfoPtr()) {
                dashboard->GetAlertsInfo();
                appModel->startReloadWallets();
            }
        } else {
            emit showToast(0, result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void HomePendingAcceptedViewModel::handlePolicyChangInProgress(QJsonObject alertJson) {
    DBG_INFO << "Handling policy change in progress alert: " << alertJson;
    QString alertId = alertJson["id"].toString();
    auto payload = alertJson["payload"].toObject();
    QString dummy_transaction_id = payload["dummy_transaction_id"].toString();
    GUARD_FLOW_MANAGER()
    GUARD_APP_MODEL()
    auto wallet = appModel->walletInfoPtr();
    if (!wallet) {
        DBG_ERROR << "No wallet info available in app model";
        return;
    }
    auto walletId = wallet->walletId();
    
    // Mark alert as viewed
    MarkGroupWalletAlertViewedInput markViewedInput;
    markViewedInput.wallet_id = walletId;
    markViewedInput.alert_id = alertId;
    
    m_markAlertViewedUseCase.executeAsync(markViewedInput, [this, walletId, dummy_transaction_id](const core::usecase::Result<MarkGroupWalletAlertViewedResult> &result) {
        if (result.isSuccess()) {
            GUARD_GROUP_WALLETS()
            GUARD_APP_MODEL()
            if (auto dashboard = groupWallets->dashboardInfoPtr()) {
                dashboard->GetAlertsInfo();
                appModel->startReloadWallets();
            }
        } else {
            DBG_ERROR << "Failed to mark alert as viewed: " << result.error();
        }
    });
    
    GetTransaction(walletId, dummy_transaction_id);
}

void HomePendingAcceptedViewModel::GetTransaction(const QString &walletId, const QString &dummyTransactionId) {
    GetGroupDummyTransactionInput input;
    input.wallet_id = walletId;
    input.dummy_transaction_id = dummyTransactionId;

    m_getTransactionUseCase.executeAsync(input, [this, walletId](const core::usecase::Result<GetGroupDummyTransactionResult> &result) {
        if (result.isSuccess()) {
            auto dummyTransaction = result.value().dummy_transaction;
            GUARD_FLOW_MANAGER()
            flowMng->stopFlow<SetupPlatformKeyPolicyFlow>();
            auto flow = flowMng->startFlow<SetupPlatformKeyPolicyFlow>();
            if (flow) {
                flow->setentryPoint(Constants::EntryPointAlert);
                flow->setwalletId(walletId);
                flow->setdummyTx(dummyTransaction);
                DBG_INFO << "Dummy transaction loaded with ID: " << dummyTransaction.get_id();
                flow->setpendingSignatures(dummyTransaction.get_pending_signatures());
                flow->checkingPolicyChangeWithDummyTransaction();
            }
        } else {
            emit showToast(0, result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

} // namespace features::wallets::viewmodels
