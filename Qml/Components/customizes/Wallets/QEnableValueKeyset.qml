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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Lists"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id: _contentBip
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1712
    property var newWalletInfo: AppModel.newWalletInfo
    property string enableValueKeyset: "enable-value-keyset"
    property var listEnable: [
        { id: "enable-value-keyset"  , label: STR.STR_QML_1701 },
        { id: "disable-value-keyset" , label: STR.STR_QML_1702 },
    ]
    content: Item {
        Column {
            id:_colum
            width: parent.width
            spacing: 24
            QLato {
                width: 539
                height: 84
                anchors.left: parent.left
                text: STR.STR_QML_1700
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            QRadioList {
                width: 350
                height: listEnable.length * 48
                model: listEnable
                spacing: 16
                delegateType: _RadioTypeB
                function radioSelect() {
                    return enableValueKeyset
                }
                function setRadioSelect(id, data) {
                    enableValueKeyset = id
                }
            }
        }
        QWarningBgMulti {
            width: 728
            height: 80
            iSize: 36
            icon: "qrc:/Images/Images/info-60px.svg"
            txt.text: STR.STR_QML_1699
            anchors.bottom: parent.bottom
        }
    }
}
