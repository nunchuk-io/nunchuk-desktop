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
import "../../../Components/origins"
import "../../../Components/customizes/Texts"

Column {
    width: parent.width
    spacing: 12
    property var emails: []
    property bool emailsChanged: false
    property bool notifyEnabled: false
    property bool notifyEnabledChanged: false
    QLato {
        width: parent.width
        height: paintedHeight
        text: STR.STR_QML_865
        font.weight: Font.Bold
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
    Rectangle {
        width: parent.width
        height: columnContent.childrenRect.height + 24
        color: "#F5F5F5"
        radius: 12
        Column {
            id: columnContent
            anchors {
                fill: parent
                margins: 12
            }
            spacing: 16
            Item {
                id: _email
                width: parent.width
                height: Math.max(40, Math.max(1, emails.length) * 20)
                QLato {
                    width: 302
                    height: 40
                    anchors.left: parent.left
                    text: STR.STR_QML_867
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Column {
                    width: 302
                    anchors.right: parent.right
                    spacing: 1
                    QLato {
                        width: 302
                        height: 20
                        font.weight: Font.Bold
                        text: "None"
                        color: "#031F2B"
                        horizontalAlignment: Text.AlignRight
                        visible: emails.length === 0
                    }
                    Repeater {
                        model: emails
                        QLato {
                            width: 302
                            height: 20
                            font.weight: Font.Bold
                            text: emails[index]
                            color: emailsChanged ? "#CF4018" : "#031F2B"
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                }
            }
            Rectangle {
                width: parent.width
                height: 1
                color: "#EAEAEA"
            }
            Item {
                id: _notify
                width: parent.width
                height: 20
                QLato {
                    width: 245
                    height: 20
                    anchors.left: parent.left
                    text: STR.STR_QML_868
                }
                QLato {
                    width: 245
                    height: 20
                    font.weight: Font.Bold
                    anchors.right: parent.right
                    text: notifyEnabled ? STR.STR_QML_433 : STR.STR_QML_432
                    color: notifyEnabledChanged ? "#CF4018" : "#031F2B"
                    horizontalAlignment: Text.AlignRight
                }
            }
        }
    }
}