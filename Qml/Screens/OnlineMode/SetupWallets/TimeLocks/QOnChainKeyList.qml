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
import QtQuick.Controls
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Signers"
import "../../../../../localization/STR_QML.js" as STR

Column {
    id: _keyList
    property var model: []
    property bool isKeyHolderLimited: false
    signal inheritanceClicked(var key)
    signal hardwareClicked(var key)
    signal serverClicked(var key)
    signal backupClicked(var key)
    signal changeClicked(var key)
    width: parent.width
    spacing: 16
    Repeater {
        model: _keyList.model
        Item {
            width: _keyList.width
            height: _req.height
            QAddRequestKey {
                id: _req
                width: parent.width
                onInheritanceKeyClicked: _keyList.inheritanceClicked(modelData)
                onHardwareClicked: _keyList.hardwareClicked(modelData)
                onSerkeyClicked: _keyList.serverClicked(modelData)
                onBackupClicked: _keyList.backupClicked(modelData)
                onChangeClicked: _keyList.changeClicked(modelData)
            }
            Rectangle {
                anchors.fill: parent
                visible: modelData.has_error === true
                color: "transparent"
                radius: 8
                border.width: 2
                border.color: "#CF4018"
            }
        }
    }
}
