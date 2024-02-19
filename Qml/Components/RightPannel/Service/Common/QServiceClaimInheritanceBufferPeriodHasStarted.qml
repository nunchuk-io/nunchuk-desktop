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
    property var inheritanceInfo: ServiceSetting.servicesTag.inheritanceInfo
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
            QImage {
                anchors.centerIn: parent
                width: sourceSize.width
                height: sourceSize.height
                source: "qrc:/Images/Images/buffer-period-illustration.svg"
            }
        }
        Item {
            width: parent.width - 24
            height: parent.height * 0.58 - 24
            Column {
                width: parent.width
                spacing: 16
                QText {
                    text: STR.STR_QML_773
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.pixelSize: 32
                    font.weight: Font.DemiBold
                }
                QText {
                    text: STR.STR_QML_774
                    color: "#031F2B"
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QText {
                    text: STR.STR_QML_774_.arg(inheritance.remaining_display_name)
                    color: "#031F2B"
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
            QTextButton {
                width: 98
                height: 48
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    bottomMargin: 16
                }
                label.text: STR.STR_QML_341
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    ServiceSetting.servicesTag.clearBufferPeriodCountdown()
                }
            }
        }
    }
}
