
#ifndef VIEWSENUMS_H
#define VIEWSENUMS_H

#include <QObject>
class E: public QObject
{
    Q_OBJECT
    Q_ENUMS(EVT_STATE_ID_REGISTED)
    Q_ENUMS(EVT_STATE_ID_ROOT)
    Q_ENUMS(EVT_STATE_ID_SCR_HOME)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_WALLET)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER)
    Q_ENUMS(EVT_STATE_ID_SCR_MASTER_SIGNER_INFO)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION)
    Q_ENUMS(EVT_STATE_ID_SCR_RECEIVE)
    Q_ENUMS(EVT_STATE_ID_SCR_SEND)
    Q_ENUMS(EVT_STATE_ID_SCR_CREATE_TRANSACTION)
    Q_ENUMS(EVT_STATE_ID_SCR_TRANSACTION_INFO)
    Q_ENUMS(EVT_STATE_ID_SCR_TRANSACTION_HISTORY)
    Q_ENUMS(EVT_STATE_ID_SCR_APP_SETTINGS)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT)
    Q_ENUMS(EVT_STATE_ID_SCR_UTXOS)
    Q_ENUMS(EVT_STATE_ID_SCR_WALLET_INFO)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET)
    Q_ENUMS(EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT)
    Q_ENUMS(EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO)
    Q_ENUMS(EVT_STATE_ID_SCR_LOGIN)
    Q_ENUMS(EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT)
    Q_ENUMS(EVT_STATE_ID_SCR_UTXO_OUTPUT)
    Q_ENUMS(EVT_STATE_ID_SCR_INPUT_PIN)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_NEW_SIGNER)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER)
    Q_ENUMS(EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER)
    Q_ENUMS(EVT_STATE_ID_SCR_CREATE_NEW_SEED)
    Q_ENUMS(EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET)
    Q_ENUMS(EVT_STATE_ID_SCR_INPUT_PASSPHRASE)
    Q_ENUMS(EVT_STATE_ID_SCR_LOGIN_ONLINE)
    Q_ENUMS(EVT_STATE_ID_SCR_HOME_ONLINE)
    Q_ENUMS(EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS)
    Q_ENUMS(EVT_STATE_ID_SCR_PENDING_REQUEST)
    Q_ENUMS(EVT_STATE_ID_SCR_CREATE_SHARED_WALLET)
    Q_ENUMS(EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET)
    Q_ENUMS(EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET)
    Q_ENUMS(EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET)
    Q_ENUMS(EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE)
    Q_ENUMS(EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET)
    Q_ENUMS(EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION)
    Q_ENUMS(EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER)
    Q_ENUMS(EVT_STATE_ID_TOAST_MESSAGE_DISPLAY)
    Q_ENUMS(EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET)
    Q_ENUMS(EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT)
    Q_ENUMS(EVT_STATE_ID_SCR_CREATE_ACCOUNT)
    Q_ENUMS(EVT_STATE_ID_SCR_CHANGE_PASSWORD)
    Q_ENUMS(EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY)
    Q_ENUMS(EVT_STATE_ID_SCR_SIGN_IN_MANUALLY)
    Q_ENUMS(EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY)
    Q_ENUMS(EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY)
    Q_ENUMS(EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION)
    Q_ENUMS(EVT_STATE_ID_SCR_SIGN_IN)
    Q_ENUMS(EVT_STATE_ID_SCR_UPDATE_PROFILE)
