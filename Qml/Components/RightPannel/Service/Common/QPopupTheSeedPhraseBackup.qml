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
 **************************************************************************/
import QtQuick
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Buttons"
import "./../../../customizes/Popups"
import "./../../../customizes/services"
import "./../../../customizes/Texts"
import "../../../../../localization/STR_QML.js" as STR

// NUN-10192: "The seed phrase backup" info popup (mockup 07D standalone / 08bD both-methods).
// BUGFIX: was built on QOnScreenContentTypeA, whose rounded-corner chrome relies on a
// layer.enabled + OpacityMask combo that renders as a plain square box in this build. Rebuilt the
// shell with a native Rectangle radius (no GraphicalEffects) to match the bordered/rounded card
// style already used elsewhere in this CR (e.g. QInheritanceDetailsOffChain.qml).
QPopupEmpty {
    id: _id
    // Both-methods context (08bD) uses different "how to share"/warning copy than standalone (07D).
    property bool isJointVariant: false
    content: Item {
        // BUGFIX: fixed to popupWidth/popupHeight (800x700, same as "Share your secrets" and other
        // dialogs) instead of self-sizing to content -- was rendering a different size than the
        // rest of the app's popups.
        width: 800
        height: 700
        // BUGFIX: Item doesn't mirror width/height into implicitWidth/Height, which QPopup.qml's
        // boxmask sizing (contentInfo.implicitWidth/Height) relies on -- without this the outer
        // drop-shadow/padding rect collapses to 0x0.
        implicitWidth: width
        implicitHeight: height
        Rectangle {
            id: _frame
            anchors.fill: parent
            radius: 24
            color: "#FFFFFF"
        }
        Column {
            id: _layout
            anchors { left: parent.left; right: parent.right; top: parent.top; margins: 36 }
            spacing: 16
            QHeadLine {
                width: parent.width - 48
                text: STR.STR_QML_2316
            }
            Column {
                width: 539
                spacing: 24
                QLato {
                    width: 539
                    text: STR.STR_QML_2317
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
                        {height: 84, headline:STR.STR_QML_2318, content: STR.STR_QML_2319 , icon: "qrc:/Images/Images/1.Active.svg" },
                        {height: 84, headline:STR.STR_QML_2320, content: isJointVariant ? STR.STR_QML_2323 : STR.STR_QML_2321 , icon: "qrc:/Images/Images/2.Active.svg" },
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
                                source: _item.icon
                            }
                            Column {
                                width: 503
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
                                }
                            }
                        }
                    }
                }
                Rectangle {
                    width: 539
                    height: 72
                    color: "#FDEBD2"
                    radius: 8
                    Row {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 12
                        QIcon {
                            iconSize: 24
                            source: "qrc:/Images/Images/warning-dark.svg"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QLato {
                            width: 455
                            text: isJointVariant ? STR.STR_QML_2324 : STR.STR_QML_2322
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 20
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignLeft
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
            }
            Item {
                width: 539
                height: 48
                QTextButton {
                    anchors.right: parent.right
                    width: 73
                    height: 48
                    label.text: STR.STR_QML_341
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: _id.close()
                }
            }
        }
        QCloseButton {
            anchors { right: parent.right; rightMargin: 24; top: parent.top; topMargin: 24 }
            onClicked: _id.close()
        }
    }
}
