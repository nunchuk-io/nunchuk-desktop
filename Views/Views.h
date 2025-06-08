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


#ifndef VIEWS_H
#define VIEWS_H

#include "QEventProcessor.h"
#include "QCommonStructs.h"
#include "ViewStates.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const APPLICATION_STATE STATE_ID_ROOT                                     = {E::STATE_ID_ROOT                                    , ROOT_Entry                                    , ROOT_Exit                                    , LAYER::LAYER_BASE    , LIMIT::NONE , ROOT                                     };
static const APPLICATION_STATE STATE_ID_SCR_HOME                                 = {E::STATE_ID_SCR_HOME                                , SCR_HOME_Entry                                , SCR_HOME_Exit                                , LAYER::LAYER_SCREEN  , LIMIT::NONE , SCR_HOME                                 };
static const APPLICATION_STATE STATE_ID_SCR_ADD_WALLET                           = {E::STATE_ID_SCR_ADD_WALLET                          , SCR_ADD_WALLET_Entry                          , SCR_ADD_WALLET_Exit                          , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ADD_WALLET                           };
static const APPLICATION_STATE STATE_ID_SCR_ADD_HARDWARE_SIGNER                  = {E::STATE_ID_SCR_ADD_HARDWARE_SIGNER                 , SCR_ADD_HARDWARE_SIGNER_Entry                 , SCR_ADD_HARDWARE_SIGNER_Exit                 , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ADD_HARDWARE_SIGNER                  };
static const APPLICATION_STATE STATE_ID_SCR_MASTER_SIGNER_INFO                   = {E::STATE_ID_SCR_MASTER_SIGNER_INFO                  , SCR_MASTER_SIGNER_INFO_Entry                  , SCR_MASTER_SIGNER_INFO_Exit                  , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_MASTER_SIGNER_INFO                   };
static const APPLICATION_STATE STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION      = {E::STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION     , SCR_ADD_WALLET_SIGNER_CONFIGURATION_Entry     , SCR_ADD_WALLET_SIGNER_CONFIGURATION_Exit     , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ADD_WALLET_SIGNER_CONFIGURATION      };
static const APPLICATION_STATE STATE_ID_SCR_RECEIVE                              = {E::STATE_ID_SCR_RECEIVE                             , SCR_RECEIVE_Entry                             , SCR_RECEIVE_Exit                             , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_RECEIVE                              };
static const APPLICATION_STATE STATE_ID_SCR_SEND                                 = {E::STATE_ID_SCR_SEND                                , SCR_SEND_Entry                                , SCR_SEND_Exit                                , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SEND                                 };
static const APPLICATION_STATE STATE_ID_SCR_CREATE_TRANSACTION                   = {E::STATE_ID_SCR_CREATE_TRANSACTION                  , SCR_CREATE_TRANSACTION_Entry                  , SCR_CREATE_TRANSACTION_Exit                  , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_CREATE_TRANSACTION                   };
static const APPLICATION_STATE STATE_ID_SCR_TRANSACTION_INFO                     = {E::STATE_ID_SCR_TRANSACTION_INFO                    , SCR_TRANSACTION_INFO_Entry                    , SCR_TRANSACTION_INFO_Exit                    , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_TRANSACTION_INFO                     };
static const APPLICATION_STATE STATE_ID_SCR_TRANSACTION_HISTORY                  = {E::STATE_ID_SCR_TRANSACTION_HISTORY                 , SCR_TRANSACTION_HISTORY_Entry                 , SCR_TRANSACTION_HISTORY_Exit                 , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_TRANSACTION_HISTORY                  };
static const APPLICATION_STATE STATE_ID_SCR_APP_SETTINGS                         = {E::STATE_ID_SCR_APP_SETTINGS                        , SCR_APP_SETTING_Entry                         , SCR_APP_SETTING_Exit                         , LAYER::LAYER_SCREEN  , LIMIT::NONE , SCR_APP_SETTINGS                         };
static const APPLICATION_STATE STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT             = {E::STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT            , SCR_REMOTE_SIGNER_RESULT_Entry                , SCR_REMOTE_SIGNER_RESULT_Exit                , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ADD_REMOTE_SIGNER_RESULT             };
static const APPLICATION_STATE STATE_ID_SCR_UTXOS                                = {E::STATE_ID_SCR_UTXOS                               , SCR_UTXOS_Entry                               , SCR_UTXOS_Exit                               , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_UTXOS                                };
static const APPLICATION_STATE STATE_ID_SCR_WALLET_INFO                          = {E::STATE_ID_SCR_WALLET_INFO                         , SCR_WALLET_INFO_Entry                         , SCR_WALLET_INFO_Exit                         , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_WALLET_INFO                          };
static const APPLICATION_STATE STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET        = {E::STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET       , SCR_ADD_HARDWARE_SIGNER_TO_WALLET_Entry       , SCR_ADD_HARDWARE_SIGNER_TO_WALLET_Exit       , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ADD_HARDWARE_SIGNER_TO_WALLET        };
static const APPLICATION_STATE STATE_ID_SCR_WALLET_CHANGE_ADDRESSES              = {E::STATE_ID_SCR_WALLET_CHANGE_ADDRESSES             , SCR_WALLET_CHANGE_ADDRESSES_Entry             , SCR_WALLET_CHANGE_ADDRESSES_Exit             , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_WALLET_CHANGE_ADDRESSES              };
static const APPLICATION_STATE STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT             = {E::STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT            , SCR_ADD_MASTER_SIGNER_RESULT_Entry            , SCR_ADD_MASTER_SIGNER_RESULT_Exit            , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ADD_MASTER_SIGNER_RESULT             };
static const APPLICATION_STATE STATE_ID_SCR_REMOTE_SIGNER_INFO                   = {E::STATE_ID_SCR_REMOTE_SIGNER_INFO                  , SCR_REMOTE_SIGNER_INFO_Entry                  , SCR_REMOTE_SIGNER_INFO_Exit                  , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_REMOTE_SIGNER_INFO                   };
static const APPLICATION_STATE STATE_ID_SCR_UNLOCK_DB                            = {E::STATE_ID_SCR_UNLOCK_DB                           , SCR_LOGIN_Entry                               , SCR_LOGIN_Exit                               , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_UNLOCK_DB                            };
static const APPLICATION_STATE STATE_ID_SCR_CONSOLIDATE_OUTPUT                   = {E::STATE_ID_SCR_CONSOLIDATE_OUTPUT                  , SCR_CONSOLIDATE_Entry                         , SCR_CONSOLIDATE_Exit                         , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_CONSOLIDATE_OUTPUT                   };
static const APPLICATION_STATE STATE_ID_SCR_UTXO_OUTPUT                          = {E::STATE_ID_SCR_UTXO_OUTPUT                         , SCR_UTXO_OUTPUT_Entry                         , SCR_UTXO_OUTPUT_Exit                         , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_UTXO_OUTPUT                          };
static const APPLICATION_STATE STATE_ID_SCR_INPUT_PIN                            = {E::STATE_ID_SCR_INPUT_PIN                           , SCR_INPUT_PIN_Entry                           , SCR_INPUT_PIN_Exit                           , LAYER::LAYER_POPUP   , LIMIT::NONE , SCR_INPUT_PIN                            };
static const APPLICATION_STATE STATE_ID_SCR_ADD_NEW_SIGNER                       = {E::STATE_ID_SCR_ADD_NEW_SIGNER                      , SCR_ADD_NEW_SIGNER_Entry                      , SCR_ADD_NEW_SIGNER_Exit                      , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ADD_NEW_SIGNER                       };
static const APPLICATION_STATE STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER              = {E::STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER             , SCR_ADD_NEW_SOFTWARE_SIGNER_Entry             , SCR_ADD_NEW_SOFTWARE_SIGNER_Exit             , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ADD_NEW_SOFTWARE_SIGNER              };
static const APPLICATION_STATE STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER              = {E::STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER             , SCR_RECOVER_SOFTWARE_SIGNER_Entry             , SCR_RECOVER_SOFTWARE_SIGNER_Exit             , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_RECOVER_SOFTWARE_SIGNER              };
static const APPLICATION_STATE STATE_ID_SCR_CREATE_NEW_SEED                      = {E::STATE_ID_SCR_CREATE_NEW_SEED                     , SCR_CREATE_NEW_SEED_Entry                     , SCR_CREATE_NEW_SEED_Exit                     , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_CREATE_NEW_SEED                      };
static const APPLICATION_STATE STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION        = {E::STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION       , SCR_SOFTWARE_SIGNER_CONFIGURATION_Entry       , SCR_SOFTWARE_SIGNER_CONFIGURATION_Exit       , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SOFTWARE_SIGNER_CONFIGURATION        };
static const APPLICATION_STATE STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET             = {E::STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET            , SCR_ADD_NEW_SIGNER_TO_WALLET_Entry            , SCR_ADD_NEW_SIGNER_TO_WALLET_Exit            , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ADD_NEW_SIGNER_TO_WALLET             };
static const APPLICATION_STATE STATE_ID_SCR_INPUT_PASSPHRASE                     = {E::STATE_ID_SCR_INPUT_PASSPHRASE                    , SCR_INPUT_PASSPHRASE_Entry                    , SCR_INPUT_PASSPHRASE_Exit                    , LAYER::LAYER_POPUP   , LIMIT::NONE , SCR_INPUT_PASSPHRASE                     };
static const APPLICATION_STATE STATE_ID_SCR_LOGIN_ONLINE                         = {E::STATE_ID_SCR_LOGIN_ONLINE                        , SCR_LOGIN_ONLINE_Entry                        , SCR_LOGIN_ONLINE_Exit                        , LAYER::LAYER_SCREEN  , LIMIT::NONE , SCR_LOGIN_ONLINE                         };
static const APPLICATION_STATE STATE_ID_SCR_HOME_ONLINE                          = {E::STATE_ID_SCR_HOME_ONLINE                         , SCR_HOME_ONLINE_Entry                         , SCR_HOME_ONLINE_Exit                         , LAYER::LAYER_SCREEN  , LIMIT::NONE , SCR_HOME_ONLINE                          };
static const APPLICATION_STATE STATE_ID_SCR_ONLINE_ADD_CONTACTS                  = {E::STATE_ID_SCR_ONLINE_ADD_CONTACTS                 , SCR_ONLINE_ADD_CONTACTS_Entry                 , SCR_ONLINE_ADD_CONTACTS_Exit                 , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ONLINE_ADD_CONTACTS                  };
static const APPLICATION_STATE STATE_ID_SCR_PENDING_REQUEST                      = {E::STATE_ID_SCR_PENDING_REQUEST                     , SCR_PENDING_REQUEST_Entry                     , SCR_PENDING_REQUEST_Exit                     , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_PENDING_REQUEST                      };
static const APPLICATION_STATE STATE_ID_SCR_CREATE_SHARED_WALLET                 = {E::STATE_ID_SCR_CREATE_SHARED_WALLET                , SCR_CREATE_SHARED_WALLET_Entry                , SCR_CREATE_SHARED_WALLET_Exit                , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_CREATE_SHARED_WALLET                 };
static const APPLICATION_STATE STATE_ID_SCR_CONFIGURE_SHARED_WALLET              = {E::STATE_ID_SCR_CONFIGURE_SHARED_WALLET             , SCR_CONFIGURE_SHARED_WALLET_Entry             , SCR_CONFIGURE_SHARED_WALLET_Exit             , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_CONFIGURE_SHARED_WALLET              };
static const APPLICATION_STATE STATE_ID_SCR_REVIEW_SHARED_WALLET                 = {E::STATE_ID_SCR_REVIEW_SHARED_WALLET                , SCR_REVIEW_SHARED_WALLET_Entry                , SCR_REVIEW_SHARED_WALLET_Exit                , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_REVIEW_SHARED_WALLET                 };
static const APPLICATION_STATE STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET       = {E::STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET      , SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_Entry      , SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_Exit      , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ASSIGN_SIGNER_TO_SHARED_WALLET       };
static const APPLICATION_STATE STATE_ID_SCR_SHARED_WALLET_CONFIGURE              = {E::STATE_ID_SCR_SHARED_WALLET_CONFIGURE             , SCR_SHARED_WALLET_CONFIGURE_Entry             , SCR_SHARED_WALLET_CONFIGURE_Exit             , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SHARED_WALLET_CONFIGURE              };
static const APPLICATION_STATE STATE_ID_SCR_BACKUP_SHARED_WALLET                 = {E::STATE_ID_SCR_BACKUP_SHARED_WALLET                , SCR_BACKUP_SHARED_WALLET_Entry                , SCR_BACKUP_SHARED_WALLET_Exit                , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_BACKUP_SHARED_WALLET                 };
static const APPLICATION_STATE STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION        = {E::STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION       , SCR_SHARED_WL_DEVICE_REGISTRATION_Entry       , SCR_SHARED_WL_DEVICE_REGISTRATION_Exit       , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SHARED_WL_DEVICE_REGISTRATION        };
static const APPLICATION_STATE STATE_ID_SCR_STARTING_CREATE_SIGNER               = {E::STATE_ID_SCR_STARTING_CREATE_SIGNER              , SCR_STARTING_CREATE_SIGNER_Entry              , SCR_STARTING_CREATE_SIGNER_Exit              , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_STARTING_CREATE_SIGNER               };
static const APPLICATION_STATE STATE_ID_TOAST_MESSAGE_DISPLAY                    = {E::STATE_ID_TOAST_MESSAGE_DISPLAY                   , TOAST_MESSAGE_DISPLAY_Entry                   , TOAST_MESSAGE_DISPLAY_Exit                   , LAYER::LAYER_TOAST   , LIMIT::SEC_4, TOAST_MESSAGE_DISPLAY                    };
static const APPLICATION_STATE STATE_ID_SCR_RECOVER_SHARED_WALLET                = {E::STATE_ID_SCR_RECOVER_SHARED_WALLET               , SCR_RECOVER_SHARED_WALLET_Entry               , SCR_RECOVER_SHARED_WALLET_Exit               , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_RECOVER_SHARED_WALLET                };
static const APPLICATION_STATE STATE_ID_SCR_PRIMARY_KEY_ACCOUNT                  = {E::STATE_ID_SCR_PRIMARY_KEY_ACCOUNT                 , SCR_PRIMARY_KEY_ACCOUNT_Entry                 , SCR_PRIMARY_KEY_ACCOUNT_Exit                 , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_PRIMARY_KEY_ACCOUNT                  };
static const APPLICATION_STATE STATE_ID_SCR_CREATE_ACCOUNT                       = {E::STATE_ID_SCR_CREATE_ACCOUNT                      , SCR_CREATE_ACCOUNT_Entry                      , SCR_CREATE_ACCOUNT_Exit                      , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_CREATE_ACCOUNT                       };
static const APPLICATION_STATE STATE_ID_SCR_CHANGE_PASSWORD                      = {E::STATE_ID_SCR_CHANGE_PASSWORD                     , SCR_CHANGE_PASSWORD_Entry                     , SCR_CHANGE_PASSWORD_Exit                     , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_CHANGE_PASSWORD                      };
static const APPLICATION_STATE STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY = {E::STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY, SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_Entry, SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_Exit, LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY };
static const APPLICATION_STATE STATE_ID_SCR_SIGN_IN_MANUALLY                     = {E::STATE_ID_SCR_SIGN_IN_MANUALLY                    , SCR_SIGN_IN_MANUALLY_Entry                    , SCR_SIGN_IN_MANUALLY_Exit                    , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SIGN_IN_MANUALLY                     };
static const APPLICATION_STATE STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY              = {E::STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY             , SCR_LOGIN_WITH_SOFTWARE_KEY_Entry             , SCR_LOGIN_WITH_SOFTWARE_KEY_Exit             , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_LOGIN_WITH_SOFTWARE_KEY              };
static const APPLICATION_STATE STATE_ID_SCR_REPLACE_PRIMARY_KEY                  = {E::STATE_ID_SCR_REPLACE_PRIMARY_KEY                 , SCR_REPLACE_PRIMARY_KEY_Entry                 , SCR_REPLACE_PRIMARY_KEY_Exit                 , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_REPLACE_PRIMARY_KEY                  };
static const APPLICATION_STATE STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION            = {E::STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION           , SCR_PRIMARY_KEY_CONFIGURATION_Entry           , SCR_PRIMARY_KEY_CONFIGURATION_Exit           , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_PRIMARY_KEY_CONFIGURATION            };
static const APPLICATION_STATE STATE_ID_SCR_SIGN_IN                              = {E::STATE_ID_SCR_SIGN_IN                             , SCR_SIGN_IN_Entry                             , SCR_SIGN_IN_Exit                             , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SIGN_IN                              };
static const APPLICATION_STATE STATE_ID_SCR_UPDATE_PROFILE                       = {E::STATE_ID_SCR_UPDATE_PROFILE                      , SCR_UPDATE_PROFILE_Entry                      , SCR_UPDATE_PROFILE_Exit                      , LAYER::LAYER_POPUP   , LIMIT::NONE , SCR_UPDATE_PROFILE                       };
static const APPLICATION_STATE STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS          = {E::STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS         , SCR_TAPSIGNER_RECOVERED_SUCCESS_Entry         , SCR_TAPSIGNER_RECOVERED_SUCCESS_Exit         , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_TAPSIGNER_RECOVERED_SUCCESS          };
static const APPLICATION_STATE STATE_ID_SCR_ENTER_BACKUP_PASSWORD                = {E::STATE_ID_SCR_ENTER_BACKUP_PASSWORD               , SCR_ENTER_BACKUP_PASSWORD_Entry               , SCR_ENTER_BACKUP_PASSWORD_Exit               , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ENTER_BACKUP_PASSWORD                };
static const APPLICATION_STATE STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION = {E::STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION, SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_Entry, SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_Exit, LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION };
static const APPLICATION_STATE STATE_ID_SCR_KEY_RECOVERY                         = {E::STATE_ID_SCR_KEY_RECOVERY                        , SCR_KEY_RECOVERY_Entry                        , SCR_KEY_RECOVERY_Exit                        , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_KEY_RECOVERY                         };
static const APPLICATION_STATE STATE_ID_SCR_REENTER_YOUR_PASSWORD                = {E::STATE_ID_SCR_REENTER_YOUR_PASSWORD               , SCR_REENTER_YOUR_PASSWORD_Entry               , SCR_REENTER_YOUR_PASSWORD_Exit               , LAYER::LAYER_POPUP   , LIMIT::NONE , SCR_REENTER_YOUR_PASSWORD                };
static const APPLICATION_STATE STATE_ID_SCR_SERVICE_SETTINGS                     = {E::STATE_ID_SCR_SERVICE_SETTINGS                    , SCR_SERVICE_SETTINGS_Entry                    , SCR_SERVICE_SETTINGS_Exit                    , LAYER::LAYER_SCREEN  , LIMIT::NONE , SCR_SERVICE_SETTINGS                     };
static const APPLICATION_STATE STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD          = {E::STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD         , SCR_SELECT_YOUR_LOCKDOWN_PERIOD_Entry         , SCR_SELECT_YOUR_LOCKDOWN_PERIOD_Exit         , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SELECT_YOUR_LOCKDOWN_PERIOD          };
static const APPLICATION_STATE STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION     = {E::STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION    , SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_Entry    , SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_Exit    , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_LOCKDOWN_ANSER_SECURITY_QUESTION     };
static const APPLICATION_STATE STATE_ID_SCR_LOCKDOWN_SUCCESS                     = {E::STATE_ID_SCR_LOCKDOWN_SUCCESS                    , SCR_LOCKDOWN_SUCCESS_Entry                    , SCR_LOCKDOWN_SUCCESS_Exit                    , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_LOCKDOWN_SUCCESS                     };
static const APPLICATION_STATE STATE_ID_SCR_DUMMY_TRANSACTION_INFO               = {E::STATE_ID_SCR_DUMMY_TRANSACTION_INFO              , SCR_DUMMY_TRANSACTION_INFO_Entry              , SCR_DUMMY_TRANSACTION_INFO_Exit              , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_DUMMY_TRANSACTION_INFO               };
static const APPLICATION_STATE STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION      = {E::STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION     , SCR_INHERITANCE_CONFIRM_TRANSACTION_Entry     , SCR_INHERITANCE_CONFIRM_TRANSACTION_Exit     , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_INHERITANCE_CONFIRM_TRANSACTION      };
static const APPLICATION_STATE STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS      = {E::STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS     , SCR_INHERITANCE_TRANSACTION_DETAILS_Entry     , SCR_INHERITANCE_TRANSACTION_DETAILS_Exit     , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_INHERITANCE_TRANSACTION_DETAILS      };
static const APPLICATION_STATE STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE         = {E::STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE        , SCR_INHERITANCE_WITHDRAW_BALANCE_Entry        , SCR_INHERITANCE_WITHDRAW_BALANCE_Exit        , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_INHERITANCE_WITHDRAW_BALANCE         };
static const APPLICATION_STATE STATE_ID_SCR_ADD_HARDWARE_ASK                     = {E::STATE_ID_SCR_ADD_HARDWARE_ASK                    , SCR_ADD_HARDWARE_ASK_Entry                    , SCR_ADD_HARDWARE_ASK_Exit                    , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ADD_HARDWARE_ASK                     };
static const APPLICATION_STATE STATE_ID_SCR_ADD_HARDWARE                         = {E::STATE_ID_SCR_ADD_HARDWARE                        , SCR_ADD_HARDWARE_Entry                        , SCR_ADD_HARDWARE_Exit                        , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ADD_HARDWARE                         };
static const APPLICATION_STATE STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE         = {E::STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE        , SCR_SELECT_WALLET_CO_SIGN_POLICE_Entry        , SCR_SELECT_WALLET_CO_SIGN_POLICE_Exit        , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SELECT_WALLET_CO_SIGN_POLICE         };
static const APPLICATION_STATE STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN           = {E::STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN          , SCR_EDIT_YOUR_INHERITANCE_PLAN_Entry          , SCR_EDIT_YOUR_INHERITANCE_PLAN_Exit          , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_EDIT_YOUR_INHERITANCE_PLAN           };
static const APPLICATION_STATE STATE_ID_SCR_SHARE_YOUR_SECRETS                   = {E::STATE_ID_SCR_SHARE_YOUR_SECRETS                  , SCR_SHARE_YOUR_SECRETS_Entry                  , SCR_SHARE_YOUR_SECRETS_Exit                  , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SHARE_YOUR_SECRETS                   };
static const APPLICATION_STATE STATE_ID_SCR_ADD_HARDWARE_EXIST                   = {E::STATE_ID_SCR_ADD_HARDWARE_EXIST                  , SCR_ADD_HARDWARE_EXIST_Entry                  , SCR_ADD_HARDWARE_EXIST_Exit                  , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ADD_HARDWARE_EXIST                   };
static const APPLICATION_STATE STATE_ID_SCR_DASHBOARD_ALERT_INFO                 = {E::STATE_ID_SCR_DASHBOARD_ALERT_INFO                , SCR_DASHBOARD_ALERT_INFO_Entry                , SCR_DASHBOARD_ALERT_INFO_Exit                , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_DASHBOARD_ALERT_INFO                 };
static const APPLICATION_STATE STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS              = {E::STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS             , SCR_KEY_HEALTH_CHECK_STATUS_Entry             , SCR_KEY_HEALTH_CHECK_STATUS_Exit             , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_KEY_HEALTH_CHECK_STATUS              };
static const APPLICATION_STATE STATE_ID_SCR_HEALTH_CHECK_STARTING                = {E::STATE_ID_SCR_HEALTH_CHECK_STARTING               , SCR_HEALTH_CHECK_STARTING_Entry               , SCR_HEALTH_CHECK_STARTING_Exit               , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_HEALTH_CHECK_STARTING                };
static const APPLICATION_STATE STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS              = {E::STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS             , SCR_DASHBOARD_ALERT_SUCCESS_Entry             , SCR_DASHBOARD_ALERT_SUCCESS_Exit             , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_DASHBOARD_ALERT_SUCCESS              };
static const APPLICATION_STATE STATE_ID_SCR_SETUP_SECURITY_QUESTION              = {E::STATE_ID_SCR_SETUP_SECURITY_QUESTION             , SCR_SETUP_SECURITY_QUESTION_Entry             , SCR_SETUP_SECURITY_QUESTION_Exit             , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SETUP_SECURITY_QUESTION              };
static const APPLICATION_STATE STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION       = {E::STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION      , SCR_SETUP_ANSWER_SECURITY_QUESTION_Entry      , SCR_SETUP_ANSWER_SECURITY_QUESTION_Exit      , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SETUP_ANSWER_SECURITY_QUESTION       };
static const APPLICATION_STATE STATE_ID_SCR_RECURRING_PAYMENTS                   = {E::STATE_ID_SCR_RECURRING_PAYMENTS                  , SCR_RECURRING_PAYMENTS_Entry                  , SCR_RECURRING_PAYMENTS_Exit                  , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_RECURRING_PAYMENTS                   };
static const APPLICATION_STATE STATE_ID_SCR_ONBOARDING                           = {E::STATE_ID_SCR_ONBOARDING                          , SCR_ONBOARDING_Entry                          , SCR_ONBOARDING_Exit                          , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ONBOARDING                           };
static const APPLICATION_STATE STATE_ID_SCR_SELECT_SERVER                        = {E::STATE_ID_SCR_SELECT_SERVER                       , SCR_SELECT_SERVER_Entry                       , SCR_SELECT_SERVER_Exit                       , LAYER::LAYER_POPUP   , LIMIT::NONE , SCR_SELECT_SERVER                        };
static const APPLICATION_STATE STATE_ID_SCR_CHANGE_EMAIL                         = {E::STATE_ID_SCR_CHANGE_EMAIL                        , SCR_CHANGE_EMAIL_Entry                        , SCR_CHANGE_EMAIL_Exit                        , LAYER::LAYER_POPUP   , LIMIT::NONE , SCR_CHANGE_EMAIL                         };
static const APPLICATION_STATE STATE_ID_SCR_REPLACE_SELECT_KEY                   = {E::STATE_ID_SCR_REPLACE_SELECT_KEY                  , SCR_REPLACE_SELECT_KEY_Entry                  , SCR_REPLACE_SELECT_KEY_Exit                  , LAYER::LAYER_POPUP   , LIMIT::NONE , SCR_REPLACE_SELECT_KEY                   };
static const APPLICATION_STATE STATE_ID_SCR_REPLACE_KEYS                         = {E::STATE_ID_SCR_REPLACE_KEYS                        , SCR_REPLACE_KEYS_Entry                        , SCR_REPLACE_KEYS_Exit                        , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_REPLACE_KEYS                         };
static const APPLICATION_STATE STATE_ID_SCR_EDIT_MEMBERS                         = {E::STATE_ID_SCR_EDIT_MEMBERS                        , SCR_EDIT_MEMBERS_Entry                        , SCR_EDIT_MEMBERS_Exit                        , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_EDIT_MEMBERS                         };
static const APPLICATION_STATE STATE_ID_SCR_SIGN_IN_VIA_XPUB                     = {E::STATE_ID_SCR_SIGN_IN_VIA_XPUB                    , SCR_SIGN_IN_VIA_XPUB_Entry                    , SCR_SIGN_IN_VIA_XPUB_Exit                    , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SIGN_IN_VIA_XPUB                     };
static const APPLICATION_STATE STATE_ID_SCR_COIN_DETAILS_CONTROL                 = {E::STATE_ID_SCR_COIN_DETAILS_CONTROL                , SCR_COIN_DETAILS_CONTROL_Entry                , SCR_COIN_DETAILS_CONTROL_Exit                , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_COIN_DETAILS_CONTROL                 };
static const APPLICATION_STATE STATE_ID_SCR_SETUP_GROUP_WALLET                   = {E::STATE_ID_SCR_SETUP_GROUP_WALLET                  , SCR_SETUP_GROUP_WALLET_Entry                  , SCR_SETUP_GROUP_WALLET_Exit                  , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_SETUP_GROUP_WALLET                   };
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_ROOT_trigger[42] = 
{
	{E::EVT_STARTING_APPLICATION_LOCALMODE                  , EVT_STARTING_APPLICATION_LOCALMODE_HANDLER                  , NULL                                    },
	{E::EVT_STARTING_APPLICATION_ONLINEMODE                 , EVT_STARTING_APPLICATION_ONLINEMODE_HANDLER                 , NULL                                    },
	{E::EVT_STARTING_APPLICATION_ONLINE_HWL                 , EVT_STARTING_APPLICATION_ONLINE_HWL_HANDLER                 , NULL                                    },
	{E::EVT_SETTING_ACCOUNT_CHANGE_PASSWORD                 , EVT_SETTING_ACCOUNT_CHANGE_PASSWORD_HANDLER                 , NULL                                    },
	{E::EVT_ONS_CLOSE_ALL_REQUEST                           , EVT_ONS_CLOSE_ALL_REQUEST_HANDLER                           , NULL                                    },
	{E::EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST                , EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST_HANDLER                , NULL                                    },
	{E::EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST                 , EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST_HANDLER                 , NULL                                    },
	{E::EVT_GOTO_HOME_WALLET_TAB                            , EVT_GOTO_HOME_WALLET_TAB_HANDLER                            , &STATE_ID_SCR_HOME                      },
	{E::EVT_ONS_CLOSE_REQUEST                               , EVT_ONS_CLOSE_REQUEST_HANDLER                               , &STATE_ID_SCR_HOME                      },
	{E::EVT_LOGIN_DB_REQUEST                                , EVT_LOGIN_DB_REQUEST_HANDLER                                , &STATE_ID_SCR_UNLOCK_DB                 },
	{E::EVT_ROOT_PROMT_PIN                                  , EVT_ROOT_PROMT_PIN_HANDLER                                  , &STATE_ID_SCR_INPUT_PIN                 },
	{E::EVT_ROOT_PROMT_PASSPHRASE                           , EVT_ROOT_PROMT_PASSPHRASE_HANDLER                           , &STATE_ID_SCR_INPUT_PASSPHRASE          },
	{E::EVT_LOGIN_MATRIX_REQUEST                            , EVT_LOGIN_MATRIX_REQUEST_HANDLER                            , &STATE_ID_SCR_LOGIN_ONLINE              },
	{E::EVT_GOTO_HOME_CHAT_TAB                              , EVT_GOTO_HOME_CHAT_TAB_HANDLER                              , &STATE_ID_SCR_HOME_ONLINE               },
	{E::EVT_ONLINE_ONS_CLOSE_REQUEST                        , EVT_ONLINE_ONS_CLOSE_REQUEST_HANDLER                        , &STATE_ID_SCR_HOME_ONLINE               },
	{E::EVT_SHOW_TOAST_MESSAGE                              , EVT_SHOW_TOAST_MESSAGE_HANDLER                              , &STATE_ID_TOAST_MESSAGE_DISPLAY         },
	{E::EVT_ROOT_UPDATE_PROFILE                             , EVT_ROOT_UPDATE_PROFILE_HANDLER                             , &STATE_ID_SCR_UPDATE_PROFILE            },
	{E::EVT_SHOW_CREATE_ACCOUNT_REQUEST                     , EVT_SHOW_CREATE_ACCOUNT_REQUEST_HANDLER                     , &STATE_ID_SCR_CREATE_ACCOUNT            },
	{E::EVT_GOTO_APP_SETTINGS_TAB                           , EVT_GOTO_APP_SETTINGS_TAB_HANDLER                           , &STATE_ID_SCR_APP_SETTINGS              },
	{E::EVT_SIGN_IN_REQUEST                                 , EVT_SIGN_IN_REQUEST_HANDLER                                 , &STATE_ID_SCR_SIGN_IN                   },
	{E::EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST                  , EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST_HANDLER                  , &STATE_ID_SCR_PRIMARY_KEY_ACCOUNT       },
	{E::EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST                 , EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST_HANDLER                 , &STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY   },
	{E::EVT_GOTO_SERVICE_SETTING_TAB                        , EVT_GOTO_SERVICE_SETTING_TAB_HANDLER                        , &STATE_ID_SCR_SERVICE_SETTINGS          },
	{E::EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST               , EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST_HANDLER               , &STATE_ID_SCR_SERVICE_SETTINGS          },
	{E::EVT_NUNCHUK_LOGIN_SUCCEEDED                         , EVT_NUNCHUK_LOGIN_SUCCEEDED_HANDLER                         , &STATE_ID_SCR_HOME                      },
	{E::EVT_HEALTH_CHECK_STARTING_REQUEST                   , EVT_HEALTH_CHECK_STARTING_REQUEST_HANDLER                   , &STATE_ID_SCR_HEALTH_CHECK_STARTING     },
	{E::EVT_DUMMY_TRANSACTION_INFO_REQUEST                  , EVT_DUMMY_TRANSACTION_INFO_REQUEST_HANDLER                  , &STATE_ID_SCR_DUMMY_TRANSACTION_INFO    },
	{E::EVT_DASHBOARD_ALERT_SUCCESS_REQUEST                 , EVT_DASHBOARD_ALERT_SUCCESS_REQUEST_HANDLER                 , &STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS   },
	{E::EVT_SHARE_YOUR_SECRET_REQUEST                       , EVT_SHARE_YOUR_SECRET_REQUEST_HANDLER                       , &STATE_ID_SCR_SHARE_YOUR_SECRETS        },
	{E::EVT_REENTER_YOUR_PASSWORD_REQUEST                   , EVT_REENTER_YOUR_PASSWORD_REQUEST_HANDLER                   , &STATE_ID_SCR_REENTER_YOUR_PASSWORD     },
	{E::EVT_ONBOARDING_REQUEST                              , EVT_ONBOARDING_REQUEST_HANDLER                              , &STATE_ID_SCR_ONBOARDING                },
	{E::EVT_REPLACE_SELECT_KEY_REQUEST                      , EVT_REPLACE_SELECT_KEY_REQUEST_HANDLER                      , &STATE_ID_SCR_REPLACE_SELECT_KEY        },
	{E::EVT_EDIT_MEMBERS_REQUEST                            , EVT_EDIT_MEMBERS_REQUEST_HANDLER                            , &STATE_ID_SCR_EDIT_MEMBERS              },
	{E::EVT_SIGN_IN_VIA_XPUB_REQUEST                        , EVT_SIGN_IN_VIA_XPUB_REQUEST_HANDLER                        , &STATE_ID_SCR_SIGN_IN_VIA_XPUB          },
	{E::EVT_COIN_DETAILS_CONTROL_REQUEST                    , EVT_COIN_DETAILS_CONTROL_REQUEST_HANDLER                    , &STATE_ID_SCR_COIN_DETAILS_CONTROL      },
	{E::EVT_UTXOS_CONSOLIDATE_REQUEST                       , EVT_UTXOS_CONSOLIDATE_REQUEST_HANDLER                       , &STATE_ID_SCR_CONSOLIDATE_OUTPUT        },
	{E::EVT_CONSOLIDATE_COINS_MERGE_MAKE_TRANSACTION_REQUEST, EVT_CONSOLIDATE_COINS_MERGE_MAKE_TRANSACTION_REQUEST_HANDLER, &STATE_ID_SCR_CREATE_TRANSACTION        },
	{E::EVT_SETUP_GROUP_WALLET_REQUEST                      , EVT_SETUP_GROUP_WALLET_REQUEST_HANDLER                      , &STATE_ID_SCR_SETUP_GROUP_WALLET        },
	{E::EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED         , EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED_HANDLER         , &STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER   },
	{E::EVT_PRIMARY_KEY_CONFIGURATION_REQUEST               , EVT_PRIMARY_KEY_CONFIGURATION_REQUEST_HANDLER               , &STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION },
	{E::EVT_HOME_ADD_NEW_SIGNER_REQUEST                     , EVT_HOME_ADD_NEW_SIGNER_REQUEST_HANDLER                     , &STATE_ID_SCR_ADD_NEW_SIGNER            },
	{E::EVT_HOME_ADD_WALLET_REQUEST                         , EVT_HOME_ADD_WALLET_REQUEST_HANDLER                         , &STATE_ID_SCR_ADD_WALLET                },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ADD_HARDWARE_trigger[2] = 
{
	{E::EVT_SCAN_HARDWARE_DEVICE_REQUEST, EVT_SCAN_HARDWARE_DEVICE_REQUEST_HANDLER, NULL  },
	{E::EVT_ADD_HARDWARE_DEVICE_REQUEST , EVT_ADD_HARDWARE_DEVICE_REQUEST_HANDLER , NULL  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ADD_HARDWARE_ASK_trigger[1] = 
{
	{E::EVT_ADD_HARDWARE_ASK_ENTER, EVT_ADD_HARDWARE_ASK_ENTER_HANDLER, NULL  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ADD_HARDWARE_EXIST_trigger[2] = 
{
	{E::EVT_ADD_EXIST_HARDWARE_REQUEST, EVT_ADD_EXIST_HARDWARE_REQUEST_HANDLER, NULL                       },
	{E::EVT_ADD_NEW_HARDWARE_REQUEST  , EVT_ADD_NEW_HARDWARE_REQUEST_HANDLER  , &STATE_ID_SCR_ADD_HARDWARE },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ADD_HARDWARE_SIGNER_trigger[8] = 
{
	{E::EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST, EVT_ADD_HARDWARE_SIGNER_ADD_MASTER_SIGNER_REQUEST_HANDLER, NULL                                   },
	{E::EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST          , EVT_ADD_HARDWARE_SIGNER_REFRESH_REQUEST_HANDLER          , NULL                                   },
	{E::EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST, EVT_ADD_HARDWARE_SIGNER_ADD_REMOTE_SIGNER_REQUEST_HANDLER, NULL                                   },
	{E::EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST        , EVT_ADD_HARDWARE_SIGNER_PROMT_PIN_REQUEST_HANDLER        , NULL                                   },
	{E::EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST         , EVT_ADD_HARDWARE_SIGNER_SEND_PIN_REQUEST_HANDLER         , NULL                                   },
	{E::EVT_ADD_MASTER_SIGNER_RESULT                     , EVT_ADD_MASTER_SIGNER_RESULT_HANDLER                     , &STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT },
	{E::EVT_ADD_REMOTE_SIGNER_RESULT                     , EVT_ADD_REMOTE_SIGNER_RESULT_HANDLER                     , &STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT },
	{E::EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST             , EVT_ADD_HARDWARE_SIGNER_BACK_REQUEST_HANDLER             , &STATE_ID_SCR_ADD_NEW_SIGNER           },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_trigger[8] = 
{
	{E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER        , EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_HANDLER        , NULL                                   },
	{E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER        , EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_HANDLER        , NULL                                   },
	{E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH, EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_REFRESH_HANDLER, NULL                                   },
	{E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN             , EVT_ADD_HARDWARE_SIGNER_TO_WALLET_SEND_PIN_HANDLER             , NULL                                   },
	{E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN            , EVT_ADD_HARDWARE_SIGNER_TO_WALLET_PROMT_PIN_HANDLER            , NULL                                   },
	{E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT , EVT_ADD_HARDWARE_SIGNER_TO_WALLET_REMOTE_SIGNER_RESULT_HANDLER , &STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT },
	{E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT , EVT_ADD_HARDWARE_SIGNER_TO_WALLET_MASTER_SIGNER_RESULT_HANDLER , &STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT },
	{E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK                 , EVT_ADD_HARDWARE_SIGNER_TO_WALLET_BACK_HANDLER                 , &STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_trigger[6] = 
{
	{E::EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK                    , EVT_ADD_MASTER_SIGNER_RESULT_RUN_HEALTHCHECK_HANDLER                    , NULL                                          },
	{E::EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN                          , EVT_ADD_MASTER_SIGNER_RESULT_PROMT_PIN_HANDLER                          , NULL                                          },
	{E::EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS                          , EVT_ADD_MASTER_SIGNER_RESULT_GET_XPUBS_HANDLER                          , NULL                                          },
	{E::EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST                       , EVT_ADD_MASTER_SIGNER_INFO_REMOVE_REQUEST_HANDLER                       , NULL                                          },
	{E::EVT_ADD_MASTER_SIGNER_FINISHED                                  , EVT_ADD_MASTER_SIGNER_FINISHED_HANDLER                                  , &STATE_ID_SCR_HOME                            },
	{E::EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION, EVT_ADD_MASTER_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_CONFIGURATION_HANDLER, &STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ADD_NEW_SIGNER_trigger[4] = 
{
	{E::EVT_ADD_HARDWARE_SIGNER_REQUEST              , EVT_ADD_HARDWARE_SIGNER_REQUEST_HANDLER              , &STATE_ID_SCR_ADD_HARDWARE_SIGNER     },
	{E::EVT_ADD_NEW_SIGNER_BACK_REQUEST              , EVT_ADD_NEW_SIGNER_BACK_REQUEST_HANDLER              , &STATE_ID_SCR_HOME                    },
	{E::EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED, EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_EXIST_SEED_HANDLER, &STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER },
	{E::EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE       , EVT_ADD_NEW_SIGNER_BACK_TO_HOME_ONLINE_HANDLER       , &STATE_ID_SCR_STARTING_CREATE_SIGNER  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_trigger[4] = 
{
	{E::EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST, EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST_HANDLER, &STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION },
	{E::EVT_ADD_HARDWARE_SIGNER_TO_WALLET    , EVT_ADD_HARDWARE_SIGNER_TO_WALLET_HANDLER    , &STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET   },
	{E::EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET, EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_HANDLER, &STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER         },
	{E::EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET, EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_HANDLER, &STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER         },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_trigger[4] = 
{
	{E::EVT_ADD_NEW_SOFTWARE_SIGNER_BACK                           , EVT_ADD_NEW_SOFTWARE_SIGNER_BACK_HANDLER                           , &STATE_ID_SCR_ADD_NEW_SIGNER                  },
	{E::EVT_CREATE_NEW_SEED                                        , EVT_CREATE_NEW_SEED_HANDLER                                        , &STATE_ID_SCR_CREATE_NEW_SEED                 },
	{E::EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK                 , EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK_HANDLER                 , &STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET        },
	{E::EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION, EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION_HANDLER, &STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_trigger[7] = 
{
	{E::EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK                       , EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK_HANDLER                       , NULL                                          },
	{E::EVT_REMOTE_SIGNER_RESULT_EDIT_NAME                          , EVT_REMOTE_SIGNER_RESULT_EDIT_NAME_HANDLER                          , NULL                                          },
	{E::EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE                   , EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE_HANDLER                   , NULL                                          },
	{E::EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE                     , EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE_HANDLER                     , NULL                                          },
	{E::EVT_REMOTE_SIGNER_RESULT_GET_XPUBS                          , EVT_REMOTE_SIGNER_RESULT_GET_XPUBS_HANDLER                          , NULL                                          },
	{E::EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST                     , EVT_REMOTE_SIGNER_RESULT_DELETE_REQUEST_HANDLER                     , NULL                                          },
	{E::EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG, EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG_HANDLER, &STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ADD_WALLET_trigger[4] = 
{
	{E::EVT_ADD_WALLET_IMPORT                      , EVT_ADD_WALLET_IMPORT_HANDLER                      , NULL                                          },
	{E::EVT_ADD_WALLET_BACK_REQUEST                , EVT_ADD_WALLET_BACK_REQUEST_HANDLER                , &STATE_ID_SCR_HOME                            },
	{E::EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST, EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST_HANDLER, &STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION },
	{E::EVT_ADD_WALLET_IMPORT_SUCCEED              , EVT_ADD_WALLET_IMPORT_SUCCEED_HANDLER              , &STATE_ID_SCR_WALLET_INFO                     },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_trigger[7] = 
{
	{E::EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER         , EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER_HANDLER         , NULL                                   },
	{E::EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                , EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER_HANDLER                , NULL                                   },
	{E::EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER         , EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER_HANDLER         , NULL                                   },
	{E::EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE, EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE_HANDLER, NULL                                   },
	{E::EVT_SIGNER_CONFIGURATION_TRY_REVIEW                   , EVT_SIGNER_CONFIGURATION_TRY_REVIEW_HANDLER                   , NULL                                   },
	{E::EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK              , EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK_HANDLER              , &STATE_ID_SCR_ADD_WALLET               },
	{E::EVT_ADD_SIGNER_TO_WALLET_REQUEST                      , EVT_ADD_SIGNER_TO_WALLET_REQUEST_HANDLER                      , &STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_APP_SETTINGS_trigger[10] = 
{
	{E::EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST            , EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST_HANDLER            , NULL                              },
	{E::EVT_APP_SETTING_CHANGE_PASSPHRASE                 , EVT_APP_SETTING_CHANGE_PASSPHRASE_HANDLER                 , NULL                              },
	{E::EVT_APP_SETTING_REQUEST_RESTART                   , EVT_APP_SETTING_REQUEST_RESTART_HANDLER                   , NULL                              },
	{E::EVT_APP_SETTING_REQUEST_RESCAN                    , EVT_APP_SETTING_REQUEST_RESCAN_HANDLER                    , NULL                              },
	{E::EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST, EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST_HANDLER, NULL                              },
	{E::EVT_APP_SETTING_BACK_REQUEST                      , EVT_APP_SETTING_BACK_REQUEST_HANDLER                      , &STATE_ID_SCR_HOME                },
	{E::EVT_APP_SETTING_BACK_TO_ONLINE_MODE               , EVT_APP_SETTING_BACK_TO_ONLINE_MODE_HANDLER               , &STATE_ID_SCR_HOME_ONLINE         },
	{E::EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST              , EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST_HANDLER              , &STATE_ID_SCR_REPLACE_PRIMARY_KEY },
	{E::EVT_SELECT_SERVER_REQUEST                         , EVT_SELECT_SERVER_REQUEST_HANDLER                         , &STATE_ID_SCR_SELECT_SERVER       },
	{E::EVT_CHANGE_EMAIL_REQUEST                          , EVT_CHANGE_EMAIL_REQUEST_HANDLER                          , &STATE_ID_SCR_CHANGE_EMAIL        },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_trigger[3] = 
{
	{E::EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST                  , EVT_ASSIGN_SIGNER_TO_SHARED_REQUEST_HANDLER                  , NULL                                  },
	{E::EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET, EVT_ASSIGN_SIGNER_TO_SHARED_WALLET_BACK_REVIEW_WALLET_HANDLER, &STATE_ID_SCR_REVIEW_SHARED_WALLET    },
	{E::EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG       , EVT_ASSIGN_SIGNER_TO_SHARED_BACK_WALLET_CONFIG_HANDLER       , &STATE_ID_SCR_SHARED_WALLET_CONFIGURE },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_BACKUP_SHARED_WALLET_trigger[5] = 
{
	{E::EVT_BACKUP_SHARED_WALLET_REQUEST     , EVT_BACKUP_SHARED_WALLET_REQUEST_HANDLER     , NULL                                        },
	{E::EVT_BACKUP_SHARED_WALLET_SKIP        , EVT_BACKUP_SHARED_WALLET_SKIP_HANDLER        , NULL                                        },
	{E::EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS , EVT_BACKUP_SHARED_WALLET_EXPORT_BSMS_HANDLER , NULL                                        },
	{E::EVT_BACKUP_SHARED_WALLET_BACK        , EVT_BACKUP_SHARED_WALLET_BACK_HANDLER        , &STATE_ID_SCR_HOME_ONLINE                   },
	{E::EVT_BACKUP_WALLET_DEVICE_REGISTRATION, EVT_BACKUP_WALLET_DEVICE_REGISTRATION_HANDLER, &STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_CHANGE_EMAIL_trigger[1] = 
{
	{E::EVT_CHANGE_EMAIL_ENTER, EVT_CHANGE_EMAIL_ENTER_HANDLER, NULL  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_CHANGE_PASSWORD_trigger[3] = 
{
	{E::EVT_CHANGE_PASSWORD_REQUEST, EVT_CHANGE_PASSWORD_REQUEST_HANDLER, NULL                         },
	{E::EVT_CHANGE_PASSWORD_BACK   , EVT_CHANGE_PASSWORD_BACK_HANDLER   , &STATE_ID_SCR_CREATE_ACCOUNT },
	{E::EVT_CHANGE_PASSWORD_SUCCESS, EVT_CHANGE_PASSWORD_SUCCESS_HANDLED, &STATE_ID_ROOT               },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_COIN_DETAILS_CONTROL_trigger[2] = 
{
	{E::EVT_COIN_DETAILS_CONTROL_ENTER, EVT_COIN_DETAILS_CONTROL_ENTER_HANDLER, NULL           },
	{E::EVT_COIN_DETAILS_CONTROL_CLOSE, EVT_COIN_DETAILS_CONTROL_CLOSE_HANDLER, &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_CONFIGURE_SHARED_WALLET_trigger[3] = 
{
	{E::EVT_CONFIGURE_SHARED_WALLET_REQUEST       , EVT_CONFIGURE_SHARED_WALLET_REQUEST_HANDLER       , NULL                               },
	{E::EVT_CONFIGURE_SHARED_WALLET_BACK          , EVT_CONFIGURE_SHARED_WALLET_BACK_HANDLER          , &STATE_ID_SCR_CREATE_SHARED_WALLET },
	{E::EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST, EVT_CONFIGURE_SHARED_WALLET_REVIEW_REQUEST_HANDLER, &STATE_ID_SCR_REVIEW_SHARED_WALLET },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_CONSOLIDATE_OUTPUT_trigger[3] = 
{
	{E::EVT_CONSOLIDATE_MAKE_TRANSACTION        , EVT_CONSOLIDATE_MAKE_TRANSACTION_HANDLER        , NULL                             },
	{E::EVT_CONSOLIDATE_BACK_REQUEST            , EVT_CONSOLIDATE_BACK_REQUEST_HANDLER            , &STATE_ID_SCR_UTXOS              },
	{E::EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED, EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED_HANDLER, &STATE_ID_SCR_CREATE_TRANSACTION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_CREATE_ACCOUNT_trigger[4] = 
{
	{E::EVT_CREATE_ACCOUNT_REQUEST            , EVT_CREATE_ACCOUNT_REQUEST_HANDLER            , NULL                              },
	{E::EVT_CREATE_PRIMARY_KEY_REQUEST        , EVT_CREATE_PRIMARY_KEY_REQUEST_HANDLER        , NULL                              },
	{E::EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST, EVT_CREATE_ACCOUNT_PRIMARY_KEY_REQUEST_HANDLER, &STATE_ID_SCR_PRIMARY_KEY_ACCOUNT },
	{E::EVT_CHANGE_PASSWORD_SHOW_REQUEST      , EVT_CHANGE_PASSWORD_SHOW_REQUEST_HANDLER      , &STATE_ID_SCR_CHANGE_PASSWORD     },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_CREATE_NEW_SEED_trigger[3] = 
{
	{E::EVT_CREATE_NEW_SEED_BACK                        , EVT_CREATE_NEW_SEED_BACK_HANDLER                        , &STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER         },
	{E::EVT_CREATE_NEW_SEED_SUCCEED                     , EVT_CREATE_NEW_SEED_SUCCEED_HANDLER                     , &STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION   },
	{E::EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION, EVT_NEW_SEED_BACK_TO_WALLET_SIGNER_CONFIGURATION_HANDLER, &STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_CREATE_SHARED_WALLET_trigger[5] = 
{
	{E::EVT_CREATE_SHARED_WALLET_REQUEST              , EVT_CREATE_SHARED_WALLET_REQUEST_HANDLER              , NULL                                  },
	{E::EVT_CANCEL_SHARED_WALLET_REQUEST              , EVT_CANCEL_SHARED_WALLET_REQUEST_HANDLER              , NULL                                  },
	{E::EVT_CREATE_SHARED_WALLET_BACK                 , EVT_CREATE_SHARED_WALLET_BACK_HANDLER                 , &STATE_ID_SCR_HOME_ONLINE             },
	{E::EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION, EVT_CREATE_SHARED_WALLET_REQUEST_CONFIGURATION_HANDLER, &STATE_ID_SCR_CONFIGURE_SHARED_WALLET },
	{E::EVT_RECOVER_SHARED_WALLET_REQUEST             , EVT_RECOVER_SHARED_WALLET_REQUEST_HANDLER             , &STATE_ID_SCR_RECOVER_SHARED_WALLET   },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_CREATE_TRANSACTION_trigger[7] = 
{
	{E::EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX        , EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX_HANDLER        , NULL                             },
	{E::EVT_CREATE_TRANSACTION_SAVE_REQUEST         , EVT_CREATE_TRANSACTION_SAVE_REQUEST_HANDLER         , NULL                             },
	{E::EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST    , EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST_HANDLER    , NULL                             },
	{E::EVT_CREATE_TRANSACTION_SIGN_REQUEST         , EVT_CREATE_TRANSACTION_SIGN_REQUEST_HANDLER         , NULL                             },
	{E::EVT_CREATE_TRANSACTION_BACK_REQUEST         , EVT_CREATE_TRANSACTION_BACK_REQUEST_HANDLER         , &STATE_ID_SCR_SEND               },
	{E::EVT_CREATE_TRANSACTION_SIGN_SUCCEED         , EVT_CREATE_TRANSACTION_SIGN_SUCCEED_HANDLER         , &STATE_ID_SCR_TRANSACTION_INFO   },
	{E::EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE, EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE_HANDLER, &STATE_ID_SCR_CONSOLIDATE_OUTPUT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_DASHBOARD_ALERT_INFO_trigger[3] = 
{
	{E::EVT_DASHBOARD_ALERT_INFO_ENTER, EVT_DASHBOARD_ALERT_INFO_ENTER_HANDLER, NULL                             },
	{E::EVT_ADD_HARDWARE_KEY_EXIST_REQ, EVT_ADD_HARDWARE_KEY_EXIST_REQ_HANDLER, &STATE_ID_SCR_ADD_HARDWARE_EXIST },
	{E::EVT_ADD_HARDWARE_KEY_REQUEST  , EVT_ADD_HARDWARE_KEY_REQUEST_HANDLER  , &STATE_ID_SCR_ADD_HARDWARE       },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_trigger[2] = 
{
	{E::EVT_DASHBOARD_ALERT_SUCCESS_ENTER_REQUEST, EVT_DASHBOARD_ALERT_SUCCESS_ENTER_REQUEST_HANDLER, NULL           },
	{E::EVT_DASHBOARD_ALERT_SUCCESS_CLOSE        , EVT_DASHBOARD_ALERT_SUCCESS_CLOSE_HANDLER        , &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_DUMMY_TRANSACTION_INFO_trigger[2] = 
{
	{E::EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST, EVT_DUMMY_TRANSACTION_ACTION_ENTER_REQUEST_HANDLER, NULL           },
	{E::EVT_DUMMY_TRANSACTION_INFO_BACK           , EVT_DUMMY_TRANSACTION_INFO_BACK_HANDLER           , &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_EDIT_MEMBERS_trigger[2] = 
{
	{E::EVT_EDIT_MEMBERS_ENTER_REQUEST, EVT_EDIT_MEMBERS_ENTER_REQUEST_HANDLER, NULL           },
	{E::EVT_EDIT_MEMBERS_CLOSE        , EVT_EDIT_MEMBERS_CLOSE_HANDLER        , &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_trigger[5] = 
{
	{E::EVT_UPDATE_ACTIVATION_DATE_REQUEST    , EVT_UPDATE_ACTIVATION_DATE_REQUEST_HANDLER    , NULL  },
	{E::EVT_UPDATE_MESSAGE_REQUEST            , EVT_UPDATE_MESSAGE_REQUEST_HANDLER            , NULL  },
	{E::EVT_UPDATE_BUFFER_PERIOD_REQUEST      , EVT_UPDATE_BUFFER_PERIOD_REQUEST_HANDLER      , NULL  },
	{E::EVT_UPDATE_NOTIFICATION_PREFERENCE_REQ, EVT_UPDATE_NOTIFICATION_PREFERENCE_REQ_HANDLER, NULL  },
	{E::EVT_NOT_UPDATE_ANY_NOTI_REQ           , EVT_NOT_UPDATE_ANY_NOTI_REQ_HANDLER           , NULL  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ENTER_BACKUP_PASSWORD_trigger[3] = 
{
	{E::EVT_INPUT_BACKUP_PASSWORD_REQUEST      , EVT_INPUT_BACKUP_PASSWORD_REQUEST_HANDLER      , NULL                                               },
	{E::EVT_ENTER_BACKUP_PASSWORD_BACK         , EVT_ENTER_BACKUP_PASSWORD_BACK_HANDLER         , &STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION },
	{E::EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST, EVT_TAPSIGNER_RECOVERED_SUCCESS_REQUEST_HANDLER, &STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS          },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_HEALTH_CHECK_STARTING_trigger[2] = 
{
	{E::EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST, EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST_HANDLER, NULL           },
	{E::EVT_HEALTH_CHECK_STARTING_CLOSE      , EVT_HEALTH_CHECK_STARTING_CLOSE_HANDLER      , &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_HOME_trigger[22] = 
{
	{E::EVT_HOME_WALLET_SELECTED                 , EVT_HOME_WALLET_SELECTED_HANDLER                 , NULL                                   },
	{E::EVT_HOME_WALLET_COPY_ADDRESS             , EVT_HOME_WALLET_COPY_ADDRESS_HANDLER             , NULL                                   },
	{E::EVT_HOME_SETTING_REQUEST                 , EVT_HOME_SETTING_REQUEST_HANDLER                 , NULL                                   },
	{E::EVT_HOME_DISPLAY_ADDRESS                 , EVT_HOME_DISPLAY_ADDRESS_HANDLER                 , NULL                                   },
	{E::EVT_HOME_IMPORT_PSBT                     , EVT_HOME_IMPORT_PSBT_HANDLER                     , NULL                                   },
	{E::EVT_HOME_EXPORT_BSMS                     , EVT_HOME_EXPORT_BSMS_HANDLER                     , NULL                                   },
	{E::EVT_HOME_MASTER_SIGNER_INFO_REQUEST      , EVT_HOME_MASTER_SIGNER_INFO_REQUEST_HANDLER      , &STATE_ID_SCR_MASTER_SIGNER_INFO       },
	{E::EVT_HOME_SEND_REQUEST                    , EVT_HOME_SEND_REQUEST_HANDLER                    , &STATE_ID_SCR_SEND                     },
	{E::EVT_HOME_RECEIVE_REQUEST                 , EVT_HOME_RECEIVE_REQUEST_HANDLER                 , &STATE_ID_SCR_RECEIVE                  },
	{E::EVT_HOME_TRANSACTION_HISTORY_REQUEST     , EVT_HOME_TRANSACTION_HISTORY_REQUEST_HANDLER     , &STATE_ID_SCR_TRANSACTION_HISTORY      },
	{E::EVT_HOME_WALLET_INFO_REQUEST             , EVT_HOME_WALLET_INFO_REQUEST_HANDLER             , &STATE_ID_SCR_WALLET_INFO              },
	{E::EVT_HOME_TRANSACTION_INFO_REQUEST        , EVT_HOME_TRANSACTION_INFO_REQUEST_HANDLER        , &STATE_ID_SCR_TRANSACTION_INFO         },
	{E::EVT_HOME_REMOTE_SIGNER_INFO_REQUEST      , EVT_HOME_REMOTE_SIGNER_INFO_REQUEST_HANDLER      , &STATE_ID_SCR_REMOTE_SIGNER_INFO       },
	{E::EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST, EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST_HANDLER, &STATE_ID_SCR_MASTER_SIGNER_INFO       },
	{E::EVT_ASK_HARDWARE_REQ                     , EVT_ASK_HARDWARE_REQ_HANDLER                     , &STATE_ID_SCR_ADD_HARDWARE_ASK         },
	{E::EVT_EXIST_HARDWARE_REQ                   , EVT_EXIST_HARDWARE_REQ_HANDLER                   , &STATE_ID_SCR_ADD_HARDWARE_EXIST       },
	{E::EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST     , EVT_SHOW_GROUP_WALLET_CONFIG_REQUEST_HANDLER     , &STATE_ID_SCR_DASHBOARD_ALERT_INFO     },
	{E::EVT_KEY_HEALTH_CHECK_STATUS_REQUEST      , EVT_KEY_HEALTH_CHECK_STATUS_REQUEST_HANDLER      , &STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS  },
	{E::EVT_RECURRING_PAYMENTS_REQUEST           , EVT_RECURRING_PAYMENTS_REQUEST_HANDLER           , &STATE_ID_SCR_RECURRING_PAYMENTS       },
	{E::EVT_ADD_HARDWARE_REQUEST                 , EVT_ADD_HARDWARE_REQUEST_HANDLER                 , &STATE_ID_SCR_ADD_HARDWARE             },
	{E::EVT_ADD_SOFTWARE_SIGNER_RESULT           , EVT_ADD_SOFTWARE_SIGNER_RESULT_HANDLER           , &STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT },
	{E::EVT_REPLACE_KEYS_REQUEST                 , EVT_REPLACE_KEYS_REQUEST_HANDLER                 , &STATE_ID_SCR_REPLACE_KEYS             },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_HOME_ONLINE_trigger[18] = 
{
	{E::EVT_HOME_ONLINE_ADD_SIGNER                , EVT_HOME_ONLINE_ADD_SIGNER_HANDLER                , NULL                                  },
	{E::EVT_HOME_ONLINE_CREATE_CHAT_ROOM          , EVT_HOME_ONLINE_CREATE_CHAT_ROOM_HANDLER          , NULL                                  },
	{E::EVT_HOME_ONLINE_CREATE_DIRECT_CHAT        , EVT_HOME_ONLINE_CREATE_DIRECT_CHAT_HANDLER        , NULL                                  },
	{E::EVT_HOME_ONLINE_DELETE_DIRECT_ROOM        , EVT_HOME_ONLINE_DELETE_DIRECT_ROOM_HANDLER        , NULL                                  },
	{E::EVT_HOME_ONLINE_DELETE_ROOM               , EVT_HOME_ONLINE_DELETE_ROOM_HANDLER               , NULL                                  },
	{E::EVT_HOME_ONLINE_SEND_CHAT_CONTENT         , EVT_HOME_ONLINE_SEND_CHAT_CONTENT_HANDLER         , NULL                                  },
	{E::EVT_HOME_ONLINE_CANCEL_SHARED_WL          , EVT_HOME_ONLINE_CANCEL_SHARED_WL_HANDLER          , NULL                                  },
	{E::EVT_HOME_ONLINE_CREATE_SHARED_WALLET      , EVT_HOME_ONLINE_CREATE_SHARED_WALLET_HANDLER      , NULL                                  },
	{E::EVT_HOME_ONLINE_CANCEL_TRANSACTION        , EVT_HOME_ONLINE_CANCEL_TRANSACTION_HANDLER        , NULL                                  },
	{E::EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ       , EVT_HOME_ONLINE_SERVICE_SUPPORT_REQ_HANDLER       , NULL                                  },
	{E::EVT_HOME_ONLINE_ADD_CONTACT               , EVT_HOME_ONLINE_ADD_CONTACT_HANDLER               , &STATE_ID_SCR_ONLINE_ADD_CONTACTS     },
	{E::EVT_HOME_SHOW_ALL_PENDING_CONTACT         , EVT_HOME_SHOW_ALL_PENDING_CONTACT_HANDLER         , &STATE_ID_SCR_PENDING_REQUEST         },
	{E::EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL  , EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL_HANDLER  , &STATE_ID_SCR_CREATE_SHARED_WALLET    },
	{E::EVT_HOME_SHARED_WALLET_CONFIGURE          , EVT_HOME_SHARED_WALLET_CONFIGURE_HANDLER          , &STATE_ID_SCR_SHARED_WALLET_CONFIGURE },
	{E::EVT_HOME_BACKUP_SHARED_WALLET             , EVT_HOME_BACKUP_SHARED_WALLET_HANDLER             , &STATE_ID_SCR_BACKUP_SHARED_WALLET    },
	{E::EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL, EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL_HANDLER, &STATE_ID_SCR_STARTING_CREATE_SIGNER  },
	{E::EVT_HOME_SHARED_WL_SEND_REQUEST           , EVT_HOME_SHARED_WL_SEND_REQUEST_HANDLER           , &STATE_ID_SCR_SEND                    },
	{E::EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST  , EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST_HANDLER  , &STATE_ID_SCR_TRANSACTION_INFO        },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_trigger[3] = 
{
	{E::EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ    , EVT_INHERITANCE_CREATE_DRAFT_TX_FEE_REQ_HANDLER    , NULL                                          },
	{E::EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST, EVT_INHERITANCE_TRANSACTION_DETAILS_REQUEST_HANDLER, &STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS },
	{E::EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK   , EVT_INHERITANCE_CONFIRM_TRANSACTION_BACK_HANDLER   , &STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE    },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_trigger[1] = 
{
	{E::EVT_INHERITANCE_TRANSACTION_DETAILS_BACK, EVT_INHERITANCE_TRANSACTION_DETAILS_BACK_HANDLER, &STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_trigger[4] = 
{
	{E::EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST    , EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST_HANDLER    , NULL                                          },
	{E::EVT_INHERITANCE_NEW_TRANSACTION_REQ        , EVT_INHERITANCE_NEW_TRANSACTION_REQ_HANDLER        , NULL                                          },
	{E::EVT_INHERITANCE_WITHDRAW_BALANCE_BACK      , EVT_INHERITANCE_WITHDRAW_BALANCE_BACK_HANDLER      , &STATE_ID_SCR_SERVICE_SETTINGS                },
	{E::EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST, EVT_INHERITANCE_CONFIRM_TRANSACTION_REQUEST_HANDLER, &STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_INPUT_PASSPHRASE_trigger[2] = 
{
	{E::EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE, EVT_INPUT_PASSPHRASE_SEND_PASSPHRASE_HANDLER, NULL           },
	{E::EVT_INPUT_PASSPHRASE_CLOSE          , EVT_INPUT_PASSPHRASE_CLOSE_HANDLER          , &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_INPUT_PIN_trigger[2] = 
{
	{E::EVT_INPUT_PIN_SEND_PIN, EVT_INPUT_PIN_SEND_PIN_HANDLER, NULL           },
	{E::EVT_INPUT_PIN_CLOSE   , EVT_INPUT_PIN_CLOSE_HANDLER   , &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_trigger[1] = 
{
	{E::EVT_KEY_HEALTH_CHECK_STATUS_ENTER, EVT_KEY_HEALTH_CHECK_STATUS_ENTER_HANDLER, NULL  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_KEY_RECOVERY_trigger[2] = 
{
	{E::EVT_INPUT_TAPSIGNER_SELECT_REQUEST , EVT_INPUT_TAPSIGNER_SELECT_REQUEST_HANDLER , NULL                                               },
	{E::EVT_ANSER_SECURITY_QUESTION_REQUEST, EVT_ANSER_SECURITY_QUESTION_REQUEST_HANDLER, &STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_trigger[3] = 
{
	{E::EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST, EVT_INPUT_KEY_RECOVERY_ANSER_REQUEST_HANDLER, NULL                                },
	{E::EVT_ANSER_SECURITY_QUESTION_BACK    , EVT_ANSER_SECURITY_QUESTION_BACK_HANDLER    , &STATE_ID_SCR_KEY_RECOVERY          },
	{E::EVT_ENTER_BACKUP_PASSWORD_RERQUEST  , EVT_ENTER_BACKUP_PASSWORD_RERQUEST_HANDLER  , &STATE_ID_SCR_ENTER_BACKUP_PASSWORD },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_trigger[2] = 
{
	{E::EVT_INPUT_LOCKDOWN_ANSER_REQUEST         , EVT_INPUT_LOCKDOWN_ANSER_REQUEST_HANDLER         , NULL                                      },
	{E::EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK, EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_BACK_HANDLER, &STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_LOCKDOWN_SUCCESS_trigger[2] = 
{
	{E::EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST, EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST_HANDLER, NULL           },
	{E::EVT_CLOSE_LOCKDOWN_SUCCESS        , EVT_CLOSE_LOCKDOWN_SUCCESS_HANDLER        , &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_LOGIN_ONLINE_trigger[9] = 
{
	{E::EVT_LOGIN_ONLINE_CREATE_ACCOUNT     , EVT_LOGIN_ONLINE_CREATE_ACCOUNT_HANDLER     , NULL  },
	{E::EVT_LOGIN_ONLINE_SIGN_IN            , EVT_LOGIN_ONLINE_SIGN_IN_HANDLER            , NULL  },
	{E::EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE  , EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE_HANDLER  , NULL  },
	{E::EVT_LOGIN_ONLINE_CHANGE_PASSWORD    , EVT_LOGIN_ONLINE_CHANGE_PASSWORD_HANDLER    , NULL  },
	{E::EVT_LOGIN_ONLINE_FORGOT_PASSWORD    , EVT_LOGIN_ONLINE_FORGOT_PASSWORD_HANDLER    , NULL  },
	{E::EVT_LOGIN_ONLINE_RECOVER_PASSWORD   , EVT_LOGIN_ONLINE_RECOVER_PASSWORD_HANDLER   , NULL  },
	{E::EVT_LOGIN_ONLINE_STAY_SIGNED_IN     , EVT_LOGIN_ONLINE_STAY_SIGNED_IN_HANDLER     , NULL  },
	{E::EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE  , EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE_HANDLER  , NULL  },
	{E::EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE, EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE_HANDLER, NULL  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_trigger[3] = 
{
	{E::EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST, EVT_PRIMARY_KEY_SIGNIN_ACCOUNT_REQUEST_HANDLER, NULL                              },
	{E::EVT_LOGIN_WITH_SOFTWARE_KEY_BACK      , EVT_LOGIN_WITH_SOFTWARE_KEY_BACK_HANDLER      , &STATE_ID_SCR_PRIMARY_KEY_ACCOUNT },
	{E::EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED   , EVT_LOGIN_WITH_SOFTWARE_KEY_SUCCEED_HANDLER   , &STATE_ID_SCR_HOME                },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_MASTER_SIGNER_INFO_trigger[9] = 
{
	{E::EVT_MASTER_SIGNER_INFO_EDIT_NAME       , EVT_MASTER_SIGNER_INFO_EDIT_NAME_HANDLER       , NULL                      },
	{E::EVT_MASTER_SIGNER_INFO_HEALTH_CHECK    , EVT_MASTER_SIGNER_INFO_HEALTH_CHECK_HANDLER    , NULL                      },
	{E::EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST  , EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST_HANDLER  , NULL                      },
	{E::EVT_MASTER_SIGNER_INFO_PROMT_PIN       , EVT_MASTER_SIGNER_INFO_PROMT_PIN_HANDLER       , NULL                      },
	{E::EVT_MASTER_SIGNER_INFO_SEND_PIN        , EVT_MASTER_SIGNER_INFO_SEND_PIN_HANDLER        , NULL                      },
	{E::EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE , EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE_HANDLER , NULL                      },
	{E::EVT_MASTER_SIGNER_INFO_GET_XPUBS       , EVT_MASTER_SIGNER_INFO_GET_XPUBS_HANDLER       , NULL                      },
	{E::EVT_MASTER_SIGNER_INFO_BACK_REQUEST    , EVT_MASTER_SIGNER_INFO_BACK_REQUEST_HANDLER    , &STATE_ID_SCR_HOME        },
	{E::EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO, EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO_HANDLER, &STATE_ID_SCR_WALLET_INFO },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ONBOARDING_trigger[2] = 
{
	{E::EVT_ONBOARDING_ACTION_REQUEST, EVT_ONBOARDING_ACTION_REQUEST_HANDLER, NULL           },
	{E::EVT_ONBOARDING_CLOSE         , EVT_ONBOARDING_CLOSE_HANDLER         , &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ONLINE_ADD_CONTACTS_trigger[3] = 
{
	{E::EVT_ONLINE_ADD_CONTACTS_SEARCH_ID      , EVT_ONLINE_ADD_CONTACTS_SEARCH_ID_HANDLER      , NULL                      },
	{E::EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION, EVT_ONLINE_ADD_CONTACTS_SEND_INVITATION_HANDLER, NULL                      },
	{E::EVT_ONLINE_ADD_CONTACTS_BACK           , EVT_ONLINE_ADD_CONTACTS_BACK_HANDLER           , &STATE_ID_SCR_HOME_ONLINE },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_PENDING_REQUEST_trigger[3] = 
{
	{E::EVT_PENDING_REQUEST_ACCEPT, EVT_PENDING_REQUEST_ACCEPT_HANDLER, NULL                      },
	{E::EVT_PENDING_REQUEST_IGNORE, EVT_PENDING_REQUEST_IGNORE_HANDLER, NULL                      },
	{E::EVT_PENDING_REQUEST_BACK  , EVT_PENDING_REQUEST_BACK_HANDLER  , &STATE_ID_SCR_HOME_ONLINE },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_trigger[7] = 
{
	{E::EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST               , EVT_SELECT_PRIMARY_KEY_ACCOUNT_REQUEST_HANDLER               , NULL                                               },
	{E::EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT               , EVT_PRIMARY_KEY_BACK_TO_CREATE_ACCOUNT_HANDLER               , &STATE_ID_SCR_CREATE_ACCOUNT                       },
	{E::EVT_PRIMARY_KEY_ACCOUNT_BACK                         , EVT_PRIMARY_KEY_ACCOUNT_BACK_HANDLER                         , &STATE_ID_SCR_CREATE_ACCOUNT                       },
	{E::EVT_ADD_PRIMARY_KEY_REQUEST                          , EVT_ADD_PRIMARY_KEY_REQUEST_HANDLER                          , &STATE_ID_SCR_ADD_NEW_SIGNER                       },
	{E::EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST, EVT_SHOW_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_REQUEST_HANDLER, &STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY },
	{E::EVT_SCR_SIGN_IN_MANUALLY_REQUEST                     , EVT_SCR_SIGN_IN_MANUALLY_REQUEST_HANDLER                     , &STATE_ID_SCR_SIGN_IN_MANUALLY                     },
	{E::EVT_PRIMARY_KEY_BACK_TO_SIGN_IN                      , EVT_PRIMARY_KEY_BACK_TO_SIGN_IN_HANDLER                      , &STATE_ID_SCR_SIGN_IN                              },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_trigger[3] = 
{
	{E::EVT_PRIMARY_KEY_SIGN_IN_REQUEST       , EVT_PRIMARY_KEY_SIGN_IN_REQUEST_HANDLER       , NULL               },
	{E::EVT_PRIMARY_KEY_SIGN_IN_SUCCEED       , EVT_PRIMARY_KEY_SIGN_IN_SUCCEED_HANDLER       , NULL               },
	{E::EVT_PRIMARY_KEY_CONFIGURATION_FINISHED, EVT_PRIMARY_KEY_CONFIGURATION_FINISHED_HANDLER, &STATE_ID_SCR_HOME },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_RECEIVE_trigger[6] = 
{
	{E::EVT_BTN_GEN_NEW_ADDRESS              , EVT_BTN_GEN_NEW_ADDRESS_HANDLER              , NULL               },
	{E::EVT_RECEIVE_COPY_ADDRESS             , EVT_RECEIVE_COPY_ADDRESS_HANDLER             , NULL               },
	{E::EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS, EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS_HANDLER, NULL               },
	{E::EVT_RECEIVE_DISPLAY_ADDRESS          , EVT_RECEIVE_DISPLAY_ADDRESS_HANDLER          , NULL               },
	{E::EVT_RECEIVE_ADDRESS_BALANCE          , EVT_RECEIVE_ADDRESS_BALANCE_HANDLER          , NULL               },
	{E::EVT_RECEIVE_BACK_REQUEST             , EVT_RECEIVE_BACK_REQUEST_HANDLER             , &STATE_ID_SCR_HOME },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_RECOVER_SHARED_WALLET_trigger[4] = 
{
	{E::EVT_IMPORT_BSMS_SHARED_WALLET   , EVT_IMPORT_BSMS_SHARED_WALLET_HANDLER   , NULL                               },
	{E::EVT_IMPORT_QR_SHARED_WALLET     , EVT_IMPORT_QR_SHARED_WALLET_HANDLER     , NULL                               },
	{E::EVT_IMPORT_SHARED_WALLET_SUCCEED, EVT_IMPORT_SHARED_WALLET_SUCCEED_HANDLER, &STATE_ID_SCR_REVIEW_SHARED_WALLET },
	{E::EVT_RECOVER_SHARED_WALLET_BACK  , EVT_RECOVER_SHARED_WALLET_BACK_HANDLER  , &STATE_ID_SCR_CREATE_SHARED_WALLET },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_trigger[5] = 
{
	{E::EVT_RECOVER_SOFTWARE_SIGNER_REQUEST                            , EVT_RECOVER_SOFTWARE_SIGNER_REQUEST_HANDLER                            , NULL                                          },
	{E::EVT_RECOVER_SOFTWARE_SIGNER_BACK                               , EVT_RECOVER_SOFTWARE_SIGNER_BACK_HANDLER                               , &STATE_ID_SCR_ADD_NEW_SIGNER                  },
	{E::EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED                            , EVT_RECOVER_SOFTWARE_SIGNER_SUCCEED_HANDLER                            , &STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION   },
	{E::EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK                     , EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET_BACK_HANDLER                     , &STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET        },
	{E::EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION, EVT_RECOVER_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION_HANDLER, &STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_RECURRING_PAYMENTS_trigger[1] = 
{
	{E::EVT_RECURRING_PAYMENTS_ENTER, EVT_RECURRING_PAYMENTS_ENTER_HANDLER, NULL  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_REENTER_YOUR_PASSWORD_trigger[2] = 
{
	{E::EVT_INPUT_PASSWORD_REQUEST    , EVT_INPUT_PASSWORD_REQUEST_HANDLER    , NULL           },
	{E::EVT_REENTER_YOUR_PASSWORD_BACK, EVT_REENTER_YOUR_PASSWORD_BACK_HANDLER, &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_REMOTE_SIGNER_INFO_trigger[8] = 
{
	{E::EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK    , EVT_REMOTE_SIGNER_INFO_HEALTH_CHECK_HANDLER    , NULL                      },
	{E::EVT_REMOTE_SIGNER_INFO_EDIT_NAME       , EVT_REMOTE_SIGNER_INFO_EDIT_NAME_HANDLER       , NULL                      },
	{E::EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE, EVT_REMOTE_SIGNER_INFO_IMPORT_SIGNATURE_HANDLER, NULL                      },
	{E::EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE  , EVT_REMOTE_SIGNER_INFO_EXPORT_MESSAGE_HANDLER  , NULL                      },
	{E::EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST  , EVT_REMOTE_SIGNER_INFO_DELETE_REQUEST_HANDLER  , NULL                      },
	{E::EVT_REMOTE_SIGNER_INFO_GET_XPUBS       , EVT_REMOTE_SIGNER_INFO_GET_XPUBS_HANDLER       , NULL                      },
	{E::EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO, EVT_REMOTE_SIGNER_INFO_BACK_WALLET_INFO_HANDLER, &STATE_ID_SCR_WALLET_INFO },
	{E::EVT_REMOTE_SIGNER_INFO_BACK_HOME       , EVT_REMOTE_SIGNER_INFO_BACK_HOME_HANDLER       , &STATE_ID_SCR_HOME        },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_REPLACE_KEYS_trigger[1] = 
{
	{E::EVT_REPLACE_KEYS_ENTER, EVT_REPLACE_KEYS_ENTER_HANDLER, NULL  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_REPLACE_PRIMARY_KEY_trigger[1] = 
{
	{E::EVT_REPLACE_PRIMARY_KEY_REQUEST, EVT_REPLACE_PRIMARY_KEY_REQUEST_HANDLER, &STATE_ID_SCR_ADD_NEW_SIGNER },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_REPLACE_SELECT_KEY_trigger[1] = 
{
	{E::EVT_REPLACE_SELECT_KEY_ENTER, EVT_REPLACE_SELECT_KEY_ENTER_HANDLER, NULL  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_REVIEW_SHARED_WALLET_trigger[2] = 
{
	{E::EVT_REVIEW_SHARED_WALLET_BACK         , EVT_REVIEW_SHARED_WALLET_BACK_HANDLER         , &STATE_ID_SCR_CONFIGURE_SHARED_WALLET        },
	{E::EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER, EVT_REVIEW_SHARED_WALLET_INVITE_SIGNER_HANDLER, &STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SELECT_SERVER_trigger[1] = 
{
	{E::EVT_SELECT_SERVER_ENTER, EVT_SELECT_SERVER_ENTER_HANDLER, NULL  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_trigger[1] = 
{
	{E::EVT_SELECT_WALLET_REQUEST, EVT_SELECT_WALLET_REQUEST_HANDLER, NULL  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_trigger[2] = 
{
	{E::EVT_INPUT_DAYS_VALUE_REQUEST                , EVT_INPUT_DAYS_VALUE_REQUEST_HANDLER                , NULL                                           },
	{E::EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST, EVT_LOCKDOWN_ANSER_SECURITY_QUESTION_REQUEST_HANDLER, &STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SEND_trigger[5] = 
{
	{E::EVT_SEND_ADD_DESTINATION_REQUEST   , EVT_SEND_ADD_DESTINATION_REQUEST_HANDLER   , NULL                             },
	{E::EVT_SEND_CREATE_TRANSACTION_REQUEST, EVT_SEND_CREATE_TRANSACTION_REQUEST_HANDLER, NULL                             },
	{E::EVT_SEND_BACK_REQUEST              , EVT_SEND_BACK_REQUEST_HANDLER              , &STATE_ID_SCR_HOME               },
	{E::EVT_SEND_CREATE_TRANSACTION_SUCCEED, EVT_SEND_CREATE_TRANSACTION_SUCCEED_HANDLER, &STATE_ID_SCR_CREATE_TRANSACTION },
	{E::EVT_SEND_BACK_HOME_SHARED_WL       , EVT_SEND_BACK_HOME_SHARED_WL_HANDLER       , &STATE_ID_SCR_HOME_ONLINE        },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SERVICE_SETTINGS_trigger[13] = 
{
	{E::EVT_CLAIM_INHERITANCE_CHECK_REQUEST     , EVT_CLAIM_INHERITANCE_CHECK_REQUEST_HANDLER     , NULL                                       },
	{E::EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST, EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST_HANDLER, NULL                                       },
	{E::EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED, EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED_HANDLER, NULL                                       },
	{E::EVT_SERVICE_SELECT_WALLET_REQUEST       , EVT_SERVICE_SELECT_WALLET_REQUEST_HANDLER       , NULL                                       },
	{E::EVT_INHERITANCE_PLAN_FINALIZE_REQUEST   , EVT_INHERITANCE_PLAN_FINALIZE_REQUEST_HANDLER   , NULL                                       },
	{E::EVT_SERVICE_SUPPORT_REQUEST             , EVT_SERVICE_SUPPORT_REQUEST_HANDLER             , &STATE_ID_SCR_HOME_ONLINE                  },
	{E::EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST, EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST_HANDLER, &STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE },
	{E::EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST  , EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST_HANLDER  , &STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN   },
	{E::EVT_KEY_RECOVERY_REQUEST                , EVT_KEY_RECOVERY_REQUEST_HANDLER                , &STATE_ID_SCR_KEY_RECOVERY                 },
	{E::EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST , EVT_SELECT_YOUR_LOCKDOWN_PERIOD_REQUEST_HANDLER , &STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD  },
	{E::EVT_WALLET_CO_SIGN_POLICE_REQUEST       , EVT_WALLET_CO_SIGN_POLICE_REQUEST_HANDLER       , &STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE },
	{E::EVT_LOCKDOWN_SUCCESS_REQUEST            , EVT_LOCKDOWN_SUCCESS_REQUEST_HANDLER            , &STATE_ID_SCR_LOCKDOWN_SUCCESS             },
	{E::EVT_SETUP_SECURITY_QUESTION_REQUEST     , EVT_SETUP_SECURITY_QUESTION_REQUEST_HANDLER     , &STATE_ID_SCR_SETUP_SECURITY_QUESTION      },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_trigger[2] = 
{
	{E::EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ , EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ_HANDLER , NULL                                  },
	{E::EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK, EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK_HANDLER, &STATE_ID_SCR_SETUP_SECURITY_QUESTION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SETUP_GROUP_WALLET_trigger[2] = 
{
	{E::EVT_SETUP_GROUP_WALLET_ENTER, EVT_SETUP_GROUP_WALLET_ENTER_HANDLER, NULL           },
	{E::EVT_SETUP_GROUP_WALLET_BACK , EVT_SETUP_GROUP_WALLET_BACK_HANDLER , &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SETUP_SECURITY_QUESTION_trigger[2] = 
{
	{E::EVT_INPUT_SECURITY_QUESTION_REQUEST   , EVT_INPUT_SECURITY_QUESTION_REQUEST_HANDLER   , NULL                                         },
	{E::EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ, EVT_SETUP_ANSWER_SECURITY_QUESTION_REQ_HANDLER, &STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SHARED_WALLET_CONFIGURE_trigger[10] = 
{
	{E::EVT_SHARED_WALLET_FINALIZE_WALLET          , EVT_SHARED_WALLET_FINALIZE_WALLET_HANDLER          , NULL                                         },
	{E::EVT_SHARED_WALLET_CONFIGURE_CANCEL         , EVT_SHARED_WALLET_CONFIGURE_CANCEL_HANDLER         , NULL                                         },
	{E::EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD, EVT_SHARED_WALLET_CONFIGURE_EXPORT_COLDCARD_HANDLER, NULL                                         },
	{E::EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE  , EVT_SHARED_WALLET_CONFIGURE_EXPORT_QRCODE_HANDLER  , NULL                                         },
	{E::EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL     , EVT_SHARED_WALLET_CONFIGURE_DELETE_SWL_HANDLER     , NULL                                         },
	{E::EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS    , EVT_SHARED_WALLET_CONFIGURE_EXPORT_BSMS_HANDLER    , NULL                                         },
	{E::EVT_SHARED_WALLET_CONFIGURE_BACK           , EVT_SHARED_WALLET_CONFIGURE_BACK_HANDLER           , &STATE_ID_SCR_HOME_ONLINE                    },
	{E::EVT_SHARED_WALLET_ADD_SIGNER_REQUEST       , EVT_SHARED_WALLET_ADD_SIGNER_REQUEST_HANDLER       , &STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET },
	{E::EVT_SHARED_WALLET_UTXO_OUTPUT              , EVT_SHARED_WALLET_UTXO_OUTPUT_HANDLER              , &STATE_ID_SCR_UTXOS                          },
	{E::EVT_SHARED_WALLET_CHANGE_ADDRESS           , EVT_SHARED_WALLET_CHANGE_ADDRESS_HANDLER           , &STATE_ID_SCR_WALLET_CHANGE_ADDRESSES        },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_trigger[4] = 
{
	{E::EVT_SHARED_WALLET_EXPORT_COLDCARD    , EVT_SHARED_WALLET_EXPORT_COLDCARD_HANDLER    , NULL                               },
	{E::EVT_SHARED_WALLET_EXPORT_QRCODE      , EVT_SHARED_WALLET_EXPORT_QRCODE_HANDLER      , NULL                               },
	{E::EVT_SHARED_WALLET_SKIP_REGISTRATION  , EVT_SHARED_WALLET_SKIP_REGISTRATION_HANDLER  , NULL                               },
	{E::EVT_SHARED_WL_DEVICE_REGISTRAION_BACK, EVT_SHARED_WL_DEVICE_REGISTRAION_BACK_HANDLER, &STATE_ID_SCR_BACKUP_SHARED_WALLET },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SHARE_YOUR_SECRETS_trigger[1] = 
{
	{E::EVT_UPDATE_YOUR_SECRET_REQUEST, EVT_UPDATE_YOUR_SECRET_REQUEST_HANDLER, NULL  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SIGN_IN_trigger[3] = 
{
	{E::EVT_SIGN_IN_PASSWORD_REQUEST        , EVT_SIGN_IN_PASSWORD_REQUEST_HANDLER        , NULL                              },
	{E::EVT_SIGN_IN_PRIMARY_KEY_REQUEST     , EVT_SIGN_IN_PRIMARY_KEY_REQUEST_HANDLER     , NULL                              },
	{E::EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST, EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST_HANDLER, &STATE_ID_SCR_PRIMARY_KEY_ACCOUNT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_trigger[5] = 
{
	{E::EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST        , EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST_HANDLER        , NULL                              },
	{E::EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST, EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST_HANDLER, NULL                              },
	{E::EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST        , EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST_HANDLER        , NULL                              },
	{E::EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED        , EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED_HANDLER        , &STATE_ID_SCR_HOME                },
	{E::EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK   , EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK_HANDLER   , &STATE_ID_SCR_PRIMARY_KEY_ACCOUNT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SIGN_IN_MANUALLY_trigger[4] = 
{
	{E::EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST   , EVT_ADD_PRIMARY_KEY_ACCOUNT_REQUEST_HANDLER   , NULL               },
	{E::EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST, EVT_PRIMARY_KEY_CHECK_USERNAME_REQUEST_HANDLER, NULL               },
	{E::EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST , EVT_CHALLENGE_MESSAGE_REFRESH_REQUEST_HANDLER , NULL               },
	{E::EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED   , EVT_ADD_PRIMARY_KEY_ACCOUNT_SUCCEED_HANDLER   , &STATE_ID_SCR_HOME },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SIGN_IN_VIA_XPUB_trigger[2] = 
{
	{E::EVT_SIGN_IN_VIA_XPUB_ENTER, EVT_SIGN_IN_VIA_XPUB_ENTER_HANDLER, NULL           },
	{E::EVT_SIGN_IN_VIA_XPUB_CLOSE, EVT_SIGN_IN_VIA_XPUB_CLOSE_HANDLER, &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_trigger[3] = 
{
	{E::EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                   , EVT_SOFTWARE_SIGNER_REQUEST_CREATE_HANDLER                                   , NULL                                          },
	{E::EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                               , EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_HANDLER                               , &STATE_ID_SCR_CREATE_NEW_SEED                 },
	{E::EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION, EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION_HANDLER, &STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_STARTING_CREATE_SIGNER_trigger[3] = 
{
	{E::EVT_STARTING_CREATE_SIGNER_DUMMY, EVT_STARTING_CREATE_SIGNER_DUMMY_HANDLER, NULL                         },
	{E::EVT_STARTING_CREATE_SIGNER_BACK , EVT_STARTING_CREATE_SIGNER_BACK_HANDLER , &STATE_ID_SCR_HOME_ONLINE    },
	{E::EVT_ONLINE_MODE_ADD_NEW_SIGNER  , EVT_ONLINE_MODE_ADD_NEW_SIGNER_HANDLER  , &STATE_ID_SCR_ADD_NEW_SIGNER },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_trigger[1] = 
{
	{E::EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST, EVT_TAPSIGNER_RECOVERED_KEY_INFO_REQUEST_HANDLER, &STATE_ID_SCR_MASTER_SIGNER_INFO },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_TRANSACTION_HISTORY_trigger[3] = 
{
	{E::EVT_TRANSACTION_HISTORY_SORT_REQUEST, EVT_TRANSACTION_HISTORY_SORT_REQUEST_HANDLER, NULL                           },
	{E::EVT_TRANSACTION_HISTORY_BACK_REQUEST, EVT_TRANSACTION_HISTORY_BACK_REQUEST_HANDLER, &STATE_ID_SCR_HOME             },
	{E::EVT_TRANSACTION_INFO_ITEM_SELECTED  , EVT_TRANSACTION_INFO_ITEM_SELECTED_HANDLER  , &STATE_ID_SCR_TRANSACTION_INFO },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_TRANSACTION_INFO_trigger[18] = 
{
	{E::EVT_TRANSACTION_SIGN_REQUEST                           , EVT_TRANSACTION_SIGN_REQUEST_HANDLER                           , NULL                              },
	{E::EVT_TRANSACTION_EXPORT_REQUEST                         , EVT_TRANSACTION_EXPORT_REQUEST_HANDLER                         , NULL                              },
	{E::EVT_TRANSACTION_EXPORT_QRCODE                          , EVT_TRANSACTION_EXPORT_QRCODE_HANDLER                          , NULL                              },
	{E::EVT_TRANSACTION_IMPORT_REQUEST                         , EVT_TRANSACTION_IMPORT_REQUEST_HANDLER                         , NULL                              },
	{E::EVT_TRANSACTION_IMPORT_QRCODE                          , EVT_TRANSACTION_IMPORT_QRCODE_HANDLER                          , NULL                              },
	{E::EVT_TRANSACTION_BROADCAST_REQUEST                      , EVT_TRANSACTION_BROADCAST_REQUEST_HANDLER                      , NULL                              },
	{E::EVT_TRANSACTION_SET_MEMO_REQUEST                       , EVT_TRANSACTION_SET_MEMO_REQUEST_HANDLER                       , NULL                              },
	{E::EVT_TRANSACTION_SCAN_DEVICE_REQUEST                    , EVT_TRANSACTION_SCAN_DEVICE_REQUEST_HANDLER                    , NULL                              },
	{E::EVT_TRANSACTION_REMOVE_REQUEST                         , EVT_TRANSACTION_REMOVE_REQUEST_HANDLER                         , NULL                              },
	{E::EVT_TRANSACTION_SEND_PIN_REQUEST                       , EVT_TRANSACTION_SEND_PIN_REQUEST_HANDLER                       , NULL                              },
	{E::EVT_TRANSACTION_PROMT_PIN_REQUEST                      , EVT_TRANSACTION_PROMT_PIN_REQUEST_HANDLER                      , NULL                              },
	{E::EVT_TRANSACTION_VERIFY_ADDRESS                         , EVT_TRANSACTION_VERIFY_ADDRESS_HANDLER                         , NULL                              },
	{E::EVT_TRANSACTION_SEND_PASSPHRASE                        , EVT_TRANSACTION_SEND_PASSPHRASE_HANDLER                        , NULL                              },
	{E::EVT_TRANSACTION_CANCEL_REQUEST                         , EVT_TRANSACTION_CANCEL_REQUEST_HANDLER                         , NULL                              },
	{E::EVT_TRANSACTION_INFO_BACK_REQUEST                      , EVT_TRANSACTION_INFO_BACK_REQUEST_HANDLER                      , &STATE_ID_SCR_TRANSACTION_HISTORY },
	{E::EVT_TRANSACTION_INFO_CANCEL_PENDING_TRANSACTION_REQUEST, EVT_TRANSACTION_INFO_CANCEL_PENDING_TRANSACTION_REQUEST_HANDLER, &STATE_ID_SCR_CREATE_TRANSACTION  },
	{E::EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST                 , EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST_HANDLER                 , &STATE_ID_SCR_CREATE_TRANSACTION  },
	{E::EVT_TRANSACTION_INFO_BACK_UTXO_INFO                    , EVT_TRANSACTION_INFO_BACK_UTXO_INFO_HANDLE                     , &STATE_ID_SCR_UTXO_OUTPUT         },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_UNLOCK_DB_trigger[1] = 
{
	{E::EVT_LOGIN_PERFORM_PASSWORD_REQUEST, EVT_LOGIN_PERFORM_PASSWORD_REQUEST_HANDLER, NULL  },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_UPDATE_PROFILE_trigger[2] = 
{
	{E::EVT_UPDATE_PROFILE_REQUEST, EVT_UPDATE_PROFILE_REQUEST_HANDLER, NULL           },
	{E::EVT_UPDATE_PROFILE_BACK   , EVT_UPDATE_PROFILE_BACK_HANDLER   , &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_UTXOS_trigger[4] = 
{
	{E::EVT_UTXOS_SORT_REQUEST            , EVT_UTXOS_SORT_REQUEST_HANDLER            , NULL                                  },
	{E::EVT_UTXOS_BACK_REQUEST            , EVT_UTXOS_BACK_REQUEST_HANDLER            , &STATE_ID_SCR_WALLET_INFO             },
	{E::EVT_UTXOS_ITEM_SELECTED           , EVT_UTXOS_ITEM_SELECTED_HANDLER           , &STATE_ID_SCR_UTXO_OUTPUT             },
	{E::EVT_UTXO_OUTPUT_BACK_SHARED_WALLET, EVT_UTXO_OUTPUT_BACK_SHARED_WALLET_HANDLER, &STATE_ID_SCR_SHARED_WALLET_CONFIGURE },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_UTXO_OUTPUT_trigger[3] = 
{
	{E::EVT_UTXO_INFO_CHECKOUT_TX_RELATED, EVT_UTXO_INFO_CHECKOUT_TX_RELATED_HANDLER, NULL                           },
	{E::EVT_UTXO_INFO_BACK_REQUEST       , EVT_UTXO_INFO_BACK_REQUEST_HANDLER       , &STATE_ID_SCR_UTXOS            },
	{E::EVT_UTXO_INFO_VIEW_TX_RELATED    , EVT_UTXO_INFO_VIEW_TX_RELATED_HANDLER    , &STATE_ID_SCR_TRANSACTION_INFO },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_trigger[4] = 
{
	{E::EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS, EVT_WALLET_CHANGE_ADDRESSES_GEN_NEW_ADDRESS_HANDLER, NULL                                  },
	{E::EVT_WALLET_CHANGE_ADDRESSES_COPY           , EVT_WALLET_CHANGE_ADDRESSES_COPY_HANDLER           , NULL                                  },
	{E::EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST     , EVT_WALLET_CHANGE_ADDRESS_BACK_REQUEST_HANDLER     , &STATE_ID_SCR_WALLET_INFO             },
	{E::EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET      , EVT_CHANGE_ADDRESS_BACK_SHARED_WALLET_HANDLER      , &STATE_ID_SCR_SHARED_WALLET_CONFIGURE },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_WALLET_INFO_trigger[17] = 
{
	{E::EVT_WALLET_INFO_EDIT_NAME                 , EVT_WALLET_INFO_EDIT_NAME_HANDLER                 , NULL                                  },
	{E::EVT_WALLET_INFO_REMOVE                    , EVT_WALLET_INFO_REMOVE_HANDLER                    , NULL                                  },
	{E::EVT_WALLET_INFO_EXPORT_DB                 , EVT_WALLET_INFO_EXPORT_DB_HANDLER                 , NULL                                  },
	{E::EVT_WALLET_INFO_EXPORT_CSV                , EVT_WALLET_INFO_EXPORT_CSV_HANDLER                , NULL                                  },
	{E::EVT_WALLET_INFO_EXPORT_DESCRIPTOR         , EVT_WALLET_INFO_EXPORT_DESCRIPTOR_HANDLER         , NULL                                  },
	{E::EVT_WALLET_INFO_EXPORT_COLDCARD           , EVT_WALLET_INFO_EXPORT_COLDCARD_HANDLER           , NULL                                  },
	{E::EVT_WALLET_INFO_EXPORT_QRCODE             , EVT_WALLET_INFO_EXPORT_QRCODE_HANDLER             , NULL                                  },
	{E::EVT_WALLET_INFO_SIGNER_INFO_REQUEST       , EVT_WALLET_INFO_SIGNER_INFO_REQUEST_HANDLER       , NULL                                  },
	{E::EVT_WALLET_INFO_EDIT_DESCRIPTION          , EVT_WALLET_INFO_EDIT_DESCRIPTION_HANDLER          , NULL                                  },
	{E::EVT_WALLET_INFO_IMPORT_PSBT               , EVT_WALLET_INFO_IMPORT_PSBT_HANDLER               , NULL                                  },
	{E::EVT_WALLET_INFO_REFRESH_WALLET_REQUEST    , EVT_WALLET_INFO_REFRESH_WALLET_REQUEST_HANDLER    , NULL                                  },
	{E::EVT_WALLET_INFO_GAP_LIMIT_REQUEST         , EVT_WALLET_INFO_GAP_LIMIT_REQUEST_HANDLER         , NULL                                  },
	{E::EVT_WALLET_INFO_BACK_REQUEST              , EVT_WALLET_INFO_BACK_REQUEST_HANDLER              , &STATE_ID_SCR_HOME                    },
	{E::EVT_WALLET_INFO_UTXOS_REQUEST             , EVT_WALLET_INFO_UTXOS_REQUEST_HANDLER             , &STATE_ID_SCR_UTXOS                   },
	{E::EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST    , EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST_HANDLER    , &STATE_ID_SCR_WALLET_CHANGE_ADDRESSES },
	{E::EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST, EVT_WALLET_INFO_MASTER_SIGNER_INFO_REQUEST_HANDLER, &STATE_ID_SCR_MASTER_SIGNER_INFO      },
	{E::EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST, EVT_WALLET_INFO_REMOTE_SIGNER_INFO_REQUEST_HANDLER, &STATE_ID_SCR_REMOTE_SIGNER_INFO      },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_TOAST_MESSAGE_DISPLAY_trigger[1] = 
{
	{E::EVT_CLOSE_TOAST_MESSAGE, EVT_CLOSE_TOAST_MESSAGE_HANDLER, &STATE_ID_ROOT },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_SYSTEM STATE_ALL[89] = 
{
	{E::STATE_ID_ROOT                                    , STATE_ID_ROOT_trigger                                    , ALEN(STATE_ID_ROOT_trigger)                                    , &STATE_ID_ROOT                                     },
	{E::STATE_ID_SCR_HOME                                , STATE_ID_SCR_HOME_trigger                                , ALEN(STATE_ID_SCR_HOME_trigger)                                , &STATE_ID_SCR_HOME                                 },
	{E::STATE_ID_SCR_ADD_WALLET                          , STATE_ID_SCR_ADD_WALLET_trigger                          , ALEN(STATE_ID_SCR_ADD_WALLET_trigger)                          , &STATE_ID_SCR_ADD_WALLET                           },
	{E::STATE_ID_SCR_ADD_HARDWARE_SIGNER                 , STATE_ID_SCR_ADD_HARDWARE_SIGNER_trigger                 , ALEN(STATE_ID_SCR_ADD_HARDWARE_SIGNER_trigger)                 , &STATE_ID_SCR_ADD_HARDWARE_SIGNER                  },
	{E::STATE_ID_SCR_MASTER_SIGNER_INFO                  , STATE_ID_SCR_MASTER_SIGNER_INFO_trigger                  , ALEN(STATE_ID_SCR_MASTER_SIGNER_INFO_trigger)                  , &STATE_ID_SCR_MASTER_SIGNER_INFO                   },
	{E::STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION     , STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_trigger     , ALEN(STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_trigger)     , &STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION      },
	{E::STATE_ID_SCR_RECEIVE                             , STATE_ID_SCR_RECEIVE_trigger                             , ALEN(STATE_ID_SCR_RECEIVE_trigger)                             , &STATE_ID_SCR_RECEIVE                              },
	{E::STATE_ID_SCR_SEND                                , STATE_ID_SCR_SEND_trigger                                , ALEN(STATE_ID_SCR_SEND_trigger)                                , &STATE_ID_SCR_SEND                                 },
	{E::STATE_ID_SCR_CREATE_TRANSACTION                  , STATE_ID_SCR_CREATE_TRANSACTION_trigger                  , ALEN(STATE_ID_SCR_CREATE_TRANSACTION_trigger)                  , &STATE_ID_SCR_CREATE_TRANSACTION                   },
	{E::STATE_ID_SCR_TRANSACTION_INFO                    , STATE_ID_SCR_TRANSACTION_INFO_trigger                    , ALEN(STATE_ID_SCR_TRANSACTION_INFO_trigger)                    , &STATE_ID_SCR_TRANSACTION_INFO                     },
	{E::STATE_ID_SCR_TRANSACTION_HISTORY                 , STATE_ID_SCR_TRANSACTION_HISTORY_trigger                 , ALEN(STATE_ID_SCR_TRANSACTION_HISTORY_trigger)                 , &STATE_ID_SCR_TRANSACTION_HISTORY                  },
	{E::STATE_ID_SCR_APP_SETTINGS                        , STATE_ID_SCR_APP_SETTINGS_trigger                        , ALEN(STATE_ID_SCR_APP_SETTINGS_trigger)                        , &STATE_ID_SCR_APP_SETTINGS                         },
	{E::STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT            , STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_trigger            , ALEN(STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT_trigger)            , &STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT             },
	{E::STATE_ID_SCR_UTXOS                               , STATE_ID_SCR_UTXOS_trigger                               , ALEN(STATE_ID_SCR_UTXOS_trigger)                               , &STATE_ID_SCR_UTXOS                                },
	{E::STATE_ID_SCR_WALLET_INFO                         , STATE_ID_SCR_WALLET_INFO_trigger                         , ALEN(STATE_ID_SCR_WALLET_INFO_trigger)                         , &STATE_ID_SCR_WALLET_INFO                          },
	{E::STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET       , STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_trigger       , ALEN(STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET_trigger)       , &STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET        },
	{E::STATE_ID_SCR_WALLET_CHANGE_ADDRESSES             , STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_trigger             , ALEN(STATE_ID_SCR_WALLET_CHANGE_ADDRESSES_trigger)             , &STATE_ID_SCR_WALLET_CHANGE_ADDRESSES              },
	{E::STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT            , STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_trigger            , ALEN(STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT_trigger)            , &STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT             },
	{E::STATE_ID_SCR_REMOTE_SIGNER_INFO                  , STATE_ID_SCR_REMOTE_SIGNER_INFO_trigger                  , ALEN(STATE_ID_SCR_REMOTE_SIGNER_INFO_trigger)                  , &STATE_ID_SCR_REMOTE_SIGNER_INFO                   },
	{E::STATE_ID_SCR_UNLOCK_DB                           , STATE_ID_SCR_UNLOCK_DB_trigger                           , ALEN(STATE_ID_SCR_UNLOCK_DB_trigger)                           , &STATE_ID_SCR_UNLOCK_DB                            },
	{E::STATE_ID_SCR_CONSOLIDATE_OUTPUT                  , STATE_ID_SCR_CONSOLIDATE_OUTPUT_trigger                  , ALEN(STATE_ID_SCR_CONSOLIDATE_OUTPUT_trigger)                  , &STATE_ID_SCR_CONSOLIDATE_OUTPUT                   },
	{E::STATE_ID_SCR_UTXO_OUTPUT                         , STATE_ID_SCR_UTXO_OUTPUT_trigger                         , ALEN(STATE_ID_SCR_UTXO_OUTPUT_trigger)                         , &STATE_ID_SCR_UTXO_OUTPUT                          },
	{E::STATE_ID_SCR_INPUT_PIN                           , STATE_ID_SCR_INPUT_PIN_trigger                           , ALEN(STATE_ID_SCR_INPUT_PIN_trigger)                           , &STATE_ID_SCR_INPUT_PIN                            },
	{E::STATE_ID_SCR_ADD_NEW_SIGNER                      , STATE_ID_SCR_ADD_NEW_SIGNER_trigger                      , ALEN(STATE_ID_SCR_ADD_NEW_SIGNER_trigger)                      , &STATE_ID_SCR_ADD_NEW_SIGNER                       },
	{E::STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER             , STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_trigger             , ALEN(STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER_trigger)             , &STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER              },
	{E::STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER             , STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_trigger             , ALEN(STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER_trigger)             , &STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER              },
	{E::STATE_ID_SCR_CREATE_NEW_SEED                     , STATE_ID_SCR_CREATE_NEW_SEED_trigger                     , ALEN(STATE_ID_SCR_CREATE_NEW_SEED_trigger)                     , &STATE_ID_SCR_CREATE_NEW_SEED                      },
	{E::STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION       , STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_trigger       , ALEN(STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_trigger)       , &STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION        },
	{E::STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET            , STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_trigger            , ALEN(STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET_trigger)            , &STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET             },
	{E::STATE_ID_SCR_INPUT_PASSPHRASE                    , STATE_ID_SCR_INPUT_PASSPHRASE_trigger                    , ALEN(STATE_ID_SCR_INPUT_PASSPHRASE_trigger)                    , &STATE_ID_SCR_INPUT_PASSPHRASE                     },
	{E::STATE_ID_SCR_LOGIN_ONLINE                        , STATE_ID_SCR_LOGIN_ONLINE_trigger                        , ALEN(STATE_ID_SCR_LOGIN_ONLINE_trigger)                        , &STATE_ID_SCR_LOGIN_ONLINE                         },
	{E::STATE_ID_SCR_HOME_ONLINE                         , STATE_ID_SCR_HOME_ONLINE_trigger                         , ALEN(STATE_ID_SCR_HOME_ONLINE_trigger)                         , &STATE_ID_SCR_HOME_ONLINE                          },
	{E::STATE_ID_SCR_ONLINE_ADD_CONTACTS                 , STATE_ID_SCR_ONLINE_ADD_CONTACTS_trigger                 , ALEN(STATE_ID_SCR_ONLINE_ADD_CONTACTS_trigger)                 , &STATE_ID_SCR_ONLINE_ADD_CONTACTS                  },
	{E::STATE_ID_SCR_PENDING_REQUEST                     , STATE_ID_SCR_PENDING_REQUEST_trigger                     , ALEN(STATE_ID_SCR_PENDING_REQUEST_trigger)                     , &STATE_ID_SCR_PENDING_REQUEST                      },
	{E::STATE_ID_SCR_CREATE_SHARED_WALLET                , STATE_ID_SCR_CREATE_SHARED_WALLET_trigger                , ALEN(STATE_ID_SCR_CREATE_SHARED_WALLET_trigger)                , &STATE_ID_SCR_CREATE_SHARED_WALLET                 },
	{E::STATE_ID_SCR_CONFIGURE_SHARED_WALLET             , STATE_ID_SCR_CONFIGURE_SHARED_WALLET_trigger             , ALEN(STATE_ID_SCR_CONFIGURE_SHARED_WALLET_trigger)             , &STATE_ID_SCR_CONFIGURE_SHARED_WALLET              },
	{E::STATE_ID_SCR_REVIEW_SHARED_WALLET                , STATE_ID_SCR_REVIEW_SHARED_WALLET_trigger                , ALEN(STATE_ID_SCR_REVIEW_SHARED_WALLET_trigger)                , &STATE_ID_SCR_REVIEW_SHARED_WALLET                 },
	{E::STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET      , STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_trigger      , ALEN(STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET_trigger)      , &STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET       },
	{E::STATE_ID_SCR_SHARED_WALLET_CONFIGURE             , STATE_ID_SCR_SHARED_WALLET_CONFIGURE_trigger             , ALEN(STATE_ID_SCR_SHARED_WALLET_CONFIGURE_trigger)             , &STATE_ID_SCR_SHARED_WALLET_CONFIGURE              },
	{E::STATE_ID_SCR_BACKUP_SHARED_WALLET                , STATE_ID_SCR_BACKUP_SHARED_WALLET_trigger                , ALEN(STATE_ID_SCR_BACKUP_SHARED_WALLET_trigger)                , &STATE_ID_SCR_BACKUP_SHARED_WALLET                 },
	{E::STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION       , STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_trigger       , ALEN(STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION_trigger)       , &STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION        },
	{E::STATE_ID_SCR_STARTING_CREATE_SIGNER              , STATE_ID_SCR_STARTING_CREATE_SIGNER_trigger              , ALEN(STATE_ID_SCR_STARTING_CREATE_SIGNER_trigger)              , &STATE_ID_SCR_STARTING_CREATE_SIGNER               },
	{E::STATE_ID_TOAST_MESSAGE_DISPLAY                   , STATE_ID_TOAST_MESSAGE_DISPLAY_trigger                   , ALEN(STATE_ID_TOAST_MESSAGE_DISPLAY_trigger)                   , &STATE_ID_TOAST_MESSAGE_DISPLAY                    },
	{E::STATE_ID_SCR_RECOVER_SHARED_WALLET               , STATE_ID_SCR_RECOVER_SHARED_WALLET_trigger               , ALEN(STATE_ID_SCR_RECOVER_SHARED_WALLET_trigger)               , &STATE_ID_SCR_RECOVER_SHARED_WALLET                },
	{E::STATE_ID_SCR_PRIMARY_KEY_ACCOUNT                 , STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_trigger                 , ALEN(STATE_ID_SCR_PRIMARY_KEY_ACCOUNT_trigger)                 , &STATE_ID_SCR_PRIMARY_KEY_ACCOUNT                  },
	{E::STATE_ID_SCR_CREATE_ACCOUNT                      , STATE_ID_SCR_CREATE_ACCOUNT_trigger                      , ALEN(STATE_ID_SCR_CREATE_ACCOUNT_trigger)                      , &STATE_ID_SCR_CREATE_ACCOUNT                       },
	{E::STATE_ID_SCR_CHANGE_PASSWORD                     , STATE_ID_SCR_CHANGE_PASSWORD_trigger                     , ALEN(STATE_ID_SCR_CHANGE_PASSWORD_trigger)                     , &STATE_ID_SCR_CHANGE_PASSWORD                      },
	{E::STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY, STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_trigger, ALEN(STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_trigger), &STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY },
	{E::STATE_ID_SCR_SIGN_IN_MANUALLY                    , STATE_ID_SCR_SIGN_IN_MANUALLY_trigger                    , ALEN(STATE_ID_SCR_SIGN_IN_MANUALLY_trigger)                    , &STATE_ID_SCR_SIGN_IN_MANUALLY                     },
	{E::STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY             , STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_trigger             , ALEN(STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY_trigger)             , &STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY              },
	{E::STATE_ID_SCR_REPLACE_PRIMARY_KEY                 , STATE_ID_SCR_REPLACE_PRIMARY_KEY_trigger                 , ALEN(STATE_ID_SCR_REPLACE_PRIMARY_KEY_trigger)                 , &STATE_ID_SCR_REPLACE_PRIMARY_KEY                  },
	{E::STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION           , STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_trigger           , ALEN(STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_trigger)           , &STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION            },
	{E::STATE_ID_SCR_SIGN_IN                             , STATE_ID_SCR_SIGN_IN_trigger                             , ALEN(STATE_ID_SCR_SIGN_IN_trigger)                             , &STATE_ID_SCR_SIGN_IN                              },
	{E::STATE_ID_SCR_UPDATE_PROFILE                      , STATE_ID_SCR_UPDATE_PROFILE_trigger                      , ALEN(STATE_ID_SCR_UPDATE_PROFILE_trigger)                      , &STATE_ID_SCR_UPDATE_PROFILE                       },
	{E::STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS         , STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_trigger         , ALEN(STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS_trigger)         , &STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS          },
	{E::STATE_ID_SCR_ENTER_BACKUP_PASSWORD               , STATE_ID_SCR_ENTER_BACKUP_PASSWORD_trigger               , ALEN(STATE_ID_SCR_ENTER_BACKUP_PASSWORD_trigger)               , &STATE_ID_SCR_ENTER_BACKUP_PASSWORD                },
	{E::STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION, STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_trigger, ALEN(STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION_trigger), &STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION },
	{E::STATE_ID_SCR_KEY_RECOVERY                        , STATE_ID_SCR_KEY_RECOVERY_trigger                        , ALEN(STATE_ID_SCR_KEY_RECOVERY_trigger)                        , &STATE_ID_SCR_KEY_RECOVERY                         },
	{E::STATE_ID_SCR_REENTER_YOUR_PASSWORD               , STATE_ID_SCR_REENTER_YOUR_PASSWORD_trigger               , ALEN(STATE_ID_SCR_REENTER_YOUR_PASSWORD_trigger)               , &STATE_ID_SCR_REENTER_YOUR_PASSWORD                },
	{E::STATE_ID_SCR_SERVICE_SETTINGS                    , STATE_ID_SCR_SERVICE_SETTINGS_trigger                    , ALEN(STATE_ID_SCR_SERVICE_SETTINGS_trigger)                    , &STATE_ID_SCR_SERVICE_SETTINGS                     },
	{E::STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD         , STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_trigger         , ALEN(STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD_trigger)         , &STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD          },
	{E::STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION    , STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_trigger    , ALEN(STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION_trigger)    , &STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION     },
	{E::STATE_ID_SCR_LOCKDOWN_SUCCESS                    , STATE_ID_SCR_LOCKDOWN_SUCCESS_trigger                    , ALEN(STATE_ID_SCR_LOCKDOWN_SUCCESS_trigger)                    , &STATE_ID_SCR_LOCKDOWN_SUCCESS                     },
	{E::STATE_ID_SCR_DUMMY_TRANSACTION_INFO              , STATE_ID_SCR_DUMMY_TRANSACTION_INFO_trigger              , ALEN(STATE_ID_SCR_DUMMY_TRANSACTION_INFO_trigger)              , &STATE_ID_SCR_DUMMY_TRANSACTION_INFO               },
	{E::STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION     , STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_trigger     , ALEN(STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION_trigger)     , &STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION      },
	{E::STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS     , STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_trigger     , ALEN(STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS_trigger)     , &STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS      },
	{E::STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE        , STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_trigger        , ALEN(STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE_trigger)        , &STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE         },
	{E::STATE_ID_SCR_ADD_HARDWARE_ASK                    , STATE_ID_SCR_ADD_HARDWARE_ASK_trigger                    , ALEN(STATE_ID_SCR_ADD_HARDWARE_ASK_trigger)                    , &STATE_ID_SCR_ADD_HARDWARE_ASK                     },
	{E::STATE_ID_SCR_ADD_HARDWARE                        , STATE_ID_SCR_ADD_HARDWARE_trigger                        , ALEN(STATE_ID_SCR_ADD_HARDWARE_trigger)                        , &STATE_ID_SCR_ADD_HARDWARE                         },
	{E::STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE        , STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_trigger        , ALEN(STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE_trigger)        , &STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE         },
	{E::STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN          , STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_trigger          , ALEN(STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN_trigger)          , &STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN           },
	{E::STATE_ID_SCR_SHARE_YOUR_SECRETS                  , STATE_ID_SCR_SHARE_YOUR_SECRETS_trigger                  , ALEN(STATE_ID_SCR_SHARE_YOUR_SECRETS_trigger)                  , &STATE_ID_SCR_SHARE_YOUR_SECRETS                   },
	{E::STATE_ID_SCR_ADD_HARDWARE_EXIST                  , STATE_ID_SCR_ADD_HARDWARE_EXIST_trigger                  , ALEN(STATE_ID_SCR_ADD_HARDWARE_EXIST_trigger)                  , &STATE_ID_SCR_ADD_HARDWARE_EXIST                   },
	{E::STATE_ID_SCR_DASHBOARD_ALERT_INFO                , STATE_ID_SCR_DASHBOARD_ALERT_INFO_trigger                , ALEN(STATE_ID_SCR_DASHBOARD_ALERT_INFO_trigger)                , &STATE_ID_SCR_DASHBOARD_ALERT_INFO                 },
	{E::STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS             , STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_trigger             , ALEN(STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS_trigger)             , &STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS              },
	{E::STATE_ID_SCR_HEALTH_CHECK_STARTING               , STATE_ID_SCR_HEALTH_CHECK_STARTING_trigger               , ALEN(STATE_ID_SCR_HEALTH_CHECK_STARTING_trigger)               , &STATE_ID_SCR_HEALTH_CHECK_STARTING                },
	{E::STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS             , STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_trigger             , ALEN(STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS_trigger)             , &STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS              },
	{E::STATE_ID_SCR_SETUP_SECURITY_QUESTION             , STATE_ID_SCR_SETUP_SECURITY_QUESTION_trigger             , ALEN(STATE_ID_SCR_SETUP_SECURITY_QUESTION_trigger)             , &STATE_ID_SCR_SETUP_SECURITY_QUESTION              },
	{E::STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION      , STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_trigger      , ALEN(STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION_trigger)      , &STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION       },
	{E::STATE_ID_SCR_RECURRING_PAYMENTS                  , STATE_ID_SCR_RECURRING_PAYMENTS_trigger                  , ALEN(STATE_ID_SCR_RECURRING_PAYMENTS_trigger)                  , &STATE_ID_SCR_RECURRING_PAYMENTS                   },
	{E::STATE_ID_SCR_ONBOARDING                          , STATE_ID_SCR_ONBOARDING_trigger                          , ALEN(STATE_ID_SCR_ONBOARDING_trigger)                          , &STATE_ID_SCR_ONBOARDING                           },
	{E::STATE_ID_SCR_SELECT_SERVER                       , STATE_ID_SCR_SELECT_SERVER_trigger                       , ALEN(STATE_ID_SCR_SELECT_SERVER_trigger)                       , &STATE_ID_SCR_SELECT_SERVER                        },
	{E::STATE_ID_SCR_CHANGE_EMAIL                        , STATE_ID_SCR_CHANGE_EMAIL_trigger                        , ALEN(STATE_ID_SCR_CHANGE_EMAIL_trigger)                        , &STATE_ID_SCR_CHANGE_EMAIL                         },
	{E::STATE_ID_SCR_REPLACE_SELECT_KEY                  , STATE_ID_SCR_REPLACE_SELECT_KEY_trigger                  , ALEN(STATE_ID_SCR_REPLACE_SELECT_KEY_trigger)                  , &STATE_ID_SCR_REPLACE_SELECT_KEY                   },
	{E::STATE_ID_SCR_REPLACE_KEYS                        , STATE_ID_SCR_REPLACE_KEYS_trigger                        , ALEN(STATE_ID_SCR_REPLACE_KEYS_trigger)                        , &STATE_ID_SCR_REPLACE_KEYS                         },
	{E::STATE_ID_SCR_EDIT_MEMBERS                        , STATE_ID_SCR_EDIT_MEMBERS_trigger                        , ALEN(STATE_ID_SCR_EDIT_MEMBERS_trigger)                        , &STATE_ID_SCR_EDIT_MEMBERS                         },
	{E::STATE_ID_SCR_SIGN_IN_VIA_XPUB                    , STATE_ID_SCR_SIGN_IN_VIA_XPUB_trigger                    , ALEN(STATE_ID_SCR_SIGN_IN_VIA_XPUB_trigger)                    , &STATE_ID_SCR_SIGN_IN_VIA_XPUB                     },
	{E::STATE_ID_SCR_COIN_DETAILS_CONTROL                , STATE_ID_SCR_COIN_DETAILS_CONTROL_trigger                , ALEN(STATE_ID_SCR_COIN_DETAILS_CONTROL_trigger)                , &STATE_ID_SCR_COIN_DETAILS_CONTROL                 },
	{E::STATE_ID_SCR_SETUP_GROUP_WALLET                  , STATE_ID_SCR_SETUP_GROUP_WALLET_trigger                  , ALEN(STATE_ID_SCR_SETUP_GROUP_WALLET_trigger)                  , &STATE_ID_SCR_SETUP_GROUP_WALLET                   },

};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // VIEWS_H
