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
import "../../customizes/Chats"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: gettingPublic
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    background: Item{}
    Rectangle {
        width: popupWidth
        height: popupHeight
        radius: 24
        color: Qt.rgba(255, 255, 255, 0)
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: popupWidth
                height: popupHeight
                radius: 24
            }
        }
        QOnScreenContent {
            id: loadingState
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            enableHeader: false
            content: Item {
                Column {
                    width: 400
                    height: 56
                    anchors.centerIn: parent
                    spacing: 16
                    QBusyIndicator {
                        width: 70
                        height: 70
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    QLato{
                        font.weight: Font.Bold
                        font.pixelSize: 20
                        text: STR.STR_QML_1683
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }
    }
    onOpened: {
        forceRefreshTimer.start()
    }
    Timer {
        id: forceRefreshTimer
        interval: 120000
        repeat: false
        onTriggered: {
            stop()
            gettingPublic.close()
        }
    }
}
