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


#ifndef VIEWSDEFINES_H
#define VIEWSDEFINES_H
namespace LocalMode {
#define ROOT                                	 ""
#define SCR_ADD_HARDWARE_SIGNER             	 "qrc:/Qml/Screens/LocalMode/SCR_ADD_HARDWARE_SIGNER.qml"
#define SCR_ADD_HARDWARE_SIGNER_TO_WALLET   	 "qrc:/Qml/Screens/LocalMode/SCR_ADD_HARDWARE_SIGNER_TO_WALLET.qml"
#define SCR_ADD_MASTER_SIGNER_RESULT        	 "qrc:/Qml/Screens/LocalMode/SCR_ADD_MASTER_SIGNER_RESULT.qml"
#define SCR_ADD_NEW_SIGNER                  	 "qrc:/Qml/Screens/LocalMode/SCR_ADD_NEW_SIGNER.qml"
#define SCR_ADD_NEW_SIGNER_TO_WALLET        	 "qrc:/Qml/Screens/LocalMode/SCR_ADD_NEW_SIGNER_TO_WALLET.qml"
#define SCR_ADD_NEW_SOFTWARE_SIGNER         	 "qrc:/Qml/Screens/LocalMode/SCR_ADD_NEW_SOFTWARE_SIGNER.qml"
#define SCR_ADD_REMOTE_SIGNER_RESULT        	 "qrc:/Qml/Screens/LocalMode/SCR_ADD_REMOTE_SIGNER_RESULT.qml"
#define SCR_ADD_WALLET                      	 "qrc:/Qml/Screens/LocalMode/SCR_ADD_WALLET.qml"
#define SCR_ADD_WALLET_CONFIRMATION         	 "qrc:/Qml/Screens/LocalMode/SCR_ADD_WALLET_CONFIRMATION.qml"
#define SCR_ADD_WALLET_SIGNER_CONFIGURATION 	 "qrc:/Qml/Screens/LocalMode/SCR_ADD_WALLET_SIGNER_CONFIGURATION.qml"
#define SCR_APP_SETTINGS                    	 "qrc:/Qml/Screens/LocalMode/SCR_APP_SETTINGS.qml"
#define SCR_CONSOLIDATE_OUTPUT              	 "qrc:/Qml/Screens/LocalMode/SCR_UTXO_CONSOLIDATE.qml"
#define SCR_CREATE_NEW_SEED                 	 "qrc:/Qml/Screens/LocalMode/SCR_CREATE_NEW_SEED.qml"
#define SCR_CREATE_TRANSACTION              	 "qrc:/Qml/Screens/LocalMode/SCR_CREATE_TRANSACTION.qml"
#define SCR_HOME                            	 "qrc:/Qml/Screens/LocalMode/SCR_HOME.qml"
#define SCR_INPUT_PASSPHRASE                	 "qrc:/Qml/Screens/LocalMode/SCR_INPUT_PASSPHRASE.qml"
#define SCR_INPUT_PIN                       	 "qrc:/Qml/Screens/LocalMode/SCR_INPUT_PIN.qml"
#define SCR_MASTER_SIGNER_INFO              	 "qrc:/Qml/Screens/LocalMode/SCR_MASTER_SIGNER_INFO.qml"
#define SCR_RECEIVE                         	 "qrc:/Qml/Screens/LocalMode/SCR_RECEIVE.qml"
#define SCR_RECOVER_SOFTWARE_SIGNER         	 "qrc:/Qml/Screens/LocalMode/SCR_RECOVER_SOFTWARE_SIGNER.qml"
#define SCR_REMOTE_SIGNER_INFO              	 "qrc:/Qml/Screens/LocalMode/SCR_REMOTE_SIGNER_INFO.qml"
#define SCR_SEND                            	 "qrc:/Qml/Screens/LocalMode/SCR_SEND_ADD_DESTINATION.qml"
#define SCR_SOFTWARE_SIGNER_CONFIGURATION   	 "qrc:/Qml/Screens/LocalMode/SCR_SOFTWARE_SIGNER_CONFIGURATION.qml"
#define SCR_TRANSACTION_HISTORY             	 "qrc:/Qml/Screens/LocalMode/SCR_TRANSACTION_HISTORY.qml"
#define SCR_TRANSACTION_INFO                	 "qrc:/Qml/Screens/LocalMode/SCR_TRANSACTION_INFO.qml"
#define SCR_UNLOCK_DB                          "qrc:/Qml/Screens/LocalMode/SCR_UNLOCK_DB.qml"
#define SCR_UTXOS                           	 "qrc:/Qml/Screens/LocalMode/SCR_UTXOS.qml"
#define SCR_UTXO_OUTPUT                     	 "qrc:/Qml/Screens/LocalMode/SCR_UTXO_INFO.qml"
#define SCR_WALLET_CHANGE_ADDRESSES         	 "qrc:/Qml/Screens/LocalMode/SCR_CHANGE_ADDRESS.qml"
#define SCR_WALLET_INFO                     	 "qrc:/Qml/Screens/LocalMode/SCR_WALLET_INFO.qml"
#define SCR_ONBOARDING                         "qrc:/Qml/Screens/LocalMode/SCR_ONBOARDING.qml"
#define SCR_SELECT_SERVER                      "qrc:/Qml/Screens/LocalMode/SCR_SELECT_SERVER.qml"
#define SCR_CHANGE_EMAIL                       "qrc:/Qml/Screens/LocalMode/SCR_CHANGE_EMAIL.qml"
}

