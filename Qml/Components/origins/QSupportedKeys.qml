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
import NUNCHUCKTYPE 1.0

QtObject {
    id: supportedKeys
    property bool isKeyHolderLimited: false
    property bool isMiniscript: false
    property bool isInheritance: false
    
    readonly property var allKeys: [
        { type: NUNCHUCKTYPE.ADD_BITBOX,     name: "BitBox",            device_type: "bitbox02",   tag: "BITBOX"      },
        { type: NUNCHUCKTYPE.ADD_COLDCARD,   name: "COLDCARD",          device_type: "coldcard",   tag: "COLDCARD"    },
        { type: NUNCHUCKTYPE.ADD_JADE,       name: "Blockstream Jade",  device_type: "jade",       tag: "JADE"        },
        { type: NUNCHUCKTYPE.ADD_LEDGER,     name: "Ledger",            device_type: "ledger",     tag: "LEDGER"      },
        { type: NUNCHUCKTYPE.ADD_TREZOR,     name: "Trezor",            device_type: "trezor",     tag: "TREZOR"      },
        { type: NUNCHUCKTYPE.ADD_TAPSIGNER,  name: "TAPSIGNER",         device_type: "tapsigner",  tag: "INHERITANCE" },
    ]
    function isSupportedInheritance(tag) {
        if (isMiniscript) {
            return SignerManagement.isSupportedInheritance(tag)
        } else {
            switch (tag) {
            case "COLDCARD":
                return !isKeyHolderLimited
            case "INHERITANCE": // TAPSIGNER
                return true
            default:
                return false
            }
        }
    }
    function isSupportedNotInheritance(tag) {
        if (isMiniscript) {
            return SignerManagement.isSupportedNotInheritance(tag)
        } else {
            switch (tag) {
            case "BITBOX":
            case "COLDCARD":
                return !isKeyHolderLimited
            case "LEDGER":
            case "TREZOR":
            case "JADE":
                return true
            default:
                return false
            }
        }
    }

    function listSupportedKeys() {
        var isSupported = isInheritance
                          ? function(tag) { return isSupportedInheritance(tag) }
                          : function(tag) { return isSupportedNotInheritance(tag) }

        // Filter keys by support predicate, ignoring invalid entries
        var ret = allKeys.filter(function(key) {
            return key && key.tag && isSupported(key.tag)
        })
        console.log("Supported Keys: ", ret)
        return ret
    }
}
