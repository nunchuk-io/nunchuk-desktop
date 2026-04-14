/*
 * This file is part of the Nunchuk software (https://nunchuk.io/)
 * Copyright (C) 2020-2024 Enigmo
 * Copyright (C) 2024 Nunchuk
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import Features.Draftwallets.Sandbox.ViewModels 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Lists"
import "../../Components/customizes/Texts"

QOnScreenContentTypeA {
    id: _content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2143
    onCloseClicked: vm.close()
    onPrevClicked: vm.back()
    property var emails: vm.invitesList
    content: Item {
         function requestFinishInputEmail() {
            _email_user.requestFinishInputEmail()
        }
        Column {
            anchors.fill: parent
            spacing: 16
            QLato {
                width: 728
                height: 28
                text: STR.STR_QML_2144
                anchors.left: parent.left
                font.weight: Font.Normal
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QEmailAndUserInput {
                id: _email_user
                title: ""
                width: 728
                height: 330
                requestlist: emails.length === 0 ? [] : emails //.split(",")
                onEmailUpdated: {
                    var emails_valid = [];
                    for(var i = 0; i < requestlist.length; i ++){
                        var emailAddress = requestlist[i]
                        var isValid = validateEmail(emailAddress)
                        if(isValid){
                            emails_valid[emails_valid.length] = emailAddress
                        }
                    }
                                        
                    vm.isSendEnabled = requestlist.length == emails_valid.length && emails_valid.length > 0
                    emails = emails_valid //.join(",")   
                    vm.invitesList = emails_valid
                }
            }
        }
    }
    
    bottomRight: QTextButton {
            width: 100
            height: 48
            label.text: STR.STR_QML_2145
            type: eTypeB
            enabled: vm.isSendEnabled
            onClicked: { 
                _content.contentItem.requestFinishInputEmail()
                vm.onSendInviteClicked()
            }
        }
    
    GroupSandboxInvitesViewModel {
        id: vm
    }
}
