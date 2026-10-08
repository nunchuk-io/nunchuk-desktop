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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/services"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property var reqiredSignature: ServiceSetting.servicesTag.reqiredSignatures
    Loader {
        anchors.centerIn: parent
        sourceComponent: {
            if (reqiredSignature.type === "SECURITY_QUESTION") {
                return security_question
            }
            else if (reqiredSignature.type === "CONFIRMATION_CODE") {
                return confirm_code
            }
            return null
        }
    }
    Component {
        id: security_question
        QAnswerSecurityQuestion {
            onCloseClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)

            onPrevClicked: {
                QMLHandle.sendEvent(EVT.EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK)
            }

            onNextClicked: {
                QMLHandle.sendEvent(EVT.EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ)
            }
        }
    }
    Component {
        id: confirm_code
        QAnswerSQConfirmCode {
            id: _confirm_code
            label.text: STR.STR_QML_1064
            description_top: STR.STR_QML_1028_top
            onCloseClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)

            onPrevClicked: {
                QMLHandle.sendEvent(EVT.EVT_SETUP_ANSWER_SECURITY_QUESTION_BACK)
            }

            onNextClicked: {
                _confirm_code.loading()
                QMLHandle.sendEvent(EVT.EVT_INPUT_ANSWER_SECURITY_QUESTION_REQ, _confirm_code.code())
            }
        }
    }
}
