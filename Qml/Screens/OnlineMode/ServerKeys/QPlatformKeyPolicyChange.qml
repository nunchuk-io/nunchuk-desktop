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
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: alert.title
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Item {
        Column {
            anchors.fill: parent
            spacing: 16
            QLato {
                width: 656
                height: paintedHeight
                text: AppModel.walletInfo.isUserWallet ? STR.STR_QML_1213 : STR.STR_QML_1010.arg(requester_user.name).arg(requester_user.email).replace("()","")
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
            }
            Flickable {
                width: 539 + 12
                height: 389
                flickableDirection: Flickable.VerticalFlick
                clip: true
                interactive: contentHeight > height
                contentHeight: contentDisp.height
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: contentDisp
                    width: parent.width
                    spacing: 16
                    QSpendingLimit
                    {
                        serverKeyInfo: AppModel.walletInfo.serverKeyInfo
                    }
                    QBroadcastAndDelay
                    {
                        serverKeyInfo: AppModel.walletInfo.serverKeyInfo
                    }
                }
            }
        }
    }
    isShowLine: true
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
