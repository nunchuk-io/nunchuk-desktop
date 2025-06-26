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
import "../../customizes"
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../customizes/Signers"
import "../../customizes/Chats"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    id: keyroot
    implicitWidth: 326
    implicitHeight: visible ? columnItem.height : 0
    color: "transparent"
    property string serverkeyMessage: ""
    property string devicetype: ""
    property string signername: "A B"
    property string signerxfp : "A B"
    property bool   isLocaluser: true
    property string sectionname: ""
    property bool   alreadySigned: false
    property bool   signerReadyToSign: true
    property int    signerType: -1
    property int    tx_status: -1
    property int    accountIndex: 0
    property int    keysetIndex: 0
    property int    keysetStatus: 0
    property int    keysetRemaining: 0
    property int    keysetCount: 1
    property int    keysetM: 1
    property string card_id: ""
    property string tag: ""
    property bool   has_sign_btn: true
    property bool   is_cosigning: false
    property string myRole: ""
    property bool   isValueKey: false
    property bool   enableValuekeyset: AppModel.walletInfo.enableValuekeyset

    signal signRequest()
    signal scanRequest()
    signal exportRequest()
    signal importRequest()

    Column {
        id: columnItem
        width: 326
        spacing: 12
        Loader {
            sourceComponent: titleOtherKeysets
            visible: (index == keysetM) && (transactionInfo.keysetsCount > 1)
        }
        Loader {
            visible: {
                if(sourceComponent === null) { return false;}
                else{
                    if(index < keysetM) {return true;}
                    else { return GlobalData.showOthersKeyset;}
                }
            }
            sourceComponent: {
                if(enableValuekeyset){
                    if(index % keysetM === 0){
                        if(keysetIndex == 0){
                            return valueKeyset
                        }
                        else {
                            return otherKeyset
                        }
                    }
                    else {
                        return null;
                    }
                }
                else {
                    if(index % keysetM === 0){
                        return otherKeyset
                    }
                    else {
                        return null;
                    }
                }
            }
        }
        Column {
            spacing: 4
            width: parent.width
            visible: (index < keysetM) ? true : GlobalData.showOthersKeyset
            Row {
                spacing: 8
                Rectangle {
                    id: _keyIcon
                    width: 48
                    height: 48
                    radius: width
                    color: "#F5F5F5"
                    anchors.verticalCenter: parent.verticalCenter
                    QSignerDarkIcon {
                        iconSize: 24
                        anchors.centerIn: parent
                        device_type: keyroot.devicetype
                        type: keyroot.signerType
                        tag: keyroot.tag
                    }
                }

                Column {
                    width: 160
                    spacing: 4
                    anchors.verticalCenter: parent.verticalCenter
                    QText{
                        width: parent.width
                        height: 28
                        font.family: "Lato"
                        text: signername
                        font.pixelSize: 16
                        color: "#031F2B"
                        elide: Text.ElideRight
                    }
                    Row {
                        spacing: 4
                        Rectangle {
                            width: valuekeytext.width + 10
                            height: 16
                            color: "#D0E2FF"
                            visible: isValueKey && enableValuekeyset
                            radius: 8
                            QText {
                                id: valuekeytext
                                font.family: "Lato"
                                color: "#031F2B"
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                font.weight: Font.Bold
                                text: "Value Key"
                            }
                        }
                        Rectangle {
                            width: typesigner.width + 10
                            height: 16
                            color: "#EAEAEA"
                            visible: isLocaluser && (signerType !== NUNCHUCKTYPE.SERVER)
                            radius: 8
                            QText {
                                id: typesigner
                                font.family: "Lato"
                                color: "#031F2B"
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                font.weight: Font.Bold
                                text: GlobalData.signers(signerType)
                            }
                        }
                        Rectangle {
                            width: accttext.width + 10
                            height: 16
                            color: "#EAEAEA"
                            visible: (accountIndex > 0) && (signerType !== NUNCHUCKTYPE.SERVER)
                            radius: 8
                            QText {
                                id: accttext
                                font.family: "Lato"
                                color: "#031F2B"
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                font.weight: Font.Bold
                                text: qsTr("Acct %1").arg(accountIndex)
                            }
                        }
                    }
                    QText {
                        height: 16
                        visible: (signerxfp !== "" || card_id !== "") && signerType !== NUNCHUCKTYPE.SERVER
                        font.family: "Lato"
                        font.pixelSize: 12
                        color: "#031F2B"
                        text: {
                            if (signerType === NUNCHUCKTYPE.NFC) {
                                var textR = card_id.substring(card_id.length - 5,card_id.length).toUpperCase()
                                return "Card ID: ••" + textR
                            } else {
                                return "XFP: " + signerxfp.toUpperCase()
                            }
                        }
                    }
                    QText {
                        height: 16
                        visible: (is_cosigning || serverkeyMessage !== "") && signerType === NUNCHUCKTYPE.SERVER
                        font.family: "Lato"
                        font.pixelSize: 12
                        color: "#A66800"
                        text:  is_cosigning ? STR.STR_QML_1002 : serverkeyMessage
                    }
                }
                Loader {
                    width: 90
                    height: parent.height
                    visible: myRole !== "OBSERVER"
                    sourceComponent: {
                        if(alreadySigned) return signedComp
                        else{
                            if(tx_status !== NUNCHUCKTYPE.PENDING_SIGNATURES && tx_status !== NUNCHUCKTYPE.PENDING_NONCE && tx_status !== NUNCHUCKTYPE.READY_TO_BROADCAST) {
                                return null;
                            }
                            else{
                                if(signerType === NUNCHUCKTYPE.FOREIGN_SOFTWARE || signerType === NUNCHUCKTYPE.NFC){
                                    return isDummy ? null : helpComp;
                                }
                                if(isLocaluser || isDummy){
                                    if(signerType === NUNCHUCKTYPE.AIRGAP || signerType === NUNCHUCKTYPE.UNKNOWN ) {
                                        return has_sign_btn ? keysignOption : null;
                                    }
                                    else if(signerType === NUNCHUCKTYPE.SERVER) {
                                        return null;
                                    }
                                    else {
                                        if(signerReadyToSign){
                                            return has_sign_btn ? requiredSignature : null;
                                        }
                                        else {
                                            return has_sign_btn ? requiredScan : null;
                                        }
                                    }
                                }
                                else{
                                    return null;
                                }
                            }
                        }
                    }
                }
            }
            Row {
                visible: (tx_status !== NUNCHUCKTYPE.PENDING_SIGNATURES) ?
                             false : !alreadySigned && (signerType === NUNCHUCKTYPE.HARDWARE || signerType === NUNCHUCKTYPE.COLDCARD_NFC)
                height: 16
                spacing: 4
                QImage {
                    width: 16
                    height: 16
                    source: signerReadyToSign ? "qrc:/Images/Images/check_circle_24px_n.png" : "qrc:/Images/Images/warning_amber-24px 2.png"
                    anchors.verticalCenter: parent.verticalCenter
                }
                QText {
                    font.family: "Lato"
                    color: signerReadyToSign ? "#031F2B" : "#A66800"
                    font.pixelSize: 12
                    text: signerReadyToSign ? STR.STR_QML_983 : STR.STR_QML_506
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }

    Component {
        id: helpComp
        Item {
            QTooltip {
                width: 24
                height: 24
                tipWidth: 270
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                source: "qrc:/Images/Images/help_outline_24px.svg"
                toolTip: {
                    switch(signerType){
                    case NUNCHUCKTYPE.AIRGAP: return STR.STR_QML_507
                    case NUNCHUCKTYPE.NFC: return STR.STR_QML_688
                    default: return STR.STR_QML_508
                    }
                }
                rightOfParent: true
            }
        }
    }
    Component {
        id: signedComp
        Row {
            spacing: 4
            layoutDirection: Qt.RightToLeft
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/check_circle_24px_n.png"
                anchors.verticalCenter: parent.verticalCenter
            }
            QLato {
                anchors.verticalCenter: parent.verticalCenter
                text: (keyroot.keysetStatus === NUNCHUCKTYPE.PENDING_NONCE) ? "Committed" : "Signed"
                font.pixelSize: 12
                color: "#1C1C1C"
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
    Component {
        id: requiredSignature
        Item {
            QTextButton {
                width: 57
                height: 36
                label.text: (keyroot.keysetStatus === NUNCHUCKTYPE.PENDING_NONCE) ? "Commit" : STR.STR_QML_509
                label.font.pixelSize: 12
                label.font.family: "Lato"
                type: eTypeE
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                onButtonClicked: { signRequest() }
            }
        }
    }
    Component {
        id: requiredScan
        Item {
            QRefreshButton {
                width: 70
                height: 36
                iconSize: 18
                iconSpacing: 4
                label: STR.STR_QML_510
                fontPixelSize: 12
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                onButtonClicked: { scanRequest() }
            }
        }
    }
    Component {
        id: keysignOption
        Item {
            QTextButton {
                width: 57
                height: 36
                label.text: (keyroot.keysetStatus === NUNCHUCKTYPE.PENDING_NONCE) ? "Commit" : STR.STR_QML_509
                label.font.pixelSize: 12
                label.font.family: "Lato"
                type: eTypeE
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                onButtonClicked: {
                    signOptionMenu.x = 20
                    signOptionMenu.y = 20 - signOptionMenu.height
                    signOptionMenu.open()
                }
            }
            QContextMenu {
                id: signOptionMenu
                menuWidth: 250
                labels: [
                    STR.STR_QML_294,
                    STR.STR_QML_252,
                ]
                icons: [
                    "qrc:/Images/Images/ExportFile.svg",
                    "qrc:/Images/Images/importFile.svg",
                ]
                onItemClicked: {
                    switch(index){
                    case 0: // Export transaction
                        exportRequest()
                        break;
                    case 1: // Import signature
                        importRequest()
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    Component {
        id: valueKeyset
        Item {
            id: valueKeysetItem
            property int keyset_index: keysetIndex
            property int keyset_status: keysetStatus
            property int keyset_pending_number: keysetRemaining
            implicitWidth: 326
            implicitHeight: 36
            Rectangle {
                width: 326
                height: 1
                color: "#DEDEDE"
                visible: index !== keysetM && index !== 0
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Row {
                id: valueKeysetLayout
                anchors.centerIn: parent
                spacing: 8
                QImage {
                    width: 16
                    height: 16
                    source: "qrc:/Images/Images/ValueKeyset.png"
                }
                Column {
                    spacing: 4
                    Row {
                        spacing: 4
                        QLato {
                            width: 76
                            height: 16
                            text: "Value Keyset"
                            font.pixelSize: 12
                            color: "#031F2B"
                            font.weight: Font.Bold
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                        Item {
                            width: 16
                            height: 16
                            QImage {
                                width: 16
                                height: 16
                                visible: (valueKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_NONCE) || (valueKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                                source: "qrc:/Images/Images/PendingSignatures.png"
                            }
                        }
                        Item {
                            width: 110
                            height: 16
                            QLato {
                                anchors.fill: parent
                                text: qsTr("Pending %1 %2%3").arg(valueKeysetItem.keyset_pending_number)
                                                             .arg(valueKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_NONCE ? "nonce" : "signature")
                                                             .arg(valueKeysetItem.keyset_pending_number > 1 ? "s" : "")
                                font.pixelSize: 12
                                color: "#757575"
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                visible: (valueKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_NONCE) || (valueKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                            }
                        }
                        Rectangle {
                            width: 76
                            height: 16
                            radius: 20
                            color: {
                                if (valueKeysetItem.keyset_status === NUNCHUCKTYPE.READY_TO_BROADCAST) {return "#1C652D"}
                                else if (valueKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_SIGNATURES) {return "#FFCB2E"}
                                else {return "#FDEBD2"}
                            }
                            QLato {
                                anchors.centerIn: parent
                                text: {
                                    if (valueKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_NONCE) {return "Round 1/2"}
                                    else if (valueKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_SIGNATURES) {return "Round 2/2"}
                                    else {return "Completed"}
                                }
                                font.pixelSize: 12
                                color: valueKeysetItem.keyset_status === NUNCHUCKTYPE.READY_TO_BROADCAST ? "#FFFFFF" : "#1C1C1C"
                                font.weight: Font.Bold
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                    QLato {
                        height: 16
                        text: "Better privacy and lower fees"
                        font.pixelSize: 12
                        color: "#757575"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
    Component {
        id: otherKeyset
        Item {
            id: otherKeysetItem
            property int keyset_index: keysetIndex
            property int keyset_status: keysetStatus
            property int keyset_pending_number: keysetRemaining
            width: 326
            height: 36
            Rectangle {
                width: 326
                height: 1
                color: "#DEDEDE"
                visible: index !== keysetM && index !== 0
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Column {
                id: otherKeysetItemLayout
                anchors.verticalCenter: parent.verticalCenter
                Item {
                    width: 326
                    height: 16
                    Row {
                        spacing: 4
                        QLato {
                            width: 60
                            height: 16
                            text: qsTr("Keyset %1").arg(otherKeysetItem.keyset_index+1)
                            font.pixelSize: 12
                            color: "#031F2B"
                            font.weight: Font.Bold
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                        Item {
                            width: 16
                            height: 16
                            QImage {
                                width: 16
                                height: 16
                                visible: (otherKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_NONCE) || (otherKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                                source: "qrc:/Images/Images/PendingSignatures.png"
                            }
                        }
                        Item {
                            width: 155
                            height: 16
                            QLato {
                                anchors.fill: parent
                                text: qsTr("Pending %1 %2%3").arg(otherKeysetItem.keyset_pending_number)
                                                             .arg(otherKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_NONCE ? "nonce" : "signature")
                                                             .arg(otherKeysetItem.keyset_pending_number > 1 ? "s" : "")
                                font.pixelSize: 12
                                color: "#757575"
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                visible: (otherKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_NONCE) || (otherKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                            }
                        }
                        Rectangle {
                            width: 76
                            height: 16
                            radius: 20
                            color: {
                                if (otherKeysetItem.keyset_status === NUNCHUCKTYPE.READY_TO_BROADCAST) {return "#1C652D"}
                                else if (otherKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_SIGNATURES) {return "#FFCB2E"}
                                else {return "#FDEBD2"}
                            }
                            QLato {
                                anchors.centerIn: parent
                                text: {
                                    if (otherKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_NONCE) {return "Round 1/2"}
                                    else if (otherKeysetItem.keyset_status === NUNCHUCKTYPE.PENDING_SIGNATURES) {return "Round 2/2"}
                                    else {return "Completed"}
                                }
                                font.pixelSize: 12
                                color: otherKeysetItem.keyset_status === NUNCHUCKTYPE.READY_TO_BROADCAST ? "#FFFFFF" : "#1C1C1C"
                                font.weight: Font.Bold
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                }
            }
        }
    }
    Component {
        id: titleOtherKeysets
        Item {
            id: titleOtherKeysetsItem
            property int keyset_index: keysetIndex
            property int keyset_status: keysetStatus
            property int keyset_pending_number: keysetRemaining
            width: 326
            height: 36
            Rectangle {
                width: 354
                height: 1
                color: "#DEDEDE"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Row {
                spacing: 8
                height: 52
                QImage {
                    width: 20
                    height: 20
                    source: "qrc:/Images/Images/OtherKeyset.png"
                    anchors.verticalCenter: parent.verticalCenter
                }
                QLato {
                    width: 81
                    height: 16
                    text: "Other Keyset"
                    font.pixelSize: 12
                    color: "#031F2B"
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                Rectangle {
                    width: 22
                    height: 16
                    radius: 20
                    color: "#DEDEDE"
                    anchors.verticalCenter: parent.verticalCenter

                    QLato {
                        text: (transactionInfo.keysetsCount -1)
                        font.pixelSize: 12
                        color: "#031F2B"
                        font.weight: Font.Bold
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.centerIn: parent
                    }
                }
                Item {
                    width: 115
                    height: 16
                    anchors.verticalCenter: parent.verticalCenter
                }
                Item {
                    width: 48
                    height: 16
                    anchors.verticalCenter: parent.verticalCenter
                    Row {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        spacing: 4
                        QLato {
                            text: GlobalData.showOthersKeyset ? "Hide" : "View all"
                            font.pixelSize: 12
                            color: "#031F2B"
                            font.weight: Font.Bold
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                            font.underline: true
                        }
                        QImage {
                            width: 16
                            height: 16
                            source: GlobalData.showOthersKeyset ? "qrc:/Images/Images/expand_less_24px.png" : "qrc:/Images/Images/expand_more_24px.png"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            GlobalData.showOthersKeyset = !GlobalData.showOthersKeyset
                        }
                    }
                }
            }
        }
    }
}
