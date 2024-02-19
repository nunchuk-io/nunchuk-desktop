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
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Chats"
import "./../../../customizes/Texts"
import "./../../../customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Item {
    property var inheritanceInfo: ServiceSetting.servicesTag.inheritanceInfo
    Component.onCompleted: {
        _claim.claimClear()
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            suggestionList.enterKeyRequest()
        }
    }
    Column {
        id: _claim
        anchors.fill: parent
        spacing: 24
        anchors{
            left: parent.left
            leftMargin: 24
            top: parent.top
            topMargin: 24
        }
        function claimClear() {
            magicPhrase.textInputted = ""
            backupPassword.textInputted = ""
        }
        Rectangle {
            width: parent.width - 24
            height: parent.height * 0.42
            radius: 24
            color: "#D0E2FF"
            QImage {
                anchors.centerIn: parent
                width: sourceSize.width
                height: sourceSize.height
                source: "qrc:/Images/Images/claim-inheritance-illustration.svg"
            }
        }
        Item {
            width: parent.width - 24
            height: parent.height * 0.58 - 24
            Column {
                id: _header
                width: parent.width
                spacing: 16
                QText {
                    text: STR.STR_QML_747
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.pixelSize: 32
                    font.weight: Font.DemiBold
                }
                QText {
                    text: STR.STR_QML_748
                    color: "#031F2B"
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Normal
                }
            }
            QTextInputBoxTypeB {
                id: magicPhrase
                anchors{
                    top: _header.bottom
                    topMargin: 16
                }
                z: backupPassword.z + 1
                label: STR.STR_QML_749
                boxWidth: 537
                boxHeight: 48
                isValid: true
                onTextInputtedChanged: {
                    if(!magicPhrase.isValid){
                        magicPhrase.isValid = true
                        magicPhrase.errorText = ""
                    }
                    magicPhrase.showError = false;

                    var targetY = magicPhrase.y + magicPhrase.height
                    var targetX = magicPhrase.x
                    if(textBoxFocus){
                        sglst.currentIndex = 0
                        _claim.searchingText(magicPhrase.textInputted)
                    }
                }
                onTextBoxFocusChanged: {
                    if(textBoxFocus){
                        sglst.currentIndex = 0
                        suggestionList.y = magicPhrase.y + magicPhrase.height
                        suggestionList.x = magicPhrase.x
                        _claim.searchingText(magicPhrase.textInputted)
                    }
                    else{
                        sglst.currentIndex = -1
                    }
                }
                onDownKeyRequest: {suggestionList.downKeyRequest()  }
                onUpKeyRequest:   {suggestionList.upKeyRequest()    }
                onEnterKeyRequest:{suggestionList.enterKeyRequest() }
                onTabKeyRequest: { suggestionList.enterKeyRequest() }

                Item {
                    id: suggestionList
                    width: suggestionList.visible ? magicPhrase.width : 0
                    height: suggestionList.visible ? Math.min(sglst.contentHeight + 10, 250): 0
                    visible: sglst.currentIndex !== -1
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {}
                    }

                    function downKeyRequest() {sglst.downKeyRequest() }
                    function upKeyRequest()   {sglst.upKeyRequest()   }
                    function enterKeyRequest(){sglst.enterKeyRequest()}

                    Rectangle {
                        id: bg
                        anchors {
                            fill: parent
                            leftMargin: 5
                            rightMargin: 5
                            topMargin: 5
                            bottomMargin: 5
                        }
                        radius: 4
                        color: "#FFFFFF"
                        visible: false
                    }

                    DropShadow {
                        anchors.fill: bg
                        verticalOffset: 3
                        cached: true
                        radius: 16
                        samples: 24
                        color: Qt.rgba(0, 0, 0, 0.4)
                        source: bg
                        ListView {
                            id: sglst
                            anchors.centerIn: parent
                            width: parent.width - 10
                            height: suggestionList.visible ? Math.min(sglst.contentHeight, 240): 0
                            model: modelSearch
                            clip: true
                            ScrollBar.vertical: ScrollBar { active: true }
                            delegate: Rectangle {
                                width: sglst.width
                                property bool shown: textsuggest.visible
                                height: 240/10
                                color: suggestMouse.containsMouse ? "#F1FAFE" : "#FFFFFF"
                                QText {
                                    id: textsuggest
                                    anchors.left: parent.left
                                    text: modelData
                                    color: suggestMouse.pressed ?"#031F2B" :  (suggestMouse.containsMouse || sglst.currentIndex === index) ? "#35ABEE" : "#031F2B"
                                    font.pixelSize: 16
                                    font.weight: (suggestMouse.containsMouse || sglst.currentIndex === index) ? Font.ExtraBold : Font.Normal
                                }

                                MouseArea {
                                    id: suggestMouse
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: {sglst.mouseKeyRequest(index)}
                                }
                            }

                            function downKeyRequest() { if(sglst.currentIndex < sglst.count - 1) {sglst.currentIndex += 1}; }
                            function upKeyRequest() {if(sglst.currentIndex > 0) {sglst.currentIndex -= 1};}
                            function enterKeyRequest() {
                                if (sglst.currentIndex !== -1) {
                                    var splitString = magicPhrase.textInputted.split(" ");
                                    splitString[splitString.length - 1] = modelSearch.get(sglst.currentIndex).mnemonic;
                                    magicPhrase.textInputted = splitString.toString(" ");
                                    sglst.currentIndex = -1;
                                }
                            }
                            function mouseKeyRequest(index) {
                                if(index !== -1) {
                                    var splitString = magicPhrase.textInputted.split(" ");
                                    splitString[splitString.length - 1] = modelSearch.get(index).mnemonic;
                                    magicPhrase.textInputted = splitString.toString(" ");
                                }
                                sglst.currentIndex = -1;
                            }
                        }
                    }
                }
            }
            Column {
                anchors{
                    top: _header.bottom
                    topMargin: 16 + 72 + 16
                }
                spacing: 16
                QTextInputBoxTypeB {
                    id: backupPassword
                    label: STR.STR_QML_727
                    boxWidth: 537
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!backupPassword.isValid){
                            backupPassword.isValid = true
                            backupPassword.errorText = ""
                        }
                        backupPassword.showError = false;
                    }
                }

                QTextInputBoxTypeB {
                    id: backupPassword_two
                    label: STR.STR_QML_727_optinal
                    boxWidth: 537
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!backupPassword_two.isValid){
                            backupPassword_two.isValid = true
                            backupPassword_two.errorText = ""
                        }
                        backupPassword_two.showError = false;
                    }
                }
            }

            QTextButton {
                width: 98
                height: 48
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    bottomMargin: 16
                }
                label.text: STR.STR_QML_265
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
//                    suggestionList.enterKeyRequest()
                    var input = {
                        "magic":magicPhrase.textInputted,
                        "backupPassword":backupPassword.textInputted,
                        "backupPasswordTwo":backupPassword_two.textInputted
                    }
                    QMLHandle.signalNotifySendEvent(EVT.EVT_CLAIM_INHERITANCE_CHECK_REQUEST,input)
                }
            }
        }
        function searchingText(str){
            // SPIT TEXT
            const words = str.split(" ");
            const lastIdx = words.length - 1;
            const lastWord = words[lastIdx];

            modelSearch.clear()
            for(var i = 0; i < AppModel.suggestMnemonics.length; i++){
                if(str === ""){
                    var data = {'mnemonic': AppModel.suggestMnemonics[i]};
                    modelSearch.append(data)
                }
                else{
                    var currentText = AppModel.suggestMnemonics[i].toLowerCase().substring(0, lastWord.length)
                    if (currentText.toLowerCase() === lastWord.toLowerCase()){
                        var datafilter = {'mnemonic': AppModel.suggestMnemonics[i]};
                        modelSearch.append(datafilter)
                    }
                }
            }
            sglst.currentIndex = 0
            return true
        }
        ListModel {
            id: modelSearch
        }
    }
}
