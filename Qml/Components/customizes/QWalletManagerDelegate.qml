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
import "../origins"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    id: wldlg
    width: 304
    height: rowcontent.height
    color: isCurrentIndex ? Qt.rgba(255, 255, 255, 0.3) : walletmouse.containsMouse ?  Qt.rgba(255, 255, 255, 0.1) : "transparent"
    property bool   isCurrentIndex: false
    property bool   isEscrow: false
    property bool   isShared: false
    property bool   isAssisted: false
    property string walletName      : "Name"
    property string walletBalance   : "0.0000000"
    property string walletCurrency  : "0.0000000"
    property string walletM: "0"
    property string walletN: "0"
    Row {
        id: rowcontent
        width: parent.width
        height: content.height + 20
        anchors.verticalCenter: parent.verticalCenter
        spacing: 8
        Rectangle {
            width: 8
            height: parent.height
            color: isCurrentIndex ? "#F6D65D" : "transparent"
            anchors.verticalCenter: parent.verticalCenter
        }
        Item {
            id: content
            width: 190
            height: childrenRect.height
            anchors.verticalCenter: parent.verticalCenter
            Column {
                width: parent.width
                spacing: 4
                Item {
                    width: parent.width
                    height: 20
                    QText {
                        anchors.fill: parent
                        text: wldlg.walletName
                        color: "#FFFFFF"
                        elide: Text.ElideRight
                        font.weight: Font.Bold
                        font.pixelSize: 16
                    }
                }
                Item {
                    width: parent.width
                    height: 16
                    QText {
                        anchors.fill: parent
                        text: walletBalance + RoomWalletData.unitValue
                        color: "#FFFFFF"
                        elide: Text.ElideRight
                        font.weight: Font.DemiBold
                        font.pixelSize: 12
                    }
                }
                Item {
                    width: parent.width
                    height: 16
                    QText {
                        anchors.fill: parent
                        text: qsTr("$%1 USD").arg(walletCurrency)
                        color: "#FFFFFF"
                        elide: Text.ElideRight
                        font.pixelSize: 12
                    }
                }
            }
        }
        Item {
            width: 80
            height: childrenRect.height
            anchors.verticalCenter: parent.verticalCenter
            Column {
                width: parent.width
                spacing: 4
                Item {
                    width: parent.width
                    height: 20
                    QImage {
                        width: height
                        height: parent.height
                        anchors.right: parent.right
                        visible: isEscrow
                        source: "qrc:/Images/Images/OnlineMode/Escrow Wallet.png"
                    }
                }
                Item {
                    width: parent.width
                    height: 16
                    visible: isAssisted
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
                Item {
                    width: parent.width
                    height: 16
                    visible: isShared
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
                                text: STR.STR_QML_438
                                font.weight: Font.Bold
                                anchors.verticalCenter: parent.verticalCenter
                            }
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
    }

    signal buttonClicked()
    MouseArea {
        id: walletmouse
        hoverEnabled: true
        anchors.fill: parent
        onClicked: wldlg.buttonClicked()
    }
}
