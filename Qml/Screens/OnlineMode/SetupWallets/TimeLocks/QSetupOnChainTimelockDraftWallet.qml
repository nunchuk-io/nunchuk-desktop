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
import Features.Draftwallets.OnChain.ViewModels 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../Components/customizes/Popups"
import "../../../../../localization/STR_QML.js" as STR

QSetupOnChainTimelock {
    id: _draftTimelock
    readonly property var dash: GroupWallet.dashboardInfo
    readonly property bool configurable: dash && dash.isConfigurablePolicy === true
    readonly property int fallbackIndex: dash ? dash.editingFallbackIndex : -1
    readonly property var fallbacks: configurable ? dash.fallbackPolicies : []
    readonly property var primaryLock: configurable && dash.primaryPolicy.timelock ? dash.primaryPolicy.timelock : ({})

    function fallbackAt(index) {
        for (var i = 0; i < fallbacks.length; i++) {
            if (fallbacks[i].index === index) return fallbacks[i]
        }
        return null
    }
    function lockOf(item) {
        return item && item.timelock ? item.timelock : ({})
    }
    function hasLock(lock) {
        return lock && lock.timestamp !== undefined && lock.timestamp > 0
    }
    readonly property var current: fallbackIndex >= 0 ? fallbackAt(fallbackIndex) : null
    readonly property bool isEdit: fallbackIndex >= 0 ? hasLock(lockOf(current)) : hasLock(primaryLock)
    readonly property int number: fallbackIndex + 1
    readonly property var prevLock: fallbackIndex === 0 ? primaryLock : lockOf(fallbackAt(fallbackIndex - 1))
    readonly property var nextLock: fallbackIndex >= 0 ? lockOf(fallbackAt(fallbackIndex + 1)) : lockOf(fallbackAt(0))
    readonly property bool hasNext: fallbackIndex >= 0 ? fallbackAt(fallbackIndex + 1) !== null : fallbackAt(0) !== null

    titleText: !configurable ? (vm.valueDate.length > 0 ? QSTR.STR_QML_2076 : QSTR.STR_QML_1992)
               : fallbackIndex < 0 ? (isEdit ? STR.STR_QML_2395 : STR.STR_QML_2394)
               : (isEdit ? STR.STR_QML_2397.arg(number) : STR.STR_QML_2396.arg(number))
    descriptions: !configurable ? [QSTR.STR_QML_1993, QSTR.STR_QML_1994, QSTR.STR_QML_1995]
                  : fallbackIndex < 0 ? [STR.STR_QML_2398, STR.STR_QML_2402]
                  : isEdit ? [STR.STR_QML_2401]
                  : [fallbackIndex === 0 ? STR.STR_QML_2399 : STR.STR_QML_2400.arg(number).arg(number - 1), STR.STR_QML_2402]
    references: {
        if (!configurable || fallbackIndex < 0) return []
        if (!isEdit) {
            return [{ label: fallbackIndex === 0 ? STR.STR_QML_2403 : STR.STR_QML_2404.arg(number - 1),
                      value: hasLock(prevLock) ? prevLock.displayWithZone : STR.STR_QML_2407 }]
        }
        var refs = [{ label: fallbackIndex === 0 ? STR.STR_QML_2415 : STR.STR_QML_2405.arg(number - 1),
                      value: hasLock(prevLock) ? prevLock.displayWithZone : STR.STR_QML_2407 }]
        if (hasNext) {
            refs.push({ label: STR.STR_QML_2406.arg(number + 1),
                        value: hasLock(nextLock) ? nextLock.displayWithZone : STR.STR_QML_2407 })
        }
        return refs
    }
    infoBelowInputs: configurable
    infoText: !configurable ? QSTR.STR_QML_1996
              : fallbackIndex < 0 ? STR.STR_QML_2408
              : (current && current.n > 1 ? STR.STR_QML_2410.arg(number) : STR.STR_QML_2409.arg(number))
    validateSelection: configurable ? function(timestamp) {
        if (fallbackIndex < 0) {
            if (hasNext && hasLock(nextLock) && timestamp >= nextLock.timestamp) {
                return STR.STR_QML_2414.arg(nextLock.display)
            }
            return ""
        }
        if (hasLock(prevLock) && timestamp <= prevLock.timestamp) {
            var prevName = fallbackIndex === 0 ? STR.STR_QML_2413 : STR.STR_QML_2356.arg(number - 1)
            return STR.STR_QML_2411.arg(number).arg(prevName).arg(prevLock.display)
        }
        if (hasNext && hasLock(nextLock) && timestamp >= nextLock.timestamp) {
            return STR.STR_QML_2412.arg(number).arg(number + 1).arg(nextLock.display)
        }
        return ""
    } : null

    SetupAnOnChainTimelockViewModel {
        id: vm
    }
}
