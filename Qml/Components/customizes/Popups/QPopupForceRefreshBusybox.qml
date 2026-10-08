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
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QPopup {
    id: forceRefreshBusybox
    width: parent.width
    height: parent.height
    property string addrToVerify: ""
    content: Item {
        id: boxmask
        width: 300
        height: 250
        Column {
            spacing: 16
            anchors.centerIn: parent
            QBusyIndicator {
                width: 70
                height: 70
                anchors.horizontalCenter: parent.horizontalCenter
            }
            QText {
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "Lato"
                font.pixelSize: 14
                font.weight: Font.Bold
                text: STR.STR_QML_122
            }
        }
    }

    onOpened: {
        forceRefreshTimer.start()
    }
    Timer {
        id: forceRefreshTimer
        interval: 5000
        repeat: false
        onTriggered: {
            stop()
            forceRefreshBusybox.close()
            _warning.warningType = EWARNING.SUCCESS_MSG
            _warning.warningExplain = STR.STR_QML_690
            _warning.open()
        }
    }
}
