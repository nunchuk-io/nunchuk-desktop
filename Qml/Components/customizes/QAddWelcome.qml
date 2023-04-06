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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR
Rectangle {
    color: "#FFFFFF"
    signal addButtonClicked()
    signal supportButtonClicked()
    property string btnTextLink: "btnTextLink"
    property string titleSuggest: "titleSuggest"
    property string titleWelcome: "titleWelcome"
    property string content: "content"
    property string icon: "icon"
    property int contentHeight: 180
    property bool isPremiumUser: ClientController.user.isPremiumUser
    Column {
        visible: !isPremiumUser
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
        Item {width: parent.width; height: 24}
        Item {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 600
            height: contentHeight
            Rectangle {
                id: rectEmptyState
                anchors.fill: parent
                anchors.margins: 2
                color: "#FFFFFF"
                radius: 24
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: rectEmptyState.width
                        height: rectEmptyState.height
                        radius: rectEmptyState.radius
                    }
                }
                Row {
                    anchors.fill: parent
                    anchors.margins: 28
                    spacing: 24
                    QImage {
                        width: 96
                        height: 96
                        source: icon
                    }
                    Column {
                        spacing: 4
                        QText {
                            font.family: "Lato"
                            font.pixelSize: 20
                            font.weight: Font.Bold
                            text: titleSuggest
                            color: "#000000"
                        }
                        QText {
                            width: 424
                            font.family: "Lato"
                            font.pixelSize: 16
                            text: content
                            color: "#000000"
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                            wrapMode: Text.WordWrap
                        }
                        QButtonTextLink {
                            width: 122
                            height: 24
                            label: btnTextLink
                            labelFont: "Lato"
                            direction: eRIGHT
                            fontPixelSize: 16
                            onButtonClicked: {
                                addButtonClicked()
                            }
                        }
                    }
                }
            }
            DropShadow {
                anchors.fill: rectEmptyState
                horizontalOffset: 0
                verticalOffset: 0
                spread: 0
                radius: 8
                samples: 30
                color: "#aa000000"
                source: rectEmptyState
            }
        }
    }
    Column {
        visible: isPremiumUser
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
        QImage {
            width: 600
            height: 280
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
