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
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    id: contentarea
    width: 728
    height: 480
    radius: 12
    color: "#FFFFFF"
    border.color: "#EAEAEA"
    property var transactionInfo
    property bool isDummy: false

    signal newMemoNotify(var newMemo)
    layer.enabled: true
    layer.effect: OpacityMask {
        maskSource: Rectangle {
            width: contentarea.width
            height: contentarea.height
            radius: 12
        }
    }
    Flickable {
        id: flickcontent
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        clip: true
        interactive: contentHeight > flickcontent.height
        contentHeight: contentDisp.height
        ScrollBar.vertical: ScrollBar { active: true }
        Column {
            id: contentDisp
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5
            Rectangle {
                width: parent.width
                height: 32
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#F5F5F5"
                QLato {
                    text: STR.STR_QML_290
                    font.weight: Font.Bold
                    font.pixelSize: 12
                    anchors {
                        left: parent.left
                        leftMargin: 12
                        verticalCenter: parent.verticalCenter
                    }
                }
                QLato {
                    text: STR.STR_QML_214
                    font.weight: Font.Bold
                    font.pixelSize: 12
                    anchors {
                        right: parent.right
                        rightMargin: 12
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
            // Destination infomation
            Repeater {
                model: transactionInfo.destinationList
                width: parent.width
                Item {
                    width: parent.width
                    height: 40
                    QLato {
                        width: parent.width*2/3 - 20
                        text: destination_address
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: 12
                        anchors {
                            left: parent.left
                            leftMargin: 12
                            verticalCenter: parent.verticalCenter
                        }
                    }
                    Column {
                        width: parent.width*1/3 - 20
                        anchors {
                            right: parent.right
                            rightMargin: 12
                            verticalCenter: parent.verticalCenter
                        }
                        QLato {
                            width: parent.width
                            height: 16
                            text: destination_amount + RoomWalletData.unitValue
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 16
                            font.weight: Font.Bold
                        }
                        QLato {
                            width: parent.width
                            height: 16
                            text: qsTr("%1%2 %3")
                            .arg(AppSetting.currencySymbol)
                            .arg(model.destination_amount_currency)
                            .arg(AppSetting.currency)
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 12
                        }
                    }
                }
            }
            QTransactionNoteBlock {
                id: transationNote
                visible: !isDummy
                onMemoNotify: {
                    newMemoNotify(newMemo)
                }
            }
        }
    }
}

