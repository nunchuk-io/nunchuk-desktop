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
#include "STATE_ID_SCR_CONSOLIDATE_OUTPUT.h"
#include "Models/AppModel.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"

void SCR_CONSOLIDATE_Entry(QVariant msg) {
    AppModel::instance()->setTransactionInfo(NULL);
    if(AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->escrow()){
        AppModel::instance()->showToast(0,
                                        STR_CPP_072,
                                        EWARNING::WarningType::WARNING_MSG);
    }
}

void SCR_CONSOLIDATE_Exit(QVariant msg) {

}

void EVT_CONSOLIDATE_BACK_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setTransactionInfo(NULL);
}

void EVT_CONSOLIDATE_MAKE_TRANSACTION_HANDLER(QVariant msg) {
    QString toAddress = msg.toMap().value("toAddress").toString();
    QString destinationMemo    = msg.toMap().value("destinationMemo").toString();
    qint64 toAmount = 0;
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QString amountStr = msg.toMap().value("toAmount").toString();
        amountStr.remove(",");
        toAmount = amountStr.toInt();
    }
    else{
        toAmount = qUtils::QAmountFromValue(msg.toMap().value("toAmount").toString());
    }
    QDestinationListModelPtr destinationList = QDestinationListModelPtr(new DestinationListModel());
    destinationList.data()->addDestination( toAddress, toAmount);
    AppModel::instance()->setDestinationList(destinationList);
    QUTXOListModelPtr inputs = QUTXOListModelPtr(new UTXOListModel());
    if(AppModel::instance()->utxoList()){
        for (int i = 0; i < AppModel::instance()->utxoList()->rowCount(); i++) {
            QUTXOPtr it = AppModel::instance()->utxoList()->getUTXOByIndex(i);
            if(it.data() && it.data()->selected()){
                DBG_INFO << "UTXO Selected:" << it.data()->txid() << it.data()->amountSats();
                inputs->addUTXO(it.data()->txid(),
                                it.data()->vout(),
                                it.data()->address(),
                                it.data()->amountSats(),
                                it.data()->height(),
                                it.data()->memo());
            }
        }
    }

    QMap<QString, qint64> outputs;
    if(AppModel::instance()->destinationList()){
        outputs = AppModel::instance()->destinationList()->getOutputs();
        DBG_INFO << "Destination:" << outputs;
    }

    QString wallet_id = "";
    if(AppModel::instance()->walletInfo()){
        wallet_id = AppModel::instance()->walletInfo()->id();
    }
    bool subtractFromFeeAmout = true;

    DBG_INFO << "subtract:" << subtractFromFeeAmout << "| manual Output: false" << "| manual Fee:" << true << "| free rate:" << -1;
    QWarningMessage msgwarning;
    QTransactionPtr trans = bridge::nunchukDraftTransaction(wallet_id, outputs, inputs, -1, subtractFromFeeAmout, msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
        if(trans){
            QString memo = "";
            if(AppModel::instance()->transactionInfo()){
                memo = AppModel::instance()->transactionInfo()->memo();
            }
            AppModel::instance()->setTransactionInfo(trans);
            AppModel::instance()->transactionInfo()->setMemo(memo);
            QQuickViewer::instance()->sendEvent(E::EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED);
        }
    }
    else{
        AppModel::instance()->showToast(msgwarning.code(),
                                        msgwarning.what(),
                                        (EWARNING::WarningType)msgwarning.type(),
                                        STR_CPP_073);
    }
}


void EVT_CONSOLIDATE_MAKE_TRANSACTION_SUCCEED_HANDLER(QVariant msg) {

}


