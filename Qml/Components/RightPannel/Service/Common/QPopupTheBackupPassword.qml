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
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Buttons"
import "./../../../customizes/Popups"
import "./../../../customizes/services"
import "./../../../customizes/Texts"
import "../../../../../localization/STR_QML.js" as STR

QPopupEmpty {
    id: _id
    content: QOnScreenContentTypeA {
        width: 800
        height: 489
        label.text: STR.STR_QML_1615
        onCloseClicked: _id.close()
        content: Item {
            Column {
                width: 539
                spacing: 24
                QLato {
                    width: 539
                    text: STR.STR_QML_1616
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Repeater {
                    id: _guide
                    width: 539
                    readonly property var content_map: [
                        {height: 84, headline:STR.STR_QML_1617, content: STR.STR_QML_1619 , icon: "qrc:/Images/Images/1.Active.svg" },
                        {height: 84, headline:STR.STR_QML_1618, content: STR.STR_QML_1620 , icon: "qrc:/Images/Images/2.Active.svg" },
                    ]
                    model: content_map.length
                    Rectangle {
                        property var _item: _guide.content_map[index]
                        width: 539
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
                                    width: 503
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
                                    width: 503
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

        bottomLeft: Item {}
        bottomRight: Row {
            spacing: 12
            QTextButton {
                width: 73
                height: 48
                label.text: STR.STR_QML_341
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: _id.close()
            }
        }
    }
}
