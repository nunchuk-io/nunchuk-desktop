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
        EVT_STATE_ID_REGISTED_MAX                        , // 66
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1, // 67
        EVT_STARTING_APPLICATION_LOCALMODE               , // 68
        EVT_STARTING_APPLICATION_ONLINEMODE              , // 69
        EVT_STARTING_APPLICATION_ONLINE_HWL              , // 70
        EVT_SETTING_ACCOUNT_CHANGE_PASSWORD              , // 71
        EVT_ONS_CLOSE_ALL_REQUEST                        , // 72
        EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST             , // 73
        EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST              , // 74
        EVT_GOTO_HOME_WALLET_TAB                         , // 75
        EVT_ONS_CLOSE_REQUEST                            , // 76
        EVT_LOGIN_DB_REQUEST                             , // 77
        EVT_ROOT_PROMT_PIN                               , // 78
        EVT_ROOT_PROMT_PASSPHRASE                        , // 79
        EVT_LOGIN_MATRIX_REQUEST                         , // 80
        EVT_GOTO_HOME_CHAT_TAB                           , // 81
        EVT_ONLINE_ONS_CLOSE_REQUEST                     , // 82
        EVT_SHOW_TOAST_MESSAGE                           , // 83
        EVT_ROOT_UPDATE_PROFILE                          , // 84
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                  , // 85
        EVT_GOTO_APP_SETTINGS_TAB                        , // 86
        EVT_SIGN_IN_REQUEST                              , // 87
        EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST               , // 88
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST              , // 89
        EVT_GOTO_SERVICE_SETTING_TAB                     , // 90
        EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST            , // 91
        EVT_NUNCHUK_LOGIN_SUCCEEDED                      , // 92
        EVT_STATE_ID_ROOT_MAX                            , // 93
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 94
        EVT_HOME_WALLET_SELECTED                         , // 95
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 96
        EVT_HOME_SETTING_REQUEST                         , // 97
        EVT_HOME_DISPLAY_ADDRESS                         , // 98
        EVT_HOME_IMPORT_PSBT                             , // 99
        EVT_HOME_EXPORT_BSMS                             , // 100
        EVT_HOME_ADD_WALLET_REQUEST                      , // 101
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 102
        EVT_HOME_SEND_REQUEST                            , // 103
        EVT_HOME_RECEIVE_REQUEST                         , // 104
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 105
        EVT_HOME_WALLET_INFO_REQUEST                     , // 106
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 107
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 108
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 109
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 110
        EVT_STATE_ID_SCR_HOME_MAX                        , // 111
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 112
        EVT_ADD_WALLET_IMPORT                                      , // 113
        EVT_ADD_WALLET_BACK_REQUEST                                , // 114
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 115
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 116
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 117
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 118
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 119
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 120
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 121
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 122
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 123
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 124
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 125
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 126
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 127
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 128
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 129
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 130
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 131
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 132
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 133
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 134
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 135
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 136
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 137
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 138
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 139
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 140
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 141
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 142
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 143
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 144
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 145
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 146
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 147
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 148
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 149
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 150
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 151
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 152
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 153
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 154
        EVT_RECEIVE_BACK_REQUEST                                                           , // 155
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 156
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 157
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 158
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 159
        EVT_SEND_BACK_REQUEST                                   , // 160
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 161
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 162
        EVT_STATE_ID_SCR_SEND_MAX                               , // 163
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 164
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 165
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 166
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 167
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 168
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 169
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 170
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 171
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 172
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 173
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 174
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 175
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 176
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 177
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 178
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 179
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 180
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 181
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 182
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 183
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 184
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 185
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 186
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 187
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 188
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 189
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 190
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 191
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 192
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 193
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 194
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 195
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 196
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 197
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 198
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 199
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 200
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 201
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 202
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 203
        EVT_APP_SETTING_BACK_REQUEST                                                , // 204
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 205
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 206
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 207
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 208
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 209
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 210
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 211
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 212
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 213
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 214
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 215
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 216
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 217
        EVT_UTXOS_SORT_REQUEST                                                    , // 218
        EVT_UTXOS_BACK_REQUEST                                                    , // 219
        EVT_UTXOS_CONSOLIDATE_REQUEST                                             , // 220
        EVT_UTXOS_ITEM_SELECTED                                                   , // 221
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 222
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 223
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 224
        EVT_WALLET_INFO_EDIT_NAME                                    , // 225
        EVT_WALLET_INFO_REMOVE                                       , // 226
        EVT_WALLET_INFO_EXPORT_DB                                    , // 227
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 228
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 229
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 230
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 231
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 232
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 233
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 234
        EVT_WALLET_INFO_REFRESH_WALLET_REQUEST                       , // 235
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 236
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 237
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 238
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 239
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 240
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 241
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 242
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 243
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 244
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 245
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 246
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 247
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 248
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 249
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 250
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 251
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 252
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 253
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 254
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 255
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 256
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 257
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 258
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 259
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 260
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 261
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 262
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 263
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 264
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 265
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 266
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 267
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 268
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 269
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 270
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 271
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 272
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 273
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 274
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 275
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 276
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 277
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 278
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 279
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 280
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 281
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 282
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 283
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 284
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 285
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 286
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 287
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 288
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 289
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 290
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 291
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 292
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 293
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 294
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 295
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 296
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 297
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 298
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 299
        EVT_INPUT_PIN_SEND_PIN                                           , // 300
        EVT_INPUT_PIN_CLOSE                                              , // 301
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 302
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 303
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 304
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 305
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 306
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 307
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 308
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 309
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 310
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 311
        EVT_CREATE_NEW_SEED                                                               , // 312
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 313
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 314
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 315
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 316
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 317
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 318
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 319
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 320
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 321
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 322
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 323
        EVT_CREATE_NEW_SEED_BACK                                                           , // 324
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 325
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 326
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 327
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 328
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 329
        EVT_ADD_SOFTWARE_SIGNER_RESULT                                                           , // 330
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 331
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 332
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 333
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 334
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 335
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 336
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 337
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 338
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 339
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 340
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 341
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 342
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 343
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 344
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 345
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 346
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 347
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 348
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 349
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 350
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 351
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 352
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 353
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 354
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 355
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 356
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 357
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 358
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 359
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 360
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 361
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 362
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 363
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 364
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 365
        EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ                                 , // 366
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 367
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 368
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 369
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 370
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 371
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 372
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 373
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 374
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 375
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 376
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 377
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 378
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 379
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 380
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 381
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 382
        EVT_PENDING_REQUEST_IGNORE                                                     , // 383
        EVT_PENDING_REQUEST_BACK                                                       , // 384
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 385
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 386
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 387
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 388
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 389
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 390
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 391
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 392
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 393
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 394
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 395
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 396
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 397
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 398
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 399
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 400
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 401
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 402
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 403
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 404
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 405
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 406
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 407
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 408
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 409
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 410
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 411
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 412
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 413
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 414
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 415
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 416
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 417
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 418
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 419
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 420
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 421
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 422
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 423
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 424
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 425
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 426
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 427
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 428
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 429
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 430
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 431
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 432
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 433
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 434
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 435
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 436
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 437
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 438
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 439
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 440
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 441
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 442
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 443
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 444
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 445
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 446
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 447
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 448
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 449
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 450
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 451
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 452
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 453
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 454
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 455
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 456
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 457
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 458
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 459
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 460
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 461
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 462
        EVT_CHANGE_PASSWORD_BACK                                                  , // 463
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 464
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 465
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 466
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 467
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 468
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 469
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 470
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 471
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 472
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 473
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 474
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 475
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 476
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 477
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 478
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 479
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 480
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 481
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 482
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 483
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 484
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 485
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 486
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 487
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 488
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 489
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 490
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 491
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 492
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 493
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 494
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 495
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 496
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 497
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 498
        EVT_UPDATE_PROFILE_REQUEST                                        , // 499
        EVT_UPDATE_PROFILE_BACK                                           , // 500
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 501
    };
    enum EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS
    {
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MIN=EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX+1, // 502
        EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST                                              , // 503
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX                                      , // 504
    };
    enum EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD
    {
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MIN=EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX+1, // 505
        EVT_INPUT_BACKUP_PASSWORD_REQUEST                                                            , // 506
        EVT_ENTER_BACKUP_PASSWORD_BACK                                                               , // 507
        EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST                                                      , // 508
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX                                                   , // 509
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX+1, // 510
        EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST                                                                  , // 511
        EVT_ANSER_SECURITY_QUESTION_BACK                                                                      , // 512
        EVT_ENTER_BACKUP_PASSWORD_RERQUEST                                                                    , // 513
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX                                             , // 514
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX+1, // 515
        EVT_INPUT_TAPSIGNER_SELECT_REQUEST                                                           , // 516
        EVT_ANSER_SECURITY_QUESTION_REQUEST                                                          , // 517
        EVT_STATE_ID_SCR_KEY_RECOVERY_MAX                                                            , // 518
    };
    enum EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD
    {
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_MAX+1, // 519
        EVT_INPUT_PASSWORD_REQUEST                                                    , // 520
        EVT_REENTER_YOUR_PASSWORD_BACK                                                , // 521
        EVT_KEY_RECOVERY_REQUEST                                                      , // 522
        EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST                                       , // 523
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX                                    , // 524
    };
    enum EVT_STATE_ID_SCR_SERVICE_SETTINGS
    {
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MIN=EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX+1, // 525
        EVT_REENTER_YOUR_PASSWORD_REQUEST                                                 , // 526
        EVT_SERVICE_SUPPORT_REQUEST                                                       , // 527
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX                                             , // 528
    };
    enum EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD
    {
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MIN=EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX+1, // 529
        EVT_INPUT_DAYS_VALUE_REQUEST                                                            , // 530
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST                                            , // 531
        EVT_DUMMY_TRANSACTION_INFO_REQUEST                                                      , // 532
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX                                        , // 533
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX+1, // 534
        EVT_INPUT_LOCKDOWN_ANSER_REQUEST                                                                        , // 535
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK                                                               , // 536
        SCR_LOCKDOWN_SUCCESS_REQUEST                                                                            , // 537
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX                                                   , // 538
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS
    {
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MIN=EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX+1, // 539
        EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST                                                           , // 540
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX                                                        , // 541
    };
    enum EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX+1, // 542
        EVT_DUMMY_TRANSACTION_SIGN_REQUEST                                                 , // 543
        EVT_DUMMY_TRANSACTION_SIGN_CONFIRM_REQUEST                                         , // 544
        EVT_DUMMY_TRANSACTION_VERIFY_ADDRESS                                               , // 545
        EVT_DUMMY_TRANSACTION_SET_MEMO_REQUEST                                             , // 546
        EVT_DUMMY_TRANSACTION_SCAN_DEVICE_REQUEST                                          , // 547
        EVT_DUMMY_TRANSACTION_INFO_BACK                                                    , // 548
        EVT_DUMMY_TRANSACTION_LOCKDOWN_SUCCEEDED_REQUEST                                   , // 549
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX                                        , // 550
    };

};
#endif // VIEWSENUMS_H
