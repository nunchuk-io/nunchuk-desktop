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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id: _content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: ""
    property string description_top: ""
    property string description_bottom: ""
    signal clearText
    Column {
        anchors{
            left: parent.left
            leftMargin: 36
            top: parent.top
            topMargin: description_top !== "" ? 84 : 100
        }
        spacing: 24
        QLato {
            width: 650
            height: 28
            visible: description_top !== ""
            text: description_top.arg(ClientController.user.email)
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        QTextInputBoxTypeB {
            id: answer
            label: STR.STR_QML_1027
            boxWidth: 537
            boxHeight: 48
            isValid: true
            onTextInputtedChanged: {
                if(!answer.isValid){
                    answer.isValid = true
                    answer.errorText = ""
                }
                answer.showError = false;
            }
        }
        Connections {
            target: ServiceSetting.servicesTag
            onAnswerErrorAlert: {
                answer.errorText = errormsg
                answer.isValid = false
                answer.showError = true
                _indicator.visible = false
            }
            onConfirmCodeVerified: {
                _indicator.visible = false
            }
        }
        Connections {
            target: _content
            onClearText: {
                answer.textInputted = ""
            }
        }
    }


    nextEnable: answer.textInputted !== ""

    Rectangle {
        width: 718
        height: 60
        radius: 8
        color: "#EAEAEA"
        visible: description_bottom !== ""
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 120
        }

        Row {
            anchors.fill: parent
            anchors.margins: 15
            spacing: 11
            QImage {
                width: 30
                height: 30
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/Images/Images/OnlineMode/error_outline_24px_n.png"
            }
            QLato {
                width: 650
                height: 28
                anchors.verticalCenter: parent.verticalCenter
                text: description_bottom.arg(ClientController.user.email)
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
    function code() {
        return answer.textInputted
    }

    function loading() {
        _indicator.visible = true
    }

    QBusyIndicator {
        id:_indicator
        visible: false
        anchors.centerIn: _content
    }
}
