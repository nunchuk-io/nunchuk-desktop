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
import "./../../origins"
import "./../../customizes"
import "./../../customizes/Chats"
import "./../../customizes/Texts"
import "./../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: feeSettingItem
    anchors.fill: parent
    property int    feeValue: AppSetting.feeSetting
    property bool   anyChanged: AppSetting.feeSetting !== feeValue

    Flickable {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: _feesetting.childrenRect.height
        interactive: contentHeight > height
        clip: true
        flickableDirection: Flickable.VerticalFlick
        ScrollBar.vertical: ScrollBar { active: true }

        Column{
            id: _feesetting
            anchors{
                top:parent.top
                topMargin: 24
                left: parent.left
                leftMargin: 24
            }
            spacing: 24
            QText {
                font.family: "Lato"
                font.pixelSize: 28
                font.weight: Font.Bold
                text: STR.STR_QML_1715
            }
            Column {
                spacing: 0
                QLato {
                    font.pixelSize: 12
                    font.weight: Font.Bold
                    text: STR.STR_QML_1716
                }
                Item {
                    width: 2
                    height: 8
                }
                Item {
                    id: economyitem
                    property bool selected: feeSettingItem.feeValue === NUNCHUCKTYPE.ECONOMY
                    width: 375
                    height: 62
                    Column {
                        width: parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        Row {
                            spacing: 8
                            height: 24
                            QLato {
                                text: "Economy"
                                font.pixelSize: 16
                                font.weight: Font.Normal
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            Rectangle {
                                width: 91
                                height: 16
                                radius: 20
                                color: "#EAEAEA"
                                anchors.verticalCenter: parent.verticalCenter
                                QText {
                                    text: "Recommended"
                                    font.pixelSize: 10
                                    font.weight: Font.Bold
                                    anchors.centerIn: parent
                                }
                            }
                        }
                        QLato {
                            text: STR.STR_QML_1717
                            font.pixelSize: 12
                            font.weight: Font.Normal
                            color: "#757575"
                            height: 16
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    ColorOverlay {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        width: 24
                        height: 24
                        source: QIcon {
                            iconSize: 24
                            source: economyitem.selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                        }
                        color: economyitem.enabled ? "#031F2B" : "#666666"
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            feeSettingItem.feeValue = NUNCHUCKTYPE.ECONOMY
                        }
                    }
                }
                Item {
                    id: standarditem
                    property bool selected: feeSettingItem.feeValue === NUNCHUCKTYPE.STANDARD
                    width: 375
                    height: 62
                    Column {
                        width: parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        QLato {
                            height: 24
                            verticalAlignment: Text.AlignVCenter
                            text: "Standard"
                            font.pixelSize: 16
                            font.weight: Font.Normal
                        }
                        QLato {
                            text: STR.STR_QML_1718
                            font.pixelSize: 12
                            font.weight: Font.Normal
                            color: "#757575"
                            height: 16
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    ColorOverlay {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        width: 24
                        height: 24
                        source: QIcon {
                            iconSize: 24
                            source: standarditem.selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                        }
                        color: standarditem.enabled ? "#031F2B" : "#666666"
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            feeSettingItem.feeValue = NUNCHUCKTYPE.STANDARD
                        }
                    }
                }
                Item {
                    id: priorityitem
                    property bool selected: feeSettingItem.feeValue === NUNCHUCKTYPE.PRIORITY
                    width: 375
                    height: 62
                    Column {
                        width: parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        QLato {
                            height: 24
                            verticalAlignment: Text.AlignVCenter
                            text: "Priority"
                            font.pixelSize: 16
                            font.weight: Font.Normal
                        }
                        QLato {
                            text: STR.STR_QML_1719
                            font.pixelSize: 12
                            font.weight: Font.Normal
                            color: "#757575"
                            height: 16
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    ColorOverlay {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        width: 24
                        height: 24
                        source: QIcon {
                            iconSize: 24
                            source: priorityitem.selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                        }
                        color: priorityitem.enabled ? "#031F2B" : "#666666"
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            feeSettingItem.feeValue = NUNCHUCKTYPE.PRIORITY
                        }
                    }
                }
            }
        }
    }

    QTextButton {
        width: 189
        height: 48
        label.text: STR.STR_QML_1726
        label.font.pixelSize: 16
        label.font.family: "Lato"
        type: eTypeE
        anchors{
            right: parent.right
            rightMargin: 24
            bottom: parent.bottom
            bottomMargin: 16
        }
        enabled: feeSettingItem.anyChanged
        onButtonClicked: {
            feeSettingItem.saveFeeSettings()
        }
    }
    function saveFeeSettings() {
        if(feeSettingItem.anyChanged) {
            AppSetting.feeSetting = feeSettingItem.feeValue
            AppModel.showToast(0, "Fee settings updated", EWARNING.SUCCESS_MSG);
        }
    }
}
