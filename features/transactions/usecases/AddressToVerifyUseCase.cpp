#include "AddressToVerifyUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"
#include <QSet>

namespace features::transactions::usecases {
using namespace core::usecase;

Result<AddressToVerifyResult> AddressToVerifyUseCase::execute(const AddressToVerifyInput &input) {
    QWarningMessage msg;
    std::vector<nunchuk::Device> deviceList_result{};
    if (input.isLoginRequired) {
        deviceList_result = bridge::nunchukGetOriginDevices(msg);
    } else {
        deviceList_result = qUtils::GetDevices(bridge::hwiCommand(), msg);
    }
    if (!msg.isSuccess()) {
        return Result<AddressToVerifyResult>::failure(msg.contentDisplay());
    }

    // Chỉ verify trên device là signer của wallet
    QWarningMessage walletMsg;
    const auto wallet = bridge::nunchukGetOriginWallet(input.wallet_id, walletMsg);
    if (!walletMsg.isSuccess()) {
        return Result<AddressToVerifyResult>::failure(walletMsg.contentDisplay());
    }
    QSet<QString> signerXfps;
    for (const auto &signer : wallet.get_signers()) {
        signerXfps.insert(QString::fromStdString(signer.get_master_fingerprint()));
    }
    QStringList xfps;
    for (const auto &device : deviceList_result) {
        const QString xfp = QString::fromStdString(device.get_master_fingerprint());
        if (signerXfps.contains(xfp)) {
            xfps.append(xfp);
        }
    }
    if (xfps.isEmpty()) {
        return Result<AddressToVerifyResult>::failure(QString("No connected device matches this wallet"));
    }

    // Lỗi của bất kỳ device nào đều tính là thất bại
    QString firstError;
    for (const QString &xfp : xfps) {
        QWarningMessage devMsg;
        bridge::nunchukDisplayAddressOnDevice(input.wallet_id, input.address, xfp, devMsg);
        if (!devMsg.isSuccess() && firstError.isEmpty()) {
            firstError = devMsg.contentDisplay();
        }
    }
    if (!firstError.isEmpty()) {
        return Result<AddressToVerifyResult>::failure(firstError);
    }
    return Result<AddressToVerifyResult>::success(AddressToVerifyResult{});
}

} // namespace features::transactions::usecases
