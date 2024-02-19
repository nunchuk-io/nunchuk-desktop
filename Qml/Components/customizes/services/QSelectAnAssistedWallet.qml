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
import QtQuick 2.12
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import DataPool 1.0
import HMIEVENTS 1.0
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../customizes"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _id
    property var wallets:[]
    property var walletsDisabled:[]
    function contains(id) {
        for (var i = 0; i < wallets.length; i++) {
            if (wallets[i] === id)
            {
                return true
            }
        }
        return false
    }
    function allDisabled() {
        return walletsDisabled.length > 0 && walletsDisabled.length === wallets.length
    }
    property string header: STR.STR_QML_837
    signal selectWalletRequest(var wallet_id)
    Column {
        id: _column
        anchors.fill: parent
        anchors.margins: 24
        spacing: 24
        Column {
            id: _txtCol
            spacing: 16
            QLato {
                text: _id.allDisabled() ? header : STR.STR_QML_837
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 28
                font.weight: Font.Bold
            }
            QLato {
                text: STR.STR_QML_1025
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                visible: _id.allDisabled()
            }
        }
        Flickable {
            width: _id.width
            height: _column.height - 3*24 - _txtCol.childrenRect.height
            clip: true
            ScrollBar.vertical: ScrollBar { active: true }
            Column {
                spacing: 16
                Repeater {
                    model: AppModel.walletList
                    QAssistedWalletDelegate {
                        id: dele
                        width: 651
                        height: dele.visible ? 92 : 0
                        walletName: model.wallet_name
                        walletBalance: model.wallet_Balance
                        walletCurrency: model.wallet_Balance_Currency
                        walletM: model.wallet_M
                        walletN: model.wallet_N
                        visible: model.wallet_isAssistedWallet && _id.contains(model.wallet_id)
                        isLocked: model.wallet_dashboard ? model.wallet_dashboard.isLocked : false
                        onButtonClicked: {
                            selectWalletRequest(model.wallet_id)
                        }
                    }
                }
            }
        }
    }
}
