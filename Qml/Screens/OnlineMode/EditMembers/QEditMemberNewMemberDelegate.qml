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
        height: 192
        Item {
            anchors.fill: parent
            anchors.margins: 16
            Column {
                spacing: 16
                QTextInputBoxTypeB {
                    id: email
                    label: STR.STR_QML_394
                    boxWidth: 507
                    boxHeight: 48
                    isValid: true
                    textInputted: modelData.email_or_username
                    onTextInputtedChanged: {
                        if(!email.isValid){
                            email.isValid = true
                            email.errorText = ""
                        }
                        email.showError = false;
                        memberIndex = index
                        memberEmail = email.textInputted
                        memberRole = modelData.role
                    }
                    onEnterKeyRequest: {
                        replaceMember()
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
                        replaceMember()
                        _colId.arrowClicked()
                    }
                }
            }
        }
    }
}

