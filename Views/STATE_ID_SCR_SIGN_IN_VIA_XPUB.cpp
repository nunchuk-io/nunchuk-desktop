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


#include "STATE_ID_SCR_SIGN_IN_VIA_XPUB.h"
#include "AppModel.h"

void SCR_SIGN_IN_VIA_XPUB_Entry(QVariant msg) {
    AppModel::instance()->initWallet();
}

void SCR_SIGN_IN_VIA_XPUB_Exit(QVariant msg) {

}

void EVT_SIGN_IN_VIA_XPUB_ENTER_HANDLER(QVariant msg) {
    QMap<QString, QVariant> maps = msg.toMap();
    QString type = maps["type"].toString();
    DBG_INFO << type;
    if (type == "continue-enter-xpub") {
        QString signature_xpub = maps["signature_xpub"].toString();
        AppModel::instance()->SignInUsingXPUBorWallet(signature_xpub);
    }
}

void EVT_SIGN_IN_VIA_XPUB_CLOSE_HANDLER(QVariant msg) {

}
