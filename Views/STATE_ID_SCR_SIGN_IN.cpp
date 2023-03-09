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
#include "STATE_ID_SCR_SIGN_IN.h"
#include "QQuickViewer.h"
#include "Models/AppModel.h"
#include "Models/SingleSignerModel.h"
#include "Models/WalletModel.h"
#include "bridgeifaces.h"
#include "Draco.h"

void SCR_SIGN_IN_Entry(QVariant msg) {
}

void SCR_SIGN_IN_Exit(QVariant msg) {

}

void EVT_LOGIN_SUCCEEDED_HANDLER(QVariant msg) {
    QTimer::singleShot(0,[](){
        QMap<QString, QVariant> makeInstanceData;
        makeInstanceData["state_id"] = E::STATE_ID_SCR_HOME_ONLINE;
        AppModel::instance()->makeInstanceForAccount(makeInstanceData,"");
    });
}

void EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg) {

}

void EVT_SIGN_IN_PASSWORD_REQUEST_HANDLER(QVariant msg) {
    AppModel::instance()->loginNunchuk(msg);
}

void EVT_SIGN_IN_PRIMARY_KEY_REQUEST_HANDLER(QVariant msg) {
    QQuickViewer::instance()->sendEvent(E::EVT_SHOW_SIGN_IN_PRIMARY_KEY_REQUEST,true);
}


