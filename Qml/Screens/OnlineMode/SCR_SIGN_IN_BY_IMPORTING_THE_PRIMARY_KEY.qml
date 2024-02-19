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
import DRACO_CODE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

QScreen {
    readonly property int _RECOVER_KEY: 0
    readonly property int _PASSPHRASE: 1
    readonly property int _KEY_NAME: 2
    property var _SCREENS: [recoverKeyWithSeed,enterYourPassphrase,keyName]
    property var whereIn: _RECOVER_KEY
    property var keyname: ""
    property string passphrase: ""
    Connections{
        target: Draco
        onSignalpkey_signin:{
            loadercontent.item.signalpkey_signin(https_code, error_code, error_msg)
        }
    }
    Loader {
        id: loadercontent
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: _SCREENS[whereIn]
    }

    Component{
        id:recoverKeyWithSeed
        QOnScreenContent {
            id:keyseeds
            label.text: STR.STR_QML_099
            onCloseClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
            }
            QText {
                width: 540
                text: STR.STR_QML_644
                anchors {
                    left: parent.left
                    leftMargin: 36
                    top: parent.top
                    topMargin: 70
                }
                color: "#031F2B"
                font.family: "Lato"
                font.pixelSize: 16
                wrapMode: Text.WordWrap
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                verticalAlignment: Text.AlignVCenter
            }
            Grid {
                id: gridmmonic
                width: 730
                height: 500
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 150
                columns: 6
                spacing: 12
                property bool isFullFill: false
                property string mnemonicstr: ""
                Repeater {
                    id: inputrepeater
                    model: 24
                    QTextInputBoxTypeB {
                        id: inputmnemonic
                        label: " " + ((index+1) > 9 ? (index+1) : "0"+(index+1)) + "."
                        boxWidth: (gridmmonic.width - gridmmonic.spacing*(gridmmonic.columns-1)) / gridmmonic.columns
                        boxHeight: 48
                        onTextInputtedChanged: {
                            var result = 0;
                            var str = ""
                            for(var i = 0; i < inputrepeater.count ; i++){
                                if(inputrepeater.itemAt(i).textInputted !== "") {
                                    if(result === 0){ str = inputrepeater.itemAt(i).textInputted }
                                    else{ str = str + " " + inputrepeater.itemAt(i).textInputted }
                                    result++;
                                }
                            }
                            gridmmonic.mnemonicstr = str
                            gridmmonic.isFullFill = (result === 12||result === 18||result === 24) ? true : false
                            AppModel.mnemonic = ""

                            var targetY = inputmnemonic.y + inputmnemonic.height + gridmmonic.y
                            var targetX = inputmnemonic.x + gridmmonic.x
                            if(textBoxFocus){
                                suggestionList.currentIndexBox = index
                                suggestionList.searchingText(inputmnemonic.textInputted)
                            }
                        }
                        onTextBoxFocusChanged: {
                            if(textBoxFocus){
                                suggestionList.currentIndexBox = index
                                suggestionList.y = inputmnemonic.y + inputmnemonic.height + gridmmonic.y
                                suggestionList.x = inputmnemonic.x + gridmmonic.x
                                suggestionList.searchingText(inputmnemonic.textInputted)
                            }
                            else{
                                if(suggestionList.currentIndexBox === index){
                                    suggestionList.currentIndexBox = -1
                                }
                                suggestionList.searchingText("")
                            }
                        }
                        onDownKeyRequest: {suggestionList.downKeyRequest()  }
                        onUpKeyRequest:   {suggestionList.upKeyRequest()    }
                        onEnterKeyRequest:{suggestionList.enterKeyRequest() }
                        onPasteKeyRequest: { suggestionList.pasteSequence(index) }
                        onTabKeyRequest: { suggestionList.enterKeyRequest() }
                    }
                }
            }
            QButtonTextLink {
                label: STR.STR_QML_035
                displayIcon: false
                anchors {
                    left: parent.left
                    leftMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY)
                }
            }
            QTextButton {
                width: 200
                height: 48
                label.text: STR.STR_QML_097
                label.font.pixelSize: 16
                type: eTypeE
                enabled: gridmmonic.isFullFill
                anchors {
                    right: parent.right
                    rightMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_IMPORT_PRIMARY_KEY_WITH_SEED_REQUEST, gridmmonic.mnemonicstr)
                }
            }
            Item {
                id: suggestionList
                property int currentIndexBox: -1
                width: suggestionList.visible ? (gridmmonic.width - gridmmonic.spacing*(gridmmonic.columns-1)) / gridmmonic.columns : 0
                height: suggestionList.visible ? Math.min(sglst.contentHeight + 10, 250): 0
                visible: suggestionList.currentIndexBox !== -1
                MouseArea {
                    anchors.fill: parent
                    onClicked: {}
                }

                function downKeyRequest() {sglst.downKeyRequest() }
                function upKeyRequest()   {sglst.upKeyRequest()   }
                function enterKeyRequest(){sglst.enterKeyRequest()}
                function pasteSequence(index){
                    waitPaste.index = index
                    waitPaste.restart()
                }
                function searchingText(str){
                    modelSearch.clear()
                    for(var i = 0; i < AppModel.suggestMnemonics.length; i++){
                        if(str === ""){
                            var data = {'mnemonic': AppModel.suggestMnemonics[i]};
                            modelSearch.append(data)
                        }
                        else{
                            var currentText = AppModel.suggestMnemonics[i]
                            if (currentText.toLowerCase().includes(str.toLowerCase())){
                                var datafilter = {'mnemonic': AppModel.suggestMnemonics[i]};
                                modelSearch.append(datafilter)
                            }
                        }
                    }
                    sglst.currentIndex = 0
                }
                ListModel {
                    id: modelSearch
                }
                Timer {
                    id: waitPaste
                    property int index: -1
                    interval: 50
                    running: false
                    repeat: false
                    onTriggered: {
                        var input = inputrepeater.itemAt(index).textInputted
                        var nemonics = input.split(" ")
                        if(nemonics.length === 1){ nemonics = input.split(";") }
                        if(nemonics.length === 1){ nemonics = input.split(":") }
                        if(nemonics.length === 1){ nemonics = input.split("-") }
                        if(nemonics.length === 1){ nemonics = input.split("|") }
                        if(nemonics.length === 1){ return }

                        if(index > 0){
                            for(var i = 0; i < index; i++){ inputrepeater.itemAt(i).textInputted = ""}
                        }
                        var real_idx = 0;
                        for(var j = 0; j < nemonics.length; j++){
                            real_idx = j+index
                            if(real_idx < 24){
                                inputrepeater.itemAt(real_idx).textInputted = nemonics[j]
                            }
                        }
                        real_idx = real_idx+1
                        if(real_idx < 24){
                            for(var k = real_idx+1; k < 24; k++){ inputrepeater.itemAt(k).textInputted = ""}
                        }
                    }
                }
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
                                anchors.centerIn: parent
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
                            if(suggestionList.currentIndexBox !== -1) {inputrepeater.itemAt(suggestionList.currentIndexBox).textInputted = modelSearch.get(sglst.currentIndex).mnemonic}
                            suggestionList.currentIndexBox = -1;
                        }
                        function mouseKeyRequest(index) {
                            if(suggestionList.currentIndexBox !== -1) {inputrepeater.itemAt(suggestionList.currentIndexBox).textInputted = modelSearch.get(index).mnemonic}
                            suggestionList.currentIndexBox = -1;
                        }
                    }
                }
            }

        }
    }
    Component{
        id:enterYourPassphrase
        QOnScreenContent {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_457
            onCloseClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
            }
            Column{
                spacing: 24
                anchors {
                    left: parent.left
                    leftMargin: 36
                    top: parent.top
                    topMargin: 84
                }
                QText {
                    width: 539
                    height: 56
                    text: STR.STR_QML_646
                    color: "#031F2B"
                    font.weight: Font.Normal
                    wrapMode: Text.WrapAnywhere
                    font.family: "Lato"
                    font.pixelSize: 16
                }
                QTextInputBoxTypeB {
                    id: passPhrase
                    label: STR.STR_QML_645
                    isPassword:true
                    boxWidth: 539
                    boxHeight: 48
                    isValid: true
                    onTextInputtedChanged: {
                        if(!passPhrase.isValid){
                            passPhrase.isValid = true
                            passPhrase.errorText = ""
                        }
                        passPhrase.showError = false;
                    }
                }
            }

            QButtonTextLink {
                height: 24
                label: STR.STR_QML_059
                anchors {
                    left: parent.left
                    leftMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                onButtonClicked: {
                    whereIn = _RECOVER_KEY
                }
            }
            QTextButton {
                width: 200
                height: 48
                anchors {
                    right: parent.right
                    rightMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                label.text: STR.STR_QML_097
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    passphrase = passPhrase.textInputted
                    var getKeyName = {
                        "mnemonic": AppModel.mnemonic,
                        "passphrase": passPhrase.textInputted
                    }
                    QMLHandle.sendEvent(EVT.EVT_PRIMARY_KEY_ENTER_PASSPHRASE_REQUEST,getKeyName)
                }
            }
            function signalpkey_signin(https_code, error_code, error_msg){
                if(https_code === DRACO_CODE.SUCCESSFULL && error_code === DRACO_CODE.RESPONSE_OK){
                    whereIn = _KEY_NAME
                }
            }
        }
    }
    Component{
        id:keyName
        QOnScreenContent {
            label.text: STR.STR_QML_266
            onCloseClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_ALL_REQUEST)
            }
            QTextInputBoxTypeB {
                id: signername
                label: STR.STR_QML_267
                boxWidth: 540
                boxHeight: 48
                maxLength: 20
                enableLengthLimit: true
                textInputted: keyname
                anchors {
                    left: parent.left
                    leftMargin: 36
                    top: parent.top
                    topMargin: 130
                }
            }
            QButtonTextLink {
                height: 24
                label: STR.STR_QML_059
                anchors {
                    left: parent.left
                    leftMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                onButtonClicked: {
                    whereIn = _PASSPHRASE
                }
            }
            QTextButton {
                width: 200
                height: 48
                anchors {
                    right: parent.right
                    rightMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                label.text: STR.STR_QML_097
                label.font.pixelSize: 16
                type: eTypeE
                enabled: signername.textInputted !== ""
                onButtonClicked: {
                    var getKeyName = {
                        "passphrase": passphrase,
                        "signername": signername.textInputted
                    }
                    QMLHandle.sendEvent(EVT.EVT_PRIMARY_KEY_ENTER_PASSPHRASE_SIGN_IN_REQUEST,getKeyName)
                }
            }
        }
    }

}

