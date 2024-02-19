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
    width: 350
    height: 480
    radius: 12
    border.color: "#EAEAEA"
    color: "#FFFFFF"
    layer.enabled: true
    layer.effect: OpacityMask {
        maskSource: Rectangle {
            width: 350
            height: 480
            radius: 12
        }
    }
    property string myRole: ""
    property var transactionInfo
    property bool isDummy: false
    signal addrToVerify(var addr)
    signal newMemoNotify(var newMemo)
    Flickable {
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        clip: true
        interactive: contentHeight > height
        contentHeight: contentDisp.height
        ScrollBar.vertical: ScrollBar { active: true }
        Column {
            id: contentDisp
            width: parent.width
            spacing: 12
            anchors.horizontalCenter: parent.horizontalCenter
            Rectangle {
                width: parent.width
                height: 32
                color: "#F5F5F5"
                QText {
                    text: qsTr("%1 (%2)").arg(STR.STR_QML_213).arg(lstDestination.count)
                    font.family: "Lato"
                    font.weight: Font.Bold
                    font.pixelSize: 12
                    color: "#323E4A"
                    anchors {
                        left: parent.left
                        leftMargin: 16
                        verticalCenter: parent.verticalCenter
                    }
                }
                QText {
                    text: STR.STR_QML_214
                    font.family: "Lato"
                    font.weight: Font.Bold
                    font.pixelSize: 12
                    color: "#323E4A"
                    anchors {
                        right: parent.right
                        rightMargin: 16
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
            // Destination infomation
            QListView {
                id: lstDestination
                model: transactionInfo.destinationList
                width: parent.width
                height: count > 10 ? 400 : 60*count
                delegate: destDelegate
                clip: true
                cacheBuffer: 600
                interactive: (count > 10)
                spacing: 4
                ScrollBar.vertical: ScrollBar { active: true }
                Component {
                    id: destDelegate
                    Row {
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
                            text: destination_address
                            MouseArea {
                                id: btnMouseVerify
                                enabled: myRole !== "KEYHOLDER_LIMITED"
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                anchors.fill: parent
                                onClicked: {
                                    addrToVerify(destination_address)
                                }
                            }
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
                                text: destination_amount + RoomWalletData.unitValue
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
                                .arg(model.destination_amount_currency)
                                .arg(AppSetting.currency)
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }
                }
            }
            Rectangle { width: parent.width; height: 1; color: "#F5F5F5"}
            Row {
                width: parent.width - 24
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 12
                z: 1
                Item {
                    width: 122
                    height: 24
                    z: 1
                    Row {
                        spacing: 6
                        QLato {
                            width: 97
                            font.weight: Font.Bold
                            text: STR.STR_QML_215
                        }
                        QTooltip {
                            tipWidth: 200
                            toolTip: STR.STR_QML_807
                            source: "qrc:/Images/Images/OnlineMode/help_outline_24px.svg"
                        }
                    }
                }
                Column {
                    width: 192
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
                        text: transactionInfo.fee + RoomWalletData.unitValue
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
                        .arg(transactionInfo.feeCurrency)
                        .arg(AppSetting.currency)
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
            Row {
                width: parent.width - 24
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 12
                QText {
                    width: 122
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                    wrapMode: Text.WrapAnywhere
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    color: "#031F2B"
                    font.family: "Lato"
                    text: STR.STR_QML_216
                }
                Column {
                    width: 192
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
                        text: transactionInfo.total + RoomWalletData.unitValue
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
                        .arg(transactionInfo.totalCurrency)
                        .arg(AppSetting.currency)
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
            Rectangle {
                id: changeinfo
                width: parent.width
                visible: transactionInfo.hasChange
                height: changeinfo.visible ? 32 : 0
                color: "#F5F5F5"
                QText {
                    text: STR.STR_QML_217
                    font.family: "Lato"
                    font.weight: Font.Bold
                    font.pixelSize: 12
                    color: "#323E4A"
                    anchors {
                        left: parent.left
                        leftMargin: 16
                        verticalCenter: parent.verticalCenter
                    }
                }
                QText {
                    text: STR.STR_QML_214
                    font.family: "Lato"
                    font.weight: Font.Bold
                    font.pixelSize: 12
                    color: "#323E4A"
                    anchors {
                        right: parent.right
                        rightMargin: 16
                        verticalCenter: parent.verticalCenter
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
            Rectangle {
                width: parent.width
                visible: !isDummy
                height: 32
                color: "#F5F5F5"
                QText {
                    text: STR.STR_QML_218
                    font.family: "Lato"
                    font.weight: Font.Bold
                    font.pixelSize: 12
                    color: "#323E4A"
                    anchors {
                        left: parent.left
                        leftMargin: 16
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
            QTextInputBoxTypeA {
                width: parent.width - 24
                visible: !isDummy
                placeholderText: ""
                text: transactionInfo.memo
                backgroundColor: "Transparent"
                borderColor: "Transparent"
                color: "#000000"
                font.family: "Montserrat"
                font.pixelSize: 16
                anchors.horizontalCenter: parent.horizontalCenter
                onTypingFinished: {
                    var newMemo = currentText
                    if(newMemo !== transactionInfo.memo){ newMemoNotify(newMemo) }
                }
            }
            Rectangle {
                width: parent.width
                visible: !isDummy
                height: 32
                color: "#F5F5F5"
                QText {
                    text: STR.STR_QML_219
                    font.family: "Lato"
                    font.weight: Font.Bold
                    font.pixelSize: 12
                    color: "#323E4A"
                    anchors {
                        left: parent.left
                        leftMargin: 16
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
            Item {
                width: parent.width
                height: 32
                visible: !isDummy
                QText {
                    id: numbersignaturepending
                    text: transactionInfo.numberSigned + "/" + transactionInfo.m
                    font.pixelSize: 16
                    color: "#031F2B"
                    font.family: "Lato"
                    font.weight: Font.Bold
                    anchors {
                        left: parent.left
                        leftMargin: 16
                        verticalCenter: parent.verticalCenter
                    }
                    width: 346
                    height: 32
                    wrapMode: Text.WordWrap
                }
                Row {
                    spacing: 8
                    height: 32
                    visible: (transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                    anchors {
                        left: parent.left
                        leftMargin: 70
                        verticalCenter: numbersignaturepending.verticalCenter
                    }
                    Rectangle {
                        id: indicatorStatus
                        width: 4
                        height: 24
                        radius: 1
                        color: "#E02247"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QText {
                        text: STR.str_QML_220(Math.max(0, (transactionInfo.m - transactionInfo.numberSigned)))
                        font.pixelSize: 12
                        font.family: "Lato"
                        color: indicatorStatus.color
                        width: 346
                        height: 32
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }
}
