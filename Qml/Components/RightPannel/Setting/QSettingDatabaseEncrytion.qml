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
    onVisibleChanged: { if(!visible){ dbencriptionswitch.switchOn = AppSetting.enableDBEncryption } }
    Column {
        id: dbencryptionComp
        anchors.fill: parent
        anchors.margins: 24
        spacing: 10
        QText {
            font.family: "Lato"
            font.pixelSize: 28
            font.weight: Font.Bold
            text: STR.STR_QML_530
        }
        Row {
            spacing: 0
            QText {
                width: 627-84
                text: STR.STR_QML_530
                color: "#000000"
                font.pixelSize: 16
                font.family: "Lato"
                anchors.verticalCenter: parent.verticalCenter
            }
            QSwitchTypeB {
                id: dbencriptionswitch
                width: 84
                height: 48
                switchOn: AppSetting.enableDBEncryption
                anchors.verticalCenter: parent.verticalCenter
                property bool anyChanged: (AppSetting.enableDBEncryption !== dbencriptionswitch.switchOn)
                onSwitchOnChanged: {
                    if(!switchOn){
                        inputpassphrase.textOutput = ""
                        confirmpassphrase.textOutput = ""
                        pleasewait.open()
                        setpassphraseTime.start()
                    }
                }
                function applySettings(){
                    AppSetting.enableDBEncryption = dbencriptionswitch.switchOn
                }
            }
        }
        Column {
            anchors.margins: 24
            spacing: 24
            visible: dbencriptionswitch.switchOn
            QTextInputBox {
                id: inputpassphrase
                width: 627
                border.color: "#C9DEF1"
                placeholder.text: STR.STR_QML_162
                echoMode: showpass.visiblity ? TextInput.Normal : TextInput.Password
                errorText.visible: !validInput
                validInput: (inputpassphrase.length === 0 ) || (inputpassphrase.length >= 8)
                color: "#FFFFFF"
                errorText.text: STR.STR_QML_163
                errorText.color: "#E02247"
                QIcon {
                    iconSize: 24
                    id: showpass
                    property bool visiblity: false
                    source: showpass.visiblity ? "qrc:/Images/Images/visibility-dark.svg":"qrc:/Images/Images/visibility-off-dark.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 17
                    visible: (inputpassphrase.textOutput !== "")
                    MouseArea {
                        anchors.fill: parent
                        onClicked: showpass.visiblity =! showpass.visiblity
                    }
                }
            }
            QTextInputBox {
                id: confirmpassphrase
                width: 627
                border.color: "#C9DEF1"
                placeholder.text: STR.STR_QML_166
                echoMode: showconfirmpass.visiblity ? TextInput.Normal : TextInput.Password
                color: "#FFFFFF"
                errorText.visible: !validInput
                validInput: ((confirmpassphrase.length === 0 ) || (confirmpassphrase.length >= 8)) && (inputpassphrase.textOutput === confirmpassphrase.textOutput)
                errorText.text: {
                    if(((confirmpassphrase.length === 0 ) || (confirmpassphrase.length >= 8)) && (inputpassphrase.textOutput !== confirmpassphrase.textOutput)) {return STR.STR_QML_531}
                    else { return STR.STR_QML_163 }
                }
                errorText.color: "#E02247"
                QIcon {
                    iconSize: 24
                    id: showconfirmpass
                    property bool visiblity: false
                    source: showconfirmpass.visiblity ? "qrc:/Images/Images/visibility-dark.svg":"qrc:/Images/Images/visibility-off-dark.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 17
                    visible: (confirmpassphrase.textOutput !== "")
                    MouseArea {
                        anchors.fill: parent
                        onClicked: showconfirmpass.visiblity =! showconfirmpass.visiblity
                    }
                }
            }
            Item {
                width: 627
                height: 48
                QTextButton {
                    id: changepassbtn
                    width: 162
                    label.text: STR.STR_QML_167
                    label.font.pixelSize: 14
                    type: eTypeB
                    anchors.right: parent.right
                    enabled: (inputpassphrase.length >= 8) && (confirmpassphrase.length >= 8) &&
                             (inputpassphrase.textOutput === confirmpassphrase.textOutput)
                    onButtonClicked: {
                        pleasewait.open()
                        setpassphraseTime.start()
                    }
                }
            }
        }
        Timer {
            id: setpassphraseTime
            interval: 1000
            onTriggered: {
                pleasewait.close()
                if(AppModel.enableDatabaseEncryption(inputpassphrase.textOutput)){
                    if(dbencriptionswitch.anyChanged) { dbencriptionswitch.applySettings() }
                }
                else{
                    dbencriptionswitch.switchOn = AppSetting.enableDBEncryption
                }
            }
        }
    }
}
