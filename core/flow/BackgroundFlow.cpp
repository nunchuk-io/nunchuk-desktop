#include "BackgroundFlow.h"
#include "core/ui/UiServices.inc"

namespace core::flow {
BackgroundFlow::BackgroundFlow(FlowContext* ctx, QObject *parent) : BaseFlow(ctx, parent) {
    connect(ctx->clientController(), &ClientController::isMatrixLoggedInChanged, this, &BackgroundFlow::sync);
}

bool BackgroundFlow::isBackground() const {
    return true;
}

void BackgroundFlow::start() {
    m_timer.start(120'000); // 2 minutes
    connect(&m_timer, &QTimer::timeout, this, &BackgroundFlow::sync);
    if (ctx() && (ctx()->clientController()->isMatrixLoggedIn() && ctx()->clientController()->isNunchukLoggedIn())) {
        sync();
    }
}

void BackgroundFlow::stop() {
    m_timer.stop();
}

} // namespace core::flow