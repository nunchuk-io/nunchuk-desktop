#pragma once
#include <QObject>
#include <QString>
#include "core/usecase/DefineUseCase.h"
#include <nunchuk.h>

namespace features::claiming::usecases {
  
#define FIELDS_INPUT(X)          \
    X(QString, magic)            \
    X(QString, address)          \
    X(int64_t, amount)           \
    X(double, fee_rate)          \
    X(bool, anti_fee_sniping)    \
    X(QString, bsms)            \
    X(bool, subtract_fee_from_amount) \
    X(QString, messageId)          \
    X(QStringList, authos)

DEFINE_USECASE_INPUT(CreateTransactionInput)

#define FIELDS_RESULT(X)       \
    X(QString, psbt)          \
    X(qint64, tx_fee)       \
    X(qint64, tx_fee_rate)    \
    X(qint64, tx_sub_amount)  \
    X(bool, subtract_fee_from_amount) \
    X(int, change_pos)

DEFINE_USECASE_RESULT(CreateTransactionResult)

class CreateTransactionUseCase : public core::usecase::AsyncUseCase<CreateTransactionUseCase, CreateTransactionInput, CreateTransactionResult> {
  protected:
  core::usecase::Result<CreateTransactionResult> execute(const CreateTransactionInput &input) override;
};

#undef FIELDS_RESULT
#undef FIELDS_INPUT
} // namespace features::claiming::usecases