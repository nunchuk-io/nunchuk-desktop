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
    // Safe default before keyType() is known from the flow; onInit() below picks the right one.
    setheadline(Strings.STR_QML_811());
    settitle(Strings.STR_QML_824());
    setsignerName(Strings.STR_QML_1618());
    setisLoading(false);
}

// BUGFIX: this screen is shared by 4 vendors (Ledger/Trezor/Jade/BitBox) but headline/title were
// always the COLDCARD copy. Same per-keyType() pattern as
// AddHardwareExistingKeyViewModel::initializeTextGuide(). "headline" is the short screen-chrome
// title ("Add Ledger"), "title" is the longer in-content instruction ("Connect your Ledger
// device...") - same split already used by QScreenAddLedger.qml/QColdcardRefreshDevices.qml.
void HardwareRefreshDevicesViewModel::initializeTextGuide() {
    switch (static_cast<SignerKeyType>(keyType()))
    {
    case SignerKeyType::LedgerHW:
        setheadline(Strings.STR_QML_811());
        settitle(Strings.STR_QML_824());
        break;
    case SignerKeyType::TrezorHW:
        setheadline(Strings.STR_QML_814());
        settitle(Strings.STR_QML_830());
        break;
    case SignerKeyType::JadeHW:
        setheadline(Strings.STR_QML_1535());
        settitle(Strings.STR_QML_1538());
        break;
    case SignerKeyType::BitBoxHW:
        setheadline(Strings.STR_QML_923());
        settitle(Strings.STR_QML_929());
        break;
    case SignerKeyType::ColdcardHW:
        setheadline(Strings.STR_QML_904());
        settitle(Strings.STR_QML_911());
        break;
    default:
        break;
    }
}

void HardwareRefreshDevicesViewModel::onInit() {
    initializeTextGuide();
    AddKeyBaseViewModel::onInit();
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
            // BUGFIX: was a no-op stub - a scan failure (e.g. HWI not available/linked in this build)
            // silently fell through to the empty "No devices available" state with zero indication of
            // why, indistinguishable from "no device plugged in".
            emit showToast(result.code(), result.error(), EWARNING::WarningType::ERROR_MSG);
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
