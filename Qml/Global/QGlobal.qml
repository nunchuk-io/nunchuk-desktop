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
pragma Singleton
import QtQuick 2.4
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../localization/STR_QML.js" as STR

QtObject {
    property int    stateID     : -1
    property string fingerPrint : ""
    property int    settingIndex: 0
    property bool   customMsgHealthcheck: false
    property var backgroundColor: [
        ["#F6D65D", "#9CAEB8", "#96833B", "#B8A048"],                  // eFIRST
        ["transparent", "transparent", "#031F2B", "#1A333D"],          // eSECOND
        ["transparent", "#F1FAFE", "#F1FAFE", "#c5cdd1"],              // eTHIRD
        ["transparent", "transparent", "#96833B", "#B8A048"],          // eFOURD
        ["transparent", Qt.rgba(218, 222, 224, 0.1), "transparent", Qt.rgba(218, 222, 224, 0.4)],         // eFIVE

        ["transparent", "transparent", "transparent", "transparent"], // eSIX
        ["transparent", "transparent", "transparent", "transparent"], // eSEVEN
        ["transparent", "transparent", "transparent", "transparent"], // eEIGHT
        ["transparent", "transparent", "transparent", "transparent"], // eNINE
        ["transparent", "transparent", "transparent", "transparent"], // eTEN
        ["transparent", "transparent", "transparent", "transparent"], // eELEVEN
        ["transparent", "transparent", "transparent", "transparent"], //eTWELVE
        ["transparent", "transparent", "transparent", "transparent"], // eTHIRTEEN
        ["transparent", "transparent", "transparent", "transparent"], // eFOURTEEN
        ["transparent", "transparent", "transparent", "transparent"]  // eFIFTEEN
    ]
    property var borderColor: [
        ["transparent", "transparent", "transparent", "#F6D65D"],       // eFIRST
        ["#031F2B", "#9CAEB8", "#031F2B", "#031F2B"],                   // eSECOND
        ["#F1FAFE", "#9CAEB8", "transparent", "#F1FAFE"],               // eTHIRD
        ["#F6D65D", "#839096", "transparent", "#F6D65D"],               // eFOURD
        ["#031F2B", "#031F2B", "#031F2B", "#031F2B"],                   // eFIVE

        ["transparent", "transparent", "transparent", "transparent"],   // eSIX
        ["transparent", "transparent", "transparent", "transparent"],   // eSEVEN
        ["transparent", "transparent", "transparent", "transparent"],   // eEIGHT
        ["transparent", "transparent", "transparent", "transparent"],   // eNINE
        ["transparent", "transparent", "transparent", "transparent"],   // eTEN
        ["transparent", "transparent", "transparent", "transparent"],   // eELEVEN
        ["transparent", "transparent", "transparent", "transparent"],   // eTWELVE
        ["transparent", "transparent", "transparent", "transparent"],   // eTHIRTEEN
        ["transparent", "transparent", "transparent", "transparent"],   // eFOURTEEN
        ["transparent", "transparent", "transparent", "transparent"]    // eFIFTEEN
    ]
    property var textColor: [
        ["#031F2B", "#C9DEF1", "#C9DEF1", "#031F2B"],// eFIRST
        ["#031F2B", "#9CAEB8", "#F1FAFE", "#F1FAFE"],// eSECOND
        ["#F1FAFE", "#9CAEB8", "#031F2B", "#031F2B"],// eTHIRD
        ["#F6D65D", "#839096", "#C9DEF1", "#031F2B"],// eFOURD
        ["#031F2B", "#031F2B", "#031F2B", "#031F2B"],// eFIVE

        ["transparent", "transparent", "transparent", "transparent"],// eSIX
        ["transparent", "transparent", "transparent", "transparent"],// eSEVEN
        ["transparent", "transparent", "transparent", "transparent"],// eEIGHT
        ["transparent", "transparent", "transparent", "transparent"],// eNINE
        ["transparent", "transparent", "transparent", "transparent"],// eTEN
        ["transparent", "transparent", "transparent", "transparent"],// eELEVEN
        ["transparent", "transparent", "transparent", "transparent"],// eTWELVE
        ["transparent", "transparent", "transparent", "transparent"],// eTHIRTEEN
        ["transparent", "transparent", "transparent", "transparent"],// eFOURTEEN
        ["transparent", "transparent", "transparent", "transparent"],// eFIFTEEN
    ]

    function icons(type){
        var img = ""
        switch(type){
        case "trezor":    img = "qrc:/Images/Images/Device_Icons/trezor.png"; break
        case "coldcard":  img = "qrc:/Images/Images/Device_Icons/coldcard.png"; break
        case "bitbox02":  img = "qrc:/Images/Images/Device_Icons/bitbox.png"; break
        case "ledger":    img = "qrc:/Images/Images/Device_Icons/ledger.png"; break
        case "software":  img = "qrc:/Images/Images/Device_Icons/software-key-light.svg"; break
        case "nfc"      : img = "qrc:/Images/Images/Device_Icons/nfc-key-light.svg"; break
        default:          img = "qrc:/Images/Images/Device_Icons/other.png"; break
        }
        return img
    }
    function signers(type){
        var n = ""
        switch(type){
        case NUNCHUCKTYPE.HARDWARE: n = "HARDWARE"; break
        case NUNCHUCKTYPE.AIRGAP:   n = "AIR-GAPPED"; break
        case NUNCHUCKTYPE.SOFTWARE: n = "SOFTWARE"; break
        case NUNCHUCKTYPE.FOREIGN_SOFTWARE: n = "FOREIGN"; break
        case NUNCHUCKTYPE.TAPSIGNER: n = "TAPSIGNER"; break
        default: n = ""
        }
        return n
    }

    function signerNames(type){
        var name = ""
        switch(type){
        case NUNCHUCKTYPE.HARDWARE: name = STR.STR_QML_044; break
        case NUNCHUCKTYPE.AIRGAP:   name = STR.STR_QML_045; break
        case NUNCHUCKTYPE.SOFTWARE: name = STR.STR_QML_046; break
        case NUNCHUCKTYPE.FOREIGN_SOFTWARE: name = STR.STR_QML_047; break
        case NUNCHUCKTYPE.TAPSIGNER: name = STR.STR_QML_678; break
        default: name = ""
        }
        return name
    }
}
