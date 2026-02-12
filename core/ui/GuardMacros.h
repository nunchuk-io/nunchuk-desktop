#pragma once

#define GUARD_CTX() \
if (!ctx()) { \
    return; \
}

#define GUARD_APP_MODEL() \
GUARD_CTX() \
auto appModel = ctx()->appModel(); \
if (!appModel) { \
    return; \
}

#define GUARD_GROUP_WALLETS() \
GUARD_CTX() \
auto groupWallets = ctx()->groupWallets(); \
if (!groupWallets) { \
    return; \
}

#define GUARD_DASHBOARD() \
GUARD_GROUP_WALLETS() \
auto dashboard = groupWallets->dashboardInfoPtr(); \
if (!dashboard) { \
    return; \
}

#define GUARD_SERVICE_SETTING() \
GUARD_CTX() \
auto serviceSetting = ctx()->serviceSetting(); \
if (!serviceSetting) { \
    return; \
}

#define GUARD_SERVICE_TAG() \
GUARD_SERVICE_SETTING() \
auto serviceTag = serviceSetting->servicesTagPtr(); \
if (!serviceTag) { \
    return; \
}

#define GUARD_SERVICE_WALLET() \
GUARD_SERVICE_SETTING() \
auto wallet = serviceSetting->walletInfoPtr(); \
if (!wallet) { \
    return; \
}

#define GUARD_SERVICE_WALLET_DASHBOARD() \
GUARD_SERVICE_WALLET() \
auto dashboard = wallet->dashboard(); \
if (!dashboard) { \
    return; \
}

#define GUARD_SUB_SCREEN_MANAGER() \
GUARD_CTX() \
auto subMng = ctx()->subScreenManager(); \
if (!subMng) { \
    return; \
}

#define GUARD_FLOW_MANAGER() \
GUARD_CTX() \
auto flowMng = ctx()->flowManager(); \
if (!flowMng) { \
    return; \
}

#define GUARD_SERVICE_WALLET_INHERITANCE_PLAN() \
GUARD_SERVICE_WALLET() \
auto ihPlan = wallet->inheritancePlanPtr(); \
if (!ihPlan) { \
    return; \
}

#define GUARD_WALLET(wallet_id) \
GUARD_APP_MODEL() \
auto wallet = appModel->walletListPtr()->getWalletById(wallet_id); \
if (!wallet) { \
    return; \
}

#define GUARD_TRANSACTION() \
GUARD_APP_MODEL() \
auto tran = appModel->transactionInfoPtr(); \
if (!tran) { \
    return; \
}