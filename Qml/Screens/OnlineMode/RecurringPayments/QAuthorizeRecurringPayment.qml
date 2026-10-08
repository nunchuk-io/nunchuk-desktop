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
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1130
    extraHeader: Item {}
    property var dummyTx: AppModel.walletInfo.dummyTx
    property string title: STR.STR_QML_1041
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Column {
        width: 539
        height: 196
        spacing: 24
        QLato {
            width: 539
            height: paintedHeight
            text: title.arg(dummyTx.amountLimit).arg(AppSetting.currency)
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            lineHeight: 28
            lineHeightMode: Text.FixedHeight
        }
        QLato {
            width: 539
            height: paintedHeight
            text: STR.STR_QML_1013
            font.weight: Font.Bold
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            lineHeight: 28
            lineHeightMode: Text.FixedHeight
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    bottomLeft: QButtonTextLink {
        width: 97
        height: 48
        label: STR.STR_QML_059
        onButtonClicked: {
            prevClicked()
        }
    }
    bottomRight: Row {
        spacing: 12
        QIconText {
            icon: "qrc:/Images/Images/pending-actions-dark.svg"
            text.text: STR.str_QML_220(dummyTx.pending_signatures)
            anchors.verticalCenter: parent.verticalCenter
        }
        QTextButton {
            width: 137
            height: 48
            label.text: STR.STR_QML_572
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                var _input = {
                    type: "Ill-do-this-later"
                }
                QMLHandle.notifySendEvent(EVT.EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST, _input)
            }
        }
        QTextButton {
            width: 204
            height: 48
            label.text: STR.STR_QML_981
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                var _input = {
                    type: "Sign-dummy-transaction"
                }
                QMLHandle.notifySendEvent(EVT.EVT_HEALTH_CHECK_ACTION_ENTER_REQUEST, _input)
            }
        }
    }
}
