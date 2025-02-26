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
    property bool   showOthersKeyset: true

    readonly property color color1: Qt.rgba(241, 250, 254, 0.2)

    property var backgroundColor: [
        {id: 0, normal: "#F6D65D",     hover: "#96833B",                   disable: "#9CAEB8",     clicked: "#B8A048"},
        {id: 1, normal: "transparent", hover: "#031F2B",                   disable: "transparent", clicked: "#1A333D"},
        {id: 2, normal: "transparent", hover: Qt.rgba(218, 222, 224, 0.1), disable: "transparent", clicked: Qt.rgba(218, 222, 224, 0.4)},
        {id: 3, normal: "transparent", hover: "#9FA0A1",                   disable: "transparent", clicked: "#8B8C8C"},
        {id: 4, normal: "#031F2B",     hover: "#1A333D",                   disable: "#EAEAEA",     clicked: "#031F2B"},
        {id: 5, normal: "#FFFFFF",     hover: "#031F2B",                   disable: "#EAEAEA",     clicked: "#031F2B"},
        {id: 6, normal: "#FFFFFF",     hover: "#031F2B",                   disable: "#EAEAEA",     clicked: "#1A333D"},
        {id: 7, normal: "#F6D65D",     hover: "#9CAEB8",                   disable: "#96833B",     clicked: "#B8A048"},
        {id: 8, normal: "transparent", hover: "transparent",               disable: "#031F2B",     clicked: "#1A333D"},
        {id: 9, normal: "#C9DEF1",     hover: "#9CAEB8",                   disable: "#031F2B",     clicked: "#C9DEF1"},
        {id:10, normal: "transparent", hover: "transparent",               disable: "#031F2B",     clicked: "transparent"},
        {id:11, normal: "transparent", hover: "transparent",               disable: "#F1FAFE",     clicked: "transparent"},
        {id:12, normal: "#D0E2FF",     hover: "#FFFFFF",                   disable: "transparent", clicked: "#D0E2FF"},
        {id:13, normal: "#031F2B",     hover: "#1A333D",                   disable: "#031F2B",     clicked: "#031F2B"},
        {id:14, normal: "transparent", hover: "#1A333D",                   disable: "transparent", clicked: "transparent"},
        {id:15, normal: "transparent", hover: "transparent",               disable: "transparent", clicked: "transparent"},
        {id:16, normal: color1,        hover: color1,                      disable: color1,        clicked: color1},
    ]
    property var borderColor: [
        {id: 0, normal: "transparent", hover: "transparent",               disable: "transparent",     clicked: "#F6D65D"},
        {id: 1, normal: "#031F2B",     hover: "#031F2B",                   disable: "#9CAEB8",         clicked: "#031F2B"},
        {id: 2, normal: "#F1FAFE",     hover: Qt.rgba(241, 250, 254, 0.4), disable: "#F1FAFE",         clicked: "#F1FAFE"},
        {id: 3, normal: "#031F2B",     hover: "#031F2B",                   disable: "#031F2B",         clicked: "#031F2B"},
        {id: 4, normal: "#031F2B",     hover: "#031F2B",                   disable: "#EAEAEA",         clicked: "#031F2B"},
        {id: 5, normal: "#FFFFFF",     hover: "#031F2B",                   disable: "#EAEAEA",         clicked: "#595959"},
        {id: 6, normal: "#FFFFFF",     hover: "#FFFFFF",                   disable: "#EAEAEA",         clicked: "#FFFFFF"},
        {id: 7, normal: "transparent", hover: "transparent",               disable: "transparent",     clicked: "#F6D65D"},
        {id: 8, normal: "#031F2B",     hover: "#9CAEB8",                   disable: "#031F2B",         clicked: "#031F2B"},
        {id: 9, normal: "transparent", hover: "transparent",               disable: "transparent",     clicked: "transparent"},
        {id:10, normal: "#031F2B",     hover: "#9CAEB8",                   disable: "#031F2B",         clicked: "#031F2B"},
        {id:11, normal: "#F1FAFE",     hover: "#839096",                   disable: "transparent",     clicked: "#F1FAFE"},
        {id:12, normal: "transparent", hover: "transparent",               disable: "transparent",     clicked: "transparent"},
        {id:13, normal: "#031F2B",     hover: "#031F2B",                   disable: "#EAEAEA",         clicked: "#031F2B"},
        {id:14, normal: "transparent", hover: "transparent",               disable: "transparent",     clicked: "transparent"},
        {id:15, normal: "transparent", hover: "transparent",               disable: "transparent",     clicked: "transparent"},
        {id:16, normal: color1,        hover: color1,                      disable: color1,            clicked: color1},
    ]
    property var textColor: [
        {id: 0, normal: "#031F2B",     hover: "#C9DEF1",     disable: "#C9DEF1",     clicked: "#031F2B"},
        {id: 1, normal: "#031F2B",     hover: "#F1FAFE",     disable: "#9CAEB8",     clicked: "#F1FAFE"},
        {id: 2, normal: "#F1FAFE",     hover: "#F1FAFE",     disable: "#F1FAFE",     clicked: "#F1FAFE"},
        {id: 3, normal: "#031F2B",     hover: "#031F2B",     disable: "#031F2B",     clicked: "#031F2B"},
        {id: 4, normal: "#FFFFFF",     hover: "#FFFFFF",     disable: "#595959",     clicked: "#FFFFFF"},
        {id: 5, normal: "#031F2B",     hover: "#FFFFFF",     disable: "#595959",     clicked: "#FFFFFF"},
        {id: 6, normal: "#031F2B",     hover: "#FFFFFF",     disable: "#595959",     clicked: "#FFFFFF"},
        {id: 7, normal: "#031F2B",     hover: "#C9DEF1",     disable: "#031F2B",     clicked: "#031F2B"},
        {id: 8, normal: "#031F2B",     hover: "#9CAEB8",     disable: "#F1FAFE",     clicked: "#F1FAFE"},
        {id: 9, normal: "#031F2B",     hover: "#C9DEF1",     disable: "#C9DEF1",     clicked: "#031F2B"},
        {id:10, normal: "#031F2B",     hover: "#9CAEB8",     disable: "#F1FAFE",     clicked: "#031F2B"},
        {id:11, normal: "#F1FAFE",     hover: "#839096",     disable: "#031F2B",     clicked: "#F1FAFE"},
        {id:12, normal: "#031F2B",     hover: "#031F2B",     disable: "#031F2B",     clicked: "#031F2B"},
        {id:13, normal: "#FFFFFF",     hover: "#FFFFFF",     disable: "#FFFFFF",     clicked: "#FFFFFF"},
        {id:14, normal: "#FFFFFF",     hover: "#FFFFFF",     disable: "#FFFFFF",     clicked: "#FFFFFF"},
        {id:15, normal: "#031F2B",     hover: "#031F2B",     disable: "#031F2B",     clicked: "#031F2B"},
        {id:16, normal: "#FFFFFF",     hover: "#FFFFFF",     disable: "#FFFFFF",     clicked: "#FFFFFF"},
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
