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

QBadge {
    property string device_type: ""//"coldcard"
    property int type: -1 //NUNCHUCKTYPE.HARDWARE
    property string typeStr: ""
    property string tag: "" //"COLDCARD"
    text: {
        var value = ""
        switch(type) {
        case NUNCHUCKTYPE.HARDWARE: value = ""; break
        case NUNCHUCKTYPE.AIRGAP:   value = "Airgapped"; break
        case NUNCHUCKTYPE.SOFTWARE: value = "Software"; break
        case NUNCHUCKTYPE.FOREIGN_SOFTWARE: value = "Foreign software"; break
        case NUNCHUCKTYPE.NFC: value = "NFC"; break
        case NUNCHUCKTYPE.COLDCARD_NFC: value = "COLDCARD-NFC"; break
        case NUNCHUCKTYPE.SERVER: value = ""; break
        default: value = ""
        }
        if (value === "") {
            switch(typeStr) {
            case "HARDWARE": value = ""; break
            case "AIRGAP":   value = "Airgapped"; break
            case "SOFTWARE": value = "Software"; break
            case "FOREIGN_SOFTWARE": value = "Foreign software"; break
            case "NFC": value = "NFC"; break
            case "COLDCARD_NFC": value = "COLDCARD-NFC"; break
            case "SERVER": value = ""; break
            default: value = ""
            }
            if (value === "") {
                switch(device_type){
                case "trezor":    value = "Wired"; break
                case "coldcard":  value = "Wired"; break
                case "bitbox02":  value = "Wired"; break
                case "ledger":    value = "Wired"; break
                case "jade":      value = "Wired"; break
                case "software":  value = "Software"; break
                case "nfc"      : value = "NFC"; break
                default: value = ""
                }
                if (value === "") {
                    switch(tag) {
                    case "COLDCARD":    value = "Wired"; break
                    case "LEDGER":      value = "Wired"; break
                    case "TREZOR":      value = "Wired"; break
                    case "BITBOX":      value = "Wired"; break
                    case "JADE":        value = "Wired"; break
                    case "INHERITANCE":
                    case "KEYSTONE":
                    case "PASSPORT":
                    case "SEEDSIGNER":
                    default:value = "Unknown"; break
                    }
                }
            }
        }
        return value
    }
}
