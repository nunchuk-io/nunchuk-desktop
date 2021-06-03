
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
public:
    enum EVT_STATE_ID_REGISTED
    {
        EVT_STATE_ID_REGISTED_MIN=0                 , // 0
        STATE_ID_ROOT                               , // 1
        STATE_ID_SCR_HOME                           , // 2
        STATE_ID_SCR_ADD_WALLET                     , // 3
        STATE_ID_SCR_ADD_HARDWARE_SIGNER            , // 4
        STATE_ID_SCR_MASTER_SIGNER_INFO             , // 5
        STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION, // 6
        STATE_ID_SCR_RECEIVE                        , // 7
        STATE_ID_SCR_SEND                           , // 8
        STATE_ID_SCR_CREATE_TRANSACTION             , // 9
        STATE_ID_SCR_TRANSACTION_INFO               , // 10
        STATE_ID_SCR_TRANSACTION_HISTORY            , // 11
        STATE_ID_SCR_APP_SETTINGS                   , // 12
        STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT       , // 13
        STATE_ID_SCR_UTXOS                          , // 14
        STATE_ID_SCR_WALLET_INFO                    , // 15
        STATE_ID_SCR_ADD_WALLET_CONFIRMATION        , // 16
        STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET  , // 17
        STATE_ID_SCR_WALLET_CHANGE_ADDRESSES        , // 18
        STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT       , // 19
        STATE_ID_SCR_REMOTE_SIGNER_INFO             , // 20
        STATE_ID_SCR_LOGIN                          , // 21
        STATE_ID_SCR_CONSOLIDATE_OUTPUT             , // 22
        STATE_ID_SCR_UTXO_OUTPUT                    , // 23
        STATE_ID_SCR_INPUT_PIN                      , // 24
        STATE_ID_SCR_ADD_NEW_SIGNER                 , // 25
        STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER        , // 26
        STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER        , // 27
        STATE_ID_SCR_CREATE_NEW_SEED                , // 28
        STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION  , // 29
        STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET       , // 30
        STATE_ID_SCR_INPUT_PASSPHRASE               , // 31
        EVT_STATE_ID_REGISTED_MAX                   , // 32
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1, // 33
        EVT_STARTING_APPLICATION_LOCALMODE               , // 34
        EVT_HOME_REQUEST_NOPASS                          , // 35
        EVT_ONS_CLOSE_REQUEST                            , // 36
        EVT_LOGIN_REQUEST                                , // 37
        EVT_ROOT_PROMT_PIN                               , // 38
        EVT_ROOT_PROMT_PASSPHRASE                        , // 39
        EVT_STATE_ID_ROOT_MAX                            , // 40
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 41
        EVT_HOME_WALLET_SELECTED                         , // 42
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 43
        EVT_HOME_SETTING_REQUEST                         , // 44
        EVT_HOME_DISPLAY_ADDRESS                         , // 45
        EVT_HOME_ADD_WALLET_REQUEST                      , // 46
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 47
        EVT_HOME_SEND_REQUEST                            , // 48
        EVT_HOME_RECEIVE_REQUEST                         , // 49
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 50
        EVT_HOME_WALLET_INFO_REQUEST                     , // 51
        EVT_APP_SETTING_REQUEST                          , // 52
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 53
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 54
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 55
        EVT_STATE_ID_SCR_HOME_MAX                        , // 56
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 57
        EVT_ADD_WALLET_IMPORT                                      , // 58
        EVT_ADD_WALLET_BACK_REQUEST                                , // 59
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 60
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 61
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 62
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 63
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 64
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 65
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 66
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 67
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 68
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 69
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 70
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 71
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 72
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 73
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 74
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 75
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 76
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 77
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 78
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 79
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 80
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 81
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 82
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 83
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 84
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 85
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 86
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 87
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 88
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 89
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 90
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 91
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 92
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 93
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 94
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 95
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 96
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 97
        EVT_RECEIVE_BACK_REQUEST                                                           , // 98
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 99
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 100
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 101
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 102
        EVT_SEND_BACK_REQUEST                                   , // 103
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 104
        EVT_STATE_ID_SCR_SEND_MAX                               , // 105
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 106
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 107
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 108
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 109
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 110
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 111
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 112
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 113
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 114
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 115
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 116
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 117
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 118
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 119
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 120
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 121
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 122
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 123
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 124
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 125
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 126
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 127
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 128
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 129
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 130
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 131
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 132
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 133
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 134
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 135
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 136
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 137
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 138
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 139
        EVT_APP_SETTING_UPDATE_SETTING                                              , // 140
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 141
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 142
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 143
        EVT_APP_SETTING_BACK_REQUEST                                                , // 144
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 145
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 146
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 147
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 148
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 149
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 150
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 151
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 152
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 153
        EVT_UTXOS_SORT_REQUEST                                                    , // 154
        EVT_UTXOS_BACK_REQUEST                                                    , // 155
        EVT_UTXOS_CONSOLIDATE_REQUEST                                             , // 156
        EVT_UTXOS_ITEM_SELECTED                                                   , // 157
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 158
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 159
        EVT_WALLET_INFO_EDIT_NAME                                    , // 160
        EVT_WALLET_INFO_REMOVE                                       , // 161
        EVT_WALLET_INFO_EXPORT_DB                                    , // 162
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 163
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 164
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 165
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 166
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 167
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 168
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 169
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 170
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 171
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 172
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 173
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 174
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 175
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 176
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 177
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 178
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 179
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 180
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 181
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 182
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 183
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 184
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 185
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 186
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 187
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 188
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 189
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 190
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 191
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 192
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 193
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 194
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 195
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 196
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 197
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 198
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 199
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 200
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 201
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 202
        EVT_ADD_MASTER_SIGNER_RESULT_SEND_PIN                                                       , // 203
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 204
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 205
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 206
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 207
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 208
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 209
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 210
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 211
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 212
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 213
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 214
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 215
    };
    enum EVT_STATE_ID_SCR_LOGIN
    {
        EVT_STATE_ID_SCR_LOGIN_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 216
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                  , // 217
        EVT_LOGIN_SUCCESSFULL                                               , // 218
        EVT_STATE_ID_SCR_LOGIN_MAX                                          , // 219
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_LOGIN_MAX+1, // 220
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                    , // 221
        EVT_CONSOLIDATE_BACK_REQUEST                                        , // 222
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                            , // 223
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                             , // 224
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 225
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 226
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 227
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 228
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 229
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 230
        EVT_INPUT_PIN_SEND_PIN                                           , // 231
        EVT_INPUT_PIN_CLOSE                                              , // 232
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 233
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 234
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 235
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 236
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 237
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 238
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 239
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 240
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 241
        EVT_CREATE_NEW_SEED                                                               , // 242
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 243
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 244
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 245
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 246
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 247
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 248
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 249
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 250
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 251
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 252
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 253
        EVT_CREATE_NEW_SEED_BACK                                                           , // 254
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 255
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 256
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 257
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 258
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 259
        EVT_ADD_SOFTWARE_SIGNER_RESULT                                                           , // 260
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 261
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 262
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 263
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 264
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 265
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 266
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 267
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 268
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 269
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 270
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 271
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 272
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 273
    };

};
#endif // VIEWSENUMS_H
