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
import Qt.labs.platform 1.1
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
import "../../../Components/customizes/Popups"
import "../../../Components/customizes/services"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1606
    extraHeader: Item {}
    property string option: ""
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property var formats: [
        {id: "not-have-a-passphrase",   description: STR.STR_QML_1604},
        {id: "have-a-passphrase",       description: STR.STR_QML_1605},
    ]
    signal requestBack()
    signal requestNext()
    content: Item {
        Column {
            anchors {
                top: parent.top
                topMargin: 0
            }
            spacing: 24
            QLato {
                width: 539
                height: 28
                text: STR.STR_QML_1607
                wrapMode: Text.WordWrap
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
                anchors.left: parent.left
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Column {
                id: _colum
                spacing: 16
                Repeater {
                    model: formats
                    Rectangle {
                        width: 573
                        height: 60
                        border.width: 2
                        border.color: _radio.selected ? "#000000" : "#DEDEDE"
                        radius: 12
                        QRadioButtonTypeA {
                            anchors {
                                left: parent.left
                                leftMargin: 18
                            }
                            id: _radio
                            width: 573
                            height: 60
                            layoutDirection: Qt.RightToLeft
                            label: modelData.description
                            selected: option === modelData.id
                            onButtonClicked: { option = modelData.id; }
                        }
                    }
                }
            }
        }
    }
    onPrevClicked: requestBack()
    nextEnable: option !== ""
    onNextClicked: {
        requestNext()
    }
}
