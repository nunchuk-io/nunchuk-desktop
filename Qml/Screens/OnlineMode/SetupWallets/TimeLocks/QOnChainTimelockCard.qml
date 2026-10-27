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
import QtQuick.Controls
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Item {
    id: _card
    property string dateText: ""
    property string subText: ""
    property bool hasError: false
    readonly property bool hasValue: dateText !== ""
    signal configureClicked()
    height: subText !== "" ? 88 : 72

    QDashRectangle {
        anchors.fill: parent
        visible: !_card.hasValue
        radius: 8
        borderWitdh: 2
        borderColor: _card.hasError ? "#CF4018" : "#031F2B"
        Row {
            anchors {
                fill: parent
                margins: 12
            }
            spacing: 12
            QBadge {
                width: 48
                height: 48
                radius: 48
                iconSize: 24
                icon: "qrc:/Images/Images/Timer.svg"
                anchors.verticalCenter: parent.verticalCenter
                color: "#F5F5F5"
            }
            QLato {
                width: 150
                height: 28
                text: STR.STR_QML_1896
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        QTextButton {
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 12
            }
            width: label.paintedWidth + 2*20
            height: 36
            type: eTypeB
            label.text: STR.STR_QML_958
            label.font.pixelSize: 16
            onButtonClicked: _card.configureClicked()
        }
    }

    Rectangle {
        anchors.fill: parent
        visible: _card.hasValue
        radius: 8
        color: "#A7F0BA"
        border.width: _card.hasError ? 2 : 0
        border.color: "#CF4018"
        Row {
            anchors {
                fill: parent
                margins: 12
            }
            spacing: 12
            QBadge {
                width: 48
                height: 48
                radius: 48
                iconSize: 24
                icon: "qrc:/Images/Images/Timer.svg"
                anchors.verticalCenter: parent.verticalCenter
                color: "#F5F5F5"
            }
            Column {
                width: 150
                anchors.verticalCenter: parent.verticalCenter
                spacing: 4
                QLato {
                    width: 150
                    height: 20
                    text: STR.STR_QML_1988
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    width: 150
                    height: 20
                    text: _card.dateText
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    width: 150
                    height: 16
                    visible: _card.subText !== ""
                    font.pixelSize: 12
                    text: _card.subText
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
        QTextButton {
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 12
            }
            width: label.paintedWidth + 2*20
            height: 36
            type: eTypeB
            label.text: STR.STR_QML_1413
            label.font.pixelSize: 16
            onButtonClicked: _card.configureClicked()
        }
    }
}
