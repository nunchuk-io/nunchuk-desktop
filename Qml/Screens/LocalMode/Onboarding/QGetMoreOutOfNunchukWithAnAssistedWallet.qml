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

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1247
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    readonly property var option_map: [
        {visible: true,        height: 164, headline:STR.STR_QML_1230, content: STR.STR_QML_1231, icon: "qrc:/Images/Images/mulitsig-dark.svg"             },
        {visible: true,        height: 136, headline:STR.STR_QML_1232, content: STR.STR_QML_1233, icon: "qrc:/Images/Images/inheritance-dark.svg"          },
        {visible: true,        height: 136, headline:STR.STR_QML_1393, content: STR.STR_QML_1394, icon: "qrc:/Images/Images/groups-24px.svg"               },
        {visible: true,        height: 108, headline:STR.STR_QML_1234, content: STR.STR_QML_1235, icon: "qrc:/Images/Images/emergency-lockdown-dark.svg"   },
        {visible: true,        height: 108, headline:STR.STR_QML_1236, content: STR.STR_QML_1237, icon: "qrc:/Images/Images/signing-policy-dark.svg"       },
        {visible: true,        height: 136, headline:STR.STR_QML_1238, content: STR.STR_QML_1239, icon: "qrc:/Images/Images/key-recovery-dark.svg"         },
        {visible: true,        height: 80,  headline:STR.STR_QML_1240, content: STR.STR_QML_1241, icon: "qrc:/Images/Images/contact-support-dark.svg"      },
        {visible: true,        height: 108, headline:STR.STR_QML_1242, content: STR.STR_QML_1243, icon: "qrc:/Images/Images/member-discount-dark.svg"      },
    ]
    content: Item {
        Flickable {
            id: _flick
            width: 728
            height: 462
            contentWidth: parent.width; contentHeight: 664
            clip: true
            ScrollBar.vertical: ScrollBar { active: true }
            Column {
                spacing: 24
                QLato {
                    width: 600
                    height: 28
                    anchors.left: parent.left
                    text: STR.STR_QML_1246
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle {
                    width: 728
                    height: 612
                    color: "#F5F5F5"
                    radius: 12
                    Flow {
                        width: parent.width
                        spacing: 16
                        Repeater {
                            width: parent.width
                            model: option_map.length
                            Item {
                                property var _item: option_map[index]
                                width: 336
                                height: _item.height
                                visible: _item.visible
                                Row {
                                    spacing: 12
                                    Item {
                                        width: 48
                                        height: 48
                                        visible: _ico.source != ""
                                        QIcon {
                                            iconSize: 24
                                            id: _ico
                                            anchors.centerIn: parent
                                            source: _item.icon
                                        }
                                    }
                                    Column {
                                        width: 276
                                        height: _item.height
                                        spacing: 4
                                        QText {
                                            width: 276
                                            text: _item.headline
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            font.pixelSize: 16
                                            font.weight: Font.DemiBold
                                            horizontalAlignment: Text.AlignLeft
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        QText {
                                            width: 276
                                            text: _item.content
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            font.pixelSize: 16
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
        }
    }

    onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    bottomRight: QTextButton {
        width: 153
        height: 48
        label.text: STR.STR_QML_1248
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            Qt.openUrlExternally("https://nunchuk.io/individuals")
        }
    }
}
