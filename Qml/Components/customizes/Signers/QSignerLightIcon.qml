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
import NUNCHUCKTYPE 1.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QIcon {
    property int iconSize: 24
    width: iconSize
    height: iconSize
    property string device_type: ""//"coldcard"
    property int type: -1 //NUNCHUCKTYPE.HARDWARE
    property string typeStr: ""
    property string tag: "" //"COLDCARD"
    source: {
        var img = ""
        switch(type) {
        case NUNCHUCKTYPE.HARDWARE: img = ""; break
        case NUNCHUCKTYPE.SOFTWARE: img = "qrc:/Images/Images/Device_Icons/software-key-light.svg"; break
        case NUNCHUCKTYPE.FOREIGN_SOFTWARE: img = "qrc:/Images/Images/Device_Icons/software-key-light.svg"; break
        case NUNCHUCKTYPE.NFC: img = "qrc:/Images/Images/Device_Icons/nfc-key-light.svg"; break
        case NUNCHUCKTYPE.COLDCARD_NFC: img = "qrc:/Images/Images/Device_Icons/coldcard-hardware-light.svg"; break
        case NUNCHUCKTYPE.SERVER: img = ""; break
        default: img = ""
        }
        if (img === "") {
            switch(typeStr) {
            case "HARDWARE": img = ""; break
            case "SOFTWARE": img = "qrc:/Images/Images/Device_Icons/software-key-light.svg"; break
            case "FOREIGN_SOFTWARE": img = "qrc:/Images/Images/Device_Icons/software-key-light.svg"; break
            case "NFC": img = "qrc:/Images/Images/Device_Icons/nfc-key-light.svg"; break
            case "COLDCARD_NFC": img = "qrc:/Images/Images/Device_Icons/coldcard-hardware-light.svg"; break
            case "SERVER": img = ""; break
            default: img = ""
            }
            if (img === "") {
                switch(device_type){
                case "trezor":    img = "qrc:/Images/Images/Device_Icons/trezor-hardware-light.svg"; break
                case "coldcard":  img = "qrc:/Images/Images/Device_Icons/coldcard-hardware-light.svg"; break
                case "bitbox02":  img = "qrc:/Images/Images/Device_Icons/bitbox-hardware-light.svg"; break
                case "ledger":    img = "qrc:/Images/Images/Device_Icons/ledger-hardware-light.svg"; break
                case "software":  img = "qrc:/Images/Images/Device_Icons/software-key-light.svg"; break
                case "nfc"      : img = "qrc:/Images/Images/Device_Icons/nfc-key-light.svg"; break
                case "jade"     : img = "qrc:/Images/Images/Device_Icons/jade-hardware-light.svg"; break
                default: img = ""
                }
                if (img === "") {
                    switch(tag) {
                    case "COLDCARD":    img = "qrc:/Images/Images/Device_Icons/coldcard-hardware-light.svg"; break
                    case "LEDGER":      img = "qrc:/Images/Images/Device_Icons/ledger-hardware-light.svg"; break
                    case "TREZOR":      img = "qrc:/Images/Images/Device_Icons/trezor-hardware-light.svg"; break
                    case "BITBOX":      img = "qrc:/Images/Images/Device_Icons/bitbox-hardware-light.svg"; break
                    case "INHERITANCE": img = "qrc:/Images/Images/Device_Icons/nfc-key-light.svg"; break
                    case "KEYSTONE":    img = "qrc:/Images/Images/Device_Icons/keystone-hardware-light.svg"; break
                    case "JADE":        img = "qrc:/Images/Images/Device_Icons/jade-hardware-light.svg"; break
                    case "PASSPORT":    img = "qrc:/Images/Images/Device_Icons/passport-hardware-light.svg"; break
                    case "SEEDSIGNER":  img = "qrc:/Images/Images/Device_Icons/seedsigner-hardware-light.svg"; break
                    default:img = "qrc:/Images/Images/Device_Icons/unknown-key-light.svg"; break
                    }
                }
            }
        }
        return img
    }

}
