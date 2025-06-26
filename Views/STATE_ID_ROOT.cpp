/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
#include "STATE_ID_ROOT.h"
#include "Chats/ClientController.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QGroupWalletHealthCheck.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QInheritancePlan.h"
#include "Premiums/QServerKey.h"
#include "ProfileSetting.h"
#include "QEventProcessor.h"
#include "Servers/Draco.h"
#include "ServiceSetting.h"
#include "Signers/QSignerManagement.h"
#include "bridgeifaces.h"
#include "Premiums/QWalletServicesTag.h"

void ROOT_Entry(QVariant msg) {}

void ROOT_Exit(QVariant msg) {}

void EVT_ONS_CLOSE_REQUEST_HANDLER(QVariant msg) {
    QEventProcessor::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_NONE);
    switch (msg.toInt()) {
    case E::STATE_ID_SCR_SEND:
    case E::STATE_ID_SCR_ADD_WALLET:
    case E::STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION:
    case E::STATE_ID_SCR_ADD_NEW_SIGNER: {
        AppModel::instance()->resetSignersChecked();
        break;
    }
    case E::STATE_ID_SCR_TRANSACTION_INFO: {
        AppModel::instance()->setTransactionInfo(NULL);
    } break;
    default:
        break;
    }
}

void EVT_STARTING_APPLICATION_LOCALMODE_HANDLER(QVariant msg) {
    int isPrimaryKey = msg.toInt();
    if (!AppModel::instance()->inititalized()) {
        if (isPrimaryKey > 0) {
            QEventProcessor::instance()->sendEvent(E::EVT_GOTO_APP_SETTINGS_TAB, msg);
        } else {
            QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
        }
        QMap<QString, QVariant> makeInstanceData;
        makeInstanceData["state_id"] = E::STATE_ID_SCR_HOME;
        AppModel::instance()->makeNunchukInstance(makeInstanceData, "");
    } else {
        AppModel::instance()->requestInitialData();
        if (isPrimaryKey > 0) {
            QEventProcessor::instance()->sendEvent(E::EVT_GOTO_APP_SETTINGS_TAB, msg);
        } else {
            QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
        }
    }
}

void EVT_GOTO_HOME_WALLET_TAB_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ROOT_PROMT_PIN_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_ROOT_PROMT_PASSPHRASE_HANDLER(QVariant msg) {
    Q_UNUSED(msg);
}

void EVT_STARTING_APPLICATION_ONLINEMODE_HANDLER(QVariant msg) {
    if (CLIENT_INSTANCE->isNunchukLoggedIn()) {
        QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
        timeoutHandler(1000, []() { emit CLIENT_INSTANCE->contactsChanged(); });
    } else {
        if (CLIENT_INSTANCE->checkStayLoggedIn()) {
            Draco::instance()->getMe();
            if (Draco::instance()->Uid() == CLIENT_INSTANCE->getMe().email) {
                bridge::nunchukSetCurrentMode(ONLINE_MODE);
                QEventProcessor::instance()->notifySendEvent(E::EVT_NUNCHUK_LOGIN_SUCCEEDED);
            } else {
                QEventProcessor::instance()->sendEvent(E::EVT_LOGIN_MATRIX_REQUEST);
            }
        } else {
            QEventProcessor::instance()->sendEvent(E::EVT_LOGIN_MATRIX_REQUEST);
            if ("CreateAccount" == msg.toString()) {
                Draco::instance()->requestCreateAccount();
            }
        }
    }
    timeoutHandler(1000, []() { AppModel::instance()->timerFeeRatesHandle(); });
}

void EVT_ONLINE_ONS_CLOSE_REQUEST_HANDLER(QVariant msg) {
    switch (msg.toInt()) {
    case E::STATE_ID_SCR_TRANSACTION_INFO: {
        AppModel::instance()->setTransactionInfo(NULL);
    } break;
    default:
        break;
    }
}

void EVT_STARTING_APPLICATION_ONLINE_HWL_HANDLER(QVariant msg) {
    QEventProcessor::instance()->sendEvent(E::EVT_GOTO_HOME_WALLET_TAB);
}

void EVT_SHOW_TOAST_MESSAGE_HANDLER(QVariant msg) {}

