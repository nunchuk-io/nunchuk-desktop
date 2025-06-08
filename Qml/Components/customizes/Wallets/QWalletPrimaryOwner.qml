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

QOnScreenContentTypeA {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1194
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property var walletInfo: AppModel.walletInfo
    property string membership_id: ""
    content: Item {
        MouseArea {
            anchors.fill: parent
            onClicked: {
                _name.closeByMouse()
            }
        }
        Column {
            anchors.fill: parent
            spacing: 24
            QLato {
                width: 539
                height: paintedHeight
                text: STR.STR_QML_1195
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
            }
            Item {
                width: 539
                height: _name.childrenRect.height
                QMemberPrimaryOwner {
                    id: _name
                    label: STR.STR_QML_1194
                    boxWidth: 539
                    boxHeight: 48
                    validator: RegExpValidator { regExp: /[]+/ }
                    textInputted: walletInfo.ownerPrimary.name
                    onOwnerNameChanged: {
                        membership_id = member.membership_id
                    }
                }
                MouseArea {
                    width: 539
                    height: 72
                    propagateComposedEvents: true
                    onClicked: {
                        _name.openByMouse()
                    }
                }
            }
        }
        QWarningBg {
            width: 728
            iSize: 36
            icon: "qrc:/Images/Images/info-60px.svg"
            txt.text: STR.STR_QML_1196
            anchors.bottom: parent.bottom
        }
    }
    bottomLeft: Item {}
    bottomRight: QTextButton {
        width: 100
        height: 48
        label.text: STR.STR_QML_835
        label.font.pixelSize: 16
        type: eTypeE
        enabled: membership_id !== ""
        onButtonClicked: {
            if (walletInfo.updateWalletPrimaryOwner(membership_id)) {
                eFlow = eWALLET_CONFIG
                AppModel.showToast(0, STR.STR_QML_1199, EWARNING.SUCCESS_MSG);
            }
        }
    }
}
