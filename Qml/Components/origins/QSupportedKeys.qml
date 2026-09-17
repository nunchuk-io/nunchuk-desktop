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
        { type: NUNCHUCKTYPE.ADD_KEYSTONE,   name: "Keystone",          device_type: "keystone",   tag: "KEYSTONE"    },
        { type: NUNCHUCKTYPE.ADD_PASSPORT,   name: "Foundation Passport", device_type: "passport", tag: "PASSPORT"    },
        // KEEPKEY: wired add-key flow, reuses Trezor UI. KRUX: goes through the generic AIRGAP/remote flow.
        // device_type is assumed (unconfirmed), only affects fallback icon selection.
        { type: NUNCHUCKTYPE.ADD_KEEPKEY,    name: "KeepKey",           device_type: "keepkey",    tag: "KEEPKEY"     },
        { type: NUNCHUCKTYPE.ADD_KRUX,       name: "Krux",              device_type: "krux",       tag: "KRUX"        },
    ]

    // BUGFIX: supported_signers[] entries can differ by wallet_type for the same tag (NUN-10192).
    readonly property string walletType: isMiniscript ? "MINISCRIPT" : "MULTI_SIG"

    // claim_options/claim_note available for a key type, from backend config (NUN-10192).
    function claimOptionsFor(tag) {
        return SignerManagement.claimOptionsForTag(tag, walletType)
    }
    function claimNoteFor(tag) {
        return SignerManagement.claimNoteForTag(tag, walletType)
    }

    // KEYHOLDER_LIMITED restriction, unrelated to is_inheritance_key: blocks self-adding BitBox/COLDCARD.
    function isKeyHolderLimitedRestricted(tag) {
        return isKeyHolderLimited && (tag === "BITBOX" || tag === "COLDCARD")
    }

    // Inheritance-key support is backend-driven (supported_signers[].is_inheritance_key), all wallet types.
    function isSupportedInheritance(tag) {
        if (isKeyHolderLimitedRestricted(tag)) return false
        return SignerManagement.isSupportedInheritance(tag, walletType)
    }
    function isSupportedNotInheritance(tag) {
        if (isKeyHolderLimitedRestricted(tag)) return false
        return SignerManagement.isSupportedNotInheritance(tag, walletType)
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
