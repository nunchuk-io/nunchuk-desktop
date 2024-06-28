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
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1303
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property string email_name: ""
    content: Item {
        Column {
            anchors.fill: parent
            spacing: 24
            QTextInputBoxTypeB {
                id: _name
                label: STR.STR_QML_1302
                boxWidth: 539
                boxHeight: 48
                textInputted: email_name
                onTextInputtedChanged: {
                    if(!_name.isValid){
                        _name.isValid = true
                        _name.errorText = ""
                    }
                    _name.showError = false;
                    email_name = _name.textInputted
                }
            }
        }

        Connections {
            target: _content
            onNextClicked: {
                if (email_name == ClientController.user.email) {
                    _info1.open()
                }
                else if (validateEmail(email_name)) {
                    var _input = {
                        type: "email-name",
                        new_email: email_name
                    }
                    QMLHandle.sendEvent(EVT.EVT_CHANGE_EMAIL_ENTER, _input)

                }
                else {
                    _name.isValid = false
                    _name.showError = true
                    _name.errorText = STR.STR_QML_1309
                }
            }
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    nextEnable: email_name !== ""
    function validateEmail(email) {
        if(Draco.pkey_username_availability(email)){
            return true;
        }else{
            var re = /\S+@\S+\.\S+/;
            return re.test(email);
        }
    }
    QPopupInfo{
        id:_info1
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1310
    }
}
