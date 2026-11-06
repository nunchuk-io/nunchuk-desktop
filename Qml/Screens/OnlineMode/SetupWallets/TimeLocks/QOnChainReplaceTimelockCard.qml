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

Column {
    id: _card
    property var lockState: ({})
    readonly property var lock: lockState && lockState.timelock ? lockState.timelock : ({})
    readonly property var oldLock: lockState && lockState.old_timelock ? lockState.old_timelock : ({})
    readonly property bool changed: lockState && lockState.changed === true
    readonly property bool hasError: lockState && lockState.timelock_error === true
    readonly property var messages: lockState && lockState.messages !== undefined ? lockState.messages : []
    signal changeClicked()
    spacing: 4

    function lockText(l) {
        if (!l || l.display === undefined || l.display === "") return ""
        return l.isHeightLock ? qsTr("Est. %1").arg(l.display) : l.display
    }

    Rectangle {
        width: parent.width
        height: _card.lock.isHeightLock ? 88 : 72
        radius: 8
        color: _card.hasError ? "#FFFFFF" : (_card.changed ? "#A7F0BA" : "#FFFFFF")
        border.width: _card.hasError ? 2 : 1
        border.color: _card.hasError ? "#CF4018" : (_card.changed ? "#A7F0BA" : "#DEDEDE")
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
                    text: _card.lockText(_card.lock)
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    width: 150
                    height: 16
                    visible: _card.lock.isHeightLock === true
                    font.pixelSize: 12
                    text: qsTr("%1 %2").arg(_card.lock.block_height).arg(STR.STR_QML_188)
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
            onButtonClicked: _card.changeClicked()
        }
    }
    Row {
        spacing: 4
        visible: _card.changed
        QSvgImage {
            width: 16
            height: 16
            source: "qrc:/Images/Images/replace.svg"
        }
        QLato {
            text: STR.STR_QML_2054.arg(_card.lockText(_card.oldLock))
            font.pixelSize: 12
            color: "#031F2B"
        }
    }
    Repeater {
        model: _card.messages
        QLato {
            width: _card.width
            text: modelData
            color: "#CF4018"
            font.pixelSize: 12
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
        }
    }
}
