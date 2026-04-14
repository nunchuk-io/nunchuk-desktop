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
import QtGraphicalEffects 1.0
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Wallets"
import "../Texts"
import "../Buttons"

QWalletDelegateBackground {
    id: wldlg
    width: 304
    height: gradientTopHeight + gradientBottomHeight
    property bool   isCurrentIndex: false
    property bool   isEscrow: false
    property bool   isShared: false
    property string walletName      : "Name"
    property string walletBalance   : "0.0000000"
    property string walletCurrency  : "0.0000000"
    property int    walletM: 0
    property int    walletN: 0
    property var    primaryOwner
    property bool   mouseActive: true
    property int    walletType: 0


    signal buttonClicked()
    signal dashboard()
    signal alertCountClicked()

    function getWalletTypeDes() {
        if (walletType === NUNCHUCKTYPE.MINISCRIPT) {
            return STR.STR_QML_1801
        } else {
            return (walletN === 1) ? STR.STR_QML_070 : 
                            qsTr("%1/%2 %3").arg(walletM).arg(walletN).arg(STR.STR_QML_069)
        }
    }
    MouseArea {
        id: walletmouse
        enabled: mouseActive
        hoverEnabled: true
        propagateComposedEvents: true
        anchors.fill: parent
        onClicked: wldlg.buttonClicked()
    }
    Column {
        anchors.verticalCenter: parent.verticalCenter
        Row {
            height: gradientTopHeight
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
                        text: (walletRole === "KEYHOLDER_LIMITED" || walletRole === "FACILITATOR_ADMIN") ? "••••••" : (walletBalance + RoomWalletData.unitValue)
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
                        text: (walletRole === "KEYHOLDER_LIMITED" || walletRole === "FACILITATOR_ADMIN") ? "••••••" : qsTr("%1%2 %3").arg(AppSetting.currencySymbol).arg(walletCurrency).arg(AppSetting.currency)
                        color: "#FFFFFF"
                        elide: Text.ElideRight
                        font.pixelSize: 12
                    }
                }
            }
            Column {
                width: 80
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                spacing: 4
                Item {
                    width: parent.width
                    height: 20
                    visible: isEscrow
                    QImage {
                        width: height
                        height: parent.height
                        anchors.right: parent.right
                        source: "qrc:/Images/Images/Escrow Wallet.png"
                    }
                }
                Item {
                    width: parent.width
                    height: 16
                    visible: (isDashboard && walletRole !== "OBSERVER") && !isReplaced && !isSandboxWallet
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
                        QIcon {
                            iconSize: 12
                            source: "qrc:/Images/Images/visibility-dark.svg"
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
                QTypeWallet {
                    anchors.right: parent.right
                    icon.iconSize: 12
                    icon.source: if (isShared) return "qrc:/Images/Images/collab-wallet-dark.svg"
                                 else if (isSandboxWallet && !isReplaced) return "qrc:/Images/Images/sandboxGroup.svg"
                                 else if (isAssisted && !isReplaced) return "qrc:/Images/Images/collab-wallet-dark.svg"
                                 else return ""
                                 
                    label.text: if (isShared) return STR.STR_QML_438
                                else if (isSandboxWallet && !isReplaced) return STR.STR_QML_1675
                                else if (isAssisted && !isReplaced) return STR.STR_QML_679
                                else if (isReplaced) return STR.STR_QML_1345
                                else return ""
                    label.font.weight: Font.Bold
                    label.font.pixelSize: 10
                    visible: label.text !== ""
                }
                Rectangle {
                    width: parent.width
                    height: 16
                    radius: 20
                    color: "#EAEAEA"
                    QText {
                        anchors.centerIn: parent
                        text: getWalletTypeDes()
                        color: "#031F2B"
                        font.weight: Font.Bold
                        font.pixelSize: 10
                    }
                }
            }
        }
        Item {
            height: gradientBottomHeight
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
                    username: primaryOwner === undefined ? "" : primaryOwner.name
                    avatarUrl: primaryOwner === undefined ? "" : primaryOwner.avatar
                    anchors.verticalCenter: parent.verticalCenter
                    displayStatus: false
                }
                QLato {
                    height: 16
                    font.pixelSize: 12
                    color: "#595959"
                    text: primaryOwner === undefined ? "" : primaryOwner.name
                    font.weight: Font.Bold
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
        Item {
            height: gradientBottomHeight
            width: wldlg.width
            visible: alertCount > 0 && isSandboxWallet
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    alertCountClicked()
                }
            }
            QLato {
                width: parent.width
                anchors{
                    left: parent.left
                    leftMargin: 16
                    verticalCenter: parent.verticalCenter
                }
                text: STR.STR_QML_934
                font.pixelSize: 12
                color: "#FFFFFF"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle {
                anchors{
                    right: parent.right
                    rightMargin: 12 + 24
                    verticalCenter: parent.verticalCenter
                }
                width: 24
                height: 24
                radius: 24
                color: "#CF4018"
                QLato {
                    width: parent.width
                    height: parent.height
                    color: "#FFFFFF"
                    anchors.centerIn: parent
                    text: alertCount
                    font.pixelSize: 12
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/chevron-right-light.svg"
                anchors{
                    right: parent.right
                    rightMargin: 0
                    verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
