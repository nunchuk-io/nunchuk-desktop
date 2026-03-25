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
  public:
    explicit HardwareRefreshDevicesViewModel(QObject *parent = nullptr);
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
