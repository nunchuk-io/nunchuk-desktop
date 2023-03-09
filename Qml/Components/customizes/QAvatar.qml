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
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.12
import "../origins"

Rectangle {
    id: rootAvatar
    width: 36
    height: 36
    color: "#FDD95C"
    border.color: "#FFFFFF"
    radius: width
    property int groupSize: 18
    property int userCount: 1
    property string avatarUrl: ""
    property string avatarLocal: ""
    property string username: ""
    property int  textSize: 12
    property bool activeStatus: false
    property bool displayStatus: true
    readonly property bool isAvatarUrl: avatarUrl != "";
    readonly property url realSource: isAvatarUrl ? "image://nunchuk/" + avatarUrl : ""
    property int maxChar: 1
    readonly property QImage url: QImage{
        source: realSource
    }

    function getFistCharterName() {
        if(username !== ""){
            if (maxChar == 1) {
                var ret = username.charAt(0)
                if(username.split(' ').length > 1){ ret += username.split(' ')[(username.split(' ').length - 1)].charAt(0)}
                return ret;
            } else {
                return username.substr(0,maxChar)
            }
        }
        return ""
    }

    Loader {
        sourceComponent: userCount > 2 ? avtGroupimage : url.status === Image.Ready ? avtimage : avtname
        anchors.fill: parent
    }
    Loader {
        sourceComponent: avatarLocal === "" ? null : avtChangeimage
        anchors.fill: parent
        visible: avatarLocal !== ""
    }

    Component {
        id: avtChangeimage
        Item{
            width: rootAvatar.width
            height: rootAvatar.height
            QImage {
                id:img
                anchors.fill: parent
                source: avatarLocal
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: rootAvatar.width
                        height: rootAvatar.width
                        radius: width / 2
                    }
                }
                onStatusChanged: {
                    if (img.status == Image.Ready){
                        img.visible = true
                        tmp.source = avatarLocal
                    }else{
                        img.visible = false
                    }
                }
            }

            QImage {
                id:tmp
                anchors.fill: parent
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: rootAvatar.width
                        height: rootAvatar.width
                        radius: width / 2
                    }
                }
                onStatusChanged: {
                    if (tmp.status == Image.Ready){
                        tmp.visible = true
                    }else{
                        tmp.visible = false
                    }
                }
            }
        }
    }

    Component {
        id: avtGroupimage
        Item{
            Image {
                anchors.centerIn: parent
                width: groupSize
                height: groupSize
                fillMode:Image.PreserveAspectFit
                source: "qrc:/Images/Images/groups-24px.svg"
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: rootAvatar.width
                        height: rootAvatar.width
                        radius: width / 2
                    }
                }
            }
        }
    }
    Component {
        id: avtname
        QText {
            font.family: "Lato"
            font.weight: Font.Bold
            font.pixelSize: textSize
            color: "#031F2B"
            anchors.centerIn: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: getFistCharterName()
        }
    }
    Component {
        id: avtimage
        Item{
            width: rootAvatar.width
            height: rootAvatar.height
            QImage {
                id:img
                anchors.fill: parent
                source: realSource
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: rootAvatar.width
                        height: rootAvatar.width
                        radius: width / 2
                    }
                }
                onStatusChanged: {
                    if (img.status == Image.Ready){
                        img.visible = true
                        tmp.source = realSource
                    }else{
                        img.visible = false
                    }
                }
            }

            QImage {
                id:tmp
                anchors.fill: parent
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: rootAvatar.width
                        height: rootAvatar.width
                        radius: width / 2
                    }
                }
                onStatusChanged: {
                    if (tmp.status == Image.Ready){
                        tmp.visible = true
                    }else{
                        tmp.visible = false
                    }
                }
            }
        }
    }
    Rectangle {
        width: 16
        height: 16
        radius: 16
        visible: displayStatus
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "#FFFFFF"
        border.color: "#031F2B"
        QText {
            font.family: "Lato"
            font.weight: Font.Bold
            font.pixelSize: 10
            color: "#031F2B"
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: userCount
        }
    }
}
