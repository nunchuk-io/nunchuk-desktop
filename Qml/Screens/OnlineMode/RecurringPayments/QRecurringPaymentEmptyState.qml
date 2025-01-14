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
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1065
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    content: Item {
        Item {
            anchors{
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 60
            }
            width: 500
            height: 196
            Column {
                anchors.fill: parent
                spacing: 16
                Rectangle {
                    width: 96;height: 96;
                    radius: 48
                    color: "#F5F5F5"
                    anchors.horizontalCenter: parent.horizontalCenter
                    QIcon {
                        iconSize: 60
                        anchors.centerIn: parent
                        source: "qrc:/Images/Images/pending-transaction-dark.svg"
                    }
                }
                QLato {
                    width: 500
                    height: paintedHeight
                    text: STR.STR_QML_1067
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                    lineHeight: 28
                    lineHeightMode: Text.FixedHeight
                }
            }
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    bottomRight: QTextButton {
        width: 196
        height: 48
        label.text: STR.STR_QML_1066
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            var _input = {
                type: "add-recurring-payment",
            }
            QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
        }
    }
}
