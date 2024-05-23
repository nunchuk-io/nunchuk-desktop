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
import NUNCHUCKTYPE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: rootAddsignerToWallet
    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        enableHeader: false
        anchors.centerIn: parent
        Loader {
            id: loadercontent
            anchors.fill: parent
            sourceComponent: maincontent
        }
        Component {
            id: maincontent
            Item {
                QText {
                    id: screenname
                    width: 163
                    height: 27
                    anchors {
                        left: parent.left
                        leftMargin: 36
                        top: parent.top
                        topMargin: 36
                    }
                    text: STR.STR_QML_083
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.weight: Font.ExtraBold
                    font.pixelSize: 24
                }
                QCloseButton {
                    anchors {
                        verticalCenter: screenname.verticalCenter
                        right: parent.right
                        rightMargin: 24
                    }
                    onClicked: {
                        QMLHandle.sendEvent(EVT.EVT_ADD_SIGNER_TO_WALLET_BACK_REQUEST)
                    }
                }
                Row {
                    spacing: 48
                    anchors.top: parent.top
                    anchors.topMargin: 122
                    anchors.horizontalCenter: parent.horizontalCenter
                    Item {
                        width: 246
                        height: 396
                        Column {
                            spacing: 4
                            Rectangle {
                                width: 96
                                height: 96
                                radius: width
                                color: "#F5F5F5"
                                anchors.horizontalCenter: parent.horizontalCenter
                                QImage {
                                    width: 60
                                    height: 60
                                    anchors.centerIn: parent
                                    source: "qrc:/Images/Images/hardware_add-60px.png"
                                }
                            }
                            QText {
                                width: 210
                                height: 56
                                text: STR.STR_QML_084
                                horizontalAlignment: Text.AlignHCenter
                                font.family: "Lato"
                                font.pixelSize: 20
                                font.weight: Font.Bold
                                wrapMode: Text.WordWrap
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            QText {
                                width: 210
                                height: 112
                                text: STR.STR_QML_085
                                font.family: "Lato"
                                font.pixelSize: 16
                                font.weight: Font.Light
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignHCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            QTextButton {
                                width: 180
                                height: 48
                                label.text: STR.STR_QML_086
                                label.font.pixelSize: 16
                                label.font.family: "Lato"
                                type: eTypeB
                                anchors.horizontalCenter: parent.horizontalCenter
                                onButtonClicked: {
                                    loadercontent.sourceComponent = beforeStartHardware
                                }
                            }
                        }
                    }
                    Item {
                        width: 246
                        height: 450
                        Column {
                            spacing: 4
                            Rectangle {
                                width: 96
                                height: 96
                                radius: width
                                color: "#F5F5F5"
                                anchors.horizontalCenter: parent.horizontalCenter
                                QImage {
                                    width: 60
                                    height: 60
                                    anchors.centerIn: parent
                                    source: "qrc:/Images/Images/software_add-60px.svg"
                                }
                            }
                            QText {
                                width: 210
                                height: 56
                                text: STR.STR_QML_087
                                horizontalAlignment: Text.AlignHCenter
                                font.family: "Lato"
                                font.pixelSize: 20
                                font.weight: Font.Bold
                                wrapMode: Text.WordWrap
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            QText {
                                width: 210
                                height: 112
                                text: STR.STR_QML_088
                                font.family: "Lato"
                                font.pixelSize: 16
                                font.weight: Font.Light
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignHCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            QTextButton {
                                width: 180
                                height: 48
                                label.text: enabled ? STR.STR_QML_089 : STR.STR_QML_090
                                label.font.pixelSize: 16
                                label.font.family: "Lato"
                                type: eTypeB
                                anchors.horizontalCenter: parent.horizontalCenter
                                enabled: AppModel.limitSoftwareSigner ? (AppModel.softwareSignerDeviceList.count === 0) : true
                                onButtonClicked: {
                                    loadercontent.sourceComponent = beforeStartSoftware
                                }
                            }
                        }
                    }
                }
            }
        }
        Component {
            id: beforeStartHardware
            Item {
                QText {
                    id: screenname
                    width: 163
                    height: 27
                    anchors {
                        left: parent.left
                        leftMargin: 36
                        top: parent.top
                        topMargin: 36
                    }
                    text: STR.STR_QML_091
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.weight: Font.ExtraBold
                    font.pixelSize: 24
                }
                QCloseButton {
                    anchors {
                        verticalCenter: screenname.verticalCenter
                        right: parent.right
                        rightMargin: 24
                    }
                    onClicked: {
                        loadercontent.sourceComponent = maincontent
                    }
                }
                Column {
                    spacing: 24
                    width: 536
                    height: 336
                    anchors.top: parent.top
                    anchors.topMargin: 144
                    anchors.horizontalCenter: parent.horizontalCenter
                    Rectangle {
                        width: 96
                        height: 96
                        radius: width
                        color: "#F5F5F5"
                        anchors.horizontalCenter: parent.horizontalCenter
                        QImage {
                            width: 60
                            height: 60
                            anchors.centerIn: parent
                            source: "qrc:/Images/Images/info-60px.png"
                        }
                    }
                    QText {
                        width: 500
                        height: 28
                        text: STR.STR_QML_092
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Lato"
                        font.pixelSize: 16
                        color: "#031F2B"
                    }
                    Column {
                        id: guide
                        spacing: 16
                        anchors.horizontalCenter: parent.horizontalCenter
                        Item {
                            width: 536
                            height: 84
                            Rectangle {
                                id:guidenum1
                                width: 24
                                height: 24
                                radius: width
                                color: "#031F2B"
                                border.width: 2
                                border.color: "#031F2B"
                                QText {
                                    anchors.centerIn: parent
                                    color: "#FFFFFF"
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                    text: "1"
                                }
                            }
                            Column {
                                width: parent.width - 28
                                anchors.left: guidenum1.right
                                anchors.leftMargin: 8
                                spacing: 8
                                QText {
                                    width: parent.width
                                    color: "#031F2B"
                                    font.pixelSize: 16
                                    font.weight: Font.DemiBold
                                    font.family: "Montserrat"
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 28
                                    text: STR.STR_QML_093
                                }
                                QText {
                                    width: parent.width
                                    color: "#323E4A"
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                    wrapMode: Text.WordWrap
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 28
                                    text: STR.STR_QML_094
                                }
                            }
                        }
                        Item {
                            width: 536
                            height: 84
                            Rectangle {
                                id:guidenum2
                                width: 24
                                height: 24
                                radius: width
                                color: "#031F2B"
                                border.width: 2
                                border.color: "#031F2B"
                                QText {
                                    anchors.centerIn: parent
                                    color: "#FFFFFF"
                                    font.pixelSize: 16
                                    font.weight: Font.DemiBold
                                    text: "2"
                                }
                            }
                            Column {
                                width: parent.width - 28
                                anchors.left: guidenum2.right
                                anchors.leftMargin: 8
                                spacing: 8
                                QText {
                                    width: parent.width
                                    color: "#031F2B"
                                    font.pixelSize: 16
                                    font.weight: Font.DemiBold
                                    font.family: "Montserrat"
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 28
                                    text: STR.STR_QML_095
                                }
                                QText {
                                    width: parent.width
                                    color: "#323E4A"
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                    wrapMode: Text.WordWrap
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 28
                                    text: STR.STR_QML_096
                                }
                            }
                        }
                    }
                }
                QButtonTextLink {
                    height: 24
                    label: STR.STR_QML_059
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        bottom: parent.bottom
                        bottomMargin: 40
                    }
                    onButtonClicked: {
                        loadercontent.sourceComponent = maincontent
                    }
                }
                QTextButton {
                    width: 200
                    height: 48
                    label.text: STR.STR_QML_097
                    label.font.pixelSize: 16
                    type: eTypeE
                    anchors {
                        right: parent.right
                        rightMargin: 40
                        bottom: parent.bottom
                        bottomMargin: 32
                    }
                    onButtonClicked: {
                        QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_SIGNER_TO_WALLET)
                    }
                }
            }
        }
        Component {
            id: beforeStartSoftware
            Item {
                QText {
                    id: screenname
                    width: 163
                    height: 27
                    anchors {
                        left: parent.left
                        leftMargin: 36
                        top: parent.top
                        topMargin: 36
                    }
                    text: STR.STR_QML_091
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.weight: Font.ExtraBold
                    font.pixelSize: 24
                }
                QCloseButton {
                    anchors {
                        verticalCenter: screenname.verticalCenter
                        right: parent.right
                        rightMargin: 24
                    }
                    onClicked: {
                        loadercontent.sourceComponent = maincontent
                    }
                }
                Column {
                    spacing: 24
                    width: 536
                    height: 336
                    anchors.top: parent.top
                    anchors.topMargin: 144
                    anchors.horizontalCenter: parent.horizontalCenter
                    Rectangle {
                        width: 96
                        height: 96
                        radius: width
                        color: "#F5F5F5"
                        anchors.horizontalCenter: parent.horizontalCenter
                        QImage {
                            width: 60
                            height: 60
                            anchors.centerIn: parent
                            source: "qrc:/Images/Images/warning_amber-60px.png"
                        }
                    }
                    QText {
                        width: 500
                        height: 28
                        text: STR.STR_QML_098
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Lato"
                        font.pixelSize: 16
                        color: "#031F2B"
                        wrapMode: Text.WordWrap
                    }
                }
                QButtonTextLink {
                    height: 24
                    label: STR.STR_QML_059
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        bottom: parent.bottom
                        bottomMargin: 40
                    }
                    onButtonClicked: {
                        loadercontent.sourceComponent = maincontent
                    }
                }
                Row {
                    height: 48
                    spacing: 16
                    anchors {
                        right: parent.right
                        rightMargin: 40
                        bottom: parent.bottom
                        bottomMargin: 32
                    }
                    QTextButton {
                        width: 280
                        height: 48
                        label.text: STR.STR_QML_099
                        label.font.pixelSize: 16
                        type: eTypeB
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_RECOVER_SOFTWARE_SIGNER_TO_WALLET)
                        }
                    }
                    QTextButton {
                        width: 200
                        height: 48
                        label.text: STR.STR_QML_100
                        label.font.pixelSize: 16
                        type: eTypeE
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET)
                        }
                    }
                }
            }
        }
    }
}
