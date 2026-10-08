#include "HardwareRefreshDevicesViewModel.h"
#include "core/common/resources/AppStrings.h"
#include "core/ui/UiServices.inc"
#include "features/signers/flows/KeyProceedFlow.h"
#include "features/signers/flows/KeySetupFlow.h"
#include "generated_qml_keys.hpp"

namespace features::signers::viewmodels {
using namespace features::signers::flows;
using namespace features::signers::usecases;
const QMap<QString, QString> map_keys = {
    {"LEDGER", "ledger"}, {"TREZOR", "trezor"}, {"COLDCARD", "coldcard"}, {"BITBOX", "bitbox02"}, {"JADE", "jade"},
};
HardwareRefreshDevicesViewModel::HardwareRefreshDevicesViewModel(QObject *parent) : AddKeyBaseViewModel(parent) {
    settitle(Strings.STR_QML_911());
    setsignerName(Strings.STR_QML_1618());
    setisLoading(false);
}

void HardwareRefreshDevicesViewModel::scanDevice() {
    GUARD_CLIENT_CONTROLLER()
    ScanDeviceInput input;
    input.isLoginRequired = clientCtrl->isNunchukLoggedIn();
    input.deviceType = map_keys.value(hardwareTag(), "");
    setisLoading(true);
    DBG_INFO << "Start scanning devices..." << hardwareTag();
    m_scanDeviceUsecase.executeAsync(input, [this](core::usecase::Result<ScanDeviceResult> result) {
        if (result.isSuccess()) {
            QDeviceListModelPtr deviceList = QDeviceListModelPtr(new DeviceListModel());
            if (deviceList) {
                deviceList->clearList();
                const auto &devices = result.value().devices;
                for (const auto &device : devices) {
                    QDevicePtr devicePtr = QDevicePtr(new QDevice(device));
                    deviceList->addDevice(devicePtr);
                }
                setdeviceList(deviceList);
                auto devicePtr = deviceList->getDeviceByIndex(0);
                if (devicePtr) {
                    selectDevice(devicePtr->masterFingerPrint());
                }
            }
        } else {
            // Handle error (e.g., log it, show a message, etc.)
        }
        setisLoading(false);
    });
}

void HardwareRefreshDevicesViewModel::requestCreateSigner() {
    GUARD_SUB_SCREEN_MANAGER()
    subMng->show(qml::features::signers::qaddkeyprocessloading);
}

void HardwareRefreshDevicesViewModel::checkSignerExist() {
    if (setupOption() == FeatureOption::ClaimOffChain) {
        requestCreateSigner();
        return;
    }

    GUARD_APP_MODEL()
    QString xfpSelected = selectedXfp();
    auto masterList = appModel->masterSignerListPtr();
    auto remoteList = appModel->remoteSignerListPtr();
    if (masterList->containsFingerPrint(xfpSelected)) {
        auto oldKey = masterList->getMasterSignerByXfp(xfpSelected);
        if (oldKey->originMasterSigner().get_type() == nunchuk::SignerType::SOFTWARE) {
            emit notifySignerExist(true, xfpSelected);
        } else {
            emit notifySignerExist(false, xfpSelected);
        }
    } else if (remoteList->containsFingerPrint(xfpSelected)) {
        auto oldKey = remoteList->getSingleSignerByFingerPrint(xfpSelected);
        if (oldKey->originSingleSigner().get_type() == nunchuk::SignerType::SOFTWARE) {
            emit notifySignerExist(true, xfpSelected);
        } else {
            emit notifySignerExist(false, xfpSelected);
        }
    } else {
        requestCreateSigner();
    }
}

void HardwareRefreshDevicesViewModel::selectDevice(const QString xfp) {
    setselectedXfp(xfp);
    auto devicePtr = deviceList()->getDeviceByXfp(xfp);
    if (devicePtr) {
        GUARD_FLOW_MANAGER()
        auto flow = flowMng->startFlow<KeySetupFlow>();
        flow->setdevice(devicePtr.data()->originDevice());
    }
}

} // namespace features::signers::viewmodels
