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
#include "STATE_ID_SCR_SEND.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/AppSetting.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"

void SCR_SEND_Entry(QVariant msg) {
    AppModel::instance()->setTransactionInfo(NULL);
    if(AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->escrow()){
        AppModel::instance()->showToast(0, STR_CPP_083, EWARNING::WarningType::WARNING_MSG);
    }
}

void SCR_SEND_Exit(QVariant msg) {

}

void EVT_SEND_ADD_DESTINATION_REQUEST_HANDLER(QVariant msg) {

}

void EVT_SEND_BACK_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->setTransactionInfo(NULL);
}

void EVT_SEND_CREATE_TRANSACTION_REQUEST_HANDLER(QVariant msg) {
    QString memo = msg.toMap().value("destinationMemo").toString();
    QList<QVariant> destinationInputed = msg.toMap().value("destinationList").toList();
    QDestinationListModelPtr destinationList = QDestinationListModelPtr(new DestinationListModel());
    for(QVariant var: destinationInputed){
        qint64 toAmount = 0;
        QMap<QString, QVariant> destination = var.toMap();
        if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
            QString amountStr = destination["toAmount"].toString();
            amountStr.remove(",");
            toAmount = amountStr.toLongLong();
        }
        else{
            toAmount = qUtils::QAmountFromValue(destination["toAmount"].toString());
        }
        destinationList.data()->addDestination(destination["toAddress"].toString(), toAmount);
    }
    AppModel::instance()->setDestinationList(destinationList);

    QMap<QString, qint64> outputs;
    QString wallet_id = "";
    bool subtractFromAmount = false;
    if(AppModel::instance()->walletInfo()){
        wallet_id = AppModel::instance()->walletInfo()->id();
        if(AppModel::instance()->walletInfo()->escrow()){
            subtractFromAmount = true;
        }
    }
    if(AppModel::instance()->destinationList()){
        outputs = AppModel::instance()->destinationList()->getOutputs();
        DBG_INFO << "Destination : " << outputs;
    }
    QWarningMessage msgwarning;
    QTransactionPtr trans = bridge::nunchukDraftTransaction(wallet_id,
                                                            outputs,
                                                            NULL, -1,
                                                            subtractFromAmount,
                                                            "",
                                                            msgwarning);
    if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
        if(trans){
            AppModel::instance()->setTransactionInfo(trans);
            AppModel::instance()->transactionInfo()->setMemo(memo);
            QQuickViewer::instance()->sendEvent(E::EVT_SEND_CREATE_TRANSACTION_SUCCEED);
        }
    }
    else{
        if(((int)EWARNING::WarningType::EXCEPTION_MSG == msgwarning.type()) &&
                (msgwarning.code() == nunchuk::NunchukException::COIN_SELECTION_ERROR) &&
                subtractFromAmount == false)
        {
            DBG_INFO << "Retry make draft transaction with subtract true";
            msgwarning.resetWarningMessage();
            QTransactionPtr trans = bridge::nunchukDraftTransaction(wallet_id,
                                                                    outputs,
                                                                    NULL,
                                                                    -1,
                                                                    true,
                                                                    "",
                                                                    msgwarning);
            if((int)EWARNING::WarningType::NONE_MSG == msgwarning.type()){
                if(trans){
                    AppModel::instance()->setTransactionInfo(trans);
                    AppModel::instance()->transactionInfo()->setMemo(memo);
                    QQuickViewer::instance()->sendEvent(E::EVT_SEND_CREATE_TRANSACTION_SUCCEED);
                }
            }
            else{
                AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
            }

        }
        else{
            AppModel::instance()->showToast(msgwarning.code(), msgwarning.what(), (EWARNING::WarningType)msgwarning.type());
        }
    }
}

void EVT_SEND_CREATE_TRANSACTION_SUCCEED_HANDLER(QVariant msg) {

}

void EVT_SEND_BACK_HOME_SHARED_WL_HANDLER(QVariant msg) {

}

