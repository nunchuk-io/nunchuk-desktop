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
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../origins"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    id: signermanagerdlg
    property alias  nameWidth: text.width
    property alias  signername: id_signername.text
    property alias  signerXFP: id_xfp.text
    property string devicetype: ""
    property int    signerType : 0
    property bool   isPrimaryKey: false
    color: mastersignerMouse.containsMouse ? Qt.rgba(255, 255, 255, 0.1) : "transparent"
    Rectangle {
        width: parent.width
        height: 1
        color: "#C9DEF1"
        opacity: 0.1
        anchors.bottom: parent.bottom
    }
    QImage {
        id: indicator
        width: 30
        height: 30
        source: GlobalData.icons(devicetype,signerType)
        anchors.left: parent.left
        anchors.leftMargin: 24
        anchors.verticalCenter: parent.verticalCenter
    }
    Column {
        id: text
        anchors.left: indicator.right
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        width: 160
        height: 37
        QText {
            id: id_signername
            width: parent.width
            height: paintedHeight
            font.family: "Montserrat"
            color: "#F1FAFE"
            elide: Text.ElideRight
            font.pixelSize: 14
            wrapMode: Text.WordWrap
            font.weight: Font.DemiBold
            horizontalAlignment: Text.AlignLeft
        }
        QText {
            id: id_xfp
            width: parent.width
            height: 16
            color: "#F1FAFE"
            font.family: "Lato"
            elide: Text.ElideRight
            font.pixelSize: 12
            horizontalAlignment: Text.AlignLeft
            font.capitalization: Font.AllUppercase
        }
    }
    Rectangle {
        width: 70
        height: 21
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: _Type.left
        anchors.rightMargin: 4
        color: "#FDD95C"
        visible: isPrimaryKey
        radius: 4
        QText {
            text: STR.STR_QML_641
            font.family: "Lato"
            font.weight: Font.Bold
            font.pixelSize: 10
            anchors.centerIn: parent
            color: "#031F2B"
        }
    }
    Rectangle {
        id:_Type
        width: 85
        height: 21
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        color: "#C9DEF1"
        visible: signerType !== NUNCHUCKTYPE.HARDWARE
        radius: 4
        QText {
            text: GlobalData.signers(signerType)
            font.family: "Lato"
            font.weight: Font.Bold
            font.pixelSize: 10
            anchors.centerIn: parent
            color: "#031F2B"
        }
    }
    signal buttonClicked()
    MouseArea {
        id: mastersignerMouse
        hoverEnabled: true
        anchors.fill: parent
        onClicked: signermanagerdlg.buttonClicked()
    }
}
