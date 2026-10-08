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
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../OnlineMode/AddHardwareKeys"
import "../../../../localization/STR_QML.js" as STR


QOnScreenContentTypeA {
    id: _refresh
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: {
        var type = _HARDWARE_TYPE
        if (type == "ledger") {
            return STR.STR_QML_811
        } else if (type == "trezor") {
            return STR.STR_QML_814
        } else if (type == "coldcard") {
            return STR.STR_QML_904
        } else if (type == "bitbox02") {
            return STR.STR_QML_923
        } else {
            return "Unknown"
        }
    }
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    readonly property string _HARDWARE_TYPE: walletInfo.deviceType
    readonly property string _HARDWARE: {
        var type = _HARDWARE_TYPE
        if (type == "ledger") {
            return "Ledger"
        } else if (type == "trezor") {
            return "Trezor"
        } else if (type == "coldcard") {
            return "Coldcard"
        } else if (type == "bitbox02") {
            return "BitBox"
        } else {
            return "Unknown"
        }
    }

    content: QAddKeyRefreshDevices {
        title: {
            var type = _HARDWARE_TYPE
            if (type == "ledger") {
                return STR.STR_QML_824
            } else if (type == "trezor") {
                return STR.STR_QML_830
            } else if (type == "coldcard") {
                return STR.STR_QML_911
            } else if (type == "bitbox02") {
                return STR.STR_QML_923
            } else {
                return "Unknown"
            }
        }
        state_id: EVT.STATE_ID_SCR_ADD_HARDWARE
        Connections {
            target: walletInfo
            onDeviceListChanged: {
                mDevicelist.currentIndex = -1
            }
        }

        function addDevice() {
            if(mDevicelist.currentIndex !== -1){
                var _input = {
                    type: "replace-add-signer",
                    signerNameInputted    : signerName,
                    deviceIndexSelected   : mDevicelist.currentIndex
                }
                QMLHandle.sendEvent(EVT.EVT_REPLACE_KEYS_ENTER, _input)
            }
        }
    }

    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    bottomRight: Row {
        spacing: 12
        QIconTextButton {
            width: 244
            height: 48
            label: STR.STR_QML_1050
            icons: ["importFile.svg", "importFile.svg", "importFile.svg","importFile.svg"]
            fontPixelSize: 16
            iconSize: 16
            type: eTypeB
            visible: _HARDWARE_TYPE == "coldcard"
            onButtonClicked: {
                setReplaceFlow("replace-AddColdcardViaAFile")
            }
        }
        QTextButton {
            width: 120
            height: 48
            label.text: STR.STR_QML_265
            label.font.pixelSize: 16
            type: eTypeE
            enabled: _refresh.contentItem.isEnable()
            onButtonClicked: {
                _refresh.contentItem.addDevice()
            }
        }
    }
}
