#include "AddressToVerifyUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<AddressToVerifyResult> AddressToVerifyUseCase::execute(const AddressToVerifyInput &input) {
    QWarningMessage msg;
    std::vector<nunchuk::Device> deviceList_result{};
    if (input.isLoginRequired) {
        deviceList_result = bridge::nunchukGetOriginDevices(msg);
    } else {
        deviceList_result = qUtils::GetDevices(bridge::hwiPath(), msg);
    }
    QStringList xfps;
    for (const auto &device : deviceList_result) {
        xfps.append(QString::fromStdString(device.get_master_fingerprint()));
    }
    foreach (QString xfp, xfps) {
        msg.resetWarningMessage();
        bridge::nunchukDisplayAddressOnDevice(input.wallet_id, input.address, xfp, msg);
    }

    AddressToVerifyResult result;
    if (msg.isSuccess()) {
        return Result<AddressToVerifyResult>::success(result);
    } else {
        return Result<AddressToVerifyResult>::failure(msg.contentDisplay());
    }
}

} // namespace features::transactions::usecases
