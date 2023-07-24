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
    Q_ENUMS(EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN)
    Q_ENUMS(EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_COLDCARD_ASK)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_COLDCARD)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_LEDGER_EXIST)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_TREZOR_EXIST)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_COLDCARD_EXIST)
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
        STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN          , // 74
        STATE_ID_SCR_SHARE_YOUR_SECRETS                  , // 75
        STATE_ID_SCR_ADD_COLDCARD_ASK                    , // 76
        STATE_ID_SCR_ADD_COLDCARD                        , // 77
        STATE_ID_SCR_ADD_LEDGER_EXIST                    , // 78
        STATE_ID_SCR_ADD_TREZOR_EXIST                    , // 79
        STATE_ID_SCR_ADD_COLDCARD_EXIST                  , // 80
        EVT_STATE_ID_REGISTED_MAX                        , // 81
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1, // 82
        EVT_STARTING_APPLICATION_LOCALMODE               , // 83
        EVT_STARTING_APPLICATION_ONLINEMODE              , // 84
        EVT_STARTING_APPLICATION_ONLINE_HWL              , // 85
        EVT_SETTING_ACCOUNT_CHANGE_PASSWORD              , // 86
        EVT_ONS_CLOSE_ALL_REQUEST                        , // 87
        EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST             , // 88
        EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST              , // 89
        EVT_GOTO_HOME_WALLET_TAB                         , // 90
        EVT_ONS_CLOSE_REQUEST                            , // 91
        EVT_LOGIN_DB_REQUEST                             , // 92
        EVT_ROOT_PROMT_PIN                               , // 93
        EVT_ROOT_PROMT_PASSPHRASE                        , // 94
        EVT_LOGIN_MATRIX_REQUEST                         , // 95
        EVT_GOTO_HOME_CHAT_TAB                           , // 96
        EVT_ONLINE_ONS_CLOSE_REQUEST                     , // 97
        EVT_SHOW_TOAST_MESSAGE                           , // 98
        EVT_ROOT_UPDATE_PROFILE                          , // 99
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                  , // 100
        EVT_GOTO_APP_SETTINGS_TAB                        , // 101
        EVT_SIGN_IN_REQUEST                              , // 102
        EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST               , // 103
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST              , // 104
        EVT_GOTO_SERVICE_SETTING_TAB                     , // 105
        EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST            , // 106
        EVT_NUNCHUK_LOGIN_SUCCEEDED                      , // 107
        EVT_STATE_ID_ROOT_MAX                            , // 108
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 109
        EVT_HOME_WALLET_SELECTED                         , // 110
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 111
        EVT_HOME_SETTING_REQUEST                         , // 112
        EVT_HOME_DISPLAY_ADDRESS                         , // 113
        EVT_HOME_IMPORT_PSBT                             , // 114
        EVT_HOME_EXPORT_BSMS                             , // 115
        EVT_HOME_ADD_WALLET_REQUEST                      , // 116
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 117
        EVT_HOME_SEND_REQUEST                            , // 118
        EVT_HOME_RECEIVE_REQUEST                         , // 119
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 120
        EVT_HOME_WALLET_INFO_REQUEST                     , // 121
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 122
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 123
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 124
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 125
        EVT_ASK_LEDGER_REQ                               , // 126
        EVT_ASK_TREZOR_REQ                               , // 127
        EVT_EXIST_LEDGER_REQ                             , // 128
        EVT_EXIST_TREZOR_REQ                             , // 129
        EVT_EXIST_COLDCARD_REQ                           , // 130
        EVT_ASK_COLDCARD_REQ                             , // 131
        EVT_STATE_ID_SCR_HOME_MAX                        , // 132
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 133
        EVT_ADD_WALLET_IMPORT                                      , // 134
        EVT_ADD_WALLET_BACK_REQUEST                                , // 135
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 136
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 137
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 138
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 139
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 140
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 141
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 142
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 143
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 144
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 145
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 146
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 147
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 148
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 149
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 150
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 151
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 152
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 153
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 154
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 155
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 156
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 157
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 158
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 159
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 160
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 161
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 162
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 163
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 164
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 165
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 166
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 167
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 168
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 169
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 170
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 171
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 172
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 173
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 174
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 175
        EVT_RECEIVE_BACK_REQUEST                                                           , // 176
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 177
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 178
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 179
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 180
        EVT_SEND_BACK_REQUEST                                   , // 181
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 182
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 183
        EVT_STATE_ID_SCR_SEND_MAX                               , // 184
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 185
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 186
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 187
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 188
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 189
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 190
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 191
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 192
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 193
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 194
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 195
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 196
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 197
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 198
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 199
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 200
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 201
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 202
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 203
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 204
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 205
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 206
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 207
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 208
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 209
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 210
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 211
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 212
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 213
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 214
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 215
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 216
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 217
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 218
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 219
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 220
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 221
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 222
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 223
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 224
        EVT_APP_SETTING_BACK_REQUEST                                                , // 225
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 226
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 227
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 228
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 229
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 230
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 231
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 232
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 233
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 234
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 235
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 236
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 237
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 238
        EVT_UTXOS_SORT_REQUEST                                                    , // 239
        EVT_UTXOS_BACK_REQUEST                                                    , // 240
        EVT_UTXOS_CONSOLIDATE_REQUEST                                             , // 241
        EVT_UTXOS_ITEM_SELECTED                                                   , // 242
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 243
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 244
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 245
        EVT_WALLET_INFO_EDIT_NAME                                    , // 246
        EVT_WALLET_INFO_REMOVE                                       , // 247
        EVT_WALLET_INFO_EXPORT_DB                                    , // 248
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 249
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 250
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 251
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 252
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 253
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 254
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 255
        EVT_WALLET_INFO_REFRESH_WALLET_REQUEST                       , // 256
        EVT_WALLET_INFO_GAP_LIMIT_REQUEST                            , // 257
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 258
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 259
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 260
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 261
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 262
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 263
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 264
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 265
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 266
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 267
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 268
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 269
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 270
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 271
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 272
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 273
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 274
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 275
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 276
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 277
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 278
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 279
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 280
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 281
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 282
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 283
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 284
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 285
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 286
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 287
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 288
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 289
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 290
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 291
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 292
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 293
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 294
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 295
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 296
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 297
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 298
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 299
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 300
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 301
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 302
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 303
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 304
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 305
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 306
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 307
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 308
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 309
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 310
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 311
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 312
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 313
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 314
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 315
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 316
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 317
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 318
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 319
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 320
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 321
        EVT_INPUT_PIN_SEND_PIN                                           , // 322
        EVT_INPUT_PIN_CLOSE                                              , // 323
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 324
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 325
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 326
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 327
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 328
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 329
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 330
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 331
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 332
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 333
        EVT_CREATE_NEW_SEED                                                               , // 334
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 335
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 336
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 337
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 338
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 339
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 340
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 341
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 342
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 343
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 344
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 345
        EVT_CREATE_NEW_SEED_BACK                                                           , // 346
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 347
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 348
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 349
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 350
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 351
        EVT_ADD_SOFTWARE_SIGNER_RESULT                                                           , // 352
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 353
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 354
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 355
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 356
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 357
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 358
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 359
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 360
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 361
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 362
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 363
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 364
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 365
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 366
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 367
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 368
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 369
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 370
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 371
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 372
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 373
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 374
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 375
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 376
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 377
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 378
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 379
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 380
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 381
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 382
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 383
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 384
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 385
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 386
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 387
        EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ                                 , // 388
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 389
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 390
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 391
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 392
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 393
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 394
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 395
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 396
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 397
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 398
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 399
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 400
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 401
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 402
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 403
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 404
        EVT_PENDING_REQUEST_IGNORE                                                     , // 405
        EVT_PENDING_REQUEST_BACK                                                       , // 406
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 407
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 408
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 409
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 410
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 411
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 412
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 413
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 414
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 415
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 416
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 417
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 418
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 419
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 420
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 421
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 422
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 423
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 424
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 425
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 426
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 427
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 428
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 429
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 430
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 431
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 432
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 433
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 434
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 435
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 436
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 437
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 438
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 439
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 440
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 441
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 442
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 443
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 444
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 445
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 446
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 447
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 448
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 449
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 450
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 451
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 452
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 453
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 454
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 455
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 456
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 457
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 458
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 459
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 460
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 461
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 462
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 463
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 464
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 465
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 466
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 467
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 468
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 469
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 470
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 471
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 472
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 473
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 474
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 475
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 476
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 477
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 478
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 479
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 480
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 481
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 482
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 483
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 484
        EVT_CHANGE_PASSWORD_BACK                                                  , // 485
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 486
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 487
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 488
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 489
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 490
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 491
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 492
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 493
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 494
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 495
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 496
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 497
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 498
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 499
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 500
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 501
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 502
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 503
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 504
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 505
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 506
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 507
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 508
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 509
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 510
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 511
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 512
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 513
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 514
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 515
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 516
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 517
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 518
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 519
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 520
        EVT_UPDATE_PROFILE_REQUEST                                        , // 521
        EVT_UPDATE_PROFILE_BACK                                           , // 522
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 523
    };
    enum EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS
    {
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MIN=EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX+1, // 524
        EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST                                              , // 525
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX                                      , // 526
    };
    enum EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD
    {
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MIN=EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX+1, // 527
        EVT_INPUT_BACKUP_PASSWORD_REQUEST                                                            , // 528
        EVT_ENTER_BACKUP_PASSWORD_BACK                                                               , // 529
        EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST                                                      , // 530
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX                                                   , // 531
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX+1, // 532
        EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST                                                                  , // 533
        EVT_ANSER_SECURITY_QUESTION_BACK                                                                      , // 534
        EVT_ENTER_BACKUP_PASSWORD_RERQUEST                                                                    , // 535
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX                                             , // 536
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX+1, // 537
        EVT_INPUT_TAPSIGNER_SELECT_REQUEST                                                           , // 538
        EVT_ANSER_SECURITY_QUESTION_REQUEST                                                          , // 539
        EVT_STATE_ID_SCR_KEY_RECOVERY_MAX                                                            , // 540
    };
    enum EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD
    {
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_MAX+1, // 541
        EVT_INPUT_PASSWORD_REQUEST                                                    , // 542
        EVT_REENTER_YOUR_PASSWORD_BACK                                                , // 543
        EVT_KEY_RECOVERY_REQUEST                                                      , // 544
        EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST                                       , // 545
        EVT_WALLET_CO_SIGN_POLICE_REQUEST                                             , // 546
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX                                    , // 547
    };
    enum EVT_STATE_ID_SCR_SERVICE_SETTINGS
    {
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MIN=EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX+1, // 548
        EVT_CLAIM_INHERITANCE_CHECK_REQUEST                                               , // 549
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST                                          , // 550
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED                                          , // 551
        EVT_SERVICE_SELECT_WALLET_REQUEST                                                 , // 552
        EVT_INHERITANCE_PLAN_FINALIZE_REQUEST                                             , // 553
        EVT_REENTER_YOUR_PASSWORD_REQUEST                                                 , // 554
        EVT_SERVICE_SUPPORT_REQUEST                                                       , // 555
        EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST                                          , // 556
        EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST                                            , // 557
        EVT_SHARE_YOUR_SECRET_REQUEST                                                     , // 558
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX                                             , // 559
    };
    enum EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD
    {
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MIN=EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX+1, // 560
        EVT_INPUT_DAYS_VALUE_REQUEST                                                            , // 561
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST                                            , // 562
        EVT_DUMMY_TRANSACTION_INFO_REQUEST                                                      , // 563
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX                                        , // 564
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX+1, // 565
        EVT_INPUT_LOCKDOWN_ANSER_REQUEST                                                                        , // 566
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK                                                               , // 567
        SCR_LOCKDOWN_SUCCESS_REQUEST                                                                            , // 568
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX                                                   , // 569
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS
    {
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MIN=EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX+1, // 570
        EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST                                                           , // 571
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX                                                        , // 572
    };
    enum EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX+1, // 573
        EVT_DUMMY_TRANSACTION_SIGN_REQUEST                                                 , // 574
        EVT_DUMMY_TRANSACTION_SIGN_CONFIRM_REQUEST                                         , // 575
        EVT_DUMMY_TRANSACTION_VERIFY_ADDRESS                                               , // 576
        EVT_DUMMY_TRANSACTION_SET_MEMO_REQUEST                                             , // 577
        EVT_DUMMY_TRANSACTION_SCAN_DEVICE_REQUEST                                          , // 578
        EVT_DUMMY_TRANSACTION_INFO_BACK                                                    , // 579
        EVT_DUMMY_TRANSACTION_LOCKDOWN_SUCCEEDED_REQUEST                                   , // 580
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX                                        , // 581
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION
    {
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MIN=EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX+1, // 582
        EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ                                                           , // 583
        EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST                                                       , // 584
        EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK                                                          , // 585
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX                                              , // 586
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS
    {
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MIN=EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX+1, // 587
        EVT_INHERITANCE_TRANSACTION_DETAILS_BACK                                                                   , // 588
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX                                                       , // 589
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE
    {
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MIN=EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX+1, // 590
        EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST                                                                 , // 591
        EVT_INHERITANCE_NEW_TRANSACTION_REQ                                                                     , // 592
        EVT_INHERITANCE_WITHDRAW_BALANCE_BACK                                                                   , // 593
        EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST                                                             , // 594
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX                                                       , // 595
    };
    enum EVT_STATE_ID_SCR_ADD_LEDGER_ASK
    {
        EVT_STATE_ID_SCR_ADD_LEDGER_ASK_MIN=EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX+1, // 596
        EVT_ADD_LEDGER_REQUEST                                                                 , // 597
        EVT_STATE_ID_SCR_ADD_LEDGER_ASK_MAX                                                    , // 598
    };
    enum EVT_STATE_ID_SCR_ADD_TREZOR_ASK
    {
        EVT_STATE_ID_SCR_ADD_TREZOR_ASK_MIN=EVT_STATE_ID_SCR_ADD_LEDGER_ASK_MAX+1, // 599
        EVT_ADD_TREZOR_REQUEST                                                   , // 600
        EVT_STATE_ID_SCR_ADD_TREZOR_ASK_MAX                                      , // 601
    };
    enum EVT_STATE_ID_SCR_ADD_LEDGER
    {
        EVT_STATE_ID_SCR_ADD_LEDGER_MIN=EVT_STATE_ID_SCR_ADD_TREZOR_ASK_MAX+1, // 602
        EVT_SCAN_LEDGER_DEVICE_REQUEST                                       , // 603
        EVT_ADD_LEDGER_DEVICE_REQUEST                                        , // 604
        EVT_STATE_ID_SCR_ADD_LEDGER_MAX                                      , // 605
    };
    enum EVT_STATE_ID_SCR_ADD_TREZOR
    {
        EVT_STATE_ID_SCR_ADD_TREZOR_MIN=EVT_STATE_ID_SCR_ADD_LEDGER_MAX+1, // 606
        EVT_SCAN_TREZOR_DEVICE_REQUEST                                   , // 607
        EVT_ADD_TREZOR_DEVICE_REQUEST                                    , // 608
        EVT_STATE_ID_SCR_ADD_TREZOR_MAX                                  , // 609
    };
    enum EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE
    {
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MIN=EVT_STATE_ID_SCR_ADD_TREZOR_MAX+1, // 610
        EVT_SELECT_WALLET_REQUEST                                                          , // 611
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX                                  , // 612
    };
    enum EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN
    {
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MIN=EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX+1, // 613
        EVT_UPDATE_ACTIVATION_DATE_REQUEST                                                                 , // 614
        EVT_UPDATE_MESSAGE_REQUEST                                                                         , // 615
        EVT_UPDATE_BUFFER_PERIOD_REQUEST                                                                   , // 616
        EVT_UPDATE_NOTIFICATION_PREFERENCE_REQ                                                             , // 617
        EVT_NOT_UPDATE_ANY_NOTI_REQ                                                                        , // 618
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX                                                    , // 619
    };
    enum EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS
    {
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MIN=EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX+1, // 620
        EVT_UPDATE_YOUR_SECRET_REQUEST                                                           , // 621
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX                                                  , // 622
    };
    enum EVT_STATE_ID_SCR_ADD_COLDCARD_ASK
    {
        EVT_STATE_ID_SCR_ADD_COLDCARD_ASK_MIN=EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX+1, // 623
        EVT_ADD_COLDCARD_REQUEST                                                       , // 624
        EVT_STATE_ID_SCR_ADD_COLDCARD_ASK_MAX                                          , // 625
    };
    enum EVT_STATE_ID_SCR_ADD_COLDCARD
    {
        EVT_STATE_ID_SCR_ADD_COLDCARD_MIN=EVT_STATE_ID_SCR_ADD_COLDCARD_ASK_MAX+1, // 626
        EVT_SCAN_COLDCARD_DEVICE_REQUEST                                         , // 627
        EVT_ADD_COLDCARD_DEVICE_REQUEST                                          , // 628
        EVT_STATE_ID_SCR_ADD_COLDCARD_MAX                                        , // 629
    };
    enum EVT_STATE_ID_SCR_ADD_LEDGER_EXIST
    {
        EVT_STATE_ID_SCR_ADD_LEDGER_EXIST_MIN=EVT_STATE_ID_SCR_ADD_COLDCARD_MAX+1, // 630
        EVT_ADD_EXIST_LEDGER_REQUEST                                             , // 631
        EVT_ADD_NEW_LEDGER_REQUEST                                               , // 632
        EVT_STATE_ID_SCR_ADD_LEDGER_EXIST_MAX                                    , // 633
    };
    enum EVT_STATE_ID_SCR_ADD_TREZOR_EXIST
    {
        EVT_STATE_ID_SCR_ADD_TREZOR_EXIST_MIN=EVT_STATE_ID_SCR_ADD_LEDGER_EXIST_MAX+1, // 634
        EVT_ADD_EXIST_TREZOR_REQUEST                                                 , // 635
        EVT_ADD_NEW_TREZOR_REQUEST                                                   , // 636
        EVT_STATE_ID_SCR_ADD_TREZOR_EXIST_MAX                                        , // 637
    };
    enum EVT_STATE_ID_SCR_ADD_COLDCARD_EXIST
    {
        EVT_STATE_ID_SCR_ADD_COLDCARD_EXIST_MIN=EVT_STATE_ID_SCR_ADD_TREZOR_EXIST_MAX+1, // 638
        EVT_ADD_EXIST_COLDCARD_REQUEST                                                 , // 639
        EVT_ADD_NEW_COLDCARD_REQUEST                                                   , // 640
        EVT_STATE_ID_SCR_ADD_COLDCARD_EXIST_MAX                                        , // 641
    };

};
#endif // VIEWSENUMS_H
