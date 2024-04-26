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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: ""
        onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        Column {
            anchors.fill: parent
            anchors.margins: 36
            spacing: 24
            Rectangle {
                width: 96;height: 96;
                radius: 48
                color: "#A7F0BA"
                QImage {
                    anchors.centerIn: parent
                    width: 60; height: 60;
                    source: "qrc:/Images/Images/check-dark.svg"
                }
            }
            QLato {
                width: parent.width
                height: 40
                text: {
                    var user = ClientController.user
                    return (user.isByzantineUser || user.isFinneyUser) ? STR.STR_QML_1029.arg(ServiceSetting.walletInfo.walletName) : STR.STR_QML_720
                }
                font.pixelSize: 32
                font.weight: Font.DemiBold
                verticalAlignment: Text.AlignVCenter
            }
            QLato {
                width: parent.width
                height: 28
                text: STR.STR_QML_721.arg(ServiceSetting.servicesTag.untilTime) //STR_QML_1029
                verticalAlignment: Text.AlignVCenter
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
            }
        }
        bottomRight: Row {
            spacing: 12
            QTextButton {
                width: 73
                height: 48
                label.text: STR.STR_QML_341
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    GlobalData.listFocusing = 1
                    QMLHandle.sendEvent(EVT.EVT_LOCKDOWN_SUCCESS_CLOSE_REQUEST)
                }
            }
        }
    }
}
