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

QPopup {
    id: enterAddress
    width: parent.width
    height: parent.height
    signal confirmSave()
    signal confirmCancel()
    offset: 0
    signal enterServerAddress(var address)
    function clearText() {
        enterAddress.itemInfo.clearText()
    }
    content: Item {
        id: boxmask
        width: 300
        height: 212
        function clearText() {
            addressInput.textInputted = ""
        }
        Column {
            spacing: 24
            anchors.fill: parent
            anchors.margins: 24
            QLato {
                anchors.left: parent.left
                font.weight: Font.Bold
                text: STR.STR_QML_1299
            }

            Item {
                width: 252
                height: 48
                QTextInputBoxTypeB {
                    id: addressInput
                    label: ""
                    boxWidth: 252
                    boxHeight: 48
                    onTextInputtedChanged: {
                        if(!addressInput.isValid){
                            addressInput.isValid = true
                        }
                        addressInput.showError = false;
                    }
                }
            }
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 12
                QTextButton {
                    width: 120
                    height: 48
                    label.text: STR.STR_QML_035
                    label.font.pixelSize: 16
                    type: eTypeF
                    onButtonClicked: {
                        enterAddress.close()
                    }
                }
                QTextButton {
                    width: 120
                    height: 48
                    label.text: STR.STR_QML_835
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        enterServerAddress(addressInput.textInputted)
                        enterAddress.close()
                    }
                }
            }
        }
    }
}
