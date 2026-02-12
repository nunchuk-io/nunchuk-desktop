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

#ifndef DRACODEFINES_H
#define DRACODEFINES_H
#include <QObject>
#include <QMap>
#include "AppSetting.h"

#define DRAGON_PASSPORT_URL     "https://api.nunchuk.io/v1.1/passport"
#define DRAGON_USER_URL         "https://api.nunchuk.io/v1.1/user"
#define DRAGON_CHAT_URL         "https://api.nunchuk.io/v1.1/chat"
#define DRAGON_APP_URL          "https://api.nunchuk.io/v1.1/app"
#define DRAGON_FOREX_URL        "https://api.nunchuk.io/v1.1/forex"
#define DRAGON_BANNERS_URL      "https://api.nunchuk.io/v1.1/banners"

#define DRAGON_SUBSCRIPTIONS_URL            "https://api.nunchuk.io/v1.1/subscriptions"
#define DRAGON_USER_WALLETS_URL             "https://api.nunchuk.io/v1.1/user-wallets"
#define DRAGON_USER_WALLETS_TESTNET_URL     "https://api-testnet.nunchuk.io/v1.1/user-wallets"

#define DRAGON_GROUP_WALLETS_URL            "https://api.nunchuk.io/v1.1/group-wallets"
#define DRAGON_GROUP_WALLETS_TESTNET_URL    "https://api-testnet.nunchuk.io/v1.1/group-wallets"

inline QString commandByNetwork(const QString& cmd){
    QString command = cmd;
    if ((int)AppSetting::Chain::TESTNET == AppSetting::instance()->primaryServer() && command.contains(DRAGON_USER_WALLETS_URL)) {
        command.replace(DRAGON_USER_WALLETS_URL, DRAGON_USER_WALLETS_TESTNET_URL);
    } else if ((int)AppSetting::Chain::TESTNET == AppSetting::instance()->primaryServer() && command.contains(DRAGON_GROUP_WALLETS_URL)) {
        command.replace(DRAGON_GROUP_WALLETS_URL, DRAGON_GROUP_WALLETS_TESTNET_URL);
    }
    return command;
}

struct DracoUser {
    QString id = "";
    QString name = "";
    QString email = "";
    QString gender = "";
    QString avatar = "";
    QString status = "";
    QString status_text = "";
    QString chat_id = "";
    QString username = "";
    QString login_type = "";
};
Q_DECLARE_METATYPE(DracoUser)

struct DracoDevice {
    QString id = "";
    QString name = "";
    QString lastIP = "";
    QString lastTs = "";
    bool thisId = false;
};

struct SecurityQuestion {
    QString id = "";
    QString question = "";
    QString answer = "";
    bool is_answered = false;
    bool is_changed = false;
};

struct ReqiredSignaturesInfo {
    int     type = 0;
    int     required_signatures = 0;
    int     required_answers = 0;
    int     required_confirmation_codes = 0;
};

namespace Command {
namespace Common {
enum CMD_IDX {
    CREATE_ACCOUNT = 0,
    SIGNOUT,
    SIGNIN,
    RECOVER_PASSWORD,
    FORGOT_PASSWORD,
    CHANGE_PASSWORD,
    RESEND_PASSWORD,
    GET_ME,
    SEARCH_FRIEND,
    SEARCH_FRIEND_LIST,
    REQUEST_FRIEND,
    REQUEST_FRIEND_LIST,
    CANCEL_REQUEST_FRIEND,
    IGNORE_FRIEND_REQUEST,
    GET_FRIENDS_LIST,
    GET_FRIENDS_SENT,
    GET_FRIENDS_RECEIVED,
    REMOVE_FRIEND,
    ACCEPT_FRIEND_REQUEST,
    GET_ROOM_MEMBERS,
    VERIFY_NEW_DEVICE,
    RESEND_VERIFY_NEW_DEVICE_CODE,
    INVITE_FRIEND,
    DELETE_ACCOUNT,
    DELETE_CONFIRM,
    GET_DEVICES_LIST,
    USER_PROFILE,
    REMOVE_AVATAR,
    CHANGE_AVATAR,
    CHECK_FOR_UPDATE,
    DEVICE_MARK_COMPROMISED,
    DEVICE_SIGNOUT,
    VERIFY_DELETE_DEVICES,
    DEVICE_ALL_SIGNOUT,

    // USER_SUBSCRIPTION
    USER_SUBCRIPTIONS_CURRENT,
    USER_SUBCRIPTIONS_TESTNET,

    USER_SUBCRIPTIONS_STATUS,
    USER_SUBCRIPTIONS_STATUS_TESTNET,
    VERIFY_PASSWORD_TOKEN,

    GET_ELECTRUM_SERVERS,

    // Check account availability
    ACCOUNT_AVAILABILITY,

    // Check supported signers
    TAPROOT_SUPPORTED_SIGNERS,

    CMD_MAX
};
}

namespace PrimaryKey {
enum CMD_IDX {
    PRIMARY_KEY_SIGNUP = Common::CMD_IDX::CMD_MAX + 1,
    PRIMARY_KEY_SIGNIN,
    PRIMARY_KEY_NONCE,
    PRIMARY_KEY_USERNAME,
    PRIMARY_KEY_PUBLIC_ADDRESS,
    PRIMARY_KEY_DELETE_CONFIRMTION,
    PRIMARY_KEY_CHANGE_KEY,

    CMD_MAX
};
}

namespace Premium {
enum CMD_IDX {
    // ASSISTED_WALLETS
    ASSISTED_WALLET_GET = PrimaryKey::CMD_IDX::CMD_MAX + 1,
    ASSISTED_WALLET_CREATE_TX,
    ASSISTED_WALLET_UPDATE_TX,
    ASSISTED_WALLET_SIGN_TX,
    ASSISTED_WALLET_CANCEL_TX,
    ASSISTED_WALLET_GET_TX,
    ASSISTED_WALLET_GET_LIST_TX,
    ASSISTED_WALLET_GET_LIST_TX_NOTES,
    ASSISTED_WALLET_DELETE_LIST_TX,
    ASSISTED_WALLET_SYNC_TX,
    ASSISTED_WALLET_RBF_TX,
    ASSISTED_WALLET_ADD_KEY,
    ASSISTED_WALLET_GET_LIST_KEY,
    ASSISTED_WALLET_REMOVE_ID,
    ASSISTED_WALLET_GET_CONFIG,
    ASSISTED_WALLET_GET_INFO,
    ASSISTED_WALLET_UPDATE_NAME,
    ASSISTED_KEY_UPDATE_NAME,
    ASSISTED_WALLET_REMOVE_WALLET,
    ASSISTED_WALLET_REMOVE_WALLET_REQUIRED_SIGNATURES,

    // SEC_QUES
    SEC_QUES_GET,
    SEC_QUES_SET,
    SEC_QUES_CREATE,
    SEC_QUES_UPDATE,
    SEC_QUES_VERIFY_ANSWER,
    SEC_QUES_REQUIRED_SIGNATURES,

    //LOCK_DOWN
    LOCKDOWN_SET_ANSWER_SECURITY_QUESTION,
    LOCKDOWN_SET_SIGN_DUMMY_TX,
    LOCKDOWN_REQUIRED_SIGNATURES,
    LOCKDOWN_GET_PERIOD,


    //GET_RANDOM_NONCE
    GET_RANDOM_NONCE,

    //USER_KEYS
    USER_KEYS_MARK_A_KEY_AS_VERIFIED,
    USER_KEYS_REQUEST_RECOVERY_KEY,
    USER_KEYS_RECOVERY_KEY,
    USER_KEYS_MARK_RECOVER_STATUS,
    USER_KEYS_DOWNLOAD_BACKUP,
    USER_KEYS_CALCULATE_REQUIRED_SIGNATURES,
    USER_KEYS_UPLOAD_BACKUP,
    USER_KEYS_GET_LIST_USER_KEYS,
    USER_KEYS_GET_USER_KEY,

