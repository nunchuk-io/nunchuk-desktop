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
    Q_ENUMS(EVT_STATE_ID_SCR_EDIT_MEMBERS)
    Q_ENUMS(EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB)
    Q_ENUMS(EVT_STATE_ID_SCR_COIN_DETAILS_CONTROL)
    Q_ENUMS(EVT_STATE_ID_SCR_SETUP_GROUP_WALLET)
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
        STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET       , // 16
        STATE_ID_SCR_WALLET_CHANGE_ADDRESSES             , // 17
        STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT            , // 18
        STATE_ID_SCR_REMOTE_SIGNER_INFO                  , // 19
        STATE_ID_SCR_UNLOCK_DB                           , // 20
        STATE_ID_SCR_CONSOLIDATE_OUTPUT                  , // 21
        STATE_ID_SCR_UTXO_OUTPUT                         , // 22
        STATE_ID_SCR_INPUT_PIN                           , // 23
        STATE_ID_SCR_ADD_NEW_SIGNER                      , // 24
        STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER             , // 25
        STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER             , // 26
        STATE_ID_SCR_CREATE_NEW_SEED                     , // 27
        STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION       , // 28
        STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET            , // 29
        STATE_ID_SCR_INPUT_PASSPHRASE                    , // 30
        STATE_ID_SCR_LOGIN_ONLINE                        , // 31
        STATE_ID_SCR_HOME_ONLINE                         , // 32
        STATE_ID_SCR_ONLINE_ADD_CONTACTS                 , // 33
        STATE_ID_SCR_PENDING_REQUEST                     , // 34
        STATE_ID_SCR_CREATE_SHARED_WALLET                , // 35
        STATE_ID_SCR_CONFIGURE_SHARED_WALLET             , // 36
        STATE_ID_SCR_REVIEW_SHARED_WALLET                , // 37
        STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET      , // 38
        STATE_ID_SCR_SHARED_WALLET_CONFIGURE             , // 39
        STATE_ID_SCR_BACKUP_SHARED_WALLET                , // 40
        STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION       , // 41
        STATE_ID_SCR_STARTING_CREATE_SIGNER              , // 42
        STATE_ID_TOAST_MESSAGE_DISPLAY                   , // 43
        STATE_ID_SCR_RECOVER_SHARED_WALLET               , // 44
        STATE_ID_SCR_PRIMARY_KEY_ACCOUNT                 , // 45
        STATE_ID_SCR_CREATE_ACCOUNT                      , // 46
        STATE_ID_SCR_CHANGE_PASSWORD                     , // 47
        STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY, // 48
        STATE_ID_SCR_SIGN_IN_MANUALLY                    , // 49
        STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY             , // 50
        STATE_ID_SCR_REPLACE_PRIMARY_KEY                 , // 51
        STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION           , // 52
        STATE_ID_SCR_SIGN_IN                             , // 53
        STATE_ID_SCR_UPDATE_PROFILE                      , // 54
        STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS         , // 55
        STATE_ID_SCR_ENTER_BACKUP_PASSWORD               , // 56
        STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION, // 57
        STATE_ID_SCR_KEY_RECOVERY                        , // 58
        STATE_ID_SCR_REENTER_YOUR_PASSWORD               , // 59
        STATE_ID_SCR_SERVICE_SETTINGS                    , // 60
        STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD         , // 61
        STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION    , // 62
        STATE_ID_SCR_LOCKDOWN_SUCCESS                    , // 63
        STATE_ID_SCR_DUMMY_TRANSACTION_INFO              , // 64
        STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION     , // 65
        STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS     , // 66
        STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE        , // 67
        STATE_ID_SCR_ADD_HARDWARE_ASK                    , // 68
        STATE_ID_SCR_ADD_HARDWARE                        , // 69
        STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE        , // 70
        STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN          , // 71
        STATE_ID_SCR_SHARE_YOUR_SECRETS                  , // 72
        STATE_ID_SCR_ADD_HARDWARE_EXIST                  , // 73
        STATE_ID_SCR_DASHBOARD_ALERT_INFO                , // 74
        STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS             , // 75
        STATE_ID_SCR_HEALTH_CHECK_STARTING               , // 76
        STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS             , // 77
        STATE_ID_SCR_SETUP_SECURITY_QUESTION             , // 78
        STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION      , // 79
        STATE_ID_SCR_RECURRING_PAYMENTS                  , // 80
        STATE_ID_SCR_ONBOARDING                          , // 81
        STATE_ID_SCR_SELECT_SERVER                       , // 82
        STATE_ID_SCR_CHANGE_EMAIL                        , // 83
        STATE_ID_SCR_REPLACE_SELECT_KEY                  , // 84
        STATE_ID_SCR_REPLACE_KEYS                        , // 85
        STATE_ID_SCR_EDIT_MEMBERS                        , // 86
        STATE_ID_SCR_SIGN_IN_VIA_XPUB                    , // 87
        STATE_ID_SCR_COIN_DETAILS_CONTROL                , // 88
        STATE_ID_SCR_SETUP_GROUP_WALLET                  , // 89
        EVT_STATE_ID_REGISTED_MAX                        , // 90
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1   , // 91
        EVT_STARTING_APPLICATION_LOCALMODE                  , // 92
        EVT_STARTING_APPLICATION_ONLINEMODE                 , // 93
        EVT_STARTING_APPLICATION_ONLINE_HWL                 , // 94
        EVT_SETTING_ACCOUNT_CHANGE_PASSWORD                 , // 95
        EVT_ONS_CLOSE_ALL_REQUEST                           , // 96
        EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST                , // 97
        EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST                 , // 98
        EVT_GOTO_HOME_WALLET_TAB                            , // 99
        EVT_ONS_CLOSE_REQUEST                               , // 100
        EVT_LOGIN_DB_REQUEST                                , // 101
        EVT_ROOT_PROMT_PIN                                  , // 102
        EVT_ROOT_PROMT_PASSPHRASE                           , // 103
        EVT_LOGIN_MATRIX_REQUEST                            , // 104
        EVT_GOTO_HOME_CHAT_TAB                              , // 105
        EVT_ONLINE_ONS_CLOSE_REQUEST                        , // 106
        EVT_SHOW_TOAST_MESSAGE                              , // 107
        EVT_ROOT_UPDATE_PROFILE                             , // 108
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                     , // 109
        EVT_GOTO_APP_SETTINGS_TAB                           , // 110
        EVT_SIGN_IN_REQUEST                                 , // 111
        EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST                  , // 112
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST                 , // 113
        EVT_GOTO_SERVICE_SETTING_TAB                        , // 114
        EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST               , // 115
        EVT_NUNCHUK_LOGIN_SUCCEEDED                         , // 116
        EVT_HEALTH_CHECK_STARTING_REQUEST                   , // 117
        EVT_DUMMY_TRANSACTION_INFO_REQUEST                  , // 118
        EVT_DASHBOARD_ALERT_SUCCESS_REQUEST                 , // 119
        EVT_SHARE_YOUR_SECRET_REQUEST                       , // 120
        EVT_REENTER_YOUR_PASSWORD_REQUEST                   , // 121
        EVT_ONBOARDING_REQUEST                              , // 122
        EVT_REPLACE_SELECT_KEY_REQUEST                      , // 123
        EVT_EDIT_MEMBERS_REQUEST                            , // 124
        EVT_SIGN_IN_VIA_XPUB_REQUEST                        , // 125
        EVT_COIN_DETAILS_CONTROL_REQUEST                    , // 126
        EVT_UTXOS_CONSOLIDATE_REQUEST                       , // 127
        EVT_CONSOLIDATE_COINS_MERGE_MAKE_TRANSACTION_REQUEST, // 128
        EVT_SETUP_GROUP_WALLET_REQUEST                      , // 129
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED         , // 130
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST               , // 131
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                     , // 132
        EVT_HOME_ADD_WALLET_REQUEST                         , // 133
        EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST                , // 134
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST         , // 135
        EVT_EXIST_HARDWARE_REQ                              , // 136
        EVT_ADD_HARDWARE_REQUEST                            , // 137
        EVT_STATE_ID_ROOT_MAX                               , // 138
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 139
        EVT_HOME_WALLET_SELECTED                         , // 140
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 141
        EVT_HOME_SETTING_REQUEST                         , // 142
        EVT_HOME_DISPLAY_ADDRESS                         , // 143
        EVT_HOME_IMPORT_PSBT                             , // 144
        EVT_HOME_EXPORT_BSMS                             , // 145
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 146
        EVT_HOME_SEND_REQUEST                            , // 147
        EVT_HOME_RECEIVE_REQUEST                         , // 148
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 149
        EVT_HOME_WALLET_INFO_REQUEST                     , // 150
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 151
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 152
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 153
        EVT_ASK_HARDWARE_REQ                             , // 154
        EVT_KEY_HEALTH_CHECK_STATUS_REQUEST              , // 155
        EVT_RECURRING_PAYMENTS_REQUEST                   , // 156
        EVT_ADD_SOFTWARE_SIGNER_RESULT                   , // 157
        EVT_REPLACE_KEYS_REQUEST                         , // 158
        EVT_STATE_ID_SCR_HOME_MAX                        , // 159
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 160
        EVT_ADD_WALLET_IMPORT                                      , // 161
        EVT_ADD_WALLET_BACK_REQUEST                                , // 162
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 163
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 164
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 165
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 166
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 167
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 168
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 169
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 170
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 171
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 172
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 173
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 174
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 175
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 176
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 177
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 178
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 179
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 180
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 181
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 182
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 183
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 184
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 185
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 186
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 187
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 188
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 189
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 190
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 191
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 192
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 193
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 194
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 195
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 196
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 197
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 198
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 199
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 200
        EVT_RECEIVE_BACK_REQUEST                                                           , // 201
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 202
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 203
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 204
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 205
        EVT_SEND_BACK_REQUEST                                   , // 206
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 207
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 208
        EVT_STATE_ID_SCR_SEND_MAX                               , // 209
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 210
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 211
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 212
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 213
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 214
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 215
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 216
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 217
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 218
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 219
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 220
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 221
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 222
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 223
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 224
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 225
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 226
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 227
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 228
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 229
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 230
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 231
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 232
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 233
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 234
        EVT_TRANSACTION_INFO_CANCEL_PENDING_TRANSACTION_REQUEST                        , // 235
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 236
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 237
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 238
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 239
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 240
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 241
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 242
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 243
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 244
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 245
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 246
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 247
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 248
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 249
        EVT_APP_SETTING_BACK_REQUEST                                                , // 250
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 251
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 252
        EVT_SELECT_SERVER_REQUEST                                                   , // 253
        EVT_CHANGE_EMAIL_REQUEST                                                    , // 254
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 255
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 256
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 257
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 258
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 259
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 260
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 261
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 262
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 263
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 264
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 265
        EVT_UTXOS_SORT_REQUEST                                                    , // 266
        EVT_UTXOS_BACK_REQUEST                                                    , // 267
        EVT_UTXOS_ITEM_SELECTED                                                   , // 268
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 269
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 270
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 271
        EVT_WALLET_INFO_EDIT_NAME                                    , // 272
        EVT_WALLET_INFO_REMOVE                                       , // 273
        EVT_WALLET_INFO_EXPORT_DB                                    , // 274
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 275
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 276
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 277
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 278
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 279
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 280
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 281
        EVT_WALLET_INFO_REFRESH_WALLET_REQUEST                       , // 282
        EVT_WALLET_INFO_GAP_LIMIT_REQUEST                            , // 283
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 284
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 285
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 286
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 287
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 288
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 289
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 290
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                      , // 291
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                      , // 292
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                              , // 293
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                           , // 294
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                          , // 295
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                               , // 296
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                               , // 297
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                               , // 298
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                   , // 299
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 300
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 301
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 302
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 303
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 304
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 305
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 306
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 307
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 308
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 309
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 310
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 311
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 312
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 313
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 314
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 315
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 316
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 317
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 318
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 319
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 320
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 321
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 322
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 323
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 324
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 325
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 326
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 327
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 328
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 329
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 330
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 331
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 332
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 333
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 334
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 335
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 336
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 337
        EVT_INPUT_PIN_SEND_PIN                                           , // 338
        EVT_INPUT_PIN_CLOSE                                              , // 339
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 340
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 341
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 342
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 343
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
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 369
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 370
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 371
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 372
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 373
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 374
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 375
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 376
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 377
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 378
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 379
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 380
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 381
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 382
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 383
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 384
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 385
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 386
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 387
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 388
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 389
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 390
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 391
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 392
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 393
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 394
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 395
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 396
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 397
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 398
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 399
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 400
        EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ                                 , // 401
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 402
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 403
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 404
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 405
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 406
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 407
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 408
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 409
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 410
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 411
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 412
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 413
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 414
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 415
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 416
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 417
        EVT_PENDING_REQUEST_IGNORE                                                     , // 418
        EVT_PENDING_REQUEST_BACK                                                       , // 419
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 420
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 421
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 422
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 423
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 424
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 425
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 426
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 427
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 428
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 429
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 430
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 431
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 432
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 433
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 434
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 435
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 436
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 437
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 438
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 439
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 440
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 441
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 442
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 443
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 444
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 445
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 446
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 447
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 448
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 449
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 450
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 451
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 452
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 453
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 454
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 455
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 456
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 457
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 458
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 459
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 460
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 461
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 462
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 463
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 464
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 465
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 466
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 467
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 468
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 469
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 470
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 471
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 472
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 473
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 474
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 475
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 476
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 477
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 478
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 479
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 480
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 481
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 482
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 483
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 484
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 485
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 486
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 487
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 488
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 489
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 490
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 491
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 492
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 493
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 494
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 495
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 496
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 497
        EVT_CHANGE_PASSWORD_BACK                                                  , // 498
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 499
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 500
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 501
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 502
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 503
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 504
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 505
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 506
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 507
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 508
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 509
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 510
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 511
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 512
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 513
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 514
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 515
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 516
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 517
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 518
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 519
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 520
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 521
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 522
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 523
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 524
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 525
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 526
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 527
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 528
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 529
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 530
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 531
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 532
        EVT_UPDATE_PROFILE_REQUEST                                        , // 533
        EVT_UPDATE_PROFILE_BACK                                           , // 534
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 535
    };
    enum EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS
    {
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MIN=EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX+1, // 536
        EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST                                              , // 537
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX                                      , // 538
    };
    enum EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD
    {
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MIN=EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX+1, // 539
        EVT_INPUT_BACKUP_PASSWORD_REQUEST                                                            , // 540
        EVT_ENTER_BACKUP_PASSWORD_BACK                                                               , // 541
        EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST                                                      , // 542
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX                                                   , // 543
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX+1, // 544
        EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST                                                                  , // 545
        EVT_ANSER_SECURITY_QUESTION_BACK                                                                      , // 546
        EVT_ENTER_BACKUP_PASSWORD_RERQUEST                                                                    , // 547
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX                                             , // 548
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX+1, // 549
        EVT_INPUT_TAPSIGNER_SELECT_REQUEST                                                           , // 550
        EVT_ANSER_SECURITY_QUESTION_REQUEST                                                          , // 551
        EVT_STATE_ID_SCR_KEY_RECOVERY_MAX                                                            , // 552
    };
    enum EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD
    {
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_MAX+1, // 553
        EVT_INPUT_PASSWORD_REQUEST                                                    , // 554
        EVT_REENTER_YOUR_PASSWORD_BACK                                                , // 555
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX                                    , // 556
    };
    enum EVT_STATE_ID_SCR_SERVICE_SETTINGS
    {
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MIN=EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX+1, // 557
        EVT_CLAIM_INHERITANCE_CHECK_REQUEST                                               , // 558
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST                                          , // 559
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED                                          , // 560
        EVT_SERVICE_SELECT_WALLET_REQUEST                                                 , // 561
        EVT_INHERITANCE_PLAN_FINALIZE_REQUEST                                             , // 562
        EVT_SERVICE_SUPPORT_REQUEST                                                       , // 563
        EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST                                          , // 564
        EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST                                            , // 565
        EVT_KEY_RECOVERY_REQUEST                                                          , // 566
        EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST                                           , // 567
        EVT_WALLET_CO_SIGN_POLICE_REQUEST                                                 , // 568
        EVT_LOCKDOWN_SUCCESS_REQUEST                                                      , // 569
        EVT_SETUP_SECURITY_QUESTION_REQUEST                                               , // 570
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX                                             , // 571
    };
    enum EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD
    {
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MIN=EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX+1, // 572
        EVT_INPUT_DAYS_VALUE_REQUEST                                                            , // 573
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST                                            , // 574
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX                                        , // 575
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX+1, // 576
        EVT_INPUT_LOCKDOWN_ANSER_REQUEST                                                                        , // 577
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK                                                               , // 578
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX                                                   , // 579
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS
    {
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MIN=EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX+1, // 580
        EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST                                                           , // 581
        EVT_CLOSE_LOCKDOWN_SUCCESS                                                                   , // 582
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX                                                        , // 583
    };
    enum EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX+1, // 584
        EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST                                         , // 585
        EVT_DUMMY_TRANSACTION_INFO_BACK                                                    , // 586
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX                                        , // 587
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION
    {
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MIN=EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX+1, // 588
        EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ                                                           , // 589
        EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST                                                       , // 590
        EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK                                                          , // 591
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX                                              , // 592
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS
    {
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MIN=EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX+1, // 593
        EVT_INHERITANCE_TRANSACTION_DETAILS_ACTION                                                                 , // 594
        EVT_INHERITANCE_TRANSACTION_DETAILS_BACK                                                                   , // 595
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX                                                       , // 596
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE
    {
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MIN=EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX+1, // 597
        EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST                                                                 , // 598
        EVT_INHERITANCE_NEW_TRANSACTION_REQ                                                                     , // 599
        EVT_INHERITANCE_WITHDRAW_BALANCE_BACK                                                                   , // 600
        EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST                                                             , // 601
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX                                                       , // 602
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_ASK
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MIN=EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX+1, // 603
        EVT_ADD_HARDWARE_ASK_ENTER                                                               , // 604
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX                                                    , // 605
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX+1, // 606
        EVT_SCAN_HARDWARE_DEVICE_REQUEST                                         , // 607
        EVT_ADD_HARDWARE_DEVICE_REQUEST                                          , // 608
        EVT_STATE_ID_SCR_ADD_HARDWARE_MAX                                        , // 609
    };
    enum EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE
    {
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_MAX+1, // 610
        EVT_SELECT_WALLET_REQUEST                                                            , // 611
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX                                    , // 612
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
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MIN=EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX+1, // 623
        EVT_ADD_EXIST_HARDWARE_REQUEST                                                   , // 624
        EVT_ADD_NEW_HARDWARE_REQUEST                                                     , // 625
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX                                          , // 626
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX+1, // 627
        EVT_DASHBOARD_ALERT_INFO_ENTER                                                     , // 628
        EVT_ADD_HARDWARE_KEY_EXIST_REQ                                                     , // 629
        EVT_ADD_HARDWARE_KEY_REQUEST                                                       , // 630
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX                                          , // 631
    };
    enum EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS
    {
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX+1, // 632
        EVT_KEY_HEALTH_CHECK_STATUS_ENTER                                                       , // 633
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX                                            , // 634
    };
    enum EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING
    {
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MIN=EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX+1, // 635
        EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST                                                    , // 636
        EVT_HEALTH_CHECK_STARTING_CLOSE                                                          , // 637
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX                                               , // 638
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MIN=EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX+1, // 639
        EVT_DASHBOARD_ALERT_SUCCESS_ENTER_REQUEST                                                , // 640
        EVT_DASHBOARD_ALERT_SUCCESS_CLOSE                                                        , // 641
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX                                             , // 642
    };
    enum EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX+1, // 643
        EVT_INPUT_SECURITY_QUESTION_REQUEST                                                        , // 644
        EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ                                                     , // 645
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX                                               , // 646
    };
    enum EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX+1, // 647
        EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ                                                            , // 648
        EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK                                                           , // 649
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX                                               , // 650
    };
    enum EVT_STATE_ID_SCR_RECURRING_PAYMENTS
    {
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MIN=EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX+1, // 651
        EVT_RECURRING_PAYMENTS_ENTER                                                                 , // 652
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX                                                      , // 653
    };
    enum EVT_STATE_ID_SCR_ONBOARDING
    {
        EVT_STATE_ID_SCR_ONBOARDING_MIN=EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX+1, // 654
        EVT_ONBOARDING_ACTION_REQUEST                                            , // 655
        EVT_ONBOARDING_CLOSE                                                     , // 656
        EVT_STATE_ID_SCR_ONBOARDING_MAX                                          , // 657
    };
    enum EVT_STATE_ID_SCR_SELECT_SERVER
    {
        EVT_STATE_ID_SCR_SELECT_SERVER_MIN=EVT_STATE_ID_SCR_ONBOARDING_MAX+1, // 658
        EVT_SELECT_SERVER_ENTER                                             , // 659
        EVT_STATE_ID_SCR_SELECT_SERVER_MAX                                  , // 660
    };
    enum EVT_STATE_ID_SCR_CHANGE_EMAIL
    {
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MIN=EVT_STATE_ID_SCR_SELECT_SERVER_MAX+1, // 661
        EVT_CHANGE_EMAIL_ENTER                                                , // 662
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MAX                                     , // 663
    };
    enum EVT_STATE_ID_SCR_REPLACE_SELECT_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_EMAIL_MAX+1, // 664
        EVT_REPLACE_SELECT_KEY_ENTER                                               , // 665
        EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MAX                                    , // 666
    };
    enum EVT_STATE_ID_SCR_REPLACE_KEYS
    {
        EVT_STATE_ID_SCR_REPLACE_KEYS_MIN=EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MAX+1, // 667
        EVT_REPLACE_KEYS_ENTER                                                     , // 668
        EVT_STATE_ID_SCR_REPLACE_KEYS_MAX                                          , // 669
    };
    enum EVT_STATE_ID_SCR_EDIT_MEMBERS
    {
        EVT_STATE_ID_SCR_EDIT_MEMBERS_MIN=EVT_STATE_ID_SCR_REPLACE_KEYS_MAX+1, // 670
        EVT_EDIT_MEMBERS_ENTER_REQUEST                                       , // 671
        EVT_EDIT_MEMBERS_CLOSE                                               , // 672
        EVT_STATE_ID_SCR_EDIT_MEMBERS_MAX                                    , // 673
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB
    {
        EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB_MIN=EVT_STATE_ID_SCR_EDIT_MEMBERS_MAX+1, // 674
        EVT_SIGN_IN_VIA_XPUB_ENTER                                               , // 675
        EVT_SIGN_IN_VIA_XPUB_CLOSE                                               , // 676
        EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB_MAX                                    , // 677
    };
    enum EVT_STATE_ID_SCR_COIN_DETAILS_CONTROL
    {
        EVT_STATE_ID_SCR_COIN_DETAILS_CONTROL_MIN=EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB_MAX+1, // 678
        EVT_COIN_DETAILS_CONTROL_ENTER                                                   , // 679
        EVT_COIN_DETAILS_CONTROL_CLOSE                                                   , // 680
        EVT_STATE_ID_SCR_COIN_DETAILS_CONTROL_MAX                                        , // 681
    };
    enum EVT_STATE_ID_SCR_SETUP_GROUP_WALLET
    {
        EVT_STATE_ID_SCR_SETUP_GROUP_WALLET_MIN=EVT_STATE_ID_SCR_COIN_DETAILS_CONTROL_MAX+1, // 682
        EVT_SETUP_GROUP_WALLET_ENTER                                                       , // 683
        EVT_SETUP_GROUP_WALLET_BACK                                                        , // 684
        EVT_STATE_ID_SCR_SETUP_GROUP_WALLET_MAX                                            , // 685
    };

};
#endif // VIEWSENUMS_H
