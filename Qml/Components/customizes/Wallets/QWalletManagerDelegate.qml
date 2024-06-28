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
import QtQuick 2.12
import DataPool 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Wallets"
import "../Texts"
import "../Buttons"
import "../../../../localization/STR_QML.js" as STR

QWalletDelegateBackground {
    id: wldlg
    width: 304
    height: hasOwner ? 110 : 78
    property bool   isCurrentIndex: false
    property bool   isEscrow: false
    property bool   isShared: false
    property string walletName      : "Name"
    property string walletBalance   : "0.0000000"
    property string walletCurrency  : "0.0000000"
    property string walletM: "0"
    property string walletN: "0"
    property var    primaryOwner
    MouseArea {
        id: walletmouse
        hoverEnabled: true
        propagateComposedEvents: true
        anchors.fill: parent
        onClicked: wldlg.buttonClicked()
    }
    Column {
        anchors.verticalCenter: parent.verticalCenter
        Row {
            height: 78
            spacing: 8
            Rectangle {
                width: 8
                height: wldlg.height
                color: isCurrentIndex ? "#F6D65D" : "transparent"
            }
            Column {
                width: wldlg.width - 112
                anchors.verticalCenter: parent.verticalCenter
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
                        text: (walletRole === "KEYHOLDER_LIMITED") ? "••••••" : (walletBalance + RoomWalletData.unitValue)
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
                        text: (walletRole === "KEYHOLDER_LIMITED") ? "••••••" : qsTr("%1%2 %3").arg(AppSetting.currencySymbol).arg(walletCurrency).arg(AppSetting.currency)
                        color: "#FFFFFF"
                        elide: Text.ElideRight
                        font.pixelSize: 12
                    }
                }
            }
            Column {
                width: 80
                anchors.verticalCenter: parent.verticalCenter
                spacing: 4
                Item {
                    width: parent.width
                    height: 20
                    visible: isEscrow
                    QImage {
                        width: height
                        height: parent.height
                        anchors.right: parent.right
                        source: "qrc:/Images/Images/OnlineMode/Escrow Wallet.png"
                    }
                }
                Item {
                    width: parent.width
                    height: 16
                    visible: (isDashboard && walletRole !== "OBSERVER") && !isReplaced
                    QTextLink {
                        width: 56
                        height: 16
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
                Rectangle {
                    width: parent.width
                    height: 16
                    visible: walletRole === "OBSERVER"
                    radius: 20
                    color: "#EAEAEA"
                    anchors.right: parent.right
                    Row {
                        anchors.centerIn: parent
                        spacing: 4
                        QImage {
                            width: 12
                            height: 12
                            source: "qrc:/Images/Images/OnlineMode/visibility-dark.svg"
                        }
                        QText{
                            font.family: "Lato"
                            font.pixelSize: 10
                            color: "#031F2B"
                            text: STR.STR_QML_1170
                            font.weight: Font.Bold
                        }
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 16
                    visible: isAssisted && !isReplaced
                    radius: 20
                    color: "#EAEAEA"
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
                    visible: isShared
                    radius: 20
                    color: "#EAEAEA"
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
        Item {
            height: 32
            width: wldlg.width
            visible: hasOwner
            Row {
                height: 32
                spacing: 12
                Item {
                    width: 8
                    height: 32
                }
                QAvatar {
                    width: 24
                    height: 24
                    maxChar: 2
                    username: primaryOwner.name
                    avatarUrl: primaryOwner.avatar
                    anchors.verticalCenter: parent.verticalCenter
                    displayStatus: false
                }
                QLato {
                    height: 16
                    font.pixelSize: 12
                    color: "#595959"
                    text: primaryOwner.name
                    font.weight: Font.Bold
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
    signal buttonClicked()
    signal dashboard()
}
