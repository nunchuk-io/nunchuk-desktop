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
    id: gaplimit
    width: parent.width
    height: parent.height
    signal confirmSave()
    signal confirmCancel()
    content: Item {
        id: boxmask
        width: 300
        height: 250
        Column {
            spacing: 12
            anchors.fill: parent
            anchors.margins: 24
            QText {
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "Lato"
                font.pixelSize: 16
                font.weight: Font.Bold
                text: "Gap limit"
            }
            QText {
                anchors.horizontalCenter: parent.horizontalCenter
                font.family: "Lato"
                font.pixelSize: 16
                text: "Max: 100"
            }
            Item { height: 1; width: parent.width }
            Item {
                width: 252
                height: 78
                QTextInputBoxTypeB {
                    id: inputGapLimit
                    label: ""
                    boxWidth: 252
                    boxHeight: 48
                    validator: IntValidator {bottom: 0;}
                    textInputted: AppModel.walletInfo.gapLimit
                    onTextInputtedChanged: {
                        if(!inputGapLimit.isValid){
                            inputGapLimit.isValid = true
                        }
                        inputGapLimit.showError = false;
                    }
                }
            }
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 12
                QTextButton {
                    width: 120
                    height: 36
                    label.text: STR.STR_QML_035
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        gaplimit.close()
                    }
                }
                QTextButton {
                    width: 120
                    height: 36
                    label.text: STR.STR_QML_835
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        if (parseInt(inputGapLimit.textInputted) > 100) {
                            inputGapLimit.errorText = STR.STR_QML_834
                            inputGapLimit.isValid = false
                            inputGapLimit.showError = true;
                        } else {
                            gaplimit.close()
                            _warning.warningType = EWARNING.SUCCESS_MSG
                            _warning.warningExplain = STR.STR_QML_833
                            _warning.open()
                            QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_GAP_LIMIT_REQUEST,inputGapLimit.textInputted)
                        }
                    }
                }
            }
        }
    }


}
