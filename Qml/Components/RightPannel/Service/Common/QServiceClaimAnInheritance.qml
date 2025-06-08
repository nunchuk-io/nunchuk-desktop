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
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Chats"
import "./../../../customizes/Texts"
import "./../../../customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Item {
    property var inheritanceInfo: ServiceSetting.servicesTag.inheritanceInfo
    property var inheritanceCheckStatus: ServiceSetting.servicesTag.inheritanceCheckStatus
    property var inheritanceClaimPlan: ServiceSetting.servicesTag.inheritanceClaimPlan
    property var buffer_period_countdown: inheritanceClaimPlan.buffer_period_countdown
    property var inheritance: inheritanceClaimPlan.inheritance

    Loader {
        anchors.fill: parent
        sourceComponent: {
            var is_valid = inheritanceCheckStatus.is_valid
            var is_paid = inheritanceCheckStatus.is_paid
            var is_expired = inheritanceCheckStatus.is_expired
            
            if (buffer_period_countdown) {
                return _buffer_period_has_started
            }
            if (inheritance) {
                return your_inheritance()
            }
            if (is_valid || !is_paid || !inheritanceCheckStatus) {
                return _magic_phrase_and_backup_password
            }
        }
    }

    function your_inheritance() {
        var isWithdrawBitcoin = inheritanceCheckStatus.isWithdrawBitcoin
        if (isWithdrawBitcoin) {
            return _your_inheritance_withdraw_bitcoin
        } else {
            return _your_inheritance_congratulations
        }
    }

    Connections {
        target: ServiceSetting.servicesTag
        onNotPaidAlert: {
            _InfoVer.link = "https://nunchuk.io/claim"
            _InfoVer.linkTop = true
            _InfoVer.contentText = STR.STR_QML_771
            _InfoVer.labels = [STR.STR_QML_772,STR.STR_QML_341]
            _InfoVer.open();
        }
        onIsExpiredAlert: {
            _InfoVer.link = "https://nunchuk.io/claim"
            _InfoVer.linkTop = true
            _InfoVer.contentText = STR.STR_QML_750
            _InfoVer.labels = [STR.STR_QML_751,STR.STR_QML_079]
            _InfoVer.open();
        }
        onHasNotBeenActivatedYetAlert: {
            _info1.open()
        }
    }
    Component {
        id: _magic_phrase_and_backup_password
        QServiceClaimInheritanceMagicPhraseAndBackupPassword {
        }
    }
    Component {
        id: _your_inheritance_congratulations
        QServiceClaimInheritanceYourInheritance {
        }
    }
    Component {
        id: _your_inheritance_withdraw_bitcoin
        QServiceClaimInheritanceWithdrawBitcoin {
        }
    }
    Component {
        id: _buffer_period_has_started
        QServiceClaimInheritanceBufferPeriodHasStarted {
        }
    }
}
