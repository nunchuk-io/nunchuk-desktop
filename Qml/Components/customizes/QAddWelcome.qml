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
import NUNCHUCKTYPE 1.0
import "../origins"
import "../customizes"
import "../customizes/Chats"
import "../customizes/Texts"
import "../customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    color: "#FFFFFF"
    property string titleWelcome: "titleWelcome"
    signal supportButtonClicked()
    property alias addKey: _addKey
    property alias hotWallet: _hotWallet
    property bool isSubscribedUser: ClientController.user.isSubscribedUser
    Column {
        visible: !isSubscribedUser
        width: 600
        anchors.centerIn: parent
        spacing: 24
        QText {
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Montserrat"
            font.pixelSize: 44
            horizontalAlignment: Text.AlignHCenter
            text: titleWelcome
            color: "#000000"
        }

        QEmptyWelcome {
            id: _addKey
            anchors.horizontalCenter: parent.horizontalCenter
        }

        QLato {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            text: "Or"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            visible: _hotWallet.visible
        }

        QEmptyWelcome {
            id: _hotWallet
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
    Column {
        visible: isSubscribedUser
        width: 600
        anchors.centerIn: parent
        spacing: 12
        QText {
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Montserrat"
            font.pixelSize: 44
            horizontalAlignment: Text.AlignHCenter
            text: titleWelcome
            color: "#000000"
        }
        QPicture {
            source: "qrc:/Images/Images/chat-empty-state.svg"
        }

        QTextButton {
            width: 343
            height: 48
            type: eTypeB
            label.text: STR.STR_QML_743
            anchors.horizontalCenter: parent.horizontalCenter
            label.font.pixelSize: 16
            onButtonClicked: {
                supportButtonClicked()
            }
        }
    }
}
