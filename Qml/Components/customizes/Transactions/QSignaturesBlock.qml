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
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../../localization/STR_QML.js" as STR

Column {
    width: parent.width
    spacing: 12
    Rectangle {
        width: parent.width
        height: 48
        color: "#F5F5F5"
        QText {
            text: STR.STR_QML_219
            font.family: "Lato"
            font.weight: Font.Bold
            font.pixelSize: 12
            color: "#323E4A"
            anchors {
                left: parent.left
                leftMargin: 12
                verticalCenter: parent.verticalCenter
            }
        }
    }
    Item {
        width: parent.width
        height: 64
        QText {
            id: numbersignaturepending
            text: "0 / " + transactionInfo.m
            font.pixelSize: 16
            color: "#031F2B"
            font.family: "Lato"
            font.weight: Font.Bold
            anchors {
                left: parent.left
                leftMargin: 16
                verticalCenter: parent.verticalCenter
            }
            width: 346
            height: 32
            wrapMode: Text.WordWrap
        }

        Row {
            spacing: 8
            height: 32
            anchors {
                left: parent.left
                leftMargin: 70
                verticalCenter: numbersignaturepending.verticalCenter
            }
            Rectangle {
                id: indicatorStatus
                width: 4
                height: 24
                radius: 1
                color: "#E02247"
                anchors.verticalCenter: parent.verticalCenter
            }
            QText {
                text: STR.str_QML_220(transactionInfo.m)
                font.pixelSize: 12
                font.family: "Lato"
                color: indicatorStatus.color
                width: 346
                height: 32
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
