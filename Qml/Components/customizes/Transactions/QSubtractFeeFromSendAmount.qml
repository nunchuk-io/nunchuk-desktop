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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _item
    width: parent.width
    height: 28
    property bool checked: transactionInfo.subtractFromFeeAmount
    QLato {
        z:1
        id: textsubtractfee
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        text: STR.STR_QML_226
        QTooltip {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.right
            anchors.leftMargin: 8
            toolTip: STR.STR_QML_227
            source: "qrc:/Images/Images/OnlineMode/help_outline_24px.svg"
        }
    }
    QCheckBox {
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
        }
        checked: _item.checked
        partiallyChecked: false
        onCheckboxClicked: {
            _item.checked = !_item.checked
            buttonClicked()
        }
    }

    signal buttonClicked()
}
