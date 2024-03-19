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
import QtQuick.Controls 2.5
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: _signInfo
    width: 350
    height: 492
    property var masterSignerInfo: AppModel.masterSignerInfo
    property var singleSignerInfo: AppModel.singleSignerInfo

    Column {
        anchors.fill: parent
        spacing: 8
        Column {
            id: _col
            spacing: 8
            QLabelTextInput {
                id: _signerName
                label: STR.STR_QML_102
                boxWidth: 350
                boxHeight: 48
                textInputted: signerName
                anchors.horizontalCenter: parent.horizontalCenter
                onTypingFinished: {
                    if(currentText !== signerName && currentText !== ""){
                        requestRename(currentText)
                    }
                }
            }
            QLabelTextInput {
                id: _signerSpec
                label: STR.STR_QML_127
                boxWidth: 350
                boxHeight: 28
                readOnly: true
                textInputted: signerSpec
                visible: signerSpec !== ""
                input.font.capitalization: Font.AllUppercase
                anchors.horizontalCenter: parent.horizontalCenter
            }
            QLabelTextInput {
                id: derpath
                label: STR.STR_QML_111
                boxWidth: 350
                boxHeight: 28
                readOnly: true
                textInputted: singleSignerInfo.signerDerivationPath
                visible: singleSignerInfo.signerDerivationPath !== ""
                anchors.horizontalCenter: parent.horizontalCenter
            }
            QLabelTextInput {
                id: card_id
                label: STR.STR_QML_1203
                boxWidth: 350
                boxHeight: 28
                readOnly: true
                textInputted: signerCardId
                visible: signerCardId !== ""
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        QUsedInWallets {
            listHeight: _signInfo.height + 4 - _col.childrenRect.height
        }
    }
}


