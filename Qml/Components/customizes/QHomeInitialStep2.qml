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
import QtQuick 2.4
import HMIEVENTS 1.0
import "../origins"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    id: homestep1
    width: 976
    height: 910
    color: "#F1FAFE"
    QAddWelcome{
        anchors.fill: parent
        titleWelcome: STR.STR_QML_463
        addKey {
            btnTextLink: STR.STR_QML_021
            titleSuggest: STR.STR_QML_616
            content: STR.STR_QML_617
            height: 152
            icon:"qrc:/Images/Images/wallet.svg"
            onBtnClicked: {
                QMLHandle.sendEvent(EVT.EVT_HOME_ADD_WALLET_REQUEST)
            }
        }
        hotWallet {
            btnTextLink: STR.STR_QML_1255
            titleSuggest: STR.STR_QML_1251
            content: STR.STR_QML_1252
            height: 180
            icon:"qrc:/Images/Images/person-add-24px.svg"
            onBtnClicked: {
                OnBoarding.state = "hotWallet"
                QMLHandle.sendEvent(EVT.EVT_ONBOARDING_REQUEST)
            }
        }
    }
}
