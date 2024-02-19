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
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Chats"
import "./../../../customizes/Texts"
import "./../../../customizes/Buttons"
import "../../../RightPannel/Service/Common"
import "../../../../../localization/STR_QML.js" as STR

Item {
    property var serverKeyInfo: ServiceSetting.walletInfo.serverKeyInfo
    property var policies: serverKeyInfo.policies
    Connections {
        target: ServiceSetting.servicesTag
        onServerKeyVerifyPasswordAlert: {
            ServiceSetting.optionIndex = _PLATFORM_KEY_CO_SIGNING_POLICIES
        }
        onSecurityQuestionChanged: {
            if (ServiceSetting.optionIndex === _PLATFORM_KEY_CO_SIGNING_POLICIES) {
                _Security.action = function() {
                    QMLHandle.sendEvent(EVT.EVT_CO_SIGNING_SERVER_KEY_UPDATE_SUCCEED)
                }
                _Security.open();
            }
        }
        onThereNoAssistedWalletAlert: {
            _info1.contentText = STR.STR_QML_839
            _info1.open();
        }
    }
    Connections {
        target: serverKeyInfo
        onSecurityQuestionClosed: {
            if (ServiceSetting.optionIndex === _PLATFORM_KEY_CO_SIGNING_POLICIES) {
                _Security.close()
            }
        }
        onServerKeyDummyTransactionAlert: {
            if (ServiceSetting.optionIndex === _PLATFORM_KEY_CO_SIGNING_POLICIES) {
                _info1.contentText = STR.STR_QML_806
                _info1.open();
            }
        }
        onKeyCoSigningChanged: {
            _currency.loadCompleted()
            _interal.loadCompleted()
        }
    }
    property bool anyChanged: co_signing.dataChanged ||
                             _currency.dataChanged ||
                             _interal.dataChanged ||
                             autoBroadcast.dataChanged
    Column {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 24
        QText {
            font.family: "Lato"
            font.pixelSize: 28
            color: "#031F2B"
            font.weight: Font.Bold
            text: STR.STR_QML_803
        }
        Column {
            id: honey_badger
            visible: ClientController.user.isHoneyBadgerUser
            spacing: 16
            Item {
                width: 627
                height: 52
                Column {
                    anchors.fill: parent
                    spacing: 4
                    QLato {
                        width: parent.width
                        height: 20
                        font.weight: Font.Bold
                        text: STR.STR_QML_799
                    }
                    QLato {
                        width: parent.width
                        height: 28
                        text: STR.STR_QML_800
                    }
                }
            }
            Item {
                width: 627
                height: 72
                Row {
                    anchors.fill: parent
                    spacing: 14
                    QTextInputBoxTypeB {
                        id: co_signing
                        label: STR.STR_QML_801
                        boxWidth: 306
                        boxHeight: 48
                        isValid: true
                        textInputted: policies.spending_limit.limit
                        onTextInputtedChanged: {
                            if(!co_signing.isValid){
                                co_signing.isValid = true
                                co_signing.errorText = ""
                            }
                            co_signing.showError = false;
                        }
                        function limit() {
                            return co_signing.textInputted
                        }
                        property bool dataChanged: policies.spending_limit.limit !== co_signing.textInputted
                    }
                    QComboBox {
                        id: _currency
                        anchors.bottom: co_signing.bottom
                        width: 128
                        height: 48
                        displayText: policies.spending_limit.currency
                        model: list()
                        textRole: "displayName"
                        function list() {
                            var ls = []
                            ls.push({ displayName: qsTr("%1").arg(AppSetting.currency) })
                            ls.push({ displayName: qsTr("BTC") })
                            ls.push({ displayName: qsTr("sat") })
                            return ls
                        }
                        function getIndex(displayName) {
                            var lys = list()
                            for (var i = 0; i < lys.length; i++) {
                                if (lys[i].displayName === displayName) {
                                    return i;
                                }
                            }
                            return -1;
                        }
                        function currency() {
                            return currentIndex < 0 ? AppSetting.currency : _currency.model[currentIndex].displayName
                        }
                        function loadCompleted() {
                            _currency.currentIndex = _currency.getIndex(policies.spending_limit.currency)
                        }
                        property bool dataChanged: _currency.getIndex(policies.spending_limit.currency) !== _currency.currentIndex
                    }
                    QComboBox {
                        id: _interal
                        anchors.bottom: co_signing.bottom
                        currentIndex: _interal.getIndex(currentText)
                        width: 165
                        height: 48
                        model:  [
                            { displayName: qsTr("DAILY") },
                            { displayName: qsTr("WEEKLY") },
                            { displayName: qsTr("MONTHLY") },
                            { displayName: qsTr("YEARLY") }
                        ]
                        textRole: "displayName"
                        function getIndex(displayName) {
                            var lys = ["DAILY", "WEEKLY", "MONTHLY", "YEARLY"]
                            for (var i = 0; i < lys.length; i++) {
                                if (lys[i] === displayName) {
                                    return i;
                                }
                            }
                            return 0;
                        }
                        function interval() {
                            return _interal.model[currentIndex].displayName
                        }
                        property bool dataChanged: _interal.getIndex(policies.spending_limit.interval) !== _interal.currentIndex
                        function loadCompleted() {
                            _interal.currentIndex = _interal.getIndex(policies.spending_limit.interval)
                        }
                    }
                }
            }
        }
        Rectangle {
            visible: honey_badger.visible
            width: 627
            height: 1
            color: "#EAEAEA"
        }
        QPoliciesBroadcastAndDelay {
            id: autoBroadcast
            serverKeyInfo: ServiceSetting.walletInfo.serverKeyInfo
        }
    }
    function get_policies() {
        var broadcast = autoBroadcast.policies_broadcast()
        var spending_limit = {
            "limit" : co_signing.limit(),
            "currency" : _currency.currency(),
            "interval" : _interal.interval(),
        }
        var isIronHand = ClientController.user.isIronHandUser
        var ret = {
            "signing_delay_seconds" : broadcast.signing_delay_seconds,
            "auto_broadcast_transaction" : broadcast.auto_broadcast_transaction,
            "spending_limit" : isIronHand ? {} : spending_limit,
        }
        return ret
    }
    Rectangle {
        id: normalRect
        width: parent.width
        height: 80
        anchors{
            right: parent.right
            bottom: parent.bottom
        }
        layer.enabled: true
        layer.effect: DropShadow {
            radius: 4
            samples: radius * 2
            source: normalRect
            color: Qt.rgba(0, 0, 0, 0.5)
        }
        Row {
            anchors{
                right: parent.right
                rightMargin: 24
                bottom: parent.bottom
                bottomMargin: 16
            }
            spacing: 12
            layoutDirection: Qt.RightToLeft
            QTextButton {
                id: _save
                width: 214
                height: 48
                label.text: STR.STR_QML_804
                label.font.pixelSize: 16
                type: eTypeE
                enabled: anyChanged
                onButtonClicked: {
                    serverKeyInfo.setKeyCoSigning(get_policies())
                    QMLHandle.sendEvent(EVT.EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST)
                }
            }
            QTextButton {
                width: 148
                height: 48
                label.text: STR.STR_QML_805
                label.font.pixelSize: 16
                type: eTypeF
                onButtonClicked: {
                    ServiceSetting.clearWalletInfo()
                }
            }
        }
    }
}
