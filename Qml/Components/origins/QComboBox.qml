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
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.3
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1

ComboBox {
    id: control
    width: 627
    height: 48
    signal signalActivated
    property var current_id
    property var defaultValue
    function getIndex(id) {
        var list = control.model
        for (var i = 0; i < list.length; i++) {
            if (list[i].id === id) {
                return i;
            }
        }
        return -1;
    }
    function getDisplay() {
        return control.getIndex(current_id.id) < 0 ? defaultValue : current_id
    }
    function getText(data)
    {
        return data.displayName
    }
    onActivated: {
        if (currentIndex >=0) {
            current_id = control.model[control.currentIndex]
            signalActivated()
        }
    }

    validator: IntValidator {
        top: 0
        bottom: 8
    }
    background: Rectangle{
        width: control.width
        height: control.height
        radius: 8
        color:"#FFFFFF"
        border.color: "#DEDEDE"
        border.width: 1
    }
    indicator: QIcon {
        iconSize: 24
        id: name
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        source: "qrc:/Images/Images/expand-more-dark.svg"
    }
    popup: Popup {
        y: control.height
        width: control.width
        implicitHeight: Math.min(contentItem.implicitHeight, 768)
        padding: 0
        background: Rectangle {
            anchors.fill: parent
            border.width: 1
            border.color: "#DEDEDE"
            color:"#FFFFFF"
            radius: 8
            layer.enabled: true
            layer.effect: DropShadow {
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
            }
        }
        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
//          section.property: "currency"
//          section.criteria: ViewSection.FirstCharacter
//          section.delegate: Label {
//              x: 10
//              text: section
//          }
            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }
    delegate: ItemDelegate {
        text: getText(modelData)
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: 48
        QIcon {
            iconSize: 24
            anchors {
                right: parent.right
                rightMargin: 24
                verticalCenter: parent.verticalCenter
            }
            visible: current_id.id === modelData.id
            source: "qrc:/Images/Images/check-dark.svg"
        }
        font.weight: current_id.id === modelData.id ? Font.DemiBold : Font.Normal
        font.pixelSize: 16
        highlighted: control.highlightedIndex == index
    }
}
