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

    property var  key_index: modelData.wallet_type === "MULTI_SIG" ? (modelData.key_index + 1) : modelData.key_index
    property var  walletType: modelData.wallet_type
    property int  slot_index: index + 1
    property bool isBeforeSlot: modelData.has && !modelData.hasSecond
    Loader {
        id: _source
        anchors.fill: parent
        sourceComponent: component()
    }

    function component() {
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

    FastBlur {
        anchors.fill: _source
        source: _source
        radius: 32
        visible: modelData.type === "SERVER" ? isKeyHolderLimited : (isKeyHolderLimited && modelData.has && !modelData.ourAccount)
    }

    Component {
        id: backupButton
        QTextButton {
            width: label.paintedWidth + 2*16
            height: 36
            type: eTypeB
            label.text: modelData.wallet_type === "MULTI_SIG" ? STR.STR_QML_342 : STR.STR_QML_1964
            label.font.pixelSize: 16
            onButtonClicked: {
                backupClicked()
            }
        }
    }
    Component {
        id: addButton
        QTextButton {
            width: label.paintedWidth + 2*16
            height: 36
            type: eTypeB
            label.text: STR.STR_QML_941
            label.font.pixelSize: 16
            onButtonClicked: {
                if (modelData.is_inheritance) {
                    tapsignerClicked()
                } else {
                    hardwareClicked()
                }
            }
        }
    }
    Component {
        id: addedCheck
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

    function inheritance(add, backup, added) {
        if (modelData.wallet_type === "MULTI_SIG") {
            return modelData.user_key === null ? backup : added
        }
        var needVerifyBackup = modelData.verification_type === "NONE" ? backup : added
        if (isBeforeSlot) {
            return modelData.hasSecond ? needVerifyBackup : add
        }        
        return needVerifyBackup
    }

    function normal(add, added) {
        if (modelData.wallet_type === "MULTI_SIG") {
            return added
        }
        if (isBeforeSlot) {
            return modelData.hasSecond ? added : add
        }
        return added
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
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4
                    QLato {
                        width: 150
                        height: 28
                        text: STR.STR_QML_954.arg(slot_index)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Row {
                        spacing: 4
                        QBadge {
                            width: 77
                            height: 16
                            fontSize: 10
                            text: STR.STR_QML_1600
                            color: "#EAEAEA"
                        }
                        QAccountIndexs {
                            height: 16
                            visible: modelData.signer_type !== NUNCHUCKTYPE.SERVER
                            accountIndexs: modelData.account_indexs
                            walletType: modelData.wallet_type
                        }
                    }
                }
            }
            Loader {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                sourceComponent: addButton
            }
        }
    }
    Component {
        id: inheritanceAdded
        QDashRectangle {
            anchors.fill: parent
            color: inheritance("#66A7F0BA", "#FDEBD2", "#A7F0BA")
            isDashed: inheritance(true, false, false)
            radius: 8
            borderWitdh: isDashed ? 2 : 0
            borderColor: "#031F2B"
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
                        QAccountIndexs {
                            height: 16
                            visible: (modelData.signer_type !== NUNCHUCKTYPE.SERVER) && (modelData.account_indexs.length > 0)
                            accountIndexs: modelData.account_indexs
                            walletType: modelData.wallet_type
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
                            visible: modelData.card_id === ""
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
            Loader {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                sourceComponent: inheritance(addButton, backupButton, addedCheck)
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
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4
                    QLato {
                        width: 150
                        text: STR.STR_QML_954.arg(slot_index)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QAccountIndexs {
                        height: 16
                        visible: modelData.signer_type !== NUNCHUCKTYPE.SERVER
                        accountIndexs: modelData.account_indexs
                        walletType: modelData.wallet_type
                    }
                }
            }
            Loader {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                sourceComponent: addButton
            }
        }
    }
    Component {
        id: hardwareAdded
        QDashRectangle {
            anchors.fill: parent
            color: normal("#66A7F0BA", "#A7F0BA")
            isDashed: normal(true, false)
            radius: 8
            borderWitdh: isDashed ? 2 : 0
            borderColor: "#031F2B"
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
                        QAccountIndexs {
                            height: 16
                            visible: (modelData.signer_type !== NUNCHUCKTYPE.SERVER) && (modelData.account_indexs.length > 0)
                            accountIndexs: modelData.account_indexs
                            walletType: modelData.wallet_type
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
            Loader {
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
                sourceComponent: normal(addButton, addedCheck)
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
