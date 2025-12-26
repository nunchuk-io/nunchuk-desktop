#include "DownloadWalletUseCase.h"
#include "Servers/Draco.h"

namespace features::claiming::usecases {
using namespace core::usecase;

Result<QJsonObject> DownloadWalletUseCase::execute(const DownloadWalletInput &input) {
    QJsonObject result;
    QJsonObject body;
    body["magic"] = input.magic;
    body["keys"] = input.keys;
    bool ret = ret = Draco::instance()->inheritanceClaimDownloadWallet(body, result);
    DBG_INFO << "body:" << body << "result:" << result;
    if (ret) {
        return Result<QJsonObject>::success(result);
    } else {
        int response_code = result.value("code").toInt();
        QString error_msg = result.value("message").toString();
        return Result<QJsonObject>::failure(QString("Error %1: %2").arg(response_code).arg(error_msg));
    }
}
}