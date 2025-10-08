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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
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
    id: common
    property bool hasEnter : miniscript.hasEnter != undefined ? miniscript.hasEnter : false
    property bool hasUnlocked : miniscript.hasUnlocked != undefined ? miniscript.hasUnlocked : false
    signal enterPreImageInput

    QCommonComponent {
        id: baseCommon
        width: parent.width - (actionLoader.visible ? actionLoader.width : -24) - 12
        height: parent.height
    }

    Loader {
        id: actionLoader
        anchors {
            top: parent.top
            topMargin: 8
            right: parent.right
            rightMargin: 12
        }
        height: 36
        visible: miniscript.type === ScriptNodeHelper.Type.HASH160 ||
                        miniscript.type ===  ScriptNodeHelper.Type.HASH256 ||
                        miniscript.type ===  ScriptNodeHelper.Type.RIPEMD160 ||
                        miniscript.type ===  ScriptNodeHelper.Type.SHA256
        sourceComponent: {
            if(hasEnter) return enterBtn
            else if (hasUnlocked) return unlocked
            else return null;
        }
    }
    Component {
        id: unlocked
        Row {
            spacing: 4
            layoutDirection: Qt.RightToLeft
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/check_circle_24px_n.png"
                anchors.verticalCenter: parent.verticalCenter
            }
            QLato {
                anchors.verticalCenter: parent.verticalCenter
                text: "Unlocked"
                font.pixelSize: 12
                color: "#1C1C1C"
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
    Component {
        id: enterBtn
        QTextButton {
            width: label.width + 36
            height: 36
            label.text: STR.STR_QML_1881
            label.font.pixelSize: 12
            label.font.family: "Lato"
            type: eTypeE
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            onButtonClicked: {
                enterPreImageInput()
             }
        }
    }
}
