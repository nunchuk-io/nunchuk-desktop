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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../origins"
import "../customizes"
import "../customizes/Texts"
import "../customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

Popup {
    id: _infoPopup
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    property string contentText: ""
    property string title: STR.STR_QML_339
    signal gotItClicked()
    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    background: Item{}
    Rectangle {
        width: popupWidth
        height: popupHeight
        radius: 8
        color: Qt.rgba(255, 255, 255, 0)
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: popupWidth
                height: popupHeight
                radius: 24
            }
        }
        Rectangle {
            id: bgcontentlimit
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            color: "#FFFFFF"
            radius: 24
            QText {
                anchors{
                    left: parent.left
                    leftMargin: 36
                    top: parent.top
                    topMargin: 36
                }
                font.family: "Lato"
                font.pixelSize: 32
                font.weight: Font.Bold
                text: STR.STR_QML_597
                lineHeightMode: Text.FixedHeight
                lineHeight: 40
            }

            Column {
                id:_colum
                spacing: 24
                anchors.centerIn: parent
                QImage {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 96
                    height: 96
                    source: "qrc:/Images/Images/account-circle-dark.svg"
                }

                QText {
                    width: 500
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    color: "#031F2B"
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: STR.STR_QML_598
                }
                Item {
                    width: 536
                    height: _1.paintedHeight
                    anchors.horizontalCenter: parent.horizontalCenter
                    QImage {
                        anchors.left: parent.left
                        width: 24
                        height: 24
                        source: "qrc:/Images/Images/1.Active.svg"
                    }
                    QText {
                        id:_1
                        anchors.right: parent.right
                        width: 500
                        font.family: "Lato"
                        font.pixelSize: 16
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        color: "#031F2B"
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        text: STR.STR_QML_599
                    }
                }
                Item {
                    width: 536
                    height: _2.paintedHeight
                    anchors.horizontalCenter: parent.horizontalCenter
                    QImage {
                        anchors.left: parent.left
                        width: 24
                        height: 24
                        source: "qrc:/Images/Images/2.Active.svg"
                    }
                    QText {
                        id:_2
                        anchors.right: parent.right
                        width: 500
                        font.family: "Lato"
                        font.pixelSize: 16
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        color: "#031F2B"
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        text: STR.STR_QML_600
                    }
                }

                Item {
                    width: 536
                    height: _3.paintedHeight
                    anchors.horizontalCenter: parent.horizontalCenter
                    QText {
                        id:_3
                        anchors.right: parent.right
                        width: 500
                        font.family: "Lato"
                        font.pixelSize: 16
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        color: "#031F2B"
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        text: STR.STR_QML_601
                    }
                }
            }
            QTextButton {
                anchors{
                    right: parent.right
                    rightMargin: 36
                    bottom: parent.bottom
                    bottomMargin: 36
                }
                width: 73
                height: 48
                label.text: STR.STR_QML_341
                label.font.pixelSize: 12
                type: eTypeE
                onButtonClicked: {
                    _infoPopup.close()
                    gotItClicked()
                }
            }
        }
        DropShadow {
            anchors.fill: bgcontentlimit
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: bgcontentlimit
        }
    }
}
