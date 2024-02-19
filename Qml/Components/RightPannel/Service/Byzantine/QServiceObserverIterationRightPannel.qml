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
import "../../../origins"
import "../../../customizes"
import "../../../customizes/Chats"
import "../../../customizes/Texts"
import "../../../customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Item {
    Rectangle {
        width: parent.width
        height: 60
        color: "#D0E2FF"
        visible: ClientController.isNunchukLoggedIn
        QLato {
            text: STR.STR_QML_737
            font.weight: Font.Bold
            anchors{
                left: parent.left
                leftMargin: 24
                verticalCenter: parent.verticalCenter
            }
        }
        QIconButton {
            iconSize: 24
            anchors {
                right: parent.right
                rightMargin: 24
                verticalCenter: parent.verticalCenter
            }
            icon: "qrc:/Images/Images/right-arrow-dark.svg"
            onButtonClicked: {
                if (ServiceSetting.servicesTag.inheritanceCheck()) {
                    ServiceSetting.optionIndex = _CLAIM_AN_INHERITANCE
                } else {
                    _clainNot.showPopup()
                }
            }
            bgColor: "transparent"
        }
    }
    Item {
        width: 600
        height: 379
        anchors.centerIn: parent
        Column {
            width: parent.width
            spacing: 8
            QMontserrat {
                width: parent.width
                text: STR.STR_QML_1023
                font.pixelSize: 32
                font.weight: Font.Medium
                horizontalAlignment: Text.AlignHCenter
            }
            QLato {
                width: parent.width
                text: STR.STR_QML_1024
                horizontalAlignment: Text.AlignHCenter
            }
        }
        QImage {
            width: sourceSize.width
            height: sourceSize.height
            source: "qrc:/Images/Images/key_security_tip.svg"
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: 18
            }
        }
    }
}
