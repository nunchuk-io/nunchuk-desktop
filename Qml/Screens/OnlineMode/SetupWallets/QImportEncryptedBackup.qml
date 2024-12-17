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
    label.text: STR.STR_QML_1621
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property string backup_file: "backup.7z"
    content: Item {
        Row {
            spacing: 36
            Rectangle {
                width: 346
                height: 512
                radius: 24
                color: "#D0E2FF"
                QImage {
                    width: 346
                    height: 300
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/COLDCARD-desktop-illustration.svg"
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
                        text: STR.STR_QML_1622
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
                            {height: 208, headline:STR.STR_QML_1623, content: STR.STR_QML_1624, content1: STR.STR_QML_1625 , content2: STR.STR_QML_1626.arg(backup_file) , icon: "qrc:/Images/Images/1.Active.svg" },
                            {height: 88, headline:STR.STR_QML_1627, content: STR.STR_QML_1628.arg(backup_file), icon: "qrc:/Images/Images/2.Active.svg" },
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
                                    spacing: 8
                                    QLato {
                                        width: 310
                                        text: _item.headline
                                        font.weight: Font.DemiBold
                                        horizontalAlignment: Text.AlignLeft
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                    QLato {
                                        id: _term
                                        width: 310
                                        text: _item.content
                                        lineHeightMode: Text.FixedHeight
                                        lineHeight: 20
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
                                    Rectangle {
                                        width: 310
                                        height: 64
                                        radius: 8
                                        color: "#EAEAEA"
                                        visible: _des1.text !== ""
                                        QLato {
                                            id: _des1
                                            width: 310 - 12*2
                                            text: _item.content1 ? _item.content1 : ""
                                            lineHeightMode: Text.FixedHeight
                                            lineHeight: 20
                                            wrapMode: Text.WordWrap
                                            horizontalAlignment: Text.AlignLeft
                                            verticalAlignment: Text.AlignVCenter
                                            anchors.centerIn: parent
                                        }
                                    }
                                    QLato {
                                        width: 310
                                        height: 60
                                        text: _item.content2 ? _item.content2 : ""
                                        visible: _item.content2 !== ""
                                        lineHeightMode: Text.FixedHeight
                                        lineHeight: 20
                                        wrapMode: Text.WordWrap
                                        horizontalAlignment: Text.AlignLeft
                                        verticalAlignment: Text.AlignVCenter
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
        width: 107
        height: 48
        label.text: STR.STR_QML_1165
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            fileDialog.open()
        }
    }
    FileDialog {
        id: fileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            var _input = {
                type: "import-encrypted-backup",
                fingerPrint: xfp,
                currentFile: fileDialog.currentFile
            }
            QMLHandle.sendEvent(EVT.EVT_DASHBOARD_ALERT_INFO_ENTER, _input)
        }
    }
}
