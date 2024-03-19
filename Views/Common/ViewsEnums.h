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
    Q_ENUMS(EVT_STATE_ID_SCR_ONBOARDING)
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
        STATE_ID_SCR_ONBOARDING                          , // 82
        EVT_STATE_ID_REGISTED_MAX                        , // 83
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1, // 84
        EVT_STARTING_APPLICATION_LOCALMODE               , // 85
        EVT_STARTING_APPLICATION_ONLINEMODE              , // 86
        EVT_STARTING_APPLICATION_ONLINE_HWL              , // 87
        EVT_SETTING_ACCOUNT_CHANGE_PASSWORD              , // 88
        EVT_ONS_CLOSE_ALL_REQUEST                        , // 89
        EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST             , // 90
        EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST              , // 91
        EVT_GOTO_HOME_WALLET_TAB                         , // 92
        EVT_ONS_CLOSE_REQUEST                            , // 93
        EVT_LOGIN_DB_REQUEST                             , // 94
        EVT_ROOT_PROMT_PIN                               , // 95
        EVT_ROOT_PROMT_PASSPHRASE                        , // 96
        EVT_LOGIN_MATRIX_REQUEST                         , // 97
        EVT_GOTO_HOME_CHAT_TAB                           , // 98
        EVT_ONLINE_ONS_CLOSE_REQUEST                     , // 99
        EVT_SHOW_TOAST_MESSAGE                           , // 100
        EVT_ROOT_UPDATE_PROFILE                          , // 101
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                  , // 102
        EVT_GOTO_APP_SETTINGS_TAB                        , // 103
        EVT_SIGN_IN_REQUEST                              , // 104
        EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST               , // 105
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST              , // 106
        EVT_GOTO_SERVICE_SETTING_TAB                     , // 107
        EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST            , // 108
        EVT_NUNCHUK_LOGIN_SUCCEEDED                      , // 109
        EVT_HEALTH_CHECK_STARTING_REQUEST                , // 110
        EVT_DUMMY_TRANSACTION_INFO_REQUEST               , // 111
        EVT_DASHBOARD_ALERT_SUCCESS_REQUEST              , // 112
        EVT_SHARE_YOUR_SECRET_REQUEST                    , // 113
        EVT_REENTER_YOUR_PASSWORD_REQUEST                , // 114
        EVT_ONBOARDING_REQUEST                           , // 115
        EVT_STATE_ID_ROOT_MAX                            , // 116
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 117
        EVT_HOME_WALLET_SELECTED                         , // 118
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 119
        EVT_HOME_SETTING_REQUEST                         , // 120
        EVT_HOME_DISPLAY_ADDRESS                         , // 121
        EVT_HOME_IMPORT_PSBT                             , // 122
        EVT_HOME_EXPORT_BSMS                             , // 123
        EVT_HOME_ADD_WALLET_REQUEST                      , // 124
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 125
        EVT_HOME_SEND_REQUEST                            , // 126
        EVT_HOME_RECEIVE_REQUEST                         , // 127
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 128
        EVT_HOME_WALLET_INFO_REQUEST                     , // 129
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 130
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 131
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 132
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 133
        EVT_ASK_HARDWARE_REQ                             , // 134
        EVT_EXIST_HARDWARE_REQ                           , // 135
        EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST             , // 136
        EVT_KEY_HEALTH_CHECK_STATUS_REQUEST              , // 137
        EVT_RECURRING_PAYMENTS_REQUEST                   , // 138
        EVT_STATE_ID_SCR_HOME_MAX                        , // 139
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 140
        EVT_ADD_WALLET_IMPORT                                      , // 141
        EVT_ADD_WALLET_BACK_REQUEST                                , // 142
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 143
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 144
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 145
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 146
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 147
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 148
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 149
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 150
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 151
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 152
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 153
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 154
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 155
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 156
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 157
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 158
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 159
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 160
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 161
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 162
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 163
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 164
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 165
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 166
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 167
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 168
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 169
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 170
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 171
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 172
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 173
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 174
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 175
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 176
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 177
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 178
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 179
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 180
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 181
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 182
        EVT_RECEIVE_BACK_REQUEST                                                           , // 183
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 184
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 185
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 186
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 187
        EVT_SEND_BACK_REQUEST                                   , // 188
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 189
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 190
        EVT_STATE_ID_SCR_SEND_MAX                               , // 191
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 192
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 193
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 194
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 195
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 196
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 197
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 198
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 199
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 200
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 201
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 202
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 203
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 204
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 205
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 206
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 207
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 208
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 209
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 210
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 211
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 212
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 213
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 214
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 215
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 216
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 217
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 218
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 219
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 220
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 221
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 222
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 223
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 224
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 225
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 226
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 227
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 228
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 229
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 230
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 231
        EVT_APP_SETTING_BACK_REQUEST                                                , // 232
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 233
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 234
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 235
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 236
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 237
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 238
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 239
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 240
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 241
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 242
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 243
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 244
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 245
        EVT_UTXOS_SORT_REQUEST                                                    , // 246
        EVT_UTXOS_BACK_REQUEST                                                    , // 247
        EVT_UTXOS_CONSOLIDATE_REQUEST                                             , // 248
        EVT_UTXOS_ITEM_SELECTED                                                   , // 249
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 250
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 251
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 252
        EVT_WALLET_INFO_EDIT_NAME                                    , // 253
        EVT_WALLET_INFO_REMOVE                                       , // 254
        EVT_WALLET_INFO_EXPORT_DB                                    , // 255
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 256
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 257
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 258
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 259
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 260
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 261
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 262
        EVT_WALLET_INFO_REFRESH_WALLET_REQUEST                       , // 263
        EVT_WALLET_INFO_GAP_LIMIT_REQUEST                            , // 264
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 265
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 266
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 267
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 268
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 269
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 270
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 271
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 272
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 273
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 274
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 275
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 276
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 277
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 278
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 279
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 280
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 281
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 282
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 283
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 284
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 285
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 286
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 287
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 288
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 289
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 290
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 291
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 292
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 293
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 294
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 295
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 296
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 297
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 298
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 299
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 300
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 301
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 302
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 303
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 304
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 305
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 306
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 307
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 308
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 309
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 310
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 311
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 312
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 313
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 314
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 315
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 316
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 317
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 318
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 319
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 320
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 321
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 322
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 323
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 324
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 325
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 326
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 327
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 328
        EVT_INPUT_PIN_SEND_PIN                                           , // 329
        EVT_INPUT_PIN_CLOSE                                              , // 330
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 331
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 332
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 333
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 334
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 335
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 336
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 337
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 338
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 339
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 340
        EVT_CREATE_NEW_SEED                                                               , // 341
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 342
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 343
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 344
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 345
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 346
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 347
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 348
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 349
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 350
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 351
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 352
        EVT_CREATE_NEW_SEED_BACK                                                           , // 353
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 354
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 355
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 356
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 357
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 358
        EVT_ADD_SOFTWARE_SIGNER_RESULT                                                           , // 359
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 360
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 361
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 362
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 363
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 364
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 365
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 366
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 367
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 368
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 369
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 370
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 371
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 372
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 373
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 374
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 375
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 376
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 377
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 378
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 379
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 380
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 381
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 382
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 383
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 384
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 385
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 386
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 387
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 388
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 389
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 390
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 391
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 392
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 393
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 394
        EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ                                 , // 395
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 396
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 397
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 398
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 399
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 400
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 401
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 402
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 403
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 404
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 405
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 406
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 407
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 408
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 409
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 410
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 411
        EVT_PENDING_REQUEST_IGNORE                                                     , // 412
        EVT_PENDING_REQUEST_BACK                                                       , // 413
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 414
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 415
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 416
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 417
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 418
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 419
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 420
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 421
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 422
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 423
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 424
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 425
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 426
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 427
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 428
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 429
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 430
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 431
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 432
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 433
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 434
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 435
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 436
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 437
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 438
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 439
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 440
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 441
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 442
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 443
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 444
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 445
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 446
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 447
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 448
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 449
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 450
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 451
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 452
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 453
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 454
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 455
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 456
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 457
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 458
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 459
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 460
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 461
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 462
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 463
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 464
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 465
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 466
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 467
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 468
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 469
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 470
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 471
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 472
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 473
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 474
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 475
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 476
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 477
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 478
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 479
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 480
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 481
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 482
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 483
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 484
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 485
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 486
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 487
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 488
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 489
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 490
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 491
        EVT_CHANGE_PASSWORD_BACK                                                  , // 492
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 493
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 494
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 495
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 496
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 497
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 498
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 499
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 500
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 501
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 502
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 503
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 504
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 505
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 506
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 507
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 508
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 509
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 510
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 511
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 512
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 513
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 514
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 515
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 516
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 517
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 518
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 519
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 520
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 521
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 522
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 523
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 524
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 525
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 526
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 527
        EVT_UPDATE_PROFILE_REQUEST                                        , // 528
        EVT_UPDATE_PROFILE_BACK                                           , // 529
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 530
    };
    enum EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS
    {
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MIN=EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX+1, // 531
        EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST                                              , // 532
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX                                      , // 533
    };
    enum EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD
    {
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MIN=EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX+1, // 534
        EVT_INPUT_BACKUP_PASSWORD_REQUEST                                                            , // 535
        EVT_ENTER_BACKUP_PASSWORD_BACK                                                               , // 536
        EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST                                                      , // 537
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX                                                   , // 538
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX+1, // 539
        EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST                                                                  , // 540
        EVT_ANSER_SECURITY_QUESTION_BACK                                                                      , // 541
        EVT_ENTER_BACKUP_PASSWORD_RERQUEST                                                                    , // 542
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX                                             , // 543
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX+1, // 544
        EVT_INPUT_TAPSIGNER_SELECT_REQUEST                                                           , // 545
        EVT_ANSER_SECURITY_QUESTION_REQUEST                                                          , // 546
        EVT_STATE_ID_SCR_KEY_RECOVERY_MAX                                                            , // 547
    };
    enum EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD
    {
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_MAX+1, // 548
        EVT_INPUT_PASSWORD_REQUEST                                                    , // 549
        EVT_REENTER_YOUR_PASSWORD_BACK                                                , // 550
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX                                    , // 551
    };
    enum EVT_STATE_ID_SCR_SERVICE_SETTINGS
    {
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MIN=EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX+1, // 552
        EVT_CLAIM_INHERITANCE_CHECK_REQUEST                                               , // 553
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST                                          , // 554
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED                                          , // 555
        EVT_SERVICE_SELECT_WALLET_REQUEST                                                 , // 556
        EVT_INHERITANCE_PLAN_FINALIZE_REQUEST                                             , // 557
        EVT_SERVICE_SUPPORT_REQUEST                                                       , // 558
        EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST                                          , // 559
        EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST                                            , // 560
        EVT_KEY_RECOVERY_REQUEST                                                          , // 561
        EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST                                           , // 562
        EVT_WALLET_CO_SIGN_POLICE_REQUEST                                                 , // 563
        EVT_LOCKDOWN_SUCCESS_REQUEST                                                      , // 564
        EVT_SETUP_SECURITY_QUESTION_REQUEST                                               , // 565
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX                                             , // 566
    };
    enum EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD
    {
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MIN=EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX+1, // 567
        EVT_INPUT_DAYS_VALUE_REQUEST                                                            , // 568
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST                                            , // 569
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX                                        , // 570
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX+1, // 571
        EVT_INPUT_LOCKDOWN_ANSER_REQUEST                                                                        , // 572
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK                                                               , // 573
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX                                                   , // 574
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS
    {
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MIN=EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX+1, // 575
        EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST                                                           , // 576
        EVT_CLOSE_LOCKDOWN_SUCCESS                                                                   , // 577
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX                                                        , // 578
    };
    enum EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX+1, // 579
        EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST                                         , // 580
        EVT_DUMMY_TRANSACTION_INFO_BACK                                                    , // 581
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX                                        , // 582
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION
    {
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MIN=EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX+1, // 583
        EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ                                                           , // 584
        EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST                                                       , // 585
        EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK                                                          , // 586
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX                                              , // 587
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS
    {
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MIN=EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX+1, // 588
        EVT_INHERITANCE_TRANSACTION_DETAILS_BACK                                                                   , // 589
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX                                                       , // 590
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE
    {
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MIN=EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX+1, // 591
        EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST                                                                 , // 592
        EVT_INHERITANCE_NEW_TRANSACTION_REQ                                                                     , // 593
        EVT_INHERITANCE_WITHDRAW_BALANCE_BACK                                                                   , // 594
        EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST                                                             , // 595
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX                                                       , // 596
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_ASK
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MIN=EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX+1, // 597
        EVT_ADD_HARDWARE_REQUEST                                                                 , // 598
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX                                                    , // 599
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX+1, // 600
        EVT_SCAN_HARDWARE_DEVICE_REQUEST                                         , // 601
        EVT_ADD_HARDWARE_DEVICE_REQUEST                                          , // 602
        EVT_STATE_ID_SCR_ADD_HARDWARE_MAX                                        , // 603
    };
    enum EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE
    {
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_MAX+1, // 604
        EVT_SELECT_WALLET_REQUEST                                                            , // 605
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX                                    , // 606
    };
    enum EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN
    {
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MIN=EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX+1, // 607
        EVT_UPDATE_ACTIVATION_DATE_REQUEST                                                                 , // 608
        EVT_UPDATE_MESSAGE_REQUEST                                                                         , // 609
        EVT_UPDATE_BUFFER_PERIOD_REQUEST                                                                   , // 610
        EVT_UPDATE_NOTIFICATION_PREFERENCE_REQ                                                             , // 611
        EVT_NOT_UPDATE_ANY_NOTI_REQ                                                                        , // 612
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX                                                    , // 613
    };
    enum EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS
    {
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MIN=EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX+1, // 614
        EVT_UPDATE_YOUR_SECRET_REQUEST                                                           , // 615
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX                                                  , // 616
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MIN=EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX+1, // 617
        EVT_ADD_EXIST_HARDWARE_REQUEST                                                   , // 618
        EVT_ADD_NEW_HARDWARE_REQUEST                                                     , // 619
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX                                          , // 620
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX+1, // 621
        EVT_DASHBOARD_ALERT_INFO_ENTER                                                     , // 622
        EVT_ADD_HARDWARE_KEY_EXIST_REQ                                                     , // 623
        EVT_ADD_HARDWARE_KEY_REQUEST                                                       , // 624
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX                                          , // 625
    };
    enum EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS
    {
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX+1, // 626
        EVT_KEY_HEALTH_CHECK_STATUS_ENTER                                                       , // 627
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX                                            , // 628
    };
    enum EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING
    {
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MIN=EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX+1, // 629
        EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST                                                    , // 630
        EVT_HEALTH_CHECK_STARTING_CLOSE                                                          , // 631
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX                                               , // 632
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MIN=EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX+1, // 633
        EVT_DASHBOARD_ALERT_SUCCESS_ENTER_REQUEST                                                , // 634
        EVT_DASHBOARD_ALERT_SUCCESS_CLOSE                                                        , // 635
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX                                             , // 636
    };
    enum EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX+1, // 637
        EVT_INPUT_SECURITY_QUESTION_REQUEST                                                        , // 638
        EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ                                                     , // 639
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX                                               , // 640
    };
    enum EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX+1, // 641
        EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ                                                            , // 642
        EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK                                                           , // 643
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX                                               , // 644
    };
    enum EVT_STATE_ID_SCR_RECURRING_PAYMENTS
    {
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MIN=EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX+1, // 645
        EVT_RECURRING_PAYMENTS_ENTER                                                                 , // 646
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX                                                      , // 647
    };
    enum EVT_STATE_ID_SCR_ONBOARDING
    {
        EVT_STATE_ID_SCR_ONBOARDING_MIN=EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX+1, // 648
        EVT_ONBOARDING_ACTION_REQUEST                                            , // 649
        EVT_ONBOARDING_CLOSE                                                     , // 650
        EVT_STATE_ID_SCR_ONBOARDING_MAX                                          , // 651
    };

};
#endif // VIEWSENUMS_H
