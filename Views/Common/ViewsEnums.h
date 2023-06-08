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
    Q_ENUMS(EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE)
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
        STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE        , // 73
        EVT_STATE_ID_REGISTED_MAX                        , // 74
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1, // 75
        EVT_STARTING_APPLICATION_LOCALMODE               , // 76
        EVT_STARTING_APPLICATION_ONLINEMODE              , // 77
        EVT_STARTING_APPLICATION_ONLINE_HWL              , // 78
        EVT_SETTING_ACCOUNT_CHANGE_PASSWORD              , // 79
        EVT_ONS_CLOSE_ALL_REQUEST                        , // 80
        EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST             , // 81
        EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST              , // 82
        EVT_GOTO_HOME_WALLET_TAB                         , // 83
        EVT_ONS_CLOSE_REQUEST                            , // 84
        EVT_LOGIN_DB_REQUEST                             , // 85
        EVT_ROOT_PROMT_PIN                               , // 86
        EVT_ROOT_PROMT_PASSPHRASE                        , // 87
        EVT_LOGIN_MATRIX_REQUEST                         , // 88
        EVT_GOTO_HOME_CHAT_TAB                           , // 89
        EVT_ONLINE_ONS_CLOSE_REQUEST                     , // 90
        EVT_SHOW_TOAST_MESSAGE                           , // 91
        EVT_ROOT_UPDATE_PROFILE                          , // 92
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                  , // 93
        EVT_GOTO_APP_SETTINGS_TAB                        , // 94
        EVT_SIGN_IN_REQUEST                              , // 95
        EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST               , // 96
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST              , // 97
        EVT_GOTO_SERVICE_SETTING_TAB                     , // 98
        EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST            , // 99
        EVT_NUNCHUK_LOGIN_SUCCEEDED                      , // 100
        EVT_STATE_ID_ROOT_MAX                            , // 101
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 102
        EVT_HOME_WALLET_SELECTED                         , // 103
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 104
        EVT_HOME_SETTING_REQUEST                         , // 105
        EVT_HOME_DISPLAY_ADDRESS                         , // 106
        EVT_HOME_IMPORT_PSBT                             , // 107
        EVT_HOME_EXPORT_BSMS                             , // 108
        EVT_HOME_ADD_WALLET_REQUEST                      , // 109
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 110
        EVT_HOME_SEND_REQUEST                            , // 111
        EVT_HOME_RECEIVE_REQUEST                         , // 112
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 113
        EVT_HOME_WALLET_INFO_REQUEST                     , // 114
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 115
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 116
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 117
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 118
        EVT_ASK_LEDGER_REQ                               , // 119
        EVT_ASK_TREZOR_REQ                               , // 120
        EVT_STATE_ID_SCR_HOME_MAX                        , // 121
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 122
        EVT_ADD_WALLET_IMPORT                                      , // 123
        EVT_ADD_WALLET_BACK_REQUEST                                , // 124
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 125
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 126
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 127
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 128
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 129
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 130
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 131
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 132
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 133
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 134
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 135
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 136
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 137
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 138
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 139
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 140
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 141
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 142
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 143
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 144
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 145
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 146
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 147
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 148
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 149
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 150
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 151
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 152
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 153
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 154
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 155
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 156
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 157
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 158
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 159
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 160
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 161
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 162
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 163
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 164
        EVT_RECEIVE_BACK_REQUEST                                                           , // 165
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 166
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 167
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 168
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 169
        EVT_SEND_BACK_REQUEST                                   , // 170
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 171
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 172
        EVT_STATE_ID_SCR_SEND_MAX                               , // 173
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 174
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 175
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 176
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 177
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 178
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 179
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 180
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 181
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 182
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 183
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 184
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 185
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 186
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 187
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 188
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 189
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 190
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 191
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 192
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 193
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 194
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 195
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 196
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 197
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 198
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 199
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 200
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 201
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 202
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 203
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 204
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 205
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 206
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 207
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 208
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 209
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 210
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 211
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 212
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 213
        EVT_APP_SETTING_BACK_REQUEST                                                , // 214
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 215
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 216
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 217
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 218
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 219
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 220
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 221
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 222
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 223
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 224
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 225
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 226
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 227
        EVT_UTXOS_SORT_REQUEST                                                    , // 228
        EVT_UTXOS_BACK_REQUEST                                                    , // 229
        EVT_UTXOS_CONSOLIDATE_REQUEST                                             , // 230
        EVT_UTXOS_ITEM_SELECTED                                                   , // 231
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 232
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 233
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 234
        EVT_WALLET_INFO_EDIT_NAME                                    , // 235
        EVT_WALLET_INFO_REMOVE                                       , // 236
        EVT_WALLET_INFO_EXPORT_DB                                    , // 237
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 238
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 239
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 240
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 241
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 242
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 243
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 244
        EVT_WALLET_INFO_REFRESH_WALLET_REQUEST                       , // 245
        EVT_WALLET_INFO_GAP_LIMIT_REQUEST                            , // 246
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 247
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 248
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 249
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 250
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 251
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 252
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 253
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 254
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 255
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 256
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 257
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 258
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 259
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 260
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 261
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 262
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 263
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 264
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 265
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 266
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 267
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 268
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 269
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 270
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 271
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 272
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 273
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 274
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 275
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 276
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 277
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 278
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 279
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 280
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 281
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 282
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 283
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 284
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 285
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 286
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 287
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 288
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 289
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 290
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 291
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 292
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 293
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 294
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 295
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 296
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 297
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 298
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 299
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 300
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 301
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 302
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 303
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 304
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 305
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 306
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 307
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 308
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 309
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 310
        EVT_INPUT_PIN_SEND_PIN                                           , // 311
        EVT_INPUT_PIN_CLOSE                                              , // 312
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 313
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 314
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 315
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 316
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 317
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 318
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 319
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 320
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 321
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 322
        EVT_CREATE_NEW_SEED                                                               , // 323
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 324
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 325
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 326
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 327
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 328
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 329
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 330
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 331
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 332
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 333
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 334
        EVT_CREATE_NEW_SEED_BACK                                                           , // 335
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 336
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 337
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 338
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 339
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 340
        EVT_ADD_SOFTWARE_SIGNER_RESULT                                                           , // 341
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 342
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 343
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 344
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 345
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 346
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 347
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 348
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 349
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 350
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 351
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 352
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 353
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 354
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 355
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 356
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 357
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 358
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 359
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 360
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 361
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 362
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 363
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 364
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 365
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 366
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 367
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 368
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 369
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 370
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 371
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 372
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 373
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 374
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 375
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 376
        EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ                                 , // 377
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 378
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 379
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 380
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 381
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 382
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 383
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 384
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 385
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 386
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 387
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 388
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 389
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 390
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 391
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 392
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 393
        EVT_PENDING_REQUEST_IGNORE                                                     , // 394
        EVT_PENDING_REQUEST_BACK                                                       , // 395
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 396
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 397
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 398
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 399
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 400
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 401
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 402
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 403
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 404
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 405
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 406
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 407
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 408
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 409
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 410
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 411
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 412
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 413
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 414
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 415
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 416
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 417
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 418
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 419
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 420
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 421
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 422
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 423
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 424
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 425
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 426
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 427
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 428
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 429
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 430
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 431
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 432
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 433
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 434
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 435
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 436
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 437
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 438
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 439
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 440
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 441
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 442
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 443
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 444
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 445
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 446
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 447
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 448
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 449
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 450
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 451
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 452
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 453
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 454
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 455
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 456
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 457
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 458
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 459
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 460
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 461
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 462
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 463
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 464
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 465
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 466
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 467
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 468
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 469
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 470
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 471
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 472
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 473
        EVT_CHANGE_PASSWORD_BACK                                                  , // 474
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 475
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 476
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 477
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 478
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 479
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 480
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 481
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 482
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 483
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 484
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 485
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 486
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 487
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 488
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 489
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 490
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 491
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 492
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 493
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 494
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 495
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 496
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 497
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 498
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 499
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 500
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 501
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 502
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 503
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 504
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 505
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 506
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 507
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 508
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 509
        EVT_UPDATE_PROFILE_REQUEST                                        , // 510
        EVT_UPDATE_PROFILE_BACK                                           , // 511
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 512
    };
    enum EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS
    {
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MIN=EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX+1, // 513
        EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST                                              , // 514
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX                                      , // 515
    };
    enum EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD
    {
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MIN=EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX+1, // 516
        EVT_INPUT_BACKUP_PASSWORD_REQUEST                                                            , // 517
        EVT_ENTER_BACKUP_PASSWORD_BACK                                                               , // 518
        EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST                                                      , // 519
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX                                                   , // 520
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX+1, // 521
        EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST                                                                  , // 522
        EVT_ANSER_SECURITY_QUESTION_BACK                                                                      , // 523
        EVT_ENTER_BACKUP_PASSWORD_RERQUEST                                                                    , // 524
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX                                             , // 525
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX+1, // 526
        EVT_INPUT_TAPSIGNER_SELECT_REQUEST                                                           , // 527
        EVT_ANSER_SECURITY_QUESTION_REQUEST                                                          , // 528
        EVT_STATE_ID_SCR_KEY_RECOVERY_MAX                                                            , // 529
    };
    enum EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD
    {
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_MAX+1, // 530
        EVT_INPUT_PASSWORD_REQUEST                                                    , // 531
        EVT_REENTER_YOUR_PASSWORD_BACK                                                , // 532
        EVT_KEY_RECOVERY_REQUEST                                                      , // 533
        EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST                                       , // 534
        EVT_WALLET_CO_SIGN_POLICE_REQUEST                                             , // 535
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX                                    , // 536
    };
    enum EVT_STATE_ID_SCR_SERVICE_SETTINGS
    {
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MIN=EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX+1, // 537
        EVT_CLAIM_INHERITANCE_CHECK_REQUEST                                               , // 538
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST                                          , // 539
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED                                          , // 540
        EVT_REENTER_YOUR_PASSWORD_REQUEST                                                 , // 541
        EVT_SERVICE_SUPPORT_REQUEST                                                       , // 542
        EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST                                          , // 543
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX                                             , // 544
    };
    enum EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD
    {
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MIN=EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX+1, // 545
        EVT_INPUT_DAYS_VALUE_REQUEST                                                            , // 546
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST                                            , // 547
        EVT_DUMMY_TRANSACTION_INFO_REQUEST                                                      , // 548
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX                                        , // 549
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX+1, // 550
        EVT_INPUT_LOCKDOWN_ANSER_REQUEST                                                                        , // 551
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK                                                               , // 552
        SCR_LOCKDOWN_SUCCESS_REQUEST                                                                            , // 553
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX                                                   , // 554
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS
    {
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MIN=EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX+1, // 555
        EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST                                                           , // 556
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX                                                        , // 557
    };
    enum EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX+1, // 558
        EVT_DUMMY_TRANSACTION_SIGN_REQUEST                                                 , // 559
        EVT_DUMMY_TRANSACTION_SIGN_CONFIRM_REQUEST                                         , // 560
        EVT_DUMMY_TRANSACTION_VERIFY_ADDRESS                                               , // 561
        EVT_DUMMY_TRANSACTION_SET_MEMO_REQUEST                                             , // 562
        EVT_DUMMY_TRANSACTION_SCAN_DEVICE_REQUEST                                          , // 563
        EVT_DUMMY_TRANSACTION_INFO_BACK                                                    , // 564
        EVT_DUMMY_TRANSACTION_LOCKDOWN_SUCCEEDED_REQUEST                                   , // 565
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX                                        , // 566
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION
    {
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MIN=EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX+1, // 567
        EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ                                                           , // 568
        EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST                                                       , // 569
        EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK                                                          , // 570
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX                                              , // 571
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS
    {
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MIN=EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX+1, // 572
        EVT_INHERITANCE_TRANSACTION_DETAILS_BACK                                                                   , // 573
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX                                                       , // 574
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE
    {
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MIN=EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX+1, // 575
        EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST                                                                 , // 576
        EVT_INHERITANCE_NEW_TRANSACTION_REQ                                                                     , // 577
        EVT_INHERITANCE_WITHDRAW_BALANCE_BACK                                                                   , // 578
        EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST                                                             , // 579
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX                                                       , // 580
    };
    enum EVT_STATE_ID_SCR_ADD_LEDGER_ASK
    {
        EVT_STATE_ID_SCR_ADD_LEDGER_ASK_MIN=EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX+1, // 581
        EVT_ADD_LEDGER_REQUEST                                                                 , // 582
        EVT_STATE_ID_SCR_ADD_LEDGER_ASK_MAX                                                    , // 583
    };
    enum EVT_STATE_ID_SCR_ADD_TREZOR_ASK
    {
        EVT_STATE_ID_SCR_ADD_TREZOR_ASK_MIN=EVT_STATE_ID_SCR_ADD_LEDGER_ASK_MAX+1, // 584
        EVT_ADD_TREZOR_REQUEST                                                   , // 585
        EVT_STATE_ID_SCR_ADD_TREZOR_ASK_MAX                                      , // 586
    };
    enum EVT_STATE_ID_SCR_ADD_LEDGER
    {
        EVT_STATE_ID_SCR_ADD_LEDGER_MIN=EVT_STATE_ID_SCR_ADD_TREZOR_ASK_MAX+1, // 587
        EVT_SCAN_LEDGER_DEVICE_REQUEST                                       , // 588
        EVT_ADD_LEDGER_DEVICE_REQUEST                                        , // 589
        EVT_STATE_ID_SCR_ADD_LEDGER_MAX                                      , // 590
    };
    enum EVT_STATE_ID_SCR_ADD_TREZOR
    {
        EVT_STATE_ID_SCR_ADD_TREZOR_MIN=EVT_STATE_ID_SCR_ADD_LEDGER_MAX+1, // 591
        EVT_SCAN_TREZOR_DEVICE_REQUEST                                   , // 592
        EVT_ADD_TREZOR_DEVICE_REQUEST                                    , // 593
        EVT_STATE_ID_SCR_ADD_TREZOR_MAX                                  , // 594
    };
    enum EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE
    {
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MIN=EVT_STATE_ID_SCR_ADD_TREZOR_MAX+1, // 595
        EVT_SELECT_WALLET_REQUEST                                                          , // 596
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX                                  , // 597
    };

};
#endif // VIEWSENUMS_H
