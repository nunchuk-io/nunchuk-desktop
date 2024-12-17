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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Chats"
import "../../Components/customizes/QRCodes"
import "../../Components/customizes/Popups"
import "../../Components/customizes/Signers"
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

        TabView {
            id: tabselect
            width: 728
            height: 505

            property var unuselist: AppModel.walletInfo.walletunUsedAddressList
            property var usedlist: AppModel.walletInfo.walletusedAddressList
            property var count: [unuselist.length, usedlist.length]

            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 100
            }
            Tab {
                title: STR.STR_QML_1491
                property int count: 4
                Item {
                    anchors.top: parent.top
                    anchors.topMargin: 24
                    width: 728
                    height: 504
                    Row {
                        id: unusedContent
                        spacing: 28
                        anchors.horizontalCenter: parent.horizontalCenter
                        Rectangle {
                            width: 350
                            height: 420
                            radius: 12
                            border.width: 1
                            border.color: "#DEDEDE"
                            color: "#FFFFFF"

                            QListView {
                                id: unUsed
                                anchors.fill: parent
                                anchors.margins: 1
                                clip: true
                                model: AppModel.walletInfo.walletunUsedAddressList
                                currentIndex: 0
                                ScrollBar.vertical: ScrollBar { active: true }
                                delegate: Rectangle {
                                    width: unUsed.width
                                    height: 48
                                    color: (unUsed.currentIndex === index) ? "#EAEAEA" : "#FFFFFF"
                                    radius: 12
                                    QText {
                                        width: 314
                                        height: 28
                                        anchors.centerIn: parent
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        elide: Text.ElideRight
                                        text: modelData
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        font.pixelSize: 16
                                    }
                                    MouseArea {
                                        id: mouseUnUsed
                                        hoverEnabled: true
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            unUsed.currentIndex = index
                                            selectedAddress.text = modelData
                                            QMLHandle.sendEvent(EVT.EVT_RECEIVE_ADDRESS_BALANCE, modelData)
                                        }
                                    }
                                    Component.onCompleted: {
                                        if(index === unUsed.currentIndex) {
                                            selectedAddress.text = modelData
                                        }
                                    }
                                }
                            }
                        }
                        Rectangle {
                            width: 350
                            height: 420
                            radius: 12
                            border.width: 1
                            border.color: "#DEDEDE"
                            color: "#FFFFFF"

                            Column {
                                anchors.centerIn: parent
                                spacing: 12
                                Row {
                                    spacing: 6
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    QImage {
                                        width: 18
                                        height: 18
                                        source: "qrc:/Images/Images/CheckSquareOffset.png"
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    QText {
                                        height: 16
                                        text: STR.STR_QML_1494
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        font.pixelSize: 16
                                        font.weight: Font.Bold
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        anchors.verticalCenter: parent.verticalCenter
                                        MouseArea {
                                            id: mouseMarkused
                                            hoverEnabled: true
                                            anchors.fill: parent
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: {
                                                if(AppModel.walletInfo.markAddressUsed(selectedAddress.text)){
                                                    tabselect.currentIndex = 1
                                                }
                                            }
                                        }
                                    }
                                }
                                QText {
                                    width: 314
                                    height: 16
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    elide: Text.ElideRight
                                    text: AppModel.walletInfo.walletN === 1 ? ("Path: " + AppModel.walletInfo.addressPath(selectedAddress.text)) : ""
                                    color: "#595959"
                                    font.family: "Lato"
                                    font.pixelSize: 12
                                    visible: AppModel.walletInfo.walletN === 1
                                }
                                Rectangle {
                                    width: 160
                                    height: 160
                                    border.color: "#DEDEDE"
                                    radius: 12
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    QRCodeItem {
                                        id: qrCode
                                        width: 136
                                        height: 136
                                        borderWitdh: 1
                                        anchors.centerIn: parent
                                        textInput: selectedAddress.text
                                    }
                                }
                                QText {
                                    id: selectedAddress
                                    width: 314
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    wrapMode: Text.WrapAnywhere
                                    color: "#595959"
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                    lineHeight: 28
                                    lineHeightMode: Text.FixedHeight
                                }
                                QIconTextButton {
                                    width: 314
                                    height: 36
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    label: STR.STR_QML_205
                                    icons: [
                                        "Copy_F1FAFE.png",
                                        "Copy_F1FAFE.png",
                                        "Copy_031F2B.png",
                                        "Copy_F1FAFE.png"
                                    ]
                                    fontPixelSize: 12
                                    iconSize: 16
                                    type: eTypeE
                                    onButtonClicked: {
                                        QMLHandle.sendEvent(EVT.EVT_RECEIVE_COPY_ADDRESS, selectedAddress.text)
                                    }
                                }
                                QIconTextButton {
                                    width: 314
                                    height: 36
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    icons: [
                                        "visibility_031F2B.png",
                                        "visibility_9CAEB8.png",
                                        "visibility_F1FAFE.png",
                                        "visibility_F1FAFE.png"
                                    ]
                                    fontPixelSize: 12
                                    iconSize: 16
                                    type: eTypeB
                                    label: STR.STR_QML_1495
                                    onButtonClicked: {
                                        QMLHandle.sendEvent(EVT.EVT_RECEIVE_DISPLAY_ADDRESS, selectedAddress.text)
                                    }
                                    enabled: AppModel.walletInfo.containsHWSigner
                                    // QTooltip {
                                    //     anchors.left: parent.right
                                    //     anchors.leftMargin: 8
                                    //     anchors.verticalCenter: parent.verticalCenter
                                    //     toolTip: STR.STR_QML_007
                                    // }
                                }
                            }
                        }
                    }

                    QButtonMedium {
                        width: 222
                        height: 48
                        type: eOUTLINE_NORMAL
                        label: STR.STR_QML_202
                        fontPixelSize: 16
                        plusSize: 16
                        anchors {
                            right: unusedContent.right
                            top: unusedContent.bottom
                            topMargin: 36
                        }
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_BTN_GEN_NEW_ADDRESS)
                            unUsed.currentIndex = 0
                        }
                    }

                    Popup {
                        id: displayAddressBusybox
                        width: parent.width
                        height: parent.height
                        modal: true
                        focus: true
                        closePolicy: Popup.CloseOnEscape
                        background: Item{}
                        property string addrToVerify: ""
                        Rectangle {
                            id: displayAddressMask
                            width: 500
                            height: 250
                            radius: 8
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                            layer.enabled: true
                            layer.effect: OpacityMask {
                                maskSource: Rectangle {
                                    width: 500
                                    height: 250
                                    radius: 8
                                }
                            }
                            QCloseButton {
                                anchors {
                                    right: parent.right
                                    rightMargin: 16
                                    top: parent.top
                                    topMargin: 16
                                }
                                onClicked: {displayAddressBusybox.close()}
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
                                    font.pixelSize: 14
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
                                        text: selectedAddress.text
                                        anchors.centerIn: parent
                                        font.pixelSize: 18
                                        color: "#323E4A"
                                    }
                                }
                            }
                        }
                        DropShadow {
                            anchors.fill: displayAddressMask
                            horizontalOffset: 3
                            verticalOffset: 5
                            spread: 0
                            radius: 8
                            samples: 30
                            color: "#aa000000"
                            source: displayAddressMask
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
            Tab {
                title: STR.STR_QML_1492
                property int count: 5
                Item {
                    anchors.top: parent.top
                    anchors.topMargin: 12
                    width: 728
                    height: 516
                    Column {
                        id: usedContent
                        spacing: 12
                        anchors.horizontalCenter: parent.horizontalCenter
                        QText {
                            width: 728
                            lineHeight: 28
                            lineHeightMode: Text.FixedHeight
                            text: STR.STR_QML_1493
                            color: "#000000"
                            font.family: "Lato"
                            font.pixelSize: 16
                            wrapMode: Text.WordWrap
                        }

                        Rectangle {
                            width: 728
                            height: 364
                            radius: 12
                            border.width: 1
                            border.color: "#DEDEDE"
                            color: "#FFFFFF"
                            QListView {
                                id: used
                                anchors.fill: parent
                                anchors.margins: 1
                                clip: true
                                currentIndex: 0
                                model: AppModel.walletInfo.walletusedAddressList
                                ScrollBar.vertical: ScrollBar { active: true }
                                delegate: Item {
                                    width: used.width
                                    height: 132
                                    Row {
                                        anchors.fill: parent
                                        anchors.margins: 18
                                        spacing: 24
                                        Rectangle {
                                            width: 96
                                            height: 96
                                            radius: 12
                                            border.width: 1
                                            border.color: "#DEDEDE"
                                            color: "#FFFFFF"
                                            QRCodeItem {
                                                width: 72
                                                height: 72
                                                borderWitdh: 1
                                                anchors.centerIn: parent
                                                textInput: modelData
                                            }
                                        }
                                        Column {
                                            width: 572
                                            spacing: 4
                                            anchors.verticalCenter: parent.verticalCenter
                                            QText {
                                                width: 728
                                                lineHeight: 20
                                                lineHeightMode: Text.FixedHeight
                                                text: modelData
                                                color: "#031F2B"
                                                font.family: "Lato"
                                                font.pixelSize: 16
                                            }
                                            QText {
                                                width: 728
                                                lineHeight: 20
                                                lineHeightMode: Text.FixedHeight
                                                text: qsTr("Address balance: %1 %2").arg(AppModel.walletInfo.addressBalance(modelData)).arg((AppSetting.unit === NUNCHUCKTYPE.SATOSHI) ? "sat" : "BTC")
                                                color: "#031F2B"
                                                font.family: "Lato"
                                                font.pixelSize: 16
                                                font.weight: Font.Bold
                                            }
                                            QText {
                                                width: 728
                                                lineHeight: 16
                                                lineHeightMode: Text.FixedHeight
                                                color: "#595959"
                                                font.family: "Lato"
                                                font.pixelSize: 12
                                                text: AppModel.walletInfo.walletN === 1 ? ("Path: " + AppModel.walletInfo.addressPath(modelData)) : ""
                                                visible: AppModel.walletInfo.walletN === 1
                                            }
                                        }
                                    }
                                    Rectangle {
                                        width: 692
                                        height: 1
                                        color: "#DEDEDE"
                                        anchors.bottom: parent.bottom
                                        anchors.horizontalCenter: parent.horizontalCenter
                                    }
                                }
                            }
                        }
                    }

                    QButtonMedium {
                        width: 222
                        height: 48
                        type: eOUTLINE_NORMAL
                        label: STR.STR_QML_202
                        fontPixelSize: 16
                        plusSize: 16
                        anchors {
                            right: usedContent.right
                            top: usedContent.bottom
                            topMargin: 36
                        }
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_BTN_GEN_NEW_ADDRESS)
                            tabselect.currentIndex = 0
                        }
                    }
                }
            }
            style: TabViewStyle {
                frameOverlap: 1
                tab: Rectangle {
                    implicitWidth: 364
                    implicitHeight: 48
                    color: "transparent"
                    Row{
                        spacing: 4
                        anchors.centerIn: parent
                        QText {
                            id: txt
                            anchors.verticalCenter: parent.verticalCenter
                            text: styleData.title
                            color: styleData.selected ? "#031F2B" : "#839096"
                            font.pixelSize: 16
                            font.weight: styleData.selected ? Font.Bold : Font.Normal
                            font.family: "Lato"
                        }
                        Rectangle {
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#EAEAEA"
                            implicitWidth: 24
                            implicitHeight: 24
                            radius: 20
                            // visible: styleData.index === 1
                            QText {
                                anchors.centerIn: parent
                                text: tabselect.count[styleData.index]
                                color: "#031F2B"
                                font.pixelSize: 12
                                font.weight: Font.Normal
                                font.family: "Lato"
                            }
                        }
                    }
                    Rectangle {
                        color: "#031F2B"
                        implicitWidth: 364
                        height: 2
                        anchors.bottom: parent.bottom
                        visible: styleData.selected
                    }
                }

                frame: Rectangle { color: "transparent" }
            }
        }
    }
}
