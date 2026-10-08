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
import "../../../../localization/STR_QML.js" as STR

QLinearGradient {
    id: pendingdelegateRoot
    radius: 0
    property string name_person: ""
    property string email_person: ""
    property bool   user_accepted: false
    property bool   isCurrentIndex: false

    signal deny()
    signal accept()
    signal dashboard()
    signal buttonClicked()
    height: user_accepted ? 40 : 130
    MouseArea {
        hoverEnabled: true
        propagateComposedEvents: true
        anchors.fill: parent
        onClicked: buttonClicked()
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
        sourceComponent: {
            if(user_accepted) {return activeItem}
            else{ return pendingItem}
        }
    }
    Component {
        id: pendingItem
        Column {
            width: pendingdelegateRoot.width - 16
            spacing: 8
            QLato {
                text: STR.STR_QML_944
                color: "#FFFFFF"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                anchors{
                    left: parent.left
                    leftMargin: 8
                }
            }
            QLato {
                width: parent.width
                height: 32
                anchors{
                    left: parent.left
                    leftMargin: 8
                }
                text: STR.STR_QML_945.arg(name_person).arg(email_person)
                color: "#FAFAFA"
                font.pixelSize: 12
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                lineHeight: 16
                lineHeightMode: Text.FixedHeight
            }
            Item {
                width: parent.width
                height: 50
                anchors.horizontalCenter: parent.horizontalCenter
                Row {
                    anchors.centerIn: parent
                    spacing: 8
                    QTextButton {
                        width: 120
                        height: 36
                        label.text: STR.STR_QML_946
                        type: eTypeO
                        onButtonClicked: {
                            deny()
                        }
                    }
                    QTextButton {
                        width: 120
                        height: 36
                        label.text: STR.STR_QML_468
                        type: eTypeF
                        onButtonClicked: {
                            accept()
                        }
                    }
                }
            }
        }
    }
    Component {
        id: activeItem
        Item {
            width: pendingdelegateRoot.width - 16
            height: 40
            QLato {
                text: STR.STR_QML_944
                color: "#FFFFFF"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                anchors{
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 8
                }
            }
            Item {
                width: 80
                height: 16
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                QTextLink {
                    width: 56
                    font.pixelSize: 10
                    text: STR.STR_QML_947
                    color: "#FFFFFF"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    onTextClicked: {
                        dashboard()
                    }
                }
            }
        }
    }
}
