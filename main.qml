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
import HMIEVENTS 1.0
import "./Qml/Components/customizes"
import "./localization/STR_QML.js" as STR

Item {
    width: QAPP_DEVICE_WIDTH
    height: QAPP_DEVICE_HEIGHT
    antialiasing: true
    smooth: true
    property var screen_layer_base: screen_layer_1
    Loader {
        id: screen_layer_1
        anchors.fill: parent
        transformOrigin: Item.TopLeft
        onLoaded: { switch_screen_layer_base( screen_layer_2, screen_layer_1 ); }
    }
    Loader {
        id: screen_layer_2
        anchors.fill: parent
        transformOrigin: Item.TopLeft
        onLoaded: { switch_screen_layer_base( screen_layer_1, screen_layer_2 ); }
    }
    function switch_screen_layer_base(from, to) {
        if ( from.item !== null ) { from.item.visible = false; from.sourceComponent = null }
        if ( to.item !== null ) { to.item.visible = true }
        screen_layer_base = from;
    }
    function screen_Transition() {
        screen_layer_base.sourceComponent = null
        screen_layer_base.sourceComponent = QAppScreen
    }
    ListModel { id: onsDataList }
    Repeater {
        id: ons_Creator
        transformOrigin: Item.TopLeft
        Loader {
            id: osd_Loader
            source: osdSource
        }
    }
    function load_Popup(Onsdata, popCount) {
        onsDataList.clear()
        for(var onsCnt = 0; onsCnt < popCount; onsCnt++ ) { var data = {'osdSource': Onsdata[onsCnt]}; onsDataList.append(data); }
        ons_Creator.model = onsDataList
    }

    property int popupWidth: 800
    property int popupHeight: 700

    QPopupInfo{
        id:_checkforupdate
        z:100
        coverColor: Qt.rgba(255, 255, 255, 0)
        onGotItClicked: {

        }
    }
    function funcUpdateAvailable(title,message,doItLaterCTALbl){
        _checkforupdate.title = title
        _checkforupdate.contentText = message
        _checkforupdate.btnLabel = doItLaterCTALbl
        _checkforupdate.open()
    }
    function funcUpdateRequired(title,message,doItLaterCTALbl){
        _checkforupdate.title = title
        _checkforupdate.contentText = message
        _checkforupdate.btnLabel = doItLaterCTALbl
        _checkforupdate.open()
    }
}
