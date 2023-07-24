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
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    readonly property int _INFORMATION: 0
    readonly property int _REFRESH_DEVICE: 1
    readonly property int _LOADING: 2
    readonly property int _SUCCESSFULLY: 3
    QOnScreenContentTypeA {
        visible: AppModel.addSignerWizard === _INFORMATION
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_814
        onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        content: Item {
            Row {
                spacing: 36
                Rectangle {
                    width: 346
                    height: 512
                    radius: 24
                    color: "#D0E2FF"
                    QImage {
                        width: 346
                        height: 300
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/Images/Images/trezor-illustration.svg"
                    }
                }
                Item {
                    width: 346
                    height: 512
                    Column {
                        width: parent.width
                        spacing: 24
                        QLato {
                            width: parent.width
                            text: STR.STR_QML_817
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                        Repeater {
                            id: _guide
                            width: parent.width
                            readonly property var content_map: [
                                {height: 84, headline:STR.STR_QML_818, content: STR.STR_QML_829 , icon: "qrc:/Images/Images/1.Active.svg" },
                                {height: 112, headline:STR.STR_QML_095, content: STR.STR_QML_820 , icon: "qrc:/Images/Images/2.Active.svg" },
                            ]
                            model: content_map.length
                            Rectangle {
                                property var _item: _guide.content_map[index]
                                width: 346
                                height: _item.height
                                Row {
                                    spacing: 12
                                    QImage {
                                        id: _ico
                                        width: 24
                                        height: 24
                                        source: _item.icon
                                    }
                                    Column {
                                        width: 310
                                        height: _item.height
                                        spacing: 4
                                        QText {
                                            width: 310
                                            text: _item.headline
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            font.pixelSize: 16
                                            font.weight: Font.DemiBold
                                            horizontalAlignment: Text.AlignLeft
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        QText {
                                            width: 310
                                            text: _item.content
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            font.pixelSize: 16
                                            lineHeightMode: Text.FixedHeight
                                            lineHeight: 28
                                            wrapMode: Text.WordWrap
                                            horizontalAlignment: Text.AlignLeft
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }

        onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        onNextClicked: {
            AppModel.addSignerWizard = _REFRESH_DEVICE
        }
    }

    readonly property string _TREZOR: "Trezor"
    readonly property string _TREZOR_TYPE: "trezor"
    QOnScreenContentTypeA {
        visible: AppModel.addSignerWizard === _REFRESH_DEVICE
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_814
        onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        content: Item {
            Item {
                width: 529
                QLato {
                    id: text
                    width: parent.width
                    text: STR.STR_QML_830
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QImage {
                    id: nodevice
                    width: parent.width
                    visible: AppModel.deviceList.count === 0
                    anchors {
                        left: text.left
                        top: text.bottom
                        topMargin: 8
                    }
                    source: "qrc:/Images/Images/Signer_Level2.png"
                    QRefreshButton {
                        width: 160
                        height: 48
                        label: STR.STR_QML_105
                        fontPixelSize: 16
                        borderColor: ["transparent", "transparent", "transparent"]
                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                        }
                        onButtonClicked: scanDevice()
                    }
                }
                Rectangle {
                    visible: AppModel.deviceList.count !== 0
                    anchors {
                        left: text.left
                        top: text.bottom
                        topMargin: 8
                    }
                    border.width: 1
                    border.color: "#EAEAEA"
                    width: 539
                    height: 464
                    radius: 8
                    QRefreshButton {
                        width: 160
                        height: 48
                        label: STR.STR_QML_105
                        fontPixelSize: 16
                        borderColor: ["transparent", "transparent", "transparent"]
                        anchors {
                            right: parent.right
                            top: parent.top
                            topMargin: 6
                        }
                        onButtonClicked: scanDevice()
                    }
                    QListView {
                        id: devicelist
                        property bool needPin: false
                        visible: devicelist.count
                        width: parent.width
                        height: Math.min(230, (devicelist.count*44) + ((devicelist.count-1)*8))
                        model: AppModel.deviceList
                        anchors {
                            left: parent.left
                            leftMargin: 12
                            top: parent.top
                            topMargin: 12
                        }
                        spacing: 8
                        currentIndex: -1
                        clip: true
                        interactive: devicelist.count > 3
                        ScrollBar.vertical: ScrollBar { active: true }
                        delegate: Item {
                            width: 342
                            height: 44
                            visible: device_type === _TREZOR_TYPE
                            Rectangle {
                                id: rect
                                anchors.horizontalCenter: parent.horizontalCenter
                                width: parent.width - 2
                                height: 40
                                color: "transparent"
                                QImage {
                                    anchors {
                                        left: parent.left
                                        leftMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                    source: index == devicelist.currentIndex ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                                }
                                Column {
                                    width: 290
                                    height: 37
                                    anchors {
                                        left: parent.left
                                        leftMargin: 48
                                        verticalCenter: parent.verticalCenter
                                    }
                                    QText {
                                        width: parent.width
                                        height: 21
                                        font.family: "Montserrat"
                                        font.pixelSize: 14
                                        color: "#031F2B"
                                        font.weight: Font.DemiBold
                                        text: _TREZOR
                                    }
                                    QText {
                                        width: parent.width
                                        height: 16
                                        font.family: "Lato"
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        text: "XFP: " + device_master_fingerprint
                                        font.capitalization: Font.AllUppercase
                                    }
                                }
                            }
                            Rectangle {
                                anchors.fill: parent
                                visible: !device_usable_to_add
                                color: Qt.rgba(255, 255, 255, 0.5)
                            }
                            MouseArea {
                                anchors.fill: parent
                                enabled: device_usable_to_add
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    devicelist.currentIndex = index
                                    signerName = _TREZOR
                                }
                            }
                        }
                    }
                }
            }
            property string signerName: ""
            function scanDevice() {
                QMLHandle.sendEvent(EVT.EVT_SCAN_TREZOR_DEVICE_REQUEST)
            }
            function addDevice() {
                if(devicelist.currentIndex !== -1){
                    var masterSignerObj = { "signerNameInputted"    : signerName,
                        "deviceIndexSelected"   : devicelist.currentIndex};
                    QMLHandle.sendEvent(EVT.EVT_ADD_TREZOR_DEVICE_REQUEST, masterSignerObj)
                }
            }
        }
        onPrevClicked: {
            AppModel.addSignerWizard = _INFORMATION
        }
        onNextClicked: {
            contentItem.addDevice()
        }
    }

    QOnScreenContent {
        visible: AppModel.addSignerWizard === _LOADING
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        enableHeader: false
        onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        content: Item {
            Column {
                width: 400
                height: 56
                anchors.centerIn: parent
                spacing: 16
                QProgressbarTypeA {
                    id: progresBar
                    percentage: AppModel.addSignerPercentage
                }
                QLato{
                    font.weight: Font.Bold
                    font.pixelSize: 20
                    text: STR.STR_QML_831
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
        Connections {
            target: AppModel
            onAddSignerPercentageChanged:{
                if (AppModel.addSignerPercentage === 100) {
                    AppModel.addSignerWizard = _SUCCESSFULLY
                }
            }
        }
    }

    QOnScreenContent {
        visible: AppModel.addSignerWizard === _SUCCESSFULLY
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: ""
        onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        content: Item {
            Item {
                anchors.centerIn: parent
                width: 400
                height: 228
                Column {
                    width: parent.width
                    spacing: 24
                    Rectangle {
                        width: 96;height: 96;
                        radius: 48
                        color: "#A7F0BA"
                        anchors.horizontalCenter: parent.horizontalCenter
                        QImage {
                            anchors.centerIn: parent
                            width: 60; height: 60;
                            source: "qrc:/Images/Images/check-dark.svg"
                        }
                    }
                    QLato {
                        width: parent.width
                        height: 40
                        text: STR.STR_QML_832
                        font.pixelSize: 32
                        font.weight: Font.DemiBold
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    QLato {
                        width: parent.width
                        height: 28
                        text: STR.STR_QML_828
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
        bottomRight: Row {
            spacing: 12
            QTextButton {
                width: 120
                height: 48
                label.text: STR.STR_QML_777
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
            }
        }
    }

    QPopupInfo{
        id:_info1
        contentText: STR.STR_QML_840
        onGotItClicked: {
            close()
        }
    }

    Connections {
        target: UserWallet
        onAddHardwareAlert:{
            _info1.open()
        }
    }
}
