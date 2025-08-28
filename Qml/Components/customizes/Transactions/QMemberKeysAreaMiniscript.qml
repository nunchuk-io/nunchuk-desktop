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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Transactions/miniscript"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    id: tree
    radius: 8
    border.width: 1
    border.color: "#DEDEDE"
    property var transactionInfo
    property var miniTreeForSigning: transactionInfo.miniTreeForSigning
    property var timelockedUntil: transactionInfo.timelockedUntil
    signal keySignRequest(var signer)
    signal keyScanRequest
    width: 350
    height: 480
    Flickable {
        width: tree.width
        height: tree.height
        flickableDirection: Flickable.VerticalFlick
        ScrollBar.vertical: ScrollBar { active: true }
        contentHeight: miniscriptContainer.childrenRect.height
        contentWidth: tree.width
        clip: true
        Column {
            id: miniscriptContainer
            anchors.fill: parent
            anchors.margins: 12
            width: tree.width
            spacing: 8
            Rectangle {
                id: _timeLocked
                width: tree.width
                height: 60
                visible: (timelockedUntil !== undefined && timelockedUntil !== null) && timelockedUntil.hasLocked
                color: "#FFEAF9"
                radius: 8
                Row {
                    height: parent.height
                    spacing: 8
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 12
                    }
                    QIcon {
                        iconSize: 36
                        source: "qrc:/Images/Images/Timer.svg"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QLato {
                        width: tree.width - 40
                        height: 16
                        text: STR.STR_QML_1872.arg(timelockedUntil.activeUntil)
                        font.weight: Font.Bold
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            Column {
                id: columnMiniscript
                width: tree.width
                Repeater {
                    model: miniTreeForSigning
                    delegate: QMiniscriptPolicesTransactionSignDelegate {
                        width: tree.width - 16
                        miniscript: modelData
                    }
                }
            }
            Item {
                width: tree.width
                height: 8
            }
        }
    }
}
