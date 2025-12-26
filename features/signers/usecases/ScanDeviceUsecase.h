#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::signers::usecases {

#define FIELDS_INPUT(X)       \
    X(bool, isLoginRequired) \
    X(QString, deviceType)

DEFINE_USECASE_INPUT(ScanDeviceInput)


#define FIELDS_RESULT(X)       \
    X(std::vector<nunchuk::Device>, devices)

DEFINE_USECASE_RESULT(ScanDeviceResult)

class ScanDeviceUsecase : public core::usecase::AsyncUseCase<ScanDeviceUsecase, ScanDeviceInput, ScanDeviceResult> {
  protected:
  core::usecase::Result<ScanDeviceResult> execute(const ScanDeviceInput &input) override;
};

} // namespace features::signers::usecases