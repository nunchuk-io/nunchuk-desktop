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
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: alert.title
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Column {
        width: 539
        height: 196
        spacing: 24
        QLato {
            width: 539
            height: paintedHeight
            text: STR.STR_QML_1341.arg(alert.payload.keyname)
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            lineHeight: 28
            lineHeightMode: Text.FixedHeight
        }
    }
    bottomLeft: QTextButton {
        width: 203
        height: 48
        label.text: STR.STR_QML_1340
        label.font.pixelSize: 16
        type: eTypeF
        onButtonClicked: {
            var _continue = {
                type: "cancel-key-replacement",
            }
            QMLHandle.sendEvent(EVT.EVT_DASHBOARD_ALERT_INFO_ENTER, _continue)
        }
    }
    bottomRight: QTextButton {
        width: 120
        height: 48
        label.text: STR.STR_QML_265
        label.font.pixelSize: 16
        type: eTypeE
        visible: alert.payload.can_replace
        onButtonClicked: {
            var _input = {
                type: "continue-key-replacement",
            }
            QMLHandle.sendEvent(EVT.EVT_DASHBOARD_ALERT_INFO_ENTER, _input)
        }
    }
}
