#include "LibRoomSignTxUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::transactions::usecases {
using namespace core::usecase;

Result<LibRoomSignTxResult> LibRoomSignTxUseCase::execute(const LibRoomSignTxInput &input) {
    QWarningMessage msg;
    auto selectedDv = QDevicePtr(new QDevice(input.device));
    matrixbrigde::SignTransaction(input.room_id, input.init_event_id, selectedDv, msg);
    msg.resetWarningMessage();
    nunchuk::Transaction trans = bridge::nunchukGetOriginTransaction(input.wallet_id, input.tx_id, msg);
    LibRoomSignTxResult result;
    if (msg.isSuccess()) {
        result.tx = trans;
        return Result<LibRoomSignTxResult>::success(result);
    } else {
        return Result<LibRoomSignTxResult>::failure(QString("Failed to sign transaction: %1").arg(msg.what()));
    }
}

} // namespace features::transactions::usecases