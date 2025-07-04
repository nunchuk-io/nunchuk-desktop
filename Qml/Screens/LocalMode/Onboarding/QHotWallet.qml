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
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1251
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Item {
        Row {
            spacing: 36
            Rectangle {
                width: 346
                height: 512
                radius: 24
                color: "#FDEBD2"
                QPicture {
                    width: 208
                    height: 136
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/hot-wallet-description.svg"
                }
            }
            Item {
                width: 346
                height: 512
                Column {
                    width: parent.width
                    spacing: 24
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_1227
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

    }

    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 267
            height: 48
            label.text: STR.STR_QML_1228
            label.font.pixelSize: 16
            type: eTypeB
            visible: {
                var highestPermissionAccount = GroupWallet.highestPermissionAccount
                var hasGroupWallet = highestPermissionAccount.hasGroupWallet
                var hasGroupPending = highestPermissionAccount.hasGroupPending
                var asissted = ClientController.user.isSubscribedUser || hasGroupWallet || hasGroupPending
                return !asissted
            }
            onButtonClicked: {
                OnBoarding.screenFlow = "getMore"
            }
        }
        QTextButton {
            width: 99
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                var _input = {
                    type: "create-a-hot-wallet-now"
                }
                QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
            }
        }
    }
}
