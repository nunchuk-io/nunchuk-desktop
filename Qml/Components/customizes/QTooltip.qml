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
import "../origins"
import "../customizes/Texts"
import "../customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QImage {
    id: tooltipRoot
    width: 24
    height: 24
    property string toolTip
    property string titleStr: STR.STR_QML_339
    property bool   showToolTip: false
    property bool   rightOfParent: false
    property int    tipWidth: 300
    property int    pointerPositionIndex: 9
    property int    pointerPositionRatio: 10
    property int    pointerSize: 20
    property real   pointerH: Math.sqrt(2) * pointerSize / 2
    property int    effectiveIndex: pointerPositionIndex === 0 ? 1 : pointerPositionIndex === pointerPositionRatio ? pointerPositionRatio - 1 : pointerPositionIndex
    property real   pointerRatio: effectiveIndex / pointerPositionRatio
    property real   pointerOffset: tipWidth * (0.5 - pointerRatio)

    source: "qrc:/Images/Images/tooltip.png"

    Loader {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: pointerOffset
        anchors.bottom: parent.top
        anchors.bottomMargin: -(parent.height/2) + pointerH
        sourceComponent: showToolTip ? calloutMemo : null
    }
    Rectangle {
        width: pointerSize
        height: pointerSize
        color: "#031F2B"
        rotation: 45
        anchors.bottom: parent.top
        anchors.bottomMargin: width/2
        anchors.horizontalCenter: parent.horizontalCenter
        visible: showToolTip
    }

    Component {
        id: calloutMemo
        Item {
            implicitWidth: tipWidth
            implicitHeight: textItem.height + 24
            Popup {
                id: calloutCanvas
                width: parent.width
                height: parent.height
                modal: true
                dim: false
                visible: showToolTip
                background: Item {}
                Rectangle {
                    id: textItem
                    width: tipWidth
                    height: textContent.height + 24
                    color: "#031F2B"
                    radius: 10
                    QLato {
                        id: textContent
                        anchors.centerIn: parent
                        font.pixelSize: 13
                        color: "#FFFFFF"
                        text: tooltipRoot.toolTip
                        wrapMode: Text.WordWrap
                        width: tipWidth - 24
                        height: paintedHeight
                        lineHeight: 18
                        lineHeightMode: Text.FixedHeight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        pointerPositionIndex++
                        if(pointerPositionIndex > pointerPositionRatio) {
                            pointerPositionIndex = 0
                        }
                    }
                }
            }
        }
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onEntered: { showToolTip = true }
        onExited:  { showToolTip = false }
        hoverEnabled: true
        cursorShape: Qt.WhatsThisCursor
    }
}
