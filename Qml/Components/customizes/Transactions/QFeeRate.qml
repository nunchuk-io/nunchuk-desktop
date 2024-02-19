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

Item {
    id: _item
    property var transactionInfo
    property var new_fee: 0
    /*========================================*/
    width: 350
    height: _col.childrenRect.height
    Column {
        id: _col
        spacing: 12
        QLato {
            text: STR.STR_QML_1127
            font.weight: Font.Bold
            font.pixelSize: 16
            color: "#000000"
        }
        Rectangle {
            width: _item.width
            height: 275
            color: "#F5F5F5"
            radius: 12
            Column {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16
                Item {
                    width: 318
                    height: 40
                    Row {
                        anchors.fill: parent
                        spacing: 16
                        QLato {
                            width: 151
                            text: STR.STR_QML_1174
                            font.weight: Font.Normal
                            font.pixelSize: 16
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Column {
                            width: 151
                            spacing: 4
                            QLato {
                                width: 151
                                text: transactionInfo.feeRate + " sat/vB"
                                font.weight: Font.Bold
                                font.pixelSize: 16
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignRight
                            }
                            QLato {
                                width: 151
                                text: transactionInfo.feeBTC + " BTC/vB"
                                font.pixelSize: 12
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }
                }
                Rectangle {
                    width: 318
                    height: 1
                    color: "#EAEAEA"
                }
                Column {
                    width: 318
                    height: 38
                    spacing: 4
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_1175
                        font.weight: Font.Normal
                        font.pixelSize: 16
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_1176
                        font.pixelSize: 12
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        color: "#595959"
                    }
                }
                Row {
                    width: 318
                    height: 40 + (input_fee.showError ? 36 : 0)
                    spacing: 12
                    QTextInputBoxTypeB {
                        id: input_fee
                        label: ""
                        boxWidth: 252
                        boxHeight: 48
                        validator: RegExpValidator {
                            regExp: /^[0-9]+(\.[0-9]{1,3})?$/
                        }
                        textInputted: ""
                        onTextInputtedChanged: {
                            if(!input_fee.isValid){
                                input_fee.isValid = true
                            }
                            new_fee = parseFloat(input_fee.textInputted)
                            input_fee.isValid = (parseFloat(input_fee.textInputted) > parseFloat(transactionInfo.feeRate))
                            input_fee.showError = !input_fee.isValid
                        }
                        showError: false
                        errorText: STR.STR_QML_1183
                    }
                    QLato {
                        width: 52
                        height: 16
                        text: "sat/vB"
                        font.pixelSize: 12
                        font.weight: Font.Bold
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                    }
                }
                Column {
                    spacing: 4
                    QText {
                        color: "#031F2B"
                        text: "Processing speed"
                        font.pixelSize: 16
                        font.family: "Lato"
                        height: 16
                    }
                    Row {
                        spacing: 12
                        Column {
                            QText {
                                width: 100
                                height: 16
                                text: STR.STR_QML_232
                                color: "#839096"
                                font.pixelSize: 12
                                font.family: "Lato"
                            }
                            QText {
                                width: 100
                                height: 16
                                text: AppModel.fastestFee + " sat/vB"
                                color: "#323E4A"
                                font.pixelSize: 12
                                font.family: "Lato"
                            }
                        }
                        Column {
                            QText {
                                width: 100
                                height: 16
                                text: STR.STR_QML_233
                                color: "#839096"
                                font.pixelSize: 12
                                font.family: "Lato"
                            }
                            QText {
                                width: 100
                                height: 16
                                text: AppModel.halfHourFee + " sat/vB"
                                color: "#323E4A"
                                font.pixelSize: 12
                                font.family: "Lato"
                            }
                        }
                        Column {
                            QText {
                                width: 100
                                height: 16
                                text: STR.STR_QML_234
                                color: "#839096"
                                font.pixelSize: 12
                                font.family: "Lato"
                            }
                            QText {
                                width: 100
                                height: 16
                                text: AppModel.hourFee + " sat/vB"
                                color: "#323E4A"
                                font.pixelSize: 12
                                font.family: "Lato"
                            }
                        }
                    }
                }
            }
        }

    }
}
