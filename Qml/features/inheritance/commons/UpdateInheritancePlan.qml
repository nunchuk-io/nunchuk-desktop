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
import "../../../Components/customizes"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/origins"
import "../components"
import DataPool 1.0
import EWARNING 1.0
import Features.Inheritance.Common.ViewModels 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QtGraphicalEffects 1.12
import QtQuick 2.12
import QtQuick.Controls 2.3

QOnScreenContentTypeA {
    id: root
    
    UpdateInheritancePlanViewModel {
        id: vm
    }
    
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: vm.title
    extraHeader: Item {}
    onCloseClicked: vm.close()
    property string title: STR.STR_QML_1036.arg(vm.requester_user.name).arg(vm.requester_user.email).arg(vm.walletName).replace("()","")
    content: Item {
        id: _item
        Column {
            spacing: 16
            QLato {
                width: 656
                height: paintedHeight
                text: title
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
                visible: title.length > 0
            }
            Flickable {
                width: 656 + 12
                height: 452
                contentHeight: _col.childrenRect.height + 12
                ScrollBar.vertical: ScrollBar { active: true }
                clip: true
                Column {
                    id: _col
                    width: parent.width - 12
                    spacing: 16
                    QDbAlertAssetAllocation {
                        allocationData: vm.assetAllocation
                        releaseMethod: vm.release_method
                        isReleaseMothodChanged: vm.is_release_method_changed
                        visible: vm.beneficiary_mode !== "SINGLE"
                    }
                    QDbAlertBeneficiarySchedules {
                        beneficiaries: vm.beneficiariesData
                        visible: vm.beneficiary_mode !== "SINGLE" && vm.release_method === "INDIVIDUAL"
                    }
                    QDbAlertTimezone {
                        timezone: vm.timezoneDisplay
                        timezoneChanged: vm.isTimezoneDisplayChanged
                    }
                    QDbAlertFallbackSettings {
                        inactivity_interval_count: vm.inactivity_interval_count
                        inactivity_interval_countChanged: vm.isInactivityIntervalCountChanged
                        visible: vm.beneficiary_mode !== "SINGLE"
                    }
                    QDbAlertReleaseSchedule {
                        isSingle: vm.beneficiary_mode === "SINGLE"
                        firstWithdrawalDate: vm.firstWithdrawalDate
                        firstWithdrawalDateChanged: vm.is_firstWithdrawalDateChanged
                        buffer_period: vm.buffer_period
                        buffer_period_changed: vm.is_buffer_period_changed
                        buffer_apply_on: vm.buffer_apply_on
                        buffer_apply_on_changed: vm.is_buffer_apply_on_changed
                        timelineStages: vm.timelineStagesData
                        height: 119
                        visible: vm.beneficiary_mode === "SINGLE" || (vm.beneficiary_mode !== "SINGLE" && vm.release_method === "SHARED")
                    }
                    QDbAlertNoteToBeneficiary {
                        note: vm.note
                        noteChanged: vm.is_note_changed
                        beneficiariesData: vm.beneficiariesData
                    }
                    QDbAlertNotificationPreferences {
                        emails: vm.display_emails.length > 0 ? vm.display_emails.split(",") : []
                        emailsChanged: vm.isDisplay_emailsChanged
                        notifyEnabled: vm.notifyEnabled
                        notifyEnabledChanged: vm.isNotifyEnabledChanged
                    }
                }
            }
        }
    }
    bottomRight: QTextButton {
        width: 252
        height: 48
        label.text: STR.STR_QML_1011.arg(vm.pending_signatures).arg(vm.pending_signatures > 1 ? "s":"")
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            vm.onSignDummyTxClicked()
        }
    }
}
