#pragma once
#include "core/usecase/DefineUseCase.h"
#include <QObject>
#include <QString>
#include <nunchuk.h>

namespace features::transactions::usecases {

#define FIELDS_INPUT(X)                                                                                                                                        \
    X(QString, wallet_id)                                                                                                                                      \
    X(QString, tx_id)                                                                                                                                          \
    X(QString, room_id)                                                                                                                                        \
    X(QString, init_event_id)                                                                                                                                  \
    X(nunchuk::Device, device)

DEFINE_USECASE_INPUT(LibRoomSignTxInput)

#define FIELDS_RESULT(X) X(nunchuk::Transaction, tx)

DEFINE_USECASE_RESULT(LibRoomSignTxResult)

class LibRoomSignTxUseCase : public core::usecase::AsyncUseCase<LibRoomSignTxUseCase, LibRoomSignTxInput, LibRoomSignTxResult> {
  protected:
    core::usecase::Result<LibRoomSignTxResult> execute(const LibRoomSignTxInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::transactions::usecases