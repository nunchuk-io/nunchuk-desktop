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
import "../../../Components/customizes/Popups"
import "../../../Components/customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1725
    extraHeader: Item {}
    property var newWalletInfo: AppModel.newWalletInfo
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    content: Item {
        Row {
            spacing: 48
            anchors {
                top: parent.top
                topMargin: 108
                horizontalCenter: parent.horizontalCenter
            }
            Item {
                width: 246
                height: 268
                Column {
                    spacing: 24
                    Rectangle {
                        width: 96
                        height: 96
                        radius: width
                        color: "#F5F5F5"
                        anchors.horizontalCenter: parent.horizontalCenter
                        QIcon {
                            iconSize: 60
                            anchors.centerIn: parent
                            source: "qrc:/Images/Images/hardware_add-60px.png"
                        }
                        opacity: isPrimaryKeyFlow ? 0.4 : 1.0
                    }
                    Column {
                        spacing: 12
                        Column {
                            spacing: 4
                            QLato {
                                width: 210
                                height: 28
                                text: STR.STR_QML_084
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: 20
                                font.weight: Font.Bold
                                wrapMode: Text.WordWrap
                                anchors.horizontalCenter: parent.horizontalCenter
                                opacity: isPrimaryKeyFlow ? 0.4 : 1.0
                            }
                            QLato {
                                width: 210
                                height: 56
                                text: STR.STR_QML_085
                                font.pixelSize: 16
                                font.weight: Font.Normal
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignHCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                opacity: isPrimaryKeyFlow ? 0.4 : 1.0
                            }
                        }
                        QTextButton {
                            width: 180
                            height: 48
                            label.text: STR.STR_QML_086
                            label.font.pixelSize: 16
                            enabled: {
                                if (isPrimaryKeyFlow) {
                                    return false
                                } else {
                                    var isTaproot = newWalletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT
                                    return isTaproot ? false : true
                                }
                            }
                            type: eTypeB
                            anchors.horizontalCenter: parent.horizontalCenter
                            onButtonClicked: {
                                SignerManagement.screenFlow = "before-start-hardware"
                            }
                        }
                    }
                }
            }
            Item {
                width: 246
                height: 268
                Column {
                    spacing: 24
                    Rectangle {
                        width: 96
                        height: 96
                        radius: width
                        color: "#F5F5F5"
                        anchors.horizontalCenter: parent.horizontalCenter
                        QIcon {
                            iconSize: 60
                            anchors.centerIn: parent
                            source: "qrc:/Images/Images/logo-dark.svg"
                        }
                    }
                    Column {
                        spacing: 12
                        Column {
                            spacing: 4
                            QLato {
                                width: 210
                                height: 28
                                text: STR.STR_QML_087
                                horizontalAlignment: Text.AlignHCenter
                                font.pixelSize: 20
                                font.weight: Font.Bold
                                wrapMode: Text.WordWrap
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            QLato {
                                width: 210
                                height: 56
                                text: STR.STR_QML_088
                                font.pixelSize: 16
                                font.weight: Font.Normal
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignHCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                        QTextButton {
                            width: 180
                            height: 48
                            label.text: enabled ? STR.STR_QML_089 : STR.STR_QML_090
                            label.font.pixelSize: 16
                            type: eTypeB
                            anchors.horizontalCenter: parent.horizontalCenter
                            enabled: AppModel.limitSoftwareSigner ? (AppModel.softwareSignerDeviceList.count === 0) : true
                            onButtonClicked: {
                                SignerManagement.screenFlow = "before-start-software"
                            }
                        }
                    }
                }
            }
        }
    }
    bottomRight: Item { }
    onPrevClicked: {
        closeTo(NUNCHUCKTYPE.WALLET_TAB)
    }
}

