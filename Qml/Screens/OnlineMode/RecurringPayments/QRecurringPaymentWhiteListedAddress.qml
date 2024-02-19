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
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import QtQuick.Controls.Styles 1.4
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1068
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property bool hasAddress: false
    content: Item {
        property alias currentIndex: tabselect.currentIndex
        TabView {
            id: tabselect
            width: 610 + 12
            height: 480
            anchors {
                top: parent.top
                topMargin: 8
            }
            Tab {
                title: STR.STR_QML_1131
                Flickable {
                    height: 200
                    width: tabselect.width
                    contentHeight: _listAddress.height
                    clip: true
                    ScrollBar.vertical: ScrollBar { active: true }
                    QUseWhitelistedEnterAddresses {
                        id: _listAddress
                        width: tabselect.width - 12
                        isSelected: tabselect.currentIndex === 0
                    }
                    Component.onCompleted: {
                        hasAddress = false
                    }
                }
            }
            Tab {
                title: STR.STR_QML_1132
                Flickable {
                    height: 384
                    width: tabselect.width
                    contentHeight: _addressList.height
                    clip: true
                    ScrollBar.vertical: ScrollBar { active: true }
                    QUseWhitelistedBatchImport {
                        id: _addressList
                        width: tabselect.width - 12
                        isSelected: tabselect.currentIndex === 1
                    }
                    Component.onCompleted: {
                        hasAddress = false
                    }
                }
            }
            style: TabViewStyle {
                frameOverlap: 1
                tab: Rectangle {
                    implicitWidth: tabselect.width / 2
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
                            font.weight: Font.DemiBold
                            font.family: "Lato"
                        }
                    }
                    Rectangle {
                        color: "#031F2B"
                        implicitWidth: tabselect.width / 2
                        height: 2
                        anchors.bottom: parent.bottom
                        visible: styleData.selected
                    }
                }

                frame: Rectangle { color: "transparent" }
            }
        }
    }
    signal signalAddAddress()
    signal signalSetAddress(var index, var address)
    signal signalFinalAddress()
    onPrevClicked: requestBack()
    bottomRight: Row {
        spacing: 12
        QIconTextButton {
            width: 151
            height: 48
            label: STR.STR_QML_1133
            visible: _content.contentItem.currentIndex === 0
            type: eTypeD
            icons: ["add-dark.svg", "add-dark.svg", "add-dark.svg", "add-dark.svg"]
            onButtonClicked: {
                _content.signalAddAddress()
            }
        }
        QTextButton {
            width: 99
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            enabled: hasAddress
            onButtonClicked: {
                _content.signalFinalAddress()
            }
        }
    }
    QQrImportScanner {
        id: qrscaner
        property int addressRequestIndex: -1
        onTagFound: {
            if(qrscaner.addressRequestIndex !== -1){
                _content.signalSetAddress(qrscaner.addressRequestIndex, tag)
                qrscaner.addressRequestIndex = -1
            }
            qrscaner.close()
        }
    }
}