public:
    enum EVT_STATE_ID_REGISTED
    {
        EVT_STATE_ID_REGISTED_MIN=0                      , // 0
        STATE_ID_ROOT                                    , // 1
        STATE_ID_SCR_HOME                                , // 2
        STATE_ID_SCR_ADD_WALLET                          , // 3
        STATE_ID_SCR_ADD_HARDWARE_SIGNER                 , // 4
        STATE_ID_SCR_MASTER_SIGNER_INFO                  , // 5
        STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION     , // 6
        STATE_ID_SCR_RECEIVE                             , // 7
        STATE_ID_SCR_SEND                                , // 8
        STATE_ID_SCR_CREATE_TRANSACTION                  , // 9
        STATE_ID_SCR_TRANSACTION_INFO                    , // 10
        STATE_ID_SCR_TRANSACTION_HISTORY                 , // 11
        STATE_ID_SCR_APP_SETTINGS                        , // 12
        STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT            , // 13
        STATE_ID_SCR_UTXOS                               , // 14
        STATE_ID_SCR_WALLET_INFO                         , // 15
        STATE_ID_SCR_ADD_WALLET_CONFIRMATION             , // 16
        STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET       , // 17
        STATE_ID_SCR_WALLET_CHANGE_ADDRESSES             , // 18
        STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT            , // 19
        STATE_ID_SCR_REMOTE_SIGNER_INFO                  , // 20
        STATE_ID_SCR_LOGIN                               , // 21
        STATE_ID_SCR_CONSOLIDATE_OUTPUT                  , // 22
        STATE_ID_SCR_UTXO_OUTPUT                         , // 23
        STATE_ID_SCR_INPUT_PIN                           , // 24
        STATE_ID_SCR_ADD_NEW_SIGNER                      , // 25
        STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER             , // 26
        STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER             , // 27
        STATE_ID_SCR_CREATE_NEW_SEED                     , // 28
        STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION       , // 29
        STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET            , // 30
        STATE_ID_SCR_INPUT_PASSPHRASE                    , // 31
        STATE_ID_SCR_LOGIN_ONLINE                        , // 32
        STATE_ID_SCR_HOME_ONLINE                         , // 33
        STATE_ID_SCR_ONLINE_ADD_CONTACTS                 , // 34
        STATE_ID_SCR_PENDING_REQUEST                     , // 35
        STATE_ID_SCR_CREATE_SHARED_WALLET                , // 36
        STATE_ID_SCR_CONFIGURE_SHARED_WALLET             , // 37
        STATE_ID_SCR_REVIEW_SHARED_WALLET                , // 38
        STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET      , // 39
        STATE_ID_SCR_SHARED_WALLET_CONFIGURE             , // 40
        STATE_ID_SCR_BACKUP_SHARED_WALLET                , // 41
        STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION       , // 42
        STATE_ID_SCR_STARTING_CREATE_SIGNER              , // 43
        STATE_ID_TOAST_MESSAGE_DISPLAY                   , // 44
        STATE_ID_SCR_RECOVER_SHARED_WALLET               , // 45
        STATE_ID_SCR_PRIMARY_KEY_ACCOUNT                 , // 46
        STATE_ID_SCR_CREATE_ACCOUNT                      , // 47
        STATE_ID_SCR_CHANGE_PASSWORD                     , // 48
        STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY, // 49
        STATE_ID_SCR_SIGN_IN_MANUALLY                    , // 50
        STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY             , // 51
        STATE_ID_SCR_REPLACE_PRIMARY_KEY                 , // 52
        STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION           , // 53
        STATE_ID_SCR_SIGN_IN                             , // 54
        STATE_ID_SCR_UPDATE_PROFILE                      , // 55
        EVT_STATE_ID_REGISTED_MAX                        , // 56
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1, // 57
        EVT_STARTING_APPLICATION_LOCALMODE               , // 58
        EVT_STARTING_APPLICATION_ONLINEMODE              , // 59
        EVT_STARTING_APPLICATION_ONLINE_HWL              , // 60
        EVT_SETTING_ACCOUNT_CHANGE_PASSWORD              , // 61
        EVT_GOTO_HOME_WALLET_TAB                         , // 62
        EVT_ONS_CLOSE_REQUEST                            , // 63
        EVT_LOGIN_DB_REQUEST                             , // 64
        EVT_ROOT_PROMT_PIN                               , // 65
        EVT_ROOT_PROMT_PASSPHRASE                        , // 66
        EVT_LOGIN_MATRIX_REQUEST                         , // 67
        EVT_GOTO_HOME_CHAT_TAB                           , // 68
        EVT_ONLINE_ONS_CLOSE_REQUEST                     , // 69
        EVT_SHOW_TOAST_MESSAGE                           , // 70
        EVT_ROOT_UPDATE_PROFILE                          , // 71
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                  , // 72
        EVT_SETTING_ONS_CLOSE_REQUEST                    , // 73
        EVT_GOTO_APP_SETTINGS_TAB                        , // 74
        EVT_STATE_ID_ROOT_MAX                            , // 75
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 76
        EVT_HOME_WALLET_SELECTED                         , // 77
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 78
        EVT_HOME_SETTING_REQUEST                         , // 79
        EVT_HOME_DISPLAY_ADDRESS                         , // 80
        EVT_HOME_IMPORT_PSBT                             , // 81
        EVT_HOME_EXPORT_BSMS                             , // 82
        EVT_HOME_ADD_WALLET_REQUEST                      , // 83
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 84
        EVT_HOME_SEND_REQUEST                            , // 85
        EVT_HOME_RECEIVE_REQUEST                         , // 86
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 87
        EVT_HOME_WALLET_INFO_REQUEST                     , // 88
        EVT_APP_SETTING_REQUEST                          , // 89
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 90
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 91
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 92
        EVT_STATE_ID_SCR_HOME_MAX                        , // 93
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 94
        EVT_ADD_WALLET_IMPORT                                      , // 95
        EVT_ADD_WALLET_BACK_REQUEST                                , // 96
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 97
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 98
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 99
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 100
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 101
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 102
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 103
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 104
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 105
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 106
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 107
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 108
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 109
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 110
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 111
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 112
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 113
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 114
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 115
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 116
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 117
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 118
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 119
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 120
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 121
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 122
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 123
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 124
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 125
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 126
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 127
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 128
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 129
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 130
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 131
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 132
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 133
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 134
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 135
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 136
        EVT_RECEIVE_BACK_REQUEST                                                           , // 137
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 138
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 139
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 140
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 141
        EVT_SEND_BACK_REQUEST                                   , // 142
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 143
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 144
        EVT_STATE_ID_SCR_SEND_MAX                               , // 145
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 146
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 147
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 148
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 149
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 150
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 151
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 152
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 153
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 154
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 155
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 156
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 157
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 158
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 159
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 160
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 161
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 162
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 163
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 164
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 165
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 166
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 167
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 168
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 169
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 170
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 171
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 172
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 173
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 174
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 175
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 176
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 177
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 178
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 179
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 180
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 181
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 182
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 183
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 184
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 185
        EVT_APP_SETTING_BACK_REQUEST                                                , // 186
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 187
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 188
        EVT_SIGN_IN_REQUEST                                                         , // 189
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 190
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 191
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 192
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 193
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 194
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 195
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 196
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 197
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 198
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 199
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 200
        EVT_UTXOS_SORT_REQUEST                                                    , // 201
        EVT_UTXOS_BACK_REQUEST                                                    , // 202
        EVT_UTXOS_CONSOLIDATE_REQUEST                                             , // 203
        EVT_UTXOS_ITEM_SELECTED                                                   , // 204
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 205
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 206
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 207
        EVT_WALLET_INFO_EDIT_NAME                                    , // 208
        EVT_WALLET_INFO_REMOVE                                       , // 209
        EVT_WALLET_INFO_EXPORT_DB                                    , // 210
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 211
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 212
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 213
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 214
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 215
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 216
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 217
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 218
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 219
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 220
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 221
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 222
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 223
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 224
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 225
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 226
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 227
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 228
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 229
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 230
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 231
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 232
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 233
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 234
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 235
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 236
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 237
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 238
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 239
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 240
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 241
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 242
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 243
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 244
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 245
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 246
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 247
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 248
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 249
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 250
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 251
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 252
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 253
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 254
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 255
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 256
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 257
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 258
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 259
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 260
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 261
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 262
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 263
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 264
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 265
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 266
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 267
    };
    enum EVT_STATE_ID_SCR_LOGIN
    {
        EVT_STATE_ID_SCR_LOGIN_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 268
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                  , // 269
        EVT_STATE_ID_SCR_LOGIN_MAX                                          , // 270
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_LOGIN_MAX+1, // 271
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                    , // 272
        EVT_CONSOLIDATE_BACK_REQUEST                                        , // 273
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                            , // 274
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                             , // 275
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 276
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 277
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 278
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 279
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 280
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 281
        EVT_INPUT_PIN_SEND_PIN                                           , // 282
        EVT_INPUT_PIN_CLOSE                                              , // 283
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 284
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 285
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 286
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 287
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 288
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 289
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 290
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 291
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 292
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 293
        EVT_CREATE_NEW_SEED                                                               , // 294
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 295
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 296
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 297
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 298
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 299
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 300
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 301
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 302
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 303
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 304
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 305
        EVT_CREATE_NEW_SEED_BACK                                                           , // 306
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 307
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 308
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 309
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 310
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 311
        EVT_ADD_SOFTWARE_SIGNER_RESULT                                                           , // 312
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 313
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 314
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 315
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 316
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 317
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 318
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 319
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 320
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 321
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 322
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 323
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 324
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 325
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 326
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 327
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 328
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 329
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 330
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 331
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 332
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 333
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 334
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 335
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 336
        EVT_LOGIN_ONLINE_LOGIN_SUCCEED                                           , // 337
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 338
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 339
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 340
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 341
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 342
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 343
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 344
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 345
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 346
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 347
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 348
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 349
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 350
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 351
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 352
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 353
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 354
        EVT_HOME_ONLINE_APPSETTING_REQUEST                                  , // 355
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 356
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 357
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 358
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 359
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 360
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 361
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 362
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 363
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 364
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 365
        EVT_PENDING_REQUEST_IGNORE                                                     , // 366
        EVT_PENDING_REQUEST_BACK                                                       , // 367
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 368
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 369
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 370
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 371
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 372
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 373
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 374
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 375
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 376
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 377
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 378
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 379
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 380
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 381
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 382
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 383
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 384
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 385
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 386
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 387
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 388
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 389
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 390
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 391
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 392
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 393
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 394
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 395
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 396
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 397
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 398
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 399
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 400
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 401
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 402
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 403
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 404
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 405
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 406
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 407
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 408
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 409
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 410
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 411
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 412
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 413
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 414
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 415
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 416
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 417
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 418
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 419
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 420
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 421
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 422
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 423
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 424
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 425
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 426
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 427
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 428
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 429
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 430
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 431
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 432
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 433
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 434
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST                                                  , // 435
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 436
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 437
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 438
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 439
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 440
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 441
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 442
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 443
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 444
        EVT_CHANGE_PASSWORD_BACK                                                  , // 445
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 446
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 447
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 448
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 449
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 450
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 451
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 452
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 453
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 454
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 455
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 456
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 457
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 458
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 459
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 460
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 461
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 462
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 463
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 464
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 465
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 466
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 467
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 468
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 469
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 470
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 471
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 472
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 473
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 474
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 475
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 476
        EVT_LOGIN_SUCCEEDED                                                          , // 477
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 478
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 479
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 480
        EVT_UPDATE_PROFILE_REQUEST                                        , // 481
        EVT_UPDATE_PROFILE_BACK                                           , // 482
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 483
    };

};
#endif // VIEWSENUMS_H
