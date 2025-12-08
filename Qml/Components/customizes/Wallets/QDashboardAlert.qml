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
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.3
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    width: parent.width
    height: childrenRect.height
    color: "#FFFFFF"
    radius: 8
    signal clickView()
    signal clickDismiss()
    Loader {
        id: loader
        width: parent.width
        sourceComponent: health
    }
    Component {
        id: health
        Item {
            width: parent.width
            height: childrenRect.height + 32
            Column {
                width: 650
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 16
                spacing: 6
                QLato {
                    width: parent.width
                    text: modelData.title
                    font.pixelSize: 12
                    font.bold: true
                    wrapMode: Text.WordWrap
                    visible: text !== ""
                }
                QLato {
                    width: parent.width
                    text: modelData.body
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    visible: text !== ""
                }
                QLato {
                    width: parent.width
                    text: modelData.created_time_millis
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                    visible: text !== ""
                }
            }
            QTextButton {
                visible: modelData.viewable
                width: 69
                height: 48
                label.text: STR.STR_QML_935
                label.font.pixelSize: 16
                type: eTypeE
                anchors {
                    right: parent.right
                    rightMargin: 24
                    verticalCenter: parent.verticalCenter
                }
                enabled: modelData.type === AlertType.GROUP_WALLET_PENDING ||
                         modelData.type === AlertType.WALLET_PENDING ||
                         modelData.type === AlertType.GROUP_WALLET_SETUP ||
                         modelData.type === AlertType.HEALTH_CHECK_REQUEST ||
                         modelData.type === AlertType.HEALTH_CHECK_PENDING ||
                         modelData.type === AlertType.UPDATE_SERVER_KEY ||
                         modelData.type === AlertType.REQUEST_INHERITANCE_PLANNING ||
                         modelData.type === AlertType.UPDATE_INHERITANCE_PLAN ||
                         modelData.type === AlertType.CANCEL_INHERITANCE_PLAN ||
                         modelData.type === AlertType.CREATE_INHERITANCE_PLAN ||
                         modelData.type === AlertType.REQUEST_INHERITANCE_PLANNING_APPROVED ||
                         modelData.type === AlertType.KEY_RECOVERY_REQUEST ||
                         modelData.type === AlertType.KEY_RECOVERY_APPROVED ||
                         modelData.type === AlertType.CREATE_INHERITANCE_PLAN_SUCCESS ||
                         modelData.type === AlertType.RECURRING_PAYMENT_REQUEST ||
                         modelData.type === AlertType.RECURRING_PAYMENT_CANCELATION_PENDING||
                         modelData.type === AlertType.TRANSACTION_SIGNATURE_REQUEST ||
                         modelData.type === AlertType.UPDATE_SECURITY_QUESTIONS ||
                         modelData.type === AlertType.CHANGE_EMAIL_REQUEST ||
                         modelData.type === AlertType.HEALTH_CHECK_REMINDER ||
                         modelData.type === AlertType.KEY_REPLACEMENT_PENDING ||
                         modelData.type === AlertType.KEY_REPLACEMENT_COMPLETED ||
                         modelData.type === AlertType.TRANSFER_FUNDS ||
                         modelData.type === AlertType.SETUP_INHERITANCE_PLAN ||
                         modelData.type === AlertType.BACKUP_WALLET ||
                         modelData.type === AlertType.CHANGE_TIMELOCK_TYPE ||
                         modelData.type === AlertType.KEY_TIMELOCK_UPDATE_PENDING
                onButtonClicked: {
                    clickView()
                }
            }
            QTextButton {
                visible: !modelData.viewable
                width: 76
                height: 48
                label.text: STR.STR_QML_955
                label.font.pixelSize: 16
                type: eTypeE
                anchors {
                    right: parent.right
                    rightMargin: 24
                    verticalCenter: parent.verticalCenter
                }
                onButtonClicked: {
                    clickDismiss()
                }
            }
        }
    }
}
