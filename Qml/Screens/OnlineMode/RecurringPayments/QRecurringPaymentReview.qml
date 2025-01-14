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
import "../../../Components/customizes/QRCodes"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1123
    extraHeader: Item {}
    property bool isCancel: false
    property bool isAlert: false
    property string option: STR.STR_QML_881
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property int left_width: 344
    property int right_width: 344
    property bool enableFixedPrecision: AppSetting.enableFixedPrecision
    content: Item {
        Flickable {
            anchors {
                top: parent.top
                topMargin: 8
            }
            width: 728
            height: 480
            contentHeight: _col.childrenRect.height
            ScrollBar.vertical: ScrollBar { active: true }
            clip: true
            Column {
                id: _col
                spacing: 24
                Rectangle {
                    width: parent.width
                    height: 80
                    color:"#EAEAEA"
                    radius: 8
                    Row {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8
                        QIcon {
                            iconSize: 36
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/info-60px.svg"
                        }
                        QLato {
                            anchors.verticalCenter: parent.verticalCenter
                            text: {
                                var user = ClientController.user
                                var wallet = AppModel.walletInfo
                                var is_wallet_3_of_5 = (user.isByzantineUserPremier || user.isByzantineUserPro || user.isFinneyUserPro)
                                        && (wallet.walletM === 3 && wallet.walletN === 5)
                                return is_wallet_3_of_5 ? STR.STR_QML_1124_3_of_5 : STR.STR_QML_1124
                            }
                            width: 660
                            height: 56
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
                Item {
                    width: 728
                    height: _col_amount.childrenRect.height
                    Column {
                        id: _col_amount
                        spacing: 12
                        Row {
                            spacing: 36
                            QTextInputBoxTypeB {
                                id: _payment_name
                                label: STR.STR_QML_1072
                                boxWidth: left_width
                                boxHeight: 48
                                textInputted: payment.name
                                enabled: false
                            }
                            QTextInputBoxTypeB {
                                id: _amount
                                label: STR.STR_QML_214
                                boxWidth: right_width
                                boxHeight: 48
                                textInputted: {
                                    if (payment.payment_type === "FIXED_AMOUNT") {
                                        var str = "";
                                        var cur = payment.payment_payload.currency
                                        var val = payment.payment_payload.value
                                        if (cur === "sat") {
                                            str = qsTr("%1").arg(val.toFixed())
                                        }
                                        else if (cur === "USD") {
                                            str = qsTr("%1").arg(val.toFixed(2))
                                        }
                                        else {
                                            if (enableFixedPrecision) {
                                                str = qsTr("%1").arg(val.toFixed(8))
                                            }
                                            else {
                                                str = qsTr("%1").arg(val.toPrecision())
                                            }
                                        }
                                        return qsTr("%1 %2").arg(str).arg(cur)
                                    }
                                    else {
                                        var per = payment.payment_payload.valuePercent
                                        return qsTr("%1% of wallet balance (*)").arg(per.toFixed(4))
                                    }
                                }
                                enabled: false
                            }
                        }
                        QLato {
                            id: _des
                            visible: payment.payment_type === "PERCENTAGE"
                            width: 728
                            height: _des.paintedHeight
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            wrapMode: Text.WordWrap
                            font.pixelSize: 16
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                            text: qsTr("* %1").arg(payment.payment_payload.calculation_method === "RUNNING_AVERAGE" ? STR.STR_QML_1083 : STR.STR_QML_1085)
                        }
                    }
                }
                Item {
                    id: _frequency
                    width: 728
                    height: _repeat.childrenRect.height
                    readonly property var repeats: [
                        {id: "DAILY",             frequency: STR.STR_QML_1097 },
                        {id: "WEEKLY",            frequency: STR.STR_QML_1098 },
                        {id: "MONTHLY",           frequency: STR.STR_QML_1099 },
                        {id: "THREE_MONTHLY",     frequency: STR.STR_QML_1100 },
                        {id: "SIX_MONTHLY",       frequency: STR.STR_QML_1101 },
                        {id: "YEARLY",            frequency: STR.STR_QML_1102 },
                    ]
                    Row {
                        spacing: 36
                        QTextInputBoxTypeB {
                            id: _repeat
                            label: STR.STR_QML_1096
                            boxWidth: left_width
                            boxHeight: 48
                            textInputted: _frequency.repeats.find(function(e) {if (e.id === payment.frequency) return true; else return false}).frequency
                            enabled: false
                        }
                        Row {
                            width: right_width
                            spacing: 16
                            QTextInputBoxTypeB {
                                id: _start_date
                                label: STR.STR_QML_1104
                                boxWidth: 163
                                boxHeight: 48
                                textInputted: payment.start_date !== "" ? payment.start_date : "[MM/DD/YYYY]"
                                enabled: false
                            }
                            QTextInputBoxTypeB {
                                id: _end_date
                                label: STR.STR_QML_1105
                                boxWidth: 163
                                boxHeight: 48
                                textInputted: payment.end_date !== "" ? payment.end_date : STR.STR_QML_1106
                                enabled: false
                            }
                        }
                    }
                }
                Item {
                    width: 728
                    height: Math.max(_left.childrenRect.height, _right.childrenRect.height)
                    Row {
                        spacing: 36
                        QPaymentReviewLeft {
                            id: _left
                            width: left_width
                            height: 308
                        }
                        QPaymentReviewRight {
                            id: _right
                            width: right_width
                            height: 308
                        }
                    }
                }
            }
        }
    }
    onPrevClicked: {
        if (!isAlert) {
            requestBack()
        } else {
            closeTo(NUNCHUCKTYPE.WALLET_TAB)
        }
    }
    bottomRight: {
        if (isAlert) return _alert_payment
        else if (isCancel) return _cancel_payment
        else return _create_payment
    }

    Component {
        id: _create_payment
        QTextButton {
            width: 100
            height: 48
            label.text: STR.STR_QML_265
            label.font.pixelSize: 16
            type: eTypeE
            enabled: nextEnable
            onButtonClicked: {
                var _input = {
                    type: "create-recurring-payment",
                }
                QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
            }
        }
    }
    Component {
        id: _cancel_payment
        QTextButton {
            width: 177
            height: 48
            label.text: STR.STR_QML_1145
            label.font.pixelSize: 16
            type: eTypeE
            enabled: nextEnable
            onButtonClicked: {
                _info.open()
            }
        }
    }
    Component {
        id: _alert_payment
        Row {
            spacing: 12
            QTextButton {
                width: 70
                height: 48
                label.text: STR.STR_QML_946
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    _info.open()
                }
            }
            QTextButton {
                width: 254
                height: 48
                label.text: {
                    var alert = GroupWallet.dashboardInfo.alert
                    STR.STR_QML_1044.arg(alert.payload.pending_signatures).arg(alert.payload.pending_signatures > 1 ? "s":"")
                }
                label.font.pixelSize: 16
                type: eTypeE
                enabled: nextEnable
                onButtonClicked: {
                    var _input = {
                        type: "approve-recurring-payment",
                    }
                    QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
                }
            }
        }
    }

    QQrExportOneAddress {
        id: _exportAddress
        label: STR.STR_QML_1129
    }

    QConfirmYesNoPopup {
        id:_info
        contentText: _content.isCancel ? STR.STR_QML_1152 : STR.STR_QML_1159
        onConfirmNo: close()
        onConfirmYes: {
            close()
            if (_content.isCancel) {
                var _cancel = {
                    type: "cancel-recurring-payment",
                }
                QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _cancel)
            }
            else {
                var _input = {
                    type: "deny-recurring-payment",
                }
                QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
            }
        }
    }
}
