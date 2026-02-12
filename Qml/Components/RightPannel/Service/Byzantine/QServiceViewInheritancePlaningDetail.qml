import "../../../../../localization/STR_QML.js" as STR
import "../../../customizes"
import "../../../customizes/Buttons"
import "../../../customizes/Chats"
import "../../../customizes/Texts"
import "../../../customizes/services"
import "../../../origins"
import "../Common"
import DRACO_CODE 1.0
import DataPool 1.0
import EWARNING 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import Qt.labs.platform 1.1
import QtGraphicalEffects 1.0
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
import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4

Item {
    property int walletType: ServiceSetting.walletInfo.walletType

    Loader {
        sourceComponent: walletType === NUNCHUCKTYPE.MINISCRIPT ? _onChain : _offChain
        anchors.fill: parent
    }

    Component {
        id: _offChain
        QServiceViewInheritancePlaningDetailOffChain {
        }
    }

    Component {
        id: _onChain
        QServiceViewInheritancePlaningDetailOnChain {
        }
    }

}
