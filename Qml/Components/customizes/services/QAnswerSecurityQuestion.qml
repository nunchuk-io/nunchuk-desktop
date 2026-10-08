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
    label.text: STR.STR_QML_716
    signal clearText
    Column {
        anchors{
            left: parent.left
            leftMargin: 36
            top: parent.top
            topMargin: 84
        }
        spacing: 24
        QText {
            width: 646
            height: 28
            color: "#031F2B"
            font.pixelSize: 16
            font.weight: Font.Normal
            text: STR.STR_QML_719
        }
        QListView {
            id: questions
            width: 539
            height: childrenRect.height + 16
            spacing: 16
            clip: true
            model: ServiceSetting.servicesTag.securityQuestions
            interactive: false
            delegate: Item {
                id: questionsdlg
                width: 539
                height: col.childrenRect.height
                Column {
                    id: col
                    spacing: 16
                    QText {
                        width: 537
                        height: 28
                        color: "#031F2B"
                        font.pixelSize: 16
                        font.weight: Font.DemiBold
                        text: modelData.question
                    }
                    QTextInputBoxTypeB {
                        id: answer
                        label: STR.STR_QML_718
                        boxWidth: 537
                        boxHeight: 48
                        isValid: true
                        onTextInputtedChanged: {
                            if(!answer.isValid){
                                answer.isValid = true
                                answer.errorText = ""
                            }
                            answer.showError = false;
                            ServiceSetting.servicesTag.secQuesAnswer(modelData.id, textInputted)
                        }
                    }
                }
                Connections {
                    target: ServiceSetting.servicesTag
                    onAnswerErrorAlert: {
                        answer.errorText = errormsg
                        answer.isValid = false
                        answer.showError = true
                    }
                }
                Connections {
                    target: _content
                    onClearText: {
                        answer.textInputted = ""
                    }
                }

            }
        }
    }

    QBusyIndicator {
        id:_indicator
        visible: false
        anchors.centerIn: _content
    }
}
