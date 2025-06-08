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
import QtQuick 2.12
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
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1390
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Item {
        Column {
            spacing: 24
            Rectangle {
                width: 96;height: 96;
                radius: 48
                color: "#D9D9D9"
                QIcon {
                    iconSize: 60
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/add-to-collection-dark.svg"
                }
            }
            QLato {
                width: 535
                height: paintedHeight
                text: STR.STR_QML_1386
                verticalAlignment: Text.AlignVCenter
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
            }
        }
    }
    onPrevClicked: setReplaceFlow("replace-transfer-funds")
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 256
            height: 48
            label.text: STR.STR_QML_1387
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
        }
        QTextButton {
            width: 318
            height: 48
            label.text: STR.STR_QML_1388
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                var _input = {
                    type: "create-an-new-transaction"
                }
                QMLHandle.sendEvent(EVT.EVT_REPLACE_KEYS_ENTER, _input)
            }
        }
    }
}
