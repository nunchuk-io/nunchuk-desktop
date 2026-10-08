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
import "../../customizes/Chats"
import "../../customizes/Popups"
import "../../customizes/Wallets"
import "../../../../localization/STR_QML.js" as STR

QPopupOverlayScreen {
    id: _infoPopup
    property int _INTROTAPROOT: 1
    property int _TAPROOTWARNING: 2
    property int nextState: _INTROTAPROOT
    content: switch(_infoPopup.nextState) {
             case _infoPopup._INTROTAPROOT: return introTaproot
             case _infoPopup._TAPROOTWARNING: return taprootWarning
             default: return null
          }

    function switchIntroTaproot() {
        _infoPopup.nextState = _infoPopup._INTROTAPROOT
    }
    function switchTaprootWarning() {
        _infoPopup.nextState = _infoPopup._TAPROOTWARNING
    }
    Component {
        id : introTaproot
        QIntroductionTaprootAddress {
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: _infoPopup.close()
            onNextClicked: {
                _infoPopup.switchTaprootWarning()
            }
        }
    }
    Component {
        id : taprootWarning
        QTaprootWarningSupport {
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onPrevClicked: _infoPopup.switchIntroTaproot()
            onNextClicked: {
                _content.contentItem.createWallet()
            }
        }
    }
}