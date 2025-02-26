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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Loader {
    id: _source
    signal hardwareClicked()
    signal removeClicked(var idx)
    signal recoverKeyClick(var idx, var xfp)
    signal bip32PathClick(var xfp, var path)
    sourceComponent: modelData.has ? (modelData.hisSigner ? signerAdded : notMeAdded) : signerAdd
    Component {
        id: signerAdd
        QDashRectangle {
            width: 352
            height: 72
            radius: 8
            borderWitdh: 2
            borderColor: "#031F2B"
            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                QBadge {
                    width: 48
                    height: 48
                    iconSize: 24
                    radius: 48
                    icon: "qrc:/Images/Images/Device_Icons/key-dark.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                Column {
                    width: 150
                    spacing: 4
                    anchors.verticalCenter: parent.verticalCenter
                    QLato {
                        width: 150
                        text: STR.STR_QML_1557.arg(modelData.key_index + 1)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QLato {
                        width: 150
                        text: STR.STR_QML_1665
                        color: "#A66800"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        visible: modelData.isOccupied
                    }
                }
            }
            QTextButton {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                width: label.paintedWidth + 2*16
                height: 36
                type: eTypeB
                label.text: modelData.isAdded ? STR.STR_QML_104 : STR.STR_QML_106
                label.font.pixelSize: 12
                visible: !modelData.isAdded
                onButtonClicked: {
                    hardwareClicked()
                }
            }
            QBadge {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                icon: "qrc:/Images/Images/check-circle-dark.svg"
                height: 36
                font.weight: Font.Bold
                font.pixelSize: 12
                text: STR.STR_QML_104
                visible: modelData.isAdded
            }
        }
    }
    Component {
        id: signerAdded
        Rectangle {
            width: 352
            height: maxColumn.childrenRect.height + 12*2
            border.color: "#DEDEDE"
            border.width: 1
            color: "transparent"
            radius: 8
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
                    id: maxColumn
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
                        width: parent.width + 4 + 12
                        text: qsTr("BIP32 path: %1").arg(modelData.derivationPath)
                        color: "#757575"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 12
                        font.underline: sandbox.url !== ""
                        QIcon {
                            iconSize: 12
                            source: "qrc:/Images/Images/editBIP32.svg"
                            anchors {
                                verticalCenter: parent.verticalCenter
                                right: parent.right
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                bip32PathClick(modelData.xfp, modelData.derivationPath)
                            }
                        }
                    }
                }
            }
            QTextButton {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                width: label.paintedWidth + 2*16
                height: 36
                type: eTypeB
                label.text: STR.STR_QML_1134
                label.font.pixelSize: 12
                visible: sandbox.url !== ""
                onButtonClicked: {
                    removeClicked(index)
                }
            }
        }
    }
    Component {
        id: notMeAdded
        Rectangle {
            width: 352
            height: maxColumn.childrenRect.height + 12*2
            border.color: "#DEDEDE"
            border.width: 1
            color: "transparent"
            radius: 8
            property var colorData: tmpColors[modelData.colorIndex]
            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                QBadge {
                    width: 48
                    height: 48
                    iconSize: 24
                    radius: 48
                    icon: "qrc:/Images/Images/Device_Icons/key-dark.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                    visible: sandbox.url === ""
                }
                Rectangle {
                    border.width: 1
                    border.color: "#DEDEDE"
                    color: colorData.colorStr
                    radius: 48
                    width: 48
                    height: 48
                    anchors.verticalCenter: parent.verticalCenter
                    QIcon {
                        iconSize: 18
                        anchors.centerIn: parent
                        source: colorData.url
                    }
                    visible: sandbox.url !== ""
                }
                Column {
                    id: maxColumn
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
            QTextButton {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                width: label.paintedWidth + 2*16
                height: 36
                type: eTypeB
                label.text: STR.STR_QML_106
                label.font.pixelSize: 12
                visible: sandbox.url === ""
                onButtonClicked: {
                    recoverKeyClick(index, modelData.xfp)
                }
            }
        }
    }
}
