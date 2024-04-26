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
import QRCodeItem 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_247
        extraHeader: QText {
            height: 21
            text: "(" + AppModel.walletInfo.walletName + ")"
            color: "#031F2B"
            font.weight: Font.DemiBold
            font.family: "Montserrat"
            font.pixelSize: 14
            anchors.verticalCenter: parent.verticalCenter
        }
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_RECEIVE_BACK_REQUEST)
        }
        Row {
            spacing: 4
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 92
            }
            QText {
                id: unuseListTitle
                height: 24
                text: STR.STR_QML_199.arg(unUsed.count)
                color: "#031F2B"
                font.weight: Font.DemiBold
                font.family: "Montserrat"
                font.pixelSize: 16
                anchors.verticalCenter: parent.verticalCenter
            }
            QText {
                height: 16
                text: qsTr("(%1 max)").arg(MAX_UNUSED_ADDR)
                color: "#000000"
                font.family: "Lato"
                font.pixelSize: 12
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        QText {
            id: useListTitle
            height: 24
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 387
            }
            text: STR.STR_QML_200.arg(used.count)
            color: "#031F2B"
            font.weight: Font.DemiBold
            font.family: "Montserrat"
            font.pixelSize: 16
        }
        QText {
            id: recommentNote
            width: 301
            height: 42
            anchors {
                left: useListTitle.left
                top: useListTitle.bottom
            }
            text: STR.STR_QML_201
            color: "#000000"
            wrapMode: Text.WordWrap
            font.family: "Lato"
            font.pixelSize: 10
        }
        FocusScope  {
            QListView {
                id: unUsed
                width: 301
                height: 160
                clip: true
                currentIndex: 0
                focus: true
                anchors {
                    left: parent.left
                    leftMargin: 40
                    top: parent.top
                    topMargin: 121
                }
                model: AppModel.walletInfo.walletunUsedAddressList
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: Rectangle {
                    width: 301
                    height: 40
                    color:  mouse1.containsMouse ? Qt.rgba(255, 255, 255, 1) : ((unUsed.currentIndex == index) && (unUsed.focus) ?
                                                                                    Qt.rgba(255, 255, 255, 0.5) : "#F1FAFE" )
                    Rectangle {
                        width: 8
                        height: parent.height
                        color: "#F6D65D"
                        visible: (unUsed.currentIndex == index) && (unUsed.focus)
                    }
                    Rectangle {
                        width: parent.width
                        height: 1
                        color: "#C9DEF1"
                        anchors.bottom: parent.bottom
                    }
                    QText {
                        text: modelData
                        width: (unUsed.currentIndex == index) && (unUsed.focus) ? 244 : 237
                        height: 16
                        anchors.left: parent.left
                        anchors.leftMargin: (unUsed.currentIndex == index) && (unUsed.focus) ? 16 : 0
                        anchors.verticalCenter: parent.verticalCenter
                        font.family: "Lato"
                        font.pixelSize: 12
                        elide: Text.ElideRight
                        font.weight: (unUsed.currentIndex == index) && (unUsed.focus) ? Font.Bold : Font.Normal
                        color: (unUsed.currentIndex == index) && (unUsed.focus) ? "#031F2B" : "#839096"
                    }
                    MouseArea {
                        id: mouse1
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            unUsed.focus = true
                            unUsed.currentIndex = index
                            addressSelected.text = modelData
                            QMLHandle.sendEvent(EVT.EVT_RECEIVE_ADDRESS_BALANCE, modelData)
                        }
                    }
                    Component.onCompleted: {
                        if(unUsed.currentIndex === index){
                            addressSelected.text = modelData
                            QMLHandle.sendEvent(EVT.EVT_RECEIVE_ADDRESS_BALANCE, modelData)
                        }
                    }
                }
            }
            QListView {
                id: used
                width: 301
                height: 160
                clip: true
                currentIndex: 0
                anchors {
                    left: parent.left
                    leftMargin: 40
                    top: parent.top
                    topMargin: 450
                }
                model: AppModel.walletInfo.walletusedAddressList
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: Rectangle {
                    width: 301
                    height: 40
                    color:  mouse2.containsMouse ? Qt.rgba(255, 255, 255, 1) : (used.currentIndex == index) && (used.focus) ? Qt.rgba(255, 255, 255, 0.5) : "#F1FAFE"
                    Rectangle {
                        width: 8
                        height: parent.height
                        color: "#F6D65D"
                        visible: (used.currentIndex == index) && (used.focus)
                    }
                    Rectangle {
                        width: parent.width
                        height: 1
                        color: "#C9DEF1"
                        anchors.bottom: parent.bottom
                    }
                    QText {
                        text: modelData
                        width: (used.currentIndex == index) && (used.focus) ? 244 : 237
                        height: 16
                        anchors.left: parent.left
                        anchors.leftMargin: (used.currentIndex == index) && (used.focus) ? 16 : 0
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        font.family: "Lato"
                        elide: Text.ElideRight
                        font.weight: (used.currentIndex == index) && (used.focus) ? Font.Bold : Font.Normal
                        color: (used.currentIndex == index) && (used.focus) ? "#031F2B" : "#839096"
                    }
                    MouseArea {
                        id: mouse2
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            used.focus = true
                            used.currentIndex = index
                            addressSelected.text = modelData
                            QMLHandle.sendEvent(EVT.EVT_RECEIVE_ADDRESS_BALANCE, modelData)
                        }
                    }
                }
            }
        }
        QButtonMedium {
            width: 207
            height: 32
            type: eOUTLINE_NORMAL
            label: STR.STR_QML_202
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 315
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_BTN_GEN_NEW_ADDRESS)
            }
        }
        Rectangle {
            width: 419
            height: 490
            color: Qt.rgba(255, 255, 255, 0.5)
            radius: 8
            anchors {
                left: parent.left
                leftMargin: 341
                top: parent.top
                topMargin: 121
            }
            QIconTextButton {
                width: 200
                height: 24
                label: STR.STR_QML_248
                icons: [
                    "bookmark_border_24px_031F2B.png",
                    "bookmark_border_24px_9CAEB8.png",
                    "bookmark_border_24px_F1FAFE.png",
                    "bookmark_border_24px_F1FAFE.png"
                ]
                fontPixelSize: 16
                type: eTypeB
                anchors {
                    right: parent.right
                    rightMargin: 16
                    top: parent.top
                    topMargin: 22
                }
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_RECEIVE_SET_QUICK_RECEIVE_ADDRESS, qrCode.textInput)
                }
            }
            Rectangle {
                color: unUsed.focus ? "#35ABEE" : "#FF7A00"
                width: 62
                height: 24
                radius: 4
                anchors {
                    top: parent.top
                    topMargin: 72
                    horizontalCenter: parent.horizontalCenter
                }
                QText {
                    anchors.centerIn: parent
                    color: "#F1FAFE"
                    font.pixelSize: 10
                    font.family: "Lato"
                    font.weight: Font.Bold
                    text: unUsed.focus ? STR.STR_QML_203 : STR.STR_QML_204
                }
            }
            QRCodeItem {
                id: qrCode
                width: 150
                height: width
                borderWitdh: 9
                anchors {
                    top: parent.top
                    topMargin: 113
                    horizontalCenter: parent.horizontalCenter
                }
                textInput: addressSelected.text
            }
            Rectangle {
                width: 273
                height: 40
                color: Qt.rgba(0, 0, 0, 0.1)
                anchors {
                    top: qrCode.bottom
                    topMargin: 8
                    horizontalCenter: parent.horizontalCenter
                }
                QText {
                    id: addressSelected
                    anchors.fill: parent
                    anchors.leftMargin: 5
                    anchors.rightMargin: 5
                    anchors.topMargin: 5
                    anchors.bottomMargin: 5
                    wrapMode: Text.WrapAnywhere
                    horizontalAlignment: Text.AlignHCenter
                    text: AppModel.walletInfo.walletAddress
                    anchors.centerIn: parent
                    font.pixelSize: 12
                    color: "#323E4A"
                }
            }
            Column {
                spacing: 16
                anchors {
                    top: qrCode.bottom
                    topMargin: 75
                    horizontalCenter: parent.horizontalCenter
                }
                QIconTextButton {
                    width: 150
                    height: 30
                    label: STR.STR_QML_205
                    icons: ["Copy_031F2B.png", "Copy_9CAEB8.png", "Copy_F1FAFE.png","Copy_F1FAFE.png"]
                    fontPixelSize: 12
                    iconSize: 16
                    type: eTypeB
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: {
                        QMLHandle.sendEvent(EVT.EVT_RECEIVE_COPY_ADDRESS, qrCode.textInput)
                    }
                }
                QIconTextButton {
                    width: 150
                    height: 30
                    label: STR.STR_QML_006
                    icons: ["visibility_031F2B.png","visibility_9CAEB8.png","visibility_F1FAFE.png","visibility_F1FAFE.png"]
                    fontPixelSize: 12
                    iconSize: 16
                    type: eTypeB
                    anchors.horizontalCenter: parent.horizontalCenter
                    onButtonClicked: { QMLHandle.sendEvent(EVT.EVT_RECEIVE_DISPLAY_ADDRESS, qrCode.textInput) }
                    enabled: AppModel.walletInfo.containsHWSigner
                    QTooltip {
                        anchors.left: parent.right
                        anchors.leftMargin: 8
                        anchors.verticalCenter: parent.verticalCenter
                        toolTip: STR.STR_QML_007
                    }
                }
            }
            Row {
                height: 36
                spacing: 8
                anchors {
                    top: qrCode.bottom
                    topMargin: 170
                    horizontalCenter: parent.horizontalCenter
                }
                QText {
                    text: STR.STR_QML_249
                    font.family: "Lato"
                    font.weight: Font.DemiBold
                    color: "#000000"
                    font.pixelSize: 10
                    font.capitalization: Font.AllUppercase
                    anchors.verticalCenter: parent.verticalCenter
                }
                QText {
                    text: AppModel.addressBalance + RoomWalletData.unitValue
                    font.family: "Montserrat"
                    font.weight: Font.DemiBold
                    color: "#031F2B"
                    font.pixelSize: 24
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
        Rectangle {
            id: displayAddressBusybox
            anchors.fill: parent
            color: Qt.rgba(255, 255, 255, 0.7)
            radius: 8
            MouseArea {anchors.fill: parent; onClicked: {}}
            visible: false
            Rectangle {
                id: busymask
                width: 500
                height: 250
                radius: 8
                visible: false
            }
            Rectangle {
                id: busyboxbg
                width: 500
                height: 250
                color: "#F1FAFE"
                radius: 8
                anchors.centerIn: parent
                visible: false
                Rectangle {
                    height: 4
                    width: parent.width
                    color: "#F6D65D"
                }
            }
            Rectangle {
                width: 500
                height: 250
                radius: 8
                anchors.centerIn: parent
            }
            OpacityMask {
                id: busyboxmask
                anchors.fill: busyboxbg
                source: busyboxbg
                maskSource: busymask
                QCloseButton {
                    anchors {
                        right: parent.right
                        rightMargin: 16
                        top: parent.top
                        topMargin: 16
                    }
                    onClicked: { displayAddressBusybox.visible = false }
                }
                Column {
                    spacing: 16
                    anchors.centerIn: parent
                    QBusyIndicator {
                        width: 70
                        height: 70
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    QText {
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.family: "Lato"
                        font.pixelSize: 15
                        font.weight: Font.Bold
                        text: STR.STR_QML_008
                    }
                    Rectangle {
                        width: 450
                        height: 60
                        color: Qt.rgba(0, 0, 0, 0.1)
                        anchors.horizontalCenter: parent.horizontalCenter
                        QText {
                            anchors.fill: parent
                            anchors.leftMargin: 5
                            anchors.rightMargin: 5
                            anchors.topMargin: 5
                            anchors.bottomMargin: 5
                            wrapMode: Text.WrapAnywhere
                            horizontalAlignment: Text.AlignHCenter
                            text: qrCode.textInput
                            anchors.centerIn: parent
                            font.pixelSize: 18
                            color: "#323E4A"
                        }
                    }
                }
            }
            DropShadow {
                anchors.fill: busyboxmask
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
                source: busyboxmask
            }
        }
        Connections {
            target: AppModel
            onStartDisplayAddress: {
                if(isOnTop) displayAddressBusybox.open()
                else displayAddressBusybox.close()
            }
            onFinishedDisplayAddress: {
                displayAddressBusybox.close()
            }
        }
    }
}
