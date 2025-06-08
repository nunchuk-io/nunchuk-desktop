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

QOnScreenContentTypeA {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1068
    extraHeader: Item {}
    property string transaction_note: ""
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Item {
        Column {
            anchors {
                top: parent.top
                topMargin: -8
            }
            spacing: 24
            QLato {
                width: 539
                height: 28
                text: STR.STR_QML_1122
                anchors.left: parent.left
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QTextAreaBoxTypeA  {
                id: _note
                boxWidth: 573
                boxHeight: 144
                label: STR.STR_QML_218
                onTypingFinished: {
                    transaction_note = _note.textInputted
                }
                enableLengthLimit:true
                isValid: true
                maxLength: 280
                input.verticalAlignment: Text.AlignTop
                input.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }
    }
    onPrevClicked: requestBack()
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 63
            height: 48
            label.text: STR.STR_QML_1121
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                var _input = {
                    type: "apply-transaction-note",
                    transaction_note: ""
                }
                QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
            }
        }
        QTextButton {
            width: 99
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            enabled: _content.transaction_note !== ""
            onButtonClicked: {
                var _input = {
                    type: "apply-transaction-note",
                    transaction_note: _content.transaction_note
                }
                QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
            }
        }
    }
}
