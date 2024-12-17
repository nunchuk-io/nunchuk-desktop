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
        EVT_STATE_ID_ROOT_MAX                               , // 129
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 130
        EVT_HOME_WALLET_SELECTED                         , // 131
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 132
        EVT_HOME_SETTING_REQUEST                         , // 133
        EVT_HOME_DISPLAY_ADDRESS                         , // 134
        EVT_HOME_IMPORT_PSBT                             , // 135
        EVT_HOME_EXPORT_BSMS                             , // 136
        EVT_HOME_ADD_WALLET_REQUEST                      , // 137
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 138
        EVT_HOME_SEND_REQUEST                            , // 139
        EVT_HOME_RECEIVE_REQUEST                         , // 140
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 141
        EVT_HOME_WALLET_INFO_REQUEST                     , // 142
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 143
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 144
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 145
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 146
        EVT_ASK_HARDWARE_REQ                             , // 147
        EVT_EXIST_HARDWARE_REQ                           , // 148
        EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST             , // 149
        EVT_KEY_HEALTH_CHECK_STATUS_REQUEST              , // 150
        EVT_RECURRING_PAYMENTS_REQUEST                   , // 151
        EVT_ADD_HARDWARE_REQUEST                         , // 152
        EVT_ADD_SOFTWARE_SIGNER_RESULT                   , // 153
        EVT_REPLACE_KEYS_REQUEST                         , // 154
        EVT_STATE_ID_SCR_HOME_MAX                        , // 155
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 156
        EVT_ADD_WALLET_IMPORT                                      , // 157
        EVT_ADD_WALLET_BACK_REQUEST                                , // 158
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 159
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 160
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 161
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 162
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 163
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 164
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 165
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 166
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 167
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 168
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 169
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 170
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 171
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 172
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 173
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 174
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 175
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 176
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 177
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 178
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 179
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 180
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 181
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 182
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 183
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 184
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 185
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 186
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 187
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 188
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 189
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 190
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 191
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 192
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 193
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 194
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 195
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 196
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 197
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 198
        EVT_RECEIVE_BACK_REQUEST                                                           , // 199
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 200
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 201
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 202
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 203
        EVT_SEND_BACK_REQUEST                                   , // 204
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 205
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 206
        EVT_STATE_ID_SCR_SEND_MAX                               , // 207
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 208
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 209
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 210
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 211
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 212
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 213
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 214
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 215
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 216
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 217
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 218
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 219
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 220
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 221
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 222
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 223
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 224
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 225
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 226
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 227
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 228
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 229
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 230
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 231
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 232
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 233
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 234
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 235
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 236
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 237
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 238
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 239
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 240
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 241
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 242
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 243
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 244
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 245
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 246
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 247
        EVT_APP_SETTING_BACK_REQUEST                                                , // 248
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 249
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 250
        EVT_SELECT_SERVER_REQUEST                                                   , // 251
        EVT_CHANGE_EMAIL_REQUEST                                                    , // 252
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 253
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 254
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 255
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 256
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 257
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 258
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 259
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 260
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 261
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 262
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 263
        EVT_UTXOS_SORT_REQUEST                                                    , // 264
        EVT_UTXOS_BACK_REQUEST                                                    , // 265
        EVT_UTXOS_ITEM_SELECTED                                                   , // 266
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 267
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 268
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 269
        EVT_WALLET_INFO_EDIT_NAME                                    , // 270
        EVT_WALLET_INFO_REMOVE                                       , // 271
        EVT_WALLET_INFO_EXPORT_DB                                    , // 272
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 273
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 274
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 275
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 276
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 277
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 278
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 279
        EVT_WALLET_INFO_REFRESH_WALLET_REQUEST                       , // 280
        EVT_WALLET_INFO_GAP_LIMIT_REQUEST                            , // 281
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 282
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 283
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 284
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 285
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 286
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 287
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 288
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 289
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 290
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 291
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 292
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 293
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 294
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 295
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 296
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 297
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 298
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 299
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 300
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 301
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 302
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 303
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 304
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 305
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 306
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 307
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 308
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 309
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 310
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 311
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 312
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 313
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 314
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 315
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 316
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 317
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 318
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 319
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 320
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 321
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 322
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 323
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 324
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 325
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 326
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 327
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 328
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 329
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 330
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 331
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 332
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 333
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 334
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 335
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 336
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 337
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 338
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 339
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 340
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 341
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 342
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 343
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 344
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 345
        EVT_INPUT_PIN_SEND_PIN                                           , // 346
        EVT_INPUT_PIN_CLOSE                                              , // 347
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 348
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 349
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 350
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 351
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 352
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 353
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 354
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 355
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 356
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 357
        EVT_CREATE_NEW_SEED                                                               , // 358
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 359
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 360
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 361
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 362
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 363
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 364
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 365
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 366
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 367
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 368
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 369
        EVT_CREATE_NEW_SEED_BACK                                                           , // 370
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 371
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 372
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 373
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 374
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 375
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 376
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 377
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 378
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 379
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 380
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 381
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 382
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 383
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 384
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 385
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 386
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 387
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 388
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 389
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 390
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 391
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 392
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 393
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 394
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 395
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 396
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 397
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 398
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 399
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 400
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 401
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 402
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 403
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 404
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 405
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 406
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 407
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 408
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 409
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 410
        EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ                                 , // 411
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 412
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 413
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 414
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 415
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 416
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 417
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 418
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 419
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 420
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 421
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 422
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 423
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 424
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 425
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 426
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 427
        EVT_PENDING_REQUEST_IGNORE                                                     , // 428
        EVT_PENDING_REQUEST_BACK                                                       , // 429
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 430
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 431
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 432
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 433
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 434
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 435
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 436
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 437
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 438
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 439
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 440
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 441
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 442
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 443
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 444
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 445
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 446
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 447
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 448
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 449
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 450
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 451
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 452
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 453
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 454
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 455
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 456
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 457
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 458
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 459
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 460
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 461
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 462
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 463
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 464
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 465
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 466
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 467
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 468
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 469
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 470
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 471
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 472
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 473
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 474
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 475
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 476
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 477
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 478
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 479
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 480
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 481
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 482
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 483
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 484
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 485
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 486
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 487
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 488
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 489
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 490
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 491
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 492
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 493
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 494
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 495
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 496
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 497
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 498
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 499
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 500
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 501
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 502
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 503
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 504
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 505
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 506
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 507
        EVT_CHANGE_PASSWORD_BACK                                                  , // 508
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 509
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 510
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 511
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 512
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 513
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 514
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 515
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 516
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 517
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 518
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 519
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 520
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 521
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 522
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 523
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 524
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 525
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 526
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 527
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 528
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 529
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 530
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 531
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 532
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 533
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 534
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 535
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 536
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 537
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 538
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 539
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 540
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 541
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 542
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 543
        EVT_UPDATE_PROFILE_REQUEST                                        , // 544
        EVT_UPDATE_PROFILE_BACK                                           , // 545
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 546
    };
    enum EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS
    {
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MIN=EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX+1, // 547
        EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST                                              , // 548
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX                                      , // 549
    };
    enum EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD
    {
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MIN=EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX+1, // 550
        EVT_INPUT_BACKUP_PASSWORD_REQUEST                                                            , // 551
        EVT_ENTER_BACKUP_PASSWORD_BACK                                                               , // 552
        EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST                                                      , // 553
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX                                                   , // 554
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX+1, // 555
        EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST                                                                  , // 556
        EVT_ANSER_SECURITY_QUESTION_BACK                                                                      , // 557
        EVT_ENTER_BACKUP_PASSWORD_RERQUEST                                                                    , // 558
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX                                             , // 559
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX+1, // 560
        EVT_INPUT_TAPSIGNER_SELECT_REQUEST                                                           , // 561
        EVT_ANSER_SECURITY_QUESTION_REQUEST                                                          , // 562
        EVT_STATE_ID_SCR_KEY_RECOVERY_MAX                                                            , // 563
    };
    enum EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD
    {
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_MAX+1, // 564
        EVT_INPUT_PASSWORD_REQUEST                                                    , // 565
        EVT_REENTER_YOUR_PASSWORD_BACK                                                , // 566
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX                                    , // 567
    };
    enum EVT_STATE_ID_SCR_SERVICE_SETTINGS
    {
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MIN=EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX+1, // 568
        EVT_CLAIM_INHERITANCE_CHECK_REQUEST                                               , // 569
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST                                          , // 570
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED                                          , // 571
        EVT_SERVICE_SELECT_WALLET_REQUEST                                                 , // 572
        EVT_INHERITANCE_PLAN_FINALIZE_REQUEST                                             , // 573
        EVT_SERVICE_SUPPORT_REQUEST                                                       , // 574
        EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST                                          , // 575
        EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST                                            , // 576
        EVT_KEY_RECOVERY_REQUEST                                                          , // 577
        EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST                                           , // 578
        EVT_WALLET_CO_SIGN_POLICE_REQUEST                                                 , // 579
        EVT_LOCKDOWN_SUCCESS_REQUEST                                                      , // 580
        EVT_SETUP_SECURITY_QUESTION_REQUEST                                               , // 581
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX                                             , // 582
    };
    enum EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD
    {
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MIN=EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX+1, // 583
        EVT_INPUT_DAYS_VALUE_REQUEST                                                            , // 584
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST                                            , // 585
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX                                        , // 586
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX+1, // 587
        EVT_INPUT_LOCKDOWN_ANSER_REQUEST                                                                        , // 588
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK                                                               , // 589
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX                                                   , // 590
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS
    {
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MIN=EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX+1, // 591
        EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST                                                           , // 592
        EVT_CLOSE_LOCKDOWN_SUCCESS                                                                   , // 593
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX                                                        , // 594
    };
    enum EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX+1, // 595
        EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST                                         , // 596
        EVT_DUMMY_TRANSACTION_INFO_BACK                                                    , // 597
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX                                        , // 598
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION
    {
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MIN=EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX+1, // 599
        EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ                                                           , // 600
        EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST                                                       , // 601
        EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK                                                          , // 602
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX                                              , // 603
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS
    {
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MIN=EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX+1, // 604
        EVT_INHERITANCE_TRANSACTION_DETAILS_BACK                                                                   , // 605
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX                                                       , // 606
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE
    {
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MIN=EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX+1, // 607
        EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST                                                                 , // 608
        EVT_INHERITANCE_NEW_TRANSACTION_REQ                                                                     , // 609
        EVT_INHERITANCE_WITHDRAW_BALANCE_BACK                                                                   , // 610
        EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST                                                             , // 611
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX                                                       , // 612
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_ASK
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MIN=EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX+1, // 613
        EVT_ADD_HARDWARE_ASK_ENTER                                                               , // 614
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX                                                    , // 615
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX+1, // 616
        EVT_SCAN_HARDWARE_DEVICE_REQUEST                                         , // 617
        EVT_ADD_HARDWARE_DEVICE_REQUEST                                          , // 618
        EVT_STATE_ID_SCR_ADD_HARDWARE_MAX                                        , // 619
    };
    enum EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE
    {
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_MAX+1, // 620
        EVT_SELECT_WALLET_REQUEST                                                            , // 621
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX                                    , // 622
    };
    enum EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN
    {
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MIN=EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX+1, // 623
        EVT_UPDATE_ACTIVATION_DATE_REQUEST                                                                 , // 624
        EVT_UPDATE_MESSAGE_REQUEST                                                                         , // 625
        EVT_UPDATE_BUFFER_PERIOD_REQUEST                                                                   , // 626
        EVT_UPDATE_NOTIFICATION_PREFERENCE_REQ                                                             , // 627
        EVT_NOT_UPDATE_ANY_NOTI_REQ                                                                        , // 628
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX                                                    , // 629
    };
    enum EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS
    {
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MIN=EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX+1, // 630
        EVT_UPDATE_YOUR_SECRET_REQUEST                                                           , // 631
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX                                                  , // 632
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MIN=EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX+1, // 633
        EVT_ADD_EXIST_HARDWARE_REQUEST                                                   , // 634
        EVT_ADD_NEW_HARDWARE_REQUEST                                                     , // 635
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX                                          , // 636
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX+1, // 637
        EVT_DASHBOARD_ALERT_INFO_ENTER                                                     , // 638
        EVT_ADD_HARDWARE_KEY_EXIST_REQ                                                     , // 639
        EVT_ADD_HARDWARE_KEY_REQUEST                                                       , // 640
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX                                          , // 641
    };
    enum EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS
    {
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX+1, // 642
        EVT_KEY_HEALTH_CHECK_STATUS_ENTER                                                       , // 643
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX                                            , // 644
    };
    enum EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING
    {
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MIN=EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX+1, // 645
        EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST                                                    , // 646
        EVT_HEALTH_CHECK_STARTING_CLOSE                                                          , // 647
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX                                               , // 648
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MIN=EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX+1, // 649
        EVT_DASHBOARD_ALERT_SUCCESS_ENTER_REQUEST                                                , // 650
        EVT_DASHBOARD_ALERT_SUCCESS_CLOSE                                                        , // 651
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX                                             , // 652
    };
    enum EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX+1, // 653
        EVT_INPUT_SECURITY_QUESTION_REQUEST                                                        , // 654
        EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ                                                     , // 655
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX                                               , // 656
    };
    enum EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX+1, // 657
        EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ                                                            , // 658
        EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK                                                           , // 659
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX                                               , // 660
    };
    enum EVT_STATE_ID_SCR_RECURRING_PAYMENTS
    {
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MIN=EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX+1, // 661
        EVT_RECURRING_PAYMENTS_ENTER                                                                 , // 662
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX                                                      , // 663
    };
    enum EVT_STATE_ID_SCR_ONBOARDING
    {
        EVT_STATE_ID_SCR_ONBOARDING_MIN=EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX+1, // 664
        EVT_ONBOARDING_ACTION_REQUEST                                            , // 665
        EVT_ONBOARDING_CLOSE                                                     , // 666
        EVT_STATE_ID_SCR_ONBOARDING_MAX                                          , // 667
    };
    enum EVT_STATE_ID_SCR_SELECT_SERVER
    {
        EVT_STATE_ID_SCR_SELECT_SERVER_MIN=EVT_STATE_ID_SCR_ONBOARDING_MAX+1, // 668
        EVT_SELECT_SERVER_ENTER                                             , // 669
        EVT_STATE_ID_SCR_SELECT_SERVER_MAX                                  , // 670
    };
    enum EVT_STATE_ID_SCR_CHANGE_EMAIL
    {
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MIN=EVT_STATE_ID_SCR_SELECT_SERVER_MAX+1, // 671
        EVT_CHANGE_EMAIL_ENTER                                                , // 672
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MAX                                     , // 673
    };
    enum EVT_STATE_ID_SCR_REPLACE_SELECT_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_EMAIL_MAX+1, // 674
        EVT_REPLACE_SELECT_KEY_ENTER                                               , // 675
        EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MAX                                    , // 676
    };
    enum EVT_STATE_ID_SCR_REPLACE_KEYS
    {
        EVT_STATE_ID_SCR_REPLACE_KEYS_MIN=EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MAX+1, // 677
        EVT_REPLACE_KEYS_ENTER                                                     , // 678
        EVT_STATE_ID_SCR_REPLACE_KEYS_MAX                                          , // 679
    };
    enum EVT_STATE_ID_SCR_EDIT_MEMBERS
    {
        EVT_STATE_ID_SCR_EDIT_MEMBERS_MIN=EVT_STATE_ID_SCR_REPLACE_KEYS_MAX+1, // 680
        EVT_EDIT_MEMBERS_ENTER_REQUEST                                       , // 681
        EVT_EDIT_MEMBERS_CLOSE                                               , // 682
        EVT_STATE_ID_SCR_EDIT_MEMBERS_MAX                                    , // 683
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB
    {
        EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB_MIN=EVT_STATE_ID_SCR_EDIT_MEMBERS_MAX+1, // 684
        EVT_SIGN_IN_VIA_XPUB_ENTER                                               , // 685
        EVT_SIGN_IN_VIA_XPUB_CLOSE                                               , // 686
        EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB_MAX                                    , // 687
    };
    enum EVT_STATE_ID_SCR_COIN_DETAILS_CONTROL
    {
        EVT_STATE_ID_SCR_COIN_DETAILS_CONTROL_MIN=EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB_MAX+1, // 688
        EVT_COIN_DETAILS_CONTROL_ENTER                                                   , // 689
        EVT_COIN_DETAILS_CONTROL_CLOSE                                                   , // 690
        EVT_STATE_ID_SCR_COIN_DETAILS_CONTROL_MAX                                        , // 691
    };

};
#endif // VIEWSENUMS_H
