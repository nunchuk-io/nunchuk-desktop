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
import "./../../../customizes/services"
import "./../../../customizes/Chats"
import "./../../../customizes/Texts"
import "./../../../customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Item {
    Loader {
        anchors.fill: parent
        sourceComponent: {
            if (!ServiceSetting.walletInfo) {
                return _select_wallet
            } else {
                return _planing
            }
        }
    }
    Component {
        id: _planing
        QServiceViewInheritancePlaningDetail {
        }
    }
    Component {
        id: _select_wallet
        QSelectAnAssistedWallet {
            header: STR.STR_QML_875
            wallets: ServiceSetting.servicesTag.listInheritantPlans
            onSelectWalletRequest: {
                var _input = {
                    type: "inheritance-planing",
                    wallet_id: wallet_id
                }
                QMLHandle.sendEvent(EVT.EVT_SERVICE_SELECT_WALLET_REQUEST, _input)
            }
        }
    }

    Connections {
        target: ServiceSetting.servicesTag
        onSecurityQuestionChanged: {
            if (ServiceSetting.optionIndex === _VIEW_INHERITANCE_PLANING) {
                _Security.action = function() {
                    var _input = {
                        option : "security-question"
                    }
                    QMLHandle.sendEvent(EVT.EVT_INHERITANCE_PLAN_FINALIZE_REQUEST, _input)
                }
                _Security.open();
            }
        }
        onInheritanceDiscardChangeAlert: {
            if (ServiceSetting.optionIndex === _VIEW_INHERITANCE_PLANING) {
                _confirm.title = STR.STR_QML_024
                _confirm.contentText = STR.STR_QML_919
                _confirm.open();
                _confirm.fClose = function() {
                    ServiceSetting.clearWalletInfo()
                }
            }
        }
    }

}
