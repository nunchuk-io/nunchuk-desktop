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
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import Features.Common.ViewModels 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: _timezone
    readonly property string selectedTimezone: vm.timezones.selectedTimezone
    property alias timelockVM: vm
    modal: true
    focus: true
    width: 350
    height: 300
    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    background: Item{}
    Rectangle {
        id: timezoneBoxMask
        width: _timezone.width
        height: _timezone.height
        radius: 12
        color: "#FFFFFF"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: _timezone.width
                height: _timezone.height
                radius: 12
            }
        }
        QListView {
            id: timezonelist
            anchors.fill: parent
            anchors.margins: 16
            model: vm.timezones
            currentIndex: vm.timezones.currentIndex
            clip: true
            snapMode: ListView.SnapToItem
            delegate:  Item {
                width: timezonelist.width
                height: 48
                Row {
                    spacing: 12
                    QLato {
                        width: 290
                        text: model.timezoneName
                        font.pixelSize: 16
                        color: "#031F2B"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QIcon {
                        iconSize: 24
                        source: "qrc:/Images/Images/check-dark.svg"
                        anchors.verticalCenter: parent.verticalCenter
                        visible: timezonelist.currentIndex === index
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        vm.timezones.currentIndex = index;
                        _timezone.close();
                    }
                }
            }
        }
    }

    DropShadow {
        anchors.fill: timezoneBoxMask
        horizontalOffset: 3
        verticalOffset: 5
        spread: 0
        radius: 8
        samples: 30
        color: "#aa000000"
        source: timezoneBoxMask
    }

    TimezoneViewModel {
        id: vm
        Component.onCompleted: {
            vm.attachContext(vmContext)
        }
    }
}
