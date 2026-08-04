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
import "../../origins"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QPopupInfoVertical {
    id: _infoPopup
    property string btnLabel: STR.STR_QML_341
    signal gotItClicked()
    property var action

    // When set (non-empty), the popup shows two buttons instead of the
    // single one: button 1 uses btnLabel (server's "btnCTA", just closes)
    // and button 2 uses primaryCTALabel (server's "primaryCTA", opens
    // downloadUrl externally). Used by the check-for-update popup so the
    // user can go straight to the download/learn-more page instead of only
    // being able to dismiss the notice. Left empty ("") for every other
    // existing usage of this popup, which keeps their current
    // single-button behavior unchanged.
    property string downloadUrl: ""
    // Label for the second button in the two-button case (server's
    // "primaryCTA"). Falls back to "Learn more" if the backend doesn't send
    // one, same fallback pattern as btnLabel/"Got it" above.
    property string primaryCTALabel: STR.STR_QML_2226

    buttons: downloadUrl !== "" ? _twoButtons : _singleButton

    Component {
        id: _singleButton
        QTextButton {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 252
            height: 48
            label.text: btnLabel
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                _infoPopup.close()
                gotItClicked()
                if(action)
                    action()
            }
        }
    }

    Component {
        id: _twoButtons
        Column {
            spacing: 12
            QTextButton {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 252
                height: 48
                label.text: btnLabel
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    _infoPopup.close()
                    gotItClicked()
                    if(action)
                        action()
                }
            }
            QTextButton {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 252
                height: 48
                label.text: primaryCTALabel
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    _infoPopup.close()
                    Qt.openUrlExternally(downloadUrl)
                }
            }
        }
    }
}
