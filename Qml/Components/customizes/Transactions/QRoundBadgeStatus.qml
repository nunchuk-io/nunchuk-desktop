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
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import NUNCHUCKTYPE 1.0
import "../../customizes"
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../customizes/Signers"
import "../../customizes/Chats"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    width: 76
    height: 16
    radius: 20
    property var txStatus: 0
    property bool hasSigned: true
    anchors {
        right: parent.right
        rightMargin: 0
        verticalCenter: parent.verticalCenter
    }
    color: {
        if (txStatus === NUNCHUCKTYPE.READY_TO_BROADCAST) {return "#1C652D"}
        else if (txStatus === NUNCHUCKTYPE.PENDING_SIGNATURES) {return "#FFCB2E"}
        else {return hasSigned ? "#FDEBD2" : "#EAEAEA"}
    }
    QLato {
        anchors.centerIn: parent
        text: {
            if (txStatus === NUNCHUCKTYPE.PENDING_NONCE) {return "Round 1/2"}
            else if (txStatus === NUNCHUCKTYPE.PENDING_SIGNATURES) {return "Round 2/2"}
            else {return "Completed"}
        }
        font.pixelSize: 12
        color: txStatus === NUNCHUCKTYPE.READY_TO_BROADCAST ? "#FFFFFF" : "#1C1C1C"
        font.weight: Font.Bold
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }
}