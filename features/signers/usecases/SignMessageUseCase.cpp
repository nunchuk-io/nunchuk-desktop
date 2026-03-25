#include "SignMessageUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/utils/Utils.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<SignMessageResult> SignMessageUseCase::execute(const SignMessageInput &input) {
    SignMessageResult result;
    QWarningMessage msg;
    QMap<QString, QString> sigs;
    for (const auto &signer : input.signers) {
        QString sig = bridge::SignMessage(signer, input.message, msg);
        if (msg.isSuccess()) {
            sigs.insert(QString::fromStdString(signer.get_master_fingerprint()), sig);
        } else {
            return Result<SignMessageResult>::failure(msg.what());
        }
    }
    result.signatures = sigs;
    return Result<SignMessageResult>::success(result);
}
} // namespace features::signers::usecases