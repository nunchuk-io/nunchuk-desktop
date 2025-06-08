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
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Signers"
import "../../../localization/STR_QML.js" as STR

Item {
    property string key_name: ""
    property string fingerPrint: ""
    property int type: -1
    property string notice: ""
    property string tag: ""
    property string device_type: ""
    property string bip32_path: ""
    property var modelKey
    property var existList: draftWallet.signerExistList

    onVisibleChanged: {
        if(!visible){
            fingerPrint = ""
            notice = ""
            type = -1
            tag = ""
            key_name = ""
            bip32_path = ""
        }
    }

    Column {
        spacing: 24
        QLato {
            text: notice
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            visible: notice != ""
        }
        Rectangle {
            width: 539
            height: 464
            border.width: 1
            border.color: "#EAEAEA"
            radius: 8
            QListView {
                model: existList
                clip: true
                ScrollBar.vertical: ScrollBar { active: true }
                anchors.fill: parent
                delegate: Item {
                    width: 539 - 12
                    height: visible ? 92 : 0
                    anchors{
                        left: parent.left
                        leftMargin: 12
                    }
                    Row{
                        anchors.fill: parent
                        spacing: 8
                        Rectangle {
                            width: 48
                            height: 48
                            radius: width
                            color: "#F5F5F5"
                            anchors.verticalCenter: parent.verticalCenter
                            QSignerDarkIcon {
                                iconSize: 24
                                anchors.centerIn: parent
                                device_type: modelData.signer_deviceType
                                type: modelData.signer_type
                                tag: modelData.signer_tag
                            }
                        }
                        Item{
                            width: 146
                            height: 60
                            anchors.verticalCenter: parent.verticalCenter
                            Column{
                                spacing: 4
                                QText {
                                    width: 146
                                    height: 20
                                    text: modelData.signer_name
                                    color: "#031F2B"
                                    font.weight: Font.Normal
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                }
                                QRowSingleSignerType {
                                    isPrimaryKey: modelData.signer_is_primary
                                    signerType: modelData.signer_type
                                    accountIndex: modelData.signer_account_index
                                    accountVisible: false
                                }
                                QText {
                                    width: 146
                                    height: 20
                                    text: {
                                        if (modelData.signer_type === NUNCHUCKTYPE.NFC) {
                                            var card_id_or_xfp = modelData.signer_card_id
                                            var textR = card_id_or_xfp.substring(card_id_or_xfp.length - 5, card_id_or_xfp.length).toUpperCase()
                                            return "Card ID: ••" + textR
                                        } else {
                                            return "XFP: " + modelData.signer_fingerPrint.toUpperCase()
                                        }
                                    }
                                    color: "#595959"
                                    font.weight: Font.Normal
                                    font.capitalization: Font.AllUppercase
                                    font.family: "Lato"
                                    font.pixelSize: 12
                                }
                            }
                        }
                    }
                    QIcon {
                        iconSize: 24
                        source: fingerPrint === modelData.signer_fingerPrint && key_name === modelData.signer_name ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                        scale: primaryKeyMouse.pressed ? 0.9 : 1.0
                        anchors{
                            right: parent.right
                            rightMargin: 12
                            verticalCenter: parent.verticalCenter
                        }
                    }
                    MouseArea {
                        id: primaryKeyMouse
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        anchors.fill: parent
                        onClicked: {
                            modelKey = modelData
                            fingerPrint = modelData.signer_fingerPrint
                            type = modelData.signer_type
                            tag = modelData.signer_tag
                            device_type = modelData.signer_deviceType
                            key_name = modelData.signer_name
                            bip32_path = modelData.signer_bip32_path
                        }
                    }
                }
            }
        }
    }
}
