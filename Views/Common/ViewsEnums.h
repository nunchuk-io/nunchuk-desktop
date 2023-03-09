
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
    Q_ENUMS(EVT_STATE_ID_SCR_UNLOCK_DB)
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
        STATE_ID_SCR_UNLOCK_DB                           , // 21
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
        EVT_ONS_CLOSE_ALL_REQUEST                        , // 62
        EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST             , // 63
        EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST              , // 64
        EVT_GOTO_HOME_WALLET_TAB                         , // 65
        EVT_ONS_CLOSE_REQUEST                            , // 66
        EVT_LOGIN_DB_REQUEST                             , // 67
        EVT_ROOT_PROMT_PIN                               , // 68
        EVT_ROOT_PROMT_PASSPHRASE                        , // 69
        EVT_LOGIN_MATRIX_REQUEST                         , // 70
        EVT_GOTO_HOME_CHAT_TAB                           , // 71
        EVT_ONLINE_ONS_CLOSE_REQUEST                     , // 72
        EVT_SHOW_TOAST_MESSAGE                           , // 73
        EVT_ROOT_UPDATE_PROFILE                          , // 74
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                  , // 75
        EVT_GOTO_APP_SETTINGS_TAB                        , // 76
        EVT_SIGN_IN_REQUEST                              , // 77
        EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST               , // 78
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST              , // 79
        EVT_STATE_ID_ROOT_MAX                            , // 80
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 81
        EVT_HOME_WALLET_SELECTED                         , // 82
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 83
        EVT_HOME_SETTING_REQUEST                         , // 84
        EVT_HOME_DISPLAY_ADDRESS                         , // 85
        EVT_HOME_IMPORT_PSBT                             , // 86
        EVT_HOME_EXPORT_BSMS                             , // 87
        EVT_HOME_ADD_WALLET_REQUEST                      , // 88
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 89
        EVT_HOME_SEND_REQUEST                            , // 90
        EVT_HOME_RECEIVE_REQUEST                         , // 91
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 92
        EVT_HOME_WALLET_INFO_REQUEST                     , // 93
        EVT_APP_SETTING_REQUEST                          , // 94
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 95
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 96
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 97
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 98
        EVT_STATE_ID_SCR_HOME_MAX                        , // 99
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 100
        EVT_ADD_WALLET_IMPORT                                      , // 101
        EVT_ADD_WALLET_BACK_REQUEST                                , // 102
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 103
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 104
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 105
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 106
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 107
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 108
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 109
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 110
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 111
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 112
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 113
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 114
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 115
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 116
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 117
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 118
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 119
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 120
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 121
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 122
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 123
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 124
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 125
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 126
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 127
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 128
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 129
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 130
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 131
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 132
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 133
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 134
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 135
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 136
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 137
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 138
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 139
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 140
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 141
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 142
        EVT_RECEIVE_BACK_REQUEST                                                           , // 143
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 144
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 145
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 146
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 147
        EVT_SEND_BACK_REQUEST                                   , // 148
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 149
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 150
        EVT_STATE_ID_SCR_SEND_MAX                               , // 151
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 152
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 153
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 154
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 155
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 156
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 157
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 158
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 159
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 160
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 161
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 162
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 163
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 164
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 165
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 166
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 167
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 168
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 169
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 170
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 171
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 172
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 173
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 174
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 175
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 176
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 177
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 178
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 179
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 180
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 181
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 182
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 183
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 184
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 185
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 186
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 187
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 188
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 189
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 190
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 191
        EVT_APP_SETTING_BACK_REQUEST                                                , // 192
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 193
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 194
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 195
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 196
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 197
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 198
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 199
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 200
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 201
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 202
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 203
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 204
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 205
        EVT_UTXOS_SORT_REQUEST                                                    , // 206
        EVT_UTXOS_BACK_REQUEST                                                    , // 207
        EVT_UTXOS_CONSOLIDATE_REQUEST                                             , // 208
        EVT_UTXOS_ITEM_SELECTED                                                   , // 209
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 210
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 211
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 212
        EVT_WALLET_INFO_EDIT_NAME                                    , // 213
        EVT_WALLET_INFO_REMOVE                                       , // 214
        EVT_WALLET_INFO_EXPORT_DB                                    , // 215
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 216
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 217
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 218
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 219
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 220
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 221
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 222
        EVT_WALLET_INFO_REFRESH_WALLET_REQUEST                       , // 223
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 224
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 225
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 226
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 227
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 228
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 229
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 230
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 231
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 232
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 233
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 234
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 235
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 236
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 237
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 238
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 239
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 240
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 241
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 242
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 243
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 244
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 245
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 246
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 247
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 248
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 249
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 250
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 251
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 252
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 253
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 254
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 255
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 256
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 257
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 258
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 259
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 260
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 261
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 262
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 263
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 264
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 265
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 266
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 267
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 268
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 269
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 270
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 271
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 272
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 273
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 274
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 275
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 276
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 277
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 278
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 279
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 280
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 281
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 282
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 283
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 284
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 285
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 286
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 287
        EVT_INPUT_PIN_SEND_PIN                                           , // 288
        EVT_INPUT_PIN_CLOSE                                              , // 289
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 290
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 291
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 292
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 293
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 294
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 295
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 296
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 297
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 298
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 299
        EVT_CREATE_NEW_SEED                                                               , // 300
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 301
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 302
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 303
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 304
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 305
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 306
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 307
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 308
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 309
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 310
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 311
        EVT_CREATE_NEW_SEED_BACK                                                           , // 312
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 313
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 314
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 315
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 316
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 317
        EVT_ADD_SOFTWARE_SIGNER_RESULT                                                           , // 318
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 319
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 320
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 321
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 322
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 323
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 324
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 325
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 326
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 327
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 328
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 329
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 330
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 331
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 332
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 333
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 334
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 335
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 336
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 337
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 338
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 339
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 340
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 341
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 342
        EVT_LOGIN_ONLINE_LOGIN_SUCCEED                                           , // 343
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 344
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 345
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 346
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 347
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 348
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 349
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 350
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 351
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 352
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 353
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 354
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 355
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 356
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 357
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 358
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 359
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 360
        EVT_HOME_ONLINE_APPSETTING_REQUEST                                  , // 361
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 362
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 363
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 364
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 365
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 366
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 367
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 368
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 369
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 370
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 371
        EVT_PENDING_REQUEST_IGNORE                                                     , // 372
        EVT_PENDING_REQUEST_BACK                                                       , // 373
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 374
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 375
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 376
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 377
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 378
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 379
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 380
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 381
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 382
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 383
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 384
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 385
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 386
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 387
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 388
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 389
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 390
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 391
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 392
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 393
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 394
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 395
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 396
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 397
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 398
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 399
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 400
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 401
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 402
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 403
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 404
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 405
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 406
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 407
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 408
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 409
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 410
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 411
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 412
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 413
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 414
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 415
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 416
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 417
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 418
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 419
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 420
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 421
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 422
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 423
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 424
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 425
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 426
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 427
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 428
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 429
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 430
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 431
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 432
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 433
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 434
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 435
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 436
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 437
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 438
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 439
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 440
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 441
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 442
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 443
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 444
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 445
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 446
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 447
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 448
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 449
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 450
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 451
        EVT_CHANGE_PASSWORD_BACK                                                  , // 452
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 453
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 454
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 455
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 456
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 457
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 458
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 459
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 460
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 461
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 462
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 463
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 464
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 465
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 466
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 467
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 468
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 469
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 470
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 471
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 472
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 473
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 474
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 475
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 476
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 477
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 478
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 479
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 480
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 481
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 482
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 483
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 484
        EVT_LOGIN_SUCCEEDED                                                          , // 485
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 486
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 487
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 488
        EVT_UPDATE_PROFILE_REQUEST                                        , // 489
        EVT_UPDATE_PROFILE_BACK                                           , // 490
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 491
    };

};
#endif // VIEWSENUMS_H
