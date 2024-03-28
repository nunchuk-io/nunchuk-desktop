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

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1221
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    content: Item {
        Row {
            spacing: 36
            Rectangle {
                width: 346
                height: 512
                radius: 24
                color: "#FDEBD2"
                QImage {
                    width: 260
                    height: 260
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/unassisted-wallet-description.png"
                }
            }
            Item {
                width: 346
                height: 512
                Column {
                    width: parent.width
                    spacing: 24
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_1224
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QTextButton {
                        width: 202
                        height: 48
                        label.text: STR.STR_QML_1225
                        label.font.pixelSize: 16
                        type: eTypeE
                        onButtonClicked: {
                            var _input = {
                                type: "create-a-hot-wallet-now"
                            }
                            QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
                        }
                    }
                }
            }
        }

    }

    onPrevClicked: OnBoarding.state = "onboarding"
    bottomRight: QTextButton {
        width: 187
        height: 48
        label.text: STR.STR_QML_1223
        label.font.pixelSize: 16
        type: eTypeB
        onButtonClicked: {
            var _input = {
                type: "Ill-explore-on-my-own"
            }
            QMLHandle.sendEvent(EVT.EVT_ONBOARDING_ACTION_REQUEST, _input)
        }
    }
}
