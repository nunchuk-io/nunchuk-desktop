#include "RefreshDevicesViewModel.h"
#include "localization/STR_CPP.h"
#include "core/ui/UiServices.inc"

namespace features::signers::viewmodels {
using namespace core::viewmodels;
using namespace features::signers::usecases;

const QMap<QString, QString> map_keys = {
    {"LEDGER", "ledger"},
    {"TREZOR", "trezor"},
    {"COLDCARD", "coldcard"},
    {"BITBOX", "bitbox02"},
    {"JADE", "jade"},
};

RefreshDevicesViewModel::RefreshDevicesViewModel(QObject *parent) 
: BaseViewModel(parent) {
    setisLoading(false);
}

void RefreshDevicesViewModel::forceCreateMaster(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString key_name = maps.value("signerNameInputted").toString();
    QString xfpSelected    = maps.value("xfpSelected").toString();
    QDevicePtr selectedDv = deviceList()->getDeviceByXfp(xfpSelected);
    if (selectedDv) {
        if (selectedDv.data()->needsPinSent() || selectedDv.data()->needsPassPhraseSent()) {
            emit showToast(0, STR_CPP_095, EWARNING::WarningType::WARNING_MSG);
        } else {
            ctx()->appModel()->startCreateMasterSigner(key_name, xfpSelected);
            DBG_INFO << key_name << xfpSelected;
        }
    }
}

void RefreshDevicesViewModel::scanDevice() {
    GUARD_CLIENT_CONTROLLER()
    ScanDeviceInput input;
    input.isLoginRequired = clientCtrl->isNunchukLoggedIn();
    input.deviceType = map_keys.value(m_tag, "");
    setisLoading(true);
    DBG_INFO << "Start scanning devices..." << m_tag;
    m_scanDeviceUsecase.executeAsync(input, [this](core::usecase::Result<ScanDeviceResult> result) {
        if (result.isSuccess()) {
            QDeviceListModelPtr deviceListModel = QDeviceListModelPtr(new DeviceListModel());
            if (deviceListModel) {
                deviceListModel->clearList();
                const auto &devices = result.value().devices;
                for (const auto &device : devices) {
                    QDevicePtr devicePtr = QDevicePtr(new QDevice(device));
                    deviceListModel->addDevice(devicePtr);
                }
                setdeviceList(deviceListModel);
            }
        } else {
            // Handle error (e.g., log it, show a message, etc.)
        }
        setisLoading(false);
    });
}

bool RefreshDevicesViewModel::startCreateMaster(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString xfpSelected    = maps.value("xfpSelected").toString();
    auto masterList = ctx()->appModel()->masterSignerListPtr();
    auto remoteList = ctx()->appModel()->remoteSignerListPtr();
    if (masterList->containsFingerPrint(xfpSelected)) {
        auto oldKey = masterList->getMasterSignerByXfp(xfpSelected);
        if (oldKey->originMasterSigner().get_type() == nunchuk::SignerType::SOFTWARE) {
            emit notifySignerExist(true, xfpSelected);
        } else {
            emit notifySignerExist(false, xfpSelected);
        }
        return false;
    } 
    else if (remoteList->containsFingerPrint(xfpSelected)) {
        auto oldKey = remoteList->getSingleSignerByFingerPrint(xfpSelected);
        if (oldKey->originSingleSigner().get_type() == nunchuk::SignerType::SOFTWARE) {
            emit notifySignerExist(true, xfpSelected);
        } else {
            emit notifySignerExist(false, xfpSelected);
        }
        return false;
    }
    else {
        forceCreateMaster(msg);
        return true;
    }
}

} // namespace features::signers::viewmodels