    //INHERITANCE
    INHERITANCE_CLAIM_DOWNLOAD_BACKUP,
    INHERITANCE_CLAIM_REQUEST,
    INHERITANCE_CLAIM_STATUS,
    INHERITANCE_CLAIM_CREATE_TX,
    INHERITANCE_CHECK,
    INHERITANCE_CLAIM_INIT,
    INHERITANCE_CLAIM_DOWNLOAD_WALLET,
    INHERITANCE_GET_PLAN,
    INHERITANCE_FAKE_UPDATE,
    INHERITANCE_PLAN_REQUIRED_SIGNATURES,
    INHERITANCE_PLAN_UPDATE,
    INHERITANCE_PLAN_CREATE,
    INHERITANCE_PLAN_BUFFER_PERIODS,
    INHERITANCE_PLAN_CANCEL,
    INHERITANCE_REQUEST_PLANING_DENY,
    INHERITANCE_REQUEST_PLANING_APPROVE,

    //SERVER_KEYS
    SERVER_KEYS_GET,
    SERVER_KEYS_UPDATE,
    SERVER_KEYS_REQUIRED_SIGNATURES,

    GET_CURRENCIES,
    ASSISTED_WALLET_ALERTS,
    ASSISTED_WALLET_ALERTS_COUNT,
    ASSISTED_WALLET_ALERTS_READ,
    ASSISTED_WALLET_ALERTS_DISMISS,

    // Wallet health check
    ASSISTED_WALLET_GET_KEY_HEALTH_REMINDER,
    ASSISTED_WALLET_ADD_OR_UPDATE_KEY_HEALTH_REMINDER,
    ASSISTED_WALLET_DELETE_KEY_HEALTH_REMINDER,
    ASSISTED_WALLET_SKIP_KEY_HEALTH_REMINDER,
    ASSISTED_WALLET_HEALTHCHECK,
    ASSISTED_WALLET_GET_HEALTH,

    ASSISTED_WALLET_GET_DUMMY_TX,
    ASSISTED_WALLET_UPDATE_DUMMY_TX,
    ASSISTED_WALLET_CANCEL_DUMMY_TX,
    ASSISTED_WALLET_FINALIZE_DUMMY_TX,

    ASSISTED_WALLET_HEALTH_CHECK_HISTORY_GET,

    CONFIRMATION_CODE,
    VERIFY_CONFIRMATION_CODE,
    COUNTRY_CODE_LIST,
    ONBOARDING_NO_ADVISOR,
    CALCULATE_REQUIRED_SIGNATURES_FOR_CHANGING_EMAIL,
    CHANGING_EMAIL,

    // Replace Wallet
    ASSISTED_WALLET_CONFIGURE_WALLET_REPLACEMENT,
    ASSISTED_WALLET_INITIATE_KEY_REPLACEMENT,
    ASSISTED_WALLET_CANCEL_KEY_REPLACEMENT,
    ASSISTED_WALLET_VERIFY_KEY_REPLACEMENT,
    ASSISTED_WALLET_REMOVE_KEY_REPLACEMENT,
    ASSISTED_WALLET_REPLACE_KEY,
    ASSISTED_WALLET_FINALIZE_KEY_REPLACEMENT,
    ASSISTED_WALLET_GET_STATUS_KEY_REPLACEMENT,
    ASSISTED_WALLET_RESET_KEY_REPLACEMENT,
    ASSISTED_WALLET_DOWNLOAD_BACKUP_KEY_REPLACEMENT,
    ASSISTED_WALLET_UPLOAD_BACKUP_KEY_REPLACEMENT,

    // Saved addresses
    ASSISTED_WALLET_GET_SAVED_ADDRESSES,
    ASSISTED_WALLET_ADD_SAVED_ADDRESS,
    ASSISTED_WALLET_DELETE_SAVED_ADDRESS,

    SIGN_DUMMY_TRANSACTION_TO_SIGN_IN,
    SIGN_IN_USING_XPUB_WALLET,

    ASSISTED_WALLET_COINCONTROL_GET,
    ASSISTED_WALLET_COINCONTROL_UPDATE,

    // Draft Wallet
    DRAFT_WALLET_MARK_ALERT_AS_READ,
    DRAFT_WALLET_DISMISS_ALERT,
    DRAFT_WALLET_GET_ALERTS,
    DRAFT_WALLET_GET_TOTAL_ALERTS,
    DRAFT_WALLET_SET_SERVER_KEY,
    DRAFT_WALLET_REQUEST_ADD_KEY,
    DRAFT_WALLET_PUSH_REQUEST_ADD_KEY,
    DRAFT_WALLET_INIT,
    DRAFT_WALLET_GET_REQUEST_ADD_KEY,
    DRAFT_WALLET_GET_CURRENT,
    DRAFT_WALLET_RESET_CURRENT,
    DRAFT_WALLET_UPLOAD_BACKUP,
    DRAFT_WALLET_DOWNLOAD_BACKUP,
    DRAFT_WALLET_VERIFY,

    CONFIG_WALLET_SETUP,
    DRAFT_WALLET_TIMELOCK,

    ASSISTED_WALLET_REPLACEMENT_TIMELOCK,

    // Claiming wallets
    CLAIMING_WALLET_GET_BY_ID,
    CLAIMING_WALLET_UPDATE_BY_ID,
    CLAIMING_WALLET_DELETE_BY_ID,
    CLAIMING_WALLET_LIST,

    // Claimming wallet transaction
    CLAIMING_WALLET_TX_GET_LIST,
    CLAIMING_WALLET_TX_GET_ONE,
    CLAIMING_WALLET_TX_CREATE_UPDATE,
    CLAIMING_WALLET_TX_CREATE_BATCH,
    CLAIMING_WALLET_TX_CANCEL,
    CLAIMING_WALLET_TX_RBF,

    TIMELOCK_CONVERT,

    CMD_MAX
};
}

namespace Group {
enum CMD_IDX {
    //Groups Management
    GROUP_WALLETS_ONE = Premium::CMD_IDX::CMD_MAX + 1,
    GROUP_WALLETS_LIST,
    GROUP_WALLETS_LIST_DELETED,
    GROUP_WALLETS_ACCEPT,
    GROUP_WALLETS_DENY,
    GROUP_WALLET_LIST_WALLETS,
    GROUP_WALLET_REMOVE_WALLET,
    GROUP_WALLET_REMOVE_WALLET_REQUIRED_SIGNATURES,
    GROUP_WALLET_GET_ALIAS,
    GROUP_WALLET_UPDATE_ALIAS,
    GROUP_WALLET_DELETE_ALIAS,
    GROUP_WALLET_UPDATE_PRIMARY_OWNER,
    GROUP_WALLET_EDIT_GROUP_MEMBERS,
    GROUP_WALLET_EDIT_GROUP_MEMBERS_REQUIRED_SIGNATURES,

    //Group Alert Management
    GROUP_WALLET_ALERTS,
    GROUP_WALLET_ALERTS_COUNT,
    GROUP_WALLET_ALERTS_READ,
    GROUP_WALLET_ALERTS_DISMISS,

    //Groups permissions
    GROUP_WALLETS_ALL_PERMISSION,
    GROUP_WALLETS_ONE_PERMISSION,
    GROUP_WALLETS_DEFAULT_PERMISSION,
    GROUP_WALLETS_GET_CURRENT,
    GROUP_WALLETS_GET_WALLET,

    //Assisted Draft Wallets
    GROUP_DRAFT_WALLETS_ADD_KEY,
    GROUP_DRAFT_WALLETS_GET_CURRENT,
    GROUP_DRAFT_WALLETS_RESET_CURRENT,
    GROUP_DRAFT_WALLETS_REQUEST_ADD_KEY,
    GROUP_DRAFT_WALLETS_DELETE_REQUEST_ADD_KEY,
    GROUP_DRAFT_WALLETS_DOWNLOAD_BACKUP,
    GROUP_DRAFT_WALLETS_UPLOAD_BACKUP,

