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
import "./../../origins"
import "./../../customizes"
import "./../../customizes/Chats"
import "./../../customizes/Texts"
import "./../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    property string newQuestion: ""
    property var remain_questions: []
    property var ques_index: index
    Column {
        anchors.fill: parent
        spacing: 16
        Item {
            width: 539
            height: 72
            Column {
                spacing: 4
                QLato {
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    text: qsTr("Question #%1").arg(index + 1)
                }
                QComboBoxIndex {
                    width: 539
                    height: 48
                    model: remain_questions
                    textRole: "question"
                    function getText(data)
                    {
                        return data.question
                    }
                    onActivated: {
                        if (currentIndex >=0) {
                            var new_ques = remain_questions[currentIndex]
                            keyRecovery.changeRequiredQuestion(ques_index, new_ques)
                        }
                    }
                }
            }
        }
        QTextInputBoxTypeB {
            id: _question
            visible: modelData.id === "my_question"
            label: ""
            boxWidth: 537
            boxHeight: 48
            isValid: newQuestion !== ""
            textInputted: newQuestion
            onTextInputtedChanged: {
                newQuestion = _question.textInputted
            }
            errorText: STR.STR_QML_1063
            showError: newQuestion === ""
        }
    }
}
