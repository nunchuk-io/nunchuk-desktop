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
import QtQuick 2.0
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Members"
import "../../../../localization/STR_QML.js" as STR

Column {
    id: _colId
    spacing: 12
    width: 539
    property var member
    property int roleHeight: 24
    signal arrowClicked()
    QLato {
        height: 20
        text: modelData.role === "MASTER" ? STR.STR_QML_949 : STR.STR_QML_1031.arg(1 + index)
        font.pixelSize: 16
        font.weight: Font.Bold
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Rectangle {
        color: "#F5F5F5"
        radius: 12
        width: 539
        height: 164
        Item {
            anchors.fill: parent
            anchors.margins: 16
            Column {
                spacing: 16
                Item {
                    width: 507
                    height: 48
                    QAvatar {
                        width: 48
                        height: 48
                        avatarLocal: member.user ? member.user.avatar : ""
                        avatarUrl: member.user ? member.user.avatar : ""
                        username: member.user ? member.user.name : ""
                        displayStatus: false
                        activeStatus: member.status === "PENDING" ? false : true
                        anchors.verticalCenter: parent.verticalCenter
                        color: member.status === "PENDING" ? "#EAEAEA" : "#FDD95C"
                    }
                    Column {
                        id: _col
                        anchors {
                            left: parent.left
                            leftMargin: 48 + 12
                            verticalCenter: parent.verticalCenter
                        }
                        spacing: 4
                        QLato {
                            height: 20
                            text: member.user ? member.user.name : member.email_or_username
                            font.pixelSize: 16
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        QLato {
                            height: 16
                            font.pixelSize: 12
                            color: "#595959"
                            text: member.user ? member.user.email : member.email_or_username
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    QLato {
                        anchors {
                            right: parent.right
                            rightMargin: 0
                            verticalCenter: parent.verticalCenter
                        }
                        height: 16
                        font.pixelSize: 12
                        text: member.status === "PENDING" ? STR.STR_QML_469 : ""
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                QEditMemberRole {
                    boxWidth: 507
                    boxHeight: 48
                    label: STR.STR_QML_1395
                    textInputted: roles.find(function(e) { if (e.id === member.role) return true; else return false }).name
                    enabled: modelData.role !== "MASTER"
                    showArrow: modelData.role !== "MASTER"
                    input.readOnly: true
                    onArrowClicked: {
                        _colId.arrowClicked()
                    }
                }
            }
        }
    }
}