namespace OnlineMode {
#define SCR_ADD_HARDWARE                         "qrc:/Qml/Screens/OnlineMode/SCR_ADD_HARDWARE.qml"
#define SCR_ADD_HARDWARE_ASK                     "qrc:/Qml/Screens/OnlineMode/SCR_ADD_HARDWARE_ASK.qml"
#define SCR_ADD_HARDWARE_EXIST                   "qrc:/Qml/Screens/OnlineMode/SCR_ADD_HARDWARE_EXIST.qml"
#define SCR_ASSIGN_SIGNER_TO_SHARED_WALLET  	 "qrc:/Qml/Screens/OnlineMode/SCR_ASSIGN_SIGNER_TO_SHARED_WALLET.qml"
#define SCR_BACKUP_SHARED_WALLET            	 "qrc:/Qml/Screens/OnlineMode/SCR_BACKUP_SHARED_WALLET.qml"
#define SCR_CHANGE_PASSWORD                 	 "qrc:/Qml/Screens/OnlineMode/SCR_CHANGE_PASSWORD.qml"
#define SCR_CONFIGURE_SHARED_WALLET         	 "qrc:/Qml/Screens/OnlineMode/SCR_CONFIGURE_SHARED_WALLET.qml"
#define SCR_CREATE_ACCOUNT                  	 "qrc:/Qml/Screens/OnlineMode/SCR_CREATE_ACCOUNT.qml"
#define SCR_CREATE_SHARED_WALLET            	 "qrc:/Qml/Screens/OnlineMode/SCR_CREATE_SHARED_WALLET.qml"
#define SCR_DASHBOARD_ALERT_INFO                 "qrc:/Qml/Screens/OnlineMode/SCR_DASHBOARD_ALERT_INFO.qml"
#define SCR_DASHBOARD_ALERT_SUCCESS              "qrc:/Qml/Screens/OnlineMode/SCR_DASHBOARD_ALERT_SUCCESS.qml"
#define SCR_DUMMY_TRANSACTION_INFO               "qrc:/Qml/Screens/OnlineMode/SCR_DUMMY_TRANSACTION_INFO.qml"
#define SCR_EDIT_YOUR_INHERITANCE_PLAN           "qrc:/Qml/Screens/OnlineMode/SCR_EDIT_YOUR_INHERITANCE_PLAN.qml"
#define SCR_ENTER_BACKUP_PASSWORD                "qrc:/Qml/Screens/OnlineMode/SCR_ENTER_BACKUP_PASSWORD.qml"
#define SCR_HEALTH_CHECK_STARTING                "qrc:/Qml/Screens/OnlineMode/SCR_HEALTH_CHECK_STARTING.qml"
#define SCR_KEY_HEALTH_CHECK_STATUS              "qrc:/Qml/Screens/OnlineMode/SCR_KEY_HEALTH_CHECK_STATUS.qml"
#define SCR_HOME_ONLINE                     	 "qrc:/Qml/Screens/OnlineMode/SCR_HOME_ONLINE.qml"
#define SCR_KEY_RECOVERY                         "qrc:/Qml/Screens/OnlineMode/SCR_KEY_RECOVERY.qml"
#define SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION "qrc:/Qml/Screens/OnlineMode/SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION.qml"
#define SCR_LOCKDOWN_ANSER_SECURITY_QUESTION     "qrc:/Qml/Screens/OnlineMode/SCR_LOCKDOWN_ANSER_SECURITY_QUESTION.qml"
#define SCR_LOCKDOWN_SUCCESS                     "qrc:/Qml/Screens/OnlineMode/SCR_LOCKDOWN_SUCCESS.qml"
#define SCR_LOGIN_ONLINE                    	 "qrc:/Qml/Screens/OnlineMode/SCR_LOGIN_ONLINE.qml"
#define SCR_LOGIN_WITH_SOFTWARE_KEY              "qrc:/Qml/Screens/OnlineMode/SCR_LOGIN_WITH_SOFTWARE_KEY.qml"
#define SCR_ONLINE_ADD_CONTACTS             	 "qrc:/Qml/Screens/OnlineMode/SCR_ONLINE_ADD_CONTACTS.qml"
#define SCR_PENDING_REQUEST                 	 "qrc:/Qml/Screens/OnlineMode/SCR_CONTACT_PENDING_REQUEST.qml"
#define SCR_PRIMARY_KEY_ACCOUNT             	 "qrc:/Qml/Screens/OnlineMode/SCR_PRIMARY_KEY_ACCOUNT.qml"
#define SCR_PRIMARY_KEY_CONFIGURATION            "qrc:/Qml/Screens/OnlineMode/SCR_PRIMARY_KEY_CONFIGURATION.qml"
#define SCR_RECOVER_SHARED_WALLET           	 "qrc:/Qml/Screens/OnlineMode/SCR_RECOVER_SHARED_WALLET.qml"
#define SCR_REENTER_YOUR_PASSWORD                "qrc:/Qml/Screens/OnlineMode/SCR_REENTER_YOUR_PASSWORD.qml"
#define SCR_REPLACE_PRIMARY_KEY                  "qrc:/Qml/Screens/OnlineMode/SCR_REPLACE_PRIMARY_KEY.qml"
#define SCR_REVIEW_SHARED_WALLET            	 "qrc:/Qml/Screens/OnlineMode/SCR_REVIEW_SHARED_WALLET.qml"
#define SCR_SELECT_WALLET_CO_SIGN_POLICE         "qrc:/Qml/Screens/OnlineMode/SCR_SELECT_WALLET_CO_SIGN_POLICE.qml"
#define SCR_SELECT_YOUR_LOCKDOWN_PERIOD          "qrc:/Qml/Screens/OnlineMode/SCR_SELECT_YOUR_LOCKDOWN_PERIOD.qml"
#define SCR_SERVICE_SETTINGS                     "qrc:/Qml/Screens/OnlineMode/SCR_SERVICE_SETTINGS.qml"
#define SCR_SETUP_ANSWER_SECURITY_QUESTION       "qrc:/Qml/Screens/OnlineMode/SCR_SETUP_ANSWER_SECURITY_QUESTION.qml"
#define SCR_SETUP_SECURITY_QUESTION              "qrc:/Qml/Screens/OnlineMode/SCR_SETUP_SECURITY_QUESTION.qml"
#define SCR_SHARED_WALLET_CONFIGURE         	 "qrc:/Qml/Screens/OnlineMode/SCR_SHARED_WALLET_CONFIGURE.qml"
#define SCR_SHARED_WL_DEVICE_REGISTRATION   	 "qrc:/Qml/Screens/OnlineMode/SCR_SHARED_WL_DEVICE_REGISTRATION.qml"
#define SCR_SHARE_YOUR_SECRETS                   "qrc:/Qml/Screens/OnlineMode/SCR_SHARE_YOUR_SECRETS.qml"
#define SCR_SIGN_IN                              "qrc:/Qml/Screens/OnlineMode/SCR_SIGN_IN.qml"
#define SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY "qrc:/Qml/Screens/OnlineMode/SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY.qml"
#define SCR_SIGN_IN_MANUALLY                     "qrc:/Qml/Screens/OnlineMode/SCR_SIGN_IN_MANUALLY.qml"
#define SCR_STARTING_CREATE_SIGNER          	 "qrc:/Qml/Screens/OnlineMode/SCR_STARTING_CREATE_SIGNER.qml"
#define SCR_TAPSIGNER_RECOVERED_SUCCESS          "qrc:/Qml/Screens/OnlineMode/SCR_TAPSIGNER_RECOVERED_SUCCESS.qml"
#define SCR_UPDATE_PROFILE                  	 "qrc:/Qml/Screens/OnlineMode/SCR_UPDATE_PROFILE.qml"
#define SCR_INHERITANCE_CONFIRM_TRANSACTION      "qrc:/Qml/Screens/OnlineMode/SCR_INHERITANCE_CONFIRM_TRANSACTION.qml"
#define SCR_INHERITANCE_TRANSACTION_DETAILS      "qrc:/Qml/Screens/OnlineMode/SCR_INHERITANCE_TRANSACTION_DETAILS.qml"
#define SCR_INHERITANCE_WITHDRAW_BALANCE         "qrc:/Qml/Screens/OnlineMode/SCR_INHERITANCE_WITHDRAW_BALANCE.qml"
#define SCR_RECURRING_PAYMENTS                   "qrc:/Qml/Screens/OnlineMode/SCR_RECURRING_PAYMENTS.qml"
}

namespace Popups {
#define TOAST_MESSAGE_DISPLAY               	 "qrc:/Qml/Popups/TOAST_WARNING.qml"
}

#endif // VIEWSDEFINES_H
