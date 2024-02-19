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
import QtGraphicalEffects 1.12
import QtQuick.Controls 2.5
import QRCodeItem 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/RightPannel/Wallet/Dashboard"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: membersGroup
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape
    background: Item{}
    signal requestSignature(var membership_id)

    Rectangle {
        id: qrmask
        width: 600
        height: 500
        radius: 24
        color: "#FFFFFF"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: 600
                height: 548
                radius: 24
            }
        }
        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 24
                top: parent.top
                topMargin: 32
            }
            onClicked: membersGroup.close()
        }
        QText {
            anchors {
                left: parent.left
                leftMargin: 36
                top: parent.top
                topMargin: 36
            }
            font.family: "Montserrat"
            font.pixelSize: 32
            text: "Select a member"
        }
        QText {
            anchors {
                left: parent.left
                leftMargin: 36
                top: parent.top
                topMargin: 84
            }
            font.family: "Lato"
            font.pixelSize: 16
            text: "Whom do you like to request signature from?"
        }
        Column {
            anchors {
                top: parent.top
                topMargin: 136
                horizontalCenter: parent.horizontalCenter
            }
            Item {
                width: 530
                height: 280
                anchors.horizontalCenter: parent.horizontalCenter
                QListView {
                    anchors.fill: parent
                    model: GroupWallet.dashboardInfo.memberSignatures
                    clip: true
                    ScrollBar.vertical: ScrollBar { active: true }
                    spacing: 16
                    delegate: QMemberDelegate {
                        width: ListView.view.width - 24
                        member: modelData
                        MouseArea {
                            id: _mouse
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                requestSignature(modelData.membership_id)
                                membersGroup.close()
                            }
                        }
                    }
                }
            }
            Item {
                width: 600
                height: 80
                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#DEDEDE"
                }
                QButtonTextLink {
                    height: 24
                    label: STR.STR_QML_245
                    displayIcon: false
                    anchors {
                        left: parent.left
                        leftMargin: 36
                        verticalCenter: parent.verticalCenter
                    }
                    onButtonClicked: {
                        membersGroup.close()
                    }
                }
            }
        }
    }
    DropShadow {
        anchors.fill: qrmask
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: qrmask
    }
}
