#include "SyncWalletFromRemoteFlow.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "qUtils.h"
#include "ifaces/bridgeifaces.h"

namespace features::wallets::flows {
using namespace features::wallets::usecases;

SyncWalletFromRemoteFlow::SyncWalletFromRemoteFlow(FlowContext* ctx, QObject *parent) : core::flow::BackgroundFlow(ctx, parent) {
    
    
}

void SyncWalletFromRemoteFlow::sync() {
    DBG_INFO << "SyncWalletFromRemoteFlow::sync " << CLIENT_INSTANCE->isNunchukLoggedIn() << " === " << CLIENT_INSTANCE->isMatrixLoggedIn();
    SyncWalletFromRemoteInput input;
    m_syncWalletFromRemoteUC.executeAsync(input, [](core::usecase::Result<SyncWalletFromRemoteResult> result) {
        AppModel::instance()->startReloadUserDb();
    });
}
} // namespace features::flows
