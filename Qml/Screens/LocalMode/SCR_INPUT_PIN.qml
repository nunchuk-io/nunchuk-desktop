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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/customizes"
import "../../Components/origins"
QScreen {
    id: rootinputpin
    QNumPad {
        id: pinModel
        anchors.centerIn: parent
        onCloseClicked: {
            pinModel.processing = false
            QMLHandle.sendEvent(EVT.EVT_INPUT_PIN_SEND_PIN, "")
        }
        onSenPINClicked : {
            pinModel.processing = true
            QMLHandle.sendEvent(EVT.EVT_INPUT_PIN_SEND_PIN, pinModel.pinInputted)
        }
    }
    QBusyIndicator {
        anchors.centerIn: pinModel
        visible: pinModel.processing
    }
    Connections {
        target: AppModel
        onFinishSendPinToDevice : pinModel.processing = false
    }
}
