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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

Item {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    Connections {
        target: topUpXpub
        function onNeedTopUpXpub() {
            _topUpXpub.open()
        }
    }
    QPopupInfoTwoButtons {
        id: _topUpXpub
        title: STR.STR_QML_661
        contentText: STR.STR_QML_1877
        labels: [STR.STR_QML_560,STR.STR_QML_035]
        funcs: [
            function() {
                var signerData = _content.contentItem.selectedKey
                signerData["isTopUpXpub"] = true
                AppModel.startScanDevices(signerData)
             },
            function() {
                _topUpXpub.close()
            }
        ]
    }

    QPopupBusyLoading{
        id:_busyTopUp
        warningText1:STR.STR_QML_582
    }

    Connections {
        target: AppModel
        onStartTopXPUBsSigner:{
            _busyTopUp.open()
        }
        onFinishTopXPUBsSigner:{
            _busyTopUp.close()
        }
    }
}