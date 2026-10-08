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
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _delegate
    height: 112
    property string oneAddress: ""
    signal signalRemoveAddress()
    signal signalInputAddress(var address)
    Column {
        spacing: 12
        Item {
            width: _delegate.width
            height: 20
            QLato {
                width: 339
                height: 20
                text: STR.STR_QML_1129 + " " + (index + 1)
                anchors.left: parent.left
                font.weight: Font.DemiBold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QTextLink {
                width: 58
                height: 20
                anchors.right: parent.right
                anchors.rightMargin: 1
                text: STR.STR_QML_1134
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                onTextClicked: {
                    signalRemoveAddress()
                }
            }
        }

        Rectangle {
            width: _delegate.width
            height: 80
            radius: 12
            color: "#F5F5F5"
            QTextInputBoxTypeB {
                id: toAddr
                label: ""
                boxWidth: _delegate.width - 16*2
                boxHeight: 48
                textInputted: oneAddress
                anchors{
                    verticalCenter: parent.verticalCenter
                    horizontalCenter: parent.horizontalCenter
                }
                onTypingFinished: {
                    signalInputAddress(toAddr.textInputted)
                }
            }
            Item {
                width: 36
                height: 36
                anchors {
                    right: toAddr.right
                    rightMargin: 6
                    verticalCenter: toAddr.verticalCenter
                }
                QIcon {
                    id: qricoimport
                    source: "qrc:/Images/Images/QR-dark.svg"
                    anchors.centerIn: parent
                    transformOrigin: Item.Center
                    scale: qrMouse.pressed ? 0.95 : 1
                    MouseArea {
                        id: qrMouse
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        anchors.fill: parent
                        onClicked: {
                            qrscaner.addressRequestIndex = index
                            qrscaner.open()
                        }
                    }
                }
            }
        }
    }
}