void EVT_ROOT_UPDATE_PROFILE_HANDLER(QVariant msg) {}

void EVT_SETTING_ACCOUNT_CHANGE_PASSWORD_HANDLER(QVariant msg) {
    QString oldPasswordInput = msg.toMap().value("oldPassword").toString();
    QString newPasswordInput = msg.toMap().value("newPassword").toString();
    Draco::instance()->changePassword(oldPasswordInput, newPasswordInput);
}

void EVT_SHOW_CREATE_ACCOUNT_REQUEST_HANDLER(QVariant msg) {}

void EVT_SETTING_ONS_CLOSE_REQUEST_HANDLER(QVariant msg) {}

void EVT_GOTO_HOME_CHAT_TAB_HANDLER(QVariant msg) {}

void EVT_LOGIN_DB_REQUEST_HANDLER(QVariant msg) {}

void EVT_LOGIN_MATRIX_REQUEST_HANDLER(QVariant msg) {}

void EVT_GOTO_APP_SETTINGS_TAB_HANDLER(QVariant msg) {
    QtConcurrent::run([]() {
        CLIENT_INSTANCE->refreshDevices();
    });
    if(msg.isNull()) {
        ProfileSetting::instance()->setOptionIndex(0);
    }
    else {
        ProfileSetting::instance()->setOptionIndex(msg.toInt());
    }
}

void EVT_SIGN_IN_REQUEST_HANDLER(QVariant msg) {}

void EVT_ONS_CLOSE_ALL_REQUEST_HANDLER(QVariant msg) {
    QEventProcessor::instance()->closeAllPopup();
}

void EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg) {
    QEventProcessor::instance()->sendEvent(E::EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST, true);
}

void EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg) {
    QEventProcessor::instance()->sendEvent(E::EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST, false);
}

void EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg) {}

void EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST_HANDLER(QVariant msg) {}

void EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST_HANDLER(QVariant msg) {}

void EVT_NUNCHUK_LOGIN_SUCCEEDED_HANDLER(QVariant msg) {
    AppModel::instance()->requestClearData();
    timeoutHandler(100, []() {
        qApp->setOverrideCursor(Qt::WaitCursor);
        QMap<QString, QVariant> makeInstanceData;
        makeInstanceData["state_id"] = E::STATE_ID_SCR_HOME_ONLINE;
        AppModel::instance()->makeInstanceForAccount(makeInstanceData, "");
        qApp->restoreOverrideCursor();
    });
}

void EVT_GOTO_SERVICE_SETTING_TAB_HANDLER(QVariant msg) {
    int option = msg.toInt() == 0 ? 1 : msg.toInt();
    ServiceSetting::instance()->setOptionIndex(option);
    if (option == 4) { // View Inheritance Plan
        if (auto w = AppModel::instance()->walletInfoPtr()) {
            QMap<QString, QVariant> maps;
            maps["type"] = "inheritance-planing";
            maps["wallet_id"] = w->walletId();
            DBG_INFO << maps;
            timeoutHandler(500, [maps]() { QEventProcessor::instance()->sendEvent(E::EVT_SERVICE_SELECT_WALLET_REQUEST, QVariant::fromValue(maps)); });
        }
    } else if (option == 7) { // View policies
        if (auto w = AppModel::instance()->walletInfoPtr()) {
            QMap<QString, QVariant> maps;
            maps["type"] = "platform-key-co-signing-policies";
            maps["wallet_id"] = w->walletId();
            DBG_INFO << maps;
            timeoutHandler(500, [maps]() { QEventProcessor::instance()->sendEvent(E::EVT_SERVICE_SELECT_WALLET_REQUEST, QVariant::fromValue(maps)); });
        }
    }
}

void EVT_HEALTH_CHECK_STARTING_REQUEST_HANDLER(QVariant msg) {
    if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
        if (dashboard->flow() == (int)AlertEnum::E_Alert_t::GROUP_WALLET_SETUP) {
            QMap<QString, QVariant> maps = msg.toMap();
            QString xfp = maps["xfp"].toString();
            QGroupWallets::instance()->dashboardInfoPtr()->healthPtr()->HealthCheckForKey(xfp);
        } else if (dashboard->flow() == (int)AlertEnum::E_Alert_t::WELCOME_SIGN_IN_SIGNATURE_XPUB) {
            // For sign dummy, so not implement in here
        } else {
            AppModel::instance()->setWalletInfo(dashboard->walletInfoPtr());
        }
    }
}

