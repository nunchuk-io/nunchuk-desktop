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
#include "STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION.h"
#include "Models/AppModel.h"
#include "Signers/QSignerManagement.h"

void SCR_SOFTWARE_SIGNER_CONFIGURATION_Entry(QVariant msg) {
    AppModel::instance()->setQrExported(QStringList());
    AppModel::instance()->setAddSignerPercentage(0);
}

void SCR_SOFTWARE_SIGNER_CONFIGURATION_Exit(QVariant msg) {
    AppModel::instance()->setAddSignerPercentage(0);
}

void EVT_SOFTWARE_SIGNER_REQUEST_CREATE_HANDLER(QVariant msg) {
    QMap<QString, QVariant> dataMap = msg.toMap();
    QString signername = dataMap.value("signername").toString();
    QString passphrase = dataMap.value("passphrase").toString();
    QString mnemonic = AppModel::instance()->getMnemonic();
    bool yesBtn = dataMap.value("key_yes_accept").toBool();
    QSignerManagement::instance()->requestCreateSoftwareSigner(signername, mnemonic, passphrase, yesBtn);
}

void EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_HANDLER(QVariant msg) {}

void EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION_HANDLER(QVariant msg) {}
