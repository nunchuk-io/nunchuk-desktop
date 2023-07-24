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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property string walletId: ""
    property int currentSelect: -1
    QOnScreenContentTypeB {
        id:_withdraw
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_837
        extraHeader: Item {}
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, EVT.STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE)
        }
        content: Item {
            Column {
                spacing: 24
                QLato {
                    text: STR.STR_QML_838
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Repeater {
                    model: AppModel.walletList
                    QRadioButtonTypeD {
                        width: 443
                        height: model.wallet_isAssistedWallet ? 92 : 0
                        visible: model.wallet_isAssistedWallet
                        labelTop: model.wallet_name
                        labelCenter: qsTr("%1/%2 %3").arg(model.wallet_M).arg(model.wallet_N).arg(STR.STR_QML_069)
                        labelBottom: model.wallet_Balance + RoomWalletData.unitValue
                        icon: model.wallet_isSharedWallet ? "qrc:/Images/Images/OnlineMode/Joint wallet_031F2B.png" :
                                                                 model.wallet_isAssistedWallet ? "qrc:/Images/Images/OnlineMode/Joint wallet_031F2B.png" :
                                                                                              model.wallet_Escrow ?  "qrc:/Images/Images/OnlineMode/Escrow Wallet.png" : ""
                        type: model.wallet_isSharedWallet ? STR.STR_QML_438 :
                                                                 model.wallet_isAssistedWallet ? STR.STR_QML_679 : ""
                        selected: currentSelect === index
                        onButtonClicked: {
                            currentSelect = index
                            walletId = model.wallet_id
                        }
                    }
                }
            }
        }
        onPrevClicked: {
            QMLHandle.sendEvent(EVT.EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, EVT.STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE)
        }
        onNextClicked: {
            QMLHandle.sendEvent(EVT.EVT_SELECT_WALLET_REQUEST ,walletId)
        }
    }
    property string destinationAddress: ""

}
