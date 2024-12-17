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
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    height: 48
    property string coinName: "Bitcoin"
    property int coinCount: 0
    property bool isEdit: false
    property alias isChecked: _check.checked
    signal arrowClicked()
    signal checkClicked()
    property int maxChar: 1
    function getFistCharterName(label) {
        if(label !== ""){
            if (maxChar == 1) {
                var ret = label.charAt(0)
                if(label.split(' ').length > 1){ ret += label.split(' ')[(label.split(' ').length - 1)].charAt(0)}
                return ret;
            } else {
                return label.substr(0,maxChar)
            }
        }
        return ""
    }
    Row {
        id: row
        anchors.fill: parent
        spacing: 12
        Rectangle {
            width: 48
            height: 48
            radius: 24
            color: "#FDD95C"
            QLato {
                text: getFistCharterName(coinName)
                anchors.centerIn: parent
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            anchors.verticalCenter: parent.verticalCenter
        }
        Column {
            width: parent.width - 48 - 24 - 24
            height: 40
            spacing: 4
            anchors.verticalCenter: parent.verticalCenter
            QLato {
                text: coinName
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
            QLato {
                text: qsTr("%1 %2").arg(coinCount).arg(coinCount > 1 ? "coins" : "coin")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
        }
        QImage {
            source: "qrc:/Images/Images/right-arrow-dark.svg"
            width: 24
            height: 24
            visible: !isEdit
            anchors.verticalCenter: parent.verticalCenter
            MouseArea {
                anchors.fill: parent
                onClicked: arrowClicked()
            }
        }
        QCheckBox {
            id: _check
            visible: isEdit
            anchors.verticalCenter: parent.verticalCenter
            onCheckboxClicked: {
                checkClicked()
            }
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (!isEdit) {
                arrowClicked()
            } else {
                checkClicked()
            }
        }
    }
}