void EVT_DUMMY_TRANSACTION_INFO_REQUEST_HANDLER(QVariant msg) {}

void EVT_DASHBOARD_ALERT_SUCCESS_REQUEST_HANDLER(QVariant msg) {}

void EVT_SHARE_YOUR_SECRET_REQUEST_HANDLER(QVariant msg) {
    if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
        if (dashboard->flow() != (int)AlertEnum::E_Alert_t::CREATE_INHERITANCE_PLAN_SUCCESS) {
            dashboard->setFlow((int)AlertEnum::E_Alert_t::SERVICE_TAG_SHARE_YOUR_SECRET);
        }
    }
}

void EVT_REENTER_YOUR_PASSWORD_REQUEST_HANDLER(QVariant msg) {}

void EVT_ONBOARDING_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->walletList()->saveOrderWalletIds();
    QWalletServicesTag::instance()->additionalGetWalletConfig();
}

void EVT_REPLACE_SELECT_KEY_REQUEST_HANDLER(QVariant msg) {}

void EVT_SIGN_IN_VIA_XPUB_REQUEST_HANDLER(QVariant msg) {}

void EVT_EDIT_MEMBERS_REQUEST_HANDLER(QVariant msg) {
    if (auto dashboard = QGroupWallets::instance()->dashboardInfoPtr()) {
        dashboard->initMembers();
    }
}

void EVT_COIN_DETAILS_CONTROL_REQUEST_HANDLER(QVariant msg) {
    if (auto w = AppModel::instance()->walletInfoPtr()) {
        w->RequestCoinScreen(msg);
    }
}

void EVT_UTXOS_CONSOLIDATE_REQUEST_HANDLER(QVariant msg) {}

void EVT_CONSOLIDATE_COINS_MERGE_MAKE_TRANSACTION_REQUEST_HANDLER(QVariant msg) {}

void EVT_SETUP_GROUP_WALLET_REQUEST_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    if (type == "create-group-wallet") {
        QString walletNameInputted = maps["walletNameInputted"].toString();
        QString walletDescription = maps["walletDescription"].toString();
        int addressType = maps["addressType"].toInt();
        if (auto nw = AppModel::instance()->newWalletInfo()) {
            nw->setWalletName(walletNameInputted);
            nw->setWalletDescription(walletDescription);
            nw->setWalletAddressType(addressType);
            DBG_INFO << maps;
            int walletM = maps["walletM"].toInt();
            int walletN = maps["walletN"].toInt();
            if (auto sandbox = nw->groupSandboxPtr()) {
                sandbox->CreateAGroup(walletNameInputted, walletM, walletN, addressType);
                sandbox->setScreenFlow("setup-group-wallet");
                // sandbox->setScreenFlow("register-wallet-hardware");
            }
        }
    } else if (type == "setup-group-wallet") {
    }
}

void EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED_HANDLER(QVariant msg) {}

void EVT_PRIMARY_KEY_CONFIGURATION_REQUEST_HANDLER(QVariant msg) {}

void EVT_HOME_ADD_NEW_SIGNER_REQUEST_HANDLER(QVariant msg) {
    QEventProcessor::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_ADD_SIGNER);
    QSignerManagement::instance()->setScreenFlow("add-a-key");
}

void EVT_HOME_ADD_WALLET_REQUEST_HANDLER(QVariant msg) {
    QWalletPtr newWallet(new Wallet());
    newWallet.data()->setCapableCreate(false);
    newWallet.data()->setWalletAddressType((int)nunchuk::AddressType::NATIVE_SEGWIT);
    AppModel::instance()->setNewWalletInfo(newWallet);
    AppModel::instance()->resetSignersChecked();
    AppModel::instance()->setSingleSignerInfo(QSingleSignerPtr(new QSingleSigner()));
    QEventProcessor::instance()->setCurrentFlow((int)ENUNCHUCK::IN_FLOW::FLOW_ADD_WALLET);
}
