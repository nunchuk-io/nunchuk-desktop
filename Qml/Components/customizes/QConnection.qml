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
import NUNCHUCKTYPE 1.0
import "../origins"

Rectangle {
    property alias label: text
    property alias indicator: ico
    border.width: 1
    border.color: "#EAEAEA"
    color: "#FFFFFF"
    radius: 25
    Component {
        id: colorindicator
        Item {
            width: 12
            height: 12
            Rectangle {
                width: 12
                height: 12
                radius: 12
                anchors.centerIn: parent
                color: {
                    switch(AppSetting.connectionState){
                    case NUNCHUCKTYPE.OFFLINE: return "#CF4018"
                    case NUNCHUCKTYPE.SYNCING: return "#A66800"
                    case NUNCHUCKTYPE.ONLINE: return "#1C652D"
                    default: return ""
                    }
                }
            }
        }
    }
    Component {
        id: syncindicator
        Item {
            width: 12
            height: 12
            QImage {
                id: syncingIco
                width: 12
                height: 12
                transformOrigin: Item.Center
                anchors.centerIn: parent
                source: "qrc:/Images/Images/syncing.png"
            }

            NumberAnimation {
                target: syncingIco
                property: "rotation"
                duration: 500
                from: 0
                to: 360
                loops:Animation.Infinite
                running: true
            }
        }
    }
    Row {
        height: parent.height
        spacing: 8
        anchors.centerIn: parent
        Loader {
            id: ico
            anchors.verticalCenter: parent.verticalCenter
            sourceComponent: colorindicator
        }
        QText {
            id: text
            height: parent.height
            color: "#031F2B"
            font.pixelSize: 12
            font.weight: Font.Normal
            font.family: "Lato"
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    QText {
        height: 10
        color: "#F1FAFE"
        font.pixelSize: 8
        font.family: "Lato"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.bottom
        text: "Syncing"  + (AppSetting.syncPercent > 0 ? (" " + AppSetting.syncPercent + "%") : " ...")
        visible: false//AppSetting.connectionState === NUNCHUCKTYPE.SYNCING
    }
}
