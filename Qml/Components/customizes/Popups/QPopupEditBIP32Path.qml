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
import "../../origins"
import "../../customizes"
import "../../customizes/Chats"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: editBIP32
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    signal enterText(var pathBip32)
    property var signer: {type:""; tag:""; signer_type: -1; derivationPath: ""}
    property string title: STR.STR_QML_1679
    property bool isShowListDevice: false
    property int idx: -1
    property string xfp: ""
    property string savePath: ""

    function showError(errorType) {
        _contentBip.contentItem.showError(errorType)
    }
    function closeEdit() {
        editBIP32.close()
    }
    function clearError() {
        _contentBip.contentItem.clearError()
    }
    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    background: Item{}
    Rectangle {
        width: popupWidth
        height: popupHeight
        radius: 24
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
        QOnScreenContentTypeB {
            id: _contentBip
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_1679
            onCloseClicked: closeEdit()
            content: Item {
                function showError(errorType) {
                    switch (errorType) {
                    case -1: { // Invalid BIP32 path format
                        newBip32Path.showError = true
                        newBip32Path.isValid = false
                        newBip32Path.errorText = STR.STR_QML_1673
                        break
                    }
                    case -2: { // Key not found. Please connect the key device to this computer and click Continue.
                        newBip32Path.showError = true
                        newBip32Path.isValid = true
                        newBip32Path.errorText = ""
                        break
                    }
                    case -3: { // Signer exists
                        newBip32Path.showError = true
                        newBip32Path.isValid = false
                        newBip32Path.errorText = STR.STR_QML_1685
                        break
                    }
                    default: break
                    }


                }

                function removePrefix(prefix) {
                    var str = newBip32Path.textInputted
                    if (newBip32Path.textInputted.startsWith(prefix)) {
                        str = str.replace(prefix, "");
                    }
                    return str
                }
                function bip32Path() {
                    return removePrefix(newBip32Path.fixedText)
                }
                function deviceIndex() {
                    return devicelist.currentIndex
                }

                function clearError() {
                    newBip32Path.showError = false
                    newBip32Path.isValid = true
                    newBip32Path.errorText = ""
                }

                Column {
                    width: parent.width
                    spacing: 24
                    Item {
                        width: parent.width
                        height: 64
                        Row {
                            anchors.fill: parent
                            anchors.margins: 8
                            spacing: 12
                            QCircleIcon {
                                bgSize: 48
                                icon.iconSize: 24
                                icon.typeStr: signer.type
                                icon.tag: signer.tag
                                icon.type: signer.signer_type
                                anchors.verticalCenter: parent.verticalCenter
                                color: "#F5F5F5"
                            }
                            Column {
                                spacing: 4
                                anchors.verticalCenter: parent.verticalCenter
                                QText {
                                    width: 146
                                    height: 20
                                    text: signer.name
                                    color: "#031F2B"
                                    font.weight: Font.Normal
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                }
                                QText {
                                    width: 146
                                    height: 20
                                    text: "XFP: " + signer.xfp
                                    color: "#595959"
                                    font.weight: Font.Normal
                                    font.capitalization: Font.AllUppercase
                                    font.family: "Lato"
                                    font.pixelSize: 12
                                }
                            }
                        }
                    }
                    QTextInputBoxTypeB {
                        label: STR.STR_QML_1680
                        boxWidth: 350
                        boxHeight: 48
                        isValid: true
                        enabled: false
                        titleFontSize: 12
                        disabledColor: "#F5F5F5"
                        textInputted: signer.derivationPath
                    }
                    Column {
                        width: parent.width
                        spacing: 4
                        QTextInputBoxTypeB {
                            id: newBip32Path
                            label: STR.STR_QML_1681
                            boxWidth: 350
                            boxHeight: 48
                            isValid: true
                            titleFontSize: 12
                            property string fixedText: STR.STR_QML_150 + " "
                            textInputted: fixedText + signer.derivationPath
                            onTextInputtedChanged: {
                                if (!textInputted.startsWith(fixedText)) {
                                    textInputted = fixedText + textInputted.slice(fixedText.length) + derivation32Path // Restore prefix
                                }
                                if(!newBip32Path.isValid) {
                                    newBip32Path.isValid = true
                                    newBip32Path.errorText = ""
                                }
                                newBip32Path.showError = false;
                            }
                        }
                        QLato {
                            visible: newBip32Path.showError && newBip32Path.errorText == ""
                            width: newBip32Path.boxWidth
                            height: 40
                            text: STR.STR_QML_1684
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 20
                            color: "#CF4018"
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Rectangle {
                        width: 539
                        height: 195
                        border.width: 1
                        border.color: "#EAEAEA"
                        radius: 8
                        visible: isShowListDevice
                        QRefreshButtonA {
                            width: 175
                            height: 48
                            label: STR.STR_QML_105
                            fontPixelSize: 16
                            color: "transparent"
                            border.color: "transparent"
                            anchors {
                                right: parent.right
                                rightMargin: 3
                                top:  parent.top
                                topMargin: 6
                            }
                            onButtonClicked: {
                                AppModel.startScanDevices(EVT.STATE_ID_SCR_SETUP_GROUP_WALLET)
                            }
                        }
                        QListView {
                            id: devicelist
                            property bool needPin: false
                            visible: devicelist.count
                            width: parent.width
                            height: Math.min(199, (devicelist.count*48) + ((devicelist.count-1)*16))
                            model: AppModel.deviceList
                            anchors {
                                left: parent.left
                                leftMargin: 12
                                top: parent.top
                                topMargin: 12
                            }
                            spacing: 16
                            currentIndex: -1
                            clip: true
                            interactive : devicelist.count > 3
                            ScrollBar.vertical: ScrollBar { active: true }
                            delegate: QDeviceDetailsDelegate {
                                isSelected: devicelist.currentIndex == index
                                device_name_type: device_type
                                device_fingerprint: device_master_fingerprint
                                onClickSelected: {
                                    devicelist.currentIndex = index
                                }
                            }
                        }
                    }
                }
            }
            onPrevClicked: closeEdit()
            bottomRight: QTextButton {
                width: 99
                height: 48
                label.text: STR.STR_QML_265
                label.font.pixelSize: 16
                type: eTypeE
                enabled: !isShowListDevice ? true : _contentBip.contentItem.deviceIndex() !== -1 || savePath !== _contentBip.contentItem.bip32Path()
                onButtonClicked: {
                    savePath = _contentBip.contentItem.bip32Path()
                    enterText(savePath)
                }
            }
        }
    }
}
