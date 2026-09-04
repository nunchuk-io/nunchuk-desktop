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

QOnScreenContentTypeB {
    id: _content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1061
    property var keyRecovery: ServiceSetting.servicesTag.keyRecovery
    property var questionStates: keyRecovery.requireQuestions
    // The C++ context retains a successfully staged custom draft across an
    // authorization Back transition. Local edits still take ownership of
    // these bindings as soon as the user types.
    property string newMyQuestion: keyRecovery.securityQuestionCustomDraft
    property string customQuestionDraft: keyRecovery.securityQuestionCustomDraft
    property bool submitting: false
    property bool customQuestionValidationRequested: false
    readonly property bool hasCustomQuestion: {
        var states = _content.questionStates || []
        for (var i = 0; i < states.length; i++) {
            if (states[i].id === "my_question") {
                return true
            }
        }
        return false
    }
    readonly property bool hasPendingChanges: {
        if (_content.hasCustomQuestion && _content.customQuestionDraft.length > 0) {
            return true
        }
        var states = _content.questionStates || []
        for (var i = 0; i < states.length; i++) {
            if (states[i].isChanged) {
                return true
            }
        }
        return false
    }
    readonly property bool canSubmitChanges: {
        if (!_content.hasPendingChanges) {
            return false
        }
        if (_content.hasCustomQuestion && _content.customQuestionDraft.trim().length === 0) {
            return false
        }

        var states = _content.questionStates || []
        var hasCompleteChange = false
        for (var i = 0; i < states.length; i++) {
            var question = states[i]
            if (!question.isChanged) {
                continue
            }
            var answerText = question.answer || ""
            if (!question.change || answerText.trim().length === 0) {
                return false
            }
            hasCompleteChange = true
        }
        return hasCompleteChange
    }
    closebutton.enabled: !submitting && !keyRecovery.securityQuestionUpdateInProgress

    function submitCommittedInput() {
        if (!canSubmitChanges || keyRecovery.securityQuestionUpdateInProgress) {
            submitting = false
            return
        }
        var customQuestion = customQuestionDraft
        if (hasCustomQuestion && customQuestion.trim().length === 0) {
            customQuestionValidationRequested = true
            submitting = false
            return
        }
        customQuestionValidationRequested = false
        // The C++ submit handler has its own re-entrancy guard. Clear the local
        // guard before the synchronous state event can replace this screen.
        submitting = false
        QMLHandle.sendEvent(EVT.EVT_INPUT_SECURITY_QUESTION_REQUEST, customQuestion)
    }
    content: Item {
        enabled: !_content.submitting && !_content.keyRecovery.securityQuestionUpdateInProgress
        Column {
            anchors.fill: parent
            spacing: 24
            QLato {
                width: 646
                height: paintedHeight
                text: STR.STR_QML_1062
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
            }
            QListView {
                id: questions
                width: 539 + 12
                height: 389
                spacing: 16
                clip: true
                Component.onCompleted: model = keyRecovery.requireQuestions
                ScrollBar.vertical: QScrollBar { }
                delegate: QSecurityQuestionDelegate {
                    showCustomQuestionError: _content.customQuestionValidationRequested
                    customQuestion: _content.newMyQuestion
                    onCustomQuestionCommitted: (value) => {
                        _content.newMyQuestion = value
                        _content.customQuestionDraft = value
                    }
                    onCustomQuestionDraftEdited: (value) => {
                        _content.customQuestionDraft = value
                    }
                }
            }
            Connections {
                target: keyRecovery
                function onRequireQuestionsChanged() {
                    var newList = keyRecovery.requireQuestions
                    var containsCustomQuestion = false
                    var needsRefresh = newList.length !== questions.count
                    if (!needsRefresh) {
                        var oldList = questions.model
                        for (var i = 0; i < newList.length; i++) {
                            var oldItem = oldList[i]
                            var newItem = newList[i]
                            if (newItem.id === "my_question") {
                                containsCustomQuestion = true
                            }
                            if (!oldItem || oldItem.id !== newItem.id || oldItem.question !== newItem.question) {
                                needsRefresh = true
                            }
                        }
                    } else {
                        for (var j = 0; j < newList.length; j++) {
                            if (newList[j].id === "my_question") {
                                containsCustomQuestion = true
                                break
                            }
                        }
                    }
                    if (!containsCustomQuestion) {
                        newMyQuestion = ""
                        customQuestionDraft = ""
                        customQuestionValidationRequested = false
                    }
                    if (needsRefresh) {
                        var savedY = questions.contentY
                        questions.model = newList
                        Qt.callLater(function() { questions.contentY = savedY })
                    }
                }
            }
        }
    }
    isShowLine: true
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 148
            height: 48
            label.text: STR.STR_QML_193
            label.font.pixelSize: 16
            type: eTypeB
            enabled: hasPendingChanges && !submitting && !keyRecovery.securityQuestionUpdateInProgress
            onButtonClicked: {
                if (submitting || keyRecovery.securityQuestionUpdateInProgress) {
                    return
                }
                keyRecovery.resetSecurityQuestionUpdate()
                closeTo(NUNCHUCKTYPE.SERVICE_TAB)
            }
        }
        QTextButton {
            width: 214
            height: 48
            label.text: STR.STR_QML_804
            label.font.pixelSize: 16
            type: eTypeE
            enabled: canSubmitChanges && !submitting && !keyRecovery.securityQuestionUpdateInProgress
            onButtonClicked: {
                if (submitting || keyRecovery.securityQuestionUpdateInProgress) {
                    return
                }
                Qt.inputMethod.commit()
                submitting = true
                Qt.callLater(function() { _content.submitCommittedInput() })
            }
        }
    }
}
