#include "SyncTransactionFromRemoteFlow.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::transactions::flows {

SyncTransactionFromRemoteFlow::SyncTransactionFromRemoteFlow(FlowContext* ctx, QObject *parent)
    : core::flow::BaseFlow(ctx, parent) {}


} // namespace features::transactions::flows