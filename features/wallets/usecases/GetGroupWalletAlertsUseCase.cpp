#include "GetGroupWalletAlertsUseCase.h"

#include "core/bridge/ExternalBridges.h"
#include <QJsonObject>

namespace features::wallets::usecases {
using namespace core::usecase;

Result<GetGroupWalletAlertsResult> GetGroupWalletAlertsUseCase::execute(const GetGroupWalletAlertsInput &input) {
    QWarningMessage warningMsg;
    GetGroupWalletAlertsResult result;
    
    // Get total alert count
    result.total_count = bridge::GetGroupWalletAlertCount(input.wallet_id, warningMsg);
    
    if ((int)EWARNING::WarningType::NONE_MSG != warningMsg.type()) {
        return Result<GetGroupWalletAlertsResult>::failure(warningMsg.what(), warningMsg.code());
    }
    
    DBG_INFO << "Total alerts for wallet " << input.wallet_id.toStdString() << ": " << result.total_count;
    // Fetch all alerts with pagination
    const int PAGE_SIZE = 20;
    int total_pages = (result.total_count + PAGE_SIZE - 1) / PAGE_SIZE; // Ceiling division
    
    QJsonArray alertsArray;
    
    for (int page = 0; page < total_pages; ++page) {
        warningMsg.resetWarningMessage();
        std::vector<nunchuk::GroupWalletAlert> page_alerts = bridge::GetGroupWalletAlerts(input.wallet_id, page, PAGE_SIZE, warningMsg);
        
        if ((int)EWARNING::WarningType::NONE_MSG != warningMsg.type()) {
            return Result<GetGroupWalletAlertsResult>::failure(warningMsg.what(), warningMsg.code());
        }
        
        // Convert each alert to JSON
        for (const auto &alert : page_alerts) {
            QJsonObject alertJson;
            alertJson["id"] = QString::fromStdString(alert.get_id());
            if (alert.get_type() == nunchuk::GroupWalletAlertType::POLICY_CHANGE_IN_PROGRESS) {
                alertJson["type"] = "POLICY_CHANGE_IN_PROGRESS";
            }
            else if (alert.get_type() == nunchuk::GroupWalletAlertType::POLICY_CHANGED) {
                alertJson["type"] = "POLICY_CHANGED";
            }
            else if (alert.get_type() == nunchuk::GroupWalletAlertType::REPLACE_WALLET) {
                alertJson["type"] = "REPLACE_WALLET";
            }
            alertJson["status"] = alert.get_viewable() ? "UNREAD" : "READ";
            alertJson["title"] = QString::fromStdString(alert.get_title());
            alertJson["body"] = QString::fromStdString(alert.get_body());
            alertJson["viewable"] = alert.get_viewable();
            alertJson["created_time_millis"] = static_cast<qint64>(alert.get_created_at()*1000); // Convert seconds to milliseconds
            
            // Parse payload if available
            const auto &payload = alert.get_payload();
            if (payload.has_value()) {
                QJsonObject payloadJson;
                payloadJson["dummy_transaction_id"] = QString::fromStdString(payload->get_dummy_transaction_id());
                payloadJson["replacement_group_id"] = QString::fromStdString(payload->get_replacement_group_id());
                alertJson["payload"] = payloadJson;
            }
            DBG_INFO << "Fetched alert: " << alertJson;
            alertsArray.append(alertJson);
        }
    }
    
    result.alerts = alertsArray;
    return Result<GetGroupWalletAlertsResult>::success(result);
}

} // namespace features::wallets::usecases
