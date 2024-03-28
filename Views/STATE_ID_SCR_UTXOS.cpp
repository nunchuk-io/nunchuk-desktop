/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
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
#include "STATE_ID_SCR_UTXOS.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/UTXOModel.h"
#include "bridgeifaces.h"

void SCR_UTXOS_Entry(QVariant msg) {

}

void SCR_UTXOS_Exit(QVariant msg) {

}

void EVT_UTXOS_BACK_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setUtxoInfo(QUTXOPtr(new UTXO()));
}

void EVT_UTXOS_ITEM_REQUEST_HANDLER(QVariant msg) {
    if(msg.toInt() >= 0 && AppModel::instance()->utxoList()){
        QUTXOPtr info = AppModel::instance()->utxoList()->getUTXOByIndex(msg.toInt());
        if(info){
            AppModel::instance()->setUtxoInfo(info);
        }
    }
}

void EVT_UTXOS_SORT_REQUEST_HANDLER(QVariant msg) {
    int sortRole = msg.toMap().value("sortRole").toInt();
    int sortOrder = msg.toMap().value("sortOrder").toInt();

    if(AppModel::instance()->utxoList()){
        AppModel::instance()->utxoList()->requestSort(sortRole, sortOrder);
    }
}

void EVT_UTXOS_CONSOLIDATE_REQUEST_HANDLER(QVariant msg) {

}

void EVT_UTXOS_ITEM_SELECTED_HANDLER(QVariant msg) {
    if(msg.toInt() >= 0 && AppModel::instance()->utxoList()){
        QUTXOPtr info = AppModel::instance()->utxoList()->getUTXOByIndex(msg.toInt());
        if(info){
            AppModel::instance()->setUtxoInfo(info);
        }
    }
}

void EVT_UTXO_OUTPUT_BACK_SHARED_WALLET_HANDLER(QVariant msg) {

}

