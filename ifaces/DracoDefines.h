#ifndef DRACODEFINES_H
#define DRACODEFINES_H
#include <QObject>
#include <QMap>

#define DRAGON_PASSPORT_URL     "https://api.nunchuk.io/v1.1/passport"
#define DRAGON_USER_URL         "https://api.nunchuk.io/v1.1/user"
#define DRAGON_CHAT_URL         "https://api.nunchuk.io/v1.1/chat"
#define DRAGON_APP_URL          "https://api.nunchuk.io/v1.1/app"

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
    CMD_MAX
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
