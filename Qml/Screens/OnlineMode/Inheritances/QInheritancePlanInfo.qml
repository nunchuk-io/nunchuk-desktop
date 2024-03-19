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

QOnScreenContentTypeB {
    id:_content
    property var alert: GroupWallet.dashboardInfo.alert
    property var dummyTx: AppModel.walletInfo.dummyTx
    property var requester_user: dummyTx.requester_user
    property string walletName: AppModel.walletInfo.walletName
    property string description: ""
    property string descriptionHb: ""
    property var planInfo
    property bool planIsCancel: false
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: alert.title
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    content: QInheritancePlanDetails {
        title:  {
            if (AppModel.walletInfo.isUserWallet) {
                var _old = AppModel.walletInfo.inheritancePlanInfo.planInfoOld.activation_date
                var _new = AppModel.walletInfo.inheritancePlanInfo.planInfoNew.activation_date
                descriptionHb.arg(walletName).arg(_old).arg(_new).replace("()","")
            }
            else {
                description.arg(requester_user.name).arg(requester_user.email).arg(walletName).replace("()","")
            }
        }
        inheritance: planInfo
        isCancel: planIsCancel
    }
    isShowLine: false
    bottomRight: QTextButton {
        width: 252
        height: 48
        label.text: STR.STR_QML_1011.arg(alert.payload.pending_signatures).arg(alert.payload.pending_signatures > 1 ? "s":"")
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            QMLHandle.sendEvent(EVT.EVT_HEALTH_CHECK_STARTING_REQUEST)
        }
    }
}
