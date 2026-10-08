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
pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QPopupInfoVertical {
    id: _infoPopup
    property string btnLabel: STR.STR_QML_341
    signal gotItClicked()
    property var action

    property string downloadUrl: ""
    property string primaryCTALabel: STR.STR_QML_2254

    buttons: downloadUrl !== "" ? twoButtons : singleButton

    Component {
        id: singleButton

        QTextButton {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 252
            height: 48
            label.text: _infoPopup.btnLabel
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                _infoPopup.close()
                _infoPopup.gotItClicked()
                if (_infoPopup.action) {
                    _infoPopup.action()
                }
            }
        }
    }

    Component {
        id: twoButtons

        Column {
            spacing: 12

            QTextButton {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 252
                height: 48
                label.text: _infoPopup.btnLabel
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    _infoPopup.close()
                    _infoPopup.gotItClicked()
                    if (_infoPopup.action) {
                        _infoPopup.action()
                    }
                }
            }

            QTextButton {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 252
                height: 48
                label.text: _infoPopup.primaryCTALabel
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    _infoPopup.close()
                    Qt.openUrlExternally(_infoPopup.downloadUrl)
                }
            }
        }
    }
}
