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
import "../../origins"
import "../../customizes/Texts"

Row {
    id: radioRootC
    width: childrenRect.width + 63
    property bool isDefault: false
    property bool selected: false
    property string labelTop: "value"
    property string labelBottom: "value"
    property int labelMaxWidth: 284
    signal buttonClicked()

    spacing: 10
    QImage {
        id: icon
        width: 24
        height: 24
        source: selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
        anchors.verticalCenter: parent.verticalCenter
        MouseArea {
            id: mouse
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: { buttonClicked() }
        }
    }

    Column {
        id: info
        spacing: 4
        QText {
            text: labelTop
            color: "#031F2B"
            font.weight: Font.DemiBold
            font.pixelSize: 16
            font.family: "Lato"
            width: Math.min(labelMaxWidth, implicitWidth)
            wrapMode: Text.WordWrap
            Rectangle {
                width: 55
                height: 16
                color: "#FFFFFF"
                border.color: "#EAEAEA"
                anchors.left: parent.right
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                visible: isDefault
                radius: 20
                QText {
                    text: "Default"
                    color: "#031F2B"
                    font.weight: Font.DemiBold
                    font.pixelSize: 10
                    font.family: "Lato"
                    anchors.centerIn: parent
                }
            }
        }
        QText {
            text: labelBottom
            color: "#031F2B"
            font.weight: Font.Normal
            font.pixelSize: 12
            font.family: "Lato"
            width: Math.min(labelMaxWidth, implicitWidth)
            visible: labelBottom != ""
            wrapMode: Text.WordWrap
        }
    }
}
