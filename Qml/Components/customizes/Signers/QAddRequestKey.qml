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

Item {
    width: 322
    height: 72
    signal tapsignerClicked()
    signal serkeyClicked()
    signal hardwareClicked()
    signal backupClicked()
    Loader {
        id: _source
        anchors.fill: parent
        sourceComponent: {
            if(modelData.is_inheritance) {
                return modelData.has ? inheritanceAdded : inheritanceAdd
            }
            else if (modelData.type === "SERVER") {
                return modelData.has ? serverAdded : serverAdd
            }
            else {
                return modelData.has ? hardwareAdded : hardwareAdd
            }
        }
    }

    FastBlur {
        anchors.fill: _source
        source: _source
        radius: 32
        visible: modelData.type === "SERVER" ? isKeyHolderLimited : (isKeyHolderLimited && modelData.has && !modelData.ourAccount)
    }

    Component {
        id: inheritanceAdd
        QDashRectangle {
            anchors.fill: parent
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
                    width: 36
                    height: 36
                    iconSize: 18
                    icon: "qrc:/Images/Images/Device_Icons/key-dark.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                Column {
                    height: childrenRect.height
                    width: 150
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4
                    QLato {
                        width: 150
                        height: 28
                        text: STR.STR_QML_954.arg(modelData.key_index + 1)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QBadge {
                        width: 77
                        height: 16
                        fontSize: 10
                        text: STR.STR_QML_1600
                        color: "#EAEAEA"
                    }
                }
            }
            QTextButton {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                width: label.paintedWidth + 2*20
                height: 36
                type: eTypeB
                label.text: STR.STR_QML_941
                label.font.pixelSize: 16
                onButtonClicked: {
                    tapsignerClicked()
                }
            }
        }
    }
    Component {
        id: inheritanceAdded
        Rectangle {
            anchors.fill: parent
            color: modelData.user_key !== null ? "#A7F0BA" : "#FDEBD2"
            radius: 8
            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                QCircleIcon {
                    bgSize: 36
                    icon.iconSize: 24
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
                            visible: modelData.signer_type !== NUNCHUCKTYPE.SERVER
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
                            visible: (modelData.account_index > 0) && (modelData.signer_type !== NUNCHUCKTYPE.SERVER)
                            radius: 8
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
                    Item {
                        width: parent.width
                        height: 16
                        QLato {
                            visible: modelData.card_id !== ""
                            width: parent.width
                            text: {
                                var card_id_text = modelData.card_id
                                var textR = card_id_text.substring(card_id_text.length - 5, card_id_text.length).toUpperCase()
                                return "Card ID: ••" + textR
                            }
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.capitalization: Font.AllUppercase
                            font.pixelSize: 12
                        }
                        QLato {
                            visible: modelData.xfp !== ""
                            width: parent.width
                            text: "XFP: " + modelData.xfp
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.capitalization: Font.AllUppercase
                            font.pixelSize: 12
                        }
                    }
                }
            }
            QBadge {
                visible: modelData.user_key !== null
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                width: 75
                height: 24
                iconSize: 24
                icon: "qrc:/Images/Images/check-circle-dark.svg"
                text: STR.STR_QML_104
                color: "#A7F0BA"
            }
            QTextButton {
                visible: modelData.user_key === null
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                width: label.paintedWidth + 2*20
                height: 36
                type: eTypeB
                label.text: STR.STR_QML_342
                label.font.pixelSize: 16
                onButtonClicked: {
                    backupClicked()
                }
            }
        }
    }
    Component {
        id: hardwareAdd
        QDashRectangle {
            anchors.fill: parent
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
                    width: 36
                    height: 36
                    iconSize: 24
                    icon: "qrc:/Images/Images/Device_Icons/key-dark.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                QLato {
                    width: 150
                    text: STR.STR_QML_954.arg(modelData.key_index + 1)
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
            QTextButton {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                width: label.paintedWidth + 2*20
                height: 36
                type: eTypeB
                label.text: STR.STR_QML_941
                label.font.pixelSize: 16
                onButtonClicked: {
                    hardwareClicked()
                }
            }
        }
    }
    Component {
        id: hardwareAdded
        Rectangle {
            anchors.fill: parent
            color: "#A7F0BA"
            radius: 8
            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                QCircleIcon {
                    bgSize: 36
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
                            visible: modelData.signer_type !== NUNCHUCKTYPE.SERVER
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
                            visible: (modelData.account_index > 0) && (modelData.signer_type !== NUNCHUCKTYPE.SERVER)
                            radius: 8
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
                        text: "XFP: " + modelData.xfp
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.capitalization: Font.AllUppercase
                        font.pixelSize: 12
                    }
                }
            }
            QBadge {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                width: 75
                height: 24
                iconSize: 24
                icon: "qrc:/Images/Images/check-circle-dark.svg"
                text: STR.STR_QML_104
                color: "#A7F0BA"
            }
        }
    }
    Component {
        id: serverAdd
        QDashRectangle {
            anchors.fill: parent
            radius: 8
            borderWitdh: 2
            borderColor: "#031F2B"
            enabled: !isKeyHolderLimited
            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                QBadge {
                    width: 36
                    height: 36
                    iconSize: 24
                    icon: "qrc:/Images/Images/Device_Icons/server-key-dark.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                QLato {
                    width: 150
                    text: STR.STR_QML_957
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
            QTextButton {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                width: label.paintedWidth + 2*20
                height: 36
                type: eTypeB
                label.text: STR.STR_QML_958
                label.font.pixelSize: 16
                onButtonClicked: {
                    serkeyClicked()
                }
            }
        }
    }
    Component {
        id: serverAdded
        Rectangle {
            anchors.fill: parent
            color: "#A7F0BA"
            radius: 8
            enabled: !isKeyHolderLimited
            Row {
                anchors {
                    fill: parent
                    margins: 12
                }
                spacing: 12
                QBadge {
                    width: 36
                    height: 36
                    iconSize: 24
                    icon: "qrc:/Images/Images/Device_Icons/server-key-dark.svg"
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#F5F5F5"
                }
                QLato {
                    width: 150
                    text: modelData.name
                    anchors.verticalCenter: parent.verticalCenter
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 16
                }
            }
            QBadge {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 0
                }
                height: 24
                iconSize: 24
                icon: "qrc:/Images/Images/check-circle-dark.svg"
                text: STR.STR_QML_959
                color: "#A7F0BA"
            }
        }
    }
}
