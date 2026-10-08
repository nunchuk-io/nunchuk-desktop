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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../Components/customizes/Popups"
import "../../../Components/customizes/services"
import "../../../../localization/STR_QML.js" as STR


QPopupEmpty {
    id: _select_a_role
    readonly property var roles: [
        {id: "ADMIN",               name: STR.STR_QML_950,  des1: "",               des2: STR.STR_QML_1397},
        {id: "KEYHOLDER",           name: STR.STR_QML_951,  des1: "",               des2: STR.STR_QML_1398},
        {id: "KEYHOLDER_LIMITED",   name: STR.STR_QML_952,  des1: STR.STR_QML_1402, des2: STR.STR_QML_1399},
        {id: "FACILITATOR_ADMIN",   name: STR.STR_QML_1396, des1: "",               des2: STR.STR_QML_1400},
        {id: "OBSERVER",            name: STR.STR_QML_953,  des1: "",               des2: STR.STR_QML_1401},
    ]
    property string option: ""
    onOpened: {
        option = memberRole
    }
    closePolicy:Popup.NoAutoClose
    content: QOnScreenContentTypeB {
        width: 600
        height: 600
        anchors.centerIn: parent
        label.text: STR.STR_QML_1403
        extraHeader: Item {}
        onCloseClicked: { _select_a_role.close() }
        content: Item {
            anchors.fill: parent
            QListView {
                id: _list
                width: 528
                height: parent.height
                model: roles
                clip: true
                spacing: 16
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: QEditMemberSelectRoleDelegate {
                    width: parent.width - 18
                }
            }
        }
        isShowLine: true
        onPrevClicked: { _select_a_role.close() }
        bottomRight: QTextButton {
            width: 66
            height: 48
            label.text: STR.STR_QML_835
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                if (dashboardInfo.isDowngrade(memberEmail, option)) {
                    _info1.open()
                } else {
                    memberRole = option
                    replaceMember()
                    _select_a_role.close()
                }
            }
        }
    }
    QPopupInfo{
        id:_info1
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1404
    }
}
