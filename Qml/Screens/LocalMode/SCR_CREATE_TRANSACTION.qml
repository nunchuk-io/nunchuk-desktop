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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Transactions"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property var  txInfo: AppModel.transactionInfo
    property bool isCustomize: false
    property int  new_fee_rate: 0
    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: {
            if (txInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION) {
                return (txInfo.txidReplacing !== "") ? replace_tx : (isCustomize ? customize_dest: cancel_pending_tx)
            }
            else {
                return create_tx
            }
        }
    }
    Component {
        id: create_tx
        QCreateTransaction {

        }
    }

    Component {
        id: replace_tx
        QOnScreenReplaceByFee {

        }
    }

    Component {
        id: cancel_pending_tx
        QOnScreenCancelPendingTransaction {

        }
    }
    Component {
        id: customize_dest
        QOnScreenCustomizeDestination {

        }
    }
}
