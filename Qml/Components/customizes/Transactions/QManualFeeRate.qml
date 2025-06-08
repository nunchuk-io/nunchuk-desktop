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
import HMIEVENTS 1.0
import DataPool 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../../Components/customizes"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Column {
    id: feeroot
    width: parent.width
    spacing: 12
    property alias isChecked: manualFeeRate.checked
    property alias textOutput: feeinput.text
    property bool  validInput: (feeinput.text === "") ? false : isValidNumber(feeinput.text) ? (parseFloat(feeinput.text) >= parseFloat(AppModel.minFee)) : false

    function isValidNumber(input) {
        if (input === null || input === undefined)
            return false;
        let trimmed = input.trim();
        let regex = /^[0-9]+(\.[0-9]+)?$/;
        return regex.test(trimmed);
    }

    QCheckBoxButton {
        id: manualFeeRate
        width: parent.width
        height: 28
        label: STR.STR_QML_228
        fontWeight: Font.Normal
        onButtonClicked: {
            if(!manualFeeRate.checked) {
                if(feeroot.validInput){
                    requestDraftTransaction()
                    feeinput.text = "";
                }
            }
            else{
                feeinput.text = transactionInfo.feeRate
            }
        }
    }
    Column {
        visible: manualFeeRate.checked
        width: parent.width
        spacing: 4
        Row {
            width: parent.width
            height: 48
            spacing: 8
            QTextInputBoxTypeA {
                id: feeinput
                width: 280
                height: 48
                placeholderText: ""
                borderColor: "#DEDEDE"
                backgroundColor: "#FFFFFF"
                leftPadding: 12
                activeFocusOnPress: true
                anchors.verticalCenter: parent.verticalCenter
                showEdit: false
                enabled: true

                onTypingFinished: {
                    if(currentText !== "" && feeroot.validInput) {
                        requestDraftTransaction()
                    }
                }
            }
            QLato {
                width: 52
                height: 16
                text: "sat/vbyte"
                font.pixelSize: 12
                font.weight: Font.Bold
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        QLato {
            id: errorText
            anchors.left: parent.left
            text: STR.STR_QML_230.arg(AppModel.minFee)
            font.pixelSize: 12
            color: "#E02247"
            visible: !feeroot.validInput
        }
        QLato {
            id: scriptPathFee
            color: "#595959"
            font.pixelSize: 12
            text: STR.STR_QML_1709.arg(transactionInfo.scriptPathFeeRate)
            visible: (AppModel.walletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT)
                     && (AppModel.walletInfo.walletTemplate === NUNCHUCKTYPE.DEFAULT)
                     && (AppModel.walletInfo.walletType === NUNCHUCKTYPE.MULTISIG)
        }
        QLato {
            id: cpfptext
            color: "#595959"
            font.pixelSize: 12
            text: STR.STR_QML_836.arg(transactionInfo.packageFeeRate)
            visible: transactionInfo.isCpfp
        }
    }
    Column {
        visible: manualFeeRate.checked
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
