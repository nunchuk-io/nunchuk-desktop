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
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Chats"
import "./../../../customizes/Texts"
import "./../../../customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR


Item {
    Column {
        anchors.fill: parent
        spacing: 24
        anchors{
            left: parent.left
            leftMargin: 24
            top: parent.top
            topMargin: 24
        }
        Rectangle {
            width: parent.width - 24
            height: parent.height * 0.42
            radius: 24
            color: "#D0E2FF"
            QPicture {
                anchors.centerIn: parent
                source: "qrc:/Images/Images/emergency-lockdown-illustrations.svg"
            }
        }
        Item {
            width: parent.width - 24
            height: parent.height * 0.45
            Column {
                width: parent.width
                spacing: 16
                QText {
                    text: STR.STR_QML_697
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.pixelSize: 32
                    font.weight: Font.DemiBold
                }
                Repeater {
                    id: _description
                    width: parent.width
                    readonly property var content_map: [
                        {height: 84, title:STR.STR_QML_703, icon: ""   },
                        {height: 56, title:STR.STR_QML_704, icon: "qrc:/Images/Images/1.Active.svg"   },
                        {height: 28, title:STR.STR_QML_705, icon: "qrc:/Images/Images/2.Active.svg"   },
                        {height: 28, title:STR.STR_QML_706, icon: "qrc:/Images/Images/3.Active.svg"    },
                    ]
                    model: content_map.length
                    Rectangle {
                        width: parent.width
                        height: _description.content_map[index].height
                        Row {
                            spacing: 8
                            QIcon {
                                iconSize: 24
                                id:_numicon
                                source: _description.content_map[index].icon
                                visible: source != ""
                            }
                            QText {
                                id:_numText
                                width: 591
                                height: _description.content_map[index].height
                                anchors.verticalCenter: parent.verticalCenter
                                text: _description.content_map[index].title
                                color: "#031F2B"
                                font.family: "Lato"
                                font.pixelSize: 16
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 28
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }
            }
        }
    }
    QLine
    {
        width: parent.width
        anchors {
            bottom: parent.bottom
            bottomMargin: 80
        }
    }
    QTextButton {
        width: 98
        height: 48
        label.text: STR.STR_QML_097
        label.font.pixelSize: 16
        type: eTypeE
        anchors {
            right: parent.right
            rightMargin: 24
            bottom: parent.bottom
            bottomMargin: 16
        }
        onButtonClicked: {
            var _input = {
                type: "wallet-lockdown"
            }
            QMLHandle.sendEvent(EVT.EVT_SERVICE_SELECT_WALLET_REQUEST, _input)
        }
    }
}
