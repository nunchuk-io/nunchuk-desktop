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
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import Qt.labs.platform 1.1
import DataPool 1.0
import "../../Components/customizes"
import "../../Components/origins"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR

Item {
    Column {
        anchors.fill: parent
        spacing: 12
        Item {
            width: parent.width
            height: 36
            QText {
                id: txFeeTitle
                anchors {
                    left: parent.left
                    leftMargin: 16
                    verticalCenter: parent.verticalCenter
                }
                text: STR.STR_QML_017
                color: "#FFFFFF"
                font.weight: Font.Bold
                font.pixelSize: 16
            }
            Rectangle {
                width: 140
                height: 16
                radius: 20
                anchors {
                    left: txFeeTitle.right
                    leftMargin: 8
                    verticalCenter: parent.verticalCenter
                }
                Row {
                    spacing: 4
                    anchors.centerIn: parent
                    QIcon {
                        iconSize: 12
                        source: "qrc:/Images/Images/history-24px.png"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QText{
                        font.family: "Lato"
                        font.weight: Font.Bold
                        font.pixelSize: 10
                        color: "#031F2B"
                        text: "Updated " + AppModel.lasttimeCheckEstimatedFee
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
        Row {
            id: rowFeeInfo
            readonly property var prioritiesLabel: [
                STR.STR_QML_020,
                STR.STR_QML_019,
                STR.STR_QML_018
            ]
            readonly property var prioritiesFee: [
                AppModel.hourFee,
                AppModel.halfHourFee,
                AppModel.fastestFee
            ]
            readonly property var prioritiesFeeCurrency: [
                AppModel.hourFeeCurrency,
                AppModel.halfHourFeeCurrency,
                AppModel.fastestFeeCurrency
            ]
            width: parent.width - 24
            height: 83
            spacing: 8
            anchors.horizontalCenter: parent.horizontalCenter
            Repeater {
                model: 3
                Rectangle {
                    width: 89
                    height: 83
                    radius: 8
                    color: "transparent"
                    border.color: "#EAEAEA"
                    Column {
                        anchors.fill: parent
                        anchors.topMargin: 9
                        anchors.bottomMargin: 9
                        spacing: 0
                        QText {
                            width: 50
                            font.family: "Lato"
                            font.pixelSize: 12
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 16
                            color: "#FFFFFF"
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: rowFeeInfo.prioritiesLabel[index]
                        }
                        QText {
                            width: 76
                            font.family: "Lato"
                            font.pixelSize: 12
                            font.weight: Font.Bold
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 16
                            color: "#FFFFFF"
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("%1 sat/vB").arg(rowFeeInfo.prioritiesFee[index])
                        }
                        QText {
                            width: 76
                            font.family: "Lato"
                            font.pixelSize: 12
                            font.weight: Font.Bold
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 16
                            color: "#A7F0BA"
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("%1%2")
                            .arg(AppSetting.currencySymbol)
                            .arg(rowFeeInfo.prioritiesFeeCurrency[index])
                        }
                    }
                }
            }
        }
    }
}
