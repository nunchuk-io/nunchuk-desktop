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
import QtQuick 2.12
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR

Column {
    spacing: 12
    width: 304
    QAvatar {
        width: 64
        height: 64
        avatarUrl: ""
        username: STR.STR_QML_535
        displayStatus: false
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Column {
        spacing: 4
        QText {
            width: 304
            height: paintedHeight
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            text: STR.STR_QML_627
            color: "#FFFFFF"
            font.family: "Lato"
            font.weight: Font.DemiBold
            font.pixelSize: 20
        }
        QText {
            width: 304
            height: paintedHeight
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            text: STR.STR_QML_628
            wrapMode: Text.WordWrap
            color: "#FFFFFF"
            font.family: "Lato"
            font.pixelSize: 16
        }
    }
    QTextButton {
        width: 289
        height: 48
        anchors.horizontalCenter: parent.horizontalCenter
        label.text: STR.STR_QML_395
        label.font.pixelSize: 16
        type: eTypeF
        onButtonClicked: {
            QMLHandle.sendEvent(EVT.EVT_SHOW_CREATE_ACCOUNT_REQUEST)
        }
    }
    QTextButton {
        width: 289
        height: 48
        anchors.horizontalCenter: parent.horizontalCenter
        label.text: STR.STR_QML_419
        label.font.pixelSize: 16
        type: eTypeC
        onButtonClicked: {
            QMLHandle.sendEvent(EVT.EVT_SIGN_IN_REQUEST)
        }
    }

}
