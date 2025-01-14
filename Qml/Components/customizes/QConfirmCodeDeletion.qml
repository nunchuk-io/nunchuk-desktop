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
import QtGraphicalEffects 1.12
import QtQuick.Controls 2.5
import QRCodeItem 1.0
import "../origins"
import "../customizes/Texts"
import "../customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

Popup {
    id: deletingModelRoot
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape
    background: Item{}
    property string label: ""
    property string content: ""
    property string leftBtnLabel: STR.STR_QML_245
    property string rightBtnLabel: STR.STR_QML_246
    property string placeholderText: STR.STR_QML_244
    property alias emailCode: inputDelete
    property int modalWidth: 488
    property int modalHeight: 317
    property int modelRadius: 24
    signal cancelRequest()
    signal deleteRequest()
    Rectangle {
        id: sourceBg
        width: modalWidth
        height: modalHeight
        radius: modelRadius
        color: "#FFFFFF"
        anchors.centerIn: parent
        Rectangle {
            width: parent.width
            height: 2
            color: "Red"
            radius: 8
        }
        visible: false
    }
    Rectangle {
        id: mask
        width: modalWidth
        height: modalHeight
        radius: modelRadius
        visible: false
    }
    OpacityMask {
        id: opacitymask
        anchors.fill: sourceBg
        source: sourceBg
        maskSource: mask
    }
    DropShadow {
        anchors.fill: opacitymask
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: opacitymask
    }

    Item{
        width: modalWidth
        height: modalHeight
        anchors.centerIn: parent
        Column{
            height: parent.height - 36
            width: parent.width
            anchors.centerIn: parent
            spacing: 16
            QText {
                width: modalWidth - 36*2
                height: paintedHeight
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignLeft
                text: deletingModelRoot.label
                color: "#031F2B"
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 40
                QCloseButton {
                    icon: "qrc:/Images/Images/Close.png"
                    anchors {
                        right: parent.right
                        rightMargin: 0
                        verticalCenter: parent.verticalCenter
                    }
                    onClicked: {
                        deletingModelRoot.close()
                    }
                }
            }

            Rectangle{
                width: modalWidth - 36*2
                height: 108
                color: "#EAEAEA"
                anchors.horizontalCenter: parent.horizontalCenter
                QIcon {
                    iconSize: 30
                    anchors{
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 15
                    }
                    source: "qrc:/Images/Images/info-60px.svg"
                }
                QText {
                    width: parent.width - 12 - 56
                    height: Math.max(paintedHeight,84)
                    wrapMode: Text.WordWrap
                    anchors{
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                        rightMargin: 12
                    }
                    horizontalAlignment: Text.AlignHCenter
                    text: deletingModelRoot.content
                    font.family: "Lato"
                    font.pixelSize: 16
                }
            }

            QTextInputBoxTypeB {
                id: inputDelete
                anchors.horizontalCenter: parent.horizontalCenter
                boxWidth: modalWidth - 36*2
                boxHeight: 48
                label: STR.STR_QML_426
                isValid: true
            }

            Item{
                width: modalWidth - 36*2
                height: 48 + 20
                anchors.horizontalCenter: parent.horizontalCenter
                QTextButton {
                    anchors{
                        left: parent.left
                        leftMargin: 36
                        verticalCenter: parent.verticalCenter
                    }

                    width: 120
                    height: 48
                    label.text: leftBtnLabel
                    label.font.pixelSize: 20
                    type: eTypeB
                    onButtonClicked: { cancelRequest(); deletingModelRoot.close() }
                }
                QTextButton {
                    anchors{
                        right: parent.right
                        rightMargin: 36
                        verticalCenter: parent.verticalCenter
                    }
                    width: label.paintedWidth + 36
                    height: 48
                    label.text: rightBtnLabel
                    label.font.pixelSize: 20
                    type: eTypeE
                    onButtonClicked: { deleteRequest();}
                }
            }
        }


    }





}
