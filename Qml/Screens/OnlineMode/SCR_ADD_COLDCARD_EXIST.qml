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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    signal addExistKey()
    QOnScreenContentTypeB {
        id:_content
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_901
        extraHeader: Item {}
        onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        content: QAddAnExistingKey {
            notice: STR.STR_QML_916
            keyType: "coldcard"
        }
        onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        bottomRight: Row {
            spacing: 12
            QTextButton {
                width: 215
                height: 48
                label.text: STR.STR_QML_900
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_ADD_NEW_COLDCARD_REQUEST)
                }
            }
            QTextButton {
                width: 150
                height: 48
                label.text: STR.STR_QML_899
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    var coldcardXfp = _content.contentItem.fingerPrint
                    QMLHandle.sendEvent(EVT.EVT_ADD_EXIST_COLDCARD_REQUEST, coldcardXfp)
                }
            }
        }
    }
    QPopupInfo{
        id:_info1
        contentText: STR.STR_QML_914
        onGotItClicked: {
            close()
        }
    }

    Connections {
        target: UserWallet
        onAddHardwareAlert:{
            _info1.open()
        }
        onAddHardwareSuccessAlert:{
            closeTo(NUNCHUCKTYPE.WALLET_TAB)
        }
    }
}
