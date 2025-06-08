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
import Qt.labs.platform 1.1
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

QOnScreenContentTypeA {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1208
    extraHeader: Item {}
    property string signerDerivationPath: ""
    property string fingerPrint: ""
    property string keyName: ""
    property int wallet_type: NUNCHUCKTYPE.LEGACY
    property var signerInfo

    property string message_input: ""
    property string path_input: ""
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Item {
        Flickable {
            anchors.fill: parent
            clip: true
            contentHeight: _col.childrenRect.height
            ScrollBar.vertical: ScrollBar { active: true }
            Column {
                id:_col
                anchors {
                    top: parent.top
                    topMargin: 8
                }
                spacing: 24
                Column {
                    spacing: 16
                    QTextAreaBoxTypeA  {
                        id: _message_to_sign
                        boxWidth: 576
                        boxHeight: 120
                        label: STR.STR_QML_131
                        onTypingFinished: {
                            message_input = _message_to_sign.textInputted
                        }
                        enableLengthLimit:true
                        isValid: true
                        maxLength: 280
                        input.verticalAlignment: Text.AlignTop
                        input.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }
                    QTextInputBoxTypeB {
                        id: _path
                        label: STR.STR_QML_1211
                        boxWidth: 576
                        boxHeight: 48
                        textInputted: signerDerivationPath
                        onTextInputtedChanged: {
                            if(!_path.isValid){
                                _path.isValid = true
                                _path.errorText = ""
                            }
                            _path.showError = false;
                            path_input = _path.textInputted
                            if (path_input !== "") {
                                signerInfo.signerDerivationPath = path_input
                            }
                        }
                    }
                }
                QLine {
                    visible: signerInfo.address !== ""
                    width: 576
                }
                Column {
                    spacing: 16
                    visible: signerInfo.address !== ""
                    Column {
                        spacing: 4
                        QLato {
                            text: STR.STR_QML_066
                            font.weight: Font.Bold
                        }
                        QComboBox {
                            id: _address_type
                            width: 576
                            height: 48
                            current_id: _address_type.model.find(function(e) { if (e.id === wallet_type) return true; else return false } )
                            defaultValue: {
                                return { id: qsTr("%1").arg(AppSetting.currency),  displayName: STR.STR_QML_064 }
                            }
                            displayText: getDisplay().displayName
                            model: [
                                { id: NUNCHUCKTYPE.LEGACY,         displayName: STR.STR_QML_064 },
                                { id: NUNCHUCKTYPE.NESTED_SEGWIT,  displayName: STR.STR_QML_063 },
                                { id: NUNCHUCKTYPE.NATIVE_SEGWIT,  displayName: STR.STR_QML_062 },
                                { id: NUNCHUCKTYPE.TAPROOT,        displayName: STR.STR_QML_553 },
                            ]
                            textRole: "displayName"
                            onActivated: {
                                if (currentIndex >=0) {
                                    wallet_type = _address_type.getDisplay().id
                                }
                            }
                        }
                    }
                    QTextInputBoxTypeB {
                        id: _address
                        label: STR.STR_QML_1129
                        boxWidth: 576
                        boxHeight: 48
                        textInputted: signerInfo.address
                        enabled: false
                    }
                    QTextAreaBoxTypeA  {
                        id: _signature
                        boxWidth: 576
                        boxHeight: 120
                        label: STR.STR_QML_132
                        textInputted: signerInfo.signature
                        enableLengthLimit:true
                        isValid: true
                        maxLength: 280
                        input.verticalAlignment: Text.AlignTop
                        input.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        enabled: false
                    }
                    Item {
                        width: 576
                        height: 48
                        Row {
                            spacing: 16
                            QIconTextButton {
                                width: 280
                                height: 36
                                iconInRight: false
                                label: STR.STR_QML_1209
                                icons: ["copy-dark.svg", "copy-light.svg", "copy-dark.svg","copy-light.svg"]
                                fontPixelSize: 16
                                iconSize: 16
                                type: eTypeB
                                onButtonClicked: {
                                    ClientController.copyMessage(_signature.textInputted)
                                    AppModel.showToast(0, STR.STR_QML_1212, EWARNING.SUCCESS_MSG);
                                }
                            }
                            QIconTextButton {
                                width: 280
                                height: 36
                                iconInRight: false
                                label: STR.STR_QML_1210
                                icons: ["ExportFile.svg", "ExportFile.svg", "ExportFile.svg","ExportFile.svg"]
                                fontPixelSize: 16
                                iconSize: 16
                                type: eTypeB
                                onButtonClicked: {
                                    exportMessage.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                            + RoomWalletData.getValidFilename(keyName)
                                            + "-message_to_sign.txt"
                                    exportMessage.open()
                                }
                            }
                        }
                    }
                }

            }
        }
        QLine {
            width: parent.width + 36 *2
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
        }
    }
    bottomLeft: QTextButton {
        width: 81
        height: 48
        label.text: STR.STR_QML_035
        label.font.pixelSize: 16
        type: eTypeF
        onButtonClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    }
    bottomRight: QTextButton {
        width: 63
        height: 48
        label.text: STR.STR_QML_509
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            signerInfo.message = message_input
            _indicator.loading()
            AppModel.walletInfo.updateSignMessage(fingerPrint, wallet_type)
        }
    }
    FileDialog {
        id: exportMessage
        fileMode: FileDialog.SaveFile
        onAccepted: {
            AppModel.walletInfo.exportBitcoinSignedMessage(fingerPrint, exportMessage.currentFile, wallet_type)
        }
    }

    Connections {
        target: AppModel.walletInfo
        onSignMessageChanged: {
            _indicator.stop()
        }
    }

    QBusyIndicator {
        id:_indicator
        visible: false
        anchors.centerIn: _content
    }
}
