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
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../origins"

Rectangle {
    property string unspentoutput_address: ""
    property string unspentoutput_memo: ""
    property string unspentoutput_confirmation: ""
    property string unspentoutput_amount: ""
    property bool utxoSelected: false
    property int unspendWidth: 274
    property int memoWidth: 135
    property int confirmedWidth: 180
    property int amountWidth: 200

    readonly property var iconChecked: ["qrc:/Images/Images/Checked_n.png", "qrc:/Images/Images/Checked_d.png"]
    readonly property var iconUnChecked: ["qrc:/Images/Images/UnChecked_n.png", "qrc:/Images/Images/UnChecked_d.png"]

    color: btnMouse.containsMouse ?"#C9DEF1" : "transparent"

    signal buttonClicked()
    signal checkboxClicked()

    Rectangle {
        width: parent.width
        height: 1
        color: "#C9DEF1"
        anchors.bottom: parent.bottom
    }

    MouseArea {
        id: btnMouse
        hoverEnabled: true
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: { buttonClicked() }
    }

    Row {
        height: parent.height
        Item {
            width: unspendWidth
            height: parent.height

            QImage {
                id: checkboxutxo
                width: 24
                height: 24
                source: utxoSelected ? (!AppModel.walletInfo.walletEscrow ? iconChecked[0] : iconChecked[1]) :
                                        (!AppModel.walletInfo.walletEscrow ? iconUnChecked[0] : iconUnChecked[1])
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 17

                MouseArea {
                    id: btnCheckBox
                    hoverEnabled: true
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: { if(!AppModel.walletInfo.walletEscrow) checkboxClicked() }
                }
            }

            QText {
                font.pixelSize: 12
                font.family: "Lato"
                color: "#323E4A"
                anchors {
                    fill: parent
                    topMargin: 10
                    bottomMargin: 10
                    leftMargin: checkboxutxo.width + 25
                    rightMargin: 15
                }
                text: unspentoutput_address
                elide: Text.ElideRight
                wrapMode: Text.WrapAnywhere
            }
        }

        Item {
            width: memoWidth
            height: parent.height
            QText {
                anchors {
                    fill: parent
                    topMargin: 10
                    bottomMargin: 10
                    leftMargin: 15
                    rightMargin: 15
                }
                font.family: "Lato"
                font.pixelSize: 12
                color: "#323E4A"
                text: unspentoutput_memo
                wrapMode: Text.WrapAnywhere
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }

        Item {
            width: confirmedWidth
            height: parent.height
            QText {
                anchors {
                    fill: parent
                    topMargin: 10
                    bottomMargin: 10
                    leftMargin: 15
                    rightMargin: 15
                }
                font.family: "Lato"
                font.pixelSize: 12
                color: "#323E4A"
                text: unspentoutput_confirmation
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignVCenter
            }
        }

        Item {
            width: amountWidth
            height: parent.height
            QText {
                anchors {
                    fill: parent
                    topMargin: 10
                    bottomMargin: 10
                    leftMargin: 15
                    rightMargin: 15
                }
                font.family: "Lato"
                font.pixelSize: 14
                color: "#031F2B"
                text: unspentoutput_amount + RoomWalletData.unitValue
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
            }
        }
    }
}
