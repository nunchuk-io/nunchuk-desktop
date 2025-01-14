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
import "../../../Components/customizes/QRCodes"
import "../../OnlineMode/Inheritances"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id:_withdraw
    visible: ServiceSetting.claimInheritanceFlow === ServiceType.WITHDRAW_TO_NUNCHUK_WALLET
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_779
    extraHeader: Item {}
    property string walletId: ""
    property int currentSelect: -1
    onCloseClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    content: Item {
        id: _item
        Column {
            anchors.fill: parent
            spacing: 24
            QLato {
                text: STR.STR_QML_782
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Flickable {
                id: flickable
                width: 455
                height: _item.height - 40
                flickableDirection: Flickable.VerticalFlick
                clip: true
                interactive: true
                contentHeight: _walletList.count * 92
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    spacing: 24
                    Repeater {
                        id: _walletList
                        model: AppModel.walletList
                        QRadioButtonTypeD {
                            width: 443
                            height: 92
                            labelTop: model.wallet_name
                            center1.text: qsTr("%1/%2 %3").arg(model.wallet_M).arg(model.wallet_N).arg(STR.STR_QML_069)
                            labelBottom: model.wallet_Balance + RoomWalletData.unitValue
                            center2.icon: model.wallet_isSharedWallet ? "qrc:/Images/Images/collab-wallet-dark.svg" :
                                                                model.wallet_isAssistedWallet ? "qrc:/Images/Images/collab-wallet-dark.svg" :
                                                                                                model.wallet_Escrow ?  "qrc:/Images/Images/Escrow Wallet.png" : ""
                            center2.text: model.wallet_isSharedWallet ? STR.STR_QML_438 :
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
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    onNextClicked: {
        if (isValid()) {
            AppModel.showToast(-1, STR.STR_QML_816, EWARNING.ERROR_MSG);
        } else {
            QMLHandle.sendEvent(EVT.EVT_INHERITANCE_CREATE_DRAFT_TX_REQUEST,walletId)
        }
    }
}


