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
    Column {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 10
        QText {
            font.family: "Lato"
            font.pixelSize: 28
            font.weight: Font.Bold
            text: STR.STR_QML_515
        }
        Row {
            id: hwidrivertitle
            spacing: 0
            QText {
                width: 627-84
                text: STR.STR_QML_168
                color: "#000000"
                font.pixelSize: 16
                font.family: "Lato"
                anchors.verticalCenter: parent.verticalCenter
            }
            QSwitchTypeB {
                id: hwidriverswitch
                width: 84
                height: 48
                switchOn: AppSetting.enableCustomizeHWIDriver
                anchors.verticalCenter: parent.verticalCenter
                onButtonClicked: {
                    hwidrivertitle.applySettingImmediately()
                }
            }
            property bool anyChanged: (AppSetting.enableCustomizeHWIDriver !== hwidriverswitch.switchOn)
            function applySettings(){
                AppSetting.enableCustomizeHWIDriver = hwidriverswitch.switchOn
            }
            function applySettingImmediately() {
                if(hwidrivertitle.anyChanged) { hwidrivertitle.applySettings() }
                if(AppModel.updateSettingRestartRequired()){
                    modelRestartApp.open()
                }
            }
        }
        QTextInputBoxTypeB {
            id: hwipath
            label: STR.STR_QML_169
            boxWidth: 627
            boxHeight: 48
            isValid: true
            visible: hwidriverswitch.switchOn
            textInputted: AppSetting.hwiPath
            property bool anyChanged: (AppSetting.hwiPath !== hwipath.textInputted)
            function applySettings(){
                AppSetting.hwiPath = hwipath.textInputted
            }
        }
        Item {
            width: 627
            height: 48
            visible: hwidriverswitch.switchOn
            QTextButton {
                width: 189
                height: 48
                anchors.right: parent.right
                label.text: STR.STR_QML_534
                label.font.pixelSize: 16
                label.font.family: "Lato"
                type: eTypeE
                enabled: hwidrivertitle.anyChanged || hwipath.anyChanged
                onButtonClicked: {
                    if(hwidrivertitle.anyChanged) { hwidrivertitle.applySettings() }
                    if(hwipath.anyChanged) { hwipath.applySettings() }
                    if(AppModel.updateSettingRestartRequired()){
                        modelRestartApp.open()
                    }
                }
            }
        }
    }
}
