
#ifndef VIEWS_H
#define VIEWS_H

#include "QQuickViewer.h"
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
static const APPLICATION_STATE STATE_ID_SCR_ADD_WALLET_CONFIRMATION              = {E::STATE_ID_SCR_ADD_WALLET_CONFIRMATION             , SCR_ADD_WALLET_CONFIRMATION_Entry             , SCR_ADD_WALLET_CONFIRMATION_Exit             , LAYER::LAYER_ONSCREEN, LIMIT::NONE , SCR_ADD_WALLET_CONFIRMATION              };
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_ROOT_trigger[22] = 
{
	{E::EVT_STARTING_APPLICATION_LOCALMODE  , EVT_STARTING_APPLICATION_LOCALMODE_HANDLER  , NULL                                  },
	{E::EVT_STARTING_APPLICATION_ONLINEMODE , EVT_STARTING_APPLICATION_ONLINEMODE_HANDLER , NULL                                  },
	{E::EVT_STARTING_APPLICATION_ONLINE_HWL , EVT_STARTING_APPLICATION_ONLINE_HWL_HANDLER , NULL                                  },
	{E::EVT_SETTING_ACCOUNT_CHANGE_PASSWORD , EVT_SETTING_ACCOUNT_CHANGE_PASSWORD_HANDLER , NULL                                  },
	{E::EVT_ONS_CLOSE_ALL_REQUEST           , EVT_ONS_CLOSE_ALL_REQUEST_HANDLER           , NULL                                  },
	{E::EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST, EVT_ROOT_SIGN_IN_PRIMARY_KEY_REQUEST_HANDLER, NULL                                  },
	{E::EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST , EVT_ROOT_CREATE_PRIMARY_KEY_REQUEST_HANDLER , NULL                                  },
	{E::EVT_GOTO_HOME_WALLET_TAB            , EVT_GOTO_HOME_WALLET_TAB_HANDLER            , &STATE_ID_SCR_HOME                    },
	{E::EVT_ONS_CLOSE_REQUEST               , EVT_ONS_CLOSE_REQUEST_HANDLER               , &STATE_ID_SCR_HOME                    },
	{E::EVT_LOGIN_DB_REQUEST                , EVT_LOGIN_DB_REQUEST_HANDLER                , &STATE_ID_SCR_UNLOCK_DB               },
	{E::EVT_ROOT_PROMT_PIN                  , EVT_ROOT_PROMT_PIN_HANDLER                  , &STATE_ID_SCR_INPUT_PIN               },
	{E::EVT_ROOT_PROMT_PASSPHRASE           , EVT_ROOT_PROMT_PASSPHRASE_HANDLER           , &STATE_ID_SCR_INPUT_PASSPHRASE        },
	{E::EVT_LOGIN_MATRIX_REQUEST            , EVT_LOGIN_MATRIX_REQUEST_HANDLER            , &STATE_ID_SCR_LOGIN_ONLINE            },
	{E::EVT_GOTO_HOME_CHAT_TAB              , EVT_GOTO_HOME_CHAT_TAB_HANDLER              , &STATE_ID_SCR_HOME_ONLINE             },
	{E::EVT_ONLINE_ONS_CLOSE_REQUEST        , EVT_ONLINE_ONS_CLOSE_REQUEST_HANDLER        , &STATE_ID_SCR_HOME_ONLINE             },
	{E::EVT_SHOW_TOAST_MESSAGE              , EVT_SHOW_TOAST_MESSAGE_HANDLER              , &STATE_ID_TOAST_MESSAGE_DISPLAY       },
	{E::EVT_ROOT_UPDATE_PROFILE             , EVT_ROOT_UPDATE_PROFILE_HANDLER             , &STATE_ID_SCR_UPDATE_PROFILE          },
	{E::EVT_SHOW_CREATE_ACCOUNT_REQUEST     , EVT_SHOW_CREATE_ACCOUNT_REQUEST_HANDLER     , &STATE_ID_SCR_CREATE_ACCOUNT          },
	{E::EVT_GOTO_APP_SETTINGS_TAB           , EVT_GOTO_APP_SETTINGS_TAB_HANDLER           , &STATE_ID_SCR_APP_SETTINGS            },
	{E::EVT_SIGN_IN_REQUEST                 , EVT_SIGN_IN_REQUEST_HANDLER                 , &STATE_ID_SCR_SIGN_IN                 },
	{E::EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST  , EVT_ROOT_ENTRY_PRIMARY_KEY_REQUEST_HANDLER  , &STATE_ID_SCR_PRIMARY_KEY_ACCOUNT     },
	{E::EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST , EVT_LOGIN_WITH_SOFTWARE_KEY_REQUEST_HANDLER , &STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY },
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

static const STATE_TRIGGER STATE_ID_SCR_ADD_NEW_SIGNER_trigger[5] = 
{
	{E::EVT_ADD_HARDWARE_SIGNER_REQUEST              , EVT_ADD_HARDWARE_SIGNER_REQUEST_HANDLER              , &STATE_ID_SCR_ADD_HARDWARE_SIGNER     },
	{E::EVT_ADD_NEW_SIGNER_BACK_REQUEST              , EVT_ADD_NEW_SIGNER_BACK_REQUEST_HANDLER              , &STATE_ID_SCR_HOME                    },
	{E::EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED  , EVT_ADD_NEW_SIGNER_SOFTWARE_SIGNER_NEW_SEED_HANDLER  , &STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER },
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

static const STATE_TRIGGER STATE_ID_SCR_ADD_WALLET_CONFIRMATION_trigger[8] = 
{
	{E::EVT_ADD_WALLET_CONFIRM_CREATE           , EVT_ADD_WALLET_CONFIRM_CREATE_HANDLER           , NULL                                          },
	{E::EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST     , EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST_HANDLER     , NULL                                          },
	{E::EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR      , EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR_HANDLER      , NULL                                          },
	{E::EVT_ADD_WALLET_GENERATE_SIGNER          , EVT_ADD_WALLET_GENERATE_SIGNER_HANDLER          , NULL                                          },
	{E::EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR    , EVT_ADD_WALLET_GET_WALLET_DESCRIPTOR_HANDLER    , NULL                                          },
	{E::EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR   , EVT_ADD_WALLET_COPY_WALLET_DESCRIPTOR_HANDLER   , NULL                                          },
	{E::EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST, EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST_HANDLER, &STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION },
	{E::EVT_ADD_WALLET_SUCCESSFULLY             , EVT_ADD_WALLET_SUCCESSFULLY_HANDLER             , &STATE_ID_SCR_WALLET_INFO                     },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION_trigger[8] = 
{
	{E::EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER         , EVT_SIGNER_CONFIGURATION_SELECT_MASTER_SIGNER_HANDLER         , NULL                                   },
	{E::EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER                , EVT_SIGNER_CONFIGURATION_REMOVE_SIGNER_HANDLER                , NULL                                   },
	{E::EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER         , EVT_SIGNER_CONFIGURATION_SELECT_REMOTE_SIGNER_HANDLER         , NULL                                   },
	{E::EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE, EVT_SIGNER_CONFIGURATION_MASTER_SIGNER_SEND_PASSPHRASE_HANDLER, NULL                                   },
	{E::EVT_SIGNER_CONFIGURATION_TRY_REVIEW                   , EVT_SIGNER_CONFIGURATION_TRY_REVIEW_HANDLER                   , NULL                                   },
	{E::EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK              , EVT_ADD_WALLET_SIGNER_CONFIGURATION_BACK_HANDLER              , &STATE_ID_SCR_ADD_WALLET               },
	{E::EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW            , EVT_ADD_WALLET_SIGNER_CONFIGURATION_REVIEW_HANDLER            , &STATE_ID_SCR_ADD_WALLET_CONFIRMATION  },
	{E::EVT_ADD_SIGNER_TO_WALLET_REQUEST                      , EVT_ADD_SIGNER_TO_WALLET_REQUEST_HANDLER                      , &STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET },
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const STATE_TRIGGER STATE_ID_SCR_APP_SETTINGS_trigger[8] = 
{
	{E::EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST            , EVT_APP_SETTING_DELETE_ACCOUNT_REQUEST_HANDLER            , NULL                              },
	{E::EVT_APP_SETTING_CHANGE_PASSPHRASE                 , EVT_APP_SETTING_CHANGE_PASSPHRASE_HANDLER                 , NULL                              },
	{E::EVT_APP_SETTING_REQUEST_RESTART                   , EVT_APP_SETTING_REQUEST_RESTART_HANDLER                   , NULL                              },
	{E::EVT_APP_SETTING_REQUEST_RESCAN                    , EVT_APP_SETTING_REQUEST_RESCAN_HANDLER                    , NULL                              },
	{E::EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST, EVT_APP_SETTING_DELETE_PRIMARY_KEY_ACCOUNT_REQUEST_HANDLER, NULL                              },
	{E::EVT_APP_SETTING_BACK_REQUEST                      , EVT_APP_SETTING_BACK_REQUEST_HANDLER                      , &STATE_ID_SCR_HOME                },
	{E::EVT_APP_SETTING_BACK_TO_ONLINE_MODE               , EVT_APP_SETTING_BACK_TO_ONLINE_MODE_HANDLER               , &STATE_ID_SCR_HOME_ONLINE         },
	{E::EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST              , EVT_SHOW_REPLACE_PRIMARY_KEY_REQUEST_HANDLER              , &STATE_ID_SCR_REPLACE_PRIMARY_KEY },
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

static const STATE_TRIGGER STATE_ID_SCR_CHANGE_PASSWORD_trigger[3] = 
{
	{E::EVT_CHANGE_PASSWORD_REQUEST, EVT_CHANGE_PASSWORD_REQUEST_HANDLER, NULL                         },
	{E::EVT_CHANGE_PASSWORD_BACK   , EVT_CHANGE_PASSWORD_BACK_HANDLER   , &STATE_ID_SCR_CREATE_ACCOUNT },
	{E::EVT_CHANGE_PASSWORD_SUCCESS, EVT_CHANGE_PASSWORD_SUCCESS_HANDLED, &STATE_ID_ROOT               },
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

static const STATE_TRIGGER STATE_ID_SCR_HOME_trigger[17] = 
{
	{E::EVT_HOME_WALLET_SELECTED                 , EVT_HOME_WALLET_SELECTED_HANDLER                 , NULL                              },
	{E::EVT_HOME_WALLET_COPY_ADDRESS             , EVT_HOME_WALLET_COPY_ADDRESS_HANDLER             , NULL                              },
	{E::EVT_HOME_SETTING_REQUEST                 , EVT_HOME_SETTING_REQUEST_HANDLER                 , NULL                              },
	{E::EVT_HOME_DISPLAY_ADDRESS                 , EVT_HOME_DISPLAY_ADDRESS_HANDLER                 , NULL                              },
	{E::EVT_HOME_IMPORT_PSBT                     , EVT_HOME_IMPORT_PSBT_HANDLER                     , NULL                              },
	{E::EVT_HOME_EXPORT_BSMS                     , EVT_HOME_EXPORT_BSMS_HANDLER                     , NULL                              },
	{E::EVT_HOME_ADD_WALLET_REQUEST              , EVT_HOME_ADD_WALLET_REQUEST_HANDLER              , &STATE_ID_SCR_ADD_WALLET          },
	{E::EVT_HOME_MASTER_SIGNER_INFO_REQUEST      , EVT_HOME_MASTER_SIGNER_INFO_REQUEST_HANDLER      , &STATE_ID_SCR_MASTER_SIGNER_INFO  },
	{E::EVT_HOME_SEND_REQUEST                    , EVT_HOME_SEND_REQUEST_HANDLER                    , &STATE_ID_SCR_SEND                },
	{E::EVT_HOME_RECEIVE_REQUEST                 , EVT_HOME_RECEIVE_REQUEST_HANDLER                 , &STATE_ID_SCR_RECEIVE             },
	{E::EVT_HOME_TRANSACTION_HISTORY_REQUEST     , EVT_HOME_TRANSACTION_HISTORY_REQUEST_HANDLER     , &STATE_ID_SCR_TRANSACTION_HISTORY },
	{E::EVT_HOME_WALLET_INFO_REQUEST             , EVT_HOME_WALLET_INFO_REQUEST_HANDLER             , &STATE_ID_SCR_WALLET_INFO         },
	{E::EVT_APP_SETTING_REQUEST                  , EVT_APP_SETTING_REQUEST_HANDLER                  , &STATE_ID_SCR_APP_SETTINGS        },
	{E::EVT_HOME_TRANSACTION_INFO_REQUEST        , EVT_HOME_TRANSACTION_INFO_REQUEST_HANDLER        , &STATE_ID_SCR_TRANSACTION_INFO    },
	{E::EVT_HOME_REMOTE_SIGNER_INFO_REQUEST      , EVT_HOME_REMOTE_SIGNER_INFO_REQUEST_HANDLER      , &STATE_ID_SCR_REMOTE_SIGNER_INFO  },
	{E::EVT_HOME_ADD_NEW_SIGNER_REQUEST          , EVT_HOME_ADD_NEW_SIGNER_REQUEST_HANDLER          , &STATE_ID_SCR_ADD_NEW_SIGNER      },
	{E::EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST, EVT_HOME_COLDCARD_NFC_SIGNER_INFO_REQUEST_HANDLER, &STATE_ID_SCR_MASTER_SIGNER_INFO  },
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
	{E::EVT_HOME_ONLINE_ADD_CONTACT               , EVT_HOME_ONLINE_ADD_CONTACT_HANDLER               , &STATE_ID_SCR_ONLINE_ADD_CONTACTS     },
	{E::EVT_HOME_SHOW_ALL_PENDING_CONTACT         , EVT_HOME_SHOW_ALL_PENDING_CONTACT_HANDLER         , &STATE_ID_SCR_PENDING_REQUEST         },
	{E::EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL  , EVT_HOME_ONLINE_SIGNER_AVAILABLE_FOR_SWL_HANDLER  , &STATE_ID_SCR_CREATE_SHARED_WALLET    },
	{E::EVT_HOME_SHARED_WALLET_CONFIGURE          , EVT_HOME_SHARED_WALLET_CONFIGURE_HANDLER          , &STATE_ID_SCR_SHARED_WALLET_CONFIGURE },
	{E::EVT_HOME_BACKUP_SHARED_WALLET             , EVT_HOME_BACKUP_SHARED_WALLET_HANDLER             , &STATE_ID_SCR_BACKUP_SHARED_WALLET    },
	{E::EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL, EVT_HOME_ONLINE_SIGNER_UNAVAILABLE_FOR_SWL_HANDLER, &STATE_ID_SCR_STARTING_CREATE_SIGNER  },
	{E::EVT_HOME_ONLINE_APPSETTING_REQUEST        , EVT_HOME_ONLINE_APPSETTING_REQUEST_HANDLER        , &STATE_ID_SCR_APP_SETTINGS            },
	{E::EVT_HOME_SHARED_WL_SEND_REQUEST           , EVT_HOME_SHARED_WL_SEND_REQUEST_HANDLER           , &STATE_ID_SCR_SEND                    },
	{E::EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST  , EVT_HOME_ONLINE_TRANSACTION_INFO_REQUEST_HANDLER  , &STATE_ID_SCR_TRANSACTION_INFO        },
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

static const STATE_TRIGGER STATE_ID_SCR_LOGIN_ONLINE_trigger[10] = 
{
	{E::EVT_LOGIN_ONLINE_CREATE_ACCOUNT     , EVT_LOGIN_ONLINE_CREATE_ACCOUNT_HANDLER     , NULL                      },
	{E::EVT_LOGIN_ONLINE_SIGN_IN            , EVT_LOGIN_ONLINE_SIGN_IN_HANDLER            , NULL                      },
	{E::EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE  , EVT_LOGIN_ONLINE_SWITCH_LOCAL_MODE_HANDLER  , NULL                      },
	{E::EVT_LOGIN_ONLINE_CHANGE_PASSWORD    , EVT_LOGIN_ONLINE_CHANGE_PASSWORD_HANDLER    , NULL                      },
	{E::EVT_LOGIN_ONLINE_FORGOT_PASSWORD    , EVT_LOGIN_ONLINE_FORGOT_PASSWORD_HANDLER    , NULL                      },
	{E::EVT_LOGIN_ONLINE_RECOVER_PASSWORD   , EVT_LOGIN_ONLINE_RECOVER_PASSWORD_HANDLER   , NULL                      },
	{E::EVT_LOGIN_ONLINE_STAY_SIGNED_IN     , EVT_LOGIN_ONLINE_STAY_SIGNED_IN_HANDLER     , NULL                      },
	{E::EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE  , EVT_LOGIN_ONLINE_VERIFY_NEW_DEVICE_HANDLER  , NULL                      },
	{E::EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE, EVT_LOGIN_ONLINE_RESEND_CONFIRM_CODE_HANDLER, NULL                      },
	{E::EVT_LOGIN_ONLINE_LOGIN_SUCCEED      , EVT_LOGIN_ONLINE_LOGIN_SUCCEED_HANDLER      , &STATE_ID_SCR_HOME_ONLINE },
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

static const STATE_TRIGGER STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION_trigger[4] = 
{
	{E::EVT_PRIMARY_KEY_SIGN_IN_REQUEST       , EVT_PRIMARY_KEY_SIGN_IN_REQUEST_HANDLER       , NULL                                        },
	{E::EVT_PRIMARY_KEY_SIGN_IN_SUCCEED       , EVT_PRIMARY_KEY_SIGN_IN_SUCCEED_HANDLER       , NULL                                        },
	{E::EVT_PRIMARY_KEY_CONFIGURATION_BACK    , EVT_PRIMARY_KEY_CONFIGURATION_BACK_HANDLER    , &STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION },
	{E::EVT_PRIMARY_KEY_CONFIGURATION_FINISHED, EVT_PRIMARY_KEY_CONFIGURATION_FINISHED_HANDLER, &STATE_ID_SCR_HOME                          },
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

static const STATE_TRIGGER STATE_ID_SCR_REPLACE_PRIMARY_KEY_trigger[1] = 
{
	{E::EVT_REPLACE_PRIMARY_KEY_REQUEST, EVT_REPLACE_PRIMARY_KEY_REQUEST_HANDLER, &STATE_ID_SCR_ADD_NEW_SIGNER },
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

static const STATE_TRIGGER STATE_ID_SCR_SIGN_IN_trigger[4] = 
{
	{E::EVT_SIGN_IN_PASSWORD_REQUEST        , EVT_SIGN_IN_PASSWORD_REQUEST_HANDLER        , NULL                              },
	{E::EVT_SIGN_IN_PRIMARY_KEY_REQUEST     , EVT_SIGN_IN_PRIMARY_KEY_REQUEST_HANDLER     , NULL                              },
	{E::EVT_LOGIN_SUCCEEDED                 , EVT_LOGIN_SUCCEEDED_HANDLER                 , &STATE_ID_SCR_HOME                },
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

static const STATE_TRIGGER STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION_trigger[5] = 
{
	{E::EVT_SOFTWARE_SIGNER_REQUEST_CREATE                                   , EVT_SOFTWARE_SIGNER_REQUEST_CREATE_HANDLER                                   , NULL                                          },
	{E::EVT_ADD_SOFTWARE_SIGNER_RESULT                                       , EVT_ADD_SOFTWARE_SIGNER_RESULT_HANDLER                                       , &STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT        },
	{E::EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK                               , EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_HANDLER                               , &STATE_ID_SCR_CREATE_NEW_SEED                 },
	{E::EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION, EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION_HANDLER, &STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION },
	{E::EVT_PRIMARY_KEY_CONFIGURATION_REQUEST                                , EVT_PRIMARY_KEY_CONFIGURATION_REQUEST_HANDLER                                , &STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION       },
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
	{E::EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST, EVT_TRANSACTION_INFO_BACK_TO_CREATE_TRANSACTION_REQUEST_HANDLER, &STATE_ID_SCR_CREATE_TRANSACTION  },
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

static const STATE_TRIGGER STATE_ID_SCR_UTXOS_trigger[5] = 
{
	{E::EVT_UTXOS_SORT_REQUEST            , EVT_UTXOS_SORT_REQUEST_HANDLER            , NULL                                  },
	{E::EVT_UTXOS_BACK_REQUEST            , EVT_UTXOS_BACK_REQUEST_HANDLER            , &STATE_ID_SCR_WALLET_INFO             },
	{E::EVT_UTXOS_CONSOLIDATE_REQUEST     , EVT_UTXOS_CONSOLIDATE_REQUEST_HANDLER     , &STATE_ID_SCR_CONSOLIDATE_OUTPUT      },
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

static const STATE_TRIGGER STATE_ID_SCR_WALLET_INFO_trigger[16] = 
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

static const STATE_SYSTEM STATE_ALL[55] = 
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
	{E::STATE_ID_SCR_ADD_WALLET_CONFIRMATION             , STATE_ID_SCR_ADD_WALLET_CONFIRMATION_trigger             , ALEN(STATE_ID_SCR_ADD_WALLET_CONFIRMATION_trigger)             , &STATE_ID_SCR_ADD_WALLET_CONFIRMATION              },
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

};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // VIEWS_H
