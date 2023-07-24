

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

#include "STATE_ID_SCR_SERVICE_SETTINGS.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"
#include "Chats/ClientController.h"
#include "ServiceSetting.h"
#include "Chats/QUserWallets.h"
#include "QQuickViewer.h"
using CIStatus = ServiceSetting::CIStatus;
void SCR_SERVICE_SETTINGS_Entry(QVariant msg) {
    AppModel::instance()->setTabIndex((int)ENUNCHUCK::TabSelection::SERVICE_TAB);
    ServiceSetting::instance()->setClaimInheritanceStatus((int)CIStatus::CI_NONE);
    QStringList mnemonics = qUtils::GetBIP39WordList();
    AppModel::instance()->setSuggestMnemonics(mnemonics);
}

void SCR_SERVICE_SETTINGS_Exit(QVariant msg) {

}

void EVT_CLAIM_INHERITANCE_CHECK_REQUEST_HANDLER(QVariant msg)
{
#if 0
    Draco::instance()->inheritanceFakeUpdate();//FIXME need to remove
#else
    DBG_INFO << msg.toMap();
    QMap<QString,QVariant> maps = msg.toMap();
    QString magic = maps["magic"].toString();
    QString backupPassword = maps["backupPassword"].toString();
    int status = QUserWallets::instance()->inheritanceCheck();
    if (status == (int)CIStatus::CI_NONE) {
        QUserWallets::instance()->inheritanceDownloadBackup(magic, backupPassword);
    } else {
        ServiceSetting::instance()->setClaimInheritanceStatus(status);
    }
#endif
}

void EVT_REENTER_YOUR_PASSWORD_REQUEST_HANDLER(QVariant msg) {

}

void EVT_SERVICE_SUPPORT_REQUEST_HANDLER(QVariant msg) {
    CLIENT_INSTANCE->createSupportRoom();
}

void EVT_INHERITANCE_WITHDRAW_BALANCE_REQUEST_HANDLER(QVariant msg)
{
    ServiceSetting::instance()->setClaimInheritanceFlow(msg.toInt());
}

void EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST_HANDLER(QVariant msg)
{
    QUserWallets::instance()->serverKeyUpdatePolicies();
}

void EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED_HANDLER(QVariant msg)
{
    if (QUserWallets::instance()->secQuesAnswer()) {
        QUserWallets::instance()->serverKeyUpdatePoliciesSucceed();
    }
}

void EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST_HANLDER(QVariant msg) {
    ServiceSetting::instance()->setInheritancePlan(msg.toInt());
}

void EVT_SHARE_YOUR_SECRET_REQUEST_HANDLER(QVariant msg) {

}

void EVT_SERVICE_SELECT_WALLET_REQUEST_HANDLER(QVariant msg) {
    QWalletPtr wallet = AppModel::instance()->walletList()->getWalletById(msg.toString());
    if (!wallet.isNull() && QUserWallets::instance()->inheritanceGetPlan(wallet->id())) {
        QMap<QString, QVariant> data;
        data["state_id"] = E::STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN;
        data["walletName"] = wallet->name();
        ServiceSetting::instance()->setInheritanceWalletId(wallet->id());
        QQuickViewer::instance()->sendEvent(E::EVT_REENTER_YOUR_PASSWORD_REQUEST, data);
    }
}

void EVT_INHERITANCE_PLAN_FINALIZE_REQUEST_HANDLER(QVariant msg) {
    int option = msg.toInt();
    if (option == 2) {
        emit QUserWallets::instance()->inheritanceDiscardChangeAlert();
    } else if(option == 1) {
        QUserWallets::instance()->inheritancePlanFinalizeChanges();
    } else if(option == 3) {
        if (QUserWallets::instance()->secQuesAnswer()) {
            QUserWallets::instance()->inheritancePlanUpdate();
        }
    }
}

