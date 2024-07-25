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
    property int    listFocusing: 0
    property string recoverSoftwareType: "seed"

    property var backgroundColor: [
        ["#F6D65D", "#96833B", "#9CAEB8", "#B8A048"],
        ["transparent", "#031F2B", "transparent", "#1A333D"],
        ["transparent", Qt.rgba(218, 222, 224, 0.1), "transparent", Qt.rgba(218, 222, 224, 0.4)],
        ["transparent", "#9FA0A1","transparent", "#8B8C8C"],
        ["#031F2B", "#1A333D","#EAEAEA", "#031F2B"],
        ["#FFFFFF", "#031F2B","#EAEAEA", "#031F2B"],
        ["#FFFFFF", "#031F2B","#EAEAEA", "#1A333D"],

        ["#F6D65D", "#9CAEB8", "#96833B", "#B8A048"],
        ["transparent", "transparent", "#031F2B", "#1A333D"],

        ["#C9DEF1", "#9CAEB8", "#031F2B", "#C9DEF1"],
        ["transparent", "transparent", "#031F2B" ,"transparent"],
        ["transparent", "transparent", "#F1FAFE", "transparent"],
        ["#D0E2FF", "#FFFFFF", "transparent", "#D0E2FF"],
        ["#031F2B", "#1A333D","#031F2B", "#031F2B"],
        ["transparent", "#1A333D", "transparent", "transparent"],

        ["transparent", "transparent", "transparent", "transparent"], // eFOURTEEN
        ["transparent", "transparent", "transparent", "transparent"]  // eFIFTEEN
    ]
    property var borderColor: [
        ["transparent", "transparent", "transparent", "#F6D65D"],
        ["#031F2B", "#031F2B", "#9CAEB8", "#031F2B"],
        ["#F1FAFE", Qt.rgba(241, 250, 254, 0.4), "#F1FAFE", "#F1FAFE"],
        ["#031F2B", "#031F2B", "#031F2B", "#031F2B"],
        ["#031F2B", "#031F2B", "#EAEAEA", "#031F2B"],
        ["#FFFFFF", "#031F2B", "#EAEAEA", "#595959"],
        ["#FFFFFF", "#FFFFFF", "#EAEAEA", "#FFFFFF"],

        ["transparent", "transparent", "transparent", "#F6D65D"],
        ["#031F2B", "#9CAEB8", "#031F2B", "#031F2B"],

        ["transparent", "transparent", "transparent","transparent"],
        ["#031F2B", "#9CAEB8", "#031F2B", "#031F2B"],
        ["#F1FAFE", "#839096", "transparent", "#F1FAFE"],
        ["transparent", "transparent", "transparent", "transparent"],
        ["#031F2B", "#031F2B", "#EAEAEA", "#031F2B"],
        ["transparent", "transparent", "transparent", "transparent"],

        ["transparent", "transparent", "transparent", "transparent"],   // eFOURTEEN
        ["transparent", "transparent", "transparent", "transparent"]    // eFIFTEEN
    ]
    property var textColor: [
        ["#031F2B", "#C9DEF1", "#C9DEF1", "#031F2B"],
        ["#031F2B", "#F1FAFE", "#9CAEB8", "#F1FAFE"],
        ["#F1FAFE", "#F1FAFE", "#F1FAFE", "#F1FAFE"],
        ["#031F2B", "#031F2B", "#031F2B", "#031F2B"],
        ["#FFFFFF", "#FFFFFF", "#595959", "#FFFFFF"],
        ["#031F2B", "#FFFFFF", "#595959", "#FFFFFF"],
        ["#031F2B", "#FFFFFF", "#595959", "#FFFFFF"],

        ["#031F2B", "#C9DEF1", "#031F2B", "#031F2B"],
        ["#031F2B", "#9CAEB8", "#F1FAFE", "#F1FAFE"],

        ["#031F2B", "#C9DEF1", "#C9DEF1", "#031F2B"],
        ["#031F2B", "#9CAEB8", "#F1FAFE", "#031F2B"],
        ["#F1FAFE", "#839096", "#031F2B", "#F1FAFE"],
        ["#031F2B", "#031F2B", "#031F2B", "#031F2B"],
        ["#FFFFFF", "#FFFFFF", "#FFFFFF", "#FFFFFF"],
        ["#FFFFFF", "#FFFFFF", "#FFFFFF", "#FFFFFF"],

        ["#031F2B", "#031F2B", "#031F2B", "#031F2B"],
        ["transparent", "transparent", "transparent", "transparent"],// eFIFTEEN
    ]

    function signers(type){
        switch(type){
        case NUNCHUCKTYPE.HARDWARE:          return "HARDWARE";
        case NUNCHUCKTYPE.AIRGAP:            return "AIRGAPPED";
        case NUNCHUCKTYPE.SOFTWARE:          return "SOFTWARE";
        case NUNCHUCKTYPE.FOREIGN_SOFTWARE:  return "FOREIGN";
        case NUNCHUCKTYPE.NFC:               return "NFC";
        case NUNCHUCKTYPE.COLDCARD_NFC:      return "COLDCARD-NFC";
        case NUNCHUCKTYPE.SERVER:            return "PLATFORM";
        case NUNCHUCKTYPE.UNKNOWN:           return "UNKNOWN";
        default:                             return "UNKNOWN";
        }
    }

    property int countRandom : 0
    property var generatedValues: [0, 1, 2]
    function swapPositions() {
        let temp = generatedValues[countRandom % 3];
        generatedValues[countRandom % 3] = generatedValues[(countRandom + 1) % 3];
        generatedValues[(countRandom + 1) % 3] = temp;
        countRandom++;
        if (countRandom > 5) {
            countRandom = 0;
        }
        return generatedValues;
    }

    function transactionColor(status){
        switch(status){
        case NUNCHUCKTYPE.PENDING_SIGNATURES:   return "#FFD7D9";
        case NUNCHUCKTYPE.READY_TO_BROADCAST:   return "#FDEBD2";
        case NUNCHUCKTYPE.NETWORK_REJECTED:     return "#CF4018";
        case NUNCHUCKTYPE.PENDING_CONFIRMATION: return "#E8DAFF";
        case NUNCHUCKTYPE.CONFIRMED:            return "#D0E2FF";
        default:                                return "#FFD7D9"
        }
    }

    function transactionStatus(status, confirmation){
        switch(status){
        case NUNCHUCKTYPE.PENDING_SIGNATURES:   return STR.STR_QML_283 ;
        case NUNCHUCKTYPE.READY_TO_BROADCAST:   return STR.STR_QML_284 ;
        case NUNCHUCKTYPE.NETWORK_REJECTED:     return STR.STR_QML_285 ;
        case NUNCHUCKTYPE.PENDING_CONFIRMATION: return STR.STR_QML_286 ;
        case NUNCHUCKTYPE.CONFIRMED:            return (qsTr("%1 %2").arg(confirmation).arg(STR.STR_QML_287)) ;
        default:                                return STR.STR_QML_456
        }
    }

    function getHealthStatusColor(status) {
        if (status === "LessThan6months") {
            return "#A7F0BA"
        }
        else if (status === "MoreThan6months") {
            return "#FDEBD2"
        }
        else if (status === "MoreThan1year") {
            return "#FFD7D9"
        }
        else{
            return "#FFD7D9";
        }
    }
    function getHealthStatusLabel(status) {
        if (status === "LessThan6months") {
            return STR.STR_QML_975
        }
        else if (status === "MoreThan6months") {
            return STR.STR_QML_976
        }
        else if (status === "MoreThan1year") {
            return STR.STR_QML_977
        }
        else{
            return "Not checked yet"
        }
    }
}
