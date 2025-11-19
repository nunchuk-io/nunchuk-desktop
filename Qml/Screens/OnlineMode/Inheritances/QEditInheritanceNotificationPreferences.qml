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
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.12
import QtQuick.Controls.Styles 1.4
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

QOnScreenContentTypeB {
    id: _Notification
    property var    planInfo
    property string emails: planInfo.display_emails
    property var    emails_preference: planInfo.notification_preferences
    property bool   isNotify: planInfo.buffer_period.enabled
    property int    walletType: ServiceSetting.walletInfo.walletType

    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_865

    onCloseClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)

    content: Item {
        function requestFinishInputEmail() {
            _email_user.requestFinishInputEmail()
        }
        Column {
            anchors.fill: parent
            spacing: 16
            QLato {
                width: 539
                height: 56
                text: STR.STR_QML_866
                anchors.left: parent.left
                font.weight: Font.Normal
                wrapMode: Text.WrapAnywhere
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                visible: walletType !== NUNCHUCKTYPE.MINISCRIPT
            }
            QEmailAndUserInput {
                id: _email_user
                title: walletType !== NUNCHUCKTYPE.MINISCRIPT ? STR.STR_QML_867 : STR.STR_QML_2026
                width: 728
                height: walletType !== NUNCHUCKTYPE.MINISCRIPT ? 280 : 330
                requestlist: emails.length === 0 ? [] : emails.split(",")
                onEmailUpdated: {
                    emails = requestlist.join(",")
                }
            }
            QCheckBoxButton{
                id: notify
                width: 280
                height: 24
                label: STR.STR_QML_868
                checked: isNotify
                visible: walletType !== NUNCHUCKTYPE.MINISCRIPT
                onButtonClicked: {
                    isNotify = checked
                }
            }
        }
        Rectangle {
            width: parent.width
            height: 80
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
            color:"#FDEBD2"
            radius: 8
            Row {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8
                QIcon {
                    iconSize: 36
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/warning-dark.svg"
                }
                QLato {
                    anchors.verticalCenter: parent.verticalCenter
                    text: STR.STR_QML_869
                    width: 660
                    height: 56
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 239
            height: 48
            label.text: STR.STR_QML_871
            label.font.pixelSize: 16
            type: eTypeB
            onButtonClicked: {
                closeTo(NUNCHUCKTYPE.SERVICE_TAB)
            }
        }
        QTextButton {
            width: walletType !== NUNCHUCKTYPE.MINISCRIPT ? 259 : 177
            height: 48
            label.text: walletType !== NUNCHUCKTYPE.MINISCRIPT ? STR.STR_QML_870 : STR.STR_QML_2027
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                _Notification.contentItem.requestFinishInputEmail()
                _Notification.nextClicked()
                if(walletType === NUNCHUCKTYPE.MINISCRIPT){
                    onchainSetupNotificationPreferences.open()
                }
            }
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    onNextClicked: {
        if(walletType !== NUNCHUCKTYPE.MINISCRIPT){
            var buffer_period = {
                "enabled" : isNotify
            }
            var _editOffchain = {
                "buffer_period": buffer_period,
                "display_emails": emails
            }
            inheritancePlanInfo.editPlanInfo(_editOffchain)
            closeTo(NUNCHUCKTYPE.SERVICE_TAB)
        }
        else {
            var buffer_period = {
                "enabled" : isNotify
            }
            var _editOnchain = {
                "setup_type": "emails",
                "display_emails": emails
            }
            inheritancePlanInfo.editPlanInfoOnchain(_editOnchain)
        }
    }

    Popup {
        id: onchainSetupNotificationPreferences
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape
        background: Item{}
        Rectangle {
            width: popupWidth
            height: popupHeight
            radius: 24
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: onchainSetupNotificationPreferences.width
                    height: onchainSetupNotificationPreferences.height
                    radius: 24
                }
            }

            QOnScreenContentTypeB {
                anchors.fill: parent
                label.text: STR.STR_QML_865
                onCloseClicked: onchainSetupNotificationPreferences.close()
                content: Item {
                    width: 728
                    height: 512
                    clip: true
                    Flickable {
                        width: parent.width
                        height: parent.height
                        contentWidth: width
                        contentHeight: _itempreferences.childrenRect.height
                        ScrollBar.vertical: ScrollBar { active: true }

                        Column {
                            id: _itempreferences
                            width: parent.width
                            spacing: 12
                            QLato {
                                font.pixelSize: 16
                                text: STR.STR_QML_852
                                anchors.left: parent.left
                            }

                            Rectangle {
                                width: _itempreferences.width
                                height: myemailItems.implicitHeight+32
                                radius: 12
                                color: "#F5F5F5"
                                Column {
                                    id: myemailItems
                                    width: parent.width - 32
                                    spacing: 16
                                    anchors.centerIn: parent
                                    QLato {
                                        text: ClientController.user.email
                                        width: parent.width
                                        elide: Text.ElideRight
                                        font.weight: Font.Bold
                                    }
                                    Rectangle {
                                        width: parent.width
                                        height: 1
                                        color: "#EAEAEA"
                                    }
                                    Row {
                                        width: parent.width
                                        Column {
                                            width: parent.width - 72
                                            spacing: 4
                                            anchors.verticalCenter: parent.verticalCenter
                                            QLato {
                                                width: parent.width
                                                horizontalAlignment: Text.AlignLeft
                                                text: STR.STR_QML_2019
                                            }
                                            QLato {
                                                width: parent.width
                                                horizontalAlignment: Text.AlignLeft
                                                text: STR.STR_QML_2029
                                                font.pixelSize: 12
                                                wrapMode: Text.WordWrap
                                                color: "#595959"
                                            }
                                        }
                                        QSwitchTypeB {
                                            id: email_me_wallet_config_switch
                                            width: 51
                                            height: 31
                                            switchOn: emails_preference ? emails_preference.email_me_wallet_config : false
                                            anchors.verticalCenter: parent.verticalCenter
                                            onSwitchOnChanged: {
                                                if(!switchOn){
                                                    var _editOnchain = {
                                                        "setup_type" : "owner",
                                                        "setup_email": ClientController.user.email,
                                                        "email_me_wallet_config": email_me_wallet_config_switch.switchOn
                                                    }
                                                    inheritancePlanInfo.editPlanInfoOnchain(_editOnchain)
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            QLato {
                                font.pixelSize: 16
                                text: STR.STR_QML_2028
                                anchors.left: parent.left
                            }

                            Repeater {
                                model: emails_preference.beneficiary_notifications.length
                                Rectangle {
                                    width: _itempreferences.width
                                    height: emailItems.implicitHeight+32
                                    radius: 12
                                    color: "#F5F5F5"
                                    Column {
                                        id: emailItems
                                        width: parent.width - 32
                                        spacing: 16
                                        anchors.centerIn: parent
                                        QLato {
                                            text: emails_preference.beneficiary_notifications[index] ? emails_preference.beneficiary_notifications[index].email : "unknown email"
                                            width: parent.width
                                            elide: Text.ElideRight
                                            font.weight: Font.Bold
                                        }
                                        Rectangle {
                                            width: parent.width
                                            height: 1
                                            color: "#EAEAEA"
                                        }
                                        Row {
                                            width: parent.width
                                            Column {
                                                width: parent.width - 72
                                                spacing: 4
                                                anchors.verticalCenter: parent.verticalCenter
                                                QLato {
                                                    width: parent.width
                                                    horizontalAlignment: Text.AlignLeft
                                                    text: STR.STR_QML_2020
                                                }
                                                QLato {
                                                    width: parent.width
                                                    horizontalAlignment: Text.AlignLeft
                                                    text: STR.STR_QML_2030
                                                    font.pixelSize: 12
                                                    wrapMode: Text.WordWrap
                                                    color: "#595959"
                                                }
                                            }

                                            QSwitchTypeB {
                                                id: notify_timelock_expires_switch
                                                width: 51
                                                height: 31
                                                switchOn: emails_preference.beneficiary_notifications[index] ? emails_preference.beneficiary_notifications[index].notify_timelock_expires : false
                                                anchors.verticalCenter: parent.verticalCenter
                                                onSwitchOnChanged: {
                                                    if(!switchOn){
                                                        var _editOnchain = {
                                                            "setup_type" : "preference",
                                                            "setup_email": emails_preference.beneficiary_notifications[index].email,
                                                            "notify_timelock_expires": notify_timelock_expires_switch.switchOn,
                                                            "notify_wallet_changes": notify_wallet_changes_switch.switchOn,
                                                            "include_wallet_config": include_wallet_config_switch.switchOn
                                                        }
                                                        inheritancePlanInfo.editPlanInfoOnchain(_editOnchain)
                                                    }
                                                }
                                            }
                                        }
                                        Row {
                                            width: parent.width
                                            Column {
                                                width: parent.width - 72
                                                spacing: 4
                                                anchors.verticalCenter: parent.verticalCenter
                                                QLato {
                                                    width: parent.width
                                                    horizontalAlignment: Text.AlignLeft
                                                    text: STR.STR_QML_2021
                                                }
                                                QLato {
                                                    width: parent.width
                                                    horizontalAlignment: Text.AlignLeft
                                                    text: STR.STR_QML_2031
                                                    font.pixelSize: 12
                                                    wrapMode: Text.WordWrap
                                                    color: "#595959"
                                                }
                                            }
                                            QSwitchTypeB {
                                                id: notify_wallet_changes_switch
                                                width: 51
                                                height: 31
                                                switchOn: emails_preference.beneficiary_notifications[index] ? emails_preference.beneficiary_notifications[index].notify_wallet_changes : false
                                                anchors.verticalCenter: parent.verticalCenter
                                                onSwitchOnChanged: {
                                                    if(!switchOn){
                                                        var _editOnchain = {
                                                            "setup_type" : "preference",
                                                            "setup_email": emails_preference.beneficiary_notifications[index].email,
                                                            "notify_timelock_expires": notify_timelock_expires_switch.switchOn,
                                                            "notify_wallet_changes": notify_wallet_changes_switch.switchOn,
                                                            "include_wallet_config": include_wallet_config_switch.switchOn
                                                        }
                                                        inheritancePlanInfo.editPlanInfoOnchain(_editOnchain)
                                                    }
                                                }
                                            }
                                        }
                                        Row {
                                            width: parent.width
                                            Column {
                                                width: parent.width - 72
                                                spacing: 4
                                                anchors.verticalCenter: parent.verticalCenter
                                                QLato {
                                                    width: parent.width
                                                    horizontalAlignment: Text.AlignLeft
                                                    text: STR.STR_QML_2022
                                                }
                                                QLato {
                                                    width: parent.width
                                                    horizontalAlignment: Text.AlignLeft
                                                    text: STR.STR_QML_2032
                                                    font.pixelSize: 12
                                                    wrapMode: Text.WordWrap
                                                    color: "#595959"
                                                }
                                            }
                                            QSwitchTypeB {
                                                id: include_wallet_config_switch
                                                width: 51
                                                height: 31
                                                switchOn: emails_preference.beneficiary_notifications[index] ? emails_preference.beneficiary_notifications[index].include_wallet_config : false
                                                anchors.verticalCenter: parent.verticalCenter
                                                onSwitchOnChanged: {
                                                    if(!switchOn){
                                                        var _editOnchain = {
                                                            "setup_type" : "preference",
                                                            "setup_email": emails_preference.beneficiary_notifications[index].email,
                                                            "notify_timelock_expires": notify_timelock_expires_switch.switchOn,
                                                            "notify_wallet_changes": notify_wallet_changes_switch.switchOn,
                                                            "include_wallet_config": include_wallet_config_switch.switchOn
                                                        }
                                                        inheritancePlanInfo.editPlanInfoOnchain(_editOnchain)
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                bottomRight: QTextButton {
                    width: 66
                    height: 48
                    label.text: STR.STR_QML_835
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
                }
                bottomLeft:  QButtonTextLink {
                    height: 48
                    label: STR.STR_QML_059
                    onButtonClicked: onchainSetupNotificationPreferences.close()
                }
            }
        }
    }
}

