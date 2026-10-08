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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import Features.Claiming.ViewModels 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"

QOnScreenContent {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: ""
    property string title: STR.STR_QML_2098
    onCloseClicked: vm.close()
    Item {
        anchors.fill: parent
        anchors.margins: 36
        Column {
            width: 500
            height: 256
            spacing: 24
            Rectangle {
                width: 96;
                height: 96;
                radius: 48
                color: "#A7F0BA"
                QIcon {
                    iconSize: 60
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/check-dark.svg"
                }
            }
            Column {
                width: 500
                height: 84 + 84
                spacing: 12
                QLato {
                    width: parent.width
                    height: 40
                    text: title
                    font.pixelSize: 32
                    font.weight: Font.Medium
                    verticalAlignment: Text.AlignVCenter
                }
                Item {
                    height: 12
                    width: parent.width
                }
                QLato {
                    width: parent.width
                    height: 110
                    text: STR.STR_QML_2099
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 30
                    font.pixelSize: 18
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
            }
        }
    }
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: label.paintedWidth + 32
            height: 48
            label.text: STR.STR_QML_1977
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: vm.next()
        }
    }
    ExportCompletedViewModel {
        id: vm
    }
}
