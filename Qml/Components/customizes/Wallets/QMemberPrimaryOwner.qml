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
import "./../../origins"
import "./../../customizes"
import "./../../customizes/Chats"
import "./../../customizes/Texts"
import "./../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QTextInputBoxTypeB {
    id: _owner
    label: STR.STR_QML_749
    boxWidth: 537
    boxHeight: 48
    isValid: true
    signal ownerNameChanged(var member)
    onTextInputtedChanged: {
        if(!_owner.isValid){
            _owner.isValid = true
            _owner.errorText = ""
        }
        _owner.showError = false;

        if(textBoxFocus){
            sglst.currentIndex = 0
            searchingText()
        }
    }
    onTextBoxFocusChanged: {
        if(textBoxFocus){
            sglst.currentIndex = 0
            suggestionList.y = _owner.y + _owner.height
            suggestionList.x = _owner.x
            searchingText()
        }
        else{
            sglst.currentIndex = -1
        }
    }
    function closeByMouse() {
        sglst.currentIndex = -1;
    }
    function openByMouse() {
        sglst.currentIndex = 0
        searchingText()
    }

    onDownKeyRequest: {suggestionList.downKeyRequest()  }
    onUpKeyRequest:   {suggestionList.upKeyRequest()    }
    onEnterKeyRequest:{suggestionList.enterKeyRequest() }
    onTabKeyRequest: { suggestionList.enterKeyRequest() }

    Item {
        id: suggestionList
        width: suggestionList.visible ? _owner.width : 0
        height: suggestionList.visible ? Math.min(sglst.contentHeight + 10, 250): 0
        visible: sglst.currentIndex !== -1
        MouseArea {
            anchors.fill: parent
            onClicked: {
            }
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
                    id: _rect
                    property var member: modelData
                    width: sglst.width
                    height: 64
                    color: suggestMouse.containsMouse ? "#F1FAFE" : "#FFFFFF"
                    Row {
                        anchors.fill: parent
                        spacing: 12
                        QAvatar {
                            width: 48
                            height: 48
                            avatarLocal: member.user ? member.user.avatar : ""
                            avatarUrl: member.user ? member.user.avatar : ""
                            username: member.user ? member.user.name : ""
                            displayStatus: false
                            activeStatus: member.status === "PENDING" ? false : true
                            color: member.status === "PENDING" ? "#EAEAEA" : "#FDD95C"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Column {
                            id: _col
                            spacing: 4
                            anchors.verticalCenter: parent.verticalCenter
                            QLato {
                                height: 20
                                text: member.user ? member.user.name : ""
                                font.pixelSize: 16
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            QLato {
                                height: 16
                                font.pixelSize: 12
                                color: "#595959"
                                text: member.user ? member.user.email : ""
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
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
                        var splitString = _owner.textInputted.split(" ");
                        var mb = modelSearch.get(sglst.currentIndex).member
                        splitString[splitString.length - 1] = mb.user.name;
                        _owner.textInputted = splitString.toString(" ");
                        ownerNameChanged(mb)
                        sglst.currentIndex = -1;
                    }
                }
                function mouseKeyRequest(index) {
                    if(index !== -1) {
                        var splitString = _owner.textInputted.split(" ");
                        var mb = modelSearch.get(index).member
                        splitString[splitString.length - 1] = mb.user.name;
                        _owner.textInputted = splitString.toString(" ");
                        ownerNameChanged(mb)
                    }
                    sglst.currentIndex = -1;
                }
            }
        }
    }
    property var walletInfo: AppModel.walletInfo
    function searchingText(){
        // SPIT TEXT
        modelSearch.clear()
        for(var i = 0; i < walletInfo.ownerMembers.length; i++){
            var data = {'member': walletInfo.ownerMembers[i]};
            modelSearch.append(data)
        }
        sglst.currentIndex = 0
        return true
    }
    ListModel {
        id: modelSearch
    }
}
