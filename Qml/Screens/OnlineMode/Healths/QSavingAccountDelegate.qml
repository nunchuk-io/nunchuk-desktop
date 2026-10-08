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
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    border.color: "#DEDEDE"
    border.width: 1
    color: "transparent"
    radius: 12
    signal healthCheck
    signal requestHealthCheck
    signal viewHistoryClicked
    Item {
        anchors {
            fill: parent
            margins: 16
        }
        Column {
            anchors.left: parent.left
            height: parent.height
            width: 484
            spacing: 12
            Row {
                height: 18
                width: parent.width
                spacing: 12
                QBadge {
                    height: 16
                    text: GlobalData.getHealthStatusLabel(modelData.lastState)
                    color: GlobalData.getHealthStatusColor(modelData.lastState)
                    font.pixelSize: 10
                    anchors.verticalCenter: parent.verticalCenter
                }
                QButtonTextLink {
                    height: 18
                    fontPixelSize: 12
                    btnIcon.width: 18
                    btnIcon.height: 18
                    btnText.font.pixelSize: 12
                    btnText.font.underline: true
                    label: STR.STR_QML_973
                    icon: ["qrc:/Images/Images/history-24px.svg", "qrc:/Images/Images/history-24px.svg", "qrc:/Images/Images/history-24px.svg"]
                    onButtonClicked: {
                        viewHistoryClicked()
                    }
                }
            }
            QSignerDetailDelegate {
                height: 60
                width: parent.width
                typeStr: modelData.keyinfo.type
                tag: modelData.keyinfo.tag
                signerName: modelData.keyinfo.name
                accountIndex: parseInt(modelData.keyinfo.account_index)
                card_id_or_xfp: modelData.keyinfo.tapsigner ? modelData.keyinfo.tapsigner.card_id : modelData.keyinfo.xfp
                key_color: GlobalData.getHealthStatusColor(modelData.lastState)
            }
        }
        Column {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            spacing: 8
            QTextButton {
                width: 200
                height: 36
                label.text: STR.STR_QML_129
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: healthCheck()
                enabled: AppModel.walletInfo.myRole !== "FACILITATOR_ADMIN"
            }
            QTextButton {
                width: 200
                height: 36
                label.text: STR.STR_QML_974
                label.font.pixelSize: 16
                type: eTypeB
                visible: walletInfo.isByzantineWallet
                enabled: modelData.can_request_health_check
                onButtonClicked: requestHealthCheck()
            }
        }
    }

}
