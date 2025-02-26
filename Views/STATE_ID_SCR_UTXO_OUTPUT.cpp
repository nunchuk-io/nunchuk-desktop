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
#include "STATE_ID_SCR_UTXO_OUTPUT.h"
#include "QEventProcessor.h"
#include "Models/AppModel.h"
#include "Models/UTXOModel.h"
#include "bridgeifaces.h"

void SCR_UTXO_OUTPUT_Entry(QVariant msg) {

}

void SCR_UTXO_OUTPUT_Exit(QVariant msg) {
    if(auto w = AppModel::instance()->walletInfo()){
        AppModel::instance()->walletInfo()->setUtxoInfo(QUTXOPtr(new UTXO(w->walletId())));
    }
}

void EVT_UTXO_INFO_BACK_REQUEST_HANDLER(QVariant msg) {

}

void EVT_UTXO_INFO_CHECKOUT_TX_RELATED_HANDLER(QVariant msg) {
    if(msg.toString() != "" && AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->transactionHistory()){
        QTransactionPtr it = AppModel::instance()->walletInfo()->transactionHistory()->getTransactionByTxid(msg.toString());
        AppModel::instance()->setTransactionInfo(it);
        QEventProcessor::instance()->sendEvent(E::EVT_UTXO_INFO_VIEW_TX_RELATED);
    }
}

void EVT_UTXO_INFO_VIEW_TX_RELATED_HANDLER(QVariant msg) {

}
