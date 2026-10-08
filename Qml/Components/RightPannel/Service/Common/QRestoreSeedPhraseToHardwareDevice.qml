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
    label.text: STR.STR_QML_2012
    content: Item {
        Row {
            spacing: 36
            QImage {
                width: 346
                height: 432
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/Images/Images/restore-seed-phrase-to-hardware-device.svg"
            }
            Item {
                width: 346
                height: 432
                Column {
                    width: parent.width
                    spacing: 24
                    Repeater {
                        id: _guide
                        width: parent.width
                        readonly property var content_map: [
                            {height: 40, content: STR.STR_QML_2013, icon: "qrc:/Images/Images/1.Active.svg" },
                            {height: 24, content: STR.STR_QML_2014, icon: "qrc:/Images/Images/2.Active.svg" },
                            {height: 60, content: STR.STR_QML_2015, icon: "qrc:/Images/Images/3.Active.svg" },
                            {height: 40, content: STR.STR_QML_2016, icon: "qrc:/Images/Images/4.Active.svg" },
                            {height: 40, content: STR.STR_QML_2017, icon: "qrc:/Images/Images/5.Active.svg" },
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
                                    QLato {
                                        width: 310
                                        text: _item.content
                                        lineHeightMode: Text.FixedHeight
                                        lineHeight: 28
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
        QWarningBg {
            width: 728
            iSize: 36
            icon: "qrc:/Images/Images/info-60px.svg"
            txt.text: STR.STR_QML_2018
            anchors.bottom: parent.bottom
        }
    }
}