    // Group wallet health check
    GROUP_WALLETS_GET_KEY_HEALTH_REMINDER,
    GROUP_WALLETS_ADD_OR_UPDATE_KEY_HEALTH_REMINDER,
    GROUP_WALLETS_DELETE_KEY_HEALTH_REMINDER,
    GROUP_WALLETS_SKIP_KEY_HEALTH_REMINDER,
    GROUP_WALLETS_REQUEST_HEALTHCHECK,
    GROUP_WALLETS_HEALTHCHECK,
    GROUP_WALLETS_GET_HEALTH,

    // Group wallet dummy tx
    GROUP_WALLETS_GET_DUMMY_TX,
    GROUP_WALLETS_UPDATE_DUMMY_TX,
    GROUP_WALLETS_CANCEL_DUMMY_TX,
    GROUP_WALLETS_FINALIZE_DUMMY_TX,

    // Group transactions
    GROUP_WALLETS_GET_ALL_TX,
    GROUP_WALLETS_GET_ALL_TX_NOTES,
    GROUP_WALLETS_GET_ONE_TX,
    GROUP_WALLETS_UPDATE_TX,
    GROUP_WALLETS_CANCEL_TX,
    GROUP_WALLETS_SYNC_TX,
    GROUP_WALLETS_SIGN_TX,
    GROUP_WALLETS_RBF_TX,
    GROUP_WALLETS_CREATE_TX,
    GROUP_WALLETS_SCHEDULE_TX,
    GROUP_WALLETS_CANCEL_SCHEDULE_TX,
    GROUP_WALLET_UPDATE_WALLET,
    GROUP_WALLETS_REQUEST_SIGNATURE_TX,

    LOCKDOWN_GET_PERIOD,
    LOCKDOWN_REQUIRED_SIGNATURES,
    LOCKDOWN_SET_ANSWER_SECURITY_QUESTION,
    LOCKDOWN_SET_SIGN_DUMMY_TX,
    LOCKDOWN_SET_CONFIRMATION_CODE,
    SERVER_KEYS_GET,
    SERVER_KEYS_REQUIRED_SIGNATURES,
    SERVER_KEYS_UPDATE,
    ASSISTED_WALLET_GET_CONFIG,

    // Group chat
    GROUP_WALLETS_CHAT_GET,
    GROUP_WALLETS_CHAT_UPDATE,
    GROUP_WALLETS_CHAT_DELETE,
    GROUP_WALLETS_CHAT_CREATE,
    GROUP_WALLETS_CHAT_GET_ALL,
    GROUP_WALLETS_CHAT_GET_HISTORY_PERIODS,

    // Recurring payment
    GROUP_WALLET_RECURRING_PAYMENT_LIST,
    GROUP_WALLET_RECURRING_PAYMENT_CREATE,
    GROUP_WALLET_RECURRING_PAYMENT_GET,
    GROUP_WALLET_RECURRING_PAYMENT_CANCEL,

    // Replace Wallet
    GROUP_WALLET_CONFIGURE_WALLET_REPLACEMENT,
    GROUP_WALLET_INITIATE_KEY_REPLACEMENT,
    GROUP_WALLET_CANCEL_KEY_REPLACEMENT,
    GROUP_WALLET_VERIFY_KEY_REPLACEMENT,
    GROUP_WALLET_REMOVE_KEY_REPLACEMENT,
    GROUP_WALLET_REPLACE_KEY,
    GROUP_WALLET_FINALIZE_KEY_REPLACEMENT,
    GROUP_WALLET_GET_STATUS_KEY_REPLACEMENT,
    GROUP_WALLET_RESET_KEY_REPLACEMENT,
    GROUP_WALLET_DOWNLOAD_BACKUP_KEY_REPLACEMENT,
    GROUP_WALLET_UPLOAD_BACKUP_KEY_REPLACEMENT,

    GROUP_WALLET_COINCONTROL_GET,
    GROUP_WALLET_COINCONTROL_UPDATE,

    GROUP_DRAFT_WALLET_TIMELOCK,
    GROUP_DRAFT_WALLET_VERIFY,
    GROUP_WALLET_REPLACEMENT_TIMELOCK,

    CMD_MAX
};
}

}



enum class TARGET_ACTION {
    EMERGENCY_LOCKDOWN = 0,
    DOWNLOAD_KEY_BACKUP,
    UPDATE_SECURITY_QUESTIONS,
    UPDATE_INHERITANCE_PLAN,
    UPDATE_SERVER_KEY,
    DELETE_WALLET,
    EDIT_GROUP_MEMBERS,
    CHANGE_EMAIL,
    REPLACE_KEYS
};

enum class REQUIRED_SIGNATURE_TYPE_INT {
    NONE,           // ERROR
    SIGN_MESSAGE,   // NOT SUPPORT FOR NOW
    SIGN_DUMMY_TX,
    SECURITY_QUESTION,
    SECURITY_QUESTION_AND_SIGN_DUMMY_TX,   // NOT SUPPORT FOR NOW
    CONFIRMATION_CODE
};

enum class GROUP_WALLETS_STATUS {
    PENDING_WALLET,
    ACTIVE,
    DELETED
};

enum class GROUP_ALERTS_STATUS {
    UNREAD,
    READ
};

const QMap<int, QString> target_actions_command {
    { (int)TARGET_ACTION::EMERGENCY_LOCKDOWN        , "EMERGENCY_LOCKDOWN"        },
    { (int)TARGET_ACTION::DOWNLOAD_KEY_BACKUP       , "DOWNLOAD_KEY_BACKUP"       },
    { (int)TARGET_ACTION::UPDATE_SECURITY_QUESTIONS , "UPDATE_SECURITY_QUESTIONS" },
    { (int)TARGET_ACTION::UPDATE_INHERITANCE_PLAN   , "UPDATE_INHERITANCE_PLAN"   },
    { (int)TARGET_ACTION::UPDATE_SERVER_KEY         , "UPDATE_SERVER_KEY"         },
    { (int)TARGET_ACTION::DELETE_WALLET             , "DELETE_WALLET"             },
    { (int)TARGET_ACTION::EDIT_GROUP_MEMBERS        , "EDIT_GROUP_MEMBERS"        },
    { (int)TARGET_ACTION::CHANGE_EMAIL              , "CHANGE_EMAIL"              },
    { (int)TARGET_ACTION::REPLACE_KEYS              , "REPLACE_KEYS"              },
};

