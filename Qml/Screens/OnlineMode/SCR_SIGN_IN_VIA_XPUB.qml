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
import DRACO_CODE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

QScreen {
    QOnScreenContentTypeA {
        id:_content
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_1361
        extraHeader: Item {}
        property string signature_xpub: ""
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
                    height: 112
                    text: STR.STR_QML_1360
                    anchors.left: parent.left
                    font.weight: Font.Normal
                    lineHeight: 28
                    lineHeightMode: Text.FixedHeight
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QTextAreaBoxTypeA  {
                    id: _note
                    boxWidth: 573
                    boxHeight: 144
                    label: STR.STR_QML_109
                    onTypingFinished: {
                        _content.signature_xpub = _note.textInputted
                    }
                    input.placeholderText: STR.STR_QML_1361
                    isValid: true
                    input.verticalAlignment: Text.AlignTop
                    input.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
            }
        }
        onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        bottomRight: QTextButton {
            width: 99
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            enabled: _content.signature_xpub !== ""
            onButtonClicked: {
                var _input = {
                    type: "continue-enter-xpub",
                    signature_xpub: _content.signature_xpub
                }
                QMLHandle.sendEvent(EVT.EVT_SIGN_IN_VIA_XPUB_ENTER, _input)
            }
        }
    }
}
