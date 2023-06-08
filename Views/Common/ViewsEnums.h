/************************************************************************* 
* This file is part of the Nunchuk software (https://nunchuk.io/)        * 
* Copyright (C) 2020-2022 Enigmo                                         * 
* Copyright (C) 2022 Nunchuk                                             * 
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
    Q_ENUMS(EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS)
    Q_ENUMS(EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD)
    Q_ENUMS(EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION)
    Q_ENUMS(EVT_STATE_ID_SCR_KEY_RECOVERY)
    Q_ENUMS(EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD)
    Q_ENUMS(EVT_STATE_ID_SCR_SERVICE_SETTINGS)
    Q_ENUMS(EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD)
    Q_ENUMS(EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION)
    Q_ENUMS(EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS)
    Q_ENUMS(EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO)
    Q_ENUMS(EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION)
    Q_ENUMS(EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS)
    Q_ENUMS(EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_LEDGER_ASK)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_TREZOR_ASK)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_LEDGER)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_TREZOR)
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
        STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS         , // 56
        STATE_ID_SCR_ENTER_BACKUP_PASSWORD               , // 57
        STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION, // 58
        STATE_ID_SCR_KEY_RECOVERY                        , // 59
        STATE_ID_SCR_REENTER_YOUR_PASSWORD               , // 60
        STATE_ID_SCR_SERVICE_SETTINGS                    , // 61
        STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD         , // 62
        STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION    , // 63
        STATE_ID_SCR_LOCKDOWN_SUCCESS                    , // 64
        STATE_ID_SCR_DUMMY_TRANSACTION_INFO              , // 65
        STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION     , // 66
        STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS     , // 67
        STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE        , // 68
        STATE_ID_SCR_ADD_LEDGER_ASK                      , // 69
        STATE_ID_SCR_ADD_TREZOR_ASK                      , // 70
        STATE_ID_SCR_ADD_LEDGER                          , // 71
        STATE_ID_SCR_ADD_TREZOR                          , // 72
        EVT_STATE_ID_REGISTED_MAX                        , // 73
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1, // 74
        EVT_STARTING_APPLICATION_LOCALMODE               , // 75
        EVT_STARTING_APPLICATION_ONLINEMODE              , // 76
        EVT_STARTING_APPLICATION_ONLINE_HWL              , // 77
        EVT_SETTING_ACCOUNT_CHANGE_PASSWORD              , // 78
        EVT_ONS_CLOSE_ALL_REQUEST                        , // 79
        EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST             , // 80
        EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST              , // 81
        EVT_GOTO_HOME_WALLET_TAB                         , // 82
        EVT_ONS_CLOSE_REQUEST                            , // 83
        EVT_LOGIN_DB_REQUEST                             , // 84
        EVT_ROOT_PROMT_PIN                               , // 85
        EVT_ROOT_PROMT_PASSPHRASE                        , // 86
        EVT_LOGIN_MATRIX_REQUEST                         , // 87
        EVT_GOTO_HOME_CHAT_TAB                           , // 88
        EVT_ONLINE_ONS_CLOSE_REQUEST                     , // 89
        EVT_SHOW_TOAST_MESSAGE                           , // 90
        EVT_ROOT_UPDATE_PROFILE                          , // 91
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                  , // 92
        EVT_GOTO_APP_SETTINGS_TAB                        , // 93
        EVT_SIGN_IN_REQUEST                              , // 94
        EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST               , // 95
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST              , // 96
        EVT_GOTO_SERVICE_SETTING_TAB                     , // 97
        EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST            , // 98
        EVT_NUNCHUK_LOGIN_SUCCEEDED                      , // 99
        EVT_STATE_ID_ROOT_MAX                            , // 100
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 101
        EVT_HOME_WALLET_SELECTED                         , // 102
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 103
        EVT_HOME_SETTING_REQUEST                         , // 104
        EVT_HOME_DISPLAY_ADDRESS                         , // 105
        EVT_HOME_IMPORT_PSBT                             , // 106
        EVT_HOME_EXPORT_BSMS                             , // 107
        EVT_HOME_ADD_WALLET_REQUEST                      , // 108
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 109
        EVT_HOME_SEND_REQUEST                            , // 110
        EVT_HOME_RECEIVE_REQUEST                         , // 111
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 112
        EVT_HOME_WALLET_INFO_REQUEST                     , // 113
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 114
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 115
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 116
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 117
        EVT_ASK_LEDGER_REQ                               , // 118
        EVT_ASK_TREZOR_REQ                               , // 119
        EVT_STATE_ID_SCR_HOME_MAX                        , // 120
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 121
        EVT_ADD_WALLET_IMPORT                                      , // 122
        EVT_ADD_WALLET_BACK_REQUEST                                , // 123
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 124
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 125
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 126
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 127
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 128
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 129
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 130
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 131
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 132
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 133
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 134
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 135
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 136
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 137
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 138
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 139
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 140
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 141
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 142
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 143
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 144
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 145
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 146
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 147
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 148
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 149
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 150
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 151
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 152
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 153
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 154
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 155
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 156
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 157
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 158
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 159
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 160
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 161
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 162
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 163
        EVT_RECEIVE_BACK_REQUEST                                                           , // 164
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 165
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 166
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 167
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 168
        EVT_SEND_BACK_REQUEST                                   , // 169
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 170
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 171
        EVT_STATE_ID_SCR_SEND_MAX                               , // 172
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 173
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 174
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 175
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 176
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 177
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 178
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 179
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 180
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 181
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 182
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 183
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 184
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 185
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 186
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 187
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 188
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 189
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 190
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 191
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 192
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 193
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 194
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 195
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 196
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 197
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 198
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 199
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 200
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 201
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 202
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 203
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 204
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 205
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 206
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 207
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 208
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 209
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 210
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 211
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 212
        EVT_APP_SETTING_BACK_REQUEST                                                , // 213
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 214
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 215
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 216
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 217
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 218
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 219
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 220
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 221
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 222
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 223
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 224
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 225
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 226
        EVT_UTXOS_SORT_REQUEST                                                    , // 227
        EVT_UTXOS_BACK_REQUEST                                                    , // 228
        EVT_UTXOS_CONSOLIDATE_REQUEST                                             , // 229
        EVT_UTXOS_ITEM_SELECTED                                                   , // 230
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 231
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 232
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 233
        EVT_WALLET_INFO_EDIT_NAME                                    , // 234
        EVT_WALLET_INFO_REMOVE                                       , // 235
        EVT_WALLET_INFO_EXPORT_DB                                    , // 236
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 237
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 238
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 239
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 240
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 241
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 242
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 243
        EVT_WALLET_INFO_REFRESH_WALLET_REQUEST                       , // 244
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 245
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 246
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 247
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 248
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 249
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 250
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 251
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 252
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 253
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 254
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 255
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 256
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 257
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 258
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 259
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 260
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 261
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 262
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 263
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 264
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 265
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 266
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 267
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 268
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 269
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 270
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 271
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 272
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 273
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 274
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 275
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 276
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 277
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 278
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 279
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 280
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 281
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 282
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 283
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 284
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 285
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 286
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 287
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 288
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 289
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 290
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 291
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 292
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 293
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 294
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 295
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 296
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 297
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 298
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 299
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 300
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 301
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 302
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 303
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 304
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 305
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 306
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 307
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 308
        EVT_INPUT_PIN_SEND_PIN                                           , // 309
        EVT_INPUT_PIN_CLOSE                                              , // 310
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 311
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 312
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 313
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 314
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 315
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 316
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 317
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 318
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 319
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 320
        EVT_CREATE_NEW_SEED                                                               , // 321
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 322
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 323
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 324
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 325
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 326
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 327
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 328
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 329
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 330
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 331
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 332
        EVT_CREATE_NEW_SEED_BACK                                                           , // 333
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 334
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 335
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 336
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 337
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 338
        EVT_ADD_SOFTWARE_SIGNER_RESULT                                                           , // 339
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 340
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 341
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 342
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 343
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 344
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 345
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 346
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 347
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 348
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 349
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 350
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 351
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 352
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 353
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 354
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 355
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 356
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 357
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 358
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 359
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 360
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 361
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 362
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 363
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 364
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 365
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 366
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 367
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 368
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 369
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 370
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 371
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 372
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 373
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 374
        EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ                                 , // 375
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 376
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 377
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 378
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 379
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 380
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 381
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 382
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 383
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 384
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 385
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 386
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 387
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 388
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 389
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 390
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 391
        EVT_PENDING_REQUEST_IGNORE                                                     , // 392
        EVT_PENDING_REQUEST_BACK                                                       , // 393
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 394
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 395
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 396
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 397
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 398
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 399
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 400
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 401
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 402
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 403
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 404
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 405
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 406
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 407
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 408
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 409
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 410
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 411
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 412
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 413
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 414
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 415
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 416
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 417
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 418
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 419
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 420
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 421
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 422
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 423
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 424
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 425
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 426
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 427
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 428
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 429
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 430
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 431
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 432
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 433
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 434
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 435
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 436
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 437
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 438
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 439
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 440
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 441
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 442
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 443
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 444
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 445
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 446
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 447
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 448
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 449
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 450
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 451
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 452
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 453
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 454
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 455
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 456
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 457
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 458
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 459
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 460
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 461
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 462
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 463
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 464
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 465
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 466
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 467
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 468
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 469
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 470
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 471
        EVT_CHANGE_PASSWORD_BACK                                                  , // 472
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 473
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 474
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 475
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 476
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 477
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 478
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 479
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 480
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 481
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 482
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 483
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 484
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 485
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 486
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 487
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 488
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 489
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 490
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 491
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 492
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 493
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 494
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 495
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 496
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 497
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 498
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 499
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 500
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 501
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 502
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 503
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 504
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 505
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 506
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 507
        EVT_UPDATE_PROFILE_REQUEST                                        , // 508
        EVT_UPDATE_PROFILE_BACK                                           , // 509
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 510
    };
    enum EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS
    {
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MIN=EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX+1, // 511
        EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST                                              , // 512
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX                                      , // 513
    };
    enum EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD
    {
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MIN=EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX+1, // 514
        EVT_INPUT_BACKUP_PASSWORD_REQUEST                                                            , // 515
        EVT_ENTER_BACKUP_PASSWORD_BACK                                                               , // 516
        EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST                                                      , // 517
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX                                                   , // 518
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX+1, // 519
        EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST                                                                  , // 520
        EVT_ANSER_SECURITY_QUESTION_BACK                                                                      , // 521
        EVT_ENTER_BACKUP_PASSWORD_RERQUEST                                                                    , // 522
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX                                             , // 523
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX+1, // 524
        EVT_INPUT_TAPSIGNER_SELECT_REQUEST                                                           , // 525
        EVT_ANSER_SECURITY_QUESTION_REQUEST                                                          , // 526
        EVT_STATE_ID_SCR_KEY_RECOVERY_MAX                                                            , // 527
    };
    enum EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD
    {
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_MAX+1, // 528
        EVT_INPUT_PASSWORD_REQUEST                                                    , // 529
        EVT_REENTER_YOUR_PASSWORD_BACK                                                , // 530
        EVT_KEY_RECOVERY_REQUEST                                                      , // 531
        EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST                                       , // 532
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX                                    , // 533
    };
    enum EVT_STATE_ID_SCR_SERVICE_SETTINGS
    {
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MIN=EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX+1, // 534
        EVT_CLAIM_INHERITANCE_CHECK_REQUEST                                               , // 535
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST                                          , // 536
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED                                          , // 537
        EVT_REENTER_YOUR_PASSWORD_REQUEST                                                 , // 538
        EVT_SERVICE_SUPPORT_REQUEST                                                       , // 539
        EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST                                          , // 540
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX                                             , // 541
    };
    enum EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD
    {
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MIN=EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX+1, // 542
        EVT_INPUT_DAYS_VALUE_REQUEST                                                            , // 543
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST                                            , // 544
        EVT_DUMMY_TRANSACTION_INFO_REQUEST                                                      , // 545
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX                                        , // 546
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX+1, // 547
        EVT_INPUT_LOCKDOWN_ANSER_REQUEST                                                                        , // 548
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK                                                               , // 549
        SCR_LOCKDOWN_SUCCESS_REQUEST                                                                            , // 550
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX                                                   , // 551
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS
    {
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MIN=EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX+1, // 552
        EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST                                                           , // 553
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX                                                        , // 554
    };
    enum EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX+1, // 555
        EVT_DUMMY_TRANSACTION_SIGN_REQUEST                                                 , // 556
        EVT_DUMMY_TRANSACTION_SIGN_CONFIRM_REQUEST                                         , // 557
        EVT_DUMMY_TRANSACTION_VERIFY_ADDRESS                                               , // 558
        EVT_DUMMY_TRANSACTION_SET_MEMO_REQUEST                                             , // 559
        EVT_DUMMY_TRANSACTION_SCAN_DEVICE_REQUEST                                          , // 560
        EVT_DUMMY_TRANSACTION_INFO_BACK                                                    , // 561
        EVT_DUMMY_TRANSACTION_LOCKDOWN_SUCCEEDED_REQUEST                                   , // 562
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX                                        , // 563
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION
    {
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MIN=EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX+1, // 564
        EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ                                                           , // 565
        EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST                                                       , // 566
        EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK                                                          , // 567
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX                                              , // 568
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS
    {
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MIN=EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX+1, // 569
        EVT_INHERITANCE_TRANSACTION_DETAILS_BACK                                                                   , // 570
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX                                                       , // 571
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE
    {
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MIN=EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX+1, // 572
        EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST                                                                 , // 573
        EVT_INHERITANCE_NEW_TRANSACTION_REQ                                                                     , // 574
        EVT_INHERITANCE_WITHDRAW_BALANCE_BACK                                                                   , // 575
        EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST                                                             , // 576
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX                                                       , // 577
    };
    enum EVT_STATE_ID_SCR_ADD_LEDGER_ASK
    {
        EVT_STATE_ID_SCR_ADD_LEDGER_ASK_MIN=EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX+1, // 578
        EVT_ADD_LEDGER_REQUEST                                                                 , // 579
        EVT_STATE_ID_SCR_ADD_LEDGER_ASK_MAX                                                    , // 580
    };
    enum EVT_STATE_ID_SCR_ADD_TREZOR_ASK
    {
        EVT_STATE_ID_SCR_ADD_TREZOR_ASK_MIN=EVT_STATE_ID_SCR_ADD_LEDGER_ASK_MAX+1, // 581
        EVT_ADD_TREZOR_REQUEST                                                   , // 582
        EVT_STATE_ID_SCR_ADD_TREZOR_ASK_MAX                                      , // 583
    };
    enum EVT_STATE_ID_SCR_ADD_LEDGER
    {
        EVT_STATE_ID_SCR_ADD_LEDGER_MIN=EVT_STATE_ID_SCR_ADD_TREZOR_ASK_MAX+1, // 584
        EVT_SCAN_LEDGER_DEVICE_REQUEST                                       , // 585
        EVT_ADD_LEDGER_DEVICE_REQUEST                                        , // 586
        EVT_STATE_ID_SCR_ADD_LEDGER_MAX                                      , // 587
    };
    enum EVT_STATE_ID_SCR_ADD_TREZOR
    {
        EVT_STATE_ID_SCR_ADD_TREZOR_MIN=EVT_STATE_ID_SCR_ADD_LEDGER_MAX+1, // 588
        EVT_SCAN_TREZOR_DEVICE_REQUEST                                   , // 589
        EVT_ADD_TREZOR_DEVICE_REQUEST                                    , // 590
        EVT_STATE_ID_SCR_ADD_TREZOR_MAX                                  , // 591
    };

};
#endif // VIEWSENUMS_H
