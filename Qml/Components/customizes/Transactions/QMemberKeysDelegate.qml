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

Rectangle {
    id: roomRoot
    width: 326
    height: columnItem.height
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
    property string card_id: ""
    property string tag: ""
    property bool   has_sign_btn: true
    property bool   is_cosigning: false
    property string myRole: ""
    property bool   isValueKey: false
    property bool   enableValuekeyset: AppModel.walletInfo.enableValuekeyset
    property string platformKeyMessage: ""
    property var signerEvents
    function menuClicked(action, data) {
        if (signerEvents[action]) {
            signerEvents[action](data);
        }
    }

    Column {
        id: columnItem
        spacing: 4
        width: parent.width
        Row {
            id: roomDelegateContent
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
                    device_type: roomRoot.devicetype
                    type: roomRoot.signerType
                    tag: roomRoot.tag
                }
            }

            Column {
                width: 170
                spacing: 4
                anchors.verticalCenter: parent.verticalCenter
                QText{
                    width: parent.width
                    height: 28
                    font.family: "Lato"
                    text: (signerType === NUNCHUCKTYPE.PLATFORM) ? "Platform key" : signername
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
                        visible: isLocaluser && (signerType !== NUNCHUCKTYPE.SERVER) && (signerType !== NUNCHUCKTYPE.PLATFORM)
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
                        visible: (accountIndex > 0) && (signerType !== NUNCHUCKTYPE.SERVER) && (signerType !== NUNCHUCKTYPE.PLATFORM)
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
                    visible: (signerxfp !== "" || card_id !== "") && (signerType !== NUNCHUCKTYPE.SERVER) && (signerType !== NUNCHUCKTYPE.PLATFORM)
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
                    visible: (is_cosigning || serverkeyMessage !== "") && (signerType === NUNCHUCKTYPE.SERVER)
                    font.family: "Lato"
                    font.pixelSize: 12
                    color: "#A66800"
                    text:  is_cosigning ? STR.STR_QML_1002 : serverkeyMessage
                }
                QText {
                    height: 16
                    visible: (text !== "") && (signerType === NUNCHUCKTYPE.PLATFORM)
                    font.family: "Lato"
                    font.pixelSize: 12
                    color: "#A66800"
                    text: platformKeyMessage
                }
            }
            Loader {
                width: 80
                height: parent.height
                visible: myRole !== "OBSERVER"
                sourceComponent: {
                    if(alreadySigned ) return signedComp
                    else{
                        if(tx_status !== NUNCHUCKTYPE.PENDING_SIGNATURES) {
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
                                else if((signerType === NUNCHUCKTYPE.SERVER) || (signerType === NUNCHUCKTYPE.PLATFORM)) {
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
            QIcon {
                iconSize: 16
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
        Item {
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/check_circle_24px_n.png"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
            }
        }
    }
    Component {
        id: requiredSignature
        Item {
            QTextButton {
                width: 57
                height: 36
                label.text: STR.STR_QML_509
                label.font.pixelSize: 12
                label.font.family: "Lato"
                type: eTypeE
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                onButtonClicked: {
                    menuClicked("sign", model)                
                 }
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
                onButtonClicked: { 
                    menuClicked("scan", model)                 
                }
            }
        }
    }
    Component {
        id: keysignOption
        Item {
            QTextButton {
                width: 57
                height: 36
                label.text: STR.STR_QML_509
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
            QMultiContextMenu {
                id: signOptionMenu
                menuWidth: 300
                subMenuWidth: 300
                property var exportMessage: [
                    {
                        visible: true,
                        label: STR.STR_QML_114,
                        icon: "qrc:/Images/Images/ExportFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            signOptionMenu.close()
                            menuClicked("exportQr", model)
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_1531,
                        icon: "qrc:/Images/Images/ExportFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            signOptionMenu.close()
                            menuClicked("exportBBQR", model)
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_2097,
                        icon: "qrc:/Images/Images/ExportFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            signOptionMenu.close()
                            menuClicked("exportFile", model)
                        }
                    }
                ]                 
                property var importMessage: [
                    {
                        visible: true,
                        label: STR.STR_QML_302,
                        icon: "qrc:/Images/Images/importFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            signOptionMenu.close()
                            menuClicked("importQr", model)
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_677,
                        icon: "qrc:/Images/Images/importFile.svg",
                        iconRight: "",
                        color: "#031F2B",
                        enable: true,
                        subMenu: null,
                        action: function() {
                            signOptionMenu.close()
                            menuClicked("importFile", model)
                        }
                    }
                ]
                mapMenu: [
                    {
                        visible: true,
                        label: STR.STR_QML_294,
                        icon: "qrc:/Images/Images/ExportFile.svg",
                        iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                        color: "#031F2B",
                        enable: true,
                        subMenu: exportMessage,
                        action: function() {
                        }
                    },
                    {
                        visible: true,
                        label: STR.STR_QML_252,
                        icon: "qrc:/Images/Images/importFile.svg",
                        iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                        color: "#031F2B",
                        enable: true,
                        subMenu: importMessage,
                        action: function() {
                        }
                    }
                ]
            }
        }
    }
}
