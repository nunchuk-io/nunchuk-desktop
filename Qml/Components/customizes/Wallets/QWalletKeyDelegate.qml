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
import QtQuick 2.12
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

Item {
    width: 326
    height: 48
    property string signerName: "value"
    property string signerXFP: "value"
    property string lastHealthCheck: "value"
    property int    signerType: 0
    property string deviceType: ""
    property string tagType: ""
    property string card_id: ""
    property int    accountIndex: 0

    signal viewPoliciesRequest()

    Row {
        id: roomDelegateContent
        spacing: 8
        Rectangle {
            id: _keyIcon
            width: 48
            height: 48
            radius: width
            color: "#F5F5F5"
            anchors.verticalCenter: parent.verticalCenter
            QSignerDarkIcon {
                iconSize: 24
                anchors.centerIn: parent
                device_type: deviceType
                type: signerType
                tag: tagType
            }
        }
        Column {
            width: 170
            spacing: 4
            anchors.verticalCenter: parent.verticalCenter
            QText{
                width: parent.width
                height: 28
                font.family: "Lato"
                text: signerName
                font.pixelSize: 16
                color: "#031F2B"
                elide: Text.ElideRight
            }
            Row {
                spacing: 8
                QText {
                    height: 16
                    visible: (signerXFP !== "" || card_id !== "")
                    font.family: "Lato"
                    font.pixelSize: 12
                    color: "#031F2B"
                    text: {
                        if (signerType === NUNCHUCKTYPE.NFC) {
                            var textR = card_id.substring(card_id.length - 5,card_id.length).toUpperCase()
                            return "Card ID: ••" + textR
                        } else {
                            return "XFP: " + signerXFP.toUpperCase()
                        }
                    }
                }
                Rectangle {
                    width: typesigner.width + 10
                    height: 16
                    color: "#EAEAEA"
                    visible: signerType !== NUNCHUCKTYPE.SERVER
                    radius: 8
                    QText {
                        id: typesigner
                        font.family: "Lato"
                        color: "#031F2B"
                        font.pixelSize: 10
                        anchors.centerIn: parent
                        font.weight: Font.Bold
                        text: GlobalData.signers(signerType)
                    }
                }
                Rectangle {
                    width: accttext.width + 10
                    height: 16
                    color: "#EAEAEA"
                    visible: (accountIndex > 0) && (signerType !== NUNCHUCKTYPE.SERVER)
                    radius: 8
                    QText {
                        id: accttext
                        font.family: "Lato"
                        color: "#031F2B"
                        font.pixelSize: 10
                        anchors.centerIn: parent
                        font.weight: Font.Bold
                        text: qsTr("Acct %1").arg(accountIndex)
                    }
                }
            }
        }
    }
    QTextButton {
        width: 108
        height: 36
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        label.text: STR.STR_QML_1348
        label.font.pixelSize: 12
        type: eTypeB
        enabled: !AppModel.walletInfo.isLocked
        visible: signerType === NUNCHUCKTYPE.SERVER && !AppModel.walletInfo.isReplaced
        onButtonClicked: {
            viewPoliciesRequest()
        }
    }
}
