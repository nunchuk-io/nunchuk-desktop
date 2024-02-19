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
import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _send
    property var transactionInfo
    property int pendingSignatue: 0
    property bool isDummy: false
    signal addrToVerify(var addr)
    signal newMemoNotify(var newMemo)
    signal keySignRequest(var signer)
    signal keyScanRequest
    signal keyExportRequest
    signal keyImportRequest
    property string myRole: ""
    /*========================================*/
    Row {
        spacing: 30
        QSendAddressArea {
            transactionInfo: _send.transactionInfo
            isDummy: _send.isDummy
            myRole: _send.myRole
            onAddrToVerify: _send.addrToVerify(addr)
            onNewMemoNotify: _send.newMemoNotify(newMemo)
        }
        QMemberKeysArea {
            transactionInfo: _send.transactionInfo
            pendingSignatue: _send.pendingSignatue
            myRole: _send.myRole
            isDummy: _send.isDummy
            onKeySignRequest: _send.keySignRequest(signer)
            onKeyScanRequest: _send.keyScanRequest()
            onKeyExportRequest: _send.keyExportRequest()
            onKeyImportRequest: _send.keyImportRequest()
        }
    }
}
