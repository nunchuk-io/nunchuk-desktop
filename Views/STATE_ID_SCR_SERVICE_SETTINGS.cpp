

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

void SCR_SERVICE_SETTINGS_Entry(QVariant msg) {
    AppModel::instance()->setTabIndex((int)ENUNCHUCK::TabSelection::SERVICE_TAB);
}

void SCR_SERVICE_SETTINGS_Exit(QVariant msg) {

}

void EVT_REENTER_YOUR_PASSWORD_REQUEST_HANDLER(QVariant msg) {

}

void EVT_SERVICE_SUPPORT_REQUEST_HANDLER(QVariant msg) {
    CLIENT_INSTANCE->createSupportRoom();
}


