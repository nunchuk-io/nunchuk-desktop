#pragma once
#include "TypeDefine.h"
#include "core/viewmodel/BaseViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "DeviceModel.h"
#include "features/signers/usecases/ScanDeviceUsecase.h"

namespace features::signers::viewmodels {
using core::viewmodels::BaseViewModel;
using features::signers::usecases::ScanDeviceUsecase;
class RefreshDevicesViewModel : public BaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY_PTR(DeviceListModel, deviceList)
    DEFINE_QT_PROPERTY(QString, tag)
    DEFINE_QT_PROPERTY(bool, isLoading)
  public:
    explicit RefreshDevicesViewModel(QObject *parent = nullptr);
  public slots:
    void forceCreateMaster(QVariant msg);
    void scanDevice();
    bool startCreateMaster(QVariant msg);
  signals:
    void notifySignerExist(bool isSoftware, const QString fingerPrint);
  private:
    ScanDeviceUsecase m_scanDeviceUsecase;
};
} // namespace features::signers::viewmodels