const QMap<QString, int> required_signatures_type {
    { "NONE"                                ,(int)REQUIRED_SIGNATURE_TYPE_INT::NONE                                },
    { "SIGN_MESSAGE"                        ,(int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_MESSAGE                        },
    { "SIGN_DUMMY_TX"                       ,(int)REQUIRED_SIGNATURE_TYPE_INT::SIGN_DUMMY_TX                       },
    { "SECURITY_QUESTION"                   ,(int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION                   },
    { "SECURITY_QUESTION_AND_SIGN_DUMMY_TX" ,(int)REQUIRED_SIGNATURE_TYPE_INT::SECURITY_QUESTION_AND_SIGN_DUMMY_TX },
    { "CONFIRMATION_CODE"                   ,(int)REQUIRED_SIGNATURE_TYPE_INT::CONFIRMATION_CODE },
};

const QMap<QString, int> group_wallets_status {
    { "PENDING_WALLET"  ,(int)GROUP_WALLETS_STATUS::PENDING_WALLET  },
    { "ACTIVE"          ,(int)GROUP_WALLETS_STATUS::ACTIVE          },
    { "DELETED"         ,(int)GROUP_WALLETS_STATUS::DELETED         },
};

const QMap<QString, int> group_alerts_status {
    { "UNREAD"          ,(int)GROUP_ALERTS_STATUS::UNREAD  },
    { "READ"            ,(int)GROUP_ALERTS_STATUS::READ    },
};

using namespace Command;
const QMap<int, QString> commands {
    { Common::CMD_IDX::CREATE_ACCOUNT               , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("register")             },
    { Common::CMD_IDX::ACCOUNT_AVAILABILITY         , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("username-availability")},
    { Common::CMD_IDX::SIGNIN                       , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("sign-in")              },
    { Common::CMD_IDX::SIGNOUT                      , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("log-out")              },
    { Common::CMD_IDX::RECOVER_PASSWORD             , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("recover-password")     },
    { Common::CMD_IDX::FORGOT_PASSWORD              , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("forgot-password")      },
    { Common::CMD_IDX::CHANGE_PASSWORD              , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("change-password")      },
    { Common::CMD_IDX::RESEND_PASSWORD              , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("resend-password")      },
    { Common::CMD_IDX::VERIFY_NEW_DEVICE            , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("sign-in/verify-new-device")},
    { Common::CMD_IDX::RESEND_VERIFY_NEW_DEVICE_CODE, QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("sign-in/resend-verify-new-device-code")},
    { Common::CMD_IDX::INVITE_FRIEND                , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("invite")               },
    { Common::CMD_IDX::GET_ME                       , QString("%1/%2").arg(DRAGON_USER_URL).arg("me")                       },
    { Common::CMD_IDX::SEARCH_FRIEND                , QString("%1/%2").arg(DRAGON_USER_URL).arg("search")                   },
    { Common::CMD_IDX::SEARCH_FRIEND_LIST           , QString("%1/%2").arg(DRAGON_USER_URL).arg("auto-complete-search")     },
    { Common::CMD_IDX::REQUEST_FRIEND               , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/request")         },
    { Common::CMD_IDX::REQUEST_FRIEND_LIST          , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/request")         },
    { Common::CMD_IDX::CANCEL_REQUEST_FRIEND        , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/request")         },
    { Common::CMD_IDX::IGNORE_FRIEND_REQUEST        , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/request")         },
    { Common::CMD_IDX::GET_FRIENDS_SENT             , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/request/sent")    },
    { Common::CMD_IDX::GET_FRIENDS_RECEIVED         , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/request/received")},
    { Common::CMD_IDX::GET_FRIENDS_LIST             , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/")                },
    { Common::CMD_IDX::REMOVE_FRIEND                , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/")                },
    { Common::CMD_IDX::ACCEPT_FRIEND_REQUEST        , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/accept")          },
    { Common::CMD_IDX::GET_ROOM_MEMBERS             , QString("%1/%2").arg(DRAGON_CHAT_URL).arg("rooms")                    },
    { Common::CMD_IDX::DELETE_ACCOUNT               , QString("%1/%2").arg(DRAGON_USER_URL).arg("me")                       },
    { Common::CMD_IDX::DELETE_CONFIRM               , QString("%1/%2").arg(DRAGON_USER_URL).arg("me/delete-confirmation")   },
    { Common::CMD_IDX::GET_DEVICES_LIST             , QString("%1/%2").arg(DRAGON_USER_URL).arg("devices/")                 },
    { Common::CMD_IDX::USER_PROFILE                 , QString("%1/%2").arg(DRAGON_USER_URL).arg("me")                       },
    { Common::CMD_IDX::REMOVE_AVATAR                , QString("%1/%2").arg(DRAGON_USER_URL).arg("me/avatar")                },
    { Common::CMD_IDX::CHANGE_AVATAR                , QString("%1/%2").arg(DRAGON_USER_URL).arg("me/change-avatar")         },
    { Common::CMD_IDX::CHECK_FOR_UPDATE             , QString("%1/%2").arg(DRAGON_APP_URL).arg("check-for-update")          },
    { Common::CMD_IDX::DEVICE_MARK_COMPROMISED      , QString("%1/%2").arg(DRAGON_USER_URL).arg("devices/mark-compromised") },
    { Common::CMD_IDX::DEVICE_SIGNOUT               , QString("%1/%2").arg(DRAGON_USER_URL).arg("devices/")                  },
    { Common::CMD_IDX::DEVICE_ALL_SIGNOUT           , QString("%1/%2").arg(DRAGON_USER_URL).arg("devices/__all")              },
    { Common::CMD_IDX::VERIFY_DELETE_DEVICES        , QString("%1/%2").arg(DRAGON_USER_URL).arg("devices/verify-delete-devices")},
    { Common::CMD_IDX::GET_ELECTRUM_SERVERS         , QString("%1/%2").arg(DRAGON_APP_URL).arg("electrum")                  },
    { PrimaryKey::CMD_IDX::PRIMARY_KEY_SIGNUP           , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("pkey/signup") },
    { PrimaryKey::CMD_IDX::PRIMARY_KEY_SIGNIN           , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("pkey/signin") },
    { PrimaryKey::CMD_IDX::PRIMARY_KEY_NONCE            , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("pkey/nonce")  },
    { PrimaryKey::CMD_IDX::PRIMARY_KEY_USERNAME         , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("username-availability/")  },
    { PrimaryKey::CMD_IDX::PRIMARY_KEY_PUBLIC_ADDRESS   , QString("%1/%2").arg(DRAGON_USER_URL).arg("pkey/")  },
    { PrimaryKey::CMD_IDX::PRIMARY_KEY_DELETE_CONFIRMTION, QString("%1/%2").arg(DRAGON_USER_URL).arg("pkey/delete-confirmation")  },
    { PrimaryKey::CMD_IDX::PRIMARY_KEY_CHANGE_KEY       , QString("%1/%2").arg(DRAGON_USER_URL).arg("pkey/change-pkey")  },

    // USER_SUBSCRIPTION
    { Common::CMD_IDX::USER_SUBCRIPTIONS_CURRENT    , QString("%1/%2").arg(DRAGON_SUBSCRIPTIONS_URL).arg("current")  },
    { Common::CMD_IDX::USER_SUBCRIPTIONS_TESTNET    , QString("%1/%2").arg(DRAGON_SUBSCRIPTIONS_URL).arg("testnet")  },
    { Common::CMD_IDX::USER_SUBCRIPTIONS_STATUS             , QString("%1/%2").arg(DRAGON_SUBSCRIPTIONS_URL).arg("status")  },
    { Common::CMD_IDX::USER_SUBCRIPTIONS_STATUS_TESTNET     , QString("%1/%2").arg(DRAGON_SUBSCRIPTIONS_URL).arg("status-testnet")  },

    // ASSISTED_WALLETS
    { Premium::CMD_IDX::ASSISTED_WALLET_GET          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets")  },
    { Premium::CMD_IDX::ASSISTED_WALLET_CREATE_TX    , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions")  },
    { Premium::CMD_IDX::ASSISTED_WALLET_UPDATE_TX    , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions/{transaction_id}")  },
    { Premium::CMD_IDX::ASSISTED_WALLET_GET_LIST_TX  , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions")  },
    { Premium::CMD_IDX::ASSISTED_WALLET_GET_LIST_TX_NOTES  , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions/notes")  },
    { Premium::CMD_IDX::ASSISTED_WALLET_DELETE_LIST_TX     , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions")  },
    { Premium::CMD_IDX::ASSISTED_WALLET_SIGN_TX      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions/{transaction_id}/sign") },
    { Premium::CMD_IDX::ASSISTED_WALLET_CANCEL_TX    , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions/{transaction_id}") },
    { Premium::CMD_IDX::ASSISTED_WALLET_GET_TX       , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions/{transaction_id}") },
    { Premium::CMD_IDX::ASSISTED_WALLET_SYNC_TX      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions/{transaction_id}/sync") },
    { Premium::CMD_IDX::ASSISTED_WALLET_RBF_TX       , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions/{transaction_id}/rbf") },
    { Premium::CMD_IDX::ASSISTED_WALLET_ADD_KEY      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/add-key") },
    { Premium::CMD_IDX::ASSISTED_WALLET_GET_LIST_KEY , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/request-add-key") },
    { Premium::CMD_IDX::ASSISTED_WALLET_REMOVE_ID    , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/request-add-key/{request_id}") },
    { Premium::CMD_IDX::ASSISTED_WALLET_GET_CONFIG   , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("configs") },
    { Premium::CMD_IDX::ASSISTED_KEY_UPDATE_NAME     , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallet-keys/{xfp}") },
    { Premium::CMD_IDX::ASSISTED_WALLET_GET_INFO     , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}") },
    { Premium::CMD_IDX::ASSISTED_WALLET_UPDATE_NAME  , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}") },
    { Premium::CMD_IDX::ASSISTED_WALLET_REMOVE_WALLET, QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}") },
    { Premium::CMD_IDX::ASSISTED_WALLET_REMOVE_WALLET_REQUIRED_SIGNATURES, QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/calculate-required-signatures") },

    // SEC_QUES
    { Premium::CMD_IDX::SEC_QUES_GET                 , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("security-questions") },
    { Premium::CMD_IDX::SEC_QUES_SET                 , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("security-questions") },
    { Premium::CMD_IDX::SEC_QUES_CREATE              , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("security-questions") },
    { Premium::CMD_IDX::SEC_QUES_UPDATE              , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("security-questions/update") },
    { Premium::CMD_IDX::SEC_QUES_VERIFY_ANSWER       , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("security-questions/verify-answer") },
    { Premium::CMD_IDX::SEC_QUES_REQUIRED_SIGNATURES , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("security-questions/calculate-required-signatures") },

    // LOCK_DOWN
    { Premium::CMD_IDX::LOCKDOWN_SET_ANSWER_SECURITY_QUESTION      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("lockdown/lock") },
    { Premium::CMD_IDX::LOCKDOWN_SET_SIGN_DUMMY_TX                 , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("lockdown/lock") },
    { Premium::CMD_IDX::LOCKDOWN_REQUIRED_SIGNATURES , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("lockdown/calculate-required-signatures") },
    { Premium::CMD_IDX::LOCKDOWN_GET_PERIOD          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("lockdown/period") },

    //USER_WALLET
    { Premium::CMD_IDX::GET_RANDOM_NONCE             , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("nonce") },
    { Common::CMD_IDX::VERIFY_PASSWORD_TOKEN        , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("verified-password-token")  },

    //USER_KEYS
    { Premium::CMD_IDX::USER_KEYS_MARK_A_KEY_AS_VERIFIED,        QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("user-keys/{key_id_or_xfp}/verify") },
    { Premium::CMD_IDX::USER_KEYS_REQUEST_RECOVERY_KEY,          QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("user-keys/{key_id_or_xfp}/request-recover") },
    { Premium::CMD_IDX::USER_KEYS_RECOVERY_KEY,                  QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("user-keys/{key_id_or_xfp}/recover") },
    { Premium::CMD_IDX::USER_KEYS_MARK_RECOVER_STATUS,           QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("user-keys/{key_id_or_xfp}/mark-recover-status") },
    { Premium::CMD_IDX::USER_KEYS_DOWNLOAD_BACKUP,               QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("user-keys/{key_id_or_xfp}/download-backup") },
    { Premium::CMD_IDX::USER_KEYS_CALCULATE_REQUIRED_SIGNATURES, QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("user-keys/{key_id_or_xfp}/calculate-required-signatures") },
    { Premium::CMD_IDX::USER_KEYS_UPLOAD_BACKUP,                 QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("user-keys/upload-backup") },
    { Premium::CMD_IDX::USER_KEYS_GET_LIST_USER_KEYS,            QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("user-keys") },
    { Premium::CMD_IDX::USER_KEYS_GET_USER_KEY,                  QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("user-keys/{key_id_or_xfp}") },

    //INHERITANCE
    { Premium::CMD_IDX::INHERITANCE_CLAIM_STATUS     , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance/claiming/status") },
    { Premium::CMD_IDX::INHERITANCE_CLAIM_INIT       , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance/claiming/init") },
    { Premium::CMD_IDX::INHERITANCE_CLAIM_DOWNLOAD_WALLET, QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance/claiming/download-wallet") },
    { Premium::CMD_IDX::INHERITANCE_CLAIM_DOWNLOAD_BACKUP  , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance/claiming/download-backups") },
    { Premium::CMD_IDX::INHERITANCE_CLAIM_REQUEST    , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance/claiming/claim") },
    { Premium::CMD_IDX::INHERITANCE_CLAIM_CREATE_TX  , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance/claiming/create-transaction") },
    { Premium::CMD_IDX::INHERITANCE_CHECK            , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance/check") },
    { Premium::CMD_IDX::INHERITANCE_GET_PLAN         , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance") },
    { Premium::CMD_IDX::INHERITANCE_FAKE_UPDATE      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance/fake-update") },
    { Premium::CMD_IDX::INHERITANCE_PLAN_REQUIRED_SIGNATURES      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance/calculate-required-signatures") },
    { Premium::CMD_IDX::INHERITANCE_PLAN_UPDATE      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance") },
    { Premium::CMD_IDX::INHERITANCE_PLAN_CREATE      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance") },
    { Premium::CMD_IDX::INHERITANCE_PLAN_CANCEL      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance") },
    { Premium::CMD_IDX::INHERITANCE_PLAN_BUFFER_PERIODS , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance/buffer-period") },
    { Premium::CMD_IDX::INHERITANCE_REQUEST_PLANING_DENY,    QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance/request-planning/{request_id}/deny") },
    { Premium::CMD_IDX::INHERITANCE_REQUEST_PLANING_APPROVE, QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("inheritance/request-planning/{request_id}/approve") },

    //SERVER_KEYS /v1.1/user-wallets/server-keys/{key_id_or_xfp}
    { Premium::CMD_IDX::SERVER_KEYS_GET              , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("server-keys/{key_id_or_xfp}") },
    { Premium::CMD_IDX::SERVER_KEYS_UPDATE           , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("server-keys/{key_id_or_xfp}") },
    { Premium::CMD_IDX::SERVER_KEYS_REQUIRED_SIGNATURES, QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("server-keys/{key_id_or_xfp}/calculate-required-signatures") },

    { Premium::CMD_IDX::GET_CURRENCIES               , QString("%1/%2").arg(DRAGON_FOREX_URL).arg("currencies") },

    { Premium::CMD_IDX::ASSISTED_WALLET_ALERTS               , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/alerts") },
    { Premium::CMD_IDX::ASSISTED_WALLET_ALERTS_COUNT         , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/alerts/total") },
    { Premium::CMD_IDX::ASSISTED_WALLET_ALERTS_READ          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/alerts/{alert_id}/mark-as-read") },
    { Premium::CMD_IDX::ASSISTED_WALLET_ALERTS_DISMISS       , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/alerts/{alert_id}/dismiss") },

    // Wallet health check
    { Premium::CMD_IDX::ASSISTED_WALLET_GET_KEY_HEALTH_REMINDER             , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/health/reminder") },
    { Premium::CMD_IDX::ASSISTED_WALLET_ADD_OR_UPDATE_KEY_HEALTH_REMINDER   , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/health/reminder") },
    { Premium::CMD_IDX::ASSISTED_WALLET_DELETE_KEY_HEALTH_REMINDER          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/health/reminder") },
    { Premium::CMD_IDX::ASSISTED_WALLET_SKIP_KEY_HEALTH_REMINDER            , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/health/reminder/{xfp}") },
    { Premium::CMD_IDX::ASSISTED_WALLET_HEALTHCHECK        , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/health/{xfp}/health-check") },
    { Premium::CMD_IDX::ASSISTED_WALLET_GET_HEALTH         , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/health") },

    //Dummy transaction
    { Premium::CMD_IDX::ASSISTED_WALLET_GET_DUMMY_TX       , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/dummy-transactions/{dummy_transaction_id}") },
    { Premium::CMD_IDX::ASSISTED_WALLET_UPDATE_DUMMY_TX    , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/dummy-transactions/{dummy_transaction_id}") },
    { Premium::CMD_IDX::ASSISTED_WALLET_CANCEL_DUMMY_TX    , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/dummy-transactions/{dummy_transaction_id}") },
    { Premium::CMD_IDX::ASSISTED_WALLET_FINALIZE_DUMMY_TX  , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/dummy-transactions/{dummy_transaction_id}/finalize") },

    { Premium::CMD_IDX::ASSISTED_WALLET_HEALTH_CHECK_HISTORY_GET  , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallet-keys/{xfp}/history") },

    { Premium::CMD_IDX::COUNTRY_CODE_LIST             , QString("%1/%2").arg(DRAGON_BANNERS_URL).arg("onboarding/countries") },
    { Premium::CMD_IDX::ONBOARDING_NO_ADVISOR         , QString("%1/%2").arg(DRAGON_BANNERS_URL).arg("onboarding/no-advisor") },
    { Premium::CALCULATE_REQUIRED_SIGNATURES_FOR_CHANGING_EMAIL , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("email-change/calculate-required-signatures") },
    { Premium::CHANGING_EMAIL                                   , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("email-change/change") },

    // Replace Wallet
    { Premium::CMD_IDX::ASSISTED_WALLET_CONFIGURE_WALLET_REPLACEMENT    , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/replacement/configs") },
    { Premium::CMD_IDX::ASSISTED_WALLET_INITIATE_KEY_REPLACEMENT        , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/replacement/{xfp}") },
    { Premium::CMD_IDX::ASSISTED_WALLET_CANCEL_KEY_REPLACEMENT          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/replacement/{xfp}") },
    { Premium::CMD_IDX::ASSISTED_WALLET_VERIFY_KEY_REPLACEMENT          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/replacement/{xfp}/verify") },
    { Premium::CMD_IDX::ASSISTED_WALLET_REMOVE_KEY_REPLACEMENT          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/replacement/{xfp}/remove") },
    { Premium::CMD_IDX::ASSISTED_WALLET_REPLACE_KEY                     , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/replacement/{xfp}/replace") },
    { Premium::CMD_IDX::ASSISTED_WALLET_FINALIZE_KEY_REPLACEMENT        , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/replacement/finalize") },
    { Premium::CMD_IDX::ASSISTED_WALLET_GET_STATUS_KEY_REPLACEMENT      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/replacement/status") },
    { Premium::CMD_IDX::ASSISTED_WALLET_REPLACEMENT_TIMELOCK            , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/replacement/timelock") },
    { Premium::CMD_IDX::ASSISTED_WALLET_RESET_KEY_REPLACEMENT           , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/replacement/reset") },
    { Premium::CMD_IDX::ASSISTED_WALLET_DOWNLOAD_BACKUP_KEY_REPLACEMENT         , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/replacement/{xfp}/download-backup") },
    { Premium::CMD_IDX::ASSISTED_WALLET_UPLOAD_BACKUP_KEY_REPLACEMENT           , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/replacement/upload-backup") },

    // Draft Wallet
    { Premium::CMD_IDX::DRAFT_WALLET_MARK_ALERT_AS_READ            , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/current/alerts/{alert_id}/mark-as-read") },
    { Premium::CMD_IDX::DRAFT_WALLET_DISMISS_ALERT                 , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/current/alerts/{alert_id}/dismiss") },
    { Premium::CMD_IDX::DRAFT_WALLET_GET_ALERTS                    , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/current/alerts") },
    { Premium::CMD_IDX::DRAFT_WALLET_GET_TOTAL_ALERTS              , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/current/alerts/total") },
    { Premium::CMD_IDX::DRAFT_WALLET_SET_SERVER_KEY                , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/set-server-key") },
    { Premium::CMD_IDX::DRAFT_WALLET_REQUEST_ADD_KEY               , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/request-add-key") },
    { Premium::CMD_IDX::DRAFT_WALLET_PUSH_REQUEST_ADD_KEY          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/request-add-key/{request_id}/push") },
    { Premium::CMD_IDX::DRAFT_WALLET_INIT                          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/init") },
    { Premium::CMD_IDX::DRAFT_WALLET_GET_REQUEST_ADD_KEY           , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/request-add-key/{request_id}") },
    { Premium::CMD_IDX::DRAFT_WALLET_GET_CURRENT                   , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/current") },
    { Premium::CMD_IDX::DRAFT_WALLET_RESET_CURRENT                 , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/current") },
    { Premium::CMD_IDX::DRAFT_WALLET_DOWNLOAD_BACKUP               , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/{xfp}/download-backup") },
    { Premium::CMD_IDX::DRAFT_WALLET_UPLOAD_BACKUP                 , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/upload-backup") },
    { Premium::CMD_IDX::DRAFT_WALLET_VERIFY                        , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/{xfp}/verify") },

    //DRAGON_GROUP_WALLETS
    { Group::CMD_IDX::GROUP_WALLET_LIST_WALLETS         , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("wallets") },
    { Group::CMD_IDX::GROUP_WALLETS_ONE                 , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}") },
    { Group::CMD_IDX::GROUP_WALLETS_LIST                , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups") },
    { Group::CMD_IDX::GROUP_WALLETS_LIST_DELETED        , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("wallets/deleted") },
    { Group::CMD_IDX::GROUP_WALLETS_ACCEPT              , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/members/accept") },
    { Group::CMD_IDX::GROUP_WALLETS_DENY                , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/members/deny") },
    { Group::CMD_IDX::GROUP_WALLET_EDIT_GROUP_MEMBERS                       , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/members") },
    { Group::CMD_IDX::GROUP_WALLET_EDIT_GROUP_MEMBERS_REQUIRED_SIGNATURES   , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/members/calculate-requires-signatures") },


    { Group::CMD_IDX::GROUP_WALLET_ALERTS               , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/alerts") },
    { Group::CMD_IDX::GROUP_WALLET_ALERTS_COUNT         , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/alerts/total") },
    { Group::CMD_IDX::GROUP_WALLET_ALERTS_READ          , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/alerts/{alert_id}/mark-as-read") },
    { Group::CMD_IDX::GROUP_WALLET_ALERTS_DISMISS       , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/alerts/{alert_id}/dismiss") },
    { Group::CMD_IDX::GROUP_WALLETS_ALL_PERMISSION      , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("permissions") },
    { Group::CMD_IDX::GROUP_WALLETS_ONE_PERMISSION      , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("permissions/{slug}") },
    { Group::CMD_IDX::GROUP_WALLETS_DEFAULT_PERMISSION  , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("permissions/default") },
    { Group::CMD_IDX::GROUP_WALLETS_GET_CURRENT         , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/current") },
    { Group::CMD_IDX::GROUP_WALLET_REMOVE_WALLET        , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}") },
    { Group::CMD_IDX::GROUP_WALLETS_GET_WALLET        , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}") },    
    { Group::CMD_IDX::GROUP_WALLET_REMOVE_WALLET_REQUIRED_SIGNATURES, QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/calculate-required-signatures") },
    { Group::CMD_IDX::GROUP_WALLET_GET_ALIAS            , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/alias") },
    { Group::CMD_IDX::GROUP_WALLET_UPDATE_ALIAS         , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/alias") },
    { Group::CMD_IDX::GROUP_WALLET_DELETE_ALIAS         , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/alias") },
    { Group::CMD_IDX::GROUP_WALLET_UPDATE_PRIMARY_OWNER , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/primary-owner") },
    { Group::CMD_IDX::GROUP_WALLET_UPDATE_WALLET        , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}") },
    { Group::CMD_IDX::GROUP_DRAFT_WALLET_VERIFY         , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/draft-wallets/{xfp}/verify") },

    { Group::CMD_IDX::GROUP_DRAFT_WALLETS_ADD_KEY               , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/draft-wallets/add-key") },
    { Group::CMD_IDX::GROUP_DRAFT_WALLETS_GET_CURRENT           , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/draft-wallets/current") },
    { Group::CMD_IDX::GROUP_DRAFT_WALLETS_RESET_CURRENT         , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/draft-wallets/current") },
    { Group::CMD_IDX::GROUP_DRAFT_WALLETS_REQUEST_ADD_KEY       , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/draft-wallets/request-add-key") },
    { Group::CMD_IDX::GROUP_DRAFT_WALLETS_DELETE_REQUEST_ADD_KEY, QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/draft-wallets/request-add-key/{request_id}") },
    { Group::CMD_IDX::GROUP_DRAFT_WALLETS_DOWNLOAD_BACKUP       , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/draft-wallets/{xfp}/download-backup") },
    { Group::CMD_IDX::GROUP_DRAFT_WALLETS_UPLOAD_BACKUP         , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/draft-wallets/upload-backup") },

    // Group wallet health check
    { Group::CMD_IDX::GROUP_WALLETS_GET_KEY_HEALTH_REMINDER             , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/health/reminder") },
    { Group::CMD_IDX::GROUP_WALLETS_ADD_OR_UPDATE_KEY_HEALTH_REMINDER   , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/health/reminder") },
    { Group::CMD_IDX::GROUP_WALLETS_DELETE_KEY_HEALTH_REMINDER          , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/health/reminder") },
    { Group::CMD_IDX::GROUP_WALLETS_SKIP_KEY_HEALTH_REMINDER            , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/health/reminder/{xfp}") },
    { Group::CMD_IDX::GROUP_WALLETS_REQUEST_HEALTHCHECK, QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/health/{xfp}/request-health-check") },
    { Group::CMD_IDX::GROUP_WALLETS_HEALTHCHECK        , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/health/{xfp}/health-check") },
    { Group::CMD_IDX::GROUP_WALLETS_GET_HEALTH         , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/health") },

    //Dummy transaction
    { Group::CMD_IDX::GROUP_WALLETS_GET_DUMMY_TX       , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/dummy-transactions/{dummy_transaction_id}") },
    { Group::CMD_IDX::GROUP_WALLETS_UPDATE_DUMMY_TX    , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/dummy-transactions/{dummy_transaction_id}") },
    { Group::CMD_IDX::GROUP_WALLETS_CANCEL_DUMMY_TX    , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/dummy-transactions/{dummy_transaction_id}") },
    { Group::CMD_IDX::GROUP_WALLETS_FINALIZE_DUMMY_TX  , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/dummy-transactions/{dummy_transaction_id}/finalize") },

    // Group transactions
    { Group::CMD_IDX::GROUP_WALLETS_GET_ALL_TX           , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/transactions") },
    { Group::CMD_IDX::GROUP_WALLETS_GET_ALL_TX_NOTES     , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/transactions/notes") },
    { Group::CMD_IDX::GROUP_WALLETS_GET_ONE_TX           , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/transactions/{transaction_id}") },
    { Group::CMD_IDX::GROUP_WALLETS_UPDATE_TX            , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/transactions/{transaction_id}") },
    { Group::CMD_IDX::GROUP_WALLETS_CANCEL_TX            , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/transactions/{transaction_id}") },
    { Group::CMD_IDX::GROUP_WALLETS_SYNC_TX              , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/transactions/{transaction_id}/sync") },
    { Group::CMD_IDX::GROUP_WALLETS_SIGN_TX              , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/transactions/{transaction_id}/sign") },
    { Group::CMD_IDX::GROUP_WALLETS_CREATE_TX            , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/transactions") },
    { Group::CMD_IDX::GROUP_WALLETS_SCHEDULE_TX          , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/transactions/{transaction_id}/schedule") },
    { Group::CMD_IDX::GROUP_WALLETS_CANCEL_SCHEDULE_TX   , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/transactions/{transaction_id}/schedule") },
    { Group::CMD_IDX::GROUP_WALLETS_REQUEST_SIGNATURE_TX , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/transactions/{transaction_id}/request-signature") },
    { Group::CMD_IDX::GROUP_WALLETS_RBF_TX              , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/transactions/{transaction_id}/rbf") },

    // Group server-keys
    { Group::CMD_IDX::SERVER_KEYS_GET     , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/server-keys/{key_id_or_xfp}") },
    { Group::CMD_IDX::SERVER_KEYS_UPDATE  , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/server-keys/{key_id_or_xfp}") },
    { Group::CMD_IDX::SERVER_KEYS_REQUIRED_SIGNATURES , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/server-keys/{key_id_or_xfp}/calculate-required-signatures") },

    { Group::CMD_IDX::LOCKDOWN_GET_PERIOD                   , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("lockdown/period") },
    { Group::CMD_IDX::LOCKDOWN_REQUIRED_SIGNATURES          , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("lockdown/calculate-required-signatures") },
    { Group::CMD_IDX::LOCKDOWN_SET_ANSWER_SECURITY_QUESTION , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("lockdown/lock") },
    { Group::CMD_IDX::LOCKDOWN_SET_SIGN_DUMMY_TX            , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("lockdown/lock") },
    { Group::CMD_IDX::LOCKDOWN_SET_CONFIRMATION_CODE        , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("lockdown/lock") },

    { Group::CMD_IDX::ASSISTED_WALLET_GET_CONFIG            , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("configs") },

    { Premium::CMD_IDX::CONFIRMATION_CODE                 , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("confirmation-code") },
    { Premium::CMD_IDX::VERIFY_CONFIRMATION_CODE          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("confirmation-code/{code_id}/verify") },

    // Group chat
    { Group::CMD_IDX::GROUP_WALLETS_CHAT_GET           , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("chat/{group_id}/current") },
    { Group::CMD_IDX::GROUP_WALLETS_CHAT_UPDATE        , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("chat/{group_id}/current") },
    { Group::CMD_IDX::GROUP_WALLETS_CHAT_DELETE        , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("chat/{group_id}/current") },
    { Group::CMD_IDX::GROUP_WALLETS_CHAT_CREATE        , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("chat/{group_id}") },
    { Group::CMD_IDX::GROUP_WALLETS_CHAT_GET_ALL       , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("chat") },
    { Group::CMD_IDX::GROUP_WALLETS_CHAT_GET_HISTORY_PERIODS , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("chat/settings/history-periods") },

    // Recurring payment
    { Group::CMD_IDX::GROUP_WALLET_RECURRING_PAYMENT_LIST    , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/recurring-payment") },
    { Group::CMD_IDX::GROUP_WALLET_RECURRING_PAYMENT_CREATE  , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/recurring-payment") },
    { Group::CMD_IDX::GROUP_WALLET_RECURRING_PAYMENT_GET     , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/recurring-payment/{recurring_payment_id}") },
    { Group::CMD_IDX::GROUP_WALLET_RECURRING_PAYMENT_CANCEL  , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/recurring-payment/{recurring_payment_id}") },

    // Replace Wallet
    { Group::CMD_IDX::GROUP_WALLET_CONFIGURE_WALLET_REPLACEMENT    , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/replacement/configs") },
    { Group::CMD_IDX::GROUP_WALLET_INITIATE_KEY_REPLACEMENT        , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/replacement/{xfp}") },
    { Group::CMD_IDX::GROUP_WALLET_CANCEL_KEY_REPLACEMENT          , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/replacement/{xfp}") },
    { Group::CMD_IDX::GROUP_WALLET_VERIFY_KEY_REPLACEMENT          , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/replacement/{xfp}/verify") },
    { Group::CMD_IDX::GROUP_WALLET_REMOVE_KEY_REPLACEMENT          , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/replacement/{xfp}/remove") },
    { Group::CMD_IDX::GROUP_WALLET_REPLACE_KEY                     , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/replacement/{xfp}/replace") },
    { Group::CMD_IDX::GROUP_WALLET_FINALIZE_KEY_REPLACEMENT        , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/replacement/finalize") },
    { Group::CMD_IDX::GROUP_WALLET_GET_STATUS_KEY_REPLACEMENT      , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/replacement/status") },
    { Group::CMD_IDX::GROUP_WALLET_REPLACEMENT_TIMELOCK            , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/replacement/timelock") },
    { Group::CMD_IDX::GROUP_WALLET_RESET_KEY_REPLACEMENT           , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/replacement/reset") },
    { Group::CMD_IDX::GROUP_WALLET_DOWNLOAD_BACKUP_KEY_REPLACEMENT , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/replacement/{xfp}/download-backup") },
    { Group::CMD_IDX::GROUP_WALLET_UPLOAD_BACKUP_KEY_REPLACEMENT   , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/replacement/upload-backup") },

    // Saved addresses
    { Premium::CMD_IDX::ASSISTED_WALLET_GET_SAVED_ADDRESSES        , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("saved-address") },
    { Premium::CMD_IDX::ASSISTED_WALLET_ADD_SAVED_ADDRESS          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("saved-address") },
    { Premium::CMD_IDX::ASSISTED_WALLET_DELETE_SAVED_ADDRESS       , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("saved-address/{address}") },

    { Premium::CMD_IDX::SIGN_DUMMY_TRANSACTION_TO_SIGN_IN          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("signin-dummy/{dummy_transaction_id}") },
    { Premium::CMD_IDX::SIGN_IN_USING_XPUB_WALLET                  , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("signin-dummy") },

    //Coin Control
    { Premium::CMD_IDX::ASSISTED_WALLET_COINCONTROL_GET          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/coin-control") },
    { Premium::CMD_IDX::ASSISTED_WALLET_COINCONTROL_UPDATE       , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/coin-control") },
    // Coin Control Group
    { Group::CMD_IDX::GROUP_WALLET_COINCONTROL_GET               , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/coin-control") },
    { Group::CMD_IDX::GROUP_WALLET_COINCONTROL_UPDATE            , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/wallets/{wallet_id_or_local_id}/coin-control") },

    // Check supported signers
    { Common::CMD_IDX::TAPROOT_SUPPORTED_SIGNERS                 , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("taproot/supported-signers") },

    // Miniscript
    { Command::Premium::CONFIG_WALLET_SETUP                      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("configs/setup") },
    { Command::Premium::DRAFT_WALLET_TIMELOCK                    , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("draft-wallets/timelock") },
    { Command::Premium::TIMELOCK_CONVERT                         , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("timelock/convert") },
    
    // Group Miniscript
    { Command::Group::GROUP_DRAFT_WALLET_TIMELOCK                , QString("%1/%2").arg(DRAGON_GROUP_WALLETS_URL).arg("groups/{group_id}/draft-wallets/timelock") },

    // Claiming wallets
    { Premium::CMD_IDX::CLAIMING_WALLET_GET_BY_ID      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("claiming-wallets/{local_id}") },
    { Premium::CMD_IDX::CLAIMING_WALLET_UPDATE_BY_ID   , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("claiming-wallets/{local_id}") },
    { Premium::CMD_IDX::CLAIMING_WALLET_DELETE_BY_ID   , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("claiming-wallets/{local_id}") },
    { Premium::CMD_IDX::CLAIMING_WALLET_LIST           , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("claiming-wallets") },

    // Claiming wallets transactions
    { Premium::CMD_IDX::CLAIMING_WALLET_TX_GET_LIST     , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("claiming-wallets/{local_id}/transactions") },
    { Premium::CMD_IDX::CLAIMING_WALLET_TX_GET_ONE      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("claiming-wallets/{local_id}/transactions/{transaction_id}") },
    { Premium::CMD_IDX::CLAIMING_WALLET_TX_CREATE_UPDATE, QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("claiming-wallets/{local_id}/transactions") },
    { Premium::CMD_IDX::CLAIMING_WALLET_TX_CREATE_BATCH , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("claiming-wallets/{local_id}/transactions/batch") },
    { Premium::CMD_IDX::CLAIMING_WALLET_TX_CANCEL       , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("claiming-wallets/{local_id}/transactions/{transaction_id}") },
    { Premium::CMD_IDX::CLAIMING_WALLET_TX_RBF          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("claiming-wallets/{local_id}/transactions/{transaction_id}/rbf") },
};

class DRACO_CODE: public QObject
{
    Q_OBJECT
    Q_ENUMS(RESPONSE_CODE)
    Q_ENUMS(REGISTER)
    Q_ENUMS(CHANGE_PASSWORD)
    Q_ENUMS(FORGOT_PASSWORD)
    Q_ENUMS(RECOVER_PASSWORD)
    Q_ENUMS(SIGN_IN)
public:
    enum RESPONSE_CODE
    {
        SUCCESSFULL  = 200,
        BAD_REQUEST  = 400,
        UNAUTHORIZED = 401,
        NOT_FOUND    = 404,
        INTERNAL_SERVER_ERROR = 500,
        INHERITANCE_801 = 801,
        INHERITANCE_802 = 802,
        INHERITANCE_803 = 803,
        INHERITANCE_829 = 829,
        INHERITANCE_830 = 830,
        INHERITANCE_831 = 831,
        LOGIN_NEW_DEVICE = 841,
        ACCOUNT_NOT_ACTIVATED = 1000,
        RESPONSE_OK  = 0,
    };

    enum REGISTER {
        REGISTER_EMAIL_ALREADY_USED     = -100, //"This email address is already being used"
        REGISTER_EMAIL_NOT_EXISTING     = -101, //"Can't create user profile"
        REGISTER_EXCEPTION              = -102, //ex.getMessage()
    };

    enum CHANGE_PASSWORD {
        CHANGE_PASSWORD_FAIL      = -115, // "Can't change password"
        CHANGE_PASSWORD_EXCEPTION = -116, // ex.getMessage()
    };

    enum FORGOT_PASSWORD {
        FORGOT_PASSWORD_EMAIL_NOT_EXISTING  = -130, //"This email address does not exist"
        FORGOT_PASSWORD_ACCOUNT_NOT_ACTIVED = -131, //"This account has not been activated"
        FORGOT_PASSWORD_EXCEPTION           = -139, //ex.getMessage()
    };

    enum RECOVER_PASSWORD {
        RECOVER_PASSWORD_TOKEN_INVALID_0 = -140, //"Invalid recover token key"
        RECOVER_PASSWORD_TOKEN_INVALID_1 = -141, //"Invalid recover token key"
        RECOVER_PASSWORD_TOKEN_EXPIRED   = -142, //" Recover token key expired"
        RECOVER_PASSWORD_RESET_FAIL      = -148, //"Can't reset password"
        RECOVER_PASSWORD_RESET_EXCEPTION = -149, //ex.getMessage()
    };

    enum SIGN_IN {
        SIGN_IN_FAIL        = -150, //"Wrong email or password"
        SIGN_IN_EXCEPTION   = -159, //ex.getMessage()
    };
};

#endif // DRACODEFINES_H
