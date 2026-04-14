#include "GeneralTransactionDetailsViewModel.h"
#include "Models/DeviceModel.h"
#include "core/ui/UiServices.inc"

namespace features::transactions::viewmodels {
using namespace features::signers::usecases;
using namespace features::transactions::usecases;

GeneralTransactionDetailsViewModel::GeneralTransactionDetailsViewModel(QObject *parent) : BaseTransactionViewModel(parent) {}

void GeneralTransactionDetailsViewModel::verifyAddress(const QString &address) {
    GUARD_CLIENT_CONTROLLER()
    AddressToVerifyInput input;
    input.isLoginRequired = clientCtrl->isNunchukLoggedIn();
    input.address = address;
    input.wallet_id = walletId();
    m_addressToVerifyUC.executeAsync(input, [this](const core::usecase::Result<AddressToVerifyResult> &result) {
        if (result.isSuccess()) {
            QString msg = QString("Address successfully verified");
            emit showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
        } else {
            QString msg = QString("Address verification failed");
            emit showToast(0, msg, EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void GeneralTransactionDetailsViewModel::scanDevice() {
    GUARD_CLIENT_CONTROLLER()
    ScanDeviceInput input;
    input.isLoginRequired = clientCtrl->isNunchukLoggedIn();
    input.deviceType = "";
    DBG_INFO << "Start scanning devices...";
    qApp->setOverrideCursor(Qt::WaitCursor);
    m_scanDeviceUC.executeAsync(input, [this](core::usecase::Result<ScanDeviceResult> result) {
        qApp->restoreOverrideCursor();
        if (result.isSuccess()) {
            QDeviceListModelPtr deviceList = QDeviceListModelPtr(new DeviceListModel());
            if (deviceList) {
                deviceList->clearList();
                const auto &devices = result.value().devices;
                for (const auto &device : devices) {
                    QDevicePtr devicePtr = QDevicePtr(new QDevice(device));
                    deviceList->addDevice(devicePtr);
                }
                GUARD_APP_MODEL()
                appModel->setDeviceList(deviceList);
                transactionInfo()->singleSignersAssigned()->requestSort();
                transactionInfo()->refreshScanDevices();
            }
        } else {
            emit showToast(0, result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

} // namespace features::transactions::viewmodels
