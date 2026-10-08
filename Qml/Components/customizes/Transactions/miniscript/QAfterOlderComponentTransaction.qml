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
import Qt5Compat.GraphicalEffects
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Signers"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../Components/customizes/Wallets/miniscript"
import "../../../../../localization/STR_QML.js" as STR

Item {
    property bool hasUnlocked: miniscript.hasUnlocked !== undefined ? miniscript.hasUnlocked : false
    property string description: getDescription()

    QAfterOlderComponent {
        id: baseCommon
        width: parent.width - 112 - 12
        height: parent.height
        isTransaction: true
    }

    Loader {
        id: actionLoader
        anchors {
            right: parent.right
            rightMargin: 12
        }
        height: 36
        visible: miniscript.type ===  ScriptNodeHelper.Type.OLDER || miniscript.type ===  ScriptNodeHelper.Type.AFTER
        sourceComponent: lock
        states: [
            State {
                when: description === ""
                AnchorChanges {
                    target: actionLoader
                    anchors.top: parent.top
                    anchors.verticalCenter: undefined
                }
                PropertyChanges {
                    target: actionLoader
                    anchors.topMargin: 0
                }
            },
            State {
                when: description !== ""
                AnchorChanges {
                    target: actionLoader
                    anchors.top: undefined
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        ]
    }
    
    Component {
        id: lock
        Row {
            spacing: 4
            layoutDirection: Qt.RightToLeft
            opacity: hasUnlocked ? 1.0 : 0.5
            QIcon {
                iconSize: 24
                source: hasUnlocked ? "qrc:/Images/Images/check_circle_24px_n.png" : "qrc:/Images/Images/lock-dark.svg"
                anchors.verticalCenter: parent.verticalCenter
            }
            QLato {
                anchors.verticalCenter: parent.verticalCenter
                text: hasUnlocked ? "Unlocked" : "Locked"
                font.pixelSize: 12
                color: "#1C1C1C"
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
