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
import QtQuick
import NUNCHUCKTYPE 1.0
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Buttons"
import "./../../../customizes/Popups"
import "./../../../customizes/services"
import "./../../../customizes/Texts"
import "../../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id: _notice
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    property string noticeType: "use-device"
    readonly property var noticeMap: ({
        "recover-key": { title: STR.STR_QML_2542, headline: STR.STR_QML_2543, body: STR.STR_QML_2544, bullets: [], warning: STR.STR_QML_2545 },
        "use-device":  { title: STR.STR_QML_2366, headline: STR.STR_QML_2546, body: "", bullets: [STR.STR_QML_2547, STR.STR_QML_2548], warning: STR.STR_QML_2549 },
        "enter-seed":  { title: STR.STR_QML_2366, headline: STR.STR_QML_2551, body: STR.STR_QML_2552, bullets: [], warning: STR.STR_QML_2553 }
    })
    readonly property var notice: noticeMap[noticeType] !== undefined ? noticeMap[noticeType] : noticeMap["use-device"]
    label.text: notice.title
    content: Item {
        Row {
            spacing: 36
            Rectangle {
                width: 346
                height: 512
                radius: 24
                color: "#D0E2FF"
                QPicture {
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/Images/Images/d_key_warning.svg"
                }
            }
            Column {
                width: 346
                spacing: 16
                QLato {
                    width: parent.width
                    text: _notice.notice.headline
                    font.weight: Font.Bold
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                }
                QLato {
                    width: parent.width
                    visible: _notice.notice.body !== ""
                    text: _notice.notice.body
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                }
                Column {
                    width: parent.width
                    spacing: 4
                    visible: _notice.notice.bullets.length > 0
                    Repeater {
                        model: _notice.notice.bullets
                        Row {
                            spacing: 8
                            QLato {
                                text: "•"
                                anchors.top: parent.top
                            }
                            QLato {
                                width: 330
                                text: modelData
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 20
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignLeft
                            }
                        }
                    }
                }
                QWarningBgMulti {
                    width: parent.width
                    icon: "qrc:/Images/Images/warning-dark.svg"
                    color: "#FDEBD2"
                    txt.text: _notice.notice.warning
                }
            }
        }
    }
}
