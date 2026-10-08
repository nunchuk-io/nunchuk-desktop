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
import NUNCHUCKTYPE 1.0
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Buttons"
import "./../../../customizes/Popups"
import "./../../../customizes/services"
import "./../../../customizes/Texts"
import "../../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2000
    readonly property var keys: [
        {id: "hardware-device",      textWidth: 271,    display_name: STR.STR_QML_2005, is_recommended: true},
        {id: "seed-phrase-backup",   textWidth: 271,    display_name: STR.STR_QML_2006, is_recommended: false},
    ]
    property string key_option: "hardware-device"
    content: Item {
        Row {
            spacing: 36
            QPicture {
                width: 346
                height: 512
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/Images/Images/prepare-inheritance-key.svg"
            }
            Item {
                width: 346
                height: 512
                Column {
                    width: parent.width
                    spacing: 24
                    Column {
                        width: parent.width
                        QLato {
                            width: parent.width
                            text: STR.STR_QML_2001
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
                                {height: 40, title:STR.STR_QML_2002 },
                                {height: 40, title:STR.STR_QML_2003 },
                            ]
                            model: content_map.length
                            Rectangle {
                                property var _item: _guide.content_map[index]
                                width: 346
                                height: _item.height
                                Row {
                                    spacing: 0
                                    QLato {
                                        width: 20
                                        height: 20
                                        text: "●"
                                        font.weight: Font.Normal
                                        font.pixelSize: 8
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                    QLato {
                                        id: _term
                                        width: 310
                                        height: _item.height
                                        text: _item.title
                                        textFormat: Text.RichText
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
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_2004
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 20
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QListView {
                        id: questions
                        width: 343
                        height: childrenRect.height
                        spacing: 8
                        clip: true
                        model: keys
                        interactive: false
                        delegate: QRadioButtonTypeE {
                            id: btn
                            width: 343
                            height: 76
                            label: modelData.display_name
                            textWidth: modelData.textWidth
                            fontPixelSize: 16                            
                            fontWeight: btn.selected ? Font.ExtraBold : Font.DemiBold
                            selected: key_option === modelData.id
                            textBadge: ""
                            onButtonClicked: {
                                key_option = modelData.id
                            }
                        }
                    }
                }
            }
        }
    }
}