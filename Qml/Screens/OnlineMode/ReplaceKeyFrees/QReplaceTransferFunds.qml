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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
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

QOnScreenContentTypeB {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1381
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Item {
        Column {
            spacing: 24
            QLato {
                width: parent.width
                height: 28
                anchors.left: parent.left
                text: STR.STR_QML_1382.arg(walletInfo.walletName)
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle {
                width: 539
                height: 72
                radius: 12
                color: "#F5F5F5"
                QLato {
                    width: parent.width
                    anchors {
                        left: parent.left
                        leftMargin: 16
                        verticalCenter: parent.verticalCenter
                    }
                    text: STR.STR_QML_1385
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Column {
                    height: 40
                    width: 245
                    spacing: 4
                    anchors {
                        right: parent.right
                        rightMargin: 16
                        verticalCenter: parent.verticalCenter
                    }
                    QLato {
                        width: parent.width
                        height: paintedHeight
                        anchors.right: parent.right
                        text: AppModel.walletInfo.walletBalance + RoomWalletData.unitValue
                        font.weight: Font.Bold
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                    QLato {
                        width: parent.width
                        height: paintedHeight
                        anchors.right: parent.right
                        text: qsTr("%1%2 %3").arg(AppSetting.currencySymbol).arg(AppModel.walletInfo.walletBalanceCurrency).arg(AppSetting.currency)
                        font.pixelSize: 12
                        color: "#031F2B"
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    bottomRight: QTextButton {
        width: 220
        height: 48
        label.text: STR.STR_QML_1389
        label.font.pixelSize: 16
        type: eTypeE
        enabled: nextEnable
        onButtonClicked: {
            setReplaceFlow("replace-before-you-begin")
        }
    }
}
