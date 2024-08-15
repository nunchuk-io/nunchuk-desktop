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
        EVT_STATE_ID_REGISTED_MAX                        , // 89
    };
    enum EVT_STATE_ID_ROOT
    {
        EVT_STATE_ID_ROOT_MIN=EVT_STATE_ID_REGISTED_MAX+1, // 90
        EVT_STARTING_APPLICATION_LOCALMODE               , // 91
        EVT_STARTING_APPLICATION_ONLINEMODE              , // 92
        EVT_STARTING_APPLICATION_ONLINE_HWL              , // 93
        EVT_SETTING_ACCOUNT_CHANGE_PASSWORD              , // 94
        EVT_ONS_CLOSE_ALL_REQUEST                        , // 95
        EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST             , // 96
        EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST              , // 97
        EVT_GOTO_HOME_WALLET_TAB                         , // 98
        EVT_ONS_CLOSE_REQUEST                            , // 99
        EVT_LOGIN_DB_REQUEST                             , // 100
        EVT_ROOT_PROMT_PIN                               , // 101
        EVT_ROOT_PROMT_PASSPHRASE                        , // 102
        EVT_LOGIN_MATRIX_REQUEST                         , // 103
        EVT_GOTO_HOME_CHAT_TAB                           , // 104
        EVT_ONLINE_ONS_CLOSE_REQUEST                     , // 105
        EVT_SHOW_TOAST_MESSAGE                           , // 106
        EVT_ROOT_UPDATE_PROFILE                          , // 107
        EVT_SHOW_CREATE_ACCOUNT_REQUEST                  , // 108
        EVT_GOTO_APP_SETTINGS_TAB                        , // 109
        EVT_SIGN_IN_REQUEST                              , // 110
        EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST               , // 111
        EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST              , // 112
        EVT_GOTO_SERVICE_SETTING_TAB                     , // 113
        EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST            , // 114
        EVT_NUNCHUK_LOGIN_SUCCEEDED                      , // 115
        EVT_HEALTH_CHECK_STARTING_REQUEST                , // 116
        EVT_DUMMY_TRANSACTION_INFO_REQUEST               , // 117
        EVT_DASHBOARD_ALERT_SUCCESS_REQUEST              , // 118
        EVT_SHARE_YOUR_SECRET_REQUEST                    , // 119
        EVT_REENTER_YOUR_PASSWORD_REQUEST                , // 120
        EVT_ONBOARDING_REQUEST                           , // 121
        EVT_REPLACE_SELECT_KEY_REQUEST                   , // 122
        EVT_EDIT_MEMBERS_REQUEST                         , // 123
        EVT_SIGN_IN_VIA_XPUB_REQUEST                     , // 124
        EVT_STATE_ID_ROOT_MAX                            , // 125
    };
    enum EVT_STATE_ID_SCR_HOME
    {
        EVT_STATE_ID_SCR_HOME_MIN=EVT_STATE_ID_ROOT_MAX+1, // 126
        EVT_HOME_WALLET_SELECTED                         , // 127
        EVT_HOME_WALLET_COPY_ADDRESS                     , // 128
        EVT_HOME_SETTING_REQUEST                         , // 129
        EVT_HOME_DISPLAY_ADDRESS                         , // 130
        EVT_HOME_IMPORT_PSBT                             , // 131
        EVT_HOME_EXPORT_BSMS                             , // 132
        EVT_HOME_ADD_WALLET_REQUEST                      , // 133
        EVT_HOME_MASTER_SIGNER_INFO_REQUEST              , // 134
        EVT_HOME_SEND_REQUEST                            , // 135
        EVT_HOME_RECEIVE_REQUEST                         , // 136
        EVT_HOME_TRANSACTION_HISTORY_REQUEST             , // 137
        EVT_HOME_WALLET_INFO_REQUEST                     , // 138
        EVT_HOME_TRANSACTION_INFO_REQUEST                , // 139
        EVT_HOME_REMOTE_SIGNER_INFO_REQUEST              , // 140
        EVT_HOME_ADD_NEW_SIGNER_REQUEST                  , // 141
        EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST        , // 142
        EVT_ASK_HARDWARE_REQ                             , // 143
        EVT_EXIST_HARDWARE_REQ                           , // 144
        EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST             , // 145
        EVT_KEY_HEALTH_CHECK_STATUS_REQUEST              , // 146
        EVT_RECURRING_PAYMENTS_REQUEST                   , // 147
        EVT_ADD_HARDWARE_REQUEST                         , // 148
        EVT_ADD_SOFTWARE_SIGNER_RESULT                   , // 149
        EVT_REPLACE_KEYS_REQUEST                         , // 150
        EVT_STATE_ID_SCR_HOME_MAX                        , // 151
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET
    {
        EVT_STATE_ID_SCR_ADD_WALLET_MIN=EVT_STATE_ID_SCR_HOME_MAX+1, // 152
        EVT_ADD_WALLET_IMPORT                                      , // 153
        EVT_ADD_WALLET_BACK_REQUEST                                , // 154
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST                , // 155
        EVT_ADD_WALLET_IMPORT_SUCCEED                              , // 156
        EVT_STATE_ID_SCR_ADD_WALLET_MAX                            , // 157
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_WALLET_MAX+1, // 158
        EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST                         , // 159
        EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST                                   , // 160
        EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST                         , // 161
        EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST                                 , // 162
        EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST                                  , // 163
        EVT_ADD_MASTER_SIGNER_RESULT                                              , // 164
        EVT_ADD_REMOTE_SIGNER_RESULT                                              , // 165
        EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST                                      , // 166
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX                                  , // 167
    };
    enum EVT_STATE_ID_SCR_MASTER_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_MAX+1, // 168
        EVT_MASTER_SIGNER_INFO_EDIT_NAME                                                  , // 169
        EVT_MASTER_SIGNER_INFO_HEALTH_CHECK                                               , // 170
        EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST                                             , // 171
        EVT_MASTER_SIGNER_INFO_PROMT_PIN                                                  , // 172
        EVT_MASTER_SIGNER_INFO_SEND_PIN                                                   , // 173
        EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE                                            , // 174
        EVT_MASTER_SIGNER_INFO_GET_XPUBS                                                  , // 175
        EVT_MASTER_SIGNER_INFO_BACK_REQUEST                                               , // 176
        EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO                                           , // 177
        EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX                                           , // 178
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_MASTER_SIGNER_INFO_MAX+1, // 179
        EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER                                                 , // 180
        EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                                                        , // 181
        EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER                                                 , // 182
        EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE                                        , // 183
        EVT_SIGNER_CONFIGURATION_TRY_REVIEW                                                           , // 184
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK                                                      , // 185
        EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW                                                    , // 186
        EVT_ADD_SIGNER_TO_WALLET_REQUEST                                                              , // 187
        EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX                                          , // 188
    };
    enum EVT_STATE_ID_SCR_RECEIVE
    {
        EVT_STATE_ID_SCR_RECEIVE_MIN=EVT_STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_MAX+1, // 189
        EVT_BTN_GEN_NEW_ADDRESS                                                            , // 190
        EVT_RECEIVE_COPY_ADDRESS                                                           , // 191
        EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS                                              , // 192
        EVT_RECEIVE_DISPLAY_ADDRESS                                                        , // 193
        EVT_RECEIVE_ADDRESS_BALANCE                                                        , // 194
        EVT_RECEIVE_BACK_REQUEST                                                           , // 195
        EVT_STATE_ID_SCR_RECEIVE_MAX                                                       , // 196
    };
    enum EVT_STATE_ID_SCR_SEND
    {
        EVT_STATE_ID_SCR_SEND_MIN=EVT_STATE_ID_SCR_RECEIVE_MAX+1, // 197
        EVT_SEND_ADD_DESTINATION_REQUEST                        , // 198
        EVT_SEND_CREATE_TRANSACTION_REQUEST                     , // 199
        EVT_SEND_BACK_REQUEST                                   , // 200
        EVT_SEND_CREATE_TRANSACTION_SUCCEED                     , // 201
        EVT_SEND_BACK_HOME_SHARED_WL                            , // 202
        EVT_STATE_ID_SCR_SEND_MAX                               , // 203
    };
    enum EVT_STATE_ID_SCR_CREATE_TRANSACTION
    {
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MIN=EVT_STATE_ID_SCR_SEND_MAX+1, // 204
        EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX                               , // 205
        EVT_CREATE_TRANSACTION_SAVE_REQUEST                                , // 206
        EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST                           , // 207
        EVT_CREATE_TRANSACTION_SIGN_REQUEST                                , // 208
        EVT_CREATE_TRANSACTION_BACK_REQUEST                                , // 209
        EVT_CREATE_TRANSACTION_SIGN_SUCCEED                                , // 210
        EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE                       , // 211
        EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX                            , // 212
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_CREATE_TRANSACTION_MAX+1, // 213
        EVT_TRANSACTION_SIGN_REQUEST                                                   , // 214
        EVT_TRANSACTION_EXPORT_REQUEST                                                 , // 215
        EVT_TRANSACTION_EXPORT_QRCODE                                                  , // 216
        EVT_TRANSACTION_IMPORT_REQUEST                                                 , // 217
        EVT_TRANSACTION_IMPORT_QRCODE                                                  , // 218
        EVT_TRANSACTION_BROADCAST_REQUEST                                              , // 219
        EVT_TRANSACTION_SET_MEMO_REQUEST                                               , // 220
        EVT_TRANSACTION_SCAN_DEVICE_REQUEST                                            , // 221
        EVT_TRANSACTION_REMOVE_REQUEST                                                 , // 222
        EVT_TRANSACTION_SEND_PIN_REQUEST                                               , // 223
        EVT_TRANSACTION_PROMT_PIN_REQUEST                                              , // 224
        EVT_TRANSACTION_VERIFY_ADDRESS                                                 , // 225
        EVT_TRANSACTION_SEND_PASSPHRASE                                                , // 226
        EVT_TRANSACTION_CANCEL_REQUEST                                                 , // 227
        EVT_TRANSACTION_INFO_BACK_REQUEST                                              , // 228
        EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST                        , // 229
        EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                                         , // 230
        EVT_TRANSACTION_INFO_BACK_UTXO_INFO                                            , // 231
        EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX                                          , // 232
    };
    enum EVT_STATE_ID_SCR_TRANSACTION_HISTORY
    {
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MIN=EVT_STATE_ID_SCR_TRANSACTION_INFO_MAX+1, // 233
        EVT_TRANSACTION_HISTORY_SORT_REQUEST                                            , // 234
        EVT_TRANSACTION_HISTORY_BACK_REQUEST                                            , // 235
        EVT_TRANSACTION_INFO_ITEM_SELECTED                                              , // 236
        EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX                                        , // 237
    };
    enum EVT_STATE_ID_SCR_APP_SETTINGS
    {
        EVT_STATE_ID_SCR_APP_SETTINGS_MIN=EVT_STATE_ID_SCR_TRANSACTION_HISTORY_MAX+1, // 238
        EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST                                      , // 239
        EVT_APP_SETTING_CHANGE_PASSPHRASE                                           , // 240
        EVT_APP_SETTING_REQUEST_RESTART                                             , // 241
        EVT_APP_SETTING_REQUEST_RESCAN                                              , // 242
        EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST                          , // 243
        EVT_APP_SETTING_BACK_REQUEST                                                , // 244
        EVT_APP_SETTING_BACK_TO_ONLINE_MODE                                         , // 245
        EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST                                        , // 246
        EVT_SELECT_SERVER_REQUEST                                                   , // 247
        EVT_CHANGE_EMAIL_REQUEST                                                    , // 248
        EVT_STATE_ID_SCR_APP_SETTINGS_MAX                                           , // 249
    };
    enum EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_APP_SETTINGS_MAX+1, // 250
        EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                                            , // 251
        EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                                               , // 252
        EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                                        , // 253
        EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                                          , // 254
        EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                                               , // 255
        EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                                          , // 256
        EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG                     , // 257
        EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX                                    , // 258
    };
    enum EVT_STATE_ID_SCR_UTXOS
    {
        EVT_STATE_ID_SCR_UTXOS_MIN=EVT_STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_MAX+1, // 259
        EVT_UTXOS_SORT_REQUEST                                                    , // 260
        EVT_UTXOS_BACK_REQUEST                                                    , // 261
        EVT_UTXOS_CONSOLIDATE_REQUEST                                             , // 262
        EVT_UTXOS_ITEM_SELECTED                                                   , // 263
        EVT_UTXO_OUTPUT_BACK_SHARED_WALLET                                        , // 264
        EVT_STATE_ID_SCR_UTXOS_MAX                                                , // 265
    };
    enum EVT_STATE_ID_SCR_WALLET_INFO
    {
        EVT_STATE_ID_SCR_WALLET_INFO_MIN=EVT_STATE_ID_SCR_UTXOS_MAX+1, // 266
        EVT_WALLET_INFO_EDIT_NAME                                    , // 267
        EVT_WALLET_INFO_REMOVE                                       , // 268
        EVT_WALLET_INFO_EXPORT_DB                                    , // 269
        EVT_WALLET_INFO_EXPORT_CSV                                   , // 270
        EVT_WALLET_INFO_EXPORT_DESCRIPTOR                            , // 271
        EVT_WALLET_INFO_EXPORT_COLDCARD                              , // 272
        EVT_WALLET_INFO_EXPORT_QRCODE                                , // 273
        EVT_WALLET_INFO_SIGNER_INFO_REQUEST                          , // 274
        EVT_WALLET_INFO_EDIT_DESCRIPTION                             , // 275
        EVT_WALLET_INFO_IMPORT_PSBT                                  , // 276
        EVT_WALLET_INFO_REFRESH_WALLET_REQUEST                       , // 277
        EVT_WALLET_INFO_GAP_LIMIT_REQUEST                            , // 278
        EVT_WALLET_INFO_BACK_REQUEST                                 , // 279
        EVT_WALLET_INFO_UTXOS_REQUEST                                , // 280
        EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST                       , // 281
        EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST                   , // 282
        EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST                   , // 283
        EVT_STATE_ID_SCR_WALLET_INFO_MAX                             , // 284
    };
    enum EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION
    {
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MIN=EVT_STATE_ID_SCR_WALLET_INFO_MAX+1, // 285
        EVT_ADD_WALLET_CONFIRM_CREATE                                                  , // 286
        EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST                                            , // 287
        EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR                                             , // 288
        EVT_ADD_WALLET_GENERATE_SIGNER                                                 , // 289
        EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR                                           , // 290
        EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR                                          , // 291
        EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST                                       , // 292
        EVT_ADD_WALLET_SUCCESSFULLY                                                    , // 293
        EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX                                   , // 294
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_ADD_WALLET_CONFIRMATION_MAX+1, // 295
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER                                                  , // 296
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER                                                  , // 297
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH                                          , // 298
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN                                                       , // 299
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN                                                      , // 300
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT                                           , // 301
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT                                           , // 302
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                                                           , // 303
        EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX                                               , // 304
    };
    enum EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES
    {
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_MAX+1, // 305
        EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS                                                      , // 306
        EVT_WALLET_CHANGE_ADDRESSES_COPY                                                                 , // 307
        EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST                                                           , // 308
        EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET                                                            , // 309
        EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX                                                     , // 310
    };
    enum EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT
    {
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MIN=EVT_STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_MAX+1, // 311
        EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                                                , // 312
        EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                                                      , // 313
        EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                                                      , // 314
        EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                                                   , // 315
        EVT_ADD_MASTER_SIGNER_FINISHED                                                              , // 316
        EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION                            , // 317
        EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX                                               , // 318
    };
    enum EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO
    {
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MIN=EVT_STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_MAX+1, // 319
        EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK                                                    , // 320
        EVT_REMOTE_SIGNER_INFO_EDIT_NAME                                                       , // 321
        EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE                                                , // 322
        EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE                                                  , // 323
        EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST                                                  , // 324
        EVT_REMOTE_SIGNER_INFO_GET_XPUBS                                                       , // 325
        EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO                                                , // 326
        EVT_REMOTE_SIGNER_INFO_BACK_HOME                                                       , // 327
        EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX                                                , // 328
    };
    enum EVT_STATE_ID_SCR_UNLOCK_DB
    {
        EVT_STATE_ID_SCR_UNLOCK_DB_MIN=EVT_STATE_ID_SCR_REMOTE_SIGNER_INFO_MAX+1, // 329
        EVT_LOGIN_PERFORM_PASSWORD_REQUEST                                      , // 330
        EVT_STATE_ID_SCR_UNLOCK_DB_MAX                                          , // 331
    };
    enum EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT
    {
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MIN=EVT_STATE_ID_SCR_UNLOCK_DB_MAX+1, // 332
        EVT_CONSOLIDATE_MAKE_TRANSACTION                                        , // 333
        EVT_CONSOLIDATE_BACK_REQUEST                                            , // 334
        EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED                                , // 335
        EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX                                 , // 336
    };
    enum EVT_STATE_ID_SCR_UTXO_OUTPUT
    {
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MIN=EVT_STATE_ID_SCR_CONSOLIDATE_OUTPUT_MAX+1, // 337
        EVT_UTXO_INFO_CHECKOUT_TX_RELATED                                         , // 338
        EVT_UTXO_INFO_BACK_REQUEST                                                , // 339
        EVT_UTXO_INFO_VIEW_TX_RELATED                                             , // 340
        EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX                                          , // 341
    };
    enum EVT_STATE_ID_SCR_INPUT_PIN
    {
        EVT_STATE_ID_SCR_INPUT_PIN_MIN=EVT_STATE_ID_SCR_UTXO_OUTPUT_MAX+1, // 342
        EVT_INPUT_PIN_SEND_PIN                                           , // 343
        EVT_INPUT_PIN_CLOSE                                              , // 344
        EVT_STATE_ID_SCR_INPUT_PIN_MAX                                   , // 345
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MIN=EVT_STATE_ID_SCR_INPUT_PIN_MAX+1, // 346
        EVT_ADD_HARDWARE_SIGNER_REQUEST                                     , // 347
        EVT_ADD_NEW_SIGNER_BACK_REQUEST                                     , // 348
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED                         , // 349
        EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED                       , // 350
        EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE                              , // 351
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX                                 , // 352
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_MAX+1, // 353
        EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                                                  , // 354
        EVT_CREATE_NEW_SEED                                                               , // 355
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                                        , // 356
        EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                       , // 357
        EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX                                      , // 358
    };
    enum EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER
    {
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MIN=EVT_STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_MAX+1, // 359
        EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                                                        , // 360
        EVT_RECOVER_SOFTWARE_SIGNER_BACK                                                           , // 361
        EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                                                        , // 362
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                                                 , // 363
        EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION                            , // 364
        EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX                                               , // 365
    };
    enum EVT_STATE_ID_SCR_CREATE_NEW_SEED
    {
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MIN=EVT_STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_MAX+1, // 366
        EVT_CREATE_NEW_SEED_BACK                                                           , // 367
        EVT_CREATE_NEW_SEED_SUCCEED                                                        , // 368
        EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION                                   , // 369
        EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX                                               , // 370
    };
    enum EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION
    {
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MIN=EVT_STATE_ID_SCR_CREATE_NEW_SEED_MAX+1, // 371
        EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                                       , // 372
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                                                   , // 373
        EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION                    , // 374
        EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                                    , // 375
        EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX                                       , // 376
    };
    enum EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET
    {
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MIN=EVT_STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_MAX+1, // 377
        EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST                                                             , // 378
        EVT_ADD_HARDWARE_SIGNER_TO_WALLET                                                                 , // 379
        EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET                                                             , // 380
        EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET                                                             , // 381
        EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX                                                     , // 382
    };
    enum EVT_STATE_ID_SCR_INPUT_PASSPHRASE
    {
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MIN=EVT_STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_MAX+1, // 383
        EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE                                                 , // 384
        EVT_INPUT_PASSPHRASE_CLOSE                                                           , // 385
        EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX                                                , // 386
    };
    enum EVT_STATE_ID_SCR_LOGIN_ONLINE
    {
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MIN=EVT_STATE_ID_SCR_INPUT_PASSPHRASE_MAX+1, // 387
        EVT_LOGIN_ONLINE_CREATE_ACCOUNT                                          , // 388
        EVT_LOGIN_ONLINE_SIGN_IN                                                 , // 389
        EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE                                       , // 390
        EVT_LOGIN_ONLINE_CHANGE_PASSWORD                                         , // 391
        EVT_LOGIN_ONLINE_FORGOT_PASSWORD                                         , // 392
        EVT_LOGIN_ONLINE_RECOVER_PASSWORD                                        , // 393
        EVT_LOGIN_ONLINE_STAY_SIGNED_IN                                          , // 394
        EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE                                       , // 395
        EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE                                     , // 396
        EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX                                        , // 397
    };
    enum EVT_STATE_ID_SCR_HOME_ONLINE
    {
        EVT_STATE_ID_SCR_HOME_ONLINE_MIN=EVT_STATE_ID_SCR_LOGIN_ONLINE_MAX+1, // 398
        EVT_HOME_ONLINE_ADD_SIGNER                                          , // 399
        EVT_HOME_ONLINE_CREATE_CHAT_ROOM                                    , // 400
        EVT_HOME_ONLINE_CREATE_DIRECT_CHAT                                  , // 401
        EVT_HOME_ONLINE_DELETE_DIRECT_ROOM                                  , // 402
        EVT_HOME_ONLINE_DELETE_ROOM                                         , // 403
        EVT_HOME_ONLINE_SEND_CHAT_CONTENT                                   , // 404
        EVT_HOME_ONLINE_CANCEL_SHARED_WL                                    , // 405
        EVT_HOME_ONLINE_CREATE_SHARED_WALLET                                , // 406
        EVT_HOME_ONLINE_CANCEL_TRANSACTION                                  , // 407
        EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ                                 , // 408
        EVT_HOME_ONLINE_ADD_CONTACT                                         , // 409
        EVT_HOME_SHOW_ALL_PENDING_CONTACT                                   , // 410
        EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL                            , // 411
        EVT_HOME_SHARED_WALLET_CONFIGURE                                    , // 412
        EVT_HOME_BACKUP_SHARED_WALLET                                       , // 413
        EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL                          , // 414
        EVT_HOME_SHARED_WL_SEND_REQUEST                                     , // 415
        EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST                            , // 416
        EVT_STATE_ID_SCR_HOME_ONLINE_MAX                                    , // 417
    };
    enum EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS
    {
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MIN=EVT_STATE_ID_SCR_HOME_ONLINE_MAX+1, // 418
        EVT_ONLINE_ADD_CONTACTS_SEARCH_ID                                          , // 419
        EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION                                    , // 420
        EVT_ONLINE_ADD_CONTACTS_BACK                                               , // 421
        EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX                                   , // 422
    };
    enum EVT_STATE_ID_SCR_PENDING_REQUEST
    {
        EVT_STATE_ID_SCR_PENDING_REQUEST_MIN=EVT_STATE_ID_SCR_ONLINE_ADD_CONTACTS_MAX+1, // 423
        EVT_PENDING_REQUEST_ACCEPT                                                     , // 424
        EVT_PENDING_REQUEST_IGNORE                                                     , // 425
        EVT_PENDING_REQUEST_BACK                                                       , // 426
        EVT_STATE_ID_SCR_PENDING_REQUEST_MAX                                           , // 427
    };
    enum EVT_STATE_ID_SCR_CREATE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_PENDING_REQUEST_MAX+1, // 428
        EVT_CREATE_SHARED_WALLET_REQUEST                                                , // 429
        EVT_CANCEL_SHARED_WALLET_REQUEST                                                , // 430
        EVT_CREATE_SHARED_WALLET_BACK                                                   , // 431
        EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION                                  , // 432
        EVT_RECOVER_SHARED_WALLET_REQUEST                                               , // 433
        EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX                                       , // 434
    };
    enum EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CREATE_SHARED_WALLET_MAX+1, // 435
        EVT_CONFIGURE_SHARED_WALLET_REQUEST                                                     , // 436
        EVT_CONFIGURE_SHARED_WALLET_BACK                                                        , // 437
        EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST                                              , // 438
        EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX                                            , // 439
    };
    enum EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_CONFIGURE_SHARED_WALLET_MAX+1, // 440
        EVT_REVIEW_SHARED_WALLET_BACK                                                           , // 441
        EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER                                                  , // 442
        EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX                                               , // 443
    };
    enum EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_REVIEW_SHARED_WALLET_MAX+1, // 444
        EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                                                            , // 445
        EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET                                          , // 446
        EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG                                                 , // 447
        EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX                                            , // 448
    };
    enum EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE
    {
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MIN=EVT_STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_MAX+1, // 449
        EVT_SHARED_WALLET_FINALIZE_WALLET                                                                 , // 450
        EVT_SHARED_WALLET_CONFIGURE_CANCEL                                                                , // 451
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD                                                       , // 452
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE                                                         , // 453
        EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL                                                            , // 454
        EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS                                                           , // 455
        EVT_SHARED_WALLET_CONFIGURE_BACK                                                                  , // 456
        EVT_SHARED_WALLET_ADD_SIGNER_REQUEST                                                              , // 457
        EVT_SHARED_WALLET_UTXO_OUTPUT                                                                     , // 458
        EVT_SHARED_WALLET_CHANGE_ADDRESS                                                                  , // 459
        EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX                                                      , // 460
    };
    enum EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MIN=EVT_STATE_ID_SCR_SHARED_WALLET_CONFIGURE_MAX+1, // 461
        EVT_BACKUP_SHARED_WALLET_REQUEST                                                        , // 462
        EVT_BACKUP_SHARED_WALLET_SKIP                                                           , // 463
        EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS                                                    , // 464
        EVT_BACKUP_SHARED_WALLET_BACK                                                           , // 465
        EVT_BACKUP_WALLET_DEVICE_REGISTRATION                                                   , // 466
        EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX                                               , // 467
    };
    enum EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION
    {
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MIN=EVT_STATE_ID_SCR_BACKUP_SHARED_WALLET_MAX+1, // 468
        EVT_SHARED_WALLET_EXPORT_COLDCARD                                                             , // 469
        EVT_SHARED_WALLET_EXPORT_QRCODE                                                               , // 470
        EVT_SHARED_WALLET_SKIP_REGISTRATION                                                           , // 471
        EVT_SHARED_WL_DEVICE_REGISTRAION_BACK                                                         , // 472
        EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX                                            , // 473
    };
    enum EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER
    {
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MIN=EVT_STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_MAX+1, // 474
        EVT_STARTING_CREATE_SIGNER_DUMMY                                                                , // 475
        EVT_STARTING_CREATE_SIGNER_BACK                                                                 , // 476
        EVT_ONLINE_MODE_ADD_NEW_SIGNER                                                                  , // 477
        EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX                                                     , // 478
    };
    enum EVT_STATE_ID_TOAST_MESSAGE_DISPLAY
    {
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MIN=EVT_STATE_ID_SCR_STARTING_CREATE_SIGNER_MAX+1, // 479
        EVT_CLOSE_TOAST_MESSAGE                                                             , // 480
        EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX                                              , // 481
    };
    enum EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET
    {
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MIN=EVT_STATE_ID_TOAST_MESSAGE_DISPLAY_MAX+1, // 482
        EVT_IMPORT_BSMS_SHARED_WALLET                                                      , // 483
        EVT_IMPORT_QR_SHARED_WALLET                                                        , // 484
        EVT_IMPORT_SHARED_WALLET_SUCCEED                                                   , // 485
        EVT_RECOVER_SHARED_WALLET_BACK                                                     , // 486
        EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX                                         , // 487
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MIN=EVT_STATE_ID_SCR_RECOVER_SHARED_WALLET_MAX+1, // 488
        EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST                                               , // 489
        EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT                                               , // 490
        EVT_PRIMARY_KEY_ACCOUNT_BACK                                                         , // 491
        EVT_ADD_PRIMARY_KEY_REQUEST                                                          , // 492
        EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST                                , // 493
        EVT_SCR_SIGN_IN_MANUALLY_REQUEST                                                     , // 494
        EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                                                      , // 495
        EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX                                             , // 496
    };
    enum EVT_STATE_ID_SCR_CREATE_ACCOUNT
    {
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_MAX+1, // 497
        EVT_CREATE_ACCOUNT_REQUEST                                                    , // 498
        EVT_CREATE_PRIMARY_KEY_REQUEST                                                , // 499
        EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST                                        , // 500
        EVT_CHANGE_PASSWORD_SHOW_REQUEST                                              , // 501
        EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX                                           , // 502
    };
    enum EVT_STATE_ID_SCR_CHANGE_PASSWORD
    {
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MIN=EVT_STATE_ID_SCR_CREATE_ACCOUNT_MAX+1, // 503
        EVT_CHANGE_PASSWORD_REQUEST                                               , // 504
        EVT_CHANGE_PASSWORD_BACK                                                  , // 505
        EVT_CHANGE_PASSWORD_SUCCESS                                               , // 506
        EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX                                      , // 507
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_PASSWORD_MAX+1, // 508
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST                                                        , // 509
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST                                                , // 510
        EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST                                                        , // 511
        EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED                                                        , // 512
        EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK                                                   , // 513
        EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX                                       , // 514
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_MANUALLY
    {
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MIN=EVT_STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_MAX+1, // 515
        EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST                                                              , // 516
        EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST                                                           , // 517
        EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST                                                            , // 518
        EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED                                                              , // 519
        EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX                                                            , // 520
    };
    enum EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY
    {
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MIN=EVT_STATE_ID_SCR_SIGN_IN_MANUALLY_MAX+1, // 521
        EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST                                              , // 522
        EVT_LOGIN_WITH_SOFTWARE_KEY_BACK                                                    , // 523
        EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED                                                 , // 524
        EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX                                        , // 525
    };
    enum EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MIN=EVT_STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_MAX+1, // 526
        EVT_REPLACE_PRIMARY_KEY_REQUEST                                                        , // 527
        EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX                                               , // 528
    };
    enum EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION
    {
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MIN=EVT_STATE_ID_SCR_REPLACE_PRIMARY_KEY_MAX+1, // 529
        EVT_PRIMARY_KEY_SIGN_IN_REQUEST                                                          , // 530
        EVT_PRIMARY_KEY_SIGN_IN_SUCCEED                                                          , // 531
        EVT_PRIMARY_KEY_CONFIGURATION_BACK                                                       , // 532
        EVT_PRIMARY_KEY_CONFIGURATION_FINISHED                                                   , // 533
        EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX                                           , // 534
    };
    enum EVT_STATE_ID_SCR_SIGN_IN
    {
        EVT_STATE_ID_SCR_SIGN_IN_MIN=EVT_STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_MAX+1, // 535
        EVT_SIGN_IN_PASSWORD_REQUEST                                                 , // 536
        EVT_SIGN_IN_PRIMARY_KEY_REQUEST                                              , // 537
        EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST                                         , // 538
        EVT_STATE_ID_SCR_SIGN_IN_MAX                                                 , // 539
    };
    enum EVT_STATE_ID_SCR_UPDATE_PROFILE
    {
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MIN=EVT_STATE_ID_SCR_SIGN_IN_MAX+1, // 540
        EVT_UPDATE_PROFILE_REQUEST                                        , // 541
        EVT_UPDATE_PROFILE_BACK                                           , // 542
        EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX                               , // 543
    };
    enum EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS
    {
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MIN=EVT_STATE_ID_SCR_UPDATE_PROFILE_MAX+1, // 544
        EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST                                              , // 545
        EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX                                      , // 546
    };
    enum EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD
    {
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MIN=EVT_STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_MAX+1, // 547
        EVT_INPUT_BACKUP_PASSWORD_REQUEST                                                            , // 548
        EVT_ENTER_BACKUP_PASSWORD_BACK                                                               , // 549
        EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST                                                      , // 550
        EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX                                                   , // 551
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_ENTER_BACKUP_PASSWORD_MAX+1, // 552
        EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST                                                                  , // 553
        EVT_ANSER_SECURITY_QUESTION_BACK                                                                      , // 554
        EVT_ENTER_BACKUP_PASSWORD_RERQUEST                                                                    , // 555
        EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX                                             , // 556
    };
    enum EVT_STATE_ID_SCR_KEY_RECOVERY
    {
        EVT_STATE_ID_SCR_KEY_RECOVERY_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_MAX+1, // 557
        EVT_INPUT_TAPSIGNER_SELECT_REQUEST                                                           , // 558
        EVT_ANSER_SECURITY_QUESTION_REQUEST                                                          , // 559
        EVT_STATE_ID_SCR_KEY_RECOVERY_MAX                                                            , // 560
    };
    enum EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD
    {
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MIN=EVT_STATE_ID_SCR_KEY_RECOVERY_MAX+1, // 561
        EVT_INPUT_PASSWORD_REQUEST                                                    , // 562
        EVT_REENTER_YOUR_PASSWORD_BACK                                                , // 563
        EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX                                    , // 564
    };
    enum EVT_STATE_ID_SCR_SERVICE_SETTINGS
    {
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MIN=EVT_STATE_ID_SCR_REENTER_YOUR_PASSWORD_MAX+1, // 565
        EVT_CLAIM_INHERITANCE_CHECK_REQUEST                                               , // 566
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST                                          , // 567
        EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED                                          , // 568
        EVT_SERVICE_SELECT_WALLET_REQUEST                                                 , // 569
        EVT_INHERITANCE_PLAN_FINALIZE_REQUEST                                             , // 570
        EVT_SERVICE_SUPPORT_REQUEST                                                       , // 571
        EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST                                          , // 572
        EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST                                            , // 573
        EVT_KEY_RECOVERY_REQUEST                                                          , // 574
        EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST                                           , // 575
        EVT_WALLET_CO_SIGN_POLICE_REQUEST                                                 , // 576
        EVT_LOCKDOWN_SUCCESS_REQUEST                                                      , // 577
        EVT_SETUP_SECURITY_QUESTION_REQUEST                                               , // 578
        EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX                                             , // 579
    };
    enum EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD
    {
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MIN=EVT_STATE_ID_SCR_SERVICE_SETTINGS_MAX+1, // 580
        EVT_INPUT_DAYS_VALUE_REQUEST                                                            , // 581
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST                                            , // 582
        EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX                                        , // 583
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_MAX+1, // 584
        EVT_INPUT_LOCKDOWN_ANSER_REQUEST                                                                        , // 585
        EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK                                                               , // 586
        EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX                                                   , // 587
    };
    enum EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS
    {
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MIN=EVT_STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_MAX+1, // 588
        EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST                                                           , // 589
        EVT_CLOSE_LOCKDOWN_SUCCESS                                                                   , // 590
        EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX                                                        , // 591
    };
    enum EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO
    {
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MIN=EVT_STATE_ID_SCR_LOCKDOWN_SUCCESS_MAX+1, // 592
        EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST                                         , // 593
        EVT_DUMMY_TRANSACTION_INFO_BACK                                                    , // 594
        EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX                                        , // 595
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION
    {
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MIN=EVT_STATE_ID_SCR_DUMMY_TRANSACTION_INFO_MAX+1, // 596
        EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ                                                           , // 597
        EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST                                                       , // 598
        EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK                                                          , // 599
        EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX                                              , // 600
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS
    {
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MIN=EVT_STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_MAX+1, // 601
        EVT_INHERITANCE_TRANSACTION_DETAILS_BACK                                                                   , // 602
        EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX                                                       , // 603
    };
    enum EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE
    {
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MIN=EVT_STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_MAX+1, // 604
        EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST                                                                 , // 605
        EVT_INHERITANCE_NEW_TRANSACTION_REQ                                                                     , // 606
        EVT_INHERITANCE_WITHDRAW_BALANCE_BACK                                                                   , // 607
        EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST                                                             , // 608
        EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX                                                       , // 609
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_ASK
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MIN=EVT_STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_MAX+1, // 610
        EVT_ADD_HARDWARE_ASK_ENTER                                                               , // 611
        EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX                                                    , // 612
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_ASK_MAX+1, // 613
        EVT_SCAN_HARDWARE_DEVICE_REQUEST                                         , // 614
        EVT_ADD_HARDWARE_DEVICE_REQUEST                                          , // 615
        EVT_STATE_ID_SCR_ADD_HARDWARE_MAX                                        , // 616
    };
    enum EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE
    {
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_MAX+1, // 617
        EVT_SELECT_WALLET_REQUEST                                                            , // 618
        EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX                                    , // 619
    };
    enum EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN
    {
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MIN=EVT_STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_MAX+1, // 620
        EVT_UPDATE_ACTIVATION_DATE_REQUEST                                                                 , // 621
        EVT_UPDATE_MESSAGE_REQUEST                                                                         , // 622
        EVT_UPDATE_BUFFER_PERIOD_REQUEST                                                                   , // 623
        EVT_UPDATE_NOTIFICATION_PREFERENCE_REQ                                                             , // 624
        EVT_NOT_UPDATE_ANY_NOTI_REQ                                                                        , // 625
        EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX                                                    , // 626
    };
    enum EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS
    {
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MIN=EVT_STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_MAX+1, // 627
        EVT_UPDATE_YOUR_SECRET_REQUEST                                                           , // 628
        EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX                                                  , // 629
    };
    enum EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST
    {
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MIN=EVT_STATE_ID_SCR_SHARE_YOUR_SECRETS_MAX+1, // 630
        EVT_ADD_EXIST_HARDWARE_REQUEST                                                   , // 631
        EVT_ADD_NEW_HARDWARE_REQUEST                                                     , // 632
        EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX                                          , // 633
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MIN=EVT_STATE_ID_SCR_ADD_HARDWARE_EXIST_MAX+1, // 634
        EVT_DASHBOARD_ALERT_INFO_ENTER                                                     , // 635
        EVT_ADD_HARDWARE_KEY_EXIST_REQ                                                     , // 636
        EVT_ADD_HARDWARE_KEY_REQUEST                                                       , // 637
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX                                          , // 638
    };
    enum EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS
    {
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_INFO_MAX+1, // 639
        EVT_KEY_HEALTH_CHECK_STATUS_ENTER                                                       , // 640
        EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX                                            , // 641
    };
    enum EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING
    {
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MIN=EVT_STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_MAX+1, // 642
        EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST                                                    , // 643
        EVT_HEALTH_CHECK_STARTING_CLOSE                                                          , // 644
        EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX                                               , // 645
    };
    enum EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS
    {
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MIN=EVT_STATE_ID_SCR_HEALTH_CHECK_STARTING_MAX+1, // 646
        EVT_DASHBOARD_ALERT_SUCCESS_ENTER_REQUEST                                                , // 647
        EVT_DASHBOARD_ALERT_SUCCESS_CLOSE                                                        , // 648
        EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX                                             , // 649
    };
    enum EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_MAX+1, // 650
        EVT_INPUT_SECURITY_QUESTION_REQUEST                                                        , // 651
        EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ                                                     , // 652
        EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX                                               , // 653
    };
    enum EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION
    {
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MIN=EVT_STATE_ID_SCR_SETUP_SECURITY_QUESTION_MAX+1, // 654
        EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ                                                            , // 655
        EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK                                                           , // 656
        EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX                                               , // 657
    };
    enum EVT_STATE_ID_SCR_RECURRING_PAYMENTS
    {
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MIN=EVT_STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_MAX+1, // 658
        EVT_RECURRING_PAYMENTS_ENTER                                                                 , // 659
        EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX                                                      , // 660
    };
    enum EVT_STATE_ID_SCR_ONBOARDING
    {
        EVT_STATE_ID_SCR_ONBOARDING_MIN=EVT_STATE_ID_SCR_RECURRING_PAYMENTS_MAX+1, // 661
        EVT_ONBOARDING_ACTION_REQUEST                                            , // 662
        EVT_ONBOARDING_CLOSE                                                     , // 663
        EVT_STATE_ID_SCR_ONBOARDING_MAX                                          , // 664
    };
    enum EVT_STATE_ID_SCR_SELECT_SERVER
    {
        EVT_STATE_ID_SCR_SELECT_SERVER_MIN=EVT_STATE_ID_SCR_ONBOARDING_MAX+1, // 665
        EVT_SELECT_SERVER_ENTER                                             , // 666
        EVT_STATE_ID_SCR_SELECT_SERVER_MAX                                  , // 667
    };
    enum EVT_STATE_ID_SCR_CHANGE_EMAIL
    {
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MIN=EVT_STATE_ID_SCR_SELECT_SERVER_MAX+1, // 668
        EVT_CHANGE_EMAIL_ENTER                                                , // 669
        EVT_STATE_ID_SCR_CHANGE_EMAIL_MAX                                     , // 670
    };
    enum EVT_STATE_ID_SCR_REPLACE_SELECT_KEY
    {
        EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MIN=EVT_STATE_ID_SCR_CHANGE_EMAIL_MAX+1, // 671
        EVT_REPLACE_SELECT_KEY_ENTER                                               , // 672
        EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MAX                                    , // 673
    };
    enum EVT_STATE_ID_SCR_REPLACE_KEYS
    {
        EVT_STATE_ID_SCR_REPLACE_KEYS_MIN=EVT_STATE_ID_SCR_REPLACE_SELECT_KEY_MAX+1, // 674
        EVT_REPLACE_KEYS_ENTER                                                     , // 675
        EVT_STATE_ID_SCR_REPLACE_KEYS_MAX                                          , // 676
    };
    enum EVT_STATE_ID_SCR_EDIT_MEMBERS
    {
        EVT_STATE_ID_SCR_EDIT_MEMBERS_MIN=EVT_STATE_ID_SCR_REPLACE_KEYS_MAX+1, // 677
        EVT_EDIT_MEMBERS_ENTER_REQUEST                                       , // 678
        EVT_EDIT_MEMBERS_CLOSE                                               , // 679
        EVT_STATE_ID_SCR_EDIT_MEMBERS_MAX                                    , // 680
    };
    enum EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB
    {
        EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB_MIN=EVT_STATE_ID_SCR_EDIT_MEMBERS_MAX+1, // 681
        EVT_SIGN_IN_VIA_XPUB_ENTER                                               , // 682
        EVT_SIGN_IN_VIA_XPUB_CLOSE                                               , // 683
        EVT_STATE_ID_SCR_SIGN_IN_VIA_XPUB_MAX                                    , // 684
    };

};
#endif // VIEWSENUMS_H
