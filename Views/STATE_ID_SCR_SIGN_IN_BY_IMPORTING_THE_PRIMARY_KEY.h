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


#ifndef STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_H
#define STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_Entry(QVariant msg);
void SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_Exit(QVariant msg);
void EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST_HANDLER(QVariant msg);
void EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST_HANDLER(QVariant msg);
void EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST_HANDLER(QVariant msg);
void EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SUCCEED_HANDLER(QVariant msg);
void EVT_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_BACK_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY_H
