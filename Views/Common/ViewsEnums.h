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
    Q_ENUMS(EVT_STATE_ID_SCR_REPLACE_KEYS)
    Q_ENUMS(EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB)
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
        STATE_ID_SCR_REPLACE_KEYS                        , // 86
        STATE_ID_SCR_SIGN_IN_VIA_XPUB                    , // 87
        EVT_STATE_ID_REGISTED_MAX                        , // 88
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1, // 89
        EVT_STARTING_APPLICATION_LOCALMODE               , // 90
        EVT_STARTING_APPLICATION_ONLINEMODE              , // 91
        EVT_STARTING_APPLICATION_ONLINE_HWL              , // 92
        EVT_SETTING_ACCOUNT_CHANGE_PASSWORD              , // 93
        EVT_ONS_CLOSE_ALL_REQUEST                        , // 94
        EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST             , // 95
        EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST              , // 96
        EVT_GOTO_HOME_WALLET_TAB                         , // 97
        EVT_ONS_CLOSE_REQUEST                            , // 98
        EVT_LOGIN_DB_REQUEST                             , // 99
        EVT_ROOT_PROMT_PIN                               , // 100
        EVT_ROOT_PROMT_PASSPHRASE                        , // 101
        EVT_LOGIN_MATRIX_REQUEST                         , // 102
        EVT_GOTO_HOME_CHAT_TAB                           , // 103
        EVT_ONLINE_ONS_CLOSE_REQUEST                     , // 104
        EVT_SHOW_TOAST_MESSAGE                           , // 105
        EVT_ROOT_UPDATE_PROFILE                          , // 106
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                  , // 107
        EVT_GOTO_APP_SETTINGS_TAB                        , // 108
        EVT_SIGN_IN_REQUEST                              , // 109
        EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST               , // 110
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST              , // 111
        EVT_GOTO_SERVICE_SETTING_TAB                     , // 112
        EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST            , // 113
        EVT_NUNCHUK_LOGIN_SUCCEEDED                      , // 114
        EVT_HEALTH_CHECK_STARTING_REQUEST                , // 115
        EVT_DUMMY_TRANSACTION_INFO_REQUEST               , // 116
        EVT_DASHBOARD_ALERT_SUCCESS_REQUEST              , // 117
        EVT_SHARE_YOUR_SECRET_REQUEST                    , // 118
        EVT_REENTER_YOUR_PASSWORD_REQUEST                , // 119
        EVT_ONBOARDING_REQUEST                           , // 120
        EVT_REPLACE_SELECT_KEY_REQUEST                   , // 121
        EVT_SIGN_IN_VIA_XPUB_REQUEST                     , // 122
        EVT_STATE_ID_ROOT_MAX                            , // 123
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 124
        EVT_HOME_WALLET_SELECTED                         , // 125
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 126
        EVT_HOME_SETTING_REQUEST                         , // 127
        EVT_HOME_DISPLAY_ADDRESS                         , // 128
        EVT_HOME_IMPORT_PSBT                             , // 129
        EVT_HOME_EXPORT_BSMS                             , // 130
        EVT_HOME_ADD_WALLET_REQUEST                      , // 131
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 132
        EVT_HOME_SEND_REQUEST                            , // 133
        EVT_HOME_RECEIVE_REQUEST                         , // 134
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 135
        EVT_HOME_WALLET_INFO_REQUEST                     , // 136
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 137
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 138
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 139
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 140
        EVT_ASK_HARDWARE_REQ                             , // 141
        EVT_EXIST_HARDWARE_REQ                           , // 142
        EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST             , // 143
        EVT_KEY_HEALTH_CHECK_STATUS_REQUEST              , // 144
        EVT_RECURRING_PAYMENTS_REQUEST                   , // 145
        EVT_ADD_HARDWARE_REQUEST                         , // 146
        EVT_ADD_SOFTWARE_SIGNER_RESULT                   , // 147
        EVT_REPLACE_KEYS_REQUEST                         , // 148
        EVT_STATE_ID_SCR_HOME_MAX                        , // 149
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 150
        EVT_ADD_WALLET_IMPORT                                      , // 151
        EVT_ADD_WALLET_BACK_REQUEST                                , // 152
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 153
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 154
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 155
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 156
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 157
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 158
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 159
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 160
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 161
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 162
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 163
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 164
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 165
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 166
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 167
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 168
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 169
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 170
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 171
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 172
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 173
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 174
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 175
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 176
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 177
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 178
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 179
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 180
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 181
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 182
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 183
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 184
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 185
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 186
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 187
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 188
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 189
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 190
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 191
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 192
        EVT_RECEIVE_BACK_REQUEST                                                           , // 193
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 194
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 195
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 196
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 197
        EVT_SEND_BACK_REQUEST                                   , // 198
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 199
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 200
        EVT_STATE_ID_SCR_SEND_MAX                               , // 201
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 202
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 203
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 204
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 205
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 206
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 207
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 208
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 209
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 210
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 211
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 212
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 213
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 214
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 215
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 216
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 217
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 218
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 219
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 220
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 221
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 222
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 223
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 224
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 225
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 226
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 227
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 228
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 229
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 230
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 231
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 232
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 233
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 234
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 235
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 236
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 237
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 238
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 239
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 240
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 241
        EVT_APP_SETTING_BACK_REQUEST                                                , // 242
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 243
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 244
        EVT_SELECT_SERVER_REQUEST                                                   , // 245
        EVT_CHANGE_EMAIL_REQUEST                                                    , // 246
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 247
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 248
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 249
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 250
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 251
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 252
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 253
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 254
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 255
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 256
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 257
        EVT_UTXOS_SORT_REQUEST                                                    , // 258
        EVT_UTXOS_BACK_REQUEST                                                    , // 259
        EVT_UTXOS_CONSOLIDATE_REQUEST                                             , // 260
        EVT_UTXOS_ITEM_SELECTED                                                   , // 261
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 262
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 263
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 264
        EVT_WALLET_INFO_EDIT_NAME                                    , // 265
        EVT_WALLET_INFO_REMOVE                                       , // 266
        EVT_WALLET_INFO_EXPORT_DB                                    , // 267
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 268
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 269
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 270
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 271
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 272
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 273
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 274
        EVT_WALLET_INFO_REFRESH_WALLET_REQUEST                       , // 275
        EVT_WALLET_INFO_GAP_LIMIT_REQUEST                            , // 276
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 277
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 278
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 279
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 280
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 281
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 282
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 283
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 284
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 285
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 286
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 287
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 288
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 289
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 290
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 291
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 292
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 293
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 294
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 295
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 296
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 297
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 298
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 299
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 300
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 301
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 302
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 303
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 304
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 305
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 306
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 307
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 308
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 309
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 310
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 311
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 312
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 313
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 314
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 315
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 316
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 317
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 318
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 319
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 320
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 321
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 322
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 323
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 324
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 325
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 326
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 327
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 328
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 329
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 330
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 331
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 332
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 333
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 334
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 335
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 336
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 337
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 338
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 339
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 340
        EVT_INPUT_PIN_SEND_PIN                                           , // 341
        EVT_INPUT_PIN_CLOSE                                              , // 342
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 343
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 344
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 345
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 346
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 347
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 348
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 349
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 350
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 351
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 352
        EVT_CREATE_NEW_SEED                                                               , // 353
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 354
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 355
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 356
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 357
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 358
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 359
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 360
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 361
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 362
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 363
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 364
        EVT_CREATE_NEW_SEED_BACK                                                           , // 365
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 366
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 367
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 368
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 369
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 370
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 371
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 372
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 373
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 374
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 375
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 376
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 377
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 378
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 379
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 380
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 381
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 382
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 383
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 384
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 385
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 386
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 387
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 388
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 389
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 390
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 391
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 392
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 393
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 394
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 395
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 396
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 397
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 398
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 399
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 400
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 401
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 402
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 403
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 404
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 405
        EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ                                 , // 406
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 407
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 408
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 409
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 410
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 411
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 412
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 413
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 414
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 415
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 416
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 417
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 418
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 419
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 420
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 421
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 422
        EVT_PENDING_REQUEST_IGNORE                                                     , // 423
        EVT_PENDING_REQUEST_BACK                                                       , // 424
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 425
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 426
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 427
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 428
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 429
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 430
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 431
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 432
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 433
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 434
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 435
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 436
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 437
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 438
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 439
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 440
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 441
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 442
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 443
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 444
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 445
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 446
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 447
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 448
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 449
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 450
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 451
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 452
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 453
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 454
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 455
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 456
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 457
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 458
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 459
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 460
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 461
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 462
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 463
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 464
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 465
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 466
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 467
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 468
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 469
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 470
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 471
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 472
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 473
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 474
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 475
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 476
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 477
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 478
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 479
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 480
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 481
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 482
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 483
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 484
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 485
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 486
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 487
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 488
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 489
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 490
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 491
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 492
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 493
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 494
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 495
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 496
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 497
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 498
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 499
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 500
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 501
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 502
        EVT_CHANGE_PASSWORD_BACK                                                  , // 503
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 504
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 505
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 506
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 507
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 508
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 509
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 510
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 511
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 512
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 513
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 514
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 515
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 516
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 517
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 518
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 519
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 520
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 521
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 522
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 523
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 524
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 525
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 526
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 527
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 528
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 529
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 530
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 531
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 532
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 533
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 534
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 535
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 536
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 537
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 538
        EVT_UPDATE_PROFILE_REQUEST                                        , // 539
        EVT_UPDATE_PROFILE_BACK                                           , // 540
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 541
    };
    enum EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS
    {
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MIN=EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX+1, // 542
        EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST                                              , // 543
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX                                      , // 544
    };
    enum EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD
    {
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MIN=EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX+1, // 545
        EVT_INPUT_BACKUP_PASSWORD_REQUEST                                                            , // 546
        EVT_ENTER_BACKUP_PASSWORD_BACK                                                               , // 547
        EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST                                                      , // 548
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX                                                   , // 549
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX+1, // 550
        EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST                                                                  , // 551
        EVT_ANSER_SECURITY_QUESTION_BACK                                                                      , // 552
        EVT_ENTER_BACKUP_PASSWORD_RERQUEST                                                                    , // 553
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX                                             , // 554
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX+1, // 555
        EVT_INPUT_TAPSIGNER_SELECT_REQUEST                                                           , // 556
        EVT_ANSER_SECURITY_QUESTION_REQUEST                                                          , // 557
        EVT_STATE_ID_SCR_KEY_RECOVERY_MAX                                                            , // 558
    };
    enum EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD
    {
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_MAX+1, // 559
        EVT_INPUT_PASSWORD_REQUEST                                                    , // 560
        EVT_REENTER_YOUR_PASSWORD_BACK                                                , // 561
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX                                    , // 562
    };
    enum EVT_STATE_ID_SCR_SERVICE_SETTINGS
    {
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MIN=EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX+1, // 563
        EVT_CLAIM_INHERITANCE_CHECK_REQUEST                                               , // 564
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST                                          , // 565
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED                                          , // 566
        EVT_SERVICE_SELECT_WALLET_REQUEST                                                 , // 567
        EVT_INHERITANCE_PLAN_FINALIZE_REQUEST                                             , // 568
        EVT_SERVICE_SUPPORT_REQUEST                                                       , // 569
        EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST                                          , // 570
        EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST                                            , // 571
        EVT_KEY_RECOVERY_REQUEST                                                          , // 572
        EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST                                           , // 573
        EVT_WALLET_CO_SIGN_POLICE_REQUEST                                                 , // 574
        EVT_LOCKDOWN_SUCCESS_REQUEST                                                      , // 575
        EVT_SETUP_SECURITY_QUESTION_REQUEST                                               , // 576
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX                                             , // 577
    };
    enum EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD
    {
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MIN=EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX+1, // 578
        EVT_INPUT_DAYS_VALUE_REQUEST                                                            , // 579
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST                                            , // 580
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX                                        , // 581
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX+1, // 582
        EVT_INPUT_LOCKDOWN_ANSER_REQUEST                                                                        , // 583
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK                                                               , // 584
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX                                                   , // 585
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS
    {
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MIN=EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX+1, // 586
        EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST                                                           , // 587
        EVT_CLOSE_LOCKDOWN_SUCCESS                                                                   , // 588
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX                                                        , // 589
    };
    enum EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX+1, // 590
        EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST                                         , // 591
        EVT_DUMMY_TRANSACTION_INFO_BACK                                                    , // 592
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX                                        , // 593
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION
    {
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MIN=EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX+1, // 594
        EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ                                                           , // 595
        EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST                                                       , // 596
        EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK                                                          , // 597
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX                                              , // 598
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS
    {
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MIN=EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX+1, // 599
        EVT_INHERITANCE_TRANSACTION_DETAILS_BACK                                                                   , // 600
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX                                                       , // 601
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE
    {
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MIN=EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX+1, // 602
        EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST                                                                 , // 603
        EVT_INHERITANCE_NEW_TRANSACTION_REQ                                                                     , // 604
        EVT_INHERITANCE_WITHDRAW_BALANCE_BACK                                                                   , // 605
        EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST                                                             , // 606
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX                                                       , // 607
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_ASK
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MIN=EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX+1, // 608
        EVT_ADD_HARDWARE_ASK_ENTER                                                               , // 609
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX                                                    , // 610
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX+1, // 611
        EVT_SCAN_HARDWARE_DEVICE_REQUEST                                         , // 612
        EVT_ADD_HARDWARE_DEVICE_REQUEST                                          , // 613
        EVT_STATE_ID_SCR_ADD_HARDWARE_MAX                                        , // 614
    };
    enum EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE
    {
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_MAX+1, // 615
        EVT_SELECT_WALLET_REQUEST                                                            , // 616
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX                                    , // 617
    };
    enum EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN
    {
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MIN=EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX+1, // 618
        EVT_UPDATE_ACTIVATION_DATE_REQUEST                                                                 , // 619
        EVT_UPDATE_MESSAGE_REQUEST                                                                         , // 620
        EVT_UPDATE_BUFFER_PERIOD_REQUEST                                                                   , // 621
        EVT_UPDATE_NOTIFICATION_PREFERENCE_REQ                                                             , // 622
        EVT_NOT_UPDATE_ANY_NOTI_REQ                                                                        , // 623
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX                                                    , // 624
    };
    enum EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS
    {
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MIN=EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX+1, // 625
        EVT_UPDATE_YOUR_SECRET_REQUEST                                                           , // 626
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX                                                  , // 627
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MIN=EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX+1, // 628
        EVT_ADD_EXIST_HARDWARE_REQUEST                                                   , // 629
        EVT_ADD_NEW_HARDWARE_REQUEST                                                     , // 630
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX                                          , // 631
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX+1, // 632
        EVT_DASHBOARD_ALERT_INFO_ENTER                                                     , // 633
        EVT_ADD_HARDWARE_KEY_EXIST_REQ                                                     , // 634
        EVT_ADD_HARDWARE_KEY_REQUEST                                                       , // 635
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX                                          , // 636
    };
    enum EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS
    {
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX+1, // 637
        EVT_KEY_HEALTH_CHECK_STATUS_ENTER                                                       , // 638
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX                                            , // 639
    };
    enum EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING
    {
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MIN=EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX+1, // 640
        EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST                                                    , // 641
        EVT_HEALTH_CHECK_STARTING_CLOSE                                                          , // 642
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX                                               , // 643
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MIN=EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX+1, // 644
        EVT_DASHBOARD_ALERT_SUCCESS_ENTER_REQUEST                                                , // 645
        EVT_DASHBOARD_ALERT_SUCCESS_CLOSE                                                        , // 646
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX                                             , // 647
    };
    enum EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX+1, // 648
        EVT_INPUT_SECURITY_QUESTION_REQUEST                                                        , // 649
        EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ                                                     , // 650
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX                                               , // 651
    };
    enum EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX+1, // 652
        EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ                                                            , // 653
        EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK                                                           , // 654
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX                                               , // 655
    };
    enum EVT_STATE_ID_SCR_RECURRING_PAYMENTS
    {
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MIN=EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX+1, // 656
        EVT_RECURRING_PAYMENTS_ENTER                                                                 , // 657
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX                                                      , // 658
    };
    enum EVT_STATE_ID_SCR_ONBOARDING
    {
        EVT_STATE_ID_SCR_ONBOARDING_MIN=EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX+1, // 659
        EVT_ONBOARDING_ACTION_REQUEST                                            , // 660
        EVT_ONBOARDING_CLOSE                                                     , // 661
        EVT_STATE_ID_SCR_ONBOARDING_MAX                                          , // 662
    };
    enum EVT_STATE_ID_SCR_SELECT_SERVER
    {
        EVT_STATE_ID_SCR_SELECT_SERVER_MIN=EVT_STATE_ID_SCR_ONBOARDING_MAX+1, // 663
        EVT_SELECT_SERVER_ENTER                                             , // 664
        EVT_STATE_ID_SCR_SELECT_SERVER_MAX                                  , // 665
    };
    enum EVT_STATE_ID_SCR_CHANGE_EMAIL
    {
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MIN=EVT_STATE_ID_SCR_SELECT_SERVER_MAX+1, // 666
        EVT_CHANGE_EMAIL_ENTER                                                , // 667
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MAX                                     , // 668
    };
    enum EVT_STATE_ID_SCR_REPLACE_SELECT_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_EMAIL_MAX+1, // 669
        EVT_REPLACE_SELECT_KEY_ENTER                                               , // 670
        EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MAX                                    , // 671
    };
    enum EVT_STATE_ID_SCR_REPLACE_KEYS
    {
        EVT_STATE_ID_SCR_REPLACE_KEYS_MIN=EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MAX+1, // 672
        EVT_REPLACE_KEYS_ENTER                                                     , // 673
        EVT_STATE_ID_SCR_REPLACE_KEYS_MAX                                          , // 674
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB
    {
        EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB_MIN=EVT_STATE_ID_SCR_REPLACE_KEYS_MAX+1, // 675
        EVT_SIGN_IN_VIA_XPUB_ENTER                                               , // 676
        EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB_MAX                                    , // 677
    };

};
#endif // VIEWSENUMS_H
