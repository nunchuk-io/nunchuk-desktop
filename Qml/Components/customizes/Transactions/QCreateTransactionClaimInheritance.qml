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
    readonly property var iconChecked: ["qrc:/Images/Images/Checked_n.png", "qrc:/Images/Images/Checked_d.png"]
    readonly property var iconUnChecked: ["qrc:/Images/Images/UnChecked_n.png", "qrc:/Images/Images/UnChecked_d.png"]
    property var transactionInfo
    signal requestSort(var sortData)
    signal signalDraftTransaction(var msg)
    signal signalCreateTransaction(var msg)
    property alias manualFeeValid: manualfee.validInput
    Row {
        spacing: 30
        Rectangle {
            width: 350
            height: 480
            radius: 12
            border.color: "#EAEAEA"
            color: "#FFFFFF"
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 350
                    height: 480
                    radius: 12
                }
            }
            Flickable {
                anchors.fill: parent
                flickableDirection: Flickable.VerticalFlick
                clip: true
                interactive: contentHeight > height
                contentHeight: contentDisp.height
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: contentDisp
                    width: parent.width
                    spacing: 12
                    anchors.horizontalCenter: parent.horizontalCenter
                    QSendToAddressBlock {
                        id: sendToAddress
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    QTransactionNoteBlock {
                        id: transationNote
                        visible: transactionInfo.hasChange
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
        Item {
            width: 350
            height: 480
            Column {
                anchors.fill: parent
                spacing: 12
                QSwitchTypeIconLabel {
                    id: manualfeesetting
                    width: parent.width
                    height: 36
                    label: STR.STR_QML_225
                    icon: "qrc:/Images/Images/signing-policy-dark.svg"
                    enabled: false
                    value: true
                }
                Column {
                    visible: manualfeesetting.getValue()
                    width: parent.width
                    spacing: 12
                    QCheckboxTooltip {
                        id: subtract
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: STR.STR_QML_226
                        tooltip: STR.STR_QML_227
                        checked: transactionInfo.subtractFromFeeAmount
                        rightOfParent: true
                        pointerPositionIndex: 6
                        pointerPositionRatio: 10
                        onButtonClicked: {
                            requestDraftTransaction()
                        }
                    }
                    QManualFeeRate {
                        id: manualfee
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    QCheckboxTooltip {
                        id: antisnipfee
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: STR.STR_QML_1729
                        tooltip: STR.STR_QML_1730
                        pointerPositionIndex: 6
                        pointerPositionRatio: 10
                        checked: AppSetting.enableAntiFeeSniping
                        onButtonClicked: {
                            // DON'T NEED MAKE DRAFT
                        }
                    }
                }
            }
        }
    }

    function requestDraftTransaction(){
        var msg = { "subtractFromFeeAmout"   : subtract.selected,
                    "feeRate"                : manualfee.textOutput,
                    "manualFee"              : manualfee.selected,
                    "manualOutput"           : false,
                    "antiFeeSnipping"        : false, // always false
        };
        signalDraftTransaction(msg)
    }
    function requestCreateTransaction(){
        var msg = { "subtractFromFeeAmout"   : subtract.selected,
                    "feeRate"                : manualfee.textOutput,
                    "manualFee"              : manualfee.selected,
                    "manualOutput"           : false,
                    "antiFeeSnipping"        : antisnipfee.checked,
        };
        signalCreateTransaction(msg)
    }
}
