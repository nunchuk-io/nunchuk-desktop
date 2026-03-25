#pragma once

#define GUARD_PARAMETER(param, ...)                                                                                                                            \
    if (!(param)) {                                                                                                                                            \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_CTX(...)                                                                                                                                         \
    if (!ctx()) {                                                                                                                                              \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_APP_MODEL(...)                                                                                                                                   \
    GUARD_CTX(__VA_ARGS__)                                                                                                                                     \
    auto appModel = ctx()->appModel();                                                                                                                         \
    if (!appModel) {                                                                                                                                           \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_GROUP_WALLETS(...)                                                                                                                               \
    GUARD_CTX(__VA_ARGS__)                                                                                                                                     \
    auto groupWallets = ctx()->groupWallets();                                                                                                                 \
    if (!groupWallets) {                                                                                                                                       \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_DASHBOARD(...)                                                                                                                                   \
    GUARD_GROUP_WALLETS(__VA_ARGS__)                                                                                                                           \
    auto dashboard = groupWallets->dashboardInfoPtr();                                                                                                         \
    if (!dashboard) {                                                                                                                                          \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_SERVICE_SETTING(...)                                                                                                                             \
    GUARD_CTX(__VA_ARGS__)                                                                                                                                     \
    auto serviceSetting = ctx()->serviceSetting();                                                                                                             \
    if (!serviceSetting) {                                                                                                                                     \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_SERVICE_TAG(...)                                                                                                                                 \
    GUARD_SERVICE_SETTING(__VA_ARGS__)                                                                                                                         \
    auto serviceTag = serviceSetting->servicesTagPtr();                                                                                                        \
    if (!serviceTag) {                                                                                                                                         \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_SERVICE_WALLET(...)                                                                                                                              \
    GUARD_SERVICE_SETTING(__VA_ARGS__)                                                                                                                         \
    auto wallet = serviceSetting->walletInfoPtr();                                                                                                             \
    if (!wallet) {                                                                                                                                             \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_SERVICE_WALLET_DASHBOARD(...)                                                                                                                    \
    GUARD_SERVICE_WALLET(__VA_ARGS__)                                                                                                                          \
    auto dashboard = wallet->dashboard();                                                                                                                      \
    if (!dashboard) {                                                                                                                                          \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_SUB_SCREEN_MANAGER(...)                                                                                                                          \
    GUARD_CTX(__VA_ARGS__)                                                                                                                                     \
    auto subMng = ctx()->subScreenManager();                                                                                                                   \
    if (!subMng) {                                                                                                                                             \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_FLOW_MANAGER(...)                                                                                                                                \
    GUARD_CTX(__VA_ARGS__)                                                                                                                                     \
    auto flowMng = ctx()->flowManager();                                                                                                                       \
    if (!flowMng) {                                                                                                                                            \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_SERVICE_WALLET_INHERITANCE_PLAN(...)                                                                                                             \
    GUARD_SERVICE_WALLET(__VA_ARGS__)                                                                                                                          \
    auto ihPlan = wallet->inheritancePlanPtr();                                                                                                                \
    if (!ihPlan) {                                                                                                                                             \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_WALLET(wallet_id, ...)                                                                                                                           \
    GUARD_APP_MODEL(__VA_ARGS__)                                                                                                                               \
    auto wallet = appModel->walletListPtr()->getWalletById(wallet_id);                                                                                         \
    if (!wallet) {                                                                                                                                             \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_TRANSACTION(...)                                                                                                                                 \
    GUARD_APP_MODEL(__VA_ARGS__)                                                                                                                               \
    auto tran = appModel->transactionInfoPtr();                                                                                                                \
    if (!tran) {                                                                                                                                               \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_RIGHT_PANEL_NAV(...)                                                                                                                             \
    GUARD_CTX(__VA_ARGS__)                                                                                                                                     \
    auto rightPanel = ctx()->rightPanelNavigator();                                                                                                            \
    if (!rightPanel) {                                                                                                                                         \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_APP_SETTING(...)                                                                                                                                 \
    GUARD_CTX(__VA_ARGS__)                                                                                                                                     \
    auto appSetting = ctx()->appSetting();                                                                                                                     \
    if (!appSetting) {                                                                                                                                         \
        return __VA_ARGS__;                                                                                                                                    \
    }

#define GUARD_CLIENT_CONTROLLER(...)                                                                                                                           \
    GUARD_CTX(__VA_ARGS__)                                                                                                                                     \
    auto clientCtrl = ctx()->clientController();                                                                                                         \
    if (!clientCtrl) {                                                                                                                                   \
        return __VA_ARGS__;                                                                                                                                    \
    }
