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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/services"
import "../../Components/customizes/Popups"
import "../../Components"
import "../../Components/LeftPannel"
import "../../Components/RightPannel"
import "../../Components/RightPannel/Service/Common"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: rootSettings
    readonly property int _EMERGENCY: 0
    readonly property int _EMERGENCY_LOCKDOWN: 1
    readonly property int _KEY_RECOVERY: 2
    readonly property int _INHERITANCE_PLANING: 3
    readonly property int _VIEW_INHERITANCE_PLANING: 4
    readonly property int _CLAIM_AN_INHERITANCE: 5
    readonly property int _YOUR_SUBSCRIPTION: 6
    readonly property int _PLATFORM_KEY_CO_SIGNING_POLICIES: 7
    readonly property int _GET_ADDITIONAL_WALLETS: 8
    readonly property int _ORDER_NEW_HARDWARE: 9
    readonly property int _REPLACE_KEY_IN_AN_ASSISTED_WALLET: 10
    readonly property int _MANAGE_SUBSCRIPTION: 11
    readonly property int _ROLL_OVER_TO_A_NEW_ASSISTED_WALLET: 12
    function closeRequest(to) {
        return true;
    }

    Row {
        anchors.fill: parent
        QLeftPannel {
            id: pannel_left
            left_pannel: QServiceLeftPannel {}
        }
        QRightPannel {
            width: parent.width - pannel_left.width
            right_pannel: QServiceRightPannel {}
        }
    }

    QPopupInfoTwoButtons {
        id: _InfoVer
        property string link:""
        property bool linkTop: false
        title: STR.STR_QML_339
        contentText: STR.STR_QML_684
        labels: [STR.STR_QML_341,STR.STR_QML_683]
        funcs: [
            function() {
                if (linkTop)
                    Qt.openUrlExternally(link)
            },
            function() {
                if (!linkTop)
                    Qt.openUrlExternally(link)
            }
        ]

        function orderNewHardware() {
            _InfoVer.link = "https://nunchuk.io/hardware-replacement";
            _InfoVer.contentText = STR.STR_QML_735;
            _InfoVer.labels = [STR.STR_QML_341,STR.STR_QML_683];
            _InfoVer.open();
            return false;
        }

        function getAdditionalWallets() {
            ServiceSetting.servicesTag.additionalGetWalletConfig()
            _InfoVer.link = "https://nunchuk.io/my-plan";
            var remainCount = ServiceSetting.servicesTag.walletConfig.remaining_wallet_count !== undefined ? ServiceSetting.servicesTag.walletConfig.remaining_wallet_count : 0;
            _InfoVer.contentText = (remainCount === 1 ? STR.STR_QML_842.arg(remainCount) : STR.STR_QML_841.arg(remainCount));
            _InfoVer.labels = [STR.STR_QML_341,STR.STR_QML_683];
            _InfoVer.open();
            return false;
        }

        function rollOverNewAssistedWallet() {
            _InfoVer.link = "https://nunchuk.io/hardware-replacement";
            _InfoVer.contentText = STR.STR_QML_735;
            _InfoVer.labels = [STR.STR_QML_341,STR.STR_QML_683];
            _InfoVer.open();
            return false;
        }

        function manageSubscription() {
            _InfoVer.link = "https://nunchuk.io/my-plan";
            _InfoVer.contentText = STR.STR_QML_684;
            _InfoVer.labels = [STR.STR_QML_341,STR.STR_QML_683];
            _InfoVer.open();
            return false;
        }

        function notPaidAlert() {
            _InfoVer.link = "https://nunchuk.io/individuals"
            _InfoVer.linkTop = true
            _InfoVer.contentText = STR.STR_QML_771
            _InfoVer.labels = [STR.STR_QML_772,STR.STR_QML_341]
            _InfoVer.open();
        }

        function isExpiredAlert() {
            _InfoVer.link = "https://nunchuk.io/claim"
            _InfoVer.linkTop = true
            _InfoVer.contentText = STR.STR_QML_750
            _InfoVer.labels = [STR.STR_QML_751,STR.STR_QML_079]
            _InfoVer.open();
        }

        function securityDepositRequiredAlert(errormsg) {
            _InfoVer.link = "https://nunchuk.io/claim"
            _InfoVer.linkTop = true
            _InfoVer.title = STR.STR_QML_1823
            _InfoVer.contentText = errormsg
            _InfoVer.labels = [STR.STR_QML_1825,STR.STR_QML_341]
            _InfoVer.open();
        }
    }

    QPopupInfo{
        id:_info1
        contentText: STR.STR_QML_752
        function replace_keys() {
            _info1.title = STR.STR_QML_339
            _info1.contentText = STR.STR_QML_1347
            _info1.open()
            return true;
        }
        function thereNoWalletAssitedAlert() {
            _info1.title = STR.STR_QML_339
            _info1.contentText = STR.STR_QML_839
            _info1.open()
        }
        function securityCoSigningDelayAlert() {
            _info1.contentText = STR.STR_QML_1344
            _info1.title = STR.STR_QML_339
            _info1.open()
        }
        function comeBackLaterAlert(errormsg) {
            _info1.title = STR.STR_QML_1821
            _info1.contentText = errormsg
            _info1.open()
        }        
    }
    
    
    QConfirmYesNoPopup{
        id:_confirm
        property var fClose
        contentText: STR.STR_QML_919
        onConfirmNo: close()
        onConfirmYes: {
            close()
            fClose()
        }
    }
    QPopupAnswerSecurityQuestion {
        id: _Security
    }

    QPopupTheBackupPassword {
        id: _BackupPassword
    }
}
