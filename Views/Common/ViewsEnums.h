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
    Q_ENUMS(EVT_STATE_ID_SCR_SELECT_SERVER)
    Q_ENUMS(EVT_STATE_ID_SCR_CHANGE_EMAIL)
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
        STATE_ID_SCR_SELECT_SERVER                       , // 83
        STATE_ID_SCR_CHANGE_EMAIL                        , // 84
        EVT_STATE_ID_REGISTED_MAX                        , // 85
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1, // 86
        EVT_STARTING_APPLICATION_LOCALMODE               , // 87
        EVT_STARTING_APPLICATION_ONLINEMODE              , // 88
        EVT_STARTING_APPLICATION_ONLINE_HWL              , // 89
        EVT_SETTING_ACCOUNT_CHANGE_PASSWORD              , // 90
        EVT_ONS_CLOSE_ALL_REQUEST                        , // 91
        EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST             , // 92
        EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST              , // 93
        EVT_GOTO_HOME_WALLET_TAB                         , // 94
        EVT_ONS_CLOSE_REQUEST                            , // 95
        EVT_LOGIN_DB_REQUEST                             , // 96
        EVT_ROOT_PROMT_PIN                               , // 97
        EVT_ROOT_PROMT_PASSPHRASE                        , // 98
        EVT_LOGIN_MATRIX_REQUEST                         , // 99
        EVT_GOTO_HOME_CHAT_TAB                           , // 100
        EVT_ONLINE_ONS_CLOSE_REQUEST                     , // 101
        EVT_SHOW_TOAST_MESSAGE                           , // 102
        EVT_ROOT_UPDATE_PROFILE                          , // 103
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                  , // 104
        EVT_GOTO_APP_SETTINGS_TAB                        , // 105
        EVT_SIGN_IN_REQUEST                              , // 106
        EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST               , // 107
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST              , // 108
        EVT_GOTO_SERVICE_SETTING_TAB                     , // 109
        EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST            , // 110
        EVT_NUNCHUK_LOGIN_SUCCEEDED                      , // 111
        EVT_HEALTH_CHECK_STARTING_REQUEST                , // 112
        EVT_DUMMY_TRANSACTION_INFO_REQUEST               , // 113
        EVT_DASHBOARD_ALERT_SUCCESS_REQUEST              , // 114
        EVT_SHARE_YOUR_SECRET_REQUEST                    , // 115
        EVT_REENTER_YOUR_PASSWORD_REQUEST                , // 116
        EVT_ONBOARDING_REQUEST                           , // 117
        EVT_STATE_ID_ROOT_MAX                            , // 118
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 119
        EVT_HOME_WALLET_SELECTED                         , // 120
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 121
        EVT_HOME_SETTING_REQUEST                         , // 122
        EVT_HOME_DISPLAY_ADDRESS                         , // 123
        EVT_HOME_IMPORT_PSBT                             , // 124
        EVT_HOME_EXPORT_BSMS                             , // 125
        EVT_HOME_ADD_WALLET_REQUEST                      , // 126
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 127
        EVT_HOME_SEND_REQUEST                            , // 128
        EVT_HOME_RECEIVE_REQUEST                         , // 129
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 130
        EVT_HOME_WALLET_INFO_REQUEST                     , // 131
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 132
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 133
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 134
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 135
        EVT_ASK_HARDWARE_REQ                             , // 136
        EVT_EXIST_HARDWARE_REQ                           , // 137
        EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST             , // 138
        EVT_KEY_HEALTH_CHECK_STATUS_REQUEST              , // 139
        EVT_RECURRING_PAYMENTS_REQUEST                   , // 140
        EVT_STATE_ID_SCR_HOME_MAX                        , // 141
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 142
        EVT_ADD_WALLET_IMPORT                                      , // 143
        EVT_ADD_WALLET_BACK_REQUEST                                , // 144
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 145
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 146
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 147
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 148
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 149
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 150
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 151
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 152
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 153
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 154
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 155
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 156
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 157
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 158
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 159
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 160
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 161
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 162
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 163
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 164
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 165
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 166
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 167
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 168
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 169
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 170
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 171
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 172
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 173
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 174
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 175
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 176
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 177
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 178
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 179
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 180
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 181
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 182
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 183
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 184
        EVT_RECEIVE_BACK_REQUEST                                                           , // 185
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 186
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 187
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 188
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 189
        EVT_SEND_BACK_REQUEST                                   , // 190
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 191
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 192
        EVT_STATE_ID_SCR_SEND_MAX                               , // 193
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 194
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 195
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 196
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 197
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 198
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 199
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 200
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 201
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 202
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 203
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 204
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 205
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 206
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 207
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 208
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 209
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 210
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 211
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 212
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 213
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 214
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 215
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 216
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 217
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 218
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 219
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 220
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 221
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 222
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 223
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 224
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 225
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 226
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 227
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 228
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 229
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 230
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 231
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 232
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 233
        EVT_APP_SETTING_BACK_REQUEST                                                , // 234
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 235
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 236
        EVT_SELECT_SERVER_REQUEST                                                   , // 237
        EVT_CHANGE_EMAIL_REQUEST                                                    , // 238
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 239
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 240
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 241
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 242
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 243
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 244
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 245
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 246
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 247
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 248
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 249
        EVT_UTXOS_SORT_REQUEST                                                    , // 250
        EVT_UTXOS_BACK_REQUEST                                                    , // 251
        EVT_UTXOS_CONSOLIDATE_REQUEST                                             , // 252
        EVT_UTXOS_ITEM_SELECTED                                                   , // 253
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 254
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 255
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 256
        EVT_WALLET_INFO_EDIT_NAME                                    , // 257
        EVT_WALLET_INFO_REMOVE                                       , // 258
        EVT_WALLET_INFO_EXPORT_DB                                    , // 259
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 260
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 261
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 262
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 263
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 264
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 265
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 266
        EVT_WALLET_INFO_REFRESH_WALLET_REQUEST                       , // 267
        EVT_WALLET_INFO_GAP_LIMIT_REQUEST                            , // 268
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 269
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 270
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 271
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 272
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 273
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 274
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 275
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 276
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 277
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 278
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 279
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 280
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 281
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 282
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 283
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 284
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 285
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 286
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 287
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 288
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 289
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 290
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 291
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 292
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 293
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 294
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 295
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 296
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 297
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 298
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 299
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 300
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 301
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 302
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 303
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 304
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 305
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 306
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 307
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 308
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 309
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 310
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 311
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 312
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 313
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 314
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 315
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 316
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 317
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 318
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 319
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 320
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 321
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 322
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 323
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 324
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 325
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 326
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 327
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 328
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 329
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 330
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 331
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 332
        EVT_INPUT_PIN_SEND_PIN                                           , // 333
        EVT_INPUT_PIN_CLOSE                                              , // 334
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 335
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 336
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 337
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 338
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 339
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 340
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 341
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 342
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 343
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 344
        EVT_CREATE_NEW_SEED                                                               , // 345
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 346
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 347
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 348
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 349
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 350
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 351
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 352
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 353
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 354
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 355
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 356
        EVT_CREATE_NEW_SEED_BACK                                                           , // 357
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 358
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 359
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 360
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 361
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 362
        EVT_ADD_SOFTWARE_SIGNER_RESULT                                                           , // 363
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 364
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 365
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 366
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 367
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 368
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 369
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 370
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 371
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 372
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 373
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 374
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 375
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 376
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 377
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 378
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 379
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 380
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 381
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 382
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 383
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 384
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 385
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 386
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 387
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 388
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 389
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 390
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 391
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 392
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 393
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 394
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 395
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 396
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 397
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 398
        EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ                                 , // 399
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 400
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 401
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 402
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 403
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 404
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 405
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 406
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 407
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 408
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 409
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 410
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 411
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 412
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 413
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 414
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 415
        EVT_PENDING_REQUEST_IGNORE                                                     , // 416
        EVT_PENDING_REQUEST_BACK                                                       , // 417
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 418
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 419
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 420
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 421
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 422
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 423
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 424
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 425
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 426
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 427
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 428
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 429
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 430
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 431
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 432
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 433
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 434
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 435
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 436
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 437
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 438
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 439
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 440
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 441
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 442
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 443
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 444
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 445
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 446
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 447
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 448
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 449
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 450
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 451
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 452
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 453
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 454
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 455
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 456
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 457
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 458
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 459
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 460
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 461
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 462
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 463
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 464
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 465
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 466
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 467
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 468
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 469
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 470
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 471
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 472
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 473
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 474
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 475
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 476
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 477
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 478
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 479
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 480
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 481
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 482
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 483
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 484
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 485
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 486
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 487
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 488
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 489
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 490
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 491
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 492
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 493
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 494
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 495
        EVT_CHANGE_PASSWORD_BACK                                                  , // 496
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 497
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 498
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 499
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 500
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 501
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 502
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 503
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 504
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 505
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 506
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 507
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 508
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 509
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 510
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 511
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 512
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 513
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 514
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 515
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 516
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 517
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 518
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 519
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 520
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 521
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 522
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 523
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 524
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 525
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 526
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 527
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 528
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 529
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 530
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 531
        EVT_UPDATE_PROFILE_REQUEST                                        , // 532
        EVT_UPDATE_PROFILE_BACK                                           , // 533
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 534
    };
    enum EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS
    {
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MIN=EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX+1, // 535
        EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST                                              , // 536
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX                                      , // 537
    };
    enum EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD
    {
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MIN=EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX+1, // 538
        EVT_INPUT_BACKUP_PASSWORD_REQUEST                                                            , // 539
        EVT_ENTER_BACKUP_PASSWORD_BACK                                                               , // 540
        EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST                                                      , // 541
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX                                                   , // 542
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX+1, // 543
        EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST                                                                  , // 544
        EVT_ANSER_SECURITY_QUESTION_BACK                                                                      , // 545
        EVT_ENTER_BACKUP_PASSWORD_RERQUEST                                                                    , // 546
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX                                             , // 547
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX+1, // 548
        EVT_INPUT_TAPSIGNER_SELECT_REQUEST                                                           , // 549
        EVT_ANSER_SECURITY_QUESTION_REQUEST                                                          , // 550
        EVT_STATE_ID_SCR_KEY_RECOVERY_MAX                                                            , // 551
    };
    enum EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD
    {
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_MAX+1, // 552
        EVT_INPUT_PASSWORD_REQUEST                                                    , // 553
        EVT_REENTER_YOUR_PASSWORD_BACK                                                , // 554
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX                                    , // 555
    };
    enum EVT_STATE_ID_SCR_SERVICE_SETTINGS
    {
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MIN=EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX+1, // 556
        EVT_CLAIM_INHERITANCE_CHECK_REQUEST                                               , // 557
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST                                          , // 558
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED                                          , // 559
        EVT_SERVICE_SELECT_WALLET_REQUEST                                                 , // 560
        EVT_INHERITANCE_PLAN_FINALIZE_REQUEST                                             , // 561
        EVT_SERVICE_SUPPORT_REQUEST                                                       , // 562
        EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST                                          , // 563
        EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST                                            , // 564
        EVT_KEY_RECOVERY_REQUEST                                                          , // 565
        EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST                                           , // 566
        EVT_WALLET_CO_SIGN_POLICE_REQUEST                                                 , // 567
        EVT_LOCKDOWN_SUCCESS_REQUEST                                                      , // 568
        EVT_SETUP_SECURITY_QUESTION_REQUEST                                               , // 569
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX                                             , // 570
    };
    enum EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD
    {
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MIN=EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX+1, // 571
        EVT_INPUT_DAYS_VALUE_REQUEST                                                            , // 572
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST                                            , // 573
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX                                        , // 574
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX+1, // 575
        EVT_INPUT_LOCKDOWN_ANSER_REQUEST                                                                        , // 576
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK                                                               , // 577
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX                                                   , // 578
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS
    {
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MIN=EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX+1, // 579
        EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST                                                           , // 580
        EVT_CLOSE_LOCKDOWN_SUCCESS                                                                   , // 581
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX                                                        , // 582
    };
    enum EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX+1, // 583
        EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST                                         , // 584
        EVT_DUMMY_TRANSACTION_INFO_BACK                                                    , // 585
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX                                        , // 586
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION
    {
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MIN=EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX+1, // 587
        EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ                                                           , // 588
        EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST                                                       , // 589
        EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK                                                          , // 590
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX                                              , // 591
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS
    {
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MIN=EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX+1, // 592
        EVT_INHERITANCE_TRANSACTION_DETAILS_BACK                                                                   , // 593
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX                                                       , // 594
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE
    {
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MIN=EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX+1, // 595
        EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST                                                                 , // 596
        EVT_INHERITANCE_NEW_TRANSACTION_REQ                                                                     , // 597
        EVT_INHERITANCE_WITHDRAW_BALANCE_BACK                                                                   , // 598
        EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST                                                             , // 599
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX                                                       , // 600
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_ASK
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MIN=EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX+1, // 601
        EVT_ADD_HARDWARE_REQUEST                                                                 , // 602
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX                                                    , // 603
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX+1, // 604
        EVT_SCAN_HARDWARE_DEVICE_REQUEST                                         , // 605
        EVT_ADD_HARDWARE_DEVICE_REQUEST                                          , // 606
        EVT_STATE_ID_SCR_ADD_HARDWARE_MAX                                        , // 607
    };
    enum EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE
    {
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_MAX+1, // 608
        EVT_SELECT_WALLET_REQUEST                                                            , // 609
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX                                    , // 610
    };
    enum EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN
    {
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MIN=EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX+1, // 611
        EVT_UPDATE_ACTIVATION_DATE_REQUEST                                                                 , // 612
        EVT_UPDATE_MESSAGE_REQUEST                                                                         , // 613
        EVT_UPDATE_BUFFER_PERIOD_REQUEST                                                                   , // 614
        EVT_UPDATE_NOTIFICATION_PREFERENCE_REQ                                                             , // 615
        EVT_NOT_UPDATE_ANY_NOTI_REQ                                                                        , // 616
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX                                                    , // 617
    };
    enum EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS
    {
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MIN=EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX+1, // 618
        EVT_UPDATE_YOUR_SECRET_REQUEST                                                           , // 619
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX                                                  , // 620
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MIN=EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX+1, // 621
        EVT_ADD_EXIST_HARDWARE_REQUEST                                                   , // 622
        EVT_ADD_NEW_HARDWARE_REQUEST                                                     , // 623
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX                                          , // 624
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX+1, // 625
        EVT_DASHBOARD_ALERT_INFO_ENTER                                                     , // 626
        EVT_ADD_HARDWARE_KEY_EXIST_REQ                                                     , // 627
        EVT_ADD_HARDWARE_KEY_REQUEST                                                       , // 628
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX                                          , // 629
    };
    enum EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS
    {
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX+1, // 630
        EVT_KEY_HEALTH_CHECK_STATUS_ENTER                                                       , // 631
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX                                            , // 632
    };
    enum EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING
    {
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MIN=EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX+1, // 633
        EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST                                                    , // 634
        EVT_HEALTH_CHECK_STARTING_CLOSE                                                          , // 635
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX                                               , // 636
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MIN=EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX+1, // 637
        EVT_DASHBOARD_ALERT_SUCCESS_ENTER_REQUEST                                                , // 638
        EVT_DASHBOARD_ALERT_SUCCESS_CLOSE                                                        , // 639
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX                                             , // 640
    };
    enum EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX+1, // 641
        EVT_INPUT_SECURITY_QUESTION_REQUEST                                                        , // 642
        EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ                                                     , // 643
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX                                               , // 644
    };
    enum EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX+1, // 645
        EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ                                                            , // 646
        EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK                                                           , // 647
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX                                               , // 648
    };
    enum EVT_STATE_ID_SCR_RECURRING_PAYMENTS
    {
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MIN=EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX+1, // 649
        EVT_RECURRING_PAYMENTS_ENTER                                                                 , // 650
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX                                                      , // 651
    };
    enum EVT_STATE_ID_SCR_ONBOARDING
    {
        EVT_STATE_ID_SCR_ONBOARDING_MIN=EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX+1, // 652
        EVT_ONBOARDING_ACTION_REQUEST                                            , // 653
        EVT_ONBOARDING_CLOSE                                                     , // 654
        EVT_STATE_ID_SCR_ONBOARDING_MAX                                          , // 655
    };
    enum EVT_STATE_ID_SCR_SELECT_SERVER
    {
        EVT_STATE_ID_SCR_SELECT_SERVER_MIN=EVT_STATE_ID_SCR_ONBOARDING_MAX+1, // 656
        EVT_SELECT_SERVER_ENTER                                             , // 657
        EVT_STATE_ID_SCR_SELECT_SERVER_MAX                                  , // 658
    };
    enum EVT_STATE_ID_SCR_CHANGE_EMAIL
    {
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MIN=EVT_STATE_ID_SCR_SELECT_SERVER_MAX+1, // 659
        EVT_CHANGE_EMAIL_ENTER                                                , // 660
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MAX                                     , // 661
    };

};
#endif // VIEWSENUMS_H
