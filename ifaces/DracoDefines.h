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

#define DRAGON_PASSPORT_URL     "https://api.nunchuk.io/v1.1/passport"
#define DRAGON_USER_URL         "https://api.nunchuk.io/v1.1/user"
#define DRAGON_CHAT_URL         "https://api.nunchuk.io/v1.1/chat"
#define DRAGON_APP_URL          "https://api.nunchuk.io/v1.1/app"
#define DRAGON_FOREX_URL        "https://api.nunchuk.io/v1.1/forex"

#define DRAGON_SUBSCRIPTIONS_URL "https://api.nunchuk.io/v1.1/subscriptions"
#define DRAGON_USER_WALLETS_URL  "https://api.nunchuk.io/v1.1/user-wallets"

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

struct lockDownReqiredInfo {
    int     type = 0;
    int     required_signatures = 0;
    int     required_answers = 0;
};

enum CMD_IDX {
    CREATE_ACCOUNT = 0,
    SIGNOUT,
    SIGNIN,
    RECOVER_PASSWORD,
    FORGOT_PASSWORD,
    CHANGE_PASSWORD,
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
    CHECK_FOR_UPDATE,
    DEVICE_MARK_COMPROMISED,
    DEVICE_SIGNOUT,
    VERIFY_DELETE_DEVICES,
    DEVICE_ALL_SIGNOUT,
    PRIMARY_KEY_SIGNUP,
    PRIMARY_KEY_SIGNIN,
    PRIMARY_KEY_NONCE,
    PRIMARY_KEY_USERNAME,
    PRIMARY_KEY_PUBLIC_ADDRESS,
    PRIMARY_KEY_DELETE_CONFIRMTION,
    PRIMARY_KEY_CHANGE_KEY,

    // USER_SUBSCRIPTION
    USER_SUBCRIPTIONS_CURRENT,

    // ASSISTED_WALLETS
    ASSISTED_WALLET_GET,
    ASSISTED_WALLET_CREATE_TX,
    ASSISTED_WALLET_SIGN_TX,
    ASSISTED_WALLET_CANCEL_TX,
    ASSISTED_WALLET_GET_TX,
    ASSISTED_WALLET_GET_LIST_TX,

    // SEC_QUES
    SEC_QUES_GET,
    SEC_QUES_SET,
    SEC_QUES_CREATE,
    SEC_QUES_UPDATE,
    SEC_QUES_VERIFY_ANSWER,
    SEC_QUES_REQUIRED_SIGNATURES,

    //LOCK_DOWN
    LOCKDOWN_SET,
    LOCKDOWN_REQUIRED_SIGNATURES,
    LOCKDOWN_GET_PERIOD,

    //GET_RANDOM_NONCE
    GET_RANDOM_NONCE,
    VERIFY_PASSWORD_TOKEN,

    //USER_KEYS
    USER_KEYS_DOWNLOAD_BACKUP,

    TX_SYNC,
    GET_CURRENCIES,

    CMD_MAX
};

enum class TARGET_ACTION {
    EMERGENCY_LOCKDOWN = 0,
    DOWNLOAD_KEY_BACKUP,
    UPDATE_SEC_QUES,
    UPDATE_INHERITANCE_PLAN,
    UPDATE_SERVER_KEY,
};

enum class LOCKDOWN_REQUIRED_TYPE_INT {
    NONE,           // ERROR
    SIGN_MESSAGE,   // NOT SUPPORT FOR NOW
    SIGN_DUMMY_TX,
    SECURITY_QUESTION,
    SECURITY_QUESTION_AND_SIGN_DUMMY_TX   // NOT SUPPORT FOR NOW
};

const QMap<int, QString> target_actions_command {
    { (int)TARGET_ACTION::EMERGENCY_LOCKDOWN        , "EMERGENCY_LOCKDOWN"      },
    { (int)TARGET_ACTION::DOWNLOAD_KEY_BACKUP       , "DOWNLOAD_KEY_BACKUP"     },
    { (int)TARGET_ACTION::UPDATE_SEC_QUES           , "UPDATE_SEC_QUES"         },
    { (int)TARGET_ACTION::UPDATE_INHERITANCE_PLAN   , "UPDATE_INHERITANCE_PLAN" },
    { (int)TARGET_ACTION::UPDATE_SERVER_KEY         , "UPDATE_SERVER_KEY"       },
};

