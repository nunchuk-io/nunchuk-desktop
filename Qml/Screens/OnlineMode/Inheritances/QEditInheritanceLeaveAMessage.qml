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
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.12
import QtQuick.Controls.Styles 1.4
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
    id: _Message
    property var planInfo
    property string message: planInfo.note
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_920
    onCloseClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    content: Item {
        Column {
            anchors.fill: parent
            anchors.top: parent.top
            anchors.topMargin: -16
            spacing: 24
            QLato {
                width: 539
                height: 56
                text: STR.STR_QML_859
                anchors.left: parent.left
                font.weight: Font.Normal
                wrapMode: Text.WrapAnywhere
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QTextAreaBoxTypeA {
                label: STR.STR_QML_860
                optional: " (optional)"
                boxWidth: 539
                boxHeight: 276
                isValid: true
                textInputted: message
                onTextInputtedChanged: {
                    message = textInputted
                }

                input.verticalAlignment: Text.AlignTop
                input.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }
    }
    bottomRight: QTextButton {
        width: 195
        height: 48
        label.text: STR.STR_QML_861
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            _Message.nextClicked()
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    onNextClicked: {
        var _edit = {
            "note": message
        }
        inheritancePlanInfo.editPlanInfo(_edit)
        closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    }
}

