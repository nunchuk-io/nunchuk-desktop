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
import "../../origins"
import "../../customizes"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: signermanagerdlg
    property bool   needXpub: false
    property alias  nameWidth: text.width
    property alias  signername: id_signername.text
    property alias  signerXFP: id_xfp.text
    property string devicetype: ""
    property int    signerType : 0
    property int    accountIndex: 0
    property bool   isPrimaryKey: false
    property bool   checkedState: false
    readonly property var iconUnChecked: ["qrc:/Images/Images/UnChecked_n.png"  , "qrc:/Images/Images/UnChecked_d.png"  ]
    readonly property var iconChecked:   ["qrc:/Images/Images/Checked_n.png"    , "qrc:/Images/Images/Checked_d.png"    ]
    Rectangle {
        id:_icon
        width: 48
        height: 48
        radius: width
        color: "#F5F5F5"
        anchors.verticalCenter: parent.verticalCenter

        QSignerDarkIcon {
            iconSize: 24
            anchors.centerIn: parent
            device_type: devicetype
            type: signerType
        }
    }
    Column {
        id: text
        anchors.left: _icon.right
        anchors.leftMargin: 8
        anchors.verticalCenter: _icon.verticalCenter
        spacing: 8
        Column{
            spacing: 2
            QText {
                id: id_signername
                width: 400
                height: 28
                font.family: "Lato"
                color: "#031F2B"
                font.pixelSize: 16
                font.weight: Font.DemiBold
                elide: Text.ElideRight
            }
            QText {
                id: id_xfp
                width: 91
                height: 16
                font.family: "Lato"
                color: "#031F2B"
                font.pixelSize: 12
                font.capitalization: Font.AllUppercase
            }
        }
        QRowSingleSignerType {
            isPrimaryKey: signermanagerdlg.isPrimaryKey
            signerType:   signermanagerdlg.signerType
            accountIndex: signermanagerdlg.accountIndex
        }
    }

    signal getXpubRequest()
    QTextButton {
        width: 100
        height: 24
        label.text: STR.STR_QML_071
        label.font.pixelSize: 10
        type: eTypeD
        visible: needXpub
        enabled: visible
        anchors {
            right: parent.right
            rightMargin: 38
            verticalCenter: parent.verticalCenter
        }
        onButtonClicked: {
            getXpubRequest()
        }
    }

    signal itemChecked()
    QIcon {
        iconSize: 24
        anchors.verticalCenter: parent.verticalCenter
        source: checkedState ? iconChecked[0] : iconUnChecked[0]
        anchors.right: parent.right
        anchors.rightMargin: 5
        MouseArea {
            id: mastersignerMouse
            hoverEnabled: true
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                itemChecked()
            }
        }
    }
}
