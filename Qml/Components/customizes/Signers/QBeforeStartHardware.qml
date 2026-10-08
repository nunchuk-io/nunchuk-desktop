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

QOnScreenContentTypeA {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_091
    extraHeader: Item {}
    onCloseClicked: SignerManagement.backScreen()
    content: Item {
        Column {
            spacing: 24
            anchors{
                top: parent.top
                topMargin: 68
                horizontalCenter: parent.horizontalCenter
            }
            Rectangle {
                width: 96
                height: 96
                radius: width
                color: "#F5F5F5"
                anchors.horizontalCenter: parent.horizontalCenter
                QIcon {
                    iconSize: 60
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/info-60px.svg"
                }
            }
            QText {
                width: 500
                height: 28
                text: STR.STR_QML_092
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.family: "Lato"
                font.pixelSize: 16
                color: "#031F2B"
            }
            Column {
                id: guide
                spacing: 16
                anchors.horizontalCenter: parent.horizontalCenter
                Item {
                    width: 536
                    height: 84
                    Rectangle {
                        id:guidenum1
                        width: 24
                        height: 24
                        radius: width
                        color: "#031F2B"
                        border.width: 2
                        border.color: "#031F2B"
                        QText {
                            anchors.centerIn: parent
                            color: "#FFFFFF"
                            font.pixelSize: 12
                            font.weight: Font.DemiBold
                            text: "1"
                        }
                    }
                    Column {
                        width: parent.width - 28
                        anchors.left: guidenum1.right
                        anchors.leftMargin: 8
                        spacing: 8
                        QText {
                            width: parent.width
                            color: "#031F2B"
                            font.pixelSize: 16
                            font.weight: Font.DemiBold
                            font.family: "Montserrat"
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                            text: STR.STR_QML_093
                        }
                        QText {
                            width: parent.width
                            color: "#323E4A"
                            font.family: "Lato"
                            font.pixelSize: 16
                            wrapMode: Text.WordWrap
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                            text: STR.STR_QML_094
                        }
                    }
                }
                Item {
                    width: 536
                    height: 84
                    Rectangle {
                        id:guidenum2
                        width: 24
                        height: 24
                        radius: width
                        color: "#031F2B"
                        border.width: 2
                        border.color: "#031F2B"
                        QText {
                            anchors.centerIn: parent
                            color: "#FFFFFF"
                            font.pixelSize: 16
                            font.weight: Font.DemiBold
                            text: "2"
                        }
                    }
                    Column {
                        width: parent.width - 28
                        anchors.left: guidenum2.right
                        anchors.leftMargin: 8
                        spacing: 8
                        QText {
                            width: parent.width
                            color: "#031F2B"
                            font.pixelSize: 16
                            font.weight: Font.DemiBold
                            font.family: "Montserrat"
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                            text: STR.STR_QML_095
                        }
                        QText {
                            width: parent.width
                            color: "#323E4A"
                            font.family: "Lato"
                            font.pixelSize: 16
                            wrapMode: Text.WordWrap
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                            text: STR.STR_QML_096
                        }
                    }
                }
            }
        }
    }
    onPrevClicked: SignerManagement.backScreen()
    onNextClicked: QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_REQUEST)
}

