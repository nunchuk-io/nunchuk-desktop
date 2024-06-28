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
    Q_ENUMS(EVT_STATE_ID_SCR_REPLACE_SELECT_KEY)
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
        STATE_ID_SCR_REPLACE_SELECT_KEY                  , // 85
        EVT_STATE_ID_REGISTED_MAX                        , // 86
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1, // 87
        EVT_STARTING_APPLICATION_LOCALMODE               , // 88
        EVT_STARTING_APPLICATION_ONLINEMODE              , // 89
        EVT_STARTING_APPLICATION_ONLINE_HWL              , // 90
        EVT_SETTING_ACCOUNT_CHANGE_PASSWORD              , // 91
        EVT_ONS_CLOSE_ALL_REQUEST                        , // 92
        EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST             , // 93
        EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST              , // 94
        EVT_GOTO_HOME_WALLET_TAB                         , // 95
        EVT_ONS_CLOSE_REQUEST                            , // 96
        EVT_LOGIN_DB_REQUEST                             , // 97
        EVT_ROOT_PROMT_PIN                               , // 98
        EVT_ROOT_PROMT_PASSPHRASE                        , // 99
        EVT_LOGIN_MATRIX_REQUEST                         , // 100
        EVT_GOTO_HOME_CHAT_TAB                           , // 101
        EVT_ONLINE_ONS_CLOSE_REQUEST                     , // 102
        EVT_SHOW_TOAST_MESSAGE                           , // 103
        EVT_ROOT_UPDATE_PROFILE                          , // 104
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                  , // 105
        EVT_GOTO_APP_SETTINGS_TAB                        , // 106
        EVT_SIGN_IN_REQUEST                              , // 107
        EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST               , // 108
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST              , // 109
        EVT_GOTO_SERVICE_SETTING_TAB                     , // 110
        EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST            , // 111
        EVT_NUNCHUK_LOGIN_SUCCEEDED                      , // 112
        EVT_HEALTH_CHECK_STARTING_REQUEST                , // 113
        EVT_DUMMY_TRANSACTION_INFO_REQUEST               , // 114
        EVT_DASHBOARD_ALERT_SUCCESS_REQUEST              , // 115
        EVT_SHARE_YOUR_SECRET_REQUEST                    , // 116
        EVT_REENTER_YOUR_PASSWORD_REQUEST                , // 117
        EVT_ONBOARDING_REQUEST                           , // 118
        EVT_REPLACE_SELECT_KEY_REQUEST                   , // 119
        EVT_STATE_ID_ROOT_MAX                            , // 120
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 121
        EVT_HOME_WALLET_SELECTED                         , // 122
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 123
        EVT_HOME_SETTING_REQUEST                         , // 124
        EVT_HOME_DISPLAY_ADDRESS                         , // 125
        EVT_HOME_IMPORT_PSBT                             , // 126
        EVT_HOME_EXPORT_BSMS                             , // 127
        EVT_HOME_ADD_WALLET_REQUEST                      , // 128
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 129
        EVT_HOME_SEND_REQUEST                            , // 130
        EVT_HOME_RECEIVE_REQUEST                         , // 131
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 132
        EVT_HOME_WALLET_INFO_REQUEST                     , // 133
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 134
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 135
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 136
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 137
        EVT_ASK_HARDWARE_REQ                             , // 138
        EVT_EXIST_HARDWARE_REQ                           , // 139
        EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST             , // 140
        EVT_KEY_HEALTH_CHECK_STATUS_REQUEST              , // 141
        EVT_RECURRING_PAYMENTS_REQUEST                   , // 142
        EVT_ADD_HARDWARE_REQUEST                         , // 143
        EVT_ADD_SOFTWARE_SIGNER_RESULT                   , // 144
        EVT_STATE_ID_SCR_HOME_MAX                        , // 145
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 146
        EVT_ADD_WALLET_IMPORT                                      , // 147
        EVT_ADD_WALLET_BACK_REQUEST                                , // 148
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 149
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 150
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 151
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 152
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 153
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 154
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 155
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 156
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 157
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 158
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 159
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 160
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 161
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 162
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 163
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 164
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 165
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 166
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 167
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 168
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 169
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 170
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 171
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 172
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 173
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 174
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 175
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 176
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 177
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 178
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 179
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 180
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 181
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 182
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 183
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 184
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 185
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 186
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 187
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 188
        EVT_RECEIVE_BACK_REQUEST                                                           , // 189
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 190
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 191
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 192
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 193
        EVT_SEND_BACK_REQUEST                                   , // 194
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 195
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 196
        EVT_STATE_ID_SCR_SEND_MAX                               , // 197
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 198
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 199
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 200
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 201
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 202
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 203
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 204
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 205
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 206
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 207
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 208
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 209
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 210
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 211
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 212
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 213
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 214
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 215
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 216
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 217
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 218
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 219
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 220
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 221
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 222
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 223
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 224
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 225
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 226
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 227
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 228
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 229
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 230
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 231
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 232
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 233
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 234
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 235
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 236
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 237
        EVT_APP_SETTING_BACK_REQUEST                                                , // 238
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 239
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 240
        EVT_SELECT_SERVER_REQUEST                                                   , // 241
        EVT_CHANGE_EMAIL_REQUEST                                                    , // 242
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 243
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 244
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 245
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 246
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 247
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 248
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 249
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 250
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 251
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 252
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 253
        EVT_UTXOS_SORT_REQUEST                                                    , // 254
        EVT_UTXOS_BACK_REQUEST                                                    , // 255
        EVT_UTXOS_CONSOLIDATE_REQUEST                                             , // 256
        EVT_UTXOS_ITEM_SELECTED                                                   , // 257
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 258
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 259
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 260
        EVT_WALLET_INFO_EDIT_NAME                                    , // 261
        EVT_WALLET_INFO_REMOVE                                       , // 262
        EVT_WALLET_INFO_EXPORT_DB                                    , // 263
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 264
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 265
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 266
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 267
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 268
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 269
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 270
        EVT_WALLET_INFO_REFRESH_WALLET_REQUEST                       , // 271
        EVT_WALLET_INFO_GAP_LIMIT_REQUEST                            , // 272
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 273
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 274
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 275
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 276
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 277
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 278
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 279
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 280
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 281
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 282
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 283
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 284
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 285
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 286
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 287
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 288
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 289
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 290
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 291
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 292
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 293
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 294
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 295
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 296
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 297
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 298
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 299
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 300
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 301
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 302
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 303
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 304
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 305
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 306
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 307
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 308
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 309
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 310
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 311
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 312
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 313
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 314
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 315
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 316
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 317
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 318
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 319
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 320
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 321
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 322
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 323
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 324
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 325
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 326
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 327
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 328
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 329
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 330
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 331
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 332
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 333
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 334
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 335
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 336
        EVT_INPUT_PIN_SEND_PIN                                           , // 337
        EVT_INPUT_PIN_CLOSE                                              , // 338
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 339
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 340
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 341
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 342
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 343
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 344
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 345
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 346
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 347
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 348
        EVT_CREATE_NEW_SEED                                                               , // 349
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 350
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 351
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 352
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 353
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 354
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 355
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 356
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 357
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 358
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 359
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 360
        EVT_CREATE_NEW_SEED_BACK                                                           , // 361
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 362
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 363
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 364
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 365
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 366
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 367
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 368
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 369
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 370
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 371
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 372
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 373
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 374
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 375
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 376
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 377
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 378
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 379
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 380
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 381
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 382
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 383
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 384
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 385
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 386
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 387
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 388
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 389
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 390
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 391
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 392
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 393
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 394
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 395
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 396
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 397
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 398
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 399
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 400
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 401
        EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ                                 , // 402
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 403
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 404
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 405
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 406
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 407
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 408
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 409
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 410
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 411
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 412
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 413
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 414
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 415
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 416
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 417
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 418
        EVT_PENDING_REQUEST_IGNORE                                                     , // 419
        EVT_PENDING_REQUEST_BACK                                                       , // 420
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 421
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 422
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 423
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 424
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 425
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 426
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 427
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 428
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 429
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 430
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 431
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 432
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 433
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 434
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 435
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 436
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 437
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 438
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 439
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 440
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 441
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 442
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 443
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 444
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 445
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 446
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 447
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 448
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 449
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 450
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 451
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 452
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 453
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 454
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 455
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 456
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 457
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 458
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 459
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 460
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 461
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 462
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 463
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 464
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 465
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 466
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 467
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 468
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 469
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 470
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 471
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 472
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 473
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 474
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 475
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 476
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 477
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 478
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 479
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 480
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 481
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 482
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 483
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 484
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 485
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 486
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 487
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 488
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 489
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 490
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 491
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 492
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 493
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 494
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 495
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 496
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 497
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 498
        EVT_CHANGE_PASSWORD_BACK                                                  , // 499
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 500
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 501
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 502
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 503
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 504
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 505
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 506
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 507
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 508
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 509
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 510
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 511
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 512
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 513
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 514
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 515
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 516
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 517
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 518
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 519
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 520
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 521
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 522
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 523
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 524
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 525
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 526
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 527
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 528
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 529
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 530
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 531
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 532
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 533
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 534
        EVT_UPDATE_PROFILE_REQUEST                                        , // 535
        EVT_UPDATE_PROFILE_BACK                                           , // 536
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 537
    };
    enum EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS
    {
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MIN=EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX+1, // 538
        EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST                                              , // 539
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX                                      , // 540
    };
    enum EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD
    {
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MIN=EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX+1, // 541
        EVT_INPUT_BACKUP_PASSWORD_REQUEST                                                            , // 542
        EVT_ENTER_BACKUP_PASSWORD_BACK                                                               , // 543
        EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST                                                      , // 544
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX                                                   , // 545
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX+1, // 546
        EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST                                                                  , // 547
        EVT_ANSER_SECURITY_QUESTION_BACK                                                                      , // 548
        EVT_ENTER_BACKUP_PASSWORD_RERQUEST                                                                    , // 549
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX                                             , // 550
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX+1, // 551
        EVT_INPUT_TAPSIGNER_SELECT_REQUEST                                                           , // 552
        EVT_ANSER_SECURITY_QUESTION_REQUEST                                                          , // 553
        EVT_STATE_ID_SCR_KEY_RECOVERY_MAX                                                            , // 554
    };
    enum EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD
    {
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_MAX+1, // 555
        EVT_INPUT_PASSWORD_REQUEST                                                    , // 556
        EVT_REENTER_YOUR_PASSWORD_BACK                                                , // 557
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX                                    , // 558
    };
    enum EVT_STATE_ID_SCR_SERVICE_SETTINGS
    {
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MIN=EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX+1, // 559
        EVT_CLAIM_INHERITANCE_CHECK_REQUEST                                               , // 560
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST                                          , // 561
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED                                          , // 562
        EVT_SERVICE_SELECT_WALLET_REQUEST                                                 , // 563
        EVT_INHERITANCE_PLAN_FINALIZE_REQUEST                                             , // 564
        EVT_SERVICE_SUPPORT_REQUEST                                                       , // 565
        EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST                                          , // 566
        EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST                                            , // 567
        EVT_KEY_RECOVERY_REQUEST                                                          , // 568
        EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST                                           , // 569
        EVT_WALLET_CO_SIGN_POLICE_REQUEST                                                 , // 570
        EVT_LOCKDOWN_SUCCESS_REQUEST                                                      , // 571
        EVT_SETUP_SECURITY_QUESTION_REQUEST                                               , // 572
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX                                             , // 573
    };
    enum EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD
    {
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MIN=EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX+1, // 574
        EVT_INPUT_DAYS_VALUE_REQUEST                                                            , // 575
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST                                            , // 576
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX                                        , // 577
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX+1, // 578
        EVT_INPUT_LOCKDOWN_ANSER_REQUEST                                                                        , // 579
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK                                                               , // 580
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX                                                   , // 581
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS
    {
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MIN=EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX+1, // 582
        EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST                                                           , // 583
        EVT_CLOSE_LOCKDOWN_SUCCESS                                                                   , // 584
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX                                                        , // 585
    };
    enum EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX+1, // 586
        EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST                                         , // 587
        EVT_DUMMY_TRANSACTION_INFO_BACK                                                    , // 588
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX                                        , // 589
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION
    {
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MIN=EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX+1, // 590
        EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ                                                           , // 591
        EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST                                                       , // 592
        EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK                                                          , // 593
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX                                              , // 594
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS
    {
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MIN=EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX+1, // 595
        EVT_INHERITANCE_TRANSACTION_DETAILS_BACK                                                                   , // 596
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX                                                       , // 597
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE
    {
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MIN=EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX+1, // 598
        EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST                                                                 , // 599
        EVT_INHERITANCE_NEW_TRANSACTION_REQ                                                                     , // 600
        EVT_INHERITANCE_WITHDRAW_BALANCE_BACK                                                                   , // 601
        EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST                                                             , // 602
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX                                                       , // 603
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_ASK
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MIN=EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX+1, // 604
        EVT_ADD_HARDWARE_ASK_ENTER                                                               , // 605
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX                                                    , // 606
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX+1, // 607
        EVT_SCAN_HARDWARE_DEVICE_REQUEST                                         , // 608
        EVT_ADD_HARDWARE_DEVICE_REQUEST                                          , // 609
        EVT_STATE_ID_SCR_ADD_HARDWARE_MAX                                        , // 610
    };
    enum EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE
    {
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_MAX+1, // 611
        EVT_SELECT_WALLET_REQUEST                                                            , // 612
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX                                    , // 613
    };
    enum EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN
    {
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MIN=EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX+1, // 614
        EVT_UPDATE_ACTIVATION_DATE_REQUEST                                                                 , // 615
        EVT_UPDATE_MESSAGE_REQUEST                                                                         , // 616
        EVT_UPDATE_BUFFER_PERIOD_REQUEST                                                                   , // 617
        EVT_UPDATE_NOTIFICATION_PREFERENCE_REQ                                                             , // 618
        EVT_NOT_UPDATE_ANY_NOTI_REQ                                                                        , // 619
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX                                                    , // 620
    };
    enum EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS
    {
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MIN=EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX+1, // 621
        EVT_UPDATE_YOUR_SECRET_REQUEST                                                           , // 622
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX                                                  , // 623
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MIN=EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX+1, // 624
        EVT_ADD_EXIST_HARDWARE_REQUEST                                                   , // 625
        EVT_ADD_NEW_HARDWARE_REQUEST                                                     , // 626
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX                                          , // 627
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX+1, // 628
        EVT_DASHBOARD_ALERT_INFO_ENTER                                                     , // 629
        EVT_ADD_HARDWARE_KEY_EXIST_REQ                                                     , // 630
        EVT_ADD_HARDWARE_KEY_REQUEST                                                       , // 631
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX                                          , // 632
    };
    enum EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS
    {
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX+1, // 633
        EVT_KEY_HEALTH_CHECK_STATUS_ENTER                                                       , // 634
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX                                            , // 635
    };
    enum EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING
    {
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MIN=EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX+1, // 636
        EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST                                                    , // 637
        EVT_HEALTH_CHECK_STARTING_CLOSE                                                          , // 638
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX                                               , // 639
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MIN=EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX+1, // 640
        EVT_DASHBOARD_ALERT_SUCCESS_ENTER_REQUEST                                                , // 641
        EVT_DASHBOARD_ALERT_SUCCESS_CLOSE                                                        , // 642
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX                                             , // 643
    };
    enum EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX+1, // 644
        EVT_INPUT_SECURITY_QUESTION_REQUEST                                                        , // 645
        EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ                                                     , // 646
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX                                               , // 647
    };
    enum EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX+1, // 648
        EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ                                                            , // 649
        EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK                                                           , // 650
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX                                               , // 651
    };
    enum EVT_STATE_ID_SCR_RECURRING_PAYMENTS
    {
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MIN=EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX+1, // 652
        EVT_RECURRING_PAYMENTS_ENTER                                                                 , // 653
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX                                                      , // 654
    };
    enum EVT_STATE_ID_SCR_ONBOARDING
    {
        EVT_STATE_ID_SCR_ONBOARDING_MIN=EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX+1, // 655
        EVT_ONBOARDING_ACTION_REQUEST                                            , // 656
        EVT_ONBOARDING_CLOSE                                                     , // 657
        EVT_STATE_ID_SCR_ONBOARDING_MAX                                          , // 658
    };
    enum EVT_STATE_ID_SCR_SELECT_SERVER
    {
        EVT_STATE_ID_SCR_SELECT_SERVER_MIN=EVT_STATE_ID_SCR_ONBOARDING_MAX+1, // 659
        EVT_SELECT_SERVER_ENTER                                             , // 660
        EVT_STATE_ID_SCR_SELECT_SERVER_MAX                                  , // 661
    };
    enum EVT_STATE_ID_SCR_CHANGE_EMAIL
    {
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MIN=EVT_STATE_ID_SCR_SELECT_SERVER_MAX+1, // 662
        EVT_CHANGE_EMAIL_ENTER                                                , // 663
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MAX                                     , // 664
    };
    enum EVT_STATE_ID_SCR_REPLACE_SELECT_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_EMAIL_MAX+1, // 665
        EVT_REPLACE_SELECT_KEY_ENTER                                               , // 666
        EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MAX                                    , // 667
    };

};
#endif // VIEWSENUMS_H
