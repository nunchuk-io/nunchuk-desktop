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
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12

Rectangle {
    id: root
    color: "#66D0E2FF"
    width: parent.width
    height: vm.beneficiary_mode === "SINGLE" ? 167 : (overviewRow.implicitHeight + 48)

    // Signals for child components
    signal assetAllocationEditClicked()
    signal releaseMethodEditClicked()
    signal releaseScheduleEditClicked()
    signal timezoneEditClicked()
    signal fallbackSettingsEditClicked()
    signal beneficiarySchedulesEditClicked()

    Row {
        id: overviewRow
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            leftMargin: 48
            rightMargin: 48
            topMargin: 24
        }
        spacing: 16
        Column {
            width: parent.width / 2 - 8
            spacing: 12
            QReleaseSchedule {
                isSingle: vm.beneficiary_mode === "SINGLE"
                firstWithdrawalDate: vm.firstWithdrawalDate
                buffer_period: vm.buffer_period
                buffer_apply_on: vm.buffer_apply_on
                timelineStages: vm.timelineStagesData
                onEditClicked: root.releaseScheduleEditClicked()
                height: 119
                visible: vm.beneficiary_mode === "SINGLE"
            }
            QAssetAllocation {
                allocationData: vm.assetAllocation
                releaseMethod: vm.release_method
                onAssetAllocationEditClicked: root.assetAllocationEditClicked()
                onReleaseMehodEditClicked: root.releaseMethodEditClicked()
                visible: vm.beneficiary_mode !== "SINGLE"
            }
            QTimezoneDisplay {
                timezone: vm.valueTimezone
                onEditClicked: root.timezoneEditClicked()
                visible: vm.beneficiary_mode !== "SINGLE"
                height: vm.release_method === "SHARED" ? 96 : 76
            }

            QFallbackSettings {
                fallback_interval_count: vm.fallback_interval_count
                fallback_interval: vm.fallback_interval
                fallback_dateTime: vm.fallback_dateTime
                fallback_type: vm.fallback_type
                onEditClicked: root.fallbackSettingsEditClicked()
                visible: vm.beneficiary_mode !== "SINGLE" && vm.release_method === "INDIVIDUAL"
            }
        }

        Column {
            width: parent.width / 2 - 8
            spacing: 12
            QReleaseSchedule {
                isSingle: vm.beneficiary_mode === "SINGLE"
                firstWithdrawalDate: vm.firstWithdrawalDate
                buffer_period: vm.buffer_period
                buffer_apply_on: vm.buffer_apply_on
                timelineStages: vm.timelineStagesData
                onEditClicked: root.releaseScheduleEditClicked()
                height: 182
                visible: vm.beneficiary_mode !== "SINGLE" && vm.release_method === "SHARED"
            }

            QBeneficiarySchedules {
                id: beneficiarySchedules
                beneficiaries: vm.beneficiariesData
                onEditClicked: root.beneficiarySchedulesEditClicked()
                visible: vm.beneficiary_mode !== "SINGLE" && vm.release_method === "INDIVIDUAL"
            }

            QFallbackSettings {
                fallback_interval_count: vm.fallback_interval_count
                fallback_interval: vm.fallback_interval
                fallback_dateTime: vm.fallback_dateTime
                fallback_type: vm.fallback_type
                onEditClicked: root.fallbackSettingsEditClicked()
                visible: vm.beneficiary_mode !== "SINGLE" && vm.release_method === "SHARED"
                height: 96
            }

            QTimezoneDisplay {
                timezone: vm.valueTimezone
                onEditClicked: root.timezoneEditClicked()
                visible: vm.beneficiary_mode === "SINGLE"
                height: vm.beneficiary_mode === "SINGLE" ? 119 : (beneficiarySchedules.beneficiaries.length > 0 ? 76 : 96)
            }
        }
    }
}
