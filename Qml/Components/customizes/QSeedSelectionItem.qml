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
import QtGraphicalEffects 1.0
import "../origins"

Column {
    width: 550
    height: 60
    spacing: 16
    property var        indexArray: ["","",""]
    property var        phraseArray: ["","",""]
    property int        indexNeeded: -1
    property string     phraseNeeded: ""
    property string     phraseSelect: ""
    readonly property bool correct: phraseNeeded === phraseNeeded && randomlist.currentIndex !== -1

    signal itemClicked(var index)

    QText {
        width: parent.width
        height: parent.height - parent.spacing - randomlist.height
        font.family: "Lato"
        font.pixelSize: 16
        font.weight: Font.Bold
        color: "#031F2B"
        text: "Word #" + (indexNeeded+1)
    }

    ListView {
        id: randomlist
        width: 170*count+(count-1)*spacing
        height: 48
        model: 3
        spacing: 20
        layoutDirection : Qt.RightToLeft
        orientation: ListView.Horizontal
        interactive: false
        currentIndex: -1
        delegate: Item {
            width: 170
            height: 48
            Rectangle {
                anchors.fill: parent
                color: randomlist.currentIndex === index ? "#031F2B" : "#FFFFFF"
                border.color: "#031F2B"
                radius: 8
                QText {
                    anchors.centerIn: parent
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    color: randomlist.currentIndex === index ? "#FFFFFF" : "#031F2B"
                    text: phraseArray[index]
                }
                MouseArea {
                    id: selectMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        randomlist.currentIndex = index
                        phraseSelect = phraseArray[index]
                        itemClicked(index)
                    }
                }
            }
        }
    }
}
