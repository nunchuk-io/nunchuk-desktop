#pragma once
#include "TypeDefine.h"
#include "DeviceModel.h"
#include "features/signers/viewmodels/AddKeyBaseViewModel.h"
#include "features/signers/usecases/ScanDeviceUsecase.h"

namespace features::signers::viewmodels {
using features::signers::usecases::ScanDeviceUsecase;
class HardwareRefreshDevicesViewModel : public AddKeyBaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY_PTR(DeviceListModel, deviceList)
    DEFINE_QT_PROPERTY(bool, isLoading)
    // Raw HWI device-type string (e.g. "bitbox02") matching nunchuk::Device::get_type() - NOT the
    // same as hardwareTag() (e.g. "BITBOX", the SignerTag string). QML needs this one to filter
    // device rows; comparing against hardwareTag() directly was wrong for BitBox (see .cpp).
    DEFINE_QT_PROPERTY(QString, hardwareDeviceType)
  public:
    explicit HardwareRefreshDevicesViewModel(QObject *parent = nullptr);
    // BUGFIX: title used to be hardcoded to the COLDCARD copy (constructor-only) regardless of
    // which of the 4 generic wired vendors (Ledger/Trezor/Jade/BitBox) was actually selected.
    // Mirrors AddHardwareExistingKeyViewModel::initializeTextGuide()'s per-keyType() switch.
    void initializeTextGuide();
  protected:
    void onInit() override;
  public slots:
    void scanDevice();
    void requestCreateSigner();
    void checkSignerExist();
    void selectDevice(const QString xfp);
  signals:
    void notifySignerExist(bool isSoftware, const QString& fingerPrint);
  private:
    ScanDeviceUsecase m_scanDeviceUsecase;
};
} // namespace features::signers::viewmodels
