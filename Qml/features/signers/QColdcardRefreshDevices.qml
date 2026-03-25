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
import Features.Signers.ViewModels 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"

QOnScreenContentTypeA {
    id: _refresh
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_904
    onCloseClicked: vm.close()
    onPrevClicked: vm.back()
    content: QHardwareRefreshDevices {
    }
    bottomRight: Row {
        spacing: 12
        QIconTextButton {
            width: 244
            height: 48
            label: STR.STR_QML_1922
            icons: ["QR-dark.svg", "QR-dark.svg", "QR-dark.svg", "QR-dark.svg"]
            fontPixelSize: 16
            iconSize: 16
            type: eTypeB
            onButtonClicked: vm.onViaQRClicked()
        }
        QIconTextButton {
            width: 244
            height: 48
            label: STR.STR_QML_1050
            icons: ["importFile.svg", "importFile.svg", "importFile.svg", "importFile.svg"]
            fontPixelSize: 16
            iconSize: 16
            type: eTypeB
            onButtonClicked: vm.onViaFileClicked()
        }
        QTextButton {
            width: 120
            height: 48
            label.text: STR.STR_QML_265
            label.font.pixelSize: 16
            type: eTypeE
            enabled: _refresh.contentItem.isEnable()
            onButtonClicked: {
                vm.checkSignerExist()
            }
        }

    }

    ColdcardRefreshDevicesViewModel {
        id: vm
    }
}
