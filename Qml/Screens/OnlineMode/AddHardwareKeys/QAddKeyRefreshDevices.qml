/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    property string title: ""
    property int    state_id: EVT.STATE_ID_SCR_ADD_HARDWARE
    property string selected_xfp: ""
    property var refreshDeviceList: draftWallet.refreshDeviceList
    property alias mDevicelist: devicelist
    Item {
        width: 539
        height: 500
        Column {
            anchors.fill: parent
            spacing: 8
            QLato {
                width: parent.width
                height: 40
                text: title
                font.pixelSize: 16
                font.weight: Font.Bold
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QDashRectangle {
                id: nodevice
                width: parent.width
                height: 60
                visible: devicelist.count === 0
                radius: 8
                borderWitdh: 2
                borderColor: "#EAEAEA"
                QLato {
                    width: parent.width
                    height: 40
                    anchors {
                        left: parent.left
                        leftMargin: 12
                        verticalCenter: parent.verticalCenter
                    }
                    text: STR.STR_QML_999
                    color: "#757575"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QRefreshButtonA {
                    width: 174
                    height: 48
                    label: STR.STR_QML_105
                    fontPixelSize: 16
                    color: "transparent"
                    border.color: "transparent"
                    anchors {
                        right: parent.right
                        verticalCenter: parent.verticalCenter
                    }
                    onButtonClicked: scanDevice()
                }
            }
            Rectangle {
                visible: devicelist.count !== 0
                border.width: 1
                border.color: "#EAEAEA"
                width: 539
                height: 452
                radius: 8
                QRefreshButtonA {
                    width: 174
                    height: 48
                    label: STR.STR_QML_105
                    fontPixelSize: 16
                    color: "transparent"
                    border.color: "transparent"
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
                    model: refreshDeviceList
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
                        visible: device_type === _HARDWARE_TYPE
                        Rectangle {
                            id: rect
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: parent.width - 2
                            height: 40
                            color: "transparent"
                            QIcon {
                                anchors {
                                    left: parent.left
                                    leftMargin: 0
                                    verticalCenter: parent.verticalCenter
                                }
                                iconSize: 24
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
                                    text: _HARDWARE
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
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                devicelist.currentIndex = index
                                selected_xfp = device_master_fingerprint
                                signerName = _HARDWARE
                            }
                        }
                    }
                }
            }
        }
    }

    property string signerName: ""
    function scanDevice() {
        AppModel.startScanDevices(state_id)
    }
    function addDevice() {
        if(devicelist.currentIndex !== -1){
            var masterSignerObj = {
                "signerNameInputted"    : signerName,
                "deviceIndexSelected"   : devicelist.currentIndex
            };
            QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_DEVICE_REQUEST, masterSignerObj)
        }
    }
    function isEnable() {
        return devicelist.currentIndex !== -1
    }
}
