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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    id: coinItem
    height: visible ? body.height : 0
    property string amount: ""
    property string currency: "sat"
    property string amount_currency: ""
    property string blocktime: ""
    property var    coin_tags
    property var    coin_timelocklist
    property var    coin_timelockbase
    property int    coin_timelockCount: 0
    property bool   coin_timelocked: false
    property string coin_note: ""
    property bool   checked: false
    property bool   isLocked: false
    property bool   isScheduled: false
    property bool   isChange: false
    property string backgroundColor: checked ? "#F5F5F5" : "#FFFFFF"
    property bool   allowRightClick: true
    property string removeLabel: STR.STR_QML_1451
    property string outgoing_label: ""
    property string outgoing_color: ""
    property bool   isLarge: true
    property int    horizontalMargin: 24*2
    property int    verticalMargin: 12*2
    property bool   showCheckBox: true


    property var    walletInfo: AppModel.walletInfo
    property bool   showCoinAge: (walletInfo.walletType === NUNCHUCKTYPE.MINISCRIPT) && walletInfo.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_RELATIVE

    signal coinViewClicked()
    signal coinViewCheck()
    signal removeClicked()
    color: backgroundColor
    Item {
        id: body
        width: parent.width - horizontalMargin
        height: contents.height + verticalMargin
        anchors.centerIn: parent

        Row {
            id: contents
            width: parent.width
            spacing: 12
            anchors.verticalCenter: parent.verticalCenter
            QCheckBox {
                visible: showCheckBox
                checked: coinItem.checked
                onCheckboxClicked: {
                    coinViewCheck()
                }
            }
            Item {
                width: parent.width - (showCheckBox ? 36 : 0)
                height: tagContents.height
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: {
                        if (mouse.button === Qt.RightButton) {
                            if (allowRightClick) {
                                optionMenu.functions = [function(){
                                    removeClicked()
                                }]
                                optionMenu.popup()
                            }
                        }
                        else{
                            coinViewClicked()
                        }
                    }
                }
                Column {
                    id: tagContents
                    width: parent.width
                    spacing: 4
                    Item {
                        width: parent.width
                        height: 20
                        Row {
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 4
                            QLato {
                                text: qsTr("%1 %2").arg(amount).arg(currency)
                                font.pixelSize: 16
                                font.weight: Font.Bold
                                verticalAlignment: Text.AlignVCenter
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QBadge {
                                text: STR.STR_QML_1413
                                border.width: 1
                                border.color: "#EAEAEA"
                                color: "#FFFFFF"
                                font.weight: Font.Medium
                                fontSize: 10
                                anchors.verticalCenter: parent.verticalCenter
                                visible: isChange
                            }
                            QBadge {
                                width: 28
                                height: 16
                                color: "#EAEAEA"
                                icon: "qrc:/Images/Images/secured-dark.svg"
                                anchors.verticalCenter: parent.verticalCenter
                                visible: isLocked
                            }
                            QBadge {
                                width: 28
                                height: 16
                                color: "#EAEAEA"
                                icon: "qrc:/Images/Images/scheduling-dark.svg"
                                anchors.verticalCenter: parent.verticalCenter
                                visible: isScheduled
                            }
                        }
                    }

                    Column {
                        width: parent.width
                        Item {
                            visible: isLarge
                            width: parent.width
                            height: 12
                            Row {
                                spacing: 8
                                anchors.verticalCenter: parent.verticalCenter
                                QLato {
                                    text: qsTr("%1 %2 • %3").arg(AppSetting.currency).arg(amount_currency).arg(blocktime)
                                    color: "#595959"
                                    font.pixelSize: 12
                                    font.weight: Font.Normal
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                QBadge {
                                    topMargin: 0
                                    color: outgoing_color
                                    text: outgoing_label
                                    font.weight: Font.Bold
                                    font.pixelSize: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                        Column {
                            width: parent.width
                            visible: !isLarge
                            spacing: 4
                            Item {
                                width: parent.width
                                height: 12
                                Row {
                                    spacing: 8
                                    anchors.verticalCenter: parent.verticalCenter
                                    QLato {
                                        text: qsTr("%1 %2").arg(AppSetting.currency).arg(amount_currency)
                                        color: "#595959"
                                        font.pixelSize: 12
                                        font.weight: Font.Normal
                                        verticalAlignment: Text.AlignVCenter
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }
                            }
                            Item {
                                width: parent.width
                                height: 12
                                Row {
                                    spacing: 8
                                    anchors.verticalCenter: parent.verticalCenter
                                    QLato {
                                        text: qsTr("%1").arg(blocktime)
                                        color: "#595959"
                                        font.pixelSize: 12
                                        font.weight: Font.Normal
                                        verticalAlignment: Text.AlignVCenter
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    QBadge {
                                        topMargin: 0
                                        color: outgoing_color
                                        text: outgoing_label
                                        font.weight: Font.Bold
                                        font.pixelSize: 10
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }
                            }
                        }
                    }

                    QCoinAge {
                        width: parent.width
                        height: 70
                        coin_timelocklist: coinItem.coin_timelocklist
                        coin_timelockbase: coinItem.coin_timelockbase
                        coin_timelocked: coinItem.coin_timelocked
                        visible: showCoinAge && coin_timelockCount > 0
                    }

                    Rectangle {
                        radius: 12
                        width: parent.width
                        height: tagAndMemoContent.height + 8*2
                        border.color: "#DEDEDE"
                        border.width: 1
                        color: backgroundColor
                        visible: coin_tags.count > 0 || coin_note !== ""
                        Column {
                            id: tagAndMemoContent
                            width: parent.width - 8*2
                            spacing: 8
                            anchors.centerIn: parent
                            Flow {
                                id: tagsflow
                                spacing: 8
                                width: parent.width
                                Repeater {
                                    model: coin_tags
                                    QCoinType {
                                        coinColor: coin_tag_color
                                        coinText: coin_tag_name
                                    }
                                }
                            }
                            QCoinNote {
                                visible: memo !== ""
                                memo: coin_note
                                width: parent.width
                            }
                        }
                    }
                }
            }
        }
    }
    QContextMenu {
        id: optionMenu
        menuWidth: 300
        labels: [
            removeLabel
        ]
        colors: [
            "#031F2B"
        ]
        icons: [
            ""
        ]
        onItemClicked: {
            functions[index]()
        }
    }
}
