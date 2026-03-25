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
import NUNCHUCKTYPE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Signers"

Item {
    property int widthList: 539
    property string title: ""
    Column {
        spacing: 24
        QLato {
            text: title
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            visible: title != ""
            font.weight: Font.Bold
        }
        Item {
            width: widthList
            height: 464
            QListView {
                model: vm.signerList
                clip: true
                ScrollBar.vertical: ScrollBar { active: true }
                anchors.fill: parent
                delegate: Item {
                    width: widthList - 12
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
                                device_type: single_signer_devicetype
                                type: single_signer_type
                                tag: single_signer_tag
                            }
                        }
                        Item{
                            width: 146
                            height: 60
                            anchors.verticalCenter: parent.verticalCenter
                            Column{
                                spacing: 4
                                QLato {
                                    width: 146
                                    height: 20
                                    text: singleSigner_name
                                    font.weight: Font.Normal
                                    font.pixelSize: 16
                                }
                                QRowSingleSignerType {
                                    isPrimaryKey: single_signer_primary_key
                                    signerType: single_signer_type
                                    accountIndex: single_signer_account_index
                                    accountVisible: {
                                        if (vm.walletType === NUNCHUCKTYPE.MINISCRIPT) {
                                            return true
                                        } else {
                                            return single_signer_account_index > 0
                                        }
                                    }
                                }
                                QText {
                                    width: 146
                                    height: 20
                                    text: {
                                        if (single_signer_type === NUNCHUCKTYPE.NFC) {
                                            var card_id_or_xfp = single_signer_device_cardid
                                            var textR = card_id_or_xfp.substring(card_id_or_xfp.length - 5, card_id_or_xfp.length).toUpperCase()
                                            return "Card ID: ••" + textR
                                        } else {
                                            return "XFP: " + singleSigner_masterFingerPrint.toUpperCase()
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
                        source: vm.selectedXfp === singleSigner_masterFingerPrint && 
                                vm.accountIndex === single_signer_account_index ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                        
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
                            vm.selectExistingKey(singleSigner_masterFingerPrint, single_signer_account_index)
                        }
                    }
                }
            }
        }
    }
}
