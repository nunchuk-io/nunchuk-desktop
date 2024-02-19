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
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    property var alert: GroupWallet.dashboardInfo.alert
    property var dummyTx: AppModel.walletInfo.dummyTx
    property var requester_user: dummyTx.requester_user
    property string walletName: AppModel.walletInfo.walletName
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: alert.title
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    content: Column {
        width: 539
        height: 196
        spacing: 24
        QLato {
            width: 539
            height: paintedHeight
            text: STR.STR_QML_1042.arg(requester_user.name).arg(requester_user.email).arg(walletName).replace("()","")
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            lineHeight: 28
            lineHeightMode: Text.FixedHeight
        }
        QLato {
            width: 539
            height: paintedHeight
            text: STR.STR_QML_1043
            font.weight: Font.Bold
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            lineHeight: 28
            lineHeightMode: Text.FixedHeight
        }
    }

    bottomLeft: Item{}
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 70
            height: 48
            label.text: STR.STR_QML_946
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                _info.open()
            }
        }
        QTextButton {
            width: 94
            height: 48
            label.text: STR.STR_QML_1044_Approve
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                var _input = {
                    type: "inheritance-plan-approve",
                }
                QMLHandle.sendEvent(EVT.EVT_DASHBOARD_ALERT_INFO_ENTER, _input)
            }
        }
    }
    QConfirmYesNoPopup {
        id:_info
        contentText: STR.STR_QML_1045
        onConfirmNo: close()
        onConfirmYes: {
            close()
            var _input = {
                type: "inheritance-plan-deny",
            }
            QMLHandle.sendEvent(EVT.EVT_DASHBOARD_ALERT_INFO_ENTER, _input)
        }
    }
}
