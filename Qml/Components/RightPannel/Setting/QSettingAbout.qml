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
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "./../../origins"
import "./../../customizes"
import "./../../customizes/Chats"
import "./../../customizes/Texts"
import "./../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    Column {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 24
        Item {
            height: 50
            width: parent.width
        }
        QPicture {
            anchors.horizontalCenter: parent.horizontalCenter
            source: "qrc:/Images/Images/logo-vertical-light.svg"
        }
        QText {
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Lato"
            font.pixelSize: 16
            font.weight: Font.Bold
            text: qsTr("Version %1").arg(qapplicationVersion)
        }
        Row {
            spacing: 48
            anchors.horizontalCenter: parent.horizontalCenter
            MouseArea {
                width: 96
                height: 80
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                property string twitterLink: "https://twitter.com/@nunchuk_io"
                onClicked: {
                    Qt.openUrlExternally(twitterLink)
                }
                Column {
                    anchors.fill: parent
                    spacing: 12
                    QIcon {
                        iconSize: 48
                        source: "qrc:/Images/Images/twt.png"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    QText {
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.family: "Lato"
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        text: qsTr("@nunchuk_io")
                    }
                }
            }
            MouseArea {
                width: 96
                height: 80
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                property string contactLink: "https://mail.google.com/mail/?view=cm&fs=1&tf=1&to=contact@nunchuk.io"
                onClicked: {
                    Qt.openUrlExternally(contactLink)
                }
                Column {
                    anchors.fill: parent
                    spacing: 12
                    QIcon {
                        iconSize: 48
                        source: "qrc:/Images/Images/email.png"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    QText {
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.family: "Lato"
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        text: qsTr("support@nunchuk.io")
                    }
                }
            }
        }
    }
}
