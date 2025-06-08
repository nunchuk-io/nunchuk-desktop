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
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../SetupWallets"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1046
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Item {
        Column {
            spacing: 32
            Item {
                width: parent.width
                height: 64
                Row {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 12
                    Rectangle {
                        width: 48
                        height: 48
                        radius: width
                        color: "#F5F5F5"
                        QSignerDarkIcon {
                            iconSize: 24
                            anchors.centerIn: parent
                            device_type: inputtingIndex.device_type
                            type: inputtingIndex.signer_type
                            tag: inputtingIndex.device_tag
                        }
                    }
                    Column {
                        spacing: 4
                        anchors.verticalCenter: parent.verticalCenter
                        QText {
                            width: 146
                            height: 20
                            text: inputtingIndex.device_name
                            color: "#031F2B"
                            font.weight: Font.Normal
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        QText {
                            width: 146
                            height: 20
                            text: "XFP: " + inputtingIndex.device_xfp
                            color: "#595959"
                            font.weight: Font.Normal
                            font.capitalization: Font.AllUppercase
                            font.family: "Lato"
                            font.pixelSize: 12
                        }
                    }
                }
            }
            QText {
                width: 540
                font.family: "Lato"
                font.pixelSize: 16
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
                wrapMode: Text.WordWrap
                text: STR.STR_QML_1171
            }
            Column {
                spacing: 4
                QTextInputBoxTypeB {
                    label: STR.STR_QML_1172
                    boxWidth: 350
                    boxHeight: 48
                    isValid: true
                    enabled: false
                    textInputted: (inputtingIndex.current_index >= 0) ? inputtingIndex.current_index : ""
                }
                QText {
                    visible: (inputtingIndex.current_index >= 0)
                    text: "BIP32 path: " + inputtingIndex.device_bip32_path
                }
            }
            Column {
                spacing: 4
                QTextInputBoxTypeB {
                    id: newIndexInputted
                    label: STR.STR_QML_1173
                    boxWidth: 350
                    boxHeight: 48
                    isValid: true
                    validator:  RegExpValidator {regExp: /^[0-9][0-9]*$/ }
                    textInputted: inputtingIndex.current_index === -1 ? 0 : ""
                    onTextInputtedChanged: {
                        if(textInputted !== ""){
                            inputtingIndex.new_index = textInputted
                        }
                        else{
                            inputtingIndex.new_index = -1
                        }
                    }
                }
                QText {
                    visible: newIndexInputted.textInputted !== ""
                    text: "BIP32 path: " + ((AppSetting.primaryServer === NUNCHUCKTYPE.MAIN) ? qsTr("m/48h/0h/%1h/2h").arg(newIndexInputted.textInputted !== "" ? newIndexInputted.textInputted : "0")
                                                                                             : qsTr("m/48h/1h/%1h/2h").arg(newIndexInputted.textInputted !== "" ? newIndexInputted.textInputted : "0"))
                }
            }
        }
    }

    onPrevClicked: { closeTo(NUNCHUCKTYPE.CURRENT_TAB) }
    bottomRight: QTextButton {
        width: 120
        height: 48
        label.text: STR.STR_QML_265
        label.font.pixelSize: 16
        type: eTypeE
        enabled: inputtingIndex.new_index !== -1
        onButtonClicked: {
            walletInfo.updateKeyReplace(inputtingIndex.device_xfp, inputtingIndex.new_index)
        }
    }
}
