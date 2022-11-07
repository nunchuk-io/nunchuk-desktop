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
#include "STATE_ID_SCR_INPUT_PIN.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Chats/matrixbrigde.h"
#include "localization/STR_CPP.h"

static QVariant pinObject = QVariant();

void SCR_INPUT_PIN_Entry(QVariant msg) {
    pinObject = msg;
}

void SCR_INPUT_PIN_Exit(QVariant msg) {
    pinObject = QVariant();
}

void EVT_INPUT_PIN_SEND_PIN_HANDLER(QVariant msg) {
    QString pinInputted = msg.toString();
    int state_id = pinObject.toMap().value("state_id").toInt();
    switch (state_id) {
    case E::STATE_ID_SCR_ADD_HARDWARE_SIGNER:
    case E::STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET:
    case E::STATE_ID_SCR_TRANSACTION_INFO:
    case E::STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT:
    case E::STATE_ID_SCR_MASTER_SIGNER_INFO:
    {
        int device_idx = pinObject.toMap().value("device_idx").toInt();
        if(pinInputted != ""){
            AppModel::instance()->startSendPinToDevice(state_id, device_idx, pinInputted);
        }
        else{
            QQuickViewer::instance()->sendEvent(E::EVT_INPUT_PIN_CLOSE);
            AppModel::instance()->showToast(0,
                                            0,
                                            EWARNING::WarningType::WARNING_MSG,
                                            STR_CPP_095);
        }
        break;
    }
    default:
    {
        QQuickViewer::instance()->sendEvent(E::EVT_INPUT_PIN_CLOSE);
    }
        break;
    }
}

void EVT_INPUT_PIN_CLOSE_HANDLER(QVariant msg) {

}

