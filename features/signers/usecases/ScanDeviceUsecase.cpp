#include "ScanDeviceUsecase.h"
#include "qUtils.h"
#include "ifaces/bridgeifaces.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<ScanDeviceResult> ScanDeviceUsecase::execute(const ScanDeviceInput &input) {
    std::vector<nunchuk::Device> deviceList_result {};
    ScanDeviceResult result;
    QWarningMessage msg;
    if (input.isLoginRequired) {
        deviceList_result = bridge::nunchukGetOriginDevices(msg);
    } else {
        deviceList_result = qUtils::GetDevices(bridge::hwiPath(), msg);
    }
    if (!input.deviceType.isEmpty()) {
        std::vector<nunchuk::Device> filteredDevices;
        for (const auto &device : deviceList_result) {
            if (QString::fromStdString(device.get_type()).toUpper() == input.deviceType.toUpper()) {
                filteredDevices.push_back(device);
            }
        }
        result.devices = filteredDevices;
    } else {
        result.devices = deviceList_result;
    }
    if (msg.isSuccess()) {
        return Result<ScanDeviceResult>::success(result);
    } else {
        return Result<ScanDeviceResult>::failure(msg.contentDisplay());
    }
}
}