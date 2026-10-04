#include "ScanDeviceUsecase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/utils/Utils.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<ScanDeviceResult> ScanDeviceUsecase::execute(const ScanDeviceInput &input) {
    std::vector<nunchuk::Device> deviceList_result {};
    ScanDeviceResult result;
    QWarningMessage msg;
    // BUGFIX: used to branch on input.isLoginRequired and call bridge::nunchukGetOriginDevices()
    // (the shared, session-lifetime HWIService singleton) when logged in - that's the same shared
    // child-process handle nunchuckiface.cpp flags as the source of
    // "[-4099] Wait error: No child processes" under repeated scans. Always use qUtils::GetDevices()
    // instead: it builds a fresh, disposable HWIService per call, so it never shares state with any
    // other in-flight HWI call (scan, sign, health-check...) - same approach the pending-wallet
    // refresh path (Worker::scanDevices) already used, which never hit this error.
    deviceList_result = qUtils::GetDevices(bridge::hwiCommand(), msg);
    if (!input.deviceType.isEmpty()) {
        std::vector<nunchuk::Device> filteredDevices;
        for (const auto &device : deviceList_result) {
            // BUGFIX: exact match silently drops a device whose HWI-reported type carries any
            // variant suffix we don't know about (e.g. a future/alternate model string) - contains()
            // is a safer match here since none of the known device-type tags (ledger/trezor/jade/
            // coldcard/bitbox02) are substrings of one another, so this can't cross-match vendors.
            if (QString::fromStdString(device.get_type()).toUpper().contains(input.deviceType.toUpper())) {
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
