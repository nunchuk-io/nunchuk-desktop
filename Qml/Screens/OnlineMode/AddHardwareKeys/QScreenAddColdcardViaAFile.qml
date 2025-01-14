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

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1050
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    content: Item {
        Row {
            spacing: 36
            Rectangle {
                width: 346
                height: 512
                radius: 24
                color: "#D0E2FF"
                QPicture {
                    width: 346
                    height: 300
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/coldcard-illustration.svg"
                }
            }
            Item {
                width: 346
                height: 512
                Column {
                    width: parent.width
                    spacing: 24
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_817
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Repeater {
                        id: _guide
                        width: parent.width
                        readonly property var content_map: [
                            {height: 48,  headline:STR.STR_QML_907, content: STR.STR_QML_908 , icon: "qrc:/Images/Images/1.Active.svg" },
                            {height: 84,  headline:STR.STR_QML_909, content: STR.STR_QML_910 , icon: "qrc:/Images/Images/2.Active.svg" },
                            {height: 112, headline:STR.STR_QML_1161,content: STR.STR_QML_1162 , icon: "qrc:/Images/Images/3.Active.svg" },
                            {height: 56,  headline:STR.STR_QML_1163,content: STR.STR_QML_1164 , icon: "qrc:/Images/Images/4.Active.svg" },
                        ]
                        model: content_map.length
                        Rectangle {
                            property var _item: _guide.content_map[index]
                            width: 346
                            height: _item.height
                            Row {
                                spacing: 12
                                QIcon {
                                    iconSize: 24
                                    id: _ico
                                    source: _item.icon
                                }
                                Column {
                                    width: 310
                                    height: _item.height
                                    spacing: 4
                                    QText {
                                        width: 310
                                        text: _item.headline
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        font.pixelSize: 16
                                        font.weight: Font.DemiBold
                                        horizontalAlignment: Text.AlignLeft
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                    QText {
                                        id: _term
                                        width: 310
                                        text: _item.content
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        font.pixelSize: 16
                                        lineHeightMode: Text.FixedHeight
                                        lineHeight: 28
                                        wrapMode: Text.WordWrap
                                        horizontalAlignment: Text.AlignLeft
                                        verticalAlignment: Text.AlignVCenter
                                        onLinkActivated: Qt.openUrlExternally("https://coldcard.com/docs/quick")
                                        MouseArea {
                                            anchors.fill: parent
                                            cursorShape: _term.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                                            acceptedButtons: Qt.NoButton
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    }

    bottomRight: QTextButton {
        width: 120
        height: 48
        label.text: STR.STR_QML_1165
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            fileDialog.open()
        }
    }
    property alias file: fileDialog.file
    FileDialog {
        id: fileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            addDeviceViaImportFile()
        }
    }
}
