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
import DataPool 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Screens/OnlineMode/Coins"
import "../../../../localization/STR_QML.js" as STR

Column {
    width: parent.width
    spacing: 12
    property bool editShow: false
    signal editClicked
    Rectangle {
        id: changeinfo
        width: parent.width
        height: 48
        color: "#F5F5F5"
        QText {
            text: STR.STR_QML_217
            font.family: "Lato"
            font.weight: Font.Bold
            font.pixelSize: 12
            color: "#323E4A"
            anchors {
                left: parent.left
                leftMargin: 12
                verticalCenter: parent.verticalCenter
            }
        }
        QTextLink {
            id: _edit
            width: _edit.paintedWidth
            height: 20
            font.pixelSize: 12
            text: STR.STR_QML_849
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 12
            }
            visible: editShow
            onTextClicked: {
                editClicked()
            }
        }
    }

    Row {
        visible: changeinfo.visible
        width: parent.width - 24
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 12
        QText {
            width: 192
            lineHeightMode: Text.FixedHeight
            lineHeight: 20
            wrapMode: Text.WrapAnywhere
            font.pixelSize: 16
            font.weight: Font.Bold
            color: "#031F2B"
            font.family: "Lato"
            text: transactionInfo.change.address
        }
        Column {
            width: 122
            spacing: 4
            QText {
                width: parent.width
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
                wrapMode: Text.WrapAnywhere
                font.pixelSize: 16
                font.weight: Font.Bold
                color: "#031F2B"
                font.family: "Lato"
                text: transactionInfo.change.amount + RoomWalletData.unitValue
                horizontalAlignment: Text.AlignRight
            }
            QText {
                width: parent.width
                lineHeightMode: Text.FixedHeight
                lineHeight: 16
                wrapMode: Text.WrapAnywhere
                font.pixelSize: 12
                color: "#031F2B"
                font.family: "Lato"
                text: qsTr("%1%2 %3")
                .arg(AppSetting.currencySymbol)
                .arg(transactionInfo.change.amountCurrency)
                .arg(AppSetting.currency)
                horizontalAlignment: Text.AlignRight
            }
        }
    }
    property var listTag: transactionInfo.changeCoinsTag
    Rectangle {
        radius: 12
        width: parent.width - 24
        height: tagAndMemoContent.height + 8*2
        border.color: "#DEDEDE"
        border.width: 1
        color: "#FFFFFF"
        anchors.horizontalCenter: parent.horizontalCenter
        visible: tagsflow.count > 0
        Column {
            id: tagAndMemoContent
            width: parent.width - 8*2
            spacing: 8
            anchors.centerIn: parent
            Flow {
                spacing: 8
                width: parent.width
                Repeater {
                    id: tagsflow
                    model: listTag.count <= 5 ? listTag.count : 5
                    QCoinType {
                        property var data: listTag.get(index)
                        coinColor: data.coin_tag_color
                        coinText: data.coin_tag_name
                    }
                }
                QLato {
                    width: 20
                    height: 20
                    text: qsTr("+%1").arg(listTag.count - 5)
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}

