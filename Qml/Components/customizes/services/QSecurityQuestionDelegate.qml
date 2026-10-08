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
import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
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

Item {
    id: questionsdlg
    width: 539
    height: col.childrenRect.height
    property bool answerInitialized: false
    property string lastSyncedAnswer: ""
    property bool showCustomQuestionError: false
    property string customQuestion: ""
    signal customQuestionCommitted(string value)
    signal customQuestionDraftEdited(string value)

    function syncAnswer(value) {
        if (!answerInitialized || value === lastSyncedAnswer) {
            return
        }
        lastSyncedAnswer = value
        keyRecovery.setupSecQuesAnswer(index, modelData.id, value)
    }

    Component.onCompleted: {
        lastSyncedAnswer = answer.textInputted
        answerInitialized = true
    }

    Column {
        id: col
        spacing: 16
        QSetupCreateSecurityQuestionBox {
            id: _box
            width: 539
            height: modelData.id === "my_question"
                    ? (72 + 48 + 24 + (_box.showQuestionError && !_box.customQuestionValid ? 24 : 0))
                    : 72
            remain_questions: modelData.remain_questions
            newQuestion: questionsdlg.customQuestion
            showQuestionError: questionsdlg.showCustomQuestionError
            onNewQuestionChanged: {
                if (modelData.id === "my_question") {
                    questionsdlg.customQuestionCommitted(_box.newQuestion)
                }
            }
            onQuestionDraftEdited: (value) => {
                if (modelData.id === "my_question") {
                    questionsdlg.customQuestionDraftEdited(value)
                }
            }
        }

        QTextInputBoxTypeB {
            id: answer
            label: STR.STR_QML_718
            boxWidth: 537
            boxHeight: 48
            emitEmptyTypingFinished: true
            isValid: true
            textInputted: modelData.answer
            input.placeholderText: modelData.isChanged ? "" : "••••••••"
            onTextInputtedChanged: {
                if(!answer.isValid){
                    answer.isValid = true
                    answer.errorText = ""
                }
                answer.showError = false;
                questionsdlg.syncAnswer(textInputted)
            }
            onTypingFinished: (currentText) => {
                questionsdlg.syncAnswer(currentText)
            }
        }
    }
}
