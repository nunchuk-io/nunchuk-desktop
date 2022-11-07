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
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import "../origins"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    id: loggedDevicesBox
    property int boxHeight: 0
    signal signOutClicked(var id,var name)
    signal markClicked(var id,var name)
    property string device_id: ""
    property string device_name: ""
    property int typePopup: _SignOutSingle
    readonly property int _SignOutSingle: 1
    readonly property int _MarkAsCompromised: 2
    readonly property int _SignOutAll: 3
    width: 344
    height: Math.min(devices.contentHeight + 12*2,boxHeight)
    radius: 12
    border.color:"#EAEAEA"
    border.width: 2
    color: "transparent"
    InnerShadow {
        id: drshd
        anchors.fill: parent
        source: parent
    }

    QListView {
        id: devices
        width: parent.width - 12*2
        height: Math.min(devices.contentHeight,boxHeight)
        model: ClientController.devicesLogged
        anchors {
            left: parent.left
            leftMargin: 12
            top: parent.top
            topMargin: 0
        }
        clip: true
        delegate: QLoggedInDeviceDetail {
            width: devices.width
            height: 48
            verSystemName: model.name !== "" ? model.name : STR.STR_QML_549
            verDeviceName: ""
            thisDevice: model.thisId
            rightText: STR.STR_QML_546
            onSignOutClicked: {
                loggedDevicesBox.signOutClicked(model.id,model.name)
            }
            onMarkClicked: {
                loggedDevicesBox.markClicked(model.id,model.name)

            }
        }
        ScrollBar.vertical: ScrollBar { }
    }
}
