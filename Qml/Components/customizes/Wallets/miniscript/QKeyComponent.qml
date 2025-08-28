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
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Signers"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Item {
    property alias signerData: dataSingle
    QSingleSignerData {
        id: dataSingle
    }
    property var showEdit: true
    signal bip32PathClick(string key, string xfp, string path)

    Row {
        anchors.fill: parent
        spacing: 4
        Loader {
            sourceComponent: if(miniscript.keyObj != null && miniscript.keyObj != undefined && typeof(tmpColors)!= 'undefined') {
                colorIcon
            } else {
                normalIcon
            }
            anchors {
                top: parent.top
                topMargin: 8
            }
        }
        Component {
            id: normalIcon
            QIcon {
                iconSize: 20
                source: "qrc:/Images/Images/key-dark.svg"
            }
        }
        Component {
            id: colorIcon
            Rectangle {
                property var idx: sandbox.getIndexColor(miniscript.keyObj)
                property var colorData: tmpColors[idx]
                border.width: 1
                border.color: "#DEDEDE"
                color: colorData.colorStr
                radius: 20
                width: 20
                height: 20
                QIcon {
                    iconSize: 11
                    anchors.centerIn: parent
                    source: colorData.url
                }
            }
        }
        
        Column {
            id: column
            anchors{ 
                top: parent.top
                topMargin: 8
            }
            width: parent.width
            spacing: 2
            Row {
                width: parent.width
                height: 20
                spacing: 4
                QLato {
                    width: paintedWidth
                    height: 20
                    text: miniscript.firstLine
                    visible: miniscript.firstLine !== ""
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    width: paintedWidth
                    height: 20
                    text: {
                        if(miniscript.keyObj != null && miniscript.keyObj != undefined) {
                            return miniscript.keyObj.singleSigner_name
                        } else if (miniscript.key !== "") {
                            return qsTr("Key %1").arg(miniscript.key)
                        } else {
                            return qsTr("Unknown Key")
                        }                        
                    }
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
        
            QLato {
                width: parent.width
                visible: miniscript.keyObj != null && miniscript.keyObj != undefined
                text: {
                    if (miniscript.keyObj.single_signer_device_cardid !== "") {
                        var card_id_text = miniscript.keyObj.single_signer_device_cardid
                        var textR = card_id_text.substring(card_id_text.length - 5, card_id_text.length).toUpperCase()
                        return "Card ID: ••" + textR
                    } else {
                        "XFP: " + miniscript.keyObj.singleSigner_masterFingerPrint
                    }
                }
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.capitalization: Font.AllUppercase
                font.pixelSize: 12
            }
            QLato {
                width: paintedWidth + 4 + 12
                visible: miniscript.keyObj != null && miniscript.keyObj != undefined
                text: qsTr("BIP32 path: %1").arg(miniscript.keyObj.singleSigner_derivationPath)
                color: miniscript.keyStrCount > 1 ? "#CF4018": "#757575"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 12
                font.underline: _icon.visible
                QIcon {
                    id: _icon
                    iconSize: 12
                    source: "qrc:/Images/Images/editBIP32.svg"
                    anchors {
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                    }
                    visible: (miniscript.keyObj.single_signer_type === NUNCHUCKTYPE.SOFTWARE ||
                            miniscript.keyObj.single_signer_type === NUNCHUCKTYPE.HARDWARE) && showEdit
                }
                MouseArea {
                    visible: _icon.visible
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        bip32PathClick(miniscript.key, miniscript.keyObj.singleSigner_masterFingerPrint, miniscript.keyObj.singleSigner_derivationPath)
                    }
                }
            }
        }
    }
}