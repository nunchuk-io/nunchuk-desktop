#pragma once
#include "TypeDefine.h"
#include "WalletModel.h"
#include "core/viewmodel/LoadingViewModel.h"
#include "DeviceModel.h"
#include "features/signers/usecases/ScanDeviceUsecase.h"

namespace features::signers::viewmodels {
using core::viewmodels::LoadingViewModel;
using features::signers::usecases::ScanDeviceUsecase;
class RefreshDevicesViewModel : public LoadingViewModel {
    Q_OBJECT
    Q_PROPERTY(DeviceListModel *deviceList READ deviceList NOTIFY deviceListChanged)
    Q_PROPERTY(QString tag READ tag WRITE setTag NOTIFY tagChanged)
  public:
    explicit RefreshDevicesViewModel(QObject *parent = nullptr);
    ~RefreshDevicesViewModel() override = default;
    DeviceListModel *deviceList() const;
    QString tag() const;
    void setTag(const QString &tag);
  public slots:
    void forceCreateMaster(QVariant msg);
    void scanDevice();
    bool startCreateMaster(QVariant msg);
  signals:
    void tagChanged();
    void deviceListChanged();
    void notifySignerExist(bool isSoftware, const QString fingerPrint);
  private:
    QDeviceListModelPtr m_deviceList;
    ScanDeviceUsecase m_scanDeviceUsecase;
    QString m_tag{};
};
} // namespace features::signers::viewmodels
