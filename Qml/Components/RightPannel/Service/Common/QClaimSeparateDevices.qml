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
import QtQuick
import NUNCHUCKTYPE 1.0
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Buttons"
import "./../../../customizes/Popups"
import "./../../../customizes/services"
import "./../../../customizes/Texts"
import "../../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id: _separate
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    property int keyCount: 2
    readonly property bool twoKeys: keyCount <= 2
    label.text: twoKeys ? STR.STR_QML_2583.arg(2) : STR.STR_QML_2584
    content: Item {
        Row {
            spacing: 36
            Rectangle {
                width: 346
                height: 512
                radius: 24
                color: "#D0E2FF"
                QPicture {
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/Images/Images/how_many_keys_or_seedphrase.svg"
                }
            }
            Column {
                width: 346
                spacing: 16
                QLato {
                    width: parent.width
                    text: STR.STR_QML_2585
                    font.weight: Font.Bold
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                }
                QLato {
                    width: parent.width
                    text: _separate.twoKeys ? STR.STR_QML_2586 : STR.STR_QML_2588
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                }
                QLato {
                    width: parent.width
                    text: _separate.twoKeys ? STR.STR_QML_2587.arg(2) : STR.STR_QML_2589
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                }
            }
        }
    }
}
