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
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.12
import QtQuick.Controls.Styles 1.4
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
    id: _Message
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_472
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    readonly property var roles: [
        {id: "MASTER",              name: STR.STR_QML_949  },
        {id: "ADMIN",               name: STR.STR_QML_950  },
        {id: "KEYHOLDER",           name: STR.STR_QML_951  },
        {id: "KEYHOLDER_LIMITED",   name: STR.STR_QML_952  },
        {id: "FACILITATOR_ADMIN",   name: STR.STR_QML_1396 },
        {id: "OBSERVER",            name: STR.STR_QML_953  },
        {id: "NEW-MEMBER",          name: STR.STR_QML_1403  }
    ]
    property int memberIndex: 0
    property string memberRole: ""
    property string memberEmail: ""
    content: Item {
        QListView {
            id: _members
            width: 539 + 24
            height: 506
            model: dashboardInfo.editMembers
            clip: true
            ScrollBar.vertical: ScrollBar { active: true }
            spacing: 25
            delegate: Loader {
                sourceComponent: modelData.isNew ? _editNew : _edit
                Component {
                    id: _edit
                    QEditMemberDelegate {
                        width: ListView.view.width - 24
                        member: modelData
                        onArrowClicked: {
                            memberIndex = index
                            memberRole = modelData.role
                            memberEmail = modelData.email_or_username
                            _editMemberSelectARole.open()
                        }
                        onRemoveClicked: {
                            memberEmail = modelData.email_or_username
                            removeMember()
                        }
                    }
                }
                Component {
                    id: _editNew
                    QEditMemberNewMemberDelegate {
                        width: ListView.view.width - 24
                        member: modelData
                        onArrowClicked: {
                            memberIndex = index
                            memberRole = modelData.role
                            memberEmail = modelData.email_or_username
                            _editMemberSelectARole.open()
                        }
                    }
                }

            }
        }
    }
    function replaceMember() {
        var _input = {
            type: "replace-new-member",
            roleId: memberRole,
            email_or_username: memberEmail,
            indexMember: memberIndex
        }
        QMLHandle.notifySendEvent(EVT.EVT_EDIT_MEMBERS_ENTER_REQUEST, _input)
    }
    function removeMember() {
        var _input = {
            type: "remove-member",
            email_or_username: memberEmail,
        }
        QMLHandle.notifySendEvent(EVT.EVT_EDIT_MEMBERS_ENTER_REQUEST, _input)
    }

    isShowLine: true
    onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    bottomRight: Row {
        spacing: 12
        QIconTextButton {
            width: 154
            height: 48
            label: STR.STR_QML_386
            icons: ["add-dark.svg", "add-dark.svg", "add-dark.svg","add-dark.svg"]
            fontPixelSize: 16
            iconSize: 16
            type: eTypeB
            onButtonClicked: {
                var _input = {
                    type: "add-new-member",
                    roleId: "NEW-MEMBER",
                }
                QMLHandle.notifySendEvent(EVT.EVT_EDIT_MEMBERS_ENTER_REQUEST, _input)
            }
        }
        QTextButton {
            width: 66
            height: 48
            label.text: STR.STR_QML_835
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                var _input = {
                    type: "save-member-changed"
                }
                QMLHandle.notifySendEvent(EVT.EVT_EDIT_MEMBERS_ENTER_REQUEST, _input)
            }
        }
    }
    QPopupEditMemberSelectARole {
        id: _editMemberSelectARole
        anchors.centerIn: parent
        width: 600
        height: 600
    }
}