const QMap<QString, int> lockdown_required_type {
    { "NONE"                                ,(int)LOCKDOWN_REQUIRED_TYPE_INT::NONE                                },
    { "SIGN_MESSAGE"                        ,(int)LOCKDOWN_REQUIRED_TYPE_INT::SIGN_MESSAGE                        },
    { "SIGN_DUMMY_TX"                       ,(int)LOCKDOWN_REQUIRED_TYPE_INT::SIGN_DUMMY_TX                       },
    { "SECURITY_QUESTION"                   ,(int)LOCKDOWN_REQUIRED_TYPE_INT::SECURITY_QUESTION                   },
    { "SECURITY_QUESTION_AND_SIGN_DUMMY_TX" ,(int)LOCKDOWN_REQUIRED_TYPE_INT::SECURITY_QUESTION_AND_SIGN_DUMMY_TX },
};

const QMap<int, QString> commands {
    { CMD_IDX::CREATE_ACCOUNT               , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("register")             },
    { CMD_IDX::SIGNIN                       , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("sign-in")              },
    { CMD_IDX::SIGNOUT                      , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("log-out")              },
    { CMD_IDX::RECOVER_PASSWORD             , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("recover-password")     },
    { CMD_IDX::FORGOT_PASSWORD              , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("forgot-password")      },
    { CMD_IDX::CHANGE_PASSWORD              , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("change-password")      },
    { CMD_IDX::VERIFY_NEW_DEVICE            , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("sign-in/verify-new-device")},
    { CMD_IDX::RESEND_VERIFY_NEW_DEVICE_CODE, QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("sign-in/resend-verify-new-device-code")},
    { CMD_IDX::INVITE_FRIEND                , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("invite")               },
    { CMD_IDX::GET_ME                       , QString("%1/%2").arg(DRAGON_USER_URL).arg("me")                       },
    { CMD_IDX::SEARCH_FRIEND                , QString("%1/%2").arg(DRAGON_USER_URL).arg("search")                   },
    { CMD_IDX::SEARCH_FRIEND_LIST           , QString("%1/%2").arg(DRAGON_USER_URL).arg("auto-complete-search")     },
    { CMD_IDX::REQUEST_FRIEND               , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/request")         },
    { CMD_IDX::REQUEST_FRIEND_LIST          , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/request")         },
    { CMD_IDX::CANCEL_REQUEST_FRIEND        , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/request")         },
    { CMD_IDX::IGNORE_FRIEND_REQUEST        , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/request")         },
    { CMD_IDX::GET_FRIENDS_SENT             , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/request/sent")    },
    { CMD_IDX::GET_FRIENDS_RECEIVED         , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/request/received")},
    { CMD_IDX::GET_FRIENDS_LIST             , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/")                },
    { CMD_IDX::REMOVE_FRIEND                , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/")                },
    { CMD_IDX::ACCEPT_FRIEND_REQUEST        , QString("%1/%2").arg(DRAGON_USER_URL).arg("contacts/accept")          },
    { CMD_IDX::GET_ROOM_MEMBERS             , QString("%1/%2").arg(DRAGON_CHAT_URL).arg("rooms")                    },
    { CMD_IDX::DELETE_ACCOUNT               , QString("%1/%2").arg(DRAGON_USER_URL).arg("me")                       },
    { CMD_IDX::DELETE_CONFIRM               , QString("%1/%2").arg(DRAGON_USER_URL).arg("me/delete-confirmation")   },
    { CMD_IDX::GET_DEVICES_LIST             , QString("%1/%2").arg(DRAGON_USER_URL).arg("devices/")                 },
    { CMD_IDX::USER_PROFILE                 , QString("%1/%2").arg(DRAGON_USER_URL).arg("me")                       },
    { CMD_IDX::CHECK_FOR_UPDATE             , QString("%1/%2").arg(DRAGON_APP_URL).arg("check-for-update")          },
    { CMD_IDX::DEVICE_MARK_COMPROMISED      , QString("%1/%2").arg(DRAGON_USER_URL).arg("devices/mark-compromised") },
    { CMD_IDX::DEVICE_SIGNOUT               , QString("%1/%2").arg(DRAGON_USER_URL).arg("devices/")                  },
    { CMD_IDX::DEVICE_ALL_SIGNOUT           , QString("%1/%2").arg(DRAGON_USER_URL).arg("devices/__all")              },
    { CMD_IDX::VERIFY_DELETE_DEVICES        , QString("%1/%2").arg(DRAGON_USER_URL).arg("devices/verify-delete-devices")},
    { CMD_IDX::PRIMARY_KEY_SIGNUP           , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("/pkey/signup") },
    { CMD_IDX::PRIMARY_KEY_SIGNIN           , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("/pkey/signin") },
    { CMD_IDX::PRIMARY_KEY_NONCE            , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("/pkey/nonce")  },
    { CMD_IDX::PRIMARY_KEY_USERNAME         , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("/username-availability/")  },
    { CMD_IDX::PRIMARY_KEY_PUBLIC_ADDRESS   , QString("%1/%2").arg(DRAGON_USER_URL).arg("/pkey/")  },
    { CMD_IDX::PRIMARY_KEY_DELETE_CONFIRMTION, QString("%1/%2").arg(DRAGON_USER_URL).arg("/pkey/delete-confirmation")  },
    { CMD_IDX::PRIMARY_KEY_CHANGE_KEY       , QString("%1/%2").arg(DRAGON_USER_URL).arg("/pkey/change-pkey")  },

    // USER_SUBSCRIPTION
    { CMD_IDX::USER_SUBCRIPTIONS_CURRENT    , QString("%1/%2").arg(DRAGON_SUBSCRIPTIONS_URL).arg("current")  },

    // ASSISTED_WALLETS
    { CMD_IDX::ASSISTED_WALLET_GET          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets")  },
    { CMD_IDX::ASSISTED_WALLET_CREATE_TX    , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions")  },
    { CMD_IDX::ASSISTED_WALLET_GET_LIST_TX  , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions")  },
    { CMD_IDX::ASSISTED_WALLET_SIGN_TX      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions/{transaction_id}/sign") },
    { CMD_IDX::ASSISTED_WALLET_CANCEL_TX    , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions/{transaction_id}") },
    { CMD_IDX::ASSISTED_WALLET_GET_TX       , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions/{transaction_id}") },

    // SEC_QUES
    { CMD_IDX::SEC_QUES_GET                 , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("security-questions") },
    { CMD_IDX::SEC_QUES_SET                 , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("security-questions") },
    { CMD_IDX::SEC_QUES_CREATE              , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("security-questions") },
    { CMD_IDX::SEC_QUES_UPDATE              , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("security-questions/update") },
    { CMD_IDX::SEC_QUES_VERIFY_ANSWER       , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("security-questions/verify-answer") },
    { CMD_IDX::SEC_QUES_REQUIRED_SIGNATURES , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("security-questions/calculate-required-signatures") },

    // LOCK_DOWN
    { CMD_IDX::LOCKDOWN_SET                 , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("lockdown/lock") },
    { CMD_IDX::LOCKDOWN_REQUIRED_SIGNATURES , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("lockdown/calculate-required-signatures") },
    { CMD_IDX::LOCKDOWN_GET_PERIOD          , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("lockdown/period") },

    //USER_WALLET
    { CMD_IDX::GET_RANDOM_NONCE             , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("nonce") },
    { CMD_IDX::VERIFY_PASSWORD_TOKEN        , QString("%1/%2").arg(DRAGON_PASSPORT_URL).arg("verified-password-token")  },

    //USER_KEYS
    { CMD_IDX::USER_KEYS_DOWNLOAD_BACKUP    , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("user-keys/{key_id_or_xfp}/download-backup") },

    { CMD_IDX::TX_SYNC                      , QString("%1/%2").arg(DRAGON_USER_WALLETS_URL).arg("wallets/{wallet_id_or_local_id}/transactions/{transaction_id}/sync") },
    { CMD_IDX::GET_CURRENCIES               , QString("%1/%2").arg(DRAGON_FOREX_URL).arg("currencies") },
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
        LOGIN_NEW_DEVICE = 841,
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
