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
import QtGraphicalEffects 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"

QPendingGroupWalletBackground {
    id: pendingdelegateRoot
    property string name_group: ""
    property bool   isCurrentIndex: false
    property string inviter_email: ""

    signal deny()
    signal accept()
    signal buttonClicked()
    height: isInviter ? 148 : 92
    MouseArea {
        hoverEnabled: true
        propagateComposedEvents: true
        anchors.fill: parent
        onClicked: {
            if (!isInviter) {
                buttonClicked()
            }
        }
    }
    Rectangle {
        width: 8
        height: parent.height
        color: isCurrentIndex ? "#F6D65D" : "transparent"
    }
    Loader {
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        sourceComponent: isInviter ? inviterItem : pendingItem
    }
    Component {
        id: pendingItem
        Column {
            width: pendingdelegateRoot.width - 16
            Item {
                width: parent.width
                height: 44
                QLato {
                    text: name_group
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors{
                        left: parent.left
                        leftMargin: 8
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
            Item {
                width: parent.width
                height: 48
                QLato {
                    width: parent.width
                    anchors{
                        left: parent.left
                        leftMargin: 8
                        verticalCenter: parent.verticalCenter
                    }
                    text: STR.STR_QML_1638
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QIcon {
                    iconSize: 24
                    source: "qrc:/Images/Images/chevron-right-dark.svg"
                    anchors{
                        right: parent.right
                        rightMargin: 0
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }
    Component {
        id: inviterItem
        Column {
            width: pendingdelegateRoot.width - 16
            Item {
                width: parent.width
                height: 44
                QLato {
                    text: STR.STR_QML_2146
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors{
                        left: parent.left
                        leftMargin: 8
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
            Item {
                width: parent.width
                height: 104
                QLato {
                    width: 139
                    anchors{
                        left: parent.left
                        leftMargin: 12
                        verticalCenter: parent.verticalCenter
                    }
                    text: STR.STR_QML_2147.arg(inviter_email).arg(name_group)
                    font.pixelSize: 12
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 16
                    color: "#FFFFFF"
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Item {
                    width: 114
                    height: 36
                    visible: isInviter
                    anchors{
                            right: parent.right
                            rightMargin: 0
                            verticalCenter: parent.verticalCenter
                        }
                    Row {
                        anchors.centerIn: parent
                        spacing: 12
                        QTextButton {
                            width: 56
                            height: 36
                            label.text: STR.STR_QML_468
                            label.font.pixelSize: 12
                            type: eTypeF
                            onButtonClicked: {
                                accept()
                            }
                        }
                        QTextButton {
                            width: 46
                            height: 36
                            label.text: STR.STR_QML_946
                            label.font.pixelSize: 12
                            type: eTypeO
                            onButtonClicked: {
                                deny()
                            }
                        }
                    }
                }
            }            
        }
    }
}
