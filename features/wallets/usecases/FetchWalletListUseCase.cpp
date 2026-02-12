#include "FetchWalletListUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::wallets::usecases {
using namespace core::usecase;

Result<FetchWalletListResult> FetchWalletListUseCase::execute(const FetchWalletListInput &input) {
    QJsonObject dataAssisted, dataGroup, dataClaimed;
    QString error_msg;
    Draco::instance()->getAssistedWallets(dataAssisted, error_msg);
    Byzantine::instance()->GetListGroupWallets(dataGroup, error_msg);
    Draco::instance()->ClaimingWalletList(dataClaimed, error_msg);
    QJsonArray walletList;
    if(!dataAssisted.isEmpty()){
        QJsonArray assisted_wallets = dataAssisted["wallets"].toArray();
        for (const QJsonValue& val : assisted_wallets) {
            walletList.append(val);
        }
    }
    if(!dataGroup.isEmpty()){
        QJsonArray group_wallets = dataGroup["wallets"].toArray();
        for (const QJsonValue& val : group_wallets) {
            QJsonObject walletObj = val.toObject();
            QString status = walletObj["status"].toString();
            QString group_id = walletObj["group_id"].toString();
            if (status == "ACTIVE" || status == "LOCKED") {
                QJsonObject output;
                QString error_msg = "";
                bool ret = Byzantine::instance()->GetOneGroupWallets(group_id, output, error_msg);
                walletObj["role"] = "";
                if (ret) {
                    QJsonObject group = output["group"].toObject();
                    walletObj["role"] = group["role"].toString();
                    walletObj["group"] = group;
                    QString groupStatus = group["status"].toString();
                    if (groupStatus != "ACTIVE") {
                        continue;
                    }
                }
            }
            walletList.append(walletObj);
        }
    }
    if(!dataClaimed.isEmpty()){
        QJsonArray claimed_wallets = dataClaimed.value("wallets").toArray();
        for (const QJsonValue& val : claimed_wallets) {
            auto walletObj = val.toObject();
            walletObj["is_claimed"] = true;
            walletList.append(walletObj);
        }
    }

    FetchWalletListResult result;
    result.walletList = walletList;
    if (walletList.size() > 0) {
        return Result<FetchWalletListResult>::success(result);
    } else {
        return Result<FetchWalletListResult>::failure(error_msg);
    }
}

} // namespace features::wallets::usecases