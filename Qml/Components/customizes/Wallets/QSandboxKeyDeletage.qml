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
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

Loader {
    sourceComponent: modelData.hisSigner ? hisSigner : notHisSigner
    Component {
        id: hisSigner
        Item {
            width: 352
            height: 80
            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                QCircleIcon {
                    bgSize: 48
                    icon.iconSize: 18
                    icon.typeStr: modelData.type
                    icon.tag: modelData.tag
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                Column {
                    height: childrenRect.height
                    width: 150
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4
                    QLato {
                        width: parent.width
                        text: modelData.name
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Row {
                        spacing: 4
                        Rectangle {
                            width: signerTypeText.width + 8
                            height: 16
                            color: "#EAEAEA"
                            radius: 20
                            QText {
                                id: signerTypeText
                                text: GlobalData.signers(modelData.signer_type)
                                font.family: "Lato"
                                font.weight: Font.Bold
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                color: "#031F2B"
                            }
                        }
                        Rectangle {
                            width: accttext.width + 10
                            height: 16
                            color: "#EAEAEA"
                            radius: 8
                            visible: modelData.account_index > 0
                            QText {
                                id: accttext
                                font.family: "Lato"
                                color: "#031F2B"
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                font.weight: Font.Bold
                                text: qsTr("Acct %1").arg(modelData.account_index)
                            }
                        }
                    }
                    QLato {
                        width: parent.width
                        text: {
                            if (modelData.card_id !== "") {
                                var card_id_text = modelData.card_id
                                var textR = card_id_text.substring(card_id_text.length - 5, card_id_text.length).toUpperCase()
                                return "Card ID: ••" + textR
                            } else {
                                "XFP: " + modelData.xfp
                            }
                        }
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.capitalization: Font.AllUppercase
                        font.pixelSize: 12
                    }
                    QLato {
                        width: parent.width
                        text: qsTr("BIP32 path: %1").arg(modelData.derivationPath)
                        color: "#757575"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 12
                    }
                }
            }
        }
    }
    Component {
        id: notHisSigner
        Item {
            width: 352
            height: 80
            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                Rectangle {
                    color: "#F5F5F5"
                    radius: 48
                    width: 48
                    height: 48
                    anchors.verticalCenter: parent.verticalCenter
                    QIcon {
                        iconSize: 24
                        anchors.centerIn: parent
                        source: "qrc:/Images/Images/key-dark.svg"
                    }
                }
                Column {
                    height: childrenRect.height
                    width: 150
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4
                    QLato {
                        width: parent.width
                        text: modelData.name !== "" ? modelData.name : STR.STR_QML_1557.arg(modelData.key_index + 1)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Row {
                        spacing: 4
                        Rectangle {
                            width: signerTypeText.width + 8
                            height: 16
                            color: "#EAEAEA"
                            radius: 20
                            QText {
                                id: signerTypeText
                                text: GlobalData.signers(modelData.signer_type)
                                font.family: "Lato"
                                font.weight: Font.Bold
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                color: "#031F2B"
                            }
                        }
                        Rectangle {
                            width: accttext.width + 10
                            height: 16
                            color: "#EAEAEA"
                            radius: 8
                            visible: modelData.account_index > 0
                            QText {
                                id: accttext
                                font.family: "Lato"
                                color: "#031F2B"
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                font.weight: Font.Bold
                                text: qsTr("Acct %1").arg(modelData.account_index)
                            }
                        }
                    }
                    QLato {
                        width: parent.width
                        text: {
                            if (modelData.card_id !== "") {
                                var card_id_text = modelData.card_id
                                var textR = card_id_text.substring(card_id_text.length - 5, card_id_text.length).toUpperCase()
                                return "Card ID: ••" + textR
                            } else {
                                "XFP: " + modelData.xfp
                            }
                        }
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.capitalization: Font.AllUppercase
                        font.pixelSize: 12
                    }
                    QLato {
                        width: parent.width
                        text: qsTr("BIP32 path: %1").arg(modelData.derivationPath)
                        color: "#757575"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 12
                    }
                }
            }
        }
    }
}


