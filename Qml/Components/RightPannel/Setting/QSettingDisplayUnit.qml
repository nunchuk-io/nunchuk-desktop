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
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "./../../origins"
import "./../../customizes"
import "./../../customizes/Chats"
import "./../../customizes/Texts"
import "./../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    readonly property bool anyChanged: unitselection.anyChanged

    function applySettings(){
        unitselection.applySettings()
    }
    Column {
        id: unitselection
        anchors.fill: parent
        anchors.margins: 24
        spacing: 24
        property int unitSelected: AppSetting.unit
        property bool enableFixedPrecision: AppSetting.enableFixedPrecision
        QText {
            font.family: "Lato"
            font.pixelSize: 28
            font.weight: Font.Bold
            text: STR.STR_QML_519
        }
        QRadioButtonTypeA {
            id: btc
            width: 627
            height: 24
            label: "BTC"
            fontFamily: "Lato"
            fontPixelSize: 16
            fontWeight: btc.selected ? Font.ExtraBold : Font.DemiBold
            selected: (unitselection.unitSelected === NUNCHUCKTYPE.BTC)
            onButtonClicked: {
                unitselection.unitSelected = NUNCHUCKTYPE.BTC
                unitselection.applySettingImmediately()
            }
        }
        Row {
            spacing: 8
            width: 627
            enabled: (unitselection.unitSelected === NUNCHUCKTYPE.BTC)
            QText {
                width: parent.width - icon.width - 8
                height: 16
                anchors.verticalCenter: parent.verticalCenter
                text: STR.STR_QML_520
                font.family: "Lato"
                font.pixelSize: 12
                font.weight: unitselection.enableFixedPrecision && btc.selected ? Font.Bold : Font.Normal
            }
            QIcon {
                iconSize: 24
                id: icon
                anchors.verticalCenter: parent.verticalCenter
                source: unitselection.enableFixedPrecision ? "qrc:/Images/Images/checkbox-checked-dark.svg" : "qrc:/Images/Images/checkbox-dark.svg"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        unitselection.enableFixedPrecision = !unitselection.enableFixedPrecision
                        unitselection.applySettingImmediately()
                    }
                }
            }
        }
        Rectangle { width: 627; height: 1; color: "#EAEAEA"}
        QRadioButtonTypeA {
            id: satoshi
            width: 627
            height: 24
            label: "Satoshi (1 BTC = 100,000,000 sat)"
            fontFamily: "Lato"
            fontWeight: satoshi.selected ? Font.ExtraBold : Font.DemiBold
            fontPixelSize: 16
            selected: (unitselection.unitSelected === NUNCHUCKTYPE.SATOSHI)
            onButtonClicked: {
                unitselection.unitSelected = NUNCHUCKTYPE.SATOSHI
                unitselection.applySettingImmediately()
            }
        }
        property bool anyChanged: (unitselection.unitSelected !== AppSetting.unit) || (unitselection.enableFixedPrecision !== AppSetting.enableFixedPrecision)
        function applySettings(){
            AppSetting.unit = unitselection.unitSelected
            AppSetting.enableFixedPrecision = unitselection.enableFixedPrecision
        }
        function applySettingImmediately() {
            if(unitselection.anyChanged) { unitselection.applySettings() }
        }
    }
}
