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
import "../../../Components/customizes"
import "../../../Components/origins"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    id: roomRoot
    width: 326
    height: 46
    color: "transparent"
    property string signername: "A B"
    property string signerxfp : "A B"
    property bool   isLocaluser: false
    property string sectionname: ""
    property bool   isAirgappedSigner: false
    property bool   isSoftwareSigner : false
    property int    signerType: -1
    property bool   isPrimaryKey: false
    signal itemClicked()
    Row {
        id: roomDelegateContent
        anchors.fill: parent
        anchors.verticalCenter: parent.verticalCenter
        spacing: 8
        QAvatar {
            id: avatar
            width: 36
            height: 36
            username: signername
            anchors.verticalCenter: parent.verticalCenter
            displayStatus: false
        }
        Column {
            width: isLocaluser ? 185 + assignedText.width : 185
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
            Row{
                height: 16
                spacing: 4
                QText {
                    height: 16
                    visible: signerxfp !== ""
                    font.family: "Lato"
                    font.pixelSize: 12
                    color: "#031F2B"
                    text: "XFP: " + signerxfp
                    font.capitalization: Font.AllUppercase
                }

                Rectangle {
                    width: _txt1.paintedWidth + 8*2
                    height: 16
                    color: "#FDD95C"
                    visible: isPrimaryKey
                    radius: 4
                    QText {
                        id:_txt1
                        text: STR.STR_QML_641
                        font.family: "Lato"
                        font.weight: Font.Bold
                        font.pixelSize: 10
                        anchors.centerIn: parent
                        color: "#031F2B"
                    }
                }

                Rectangle {
                    width: typesigner.width + 10
                    height: parent.height
                    color: "#EAEAEA"
                    visible: isLocaluser && (signerType === NUNCHUCKTYPE.AIRGAP || signerType === NUNCHUCKTYPE.SOFTWARE)
                    radius: 20
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
            }


        }
        QText{
            id: assignedText
            font.family: "Lato"
            text: STR.STR_QML_511
            font.pixelSize: 12
            font.weight: Font.Bold
            color: "#031F2B"
            elide: Text.ElideRight
            anchors.verticalCenter: parent.verticalCenter
            visible: signerxfp !== "" && !isLocaluser
        }
        QImage {
            width: 24
            height: 24
            source: "qrc:/Images/Images/OnlineMode/check_circle_24px_n.png"
            anchors.verticalCenter: parent.verticalCenter
            visible: signerxfp !== "" && !isLocaluser
        }
        QImage {
            width: 24
            height: 24
            scale: leavemouse.pressed? 1 : leavemouse.containsMouse ? 1.1 : 1
            transformOrigin: Item.Center
            source: "qrc:/Images/Images/Close.png"
            visible: signerxfp !== "" && isLocaluser && !RoomWalletData.roomWalletCreated
            enabled: visible
            anchors.verticalCenter: parent.verticalCenter
            MouseArea {
                id: leavemouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: { itemClicked()}
            }
        }
    }
}
