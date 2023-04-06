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


#ifndef STATE_ID_SCR_UTXO_OUTPUT_H
#define STATE_ID_SCR_UTXO_OUTPUT_H

#include <QObject>
#include <QVariant>
#include "ViewsDefines.h"
#include "ViewsEnums.h"

void SCR_UTXO_OUTPUT_Entry(QVariant msg);
void SCR_UTXO_OUTPUT_Exit(QVariant msg);
void EVT_UTXO_INFO_CHECKOUT_TX_RELATED_HANDLER(QVariant msg);
void EVT_UTXO_INFO_BACK_REQUEST_HANDLER(QVariant msg);
void EVT_UTXO_INFO_VIEW_TX_RELATED_HANDLER(QVariant msg);

#endif // STATE_ID_SCR_UTXO_OUTPUT_H
