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
    id: wldlg
    width: 304
    height: 92
    property string walletName      : "Name"
    property string walletBalance   : "0.0000000"
    property string walletCurrency  : "0.0000000"
    property string walletM: "0"
    property string walletN: "0"
    property bool   isLocked: false
    property bool   isReplaced: false
    iEnabled: !isLocked

    Column{
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 24
        }
        QLato{
            font.weight: Font.Bold
            text: walletName
            color: "#FFFFFF"
        }
        QLato{
            font.weight: Font.Bold
            text: walletBalance + RoomWalletData.unitValue
            color: "#FFFFFF"
        }
        QLato{
            text: qsTr("(%1 %2)").arg(AppSetting.currency).arg(walletCurrency)
            color: "#FFFFFF"
        }
    }

    Item {
        width: 80
        height: childrenRect.height
        anchors {
            right: parent.right
            rightMargin: 24
            bottom: parent.bottom
            bottomMargin: 24
        }
        Column {
            width: parent.width
            spacing: 4
            Item {
                width: parent.width
                height: 16
                visible: !isReplaced
                Rectangle{
                    width: 70
                    height: parent.height
                    radius: 20
                    color: "#EAEAEA"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    Row {
                        anchors.centerIn: parent
                        spacing: 4
                        QImage {
                            width: 12
                            height: 12
                            source: "qrc:/Images/Images/OnlineMode/Joint wallet_031F2B.png"
                        }
                        QText{
                            font.family: "Lato"
                            font.pixelSize: 10
                            color: "#031F2B"
                            text: STR.STR_QML_679
                            font.weight: Font.Bold
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
            }
            Rectangle {
                width: parent.width
                height: 16
                visible: isReplaced
                radius: 20
                color: "#EAEAEA"
                anchors.right: parent.right
                Row {
                    anchors.centerIn: parent
                    spacing: 4
                    QText{
                        font.family: "Lato"
                        font.pixelSize: 10
                        color: "#031F2B"
                        text: STR.STR_QML_1345
                        font.weight: Font.Bold
                    }
                }
            }
            Rectangle {
                width: parent.width
                height: 16
                radius: 20
                color: "#EAEAEA"
                QText {
                    anchors.centerIn: parent
                    text: walletN === "1" ? STR.STR_QML_070 : qsTr("%1/%2 %3").arg(walletM).arg(walletN).arg(STR.STR_QML_069);
                    color: "#031F2B"
                    font.weight: Font.Bold
                    font.pixelSize: 10
                }
            }
        }
    }


    signal buttonClicked()
    MouseArea {
        id: walletmouse
        enabled: wldlg.iEnabled
        hoverEnabled: true
        anchors.fill: parent
        onClicked: wldlg.buttonClicked()
    }
}
