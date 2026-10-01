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
import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import Features.Signers.ViewModels 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"

// BUGFIX: QHardwareRefreshDevices.qml is content-only (no title/Prev/Continue chrome) and expects
// an ancestor-provided "vm" id - it was never meant to be shown directly as a screen. It was being
// registered and shown bare via subMng->show(...) for Ledger/Trezor/Jade/BitBox, which left it with
// no screen chrome and an undefined "vm" (bindings silently falling back/breaking). This wraps it
// the same way QColdcardRefreshDevices.qml already wraps it for COLDCARD.
QOnScreenContentTypeA {
    id: _refresh
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: vm.headline
    onCloseClicked: vm.close()
    onPrevClicked: vm.back()
    content: QHardwareRefreshDevices {
    }
    bottomRight: QTextButton {
        width: label.paintedWidth + 32
        height: 48
        label.text: QSTR.STR_QML_265
        label.font.pixelSize: 16
        type: eTypeE
        enabled: _refresh.contentItem.isEnable()
        onButtonClicked: {
            vm.checkSignerExist()
        }
    }

    HardwareRefreshDevicesViewModel {
        id: vm
    }
}
