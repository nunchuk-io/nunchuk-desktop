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
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_HARDWARE_ASK)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_HARDWARE)
    Q_ENUMS(EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE)
    Q_ENUMS(EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN)
    Q_ENUMS(EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS)
    Q_ENUMS(EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST)
    Q_ENUMS(EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO)
    Q_ENUMS(EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS)
    Q_ENUMS(EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING)
    Q_ENUMS(EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS)
    Q_ENUMS(EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION)
    Q_ENUMS(EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION)
    Q_ENUMS(EVT_STATE_ID_SCR_RECURRING_PAYMENTS)
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
        STATE_ID_SCR_ADD_HARDWARE_ASK                    , // 69
        STATE_ID_SCR_ADD_HARDWARE                        , // 70
        STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE        , // 71
        STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN          , // 72
        STATE_ID_SCR_SHARE_YOUR_SECRETS                  , // 73
        STATE_ID_SCR_ADD_HARDWARE_EXIST                  , // 74
        STATE_ID_SCR_DASHBOARD_ALERT_INFO                , // 75
        STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS             , // 76
        STATE_ID_SCR_HEALTH_CHECK_STARTING               , // 77
        STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS             , // 78
        STATE_ID_SCR_SETUP_SECURITY_QUESTION             , // 79
        STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION      , // 80
        STATE_ID_SCR_RECURRING_PAYMENTS                  , // 81
        EVT_STATE_ID_REGISTED_MAX                        , // 82
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1, // 83
        EVT_STARTING_APPLICATION_LOCALMODE               , // 84
        EVT_STARTING_APPLICATION_ONLINEMODE              , // 85
        EVT_STARTING_APPLICATION_ONLINE_HWL              , // 86
        EVT_SETTING_ACCOUNT_CHANGE_PASSWORD              , // 87
        EVT_ONS_CLOSE_ALL_REQUEST                        , // 88
        EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST             , // 89
        EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST              , // 90
        EVT_GOTO_HOME_WALLET_TAB                         , // 91
        EVT_ONS_CLOSE_REQUEST                            , // 92
        EVT_LOGIN_DB_REQUEST                             , // 93
        EVT_ROOT_PROMT_PIN                               , // 94
        EVT_ROOT_PROMT_PASSPHRASE                        , // 95
        EVT_LOGIN_MATRIX_REQUEST                         , // 96
        EVT_GOTO_HOME_CHAT_TAB                           , // 97
        EVT_ONLINE_ONS_CLOSE_REQUEST                     , // 98
        EVT_SHOW_TOAST_MESSAGE                           , // 99
        EVT_ROOT_UPDATE_PROFILE                          , // 100
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                  , // 101
        EVT_GOTO_APP_SETTINGS_TAB                        , // 102
        EVT_SIGN_IN_REQUEST                              , // 103
        EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST               , // 104
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST              , // 105
        EVT_GOTO_SERVICE_SETTING_TAB                     , // 106
        EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST            , // 107
        EVT_NUNCHUK_LOGIN_SUCCEEDED                      , // 108
        EVT_HEALTH_CHECK_STARTING_REQUEST                , // 109
        EVT_DUMMY_TRANSACTION_INFO_REQUEST               , // 110
        EVT_DASHBOARD_ALERT_SUCCESS_REQUEST              , // 111
        EVT_SHARE_YOUR_SECRET_REQUEST                    , // 112
        EVT_REENTER_YOUR_PASSWORD_REQUEST                , // 113
        EVT_STATE_ID_ROOT_MAX                            , // 114
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 115
        EVT_HOME_WALLET_SELECTED                         , // 116
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 117
        EVT_HOME_SETTING_REQUEST                         , // 118
        EVT_HOME_DISPLAY_ADDRESS                         , // 119
        EVT_HOME_IMPORT_PSBT                             , // 120
        EVT_HOME_EXPORT_BSMS                             , // 121
        EVT_HOME_ADD_WALLET_REQUEST                      , // 122
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 123
        EVT_HOME_SEND_REQUEST                            , // 124
        EVT_HOME_RECEIVE_REQUEST                         , // 125
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 126
        EVT_HOME_WALLET_INFO_REQUEST                     , // 127
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 128
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 129
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 130
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 131
        EVT_ASK_HARDWARE_REQ                             , // 132
        EVT_EXIST_HARDWARE_REQ                           , // 133
        EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST             , // 134
        EVT_KEY_HEALTH_CHECK_STATUS_REQUEST              , // 135
        EVT_RECURRING_PAYMENTS_REQUEST                   , // 136
        EVT_STATE_ID_SCR_HOME_MAX                        , // 137
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 138
        EVT_ADD_WALLET_IMPORT                                      , // 139
        EVT_ADD_WALLET_BACK_REQUEST                                , // 140
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 141
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 142
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 143
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 144
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 145
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 146
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 147
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 148
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 149
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 150
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 151
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 152
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 153
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 154
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 155
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 156
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 157
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 158
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 159
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 160
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 161
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 162
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 163
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 164
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 165
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 166
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 167
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 168
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 169
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 170
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 171
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 172
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 173
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 174
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 175
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 176
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 177
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 178
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 179
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 180
        EVT_RECEIVE_BACK_REQUEST                                                           , // 181
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 182
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 183
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 184
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 185
        EVT_SEND_BACK_REQUEST                                   , // 186
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 187
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 188
        EVT_STATE_ID_SCR_SEND_MAX                               , // 189
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 190
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 191
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 192
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 193
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 194
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 195
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 196
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 197
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 198
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 199
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 200
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 201
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 202
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 203
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 204
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 205
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 206
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 207
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 208
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 209
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 210
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 211
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 212
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 213
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 214
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 215
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 216
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 217
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 218
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 219
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 220
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 221
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 222
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 223
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 224
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 225
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 226
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 227
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 228
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 229
        EVT_APP_SETTING_BACK_REQUEST                                                , // 230
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 231
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 232
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 233
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 234
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 235
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 236
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 237
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 238
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 239
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 240
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 241
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 242
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 243
        EVT_UTXOS_SORT_REQUEST                                                    , // 244
        EVT_UTXOS_BACK_REQUEST                                                    , // 245
        EVT_UTXOS_CONSOLIDATE_REQUEST                                             , // 246
        EVT_UTXOS_ITEM_SELECTED                                                   , // 247
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 248
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 249
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 250
        EVT_WALLET_INFO_EDIT_NAME                                    , // 251
        EVT_WALLET_INFO_REMOVE                                       , // 252
        EVT_WALLET_INFO_EXPORT_DB                                    , // 253
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 254
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 255
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 256
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 257
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 258
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 259
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 260
        EVT_WALLET_INFO_REFRESH_WALLET_REQUEST                       , // 261
        EVT_WALLET_INFO_GAP_LIMIT_REQUEST                            , // 262
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 263
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 264
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 265
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 266
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 267
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 268
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 269
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 270
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 271
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 272
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 273
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 274
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 275
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 276
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 277
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 278
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 279
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 280
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 281
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 282
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 283
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 284
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 285
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 286
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 287
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 288
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 289
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 290
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 291
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 292
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 293
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 294
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 295
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 296
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 297
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 298
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 299
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 300
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 301
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 302
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 303
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 304
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 305
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 306
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 307
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 308
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 309
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 310
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 311
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 312
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 313
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 314
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 315
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 316
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 317
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 318
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 319
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 320
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 321
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 322
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 323
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 324
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 325
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 326
        EVT_INPUT_PIN_SEND_PIN                                           , // 327
        EVT_INPUT_PIN_CLOSE                                              , // 328
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 329
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 330
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 331
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 332
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 333
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 334
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 335
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 336
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 337
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 338
        EVT_CREATE_NEW_SEED                                                               , // 339
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 340
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 341
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 342
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 343
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 344
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 345
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 346
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 347
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 348
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 349
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 350
        EVT_CREATE_NEW_SEED_BACK                                                           , // 351
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 352
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 353
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 354
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 355
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 356
        EVT_ADD_SOFTWARE_SIGNER_RESULT                                                           , // 357
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 358
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 359
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 360
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 361
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 362
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 363
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 364
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 365
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 366
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 367
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 368
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 369
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 370
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 371
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 372
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 373
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 374
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 375
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 376
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 377
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 378
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 379
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 380
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 381
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 382
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 383
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 384
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 385
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 386
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 387
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 388
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 389
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 390
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 391
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 392
        EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ                                 , // 393
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 394
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 395
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 396
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 397
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 398
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 399
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 400
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 401
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 402
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 403
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 404
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 405
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 406
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 407
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 408
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 409
        EVT_PENDING_REQUEST_IGNORE                                                     , // 410
        EVT_PENDING_REQUEST_BACK                                                       , // 411
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 412
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 413
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 414
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 415
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 416
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 417
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 418
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 419
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 420
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 421
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 422
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 423
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 424
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 425
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 426
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 427
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 428
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 429
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 430
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 431
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 432
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 433
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 434
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 435
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 436
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 437
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 438
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 439
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 440
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 441
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 442
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 443
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 444
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 445
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 446
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 447
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 448
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 449
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 450
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 451
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 452
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 453
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 454
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 455
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 456
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 457
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 458
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 459
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 460
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 461
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 462
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 463
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 464
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 465
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 466
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 467
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 468
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 469
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 470
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 471
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 472
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 473
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 474
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 475
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 476
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 477
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 478
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 479
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 480
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 481
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 482
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 483
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 484
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 485
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 486
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 487
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 488
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 489
        EVT_CHANGE_PASSWORD_BACK                                                  , // 490
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 491
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 492
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 493
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 494
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 495
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 496
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 497
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 498
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 499
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 500
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 501
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 502
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 503
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 504
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 505
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 506
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 507
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 508
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 509
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 510
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 511
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 512
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 513
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 514
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 515
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 516
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 517
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 518
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 519
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 520
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 521
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 522
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 523
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 524
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 525
        EVT_UPDATE_PROFILE_REQUEST                                        , // 526
        EVT_UPDATE_PROFILE_BACK                                           , // 527
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 528
    };
    enum EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS
    {
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MIN=EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX+1, // 529
        EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST                                              , // 530
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX                                      , // 531
    };
    enum EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD
    {
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MIN=EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX+1, // 532
        EVT_INPUT_BACKUP_PASSWORD_REQUEST                                                            , // 533
        EVT_ENTER_BACKUP_PASSWORD_BACK                                                               , // 534
        EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST                                                      , // 535
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX                                                   , // 536
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX+1, // 537
        EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST                                                                  , // 538
        EVT_ANSER_SECURITY_QUESTION_BACK                                                                      , // 539
        EVT_ENTER_BACKUP_PASSWORD_RERQUEST                                                                    , // 540
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX                                             , // 541
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX+1, // 542
        EVT_INPUT_TAPSIGNER_SELECT_REQUEST                                                           , // 543
        EVT_ANSER_SECURITY_QUESTION_REQUEST                                                          , // 544
        EVT_STATE_ID_SCR_KEY_RECOVERY_MAX                                                            , // 545
    };
    enum EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD
    {
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_MAX+1, // 546
        EVT_INPUT_PASSWORD_REQUEST                                                    , // 547
        EVT_REENTER_YOUR_PASSWORD_BACK                                                , // 548
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX                                    , // 549
    };
    enum EVT_STATE_ID_SCR_SERVICE_SETTINGS
    {
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MIN=EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX+1, // 550
        EVT_CLAIM_INHERITANCE_CHECK_REQUEST                                               , // 551
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST                                          , // 552
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED                                          , // 553
        EVT_SERVICE_SELECT_WALLET_REQUEST                                                 , // 554
        EVT_INHERITANCE_PLAN_FINALIZE_REQUEST                                             , // 555
        EVT_SERVICE_SUPPORT_REQUEST                                                       , // 556
        EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST                                          , // 557
        EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST                                            , // 558
        EVT_KEY_RECOVERY_REQUEST                                                          , // 559
        EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST                                           , // 560
        EVT_WALLET_CO_SIGN_POLICE_REQUEST                                                 , // 561
        EVT_LOCKDOWN_SUCCESS_REQUEST                                                      , // 562
        EVT_SETUP_SECURITY_QUESTION_REQUEST                                               , // 563
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX                                             , // 564
    };
    enum EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD
    {
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MIN=EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX+1, // 565
        EVT_INPUT_DAYS_VALUE_REQUEST                                                            , // 566
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST                                            , // 567
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX                                        , // 568
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX+1, // 569
        EVT_INPUT_LOCKDOWN_ANSER_REQUEST                                                                        , // 570
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK                                                               , // 571
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX                                                   , // 572
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS
    {
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MIN=EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX+1, // 573
        EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST                                                           , // 574
        EVT_CLOSE_LOCKDOWN_SUCCESS                                                                   , // 575
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX                                                        , // 576
    };
    enum EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX+1, // 577
        EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST                                         , // 578
        EVT_DUMMY_TRANSACTION_INFO_BACK                                                    , // 579
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX                                        , // 580
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION
    {
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MIN=EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX+1, // 581
        EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ                                                           , // 582
        EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST                                                       , // 583
        EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK                                                          , // 584
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX                                              , // 585
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS
    {
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MIN=EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX+1, // 586
        EVT_INHERITANCE_TRANSACTION_DETAILS_BACK                                                                   , // 587
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX                                                       , // 588
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE
    {
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MIN=EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX+1, // 589
        EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST                                                                 , // 590
        EVT_INHERITANCE_NEW_TRANSACTION_REQ                                                                     , // 591
        EVT_INHERITANCE_WITHDRAW_BALANCE_BACK                                                                   , // 592
        EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST                                                             , // 593
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX                                                       , // 594
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_ASK
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MIN=EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX+1, // 595
        EVT_ADD_HARDWARE_REQUEST                                                                 , // 596
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX                                                    , // 597
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX+1, // 598
        EVT_SCAN_HARDWARE_DEVICE_REQUEST                                         , // 599
        EVT_ADD_HARDWARE_DEVICE_REQUEST                                          , // 600
        EVT_STATE_ID_SCR_ADD_HARDWARE_MAX                                        , // 601
    };
    enum EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE
    {
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_MAX+1, // 602
        EVT_SELECT_WALLET_REQUEST                                                            , // 603
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX                                    , // 604
    };
    enum EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN
    {
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MIN=EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX+1, // 605
        EVT_UPDATE_ACTIVATION_DATE_REQUEST                                                                 , // 606
        EVT_UPDATE_MESSAGE_REQUEST                                                                         , // 607
        EVT_UPDATE_BUFFER_PERIOD_REQUEST                                                                   , // 608
        EVT_UPDATE_NOTIFICATION_PREFERENCE_REQ                                                             , // 609
        EVT_NOT_UPDATE_ANY_NOTI_REQ                                                                        , // 610
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX                                                    , // 611
    };
    enum EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS
    {
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MIN=EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX+1, // 612
        EVT_UPDATE_YOUR_SECRET_REQUEST                                                           , // 613
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX                                                  , // 614
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MIN=EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX+1, // 615
        EVT_ADD_EXIST_HARDWARE_REQUEST                                                   , // 616
        EVT_ADD_NEW_HARDWARE_REQUEST                                                     , // 617
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX                                          , // 618
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX+1, // 619
        EVT_DASHBOARD_ALERT_INFO_ENTER                                                     , // 620
        EVT_ADD_HARDWARE_KEY_EXIST_REQ                                                     , // 621
        EVT_ADD_HARDWARE_KEY_REQUEST                                                       , // 622
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX                                          , // 623
    };
    enum EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS
    {
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX+1, // 624
        EVT_KEY_HEALTH_CHECK_STATUS_ENTER                                                       , // 625
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX                                            , // 626
    };
    enum EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING
    {
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MIN=EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX+1, // 627
        EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST                                                    , // 628
        EVT_HEALTH_CHECK_STARTING_CLOSE                                                          , // 629
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX                                               , // 630
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MIN=EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX+1, // 631
        EVT_DASHBOARD_ALERT_SUCCESS_ENTER_REQUEST                                                , // 632
        EVT_DASHBOARD_ALERT_SUCCESS_CLOSE                                                        , // 633
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX                                             , // 634
    };
    enum EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX+1, // 635
        EVT_INPUT_SECURITY_QUESTION_REQUEST                                                        , // 636
        EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ                                                     , // 637
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX                                               , // 638
    };
    enum EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX+1, // 639
        EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ                                                            , // 640
        EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK                                                           , // 641
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX                                               , // 642
    };
    enum EVT_STATE_ID_SCR_RECURRING_PAYMENTS
    {
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MIN=EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX+1, // 643
        EVT_RECURRING_PAYMENTS_ENTER                                                                 , // 644
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX                                                      , // 645
    };

};
#endif // VIEWSENUMS_H
