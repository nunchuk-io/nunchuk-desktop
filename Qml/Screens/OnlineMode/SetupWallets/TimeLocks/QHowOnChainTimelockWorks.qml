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
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1950
    property var    user: ClientController.user
    property var    walletInfo: AppModel.walletInfo
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    readonly property var hb_description_map: [
        {height: 40,  description: STR.STR_QML_1889 },
        {height: 40,  description: STR.STR_QML_1890 },
    ]
    readonly property var hb_premier_description_map: [
        {height: 40,  description: STR.STR_QML_1892 },
        {height: 40,  description: STR.STR_QML_1893 },
    ]
    readonly property var free_description_map: [
        {height: 40,  description: STR.STR_QML_1892 },
        {height: 40,  description: STR.STR_QML_1893 },
    ]
    function guideImageSource() {
        var m = dashInfo.mInfo
        var n = dashInfo.nInfo
        var allowInheritance = dashInfo.allowInheritance
        if (m === 2 && n === 4) {
            return "qrc:/Images/Images/inheritance-illustration-2-of-4-and-1-of-3.svg"
        }
        else if (m === 3 && n === 5) {
            return allowInheritance ? "qrc:/Images/Images/inheritance-illustration-3-of-5-and-2-of-4.svg"
                                    : "qrc:/Images/Images/inheritance-illustration-3-of-5-and-2-of-4-no-inheritance.svg"
        }
        else {
            return "qrc:/Images/Images/inheritance-illustration-3-of-5-and-2-of-4-no-inheritance.svg"
        }
    }
    function guideTextMap() {
        if (walletInfo.isHoneyBadger) {
            return hb_description_map
        }
        else if (walletInfo.isHoneyBadgerPremier || walletInfo.isByzantinePro) {
            return hb_premier_description_map;
        }
        else if (walletInfo.isFreeWallet) {
            return free_description_map;
        }
        else {
            return hb_description_map;
        }
    }
    function guideInheritance() {
        var m = dashInfo.mInfo
        var n = dashInfo.nInfo
        if (m === 2 && n === 4) {
            return STR.STR_QML_940_one
        } else if (m === 3 && n === 5) {
            return STR.STR_QML_940_two
        } else {
            // Default fallback
            return ""
        }
    }
    content: Item {
        Row {
            spacing: 36
            Rectangle {
                width: 346
                height: 512
                radius: 24
                color: "#D0E2FF"
                QSvgImage {
                    width: 346
                    height: width * (implicitHeight / implicitWidth)
                    anchors.centerIn: parent
                    source: guideImageSource()
                }
            }
            Item {
                width: 346
                height: 512
                Column {
                    width: parent.width
                    spacing: 4
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_1951
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 20
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Repeater {
                        id: _guide
                        width: parent.width                        
                        readonly property var content_map: guideTextMap()
                        model: content_map.length
                        Rectangle {
                            property var _item: _guide.content_map[index]
                            width: 346
                            height: _itemrow.implicitHeight + 8
                            Row {
                                id: _itemrow
                                spacing: 12
                                anchors.verticalCenter: parent.verticalCenter
                                QLato {
                                    text: "•"
                                    font.pixelSize: 24
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment: Text.AlignTop
                                    anchors.top: parent.top
                                }
                                QLato {
                                    width: 310
                                    text: _item.description
                                    color: "#031F2B"
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                    font.weight: Font.Normal
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 20
                                    wrapMode: Text.WordWrap
                                    horizontalAlignment: Text.AlignLeft
                                }
                            }
                        }
                    }
                    QLato {
                        width: parent.width
                        text: guideInheritance()
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 20
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        visible: dashInfo.allowInheritance
                    }
                }
            }
        }
    }
}
