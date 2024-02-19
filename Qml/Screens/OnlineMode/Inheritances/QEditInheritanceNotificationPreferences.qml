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
    property var planInfo
    property string emails: planInfo.display_emails
    property bool isNotify: planInfo.buffer_period.enabled
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_865
    onCloseClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    content: Item {
        Column {
            anchors.fill: parent
            anchors.top: parent.top
            anchors.topMargin: -16
            spacing: 24
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
            }
            QEmailAndUserInput {
                id: _email_user
                title: STR.STR_QML_867
                width: 539
                height: 140
                requestlist: emails.length === 0 ? [] : emails.split(",")
                onEmailUpdated: {
                    emails = requestlist.join(",")
                }
            }
            QCheckBoxButton{
                id: notify
                width: 539
                height: 24
                label: STR.STR_QML_868
                checked: isNotify
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
                bottomMargin: 36
            }
            color:"#FDEBD2"
            radius: 8
            Row {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8
                QImage {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 36; height: 36;
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
            onButtonClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
        }
        QTextButton {
            width: 259
            height: 48
            label.text: STR.STR_QML_870
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                _Notification.nextClicked()
            }
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    onNextClicked: {
        var buffer_period = {
            "enabled" : isNotify
        }
        var _edit = {
            "buffer_period": buffer_period,
            "display_emails": emails
        }
        inheritancePlanInfo.editPlanInfo(_edit)
        closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    }
}

