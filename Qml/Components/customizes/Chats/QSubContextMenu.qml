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
import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../Components/customizes/Chats"
import "../../../../localization/STR_QML.js" as STR

Menu {
    id: subOptionsMenu
    implicitWidth: menuWidth
    implicitHeight: {
        if (mapMenu == null) return 0
        var cnt = 0;
        for(var i=0; i< mapMenu.length; i++){
            var show = mapMenu[i].visible
            if(show){cnt++}
        }
        return cnt*menuHeight
    }
    property int menuWidth: 250
    property int menuHeight: 48
    property var mapMenu: [
        {
            visible: true,
            label: qsTr("menu1"),
            icon: "",
            iconRight: "",
            color: "#031F2B",
            enable: true,
            subMenu: null,
            action: function(){
                console.log("menu1")
            }
        },
    ]


    background: Rectangle {
        implicitWidth: menuWidth
        implicitHeight: mapMenu == null ? 0: mapMenu.length*menuHeight
        radius: 8
        color: "#FFFFFF"
        layer.enabled: true
        layer.effect: DropShadow {
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
        }
    }

    Repeater {
        model: mapMenu
        QMenuDelegate {
            itemMenu: modelData
            onItemClicked: {
                var _item = subOptionsMenu.mapMenu[index]
                console.log("Menu clicked: " + _item.label + " - " + _item.action)
                if (_item.action()) {
                    console.log("Menu clicked: " + _item.label)
                }
            }
        }
    }
}
