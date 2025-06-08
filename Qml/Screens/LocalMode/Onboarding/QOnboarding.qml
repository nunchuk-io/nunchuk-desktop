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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: ""
    extraHeader: Item {}
    onCloseClicked: {
        AppSetting.isFirstTimeOnboarding = true
        closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    }
    content: Item {
        Column {
            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: 24
            }
            spacing: 24
            QLato {
                id: title
                text: STR.STR_QML_1218
                font.pixelSize: 32
                font.weight: Font.Normal
                anchors.horizontalCenter: parent.horizontalCenter
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Column {
                spacing: 16
                QOnboardingWallet {
                    width: 500
                    height: 196
                    title: STR.STR_QML_1219
                    description: STR.STR_QML_1220
                    desImg: "qrc:/Images/Images/onboarding-assisted-wallet.png"
                    color: "#D0E2FF"
                    onBoardingclicked: {
                        AppSetting.isFirstTimeOnboarding = true
                        OnBoarding.screenFlow = "assistedWallet"
                    }
                }
                QOnboardingWallet {
                    width: 500
                    height: 168
                    title: STR.STR_QML_1221
                    description: STR.STR_QML_1222
                    desImg: "qrc:/Images/Images/onboarding-unassisted-wallet.png"
                    color: "#FDEBD2"
                    onBoardingclicked: {
                        AppSetting.isFirstTimeOnboarding = true
                        OnBoarding.screenFlow = "unassistedWallet"
                    }
                }
            }
        }
    }
    bottomLeft: Item {}
    bottomRight: Item {}
}
