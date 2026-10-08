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
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import "../../../origins"
import "../../../customizes/Texts"
import "../../../../../localization/STR_QML.js" as STR

Rectangle {
    width: 393
    height: 92
    color: "#FFFFFF"
    radius: 12
    property var magicPhrases: []
    property string magic: ""

    Row {
        spacing: 12
        anchors.fill: parent
        anchors.margins: 12
        QIcon {
            iconSize: 24
            source: "qrc:/Images/Images/star-dark.png"
        }
        Column {
            id: _magicPhrase
            spacing: 12
            visible: magicPhrases.length === 0
            QLato {
                font.pixelSize: 16
                text: STR.STR_QML_749
                font.weight: Font.Bold
            }
            QLato {
                font.pixelSize: 16
                width: parent.width
                text: magic != "" ? magic : "null null null"
            }
        }        

        Column {
            id: _magicPhrases
            spacing: 12
            visible: magicPhrases.length > 0
            QLato {
                font.pixelSize: 16
                height: 20
                text: STR.STR_QML_749
                font.weight: Font.Bold
            }
            Flickable {
                width: 283
                height: 40
                clip: true
                contentHeight: infoColumn.implicitHeight
                flickableDirection: Flickable.VerticalFlick
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: infoColumn
                    anchors {
                        fill: parent
                        topMargin: -12
                    }
                    Repeater {
                        model: magicPhrases
                        Rectangle {
                            width: 283
                            height: 64
                            color: "#FFFFFF"
                            Column {
                                anchors {
                                    fill: parent
                                    topMargin: 12
                                    bottomMargin: 12
                                }
                                width: parent.width
                                spacing: 0
                                QLato {
                                    font.pixelSize: 16
                                    width: parent.width
                                    height: 20
                                    font.weight: Font.DemiBold
                                    text: modelData.email ?? "null null null"
                                }
                                QLato {
                                    font.pixelSize: 16
                                    height: 20
                                    width: parent.width
                                    text: modelData.magic ?? "null null null"
                                }
                                Item {
                                    width: parent.width
                                    height: 11
                                }
                                QLine {
                                    width: parent.width
                                    height: 1
                                    color: "#DEDEDE"
                                    visible: index < magicPhrases.length - 1
                                }
                            }                    
                        }
                    }
                }
            }            
        }
    }
}
