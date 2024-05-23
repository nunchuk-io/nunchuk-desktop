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
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1305
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property string code_email: ""
    content: Item {
        Column {
            anchors.fill: parent
            spacing: 24
            QTextInputBoxTypeB {
                id: _name
                label: STR.STR_QML_1027
                boxWidth: 539
                boxHeight: 48
                textInputted: code_email
                onTextInputtedChanged: {
                    if(!_name.isValid){
                        _name.isValid = true
                        _name.errorText = ""
                    }
                    _name.showError = false;
                    code_email = _name.textInputted
                }
            }
        }
        QWarningBg {
            width: 728
            iSize: 60
            icon: "qrc:/Images/Images/info-60px.png"
            txt.text: STR.STR_QML_1028_bottom.arg(ProfileSetting.newEmail)
            anchors.bottom: parent.bottom
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    nextEnable: code_email !== ""
    onNextClicked: {
        var _input = {
            type: "email-code",
            codeEmail: code_email
        }
        QMLHandle.sendEvent(EVT.EVT_CHANGE_EMAIL_ENTER, _input)
    }
}
