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
    property string myRole: ServiceSetting.walletInfo.myRole
    property bool isDisable: myRole === "KEYHOLDER" || myRole === "KEYHOLDER_LIMITED"
    enabled: !isDisable
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
        target: ServiceSetting.walletInfo.serverKeyInfo
        onSecurityQuestionClosed: {
            if (ServiceSetting.optionIndex === _PLATFORM_KEY_CO_SIGNING_POLICIES) {
                _Security.close()
            }
        }
        onServerKeyDummyTransactionAlert: {
            if (ServiceSetting.optionIndex === _PLATFORM_KEY_CO_SIGNING_POLICIES) {
                QMLHandle.sendEvent(EVT.EVT_HEALTH_CHECK_STARTING_REQUEST)
            }
        }
    }
    QText {
        anchors {
            left: parent.left
            leftMargin: 24
            top: parent.top
            topMargin: 24
        }
        font.family: "Lato"
        font.pixelSize: 28
        color: "#031F2B"
        font.weight: Font.Bold
        text: STR.STR_QML_803.arg(ServiceSetting.walletInfo.walletName)
    }

    Flickable {
        anchors {
            top: parent.top
            topMargin: 28 + 24 + 24
        }
        clip: true
        width: 660
        height: parent.height - 108
        contentHeight: _colum.childrenRect.height
        ScrollBar.vertical: ScrollBar { active: true }
        Column {
            id: _colum
            anchors {
                left: parent.left
                leftMargin: 24
            }
            spacing: 24
            Column {
                spacing: 16
                QPoliciesSpendingLimit {
                    description: STR.STR_QML_1030
                }
                QPoliciesApplyTheSameLimitToAllMembers {
                    id: _limit_list
                    serverKeyInfo: ServiceSetting.walletInfo.serverKeyInfo
                }
                QPoliciesCoSigningSpendingLimit {
                    id: _limit_all
                    visible: _limit_list.apply_same_spending_limit()
                    width: 595
                    height: 68
                    titleFontSize: 12
                    limit: serverKeyInfo.policies.spending_limit.limit
                    currency: serverKeyInfo.policies.spending_limit.currency
                    interval: serverKeyInfo.policies.spending_limit.interval
                }
            }
            Rectangle {
                width: 627
                height: 1
                color: "#EAEAEA"
            }
            QPoliciesBroadcastAndDelay {
                id: autoBroadcast
                serverKeyInfo: ServiceSetting.walletInfo.serverKeyInfo
            }
            Item {
                width: 627
                height: 108
            }
        }
    }

    function get_policies() {
        var broadcast = autoBroadcast.policies_broadcast()
        var list = _limit_list.list_spending_limit()
        var isApplySame = _limit_list.apply_same_spending_limit()
        var ret = {
            "apply_same_spending_limit" : _limit_list.apply_same_spending_limit(),
            "signing_delay_seconds" : broadcast.signing_delay_seconds,
            "auto_broadcast_transaction" : broadcast.auto_broadcast_transaction,
            "members_spending_limit" : isApplySame ? null : list,
            "spending_limit" : isApplySame ? _limit_all.spending_limit() : null,
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
                onButtonClicked: {
                    var broadcast = autoBroadcast.policies_broadcast()
                    var signing_delay_seconds_new = broadcast.signing_delay_seconds
                    var signing_delay_seconds_old = serverKeyInfo.policies.signing_delay_seconds
                    if (signing_delay_seconds_new < signing_delay_seconds_old) {
                        _info1.contentText = STR.STR_QML_1344
                        _info1.title = STR.STR_QML_339
                        _info1.open()
                        _info1.action = function() {
                            serverKeyInfo.setKeyCoSigning(get_policies())
                            QMLHandle.sendEvent(EVT.EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST)
                        }
                    } else {
                        serverKeyInfo.setKeyCoSigning(get_policies())
                        QMLHandle.sendEvent(EVT.EVT_CO_SIGNING_SERVER_KEY_UPDATE_REQUEST)
                    }
                }
            }
            QTextButton {
                width: 148
                height: 48
                label.text: STR.STR_QML_805
                label.font.pixelSize: 16
                type: eTypeF
                onButtonClicked: {
                    _limit_list.list_spending_limit()
                }
            }
        }
    }
}
