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
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Column {
    id: dataInput
    width: 528
    height: 175
    property string title: STR.STR_QML_429
    property var requestlist: []
    property bool errorAlert: false
    signal emailUpdated()
    QText {
        text: title
        color: "#031F2B"
        font.family: "Lato"
        font.weight: Font.Bold
        font.pixelSize: 16
    }
    Rectangle {
        width: parent.width
        height: dataInput.height - 20
        radius: 8
        color: "#FFFFFF"
        border.color: "#DEDEDE"
        Flickable {
            anchors.fill: parent
            clip: true
            contentHeight: flowemail.implicitHeight
            interactive: contentHeight > height
            flickableDirection: Flickable.VerticalFlick
            contentY : contentHeight > height ? contentHeight - height + 10 : 0
            ScrollBar.vertical: ScrollBar { active: true }
            Flow {
                id: flowemail
                spacing: 6
                anchors {
                    fill: parent
                    rightMargin: 6
                    topMargin: 6
                    leftMargin: 6
                    bottomMargin: 6
                }
                Repeater {
                    id: emailrepeat
                    model: 0
                    function validateEmail(email) {
                        if(Draco.pkey_username_availability(email)){
                            return true;
                        }else{
                            var re = /\S+@\S+\.\S+/;
                            return re.test(email);
                        }
                    }
                    Rectangle {
                        id: background
                        width: emailelement.width + 16
                        height: 36
                        radius: 36
                        color: emailelement.isemail ? "#A7F0BA" : "#FFD7D9"
                        Row {
                            id: emailelement
                            width: emailtext.width + 60
                            height: 24
                            anchors.centerIn: parent
                            spacing: 6
                            property bool isemail: emailrepeat.validateEmail(dataInput.requestlist[index])
                            QIcon {
                                iconSize: 24
                                source: emailelement.isemail ? "qrc:/Images/Images/OnlineMode/check_circle_outline_24px.png"
                                                             : "qrc:/Images/Images/OnlineMode/error_outline_24px_n.png";
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QText {
                                id: emailtext
                                text: dataInput.requestlist[index]
                                height: 24
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QIcon {
                                iconSize: 24
                                source: "qrc:/Images/Images/close-dark.svg"
                                anchors.verticalCenter: parent.verticalCenter
                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: {
                                        dataInput.requestlist.splice(index, 1)
                                        dataInput.errorAlert = !dataInput.isVaildAll()
                                        emailrepeat.model = dataInput.requestlist.length
                                    }
                                }
                            }
                        }
                    }
                }
                QTextField {
                    id: emailinput
                    height: 36
                    verticalAlignment: Text.AlignVCenter
                    placeholderText: STR.STR_QML_591
                    color: "#031F2B"
                    font.pixelSize: 16
                    Keys.onSpacePressed:  { emailinput.finishInputEmail()}
                    Keys.onEnterPressed:  { emailinput.finishInputEmail()}
                    Keys.onReturnPressed: { emailinput.finishInputEmail()}
                    background: Item {
                        anchors.fill: parent
                    }
                    function finishInputEmail(){
                        if(emailinput.text !== ""){
                            dataInput.requestlist[dataInput.requestlist.length] = emailinput.text
                            emailrepeat.model = dataInput.requestlist.length
                            emailinput.text = ""
                            dataInput.errorAlert = !dataInput.isVaildAll()
                            emailUpdated()
                        }
                    }
                }
            }
        }
    }
    function isVaildAll(){
        var ret = true;
        for(var i = 0; i < dataInput.requestlist.length; i ++){
            var emailAddress = dataInput.requestlist[i]
            ret = ret && emailrepeat.validateEmail(emailAddress)
        }
        return ret;
    }

    Row{
        id: _ErrorAlert
        spacing: 6
        height: 28
        anchors.left: parent.left
        visible: dataInput.errorAlert
        QImage {
            width: 20
            height: 20
            source: "qrc:/Images/Images/error_outline_24px.png"
            anchors.verticalCenter: parent.verticalCenter
        }
        QText {
            text: STR.STR_QML_575
            color: "#CF4018"
            font.family: "Lato"
            font.weight: Font.Medium
            font.pixelSize: 16
            horizontalAlignment: Text.AlignLeft
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
