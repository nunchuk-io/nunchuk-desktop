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

QOnScreenContentTypeB {
    id: _content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1061
    property var keyRecovery: ServiceSetting.servicesTag.keyRecovery
    property var firstQuestion: keyRecovery.firstQuestion
    property string newMyQuestion: ""
    property int newQuesIndex: -1
    content: Item {
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
                model: keyRecovery.requireQuestions
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: QSecurityQuestionDelegate {

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
            enabled: firstQuestion.isChanged
            onButtonClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
        }
        QTextButton {
            width: 214
            height: 48
            label.text: STR.STR_QML_804
            label.font.pixelSize: 16
            type: eTypeE
            enabled: firstQuestion.isChanged
            onButtonClicked: {
                if (newMyQuestion !== "") {
                    keyRecovery.createUserCustomizedQuestion(newQuesIndex, newMyQuestion)
                }
                QMLHandle.sendEvent(EVT.EVT_INPUT_SECURITY_QUESTION_REQUEST)
            }
        }
    }
}
