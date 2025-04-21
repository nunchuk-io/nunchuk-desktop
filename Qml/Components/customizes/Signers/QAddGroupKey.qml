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
    property alias signerData: dataSingle
    QSingleSignerData {
        id: dataSingle
    }

    signal hardwareClicked()
    signal removeClicked(var idx)
    signal recoverKeyClick(var idx, var xfp)
    signal bip32PathClick(var xfp, var path)
    sourceComponent: if (newWalletInfo.isReplaceGroupWallet) {
                         if (dataSingle.single_name === "" || dataSingle.single_name === "ADDED") {
                            return replaceSignerAdd
                         } else {
                             return dataSingle.single_is_local ? replaceSignerAdded : replaceNotMeAdded
                         }
                     } else {
                         return dataSingle.single_masterFingerPrint !== ""
                                 ? (dataSingle.single_is_local ? signerAdded : notMeAdded)
                                 : signerAdd
                     }

    function isAdded() {
        return dataSingle.single_masterFingerPrint === "" && dataSingle.single_name === "ADDED"
    }

    Component {
        id: signerAdd
        QDashRectangle {
            width: 352
            height: Math.max(maxColumn.childrenRect.height + 12*2, 72)
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
                    id: maxColumn
                    width: 150
                    spacing: 4
                    anchors.verticalCenter: parent.verticalCenter
                    QLato {
                        width: 150
                        text: STR.STR_QML_1557.arg(index + 1)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QLato {
                        width: 150
                        text: STR.STR_QML_1665
                        color: "#A66800"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        visible: dataSingle.single_isOccupied
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
                label.text: isAdded() ? STR.STR_QML_104 : STR.STR_QML_941
                label.font.pixelSize: 12
                visible: !isAdded()
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
                visible: isAdded()
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
                    icon.type: dataSingle.single_type
                    icon.tag: dataSingle.single_tag
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
                        text: dataSingle.single_name
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
                                text: GlobalData.signers(dataSingle.single_type)
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
                            visible: dataSingle.single_account_index > 0
                            QText {
                                id: accttext
                                font.family: "Lato"
                                color: "#031F2B"
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                font.weight: Font.Bold
                                text: qsTr("Acct %1").arg(dataSingle.single_account_index)
                            }
                        }
                    }
                    QLato {
                        width: parent.width
                        text: {
                            if (dataSingle.single_device_cardid !== "") {
                                var card_id_text = dataSingle.single_device_cardid
                                var textR = card_id_text.substring(card_id_text.length - 5, card_id_text.length).toUpperCase()
                                return "Card ID: ••" + textR
                            } else {
                                "XFP: " + dataSingle.single_masterFingerPrint
                            }
                        }
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.capitalization: Font.AllUppercase
                        font.pixelSize: 12
                    }

                    QLato {
                        width: paintedWidth + 4 + 12
                        text: qsTr("BIP32 path: %1").arg(dataSingle.single_derivationPath)
                        color: "#757575"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 12
                        font.underline: sandbox.url !== "" && _icon.visible
                        QIcon {
                            id: _icon
                            iconSize: 12
                            source: "qrc:/Images/Images/editBIP32.svg"
                            anchors {
                                verticalCenter: parent.verticalCenter
                                right: parent.right
                            }
                            visible: dataSingle.single_type === NUNCHUCKTYPE.SOFTWARE ||
                                     dataSingle.single_type === NUNCHUCKTYPE.HARDWARE
                        }
                        MouseArea {
                            visible: _icon.visible
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                bip32PathClick(dataSingle.single_masterFingerPrint, dataSingle.single_derivationPath)
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
            property var colorData: tmpColors[index]
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
                        text: STR.STR_QML_1557.arg(index + 1)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Rectangle {
                        width: accttext.width + 10
                        height: 16
                        color: "#EAEAEA"
                        radius: 8
                        visible: dataSingle.single_account_index > 0
                        QText {
                            id: accttext
                            font.family: "Lato"
                            color: "#031F2B"
                            font.pixelSize: 10
                            anchors.centerIn: parent
                            font.weight: Font.Bold
                            text: qsTr("Acct %1").arg(dataSingle.single_account_index)
                        }
                    }
                    QLato {
                        width: parent.width
                        text: {
                            if (dataSingle.single_device_cardid !== "") {
                                var card_id_text = dataSingle.single_device_cardid
                                var textR = card_id_text.substring(card_id_text.length - 5, card_id_text.length).toUpperCase()
                                return "Card ID: ••" + textR
                            } else {
                                "XFP: " + dataSingle.single_masterFingerPrint
                            }
                        }
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.capitalization: Font.AllUppercase
                        font.pixelSize: 12
                    }
                    QLato {
                        width: parent.width
                        text: qsTr("BIP32 path: %1").arg(dataSingle.single_derivationPath)
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
                    recoverKeyClick(index, dataSingle.single_masterFingerPrint)
                }
            }
        }
    }
    Component {
        id: replaceSignerAdd
        QDashRectangle {
            width: 352
            height: maxColumn.childrenRect.height + 12*2
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
                    opacity: 0.4
                }
                Column {
                    id: maxColumn
                    width: 150
                    spacing: 4
                    anchors.verticalCenter: parent.verticalCenter
                    QLato {
                        width: 150
                        text: STR.STR_QML_1557.arg(index + 1)
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QLato {
                        width: parent.width
                        text: {
                            if (dataSingle.single_device_cardid !== "") {
                                var card_id_text = dataSingle.single_device_cardid
                                var textR = card_id_text.substring(card_id_text.length - 5, card_id_text.length).toUpperCase()
                                return "Card ID: ••" + textR
                            } else {
                                "XFP: " + dataSingle.single_masterFingerPrint
                            }
                        }
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.capitalization: Font.AllUppercase
                        font.pixelSize: 12
                    }
                    QLato {
                        width: 150
                        text: STR.STR_QML_1705
                        font.weight: Font.Normal
                        font.pixelSize: 12
                        color: "#A66800"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
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
                label.text: STR.STR_QML_1368
                label.font.pixelSize: 12
                onButtonClicked: {
                    hardwareClicked()
                }
            }
        }
    }
    Component {
        id: replaceSignerAdded
        Column {
            width: 352
            spacing: 8
            Rectangle {
                width: 352
                height: maxColumn.childrenRect.height + 12*2
                color: dataSingle.single_isReplaced ? "#A7F0BA" : "#FDEBD2"
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
                        icon.type: dataSingle.single_type
                        icon.tag: dataSingle.single_tag
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
                            text: dataSingle.single_name
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
                                    text: GlobalData.signers(dataSingle.single_type)
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
                                visible: dataSingle.single_account_index > 0
                                QText {
                                    id: accttext
                                    font.family: "Lato"
                                    color: "#031F2B"
                                    font.pixelSize: 10
                                    anchors.centerIn: parent
                                    font.weight: Font.Bold
                                    text: qsTr("Acct %1").arg(dataSingle.single_account_index)
                                }
                            }
                        }
                        QLato {
                            width: parent.width
                            text: {
                                if (dataSingle.single_device_cardid !== "") {
                                    var card_id_text = dataSingle.single_device_cardid
                                    var textR = card_id_text.substring(card_id_text.length - 5, card_id_text.length).toUpperCase()
                                    return "Card ID: ••" + textR
                                } else {
                                    "XFP: " + dataSingle.single_masterFingerPrint
                                }
                            }
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.capitalization: Font.AllUppercase
                            font.pixelSize: 12
                        }

                        QLato {
                            width: paintedWidth + 4 + 12
                            text: qsTr("BIP32 path: %1").arg(dataSingle.single_derivationPath)
                            color: "#757575"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 12
                            font.underline: sandbox.url !== "" && _icon.visible
                            QIcon {
                                id: _icon
                                iconSize: 12
                                source: "qrc:/Images/Images/editBIP32.svg"
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    right: parent.right
                                }
                                visible: dataSingle.single_type === NUNCHUCKTYPE.SOFTWARE ||
                                         dataSingle.single_type === NUNCHUCKTYPE.HARDWARE
                            }
                            MouseArea {
                                visible: _icon.visible
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    bip32PathClick(dataSingle.single_masterFingerPrint, dataSingle.single_derivationPath)
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
                    label.text: dataSingle.single_isReplaced ? STR.STR_QML_1134 : STR.STR_QML_1368
                    label.font.pixelSize: 12
                    onButtonClicked: {
                        if (dataSingle.single_isReplaced) {
                            removeClicked(index)
                        } else {
                            hardwareClicked()
                        }
                    }
                }
            }
            Item {
                height: 16
                width: parent.width
                visible: dataSingle.single_isReplaced
                anchors {
                    left: parent.left
                    leftMargin: 16
                }
                Row {
                    spacing: 4
                    QIcon {
                        iconSize: 16
                        source: "qrc:/Images/Images/replace.svg"
                    }
                    QLato {
                        height: 16
                        text: {
                            if (dataSingle.single_isReplaced === false) {
                                return ""
                            }
                            var keyReplaced = dataSingle.single_keyReplaced
                            var type = keyReplaced.single_signer_type
                            var id = ""
                            if (type === NUNCHUCKTYPE.NFC) {
                                var card_id = keyReplaced.single_signer_device_cardid
                                var textR = card_id.substring(card_id.length - 5,card_id.length).toUpperCase()
                                id = "CARD ID: ••" + textR
                            } else {
                                id = "XFP: " + keyReplaced.singleSigner_masterFingerPrint.toUpperCase()
                            }
                            return STR.STR_QML_1380.arg(keyReplaced.singleSigner_name).arg(id)
                        }
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }
    }
    Component {
        id: replaceNotMeAdded
        Column {
            width: 352
            spacing: 8
            property var colorData: tmpColors[index]
            Rectangle {
                width: 352
                height: maxColumn.childrenRect.height + 12*2
                color: dataSingle.single_isReplaced ? "#A7F0BA" : "#FDEBD2"
                radius: 8
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
                            text: STR.STR_QML_1557.arg(index + 1)
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                        Rectangle {
                            width: accttext.width + 10
                            height: 16
                            color: "#EAEAEA"
                            radius: 8
                            visible: dataSingle.single_account_index > 0
                            QText {
                                id: accttext
                                font.family: "Lato"
                                color: "#031F2B"
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                font.weight: Font.Bold
                                text: qsTr("Acct %1").arg(dataSingle.single_account_index)
                            }
                        }
                        QLato {
                            width: parent.width
                            text: {
                                if (dataSingle.single_device_cardid !== "") {
                                    var card_id_text = dataSingle.single_device_cardid
                                    var textR = card_id_text.substring(card_id_text.length - 5, card_id_text.length).toUpperCase()
                                    return "Card ID: ••" + textR
                                } else {
                                    "XFP: " + dataSingle.single_masterFingerPrint
                                }
                            }
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.capitalization: Font.AllUppercase
                            font.pixelSize: 12
                        }
                        QLato {
                            width: parent.width
                            text: qsTr("BIP32 path: %1").arg(dataSingle.single_derivationPath)
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
                    label.text: STR.STR_QML_1368
                    label.font.pixelSize: 12
                    visible: !dataSingle.single_isReplaced
                    onButtonClicked: {
                        hardwareClicked()
                    }
                }
            }
            Item {
                height: 16
                width: parent.width
                visible: dataSingle.single_isReplaced
                anchors {
                    left: parent.left
                    leftMargin: 16
                }
                Row {
                    spacing: 4
                    QIcon {
                        iconSize: 16
                        source: "qrc:/Images/Images/replace.svg"
                    }
                    QLato {
                        height: 16
                        text: {
                            if (dataSingle.single_isReplaced === false) {
                                return ""
                            }
                            var keyReplaced = dataSingle.single_keyReplaced
                            var type = keyReplaced.single_signer_type
                            var id = ""
                            if (type === NUNCHUCKTYPE.NFC) {
                                var card_id = keyReplaced.single_signer_device_cardid
                                var textR = card_id.substring(card_id.length - 5,card_id.length).toUpperCase()
                                id = "CARD ID: ••" + textR
                            } else {
                                id = "XFP: " + keyReplaced.singleSigner_masterFingerPrint.toUpperCase()
                            }
                            return STR.STR_QML_1380_Other.arg(id)
                        }
                        font.pixelSize: 12
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }
    }
}
