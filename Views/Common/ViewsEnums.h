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
        STATE_ID_SCR_EDIT_MEMBERS                        , // 87
        STATE_ID_SCR_SIGN_IN_VIA_XPUB                    , // 88
        STATE_ID_SCR_COIN_DETAILS_CONTROL                , // 89
        STATE_ID_SCR_SETUP_GROUP_WALLET                  , // 90
        EVT_STATE_ID_REGISTED_MAX                        , // 91
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1   , // 92
        EVT_STARTING_APPLICATION_LOCALMODE                  , // 93
        EVT_STARTING_APPLICATION_ONLINEMODE                 , // 94
        EVT_STARTING_APPLICATION_ONLINE_HWL                 , // 95
        EVT_SETTING_ACCOUNT_CHANGE_PASSWORD                 , // 96
        EVT_ONS_CLOSE_ALL_REQUEST                           , // 97
        EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST                , // 98
        EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST                 , // 99
        EVT_GOTO_HOME_WALLET_TAB                            , // 100
        EVT_ONS_CLOSE_REQUEST                               , // 101
        EVT_LOGIN_DB_REQUEST                                , // 102
        EVT_ROOT_PROMT_PIN                                  , // 103
        EVT_ROOT_PROMT_PASSPHRASE                           , // 104
        EVT_LOGIN_MATRIX_REQUEST                            , // 105
        EVT_GOTO_HOME_CHAT_TAB                              , // 106
        EVT_ONLINE_ONS_CLOSE_REQUEST                        , // 107
        EVT_SHOW_TOAST_MESSAGE                              , // 108
        EVT_ROOT_UPDATE_PROFILE                             , // 109
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                     , // 110
        EVT_GOTO_APP_SETTINGS_TAB                           , // 111
        EVT_SIGN_IN_REQUEST                                 , // 112
        EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST                  , // 113
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST                 , // 114
        EVT_GOTO_SERVICE_SETTING_TAB                        , // 115
        EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST               , // 116
        EVT_NUNCHUK_LOGIN_SUCCEEDED                         , // 117
        EVT_HEALTH_CHECK_STARTING_REQUEST                   , // 118
        EVT_DUMMY_TRANSACTION_INFO_REQUEST                  , // 119
        EVT_DASHBOARD_ALERT_SUCCESS_REQUEST                 , // 120
        EVT_SHARE_YOUR_SECRET_REQUEST                       , // 121
        EVT_REENTER_YOUR_PASSWORD_REQUEST                   , // 122
        EVT_ONBOARDING_REQUEST                              , // 123
        EVT_REPLACE_SELECT_KEY_REQUEST                      , // 124
        EVT_EDIT_MEMBERS_REQUEST                            , // 125
        EVT_SIGN_IN_VIA_XPUB_REQUEST                        , // 126
        EVT_COIN_DETAILS_CONTROL_REQUEST                    , // 127
        EVT_UTXOS_CONSOLIDATE_REQUEST                       , // 128
        EVT_CONSOLIDATE_COINS_MERGE_MAKE_TRANSACTION_REQUEST, // 129
        EVT_SETUP_GROUP_WALLET_REQUEST                      , // 130
        EVT_STATE_ID_ROOT_MAX                               , // 131
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 132
        EVT_HOME_WALLET_SELECTED                         , // 133
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 134
        EVT_HOME_SETTING_REQUEST                         , // 135
        EVT_HOME_DISPLAY_ADDRESS                         , // 136
        EVT_HOME_IMPORT_PSBT                             , // 137
        EVT_HOME_EXPORT_BSMS                             , // 138
        EVT_HOME_ADD_WALLET_REQUEST                      , // 139
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 140
        EVT_HOME_SEND_REQUEST                            , // 141
        EVT_HOME_RECEIVE_REQUEST                         , // 142
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 143
        EVT_HOME_WALLET_INFO_REQUEST                     , // 144
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 145
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 146
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 147
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 148
        EVT_ASK_HARDWARE_REQ                             , // 149
        EVT_EXIST_HARDWARE_REQ                           , // 150
        EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST             , // 151
        EVT_KEY_HEALTH_CHECK_STATUS_REQUEST              , // 152
        EVT_RECURRING_PAYMENTS_REQUEST                   , // 153
        EVT_ADD_HARDWARE_REQUEST                         , // 154
        EVT_ADD_SOFTWARE_SIGNER_RESULT                   , // 155
        EVT_REPLACE_KEYS_REQUEST                         , // 156
        EVT_STATE_ID_SCR_HOME_MAX                        , // 157
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 158
        EVT_ADD_WALLET_IMPORT                                      , // 159
        EVT_ADD_WALLET_BACK_REQUEST                                , // 160
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 161
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 162
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 163
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 164
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 165
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 166
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 167
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 168
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 169
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 170
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 171
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 172
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 173
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 174
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 175
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 176
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 177
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 178
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 179
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 180
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 181
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 182
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 183
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 184
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 185
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 186
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 187
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 188
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 189
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 190
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 191
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 192
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
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 235
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
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 290
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 291
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 292
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 293
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 294
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 295
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 296
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 297
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 298
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 299
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 300
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 301
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 302
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 303
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 304
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 305
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 306
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 307
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 308
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 309
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 310
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 311
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 312
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 313
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 314
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 315
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 316
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 317
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 318
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 319
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 320
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 321
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 322
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 323
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 324
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 325
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 326
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 327
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 328
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 329
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 330
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 331
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 332
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 333
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 334
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 335
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 336
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 337
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 338
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 339
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 340
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 341
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 342
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 343
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 344
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 345
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 346
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 347
        EVT_INPUT_PIN_SEND_PIN                                           , // 348
        EVT_INPUT_PIN_CLOSE                                              , // 349
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 350
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 351
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 352
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 353
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 354
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 355
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 356
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 357
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 358
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 359
        EVT_CREATE_NEW_SEED                                                               , // 360
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 361
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 362
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 363
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 364
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 365
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 366
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 367
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 368
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 369
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 370
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 371
        EVT_CREATE_NEW_SEED_BACK                                                           , // 372
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 373
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 374
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 375
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 376
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 377
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 378
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 379
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 380
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 381
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 382
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 383
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 384
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 385
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 386
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 387
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 388
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 389
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 390
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 391
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 392
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 393
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 394
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 395
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 396
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 397
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 398
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 399
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 400
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 401
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 402
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 403
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 404
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 405
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 406
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 407
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 408
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 409
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 410
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 411
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 412
        EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ                                 , // 413
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 414
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 415
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 416
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 417
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 418
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 419
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 420
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 421
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 422
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 423
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 424
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 425
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 426
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 427
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 428
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 429
        EVT_PENDING_REQUEST_IGNORE                                                     , // 430
        EVT_PENDING_REQUEST_BACK                                                       , // 431
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 432
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 433
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 434
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 435
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 436
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 437
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 438
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 439
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 440
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 441
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 442
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 443
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 444
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 445
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 446
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 447
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 448
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 449
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 450
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 451
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 452
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 453
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 454
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 455
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 456
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 457
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 458
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 459
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 460
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 461
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 462
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 463
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 464
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 465
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 466
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 467
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 468
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 469
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 470
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 471
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 472
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 473
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 474
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 475
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 476
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 477
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 478
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 479
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 480
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 481
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 482
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 483
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 484
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 485
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 486
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 487
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 488
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 489
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 490
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 491
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 492
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 493
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 494
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 495
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 496
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 497
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 498
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 499
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 500
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 501
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 502
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 503
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 504
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 505
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 506
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 507
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 508
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 509
        EVT_CHANGE_PASSWORD_BACK                                                  , // 510
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 511
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 512
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 513
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 514
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 515
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 516
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 517
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 518
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 519
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 520
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 521
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 522
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 523
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 524
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 525
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 526
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 527
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 528
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 529
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 530
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 531
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 532
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 533
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 534
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 535
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 536
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 537
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 538
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 539
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 540
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 541
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 542
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 543
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 544
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 545
        EVT_UPDATE_PROFILE_REQUEST                                        , // 546
        EVT_UPDATE_PROFILE_BACK                                           , // 547
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 548
    };
    enum EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS
    {
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MIN=EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX+1, // 549
        EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST                                              , // 550
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX                                      , // 551
    };
    enum EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD
    {
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MIN=EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX+1, // 552
        EVT_INPUT_BACKUP_PASSWORD_REQUEST                                                            , // 553
        EVT_ENTER_BACKUP_PASSWORD_BACK                                                               , // 554
        EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST                                                      , // 555
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX                                                   , // 556
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX+1, // 557
        EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST                                                                  , // 558
        EVT_ANSER_SECURITY_QUESTION_BACK                                                                      , // 559
        EVT_ENTER_BACKUP_PASSWORD_RERQUEST                                                                    , // 560
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX                                             , // 561
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX+1, // 562
        EVT_INPUT_TAPSIGNER_SELECT_REQUEST                                                           , // 563
        EVT_ANSER_SECURITY_QUESTION_REQUEST                                                          , // 564
        EVT_STATE_ID_SCR_KEY_RECOVERY_MAX                                                            , // 565
    };
    enum EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD
    {
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_MAX+1, // 566
        EVT_INPUT_PASSWORD_REQUEST                                                    , // 567
        EVT_REENTER_YOUR_PASSWORD_BACK                                                , // 568
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX                                    , // 569
    };
    enum EVT_STATE_ID_SCR_SERVICE_SETTINGS
    {
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MIN=EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX+1, // 570
        EVT_CLAIM_INHERITANCE_CHECK_REQUEST                                               , // 571
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST                                          , // 572
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED                                          , // 573
        EVT_SERVICE_SELECT_WALLET_REQUEST                                                 , // 574
        EVT_INHERITANCE_PLAN_FINALIZE_REQUEST                                             , // 575
        EVT_SERVICE_SUPPORT_REQUEST                                                       , // 576
        EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST                                          , // 577
        EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST                                            , // 578
        EVT_KEY_RECOVERY_REQUEST                                                          , // 579
        EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST                                           , // 580
        EVT_WALLET_CO_SIGN_POLICE_REQUEST                                                 , // 581
        EVT_LOCKDOWN_SUCCESS_REQUEST                                                      , // 582
        EVT_SETUP_SECURITY_QUESTION_REQUEST                                               , // 583
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX                                             , // 584
    };
    enum EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD
    {
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MIN=EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX+1, // 585
        EVT_INPUT_DAYS_VALUE_REQUEST                                                            , // 586
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST                                            , // 587
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX                                        , // 588
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX+1, // 589
        EVT_INPUT_LOCKDOWN_ANSER_REQUEST                                                                        , // 590
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK                                                               , // 591
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX                                                   , // 592
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS
    {
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MIN=EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX+1, // 593
        EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST                                                           , // 594
        EVT_CLOSE_LOCKDOWN_SUCCESS                                                                   , // 595
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX                                                        , // 596
    };
    enum EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX+1, // 597
        EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST                                         , // 598
        EVT_DUMMY_TRANSACTION_INFO_BACK                                                    , // 599
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX                                        , // 600
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION
    {
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MIN=EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX+1, // 601
        EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ                                                           , // 602
        EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST                                                       , // 603
        EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK                                                          , // 604
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX                                              , // 605
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS
    {
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MIN=EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX+1, // 606
        EVT_INHERITANCE_TRANSACTION_DETAILS_BACK                                                                   , // 607
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX                                                       , // 608
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE
    {
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MIN=EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX+1, // 609
        EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST                                                                 , // 610
        EVT_INHERITANCE_NEW_TRANSACTION_REQ                                                                     , // 611
        EVT_INHERITANCE_WITHDRAW_BALANCE_BACK                                                                   , // 612
        EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST                                                             , // 613
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX                                                       , // 614
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_ASK
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MIN=EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX+1, // 615
        EVT_ADD_HARDWARE_ASK_ENTER                                                               , // 616
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX                                                    , // 617
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX+1, // 618
        EVT_SCAN_HARDWARE_DEVICE_REQUEST                                         , // 619
        EVT_ADD_HARDWARE_DEVICE_REQUEST                                          , // 620
        EVT_STATE_ID_SCR_ADD_HARDWARE_MAX                                        , // 621
    };
    enum EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE
    {
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_MAX+1, // 622
        EVT_SELECT_WALLET_REQUEST                                                            , // 623
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX                                    , // 624
    };
    enum EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN
    {
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MIN=EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX+1, // 625
        EVT_UPDATE_ACTIVATION_DATE_REQUEST                                                                 , // 626
        EVT_UPDATE_MESSAGE_REQUEST                                                                         , // 627
        EVT_UPDATE_BUFFER_PERIOD_REQUEST                                                                   , // 628
        EVT_UPDATE_NOTIFICATION_PREFERENCE_REQ                                                             , // 629
        EVT_NOT_UPDATE_ANY_NOTI_REQ                                                                        , // 630
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX                                                    , // 631
    };
    enum EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS
    {
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MIN=EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX+1, // 632
        EVT_UPDATE_YOUR_SECRET_REQUEST                                                           , // 633
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX                                                  , // 634
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MIN=EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX+1, // 635
        EVT_ADD_EXIST_HARDWARE_REQUEST                                                   , // 636
        EVT_ADD_NEW_HARDWARE_REQUEST                                                     , // 637
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX                                          , // 638
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX+1, // 639
        EVT_DASHBOARD_ALERT_INFO_ENTER                                                     , // 640
        EVT_ADD_HARDWARE_KEY_EXIST_REQ                                                     , // 641
        EVT_ADD_HARDWARE_KEY_REQUEST                                                       , // 642
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX                                          , // 643
    };
    enum EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS
    {
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX+1, // 644
        EVT_KEY_HEALTH_CHECK_STATUS_ENTER                                                       , // 645
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX                                            , // 646
    };
    enum EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING
    {
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MIN=EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX+1, // 647
        EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST                                                    , // 648
        EVT_HEALTH_CHECK_STARTING_CLOSE                                                          , // 649
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX                                               , // 650
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MIN=EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX+1, // 651
        EVT_DASHBOARD_ALERT_SUCCESS_ENTER_REQUEST                                                , // 652
        EVT_DASHBOARD_ALERT_SUCCESS_CLOSE                                                        , // 653
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX                                             , // 654
    };
    enum EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX+1, // 655
        EVT_INPUT_SECURITY_QUESTION_REQUEST                                                        , // 656
        EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ                                                     , // 657
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX                                               , // 658
    };
    enum EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX+1, // 659
        EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ                                                            , // 660
        EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK                                                           , // 661
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX                                               , // 662
    };
    enum EVT_STATE_ID_SCR_RECURRING_PAYMENTS
    {
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MIN=EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX+1, // 663
        EVT_RECURRING_PAYMENTS_ENTER                                                                 , // 664
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX                                                      , // 665
    };
    enum EVT_STATE_ID_SCR_ONBOARDING
    {
        EVT_STATE_ID_SCR_ONBOARDING_MIN=EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX+1, // 666
        EVT_ONBOARDING_ACTION_REQUEST                                            , // 667
        EVT_ONBOARDING_CLOSE                                                     , // 668
        EVT_STATE_ID_SCR_ONBOARDING_MAX                                          , // 669
    };
    enum EVT_STATE_ID_SCR_SELECT_SERVER
    {
        EVT_STATE_ID_SCR_SELECT_SERVER_MIN=EVT_STATE_ID_SCR_ONBOARDING_MAX+1, // 670
        EVT_SELECT_SERVER_ENTER                                             , // 671
        EVT_STATE_ID_SCR_SELECT_SERVER_MAX                                  , // 672
    };
    enum EVT_STATE_ID_SCR_CHANGE_EMAIL
    {
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MIN=EVT_STATE_ID_SCR_SELECT_SERVER_MAX+1, // 673
        EVT_CHANGE_EMAIL_ENTER                                                , // 674
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MAX                                     , // 675
    };
    enum EVT_STATE_ID_SCR_REPLACE_SELECT_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_EMAIL_MAX+1, // 676
        EVT_REPLACE_SELECT_KEY_ENTER                                               , // 677
        EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MAX                                    , // 678
    };
    enum EVT_STATE_ID_SCR_REPLACE_KEYS
    {
        EVT_STATE_ID_SCR_REPLACE_KEYS_MIN=EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MAX+1, // 679
        EVT_REPLACE_KEYS_ENTER                                                     , // 680
        EVT_STATE_ID_SCR_REPLACE_KEYS_MAX                                          , // 681
    };
    enum EVT_STATE_ID_SCR_EDIT_MEMBERS
    {
        EVT_STATE_ID_SCR_EDIT_MEMBERS_MIN=EVT_STATE_ID_SCR_REPLACE_KEYS_MAX+1, // 682
        EVT_EDIT_MEMBERS_ENTER_REQUEST                                       , // 683
        EVT_EDIT_MEMBERS_CLOSE                                               , // 684
        EVT_STATE_ID_SCR_EDIT_MEMBERS_MAX                                    , // 685
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB
    {
        EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB_MIN=EVT_STATE_ID_SCR_EDIT_MEMBERS_MAX+1, // 686
        EVT_SIGN_IN_VIA_XPUB_ENTER                                               , // 687
        EVT_SIGN_IN_VIA_XPUB_CLOSE                                               , // 688
        EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB_MAX                                    , // 689
    };
    enum EVT_STATE_ID_SCR_COIN_DETAILS_CONTROL
    {
        EVT_STATE_ID_SCR_COIN_DETAILS_CONTROL_MIN=EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB_MAX+1, // 690
        EVT_COIN_DETAILS_CONTROL_ENTER                                                   , // 691
        EVT_COIN_DETAILS_CONTROL_CLOSE                                                   , // 692
        EVT_STATE_ID_SCR_COIN_DETAILS_CONTROL_MAX                                        , // 693
    };
    enum EVT_STATE_ID_SCR_SETUP_GROUP_WALLET
    {
        EVT_STATE_ID_SCR_SETUP_GROUP_WALLET_MIN=EVT_STATE_ID_SCR_COIN_DETAILS_CONTROL_MAX+1, // 694
        EVT_SETUP_GROUP_WALLET_ENTER                                                       , // 695
        EVT_SETUP_GROUP_WALLET_BACK                                                        , // 696
        EVT_STATE_ID_SCR_SETUP_GROUP_WALLET_MAX                                            , // 697
    };

};
#endif // VIEWSENUMS_H
