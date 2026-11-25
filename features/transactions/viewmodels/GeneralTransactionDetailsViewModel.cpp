#include "GeneralTransactionDetailsViewModel.h"
#include "Models/DeviceModel.h"
#include "core/ui/UiServices.inc"

namespace features::transactions::viewmodels {
using namespace features::signers::usecases;
using namespace features::transactions::usecases;

GeneralTransactionDetailsViewModel::GeneralTransactionDetailsViewModel(QObject *parent) : BaseTransactionViewModel(parent) {}

GeneralTransactionDetailsViewModel::~GeneralTransactionDetailsViewModel() {
    if (m_isScanning) {
        m_isScanning = false;
        qApp->restoreOverrideCursor();
    }
}

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
            QString msg = result.error().isEmpty() ? QString("Address verification failed") : result.error();
            emit showToast(0, msg, EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void GeneralTransactionDetailsViewModel::scanDevice() {
    GUARD_CLIENT_CONTROLLER()
    ScanDeviceInput input;
    input.isLoginRequired = clientCtrl->isNunchukLoggedIn();
    input.deviceType = "";
    if (m_isScanning) {
        DBG_INFO << "Scan already in progress";
        return;
    }
    DBG_INFO << "Start scanning devices...";
    m_isScanning = true;
    qApp->setOverrideCursor(Qt::WaitCursor);
    m_scanDeviceUC.executeAsync(input, [this](core::usecase::Result<ScanDeviceResult> result) {
        if (m_isScanning) {
            m_isScanning = false;
            qApp->restoreOverrideCursor();
        }
        DBG_INFO << "Scan finished:" << result.isSuccess() << result.error();
        if (result.isSuccess()) {
            QDeviceListModelPtr deviceList = QDeviceListModelPtr(new DeviceListModel());
            if (deviceList) {
                deviceList->clearList();
                const auto &devices = result.value().devices;
                DBG_INFO << "Scanned devices:" << devices.size();
                for (const auto &device : devices) {
                    DBG_INFO << "device type:" << QString::fromStdString(device.get_type()) << "model:" << QString::fromStdString(device.get_model())
                             << "xfp:" << QString::fromStdString(device.get_master_fingerprint()) << "needs_pin:" << device.needs_pin_sent()
                             << "needs_passphrase:" << device.needs_pass_phrase_sent() << "connected:" << device.connected();
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
