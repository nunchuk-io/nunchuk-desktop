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
import NUNCHUCKTYPE 1.0
import EWARNING 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../OnlineMode/AddHardwareKeys"
import "../../../localization/STR_QML.js" as STR

QScreen {
    readonly property int hardwareType: GroupWallet.qIsByzantine ? GroupWallet.qAddHardware : UserWallet.qAddHardware
    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: {
            switch(hardwareType) {
            case NUNCHUCKTYPE.ADD_LEDGER: return _Ledger
            case NUNCHUCKTYPE.ADD_TREZOR: return _Trezor
            case NUNCHUCKTYPE.ADD_COLDCARD: return _Coldcard
            case NUNCHUCKTYPE.ADD_BITBOX: return _BitBox
            default: return null
            }
        }
    }
    Component {
        id: _Ledger
        QScreenAddLedgerExist {}
    }
    Component {
        id: _Trezor
        QScreenAddTrezorExist {}
    }
    Component {
        id: _Coldcard
        QScreenAddColdcardExist {}
    }
    Component {
        id: _BitBox
        QScreenAddBitBoxExist {}
    }
    function doneAddHardwareKey() {
        AppModel.showToast(0, STR.STR_QML_1392, EWARNING.SUCCESS_MSG);
        closeTo(NUNCHUCKTYPE.WALLET_TAB)
    }
}
