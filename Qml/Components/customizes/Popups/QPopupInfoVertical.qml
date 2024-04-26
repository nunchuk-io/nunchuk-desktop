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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR
import "../../customizes/services"

QPopup {
    property string title: STR.STR_QML_024
    property string contentText: STR.STR_QML_466
    property string contentTextTwo: ""
    property Component buttons
    readonly property int mWidth: 252
    readonly property int mHeight: usingMin ? Math.min(206, itemInfo.height) : Math.max(206, itemInfo.height)
    property bool usingMin: false
    content: Item {
        width: mWidth
        height: _column.implicitHeight
        Column {
            id: _column
            spacing: 24
            anchors.fill: parent
            Column {
                spacing: 12
                QLato {
                    width: mWidth
                    font.weight: Font.Bold
                    text: title
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    width: mWidth
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: contentText
                }
                QLato {
                    width: mWidth
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: contentTextTwo
                    visible: contentTextTwo != ""
                }
            }
            Loader{
                width: mWidth
                anchors.horizontalCenter: parent.horizontalCenter
                sourceComponent: buttons
            }
        }
    }
}
